/*
 * penalite.c: calcule la table des penalites qui sera envoyee au
 * module appari.c, et realise les appariements automatiques.
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "global.h"
#include "joueur.h"
#include "appari.h"
#include "couplage.h"

static long nbj, nbje, bip, min_score, *tbl, *tbli;
static pen_t **m;
static long optimisation_chromatique (long n1, long n2);
static void appariement_optimal (void);

/*
 * La fonction calcul_appariements() se contente de tester si on peut
 * faire un appriement toutes-rondes, et sinon initialise les tableaux
 * necessaires a la fonction appariement_optimal().
 */

void calcul_appariements (void) {
    long i, n, x, y;

    eff_ecran();
    if ((nbj = nbr_unpaired_players()) < 2)
        goto affiche;

    /* Peut-on faire un toutes-rondes ? */
    if (appariement_ttrondes() == 0) {
    /*
     * On note 'nbje' le nombre de joueurs effectifs (i.e. incluant Bip)
     * et 'bip' contient l'index de Bip, ou bien 0 si Bip n'existe pas.
     *
     * Chaque joueur est alors repere par trois nombres: son ID
     * Elo 'n', son ID d'inscription 'i' et son index 'x'. L'index
     * sert a indicer la matrice des penalites, et varie entre 0 et
     * nbje-1. On construit deux tables tbl et tbli donnant les
     * correspondances:
     *
     *    tbl[x] == n    et    tbli[i] == x.
     *
     * Nous en profitons pour calculer score_min (le score minimum de
     * tous les joueurs presents, pas uniquement ceux qu'on nous
     * demande d'apparier).
     */
        nbje = nbj%2 ? nbj+1 : nbj;
        bip  = nbj%2 ? nbj   : 0;
        assert(nbje%2 == 0);

        /* Construction de la matrice m des penalites */
        CALLOC(m, nbje, pen_t*);
        for (i = 0; i < nbje; i++) {
            CALLOC(m[i], nbje, pen_t);
            for (x = 0; x < nbje; x++)
                m[i][x] = 0;
        }

        /* Table des numeros Elo en fonction de l'index */
        min_score = LONG_MAX;
        CALLOC(tbl, nbje, long);
        for (x = i = 0; i < registered_players->n; i++) {
            n = registered_players->list[i]->ID;
            if (present[i] && polarity(n) == 0)
                tbl[x++] = n;
            if (present[i] && score[i] < min_score)
                min_score = score[i];
        }
        assert(x == nbj);
        if (bip)
            tbl[bip] = -1;  /* Afin de bien le distinguer */
        /*
         * A la premiere ronde, nous appliquons une permutation aleatoire
         * sur cette table, afin que l'appariement choisi ne soit
         * pas toujours le meme.
         *
         * Si le symbole MELANGE est defini, nous melangeons toujours les
         * joueurs avant d'optimiser; attention! ceci rend les appariements
         * difficilement reproductibles...
         */
#ifndef MELANGE
        if (current_round == 0)
#endif
        for (x = 0; x < nbj; x++) {
            y = hasard(nbj-x) + x;
            assert(0 <= y && y < nbj);
            n = tbl[x];
            tbl[x] = tbl[y];
            tbl[y] = n;
        }
        /* Table des index en fonction du no d'inscription */
        CALLOC(tbli, registered_players->n, long);
        for (i = 0; i < registered_players->n; i++)
            tbli[i] = -1;
        for (x = 0; x < nbj; x++) {
            i = numero_inscription(tbl[x]);
            tbli[i] = x;
        }
        /*
         * Nous laissons appariement_optimal() faire le reste des
         * initialisations, en particulier le calcul des penalites, puis
         * realiser l'appariement.
         */
        appariement_optimal();
        free(tbl);
        free(tbli);
        for (i = 0; i < nbje; i++)
            free(m[i]);
        free(m);
    }
affiche:
    display_pairings(NULL, 0);
    save_pairings_file();
}

/*
 * Nous ne faisons pas de test de debordement ici, afin que edmonds.c
 * puisse les detecter
 */

#define  AJOUTE_PENALITE(p,x)       (p) += (x)
#define  RETRANCHE_PENALITE(p,x)    (p) -= (x)

#ifdef ENGLISH
# define PENA_ANNOUNCE  "Penalties:"
# define PENA_REPLAY    " replay"
# define PENA_COLOURS   " colours"
# define PENA_FLOAT     " float"
# define PENA_COUNTRY   " chauvinism"
# define PENA_ELITISM   " elitism"
# define OPTIMAL_PAIR   "Looking for an optimal pairing..."
# define HIT_ANY_KEY    "Hit any key..."
# define BYE_NAME       "Bye"
#else
# define PENA_ANNOUNCE  "Penalites:"
# define PENA_REPLAY    " repetition"
# define PENA_COLOURS   " couleurs"
# define PENA_FLOAT     " flottement"
# define PENA_COUNTRY   " chauvinisme"
# define PENA_ELITISM   " elitisme"
# define OPTIMAL_PAIR   "Recherche d'un appariement optimal..."
# define HIT_ANY_KEY    "Pressez une touche..."
# define BYE_NAME       "Bip"
#endif

static void appariement_optimal (void) {
    long i, j, n, n1, n2, ronde0, n1_est_noir;
    long *v, *ecart_couleur, *dern_couleur ;
    discs_t nb_pions_n1;
    pen_t resultat;
#ifdef COMPENSATION_FLOATS
    long **gain_partie;
    long *gain_partie_buff;
    long gp_rows, gp_columns;
#endif

    /*
     * Allocation des tableaux et initialisation
     */
    CALLOC(ecart_couleur, registered_players->n, long);
    CALLOC(dern_couleur, registered_players->n, long);
    for (i = 0; i < registered_players->n; i++)
    ecart_couleur[i] = dern_couleur[i] = 0;
    CALLOC(v, nbje, long);



#ifdef COMPENSATION_FLOATS
     /*
     * v = gain_partie[i,r] dit si le joueur i
     * a gagne (v == 2), fait nulle (v == 1) ou perdu (v == 0)
     * a la ronde r.
     */

    gp_rows = 1 + registered_players->n;
    gp_columns = 1 + current_round;

    gain_partie = (long**)malloc((unsigned) gp_rows * sizeof(long*));
    gain_partie_buff = (long*)malloc((unsigned) gp_rows * gp_columns * sizeof(long));

    for (i = 0; i < registered_players->n; i++)
         gain_partie[i] = &gain_partie_buff[i*gp_columns];

    for (i = 0; i < registered_players->n; i++)
        for (j = 0; j < current_round; j++)
            gain_partie[i][j] = 0;
#endif


    printf(PENA_ANNOUNCE);
    fflush(stdout);

    /*
     * Penalites de repetition, tout d'abord. Nous faisons une boucle sur
     * les appariements des rondes precedentes, en ajoutant a chaque fois
     * leur contribution a la matrice courante de penalite.
     *
     * Nous en profitons pour compter le nombre de fois ou les joueurs jouent
     * blanc ou noir -- plus precisement nous comptons simplement l'ecart
     * (nb de parties avec Noir) - (nb de parties avec Blanc).
     *
     * Nous en profitons egalement pour remplir la table des gains des
     * joueurs dans les rondes passees.
     */
    printf(PENA_REPLAY);
    fflush(stdout);
    for (ronde0 = 0; ronde0 < current_round; ronde0++) {
        long k1, k2;
        round_iterate(ronde0);
        while (next_couple(&n1, &n2, &nb_pions_n1)) {
            n1 = numero_inscription(n1); assert(n1 >= 0);
            n2 = numero_inscription(n2); assert(n2 >= 0);

            {
            /* Petit squat des "couleurs" */
            ++ecart_couleur[n1];        /* Noir = +1 */
            --ecart_couleur[n2];        /* Blanc= -1 */
            if (ronde0 == current_round-1)
                dern_couleur[n1] = 1, dern_couleur[n2] = 2;
            /* fin du squat! */
            }

#ifdef COMPENSATION_FLOATS
            {
            /* remplissage de l'historique des gains */
            if (IS_VICTORY(nb_pions_n1))
                gain_partie[n1][ronde0] = 2;
            else /*gain noir*/
            if (IS_DEFEAT(nb_pions_n1))
                gain_partie[n2][ronde0] = 2;
            else { /*gain blanc*/
                gain_partie[n1][ronde0] = 1;
                gain_partie[n2][ronde0] = 1 ;
            }  /*nulle*/
            /* fin du squat! */
            }
#endif

            k1 = tbli[n1];
            k2 = tbli[n2];
            if (k1 >= 0 && k2 >= 0) {
                assert(k1 != k2);
                AJOUTE_PENALITE(m[k1][k2], penalite_mcoul);
                AJOUTE_PENALITE(m[k2][k1], penalite_copp);
                if (ronde0 == current_round-1) {
                    AJOUTE_PENALITE(m[k1][k2], penalite_desuite);
                    AJOUTE_PENALITE(m[k2][k1], penalite_desuite);
                }
            }
        }
        /* Et les joueurs qui jouent contre Bip ? */
        if (bip)
            for (i=0; i<nbj; i++)
                if (polar2(tbl[i],ronde0) == 0) {
                    AJOUTE_PENALITE(m[i][bip], penalite_bipbip);
                    AJOUTE_PENALITE(m[bip][i], penalite_bipbip);
                    if (ronde0 == current_round-1) {
                        AJOUTE_PENALITE(m[i][bip], penalite_desuite);
                        AJOUTE_PENALITE(m[bip][i], penalite_desuite);
                    }
                }
    }

    /*
     * Penalites de couleur, ensuite. L'ecart actuel (noir)-(blanc) de
     * chaque joueur se trouvera augmente ou diminue d'une unite selon
     * la couleur jouee a la presente ronde.
     */
    printf(PENA_COLOURS);
    fflush(stdout);
    for (i=0; i<nbj; i++)
        for (j=0; j<nbj; j++)
            if (i != j) {
                long f;

                /* Si i joue les noirs */
                n = numero_inscription(tbl[i]); assert(n >= 0);
                f = abs(ecart_couleur[n] + 1);
                if (f > nmax_couleur - 1)
                    f = nmax_couleur - 1;
                AJOUTE_PENALITE(m[i][j], penalite_couleur[f]);

                /* Avait-il les noirs a la ronde precedente ? */
                if (dern_couleur[n] == 1)
                    AJOUTE_PENALITE(m[i][j], penalite_repcoul);

                /* Si i joue les blancs */
                f = abs(ecart_couleur[n] - 1);
                if (f > nmax_couleur - 1)
                    f = nmax_couleur - 1;
                AJOUTE_PENALITE(m[j][i], penalite_couleur[f]);

                /* Avait-il les blancs a la ronde precedente ? */
                if (dern_couleur[n] == 2)
                    AJOUTE_PENALITE(m[j][i], penalite_repcoul);
            }
    /* Et si on joue contre Bip ? */
    if (bip)
        for (i=0; i<nbj; i++) {
            long f;

            n = numero_inscription(tbl[i]); assert (n >= 0);
            f = abs(ecart_couleur[n]);
            if (f > nmax_couleur - 1)
                f = nmax_couleur - 1;
            AJOUTE_PENALITE(m[i][bip], penalite_couleur[f]);
            AJOUTE_PENALITE(m[bip][i], penalite_couleur[f]);
        }

    /*
     * Penalites de flottement, ici. Plutot que d'ajouter directement les
     * contributions a la matrice m[][], nous stockons la penalite de
     * flottement dans 'pflot' et nous ne l'ajoutons a m[][] qu'a la fin.
     */
    printf(PENA_FLOAT);
    fflush(stdout);
#ifdef ELITISM
    printf(PENA_ELITISM);
    fflush(stdout);
#endif
    for (i=0; i<nbje; i++)
        for (j=0; j<nbje; j++)
            if (i < j) {
                long sci, scj, f, af;
                pen_t pflot;

                n1 = tbl[i]; if (n1>=0) n1 = numero_inscription(n1);
                n2 = tbl[j]; if (n2>=0) n2 = numero_inscription(n2);
                sci = n1>=0 ? score[n1] : min_score-1;
                scj = n2>=0 ? score[n2] : min_score-1;
                /* Le flottement de i sur j vaut */
                f = sci-scj;
                /* La penalite principale de flottement vaut */
                af = abs(f);
                if (af > nmax_flottement - 1)
                    af = nmax_flottement - 1;
                pflot = penalite_flottement[af];
                /* Ajouter les penalites de flottement cumule */
                if (n1 >= 0 && f * last_float[n1] > 0)
                    AJOUTE_PENALITE(pflot, penalite_flcum);
                if (n2 >= 0 && f * last_float[n2] < 0)
                    AJOUTE_PENALITE(pflot, penalite_flcum);


#if (defined(COMPENSATION_FLOATS) )
                /*
                 * Retrancher les minorations (flottement anti-cumule)
                 * on ne les retranche que si un joueur a flotte haut a la ronde
                 * precedente et perdu (auquel cas il doit flotter bas) ou si
                 * il a flotte bas et gagne (auquel cas il doit flotter haut)
                 */
                if (current_round > 1) {
                    if (( n1 >= 0   &&   f * last_float[n1] < 0) &&
                      ((last_float[n1] < 0  &&  gain_partie[n1][current_round-1] == 0 ) ||
                      (last_float[n1] > 0  &&  gain_partie[n1][current_round-1] == 2 ) ))
                        RETRANCHE_PENALITE(pflot, minoration_fac);
                    if (( n2 >= 0   &&   f * last_float[n2] > 0) &&
                      ((last_float[n2] < 0  &&  gain_partie[n2][current_round-1] == 0 ) ||
                      (last_float[n2] > 0  &&  gain_partie[n2][current_round-1] == 2 ) ))
                        RETRANCHE_PENALITE(pflot, minoration_fac);
                }
#else

                /* l'ancien code de Thierry :
                 *
                 * Retrancher les minorations (flottement anti-cumule)
                 */

                if (n1 >= 0 && f * last_float[n1] < 0)
                    RETRANCHE_PENALITE(pflot, minoration_fac);
                if (n2 >= 0 && f * last_float[n2] > 0)
                    RETRANCHE_PENALITE(pflot, minoration_fac);

#endif


                /*
                 * Ajouter la penalite d'elitisme
                 */
#ifdef ELITISM
                if ((penalite_elitisme[current_round] != 0) && (f != 0) &&
                  (sci >= 0) && (scj >= 0)) {
                    pen_t  valeur_elitisme;

                    valeur_elitisme = (penalite_elitisme[current_round] * (sci+scj) * abs(f)) / 2;
                    assert( valeur_elitisme >= 0); /* pas de gag d'overflow ! */
                    AJOUTE_PENALITE(pflot, valeur_elitisme);
                }
#endif

                /* La penalite de flottement est maintenant dans pflot */
                assert(pflot >= 0);
                AJOUTE_PENALITE(m[i][j], pflot);
                AJOUTE_PENALITE(m[j][i], pflot);
            }
    /*
     * Penalites de chauvinisme
     */
    printf(PENA_COUNTRY);
    fflush(stdout);
    for (i = 0; i < nbj; i++)
        for (j = 0; j < nbj; j++)
            if (i < j) {
                Player *j1, *j2;

                j1 = trouver_joueur(tbl[i]);
                j2 = trouver_joueur(tbl[j]);
                if (!compare_chaines_non_sentitif(j1->country, j2->country)) {
                    AJOUTE_PENALITE(m[i][j], penalite_chauvinisme[current_round]);
                    AJOUTE_PENALITE(m[j][i], penalite_chauvinisme[current_round]);
                }
            }
    /*
     * Le calcul des penalites est termine, nous pouvons maintenant
     * appeler la fonction d'optimisation
     */
    printf(".\n");
    fflush(stdout);

    printf(OPTIMAL_PAIR "\n");
    resultat = apparie(nbje/2, m ,v);
    /* Apparier seulement les vrais joueurs */
#ifdef DEBUG_PEN
    printf("\n");
#endif
    for (i=0; i<nbje; i+=2) {
        n1 = tbl[v[i]];
        n2 = tbl[v[i+1]];
        n1_est_noir = 1;
        if (n1 >= 0 && n2 >= 0) {
            assert(numero_inscription(n1) >= 0);
            assert(numero_inscription(n2) >= 0);
            /* Derniere phase: optimisation chromatique */
            if (m[v[i]][v[i+1]] != m[v[i+1]][v[i]]) {
                make_couple(n1, n2, UNKNOWN_SCORE);
                n1_est_noir = 1;
            } else {
                long f = optimisation_chromatique(n1,n2);
                if (f == 1) {
                    make_couple(n1, n2, UNKNOWN_SCORE);
                    n1_est_noir = 1;
                } else if (f == 2) {
                    make_couple(n2, n1, UNKNOWN_SCORE);
                    n1_est_noir = 0;
                } else {
                /* Les couleurs sont indifferentes */
                    if (hasard(2)) {
                        make_couple(n1, n2, UNKNOWN_SCORE);
                        n1_est_noir = 1;
                    } else {
                        make_couple(n2, n1, UNKNOWN_SCORE);
                        n1_est_noir = 0;
                    }
                }
            }
        }

#ifdef DEBUG_PEN
        if (!n1_est_noir) {
            n = n1;
            n1 = n2;
            n2 = n;
        }
        printf("%-8.8s(%6ld)   %-8.8s(%6ld)%10ld\n",
           ((n1>=0) ? trouver_joueur(n1)->fullname : BYE_NAME),
           n1,
           ((n2>=0) ? trouver_joueur(n2)->fullname : BYE_NAME),
           n2,
           (long) m[v[i]][v[i+1]]);
/*  printf("%ld\t%ld\t%8ld\n", n1, n2, (long) m[v[i]][v[i+1]] ); */
#endif

    }
#ifdef DEBUG_PEN
    printf("\nTotal\t\t%10ld\n\n", (long)resultat);
    printf(HIT_ANY_KEY);
    (void)lire_touche();
#endif
    /* Liberation de la memoire */
    free(v);
    free(dern_couleur);
    free(ecart_couleur);

#ifdef COMPENSATION_FLOATS
    free(gain_partie_buff);
    free(gain_partie);
#endif
}



/*
 * optimisation_chromatique(n1,n2): n1 et n2 sont les deux numeros Elo
 * des joueurs a apparier. Cette fonction renvoie 1 s'il vaut mieux que
 * n1 joue les noirs, 2 s'il vaut mieux que n2 joue les noirs, et 0
 * si la couleur est indifferente.
 */

static long optimisation_chromatique (long n1, long n2) {
    long ronde0 = current_round, p1, p2;

    while (--ronde0 >= 0) {
        p1 = polar2(n1, ronde0);
        p2 = polar2(n2, ronde0);
        if (p1 == p2)
            continue;
        /* C'est la ronde la plus recente ou les couleurs sont differentes */
        if (p1 == 1 || p2 == 2)
            return 2;
        if (p1 == 2 || p2 == 1)
            return 1;
        /* Nous ne devrions jamais arriver ici */
        assert(0);
    }
    /* Les joueurs ont toujours eu les memes couleurs */
    return 0;
}
