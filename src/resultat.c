/*
 * resultat.c: entree des resultats
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, ajout dans 'sauver_fichier_classement()' et dans 'sauver_fichier_resultats()'
 *                   du test pour decider de la sauvegarde au format xml.
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 02/02/2007 : 'calcul_departage()' utilise maintenant un tableau
 *                   'tieBreak[]' compose de doubles.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "joueur.h"
#include "couplage.h"
#include "global.h"
#include "more.h"
#include "tiebreak.h"
#include "crosstable.h"

#ifdef ENGLISH
# define ERES_COMPLETE  "THE RESULTS ARE COMPLETE: type `!' to confirm."
# define ERES_PROMPT    "Player & Score: "
# define ERES_RANDOM    "Choosing random results..."
# define ERES_I_PAIR    "Incomplete pairings!"
# define ERES_I_RES     "Incomplete results!"
# define ERES_I_SCORE   "Maximum score is %ld"
# define ERES_I_EVEN    "The relative score should be even\n"
# define ERES_I_ODD     "The relative score should be odd\n"
# define ERES_I_PLAYER  "Player %ld didn't play this round"
# define ERES_I_AGAIN   "This coupon has already been entered\n"
# define ERES_I_CLOBBER "Previous score was %s\n"
# define FICH_HEADER    "Feats of player %s (%ld) after round %ld:"
# define FICH_TRAILER_IMPAIR    "Total of %ld.5 points out of %ld, with %s discs."
# define FICH_TRAILER_PAIR      "Total of %ld points out of %ld, with %s discs."
# define FICH_TRAILER   "Total of %ld half-points out of %ld, with %s discs."
# define FICH_OUT       "The player has left the tournament."
# define CORR_PROMPT    "Correction in which round (%ld-%ld)? "
# define CORR_ROUND     "Round %ld"
# define CORR_BLACK     "Real first player (%s) was:"
# define CORR_WHITE     "Real second player (%s) was:"
# define CORR_SCORE_BLACK    "Real score for %s was: "
# define CORR_SCORE_WHITE    "Real score for %s was: "
# define CORR_POSITIVE  "Number of discs must be positive!"
# define CORR_SOMME     "Sum of discs must be equal to %ld !"
# define CORR_DISCS     "%s discs"
# define CORR_PAS_JOUE  "These players didn't play together at round %ld !"
# define CORR_OK        "Correction accepted (round %ld):"
# define CORR_REGENERATE    "\n\nRegenerate file \"%s\" (Y/N) ? "
# define CORR_ERROR     "CORRECTION IS CANCELED, "
# define CORR_TOUCHE    "hit any key..."
# define MSG_CROSS_MEMORY "Not enough memory to generate the cross-table."
#else
# define ERES_COMPLETE  "RESULTATS COMPLETS: tapez `!' pour confirmer."
# define ERES_PROMPT    "Joueur & Score: "
# define ERES_RANDOM    "Tirage des resultats au hasard..."
# define ERES_I_PAIR    "Appariements incomplets !"
# define ERES_I_RES     "Resultats incomplets !"
# define ERES_I_SCORE   "Le score ne peut depasser %ld"
# define ERES_I_EVEN    "La difference de pions doit etre paire\n"
# define ERES_I_ODD     "La difference de pions doit etre impaire\n"
# define ERES_I_PLAYER  "Le joueur %ld n'a pas joue a cette ronde"
# define ERES_I_AGAIN   "Coupon deja entre\n"
# define ERES_I_CLOBBER "Le precedent score etait %s\n"
# define FICH_HEADER    "Fiche du joueur %s (%ld) apres la ronde %ld:"
# define FICH_TRAILER_IMPAIR    "Soit au total %ld.5 points sur %ld, avec %s pions."
# define FICH_TRAILER_PAIR      "Soit au total %ld points sur %ld, avec %s pions."
# define FICH_TRAILER   "Soit au total %ld demi-points sur %ld, avec %s pions."
# define FICH_OUT       "Le joueur est sorti du tournoi."
# define CORR_PROMPT    "Correction dans quelle ronde (%ld-%ld) ? "
# define CORR_ROUND     "Ronde %ld"
# define CORR_BLACK     "Premier joueur (%s) reel :"
# define CORR_WHITE     "Deuxieme joueur (%s) reel :"
# define CORR_SCORE_BLACK    "Score reel de %s : "
# define CORR_SCORE_WHITE    "Score reel de %s : "
# define CORR_POSITIVE  "Le nombre de pions doit etre positif !"
# define CORR_SOMME     "La somme des pions doit faire %ld !"
# define CORR_DISCS     "%s pions"
# define CORR_PAS_JOUE  "Ces joueurs n'ont pas joue ensemble a la ronde %ld !"
# define CORR_OK        "Correction acceptee (ronde %ld) :"
# define CORR_REGENERATE    "\n\nRegenerer le fichier \"%s\" (O/N) ? "
# define CORR_ERROR     "CORRECTION ANNULEE, "
# define CORR_TOUCHE    "pressez une touche..."
# define MSG_CROSS_MEMORY "Pas assez de memoire pour la cross-table."
#endif

static long resultats_complets(void) {
    long n1, n2;
    discs_t v;

    round_iterate(current_round);
    while (next_couple(&n1, &n2, &v))
        if (COUPON_IS_EMPTY(v)) return 0;
    return 1;
}

/*  sauvegardes dans les fichiers "class###.txt"  */
void sauver_fichier_classement() {
    char    *filename, old_more_mode[10];
    FILE    *fic;
    long     i;

    if (sauvegarde_fichier_classement) {

        filename = nom_fichier_numerote(nom_fichier_classement, current_round);

        assert(nom_fichier_classement != NULL );
        assert(nom_fichier_result != NULL);
        assert(nom_fichier_appariements != NULL);

        /* Si deux noms de fichiers se telescopent, il faut ajouter
            le classement a la fin du fichier avec le mode d'ecriture "append" */
        if ((sauvegarde_fichier_result &&
             (strcmp(nom_fichier_result, nom_fichier_classement) == 0 )) ||
            (sauvegarde_fichier_appariements &&
             (strcmp(nom_fichier_appariements, nom_fichier_classement) == 0 ))) {

            fic = myfopen_dans_sous_dossier(filename,"a", nom_sous_dossier, utiliser_sous_dossier, 1);
            fprintf(fic,"\n\n\n");
            fclose(fic);

            strcpy(old_more_mode,more_get_mode());
            more_set_mode("a");
            affiche_inscrits(filename);
            more_set_mode(old_more_mode);
        } else
            affiche_inscrits(filename);

        if (impression_automatique)
            for (i = 0; i < nb_copies_impression; i++)
                imprime_fichier(filename);
    }

	/* Faut-il generer le fichier XML du classement ? */
	if (generer_fichiers_XML) {
		creer_classement_XML();
	}

}

/*  sauvegardes dans les fichiers "team###.txt"  */
void sauver_fichier_equipes() {
    char    *filename, old_more_mode[10];
    FILE    *fic;
    long     i;

    if (sauvegarde_fichier_class_equipes) {

        filename = nom_fichier_numerote(nom_fichier_class_equipes, current_round);

        assert(nom_fichier_classement != NULL );
        assert(nom_fichier_result != NULL);
        assert(nom_fichier_appariements != NULL);
        assert(nom_fichier_class_equipes != NULL);

        /* Si deux noms de fichiers se telescopent, il faut ajouter
           le classement a la fin du fichier avec le mode d'ecriture "append" */
        if ((sauvegarde_fichier_result &&
            (strcmp(nom_fichier_result, nom_fichier_class_equipes) == 0 )) ||
            (sauvegarde_fichier_appariements &&
            (strcmp(nom_fichier_appariements, nom_fichier_class_equipes) == 0 )) ||
            (sauvegarde_fichier_classement &&
            (strcmp(nom_fichier_classement, nom_fichier_class_equipes) == 0 ))) {

            fic = myfopen_dans_sous_dossier(filename,"a", nom_sous_dossier, utiliser_sous_dossier, 1);
            fprintf(fic,"\n\n\n");
            fclose(fic);

            strcpy(old_more_mode,more_get_mode());
            more_set_mode("a");
            affiche_equipes(filename);
            more_set_mode(old_more_mode);
        } else
          affiche_equipes(filename);

        if (impression_automatique)
            for (i = 0; i < nb_copies_impression; i++)
                imprime_fichier(filename);
    }
}

/*  sauvegardes dans les fichiers "crosstable###.htm"  */
void sauver_fichier_tableau_croise_html() {

    if (sauvegarde_fichier_crosstable_HTML && (current_round >= 1)) {

        if (!peut_allouer_memoire_cross_table()) {
            eff_ecran();
            printf(MSG_CROSS_MEMORY);
            lire_touche();
        } else {
            preparer_calculs_tableau_croise(current_round-1);

            assert(nom_fichier_crosstable_HTML != NULL );
            if (strstr(nom_fichier_crosstable_HTML, "###"))
                sauvegarder_tableau_croise_HTML(nom_fichier_numerote(nom_fichier_crosstable_HTML, current_round));
            else
                sauvegarder_tableau_croise_HTML(nom_fichier_crosstable_HTML);

            liberer_memoire_cross_table();
        }
    }
}

/* affichage des coupons lors de l'entree des resultats :
 * on affiche d'abord les coupons remplis, puis les vides.
 */
void afficher_remplissage_coupons(void) {
    long i, n1, n2;
    Pair *liste;
    long nb_paires;
    discs_t v;

	/* Une ligne "vide" pour faire plaisir à Marc */
	puts("");

    /* D'abord ceux qui sont remplis... */
    CALLOC(liste, 1 + registered_players->n / 2, Pair);
    nb_paires = 0;
    round_iterate(current_round);
    while (next_couple(&n1, &n2, &v))
        if (COUPON_IS_FILLED(v)) {
        i = nb_paires++;
        liste[i].j1 = trouver_joueur(n1);
        liste[i].j2 = trouver_joueur(n2);
        liste[i].score = v;
        liste[i].sort_value = tables_sort_criteria(n1, n2);
    }
    if (current_round >= 1)
        SORT(liste, nb_paires, sizeof(Pair), pairs_sort);
    for (i = 0; i < nb_paires; i++)
        puts(coupon(liste[i].j1, liste[i].j2, liste[i].score));
    free(liste);

    /* ... puis les vides. */
    CALLOC(liste, 1 + registered_players->n / 2, Pair);
    nb_paires = 0;
    round_iterate(current_round);
    while (next_couple(&n1, &n2, &v))
        if (COUPON_IS_EMPTY(v)) {
        i = nb_paires++;
        liste[i].j1 = trouver_joueur(n1);
        liste[i].j2 = trouver_joueur(n2);
        liste[i].score = v;
        liste[i].sort_value = tables_sort_criteria(n1, n2);
    }
    if (current_round >= 1)
        SORT(liste, nb_paires, sizeof(Pair), pairs_sort);
    for (i = 0; i < nb_paires; i++)
        puts(coupon(liste[i].j1, liste[i].j2, liste[i].score));
    free(liste);
}

/* fonction interactive d'entree des resultats */
void entree_resultats() {
    long affiche_message=1, ret, n1, n2, len;
    discs_t v;
    long joueur, relatif ;
    discs_t score;
    char chaine[TAILLE_TAMPON+1], tmp[TAILLE_TAMPON+1];
    Player *j1, *j2;

    /* Y a-t-il des resultats a rentrer, vraiment ? */
    round_iterate(current_round);
    if (next_couple(&n1, &n2, &v) == 0)
        return;
    eff_ecran();
    while (1) {
        if (affiche_message && resultats_complets() && nbr_unpaired_players()<2) {
            /* oui, ils sont complets, et les appariements aussi */
            afficher_remplissage_coupons();
            puts(ERES_COMPLETE);
            affiche_message = 0;
        }
        /* Effacement du tampon d'entree */
        chaine[0] = '\0';
prompt:
        printf(ERES_PROMPT);
        ret = lire_ligne_init(chaine, TAILLE_TAMPON, 1);
        putchar('\n');

        len = strlen(chaine);
        if ((len > 0) && (enleve_espaces_de_gauche(chaine) == len)) {
            /* Que des espaces ! */
            beep();
            goto prompt;
        }

        if (ret < 0 || (ret == 0 && chaine[0] == 0)) {
            /* Quittons ce sous-menu */
            return;
        }
        if (ret > 0) {
            /*
             * L'utilisateur hesite (il a tape sur Tab), affichons-lui
             * la liste triee des coupons.
             */
            afficher_remplissage_coupons();

            /* Et revenir au prompt, sans effacer le tampon */
            goto prompt;
        }

        /* Deux cas particuliers: `!' et `?' */
        if (strcmp(chaine, "?") == 0) {
            puts(ERES_RANDOM);
            imagine_resultats();
            continue;
        }
        if (strcmp(chaine, "!") == 0) {
            if (nbr_unpaired_players() > 1) {
                /* appariements incomplets */
                puts(ERES_I_PAIR);
                beep();
                continue;
            }
            if (!resultats_complets()) {
                /* resultats incomplets */
                puts(ERES_I_RES);
                beep();
                continue;
            }

            /* Tout a l'air bon, hein? Valider les resultats et les sauvegarder*/
            validation_resultats(sauvegarde_fichier_result);
            /* Sauvegarder le nouveau classement individuel et par equipes */
            sauver_fichier_classement();
            sauver_fichier_equipes();
            sauver_fichier_tableau_croise_html();
            /* Afficher le nouveau classement */
            affiche_inscrits(NULL);
            return;
        }
        /*
         * Exemples d'entrees autorisees (tous les scores peuvent
         * etre des flottants):
         *
         * 145 35              le joueur 145 fait 35 pions
         * 145 +6              il gagne de 6 pions
         * 145 -8              il perd de 8 pions
         * 145 =               il fait nulle
         * 145 +0              idem
         * 145 -0              idem
         * 145 =0              idem
         * 145 x               effacer son coupon
         * 145 e               idem
         * -145                idem
         * POIRIER Serge 35    le fullname du joueur en toutes lettres
         * poirier +6          maj/min indifferentes
         * poi -8              la machine complemente automatiquement
         * poi35               et le score peut etre colle
         * poirier x           ne marche pas : la machine croit que x est
         *                                     une initiale de prenom
         * poirier e           non plus
         * -poi                ca, ca marche
         */

        if (chaine[0] == '-') {
            if (nb_completions_dans_coupons(chaine+1,&joueur,tmp) != 1)
                joueur = atoi(chaine+1);
            score = UNKNOWN_SCORE;
            relatif = 0;
        } else {
            if ((nb_completions_dans_coupons(chaine,&joueur,tmp) == 1) ||
                (sscanf(chaine,"%ld%s", &joueur, tmp) == 2)) {
                switch (tmp[0]) {
                  case '\0'  :
                    beep(); continue;
                  case 'x':
                  case 'e':
                    score = UNKNOWN_SCORE; relatif = 0; break;
                  case '=':
                  case '+':
                  case '-':
                  default:
                    if (comprend_score(tmp, &score, &relatif)) break;
                    else { beep(); continue; }
                }
            } else {
             /* Erreur de syntaxe */
                beep();
                continue;
            }
        }

        /* Verifier le score */
        if (SCORE_TOO_LARGE(score) ||
           (relatif && !SCORE_IS_POSITIVE(score))) {
            printf(ERES_I_SCORE "\n", total_pions);
            beep();
            continue;
        }


        /*  on n'autorise pas les scores relatifs impairs.
         *  Ceci empeche de rentrer des demi-pions en relatif...
         */
#if (!defined(USE_HALF_DISCS))

        if (relatif && ((score+total_pions)%2 != 0)) {
            if (total_pions%2) printf(ERES_I_ODD);
            else printf(ERES_I_EVEN);
            beep(); continue;
        }

#endif

        /* Chercher le coupon contenant le joueur */
        round_iterate(current_round);
        while ((ret = next_couple(&n1, &n2, &v)) != 0) {
            if (n1 == joueur)
                break;
            if (n2 == joueur) {
                if (COUPON_IS_FILLED(score))
                    score = OPPONENT_SCORE(score);
                break;
            }
        }
        if (ret == 0) {
            printf(ERES_I_PLAYER "\n", joueur);
            beep();
            continue;
        }
        /* Le coupon contenait-il deja un score? */
        if (COUPON_IS_FILLED(v) && SCORE_IS_POSITIVE(score))
            printf(SCORES_EQUALITY(score,v) ? ERES_I_AGAIN : ERES_I_CLOBBER, fscore(v));

        *couple_value(n1, n2) = score;

        /* Puis afficher le scoupon apres modification */
        j1 = trouver_joueur(n1);
        j2 = trouver_joueur(n2);
        puts(coupon(j1, j2, score));
    }
}

void imagine_resultats() {
    long n1, n2, note1, note2, demi0, demi1;
    discs_t *valc, scn, v;
    double prob_n;

    /* Petite et grande moitie du nombre de pions */
    demi0 = total_pions / 2;
    demi1 = (total_pions + 1) / 2;
    assert(!BAD_TOTAL(INTEGER_TO_SCORE(demi0), INTEGER_TO_SCORE(demi1)));

    round_iterate(current_round);
    while (next_couple(&n1, &n2, &v)) {
        if (COUPON_IS_FILLED(v))
            continue;
        note1 = trouver_joueur(n1)->rating;
        note2 = trouver_joueur(n2)->rating;
        if (note1 < 1100)  note1 = 1100;
        if (note2 < 1100)  note2 = 1100;
        /*
         * La probabilite que Noir gagne vaut
         *
         *            1
         *  -----------------------
         *        (note2-note1)/200
         *  1 + 3
         *
         */
        prob_n = 1.0 / (1.0 + pow(3.0, (note2-note1)/200.0));

        if (hasard(1000)/1000.0 < prob_n)
            scn = INTEGER_TO_SCORE(demi1 + hasard(demi0 + 1));  /* Noir  gagne */
        else
            scn = INTEGER_TO_SCORE(demi0 - hasard(demi0 + 1));  /* Blanc gagne */
        valc = couple_value(n1, n2);
        assert(valc && SCORE_IS_LEGAL(scn));
        *valc = scn;
    }
}

void raz_scores() {
    long i;

    for (i=0; i<MAX_REGISTERED ; i++) {
        present[i] = 0;
        score[i] = 0;
        nbr_discs[i]  = ZERO_DISC;
        tieBreak[i] = 0.0;
        last_float[i] = 0;
        team_score[i] = 0;
    }
}

void mettre_aj_scores() {
    long i, n1, n2, f, pcb, num_elo;
    discs_t v;

    round_iterate(current_round);
    while (next_couple(&n1, &n2, &v)) {
        assert(SCORE_IS_LEGAL(v));
        n1 = numero_inscription(n1);
        n2 = numero_inscription(n2);
        assert(n1 >= 0 && n2 >= 0);
        assert(present[n1] && present[n2]);
        ADD_SCORE(nbr_discs[n1], v);
        ADD_SCORE(nbr_discs[n2], OPPONENT_SCORE(v));
        /* Calcul du flottement de la ronde precedente */
        f = score[n1] - score[n2];
        last_float[n1] = f;
        last_float[n2] = -f;
        /* Calcul des nouveaux scores */
        if (IS_DEFEAT(v))         /* Blanc gagne */
            score[n2] += 2;
        else if (IS_VICTORY(v))   /* Noir gagne  */
            score[n1] += 2;
        else                            /* Match  nul  */
            score[n1]++,score[n2]++;
    }
    /* Mettre a jour ceux qui ont joue contre bip */
    if (IS_DEFEAT(score_bip))
        pcb = 2;
    else if (IS_VICTORY(score_bip))
        pcb = 0;
    else
        pcb = 1;
    for (i=0; i<registered_players->n; i++) {
        num_elo = registered_players->list[i]->ID;
        if (present[i] && polarity(num_elo)==0) {
            ADD_SCORE(nbr_discs[i], OPPONENT_SCORE(score_bip));
            score[i] += pcb;
            last_float[i] = pcb - 1;     /* La valeur exacte importe peu */
        }
    }
}

/*  sauvegardes dans les fichiers "resul###.txt"  */
void sauver_fichier_resultats() {
    char    *filename,  old_more_mode[10];
    FILE    *fic;
    long     i;

    if (sauvegarde_fichier_result)  {
        filename = nom_fichier_numerote(nom_fichier_result,current_round+1);

        /* si les noms de fichiers sont les memes, utiliser le mode d'ecriture "append" */
        if (sauvegarde_fichier_appariements &&
           (strcmp(nom_fichier_appariements, nom_fichier_result) == 0 )) {
            fic = myfopen_dans_sous_dossier(filename,"a", nom_sous_dossier, utiliser_sous_dossier, 1);
            fprintf(fic,"\n\n\n");
            fclose(fic);

            strcpy(old_more_mode,more_get_mode());
            more_set_mode("a");
            display_pairings(filename, 1);
            more_set_mode(old_more_mode);
        } else
            display_pairings(filename, 1);

        if (impression_automatique)
          if (!sauvegarde_fichier_classement ||
               (strcmp(nom_fichier_classement, nom_fichier_result) != 0 ))
              for (i = 0; i < nb_copies_impression; i++)
                  imprime_fichier(filename);
    }

	/* Faut-il generer le fichier XML des resultats ? */
	if (generer_fichiers_XML) {
		creer_ronde_XML();
	}
}

void validation_resultats(long sauve_results_en_clair) {

    /* Creer, si ce n'est deja fait, les numeros de table en memoire
       (mieux vaut tard que jamais !) */
    tables_numbering();
    /* Sauvegarder les resultats de la ronde courante dans le fichier intermediaire */
    sauve_inscrits();
    sauve_ronde();
    /* Sauvegarder les resultats en clair ? */
    if (sauve_results_en_clair)
        sauver_fichier_resultats();

    mettre_aj_scores();
    next_round();
}

/*
 * Correction d'un resultat dans une ronde passee ou dans
 * la ronde presente si on a deja rentre des resultats.
 * On peut intervertir les joueurs et/ou changer le score.
 */
 void correction_resultat() {
    char    chaine[256], *ligne;
    long     ronde_corr, ret, relatif;
    long     noir_corr, blanc_corr;
    discs_t sc_noir_corr, sc_blanc_corr;
    long     min_ronde, max_ronde;


    assert( current_round >= 0 && current_round < NMAX_ROUNDS);
    min_ronde = 0;
    max_ronde = couples_nbr(current_round) ? current_round : current_round - 1;
    if (min_ronde > max_ronde) return;

correction :

    eff_ecran();

    /* entree du ID de la ronde a modifier */
    assert(min_ronde <= max_ronde);
    printf(CORR_PROMPT, min_ronde + 1, max_ronde + 1);
    if (sscanf(lire_ligne(), "%ld", &ronde_corr) != 1)
      return;
    ronde_corr = ronde_corr - 1;

    if ((ronde_corr < min_ronde) || (ronde_corr > max_ronde))
      return;
    assert( ronde_corr >= min_ronde && ronde_corr <= max_ronde);
    printf("\n  " CORR_ROUND, ronde_corr + 1);

    /* entree du bon joueur noir */
    printf("\n");
    sprintf(chaine, CORR_BLACK, couleur_1);
    noir_corr = choix_d_un_joueur_au_clavier(chaine, registered_players, &ligne);
    if (noir_corr <= 0)
        goto abandon;
    printf(" %s\n",coupon(trouver_joueur(noir_corr), NULL, UNKNOWN_SCORE));

    /* entree du bon score noir */
    printf("" CORR_SCORE_BLACK, couleur_1);
    if ( !comprend_score(lire_ligne(), &sc_noir_corr, &relatif) )
        goto abandon;
    if (!SCORE_IS_POSITIVE(sc_noir_corr)) {
        printf("\n\n" CORR_POSITIVE "\n");
        goto abandon;
    }
    if (SCORE_TOO_LARGE(sc_noir_corr)) {
        printf("\n\n" ERES_I_SCORE "\n", total_pions);
        goto abandon;
    }
    printf("\n  " CORR_DISCS, pions_en_chaine(sc_noir_corr));

    /* entree du bon joueur blanc */
    printf("\n");
    sprintf(chaine, CORR_WHITE, couleur_2);
    blanc_corr = choix_d_un_joueur_au_clavier(chaine, registered_players, &ligne);
    if (blanc_corr <= 0) goto abandon;
    printf(" %s\n",coupon(trouver_joueur(blanc_corr), NULL, UNKNOWN_SCORE));

    /* entree du bon score blanc */
    printf("" CORR_SCORE_WHITE, couleur_2);
    if ( !comprend_score(lire_ligne(), &sc_blanc_corr, &relatif) )
        goto abandon;
    if (!SCORE_IS_POSITIVE(sc_blanc_corr)) {
        printf("\n\n" CORR_POSITIVE "\n");
        goto abandon;
    }
    if (SCORE_TOO_LARGE(sc_blanc_corr)) {
        printf("\n\n" ERES_I_SCORE "\n", total_pions);
        goto abandon;
    }
    printf("\n  " CORR_DISCS, pions_en_chaine(sc_blanc_corr));

    /* verification de la somme des pions */
    if (BAD_TOTAL(sc_noir_corr,sc_blanc_corr)) {
        printf("\n\n" CORR_SOMME "\n", total_pions);
        goto abandon;
    }

    assert( !BAD_TOTAL(sc_noir_corr,sc_blanc_corr) );

    switch (change_result(ronde_corr, noir_corr, blanc_corr, sc_noir_corr)) {
    case 1 :
        /* OK, on affiche le coupon corrige */
        printf("\n\n" CORR_OK "\n",ronde_corr + 1);
        puts(coupon(trouver_joueur(noir_corr), trouver_joueur(blanc_corr), sc_noir_corr));
        /* on regenere le fichier intermediaire */
        recreer_fichier_intermediaire();
        /* on le relit immediatement pour recalculer les scores */
            first_round();
        ret = lire_fichier(nom_fichier_inter, CONFIG_F);
        if (ret > 0)
            fatal_error("ERREUR DE FICHIER dans la fonction correction_resultat");

        /* demander si on veut sauver le nouveau classement */
        if ((ronde_corr < current_round) &&
            (sauvegarde_fichier_classement ||
             sauvegarde_fichier_class_equipes ||
             sauvegarde_fichier_crosstable_HTML))   {
            sprintf(chaine, CORR_REGENERATE,
                   nom_fichier_numerote(nom_fichier_classement, current_round));
            if (oui_non(chaine))    {
                sauver_fichier_classement();
                sauver_fichier_equipes();
                sauver_fichier_tableau_croise_html();
                }
            return;
        }

        /* Afficher les fiches individuelles des joueurs concernes */
        /* fiche_individuelle(noir_corr, NULL);
           fiche_individuelle(blanc_corr,NULL); */

        printf("\n\n");
        goto pressez_touche;
    case 0 : /* pas de changement */
        printf("\n\n" ERES_I_AGAIN);
        goto abandon;
    case -1 : /* les joueurs specifies n'ont pas joue ensemble ! */
        printf("\n\n" CORR_PAS_JOUE "\n", ronde_corr + 1);
        goto abandon;
    }


abandon :
    beep();
    printf( "\n\n" CORR_ERROR);
pressez_touche :
    printf(CORR_TOUCHE);
    if (tolower(lire_touche()) == 'c')
      goto correction;
    eff_ecran();
    return;
}

/*
 * Affichage de la fiche d'un joueur. La somme des points et des pions
 * peut ne pas correspondre aux totaux indiques si le joueur a joue
 * contre bip
 */
 void fiche_individuelle(long no_joueur, const char *filename) {
    long     i, ronde0, couleur, adversaire, carcoul;
    discs_t Score;
    Player  *j;
    char    *nom, ligne[256], *comm, csc[256];

    carcoul = strlen(couleur_1);
    if ((i = strlen(couleur_2)) > carcoul)
        carcoul = i;

    i = numero_inscription(no_joueur);
    if (i < 0) {
        /* Le joueur n'est pas inscrit */
    beep();
    return;
    }
    nom = trouver_joueur(no_joueur) -> fullname;

    more_init(filename);
    sprintf(ligne, FICH_HEADER, nom, no_joueur, current_round);
    more_line(ligne); more_line("");
    if (current_round < 1)
        goto fin_fiche;

    /*
     * Toute cette partie (jusqu'a fin_fiche) est sautee si nous sommes
     * avant la premiere ronde.
     */
    for (ronde0 = 0; ronde0 < current_round; ronde0++)
        if ((couleur = polar2(no_joueur,ronde0)) != 0) {
            /* Chercher l'adversaire */
            long n1, n2;
            discs_t v;

            adversaire = -1;
            Score = UNKNOWN_SCORE;
            round_iterate(ronde0);
            while (next_couple(&n1, &n2, &v)) {
                if (n1 == no_joueur) {
                    assert(couleur == 1);
                    adversaire = n2;
                    Score = v;
                    break;
                } else if (n2 == no_joueur) {
                    assert(couleur == 2);
                    adversaire = n1;
                    Score = OPPONENT_SCORE(v);
                    break;
                }
            }
            /* L'avons-nous trouve ? */
            assert(adversaire >= 0 && SCORE_IS_LEGAL(Score));
            j = trouver_joueur(adversaire);
            assert(j);
            if (aff_diff_scores) {
                strcpy(csc, fscore(Score));
            } else {
                if (IS_VICTORY(Score))
                    sprintf(csc, "%c %s", '+', pions_en_chaine(Score));
                if (IS_DEFEAT(Score))
                    sprintf(csc, "%c %s", ' ', pions_en_chaine(Score));
                if (IS_DRAW(Score))
                    sprintf(csc, "%c %s", '=', pions_en_chaine(Score));
            }
            sprintf(ligne,"%3ld. %-*s  %-*s  %s (%ld)", ronde0+1,
            (int)carcoul, (couleur == 1 ? couleur_1 : couleur_2), (int)nb_chiffres_des_scores + 4 ,csc,
            j->fullname, j->ID);
            more_line(ligne);
        } else {
            /* Le joueur a passe, indiquons-le clairement: */
            sprintf(ligne,"%3ld.", ronde0+1);
            more_line(ligne);
        }
        /* Afficher le total des points, des pions et des scores */
        more_line("");
        /* La variable i contient toujours le ID d'inscription */
        if (aff_diff_scores) {
            /*
             * On implemente la ligne suivante :
             * Score = 2*nbr_discs[i] - ronde*total_pions;
             */
            Score = nbr_discs[i];
            ADD_SCORE(Score, Score);
            ADD_SCORE(Score, INTEGER_TO_SCORE(-current_round * total_pions));
            sprintf(csc, "%s%s", SCORE_IS_POSITIVE(Score) ? "+":"", pions_en_chaine(Score));
        } else {
            sprintf(csc, "%s", pions_en_chaine(nbr_discs[i]));
        }

        if ((score[i] % 2) == 0)
            sprintf(ligne, FICH_TRAILER_PAIR, score[i] / 2, current_round, csc);
        else
            sprintf(ligne, FICH_TRAILER_IMPAIR, score[i] / 2, current_round, csc);

        more_line(ligne);

fin_fiche:
        /*
         * Meme si aucune partie n'a encore ete jouee, il peut etre interessant
         * de savoir si le joueur a deja(!) abandonne, ou s'il y a un commentaire
         * le concernant.
         */
        if (!present[i])
            more_line(FICH_OUT);
        comm = trouver_joueur(no_joueur) -> comment;
        if (comm) {
            sprintf(ligne, "--> %s", comm);
            more_line(ligne);
        }
        more_close();
}

/*
 * Calcul du departage.
 *
 * On recupere tous les resultats et on appelle
 * la fonction DepartageJoueur() du fichier tiebreak.c
 */
void calcul_departage (void) {
    long i, ronde0, n1, n2, n, num_elo;
    BQ_results TabJ ;
    discs_t v, pions;
    long SO ;
    long nb_joueurs = registered_players->n;

    /* Initialisation */
    for (i = 0; i < nb_joueurs; i++)
        tieBreak[i]  = 0.0;

    if (current_round <= 0)
        return;

    /* Calcul du departage de chaque joueur */
    for (i = 0; i < nb_joueurs; i++) {
        for (ronde0 = 0; ronde0 < current_round; ronde0++) {
            TabJ.opp[ronde0] = 0 ;
            TabJ.opp_points[ronde0] = 0 ;
            TabJ.score[ronde0] = UNKNOWN_SCORE ;
            round_iterate(ronde0);
            while (next_couple(&n1, &n2, &v)) {
                if (i==numero_inscription(n1)) {
                    n = numero_inscription(n2);
                    TabJ.opp[ronde0] = n2 ;
                    TabJ.opp_points[ronde0] = score[n] ;
                    TabJ.score[ronde0] = v ;
                }
                if (i==numero_inscription(n2)) {
                    n = numero_inscription(n1);
                    TabJ.opp[ronde0] = n1 ;
                    TabJ.opp_points[ronde0] = score[n] ;
                    TabJ.score[ronde0] = OPPONENT_SCORE(v) ;
                }
            }
            num_elo = registered_players->list[i]->ID;
            if (player_was_present(num_elo, ronde0) && polar2(num_elo, ronde0)==0) { /* joue Bip */
                TabJ.score[ronde0] = OPPONENT_SCORE(score_bip) ;
            }
        }
        TabJ.player_points = score[i] ;
        DepartageJoueur(&TabJ, current_round-1, &tieBreak[i], &pions, &SO) ;
    }
}
