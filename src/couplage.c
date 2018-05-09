/*
 * couplage.c: cree une instance de la classe "couplage", qui est
 * simplement un ensemble de couples {(x1,x2),(x3,x4)..(x2n-1,x2n)}
 * disjoints, avec des valeurs.
 * Les operations fournies sont: accoupler deux entiers,
 * decoupler un entier, obtenir la polarite d'un entier, et un iterateur
 * pour obtenir la liste de toutes les paires.
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, retour a la version anterieure de la fonction 'joueur_etait_present()'
 *                   qui ne teste plus le cas de la ronde courante. Evite le bug de la
 *                   correction d'un resultat d'une ronde precedente.
 * (EL) 16/07/2012 : v1.34, modification de la fonction 'joueur_etait_present()' pour inclure
 *                   aussi le cas de la ronde courante.
 *                   Ajout dans la fonction 'sauver_fichier_appariements()' du test
 *                   pour decider de la sauvegarde au format xml.
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, Modification de la structure de sauvegarde du fichier 'Nouveaux'.
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 12/02/2007 : Modification de 'affiche_appariements()' pour qu'il affiche
 *                   le nom du tournoi au debut.
 * (EL) 02/02/2007 : Changement du type de 'departage[]' en double
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "pions.h"
#include "joueur.h"
#include "couplage.h"
#include "global.h"
#include "more.h"


typedef struct _Couple {
    long     premier;
    long     second;
    pions_t valeur;
    long     table;
    struct  _Couple *next;
} Couple;

#ifdef ENGLISH
# define RND_SAVE           "Saving round %ld"
# define RND_RESULTS        "Results of round %ld"
# define RND_INSC           "Players inscribed for round %ld"
# define NEW_BEGIN_BLOCK    " New players, round %ld"
# define NEW_END_BLOCK      " End of new players for round %ld"
# define MIGR_BEGIN_BLOCK   " Players with changed nationalities, round %ld"
# define MIGR_END_BLOCK     " End of emigrants, round %ld"
# define RND_SINSC          "Saving inscribed players of round %ld"
# define CANT_OPEN          "Can't open file"
# define RND_PAIR           "Pairings of round %ld"
# define INCOMPLETE_P       " (not complete)"
# define RND_SPAIR          "Saving pairings of round %ld"
# define NOT_INSC           "Player %ld is not inscribed"
# define SUPP_REASON        "\nPlayer %s (%ld) hasn't played any game..."
# define SUPP_ASK           "\nShall we suppress completely that player (Y/N)?"
# define MAN_PAIRED         "Paired players"
# define MAN_UNPAIRED       "Lonely players"
# define MAN_PROMPT         "[A]ssociate, [D]issociate, [Z]ap or [Q]uit?"
# define MAN_ASS            "Associate which players?"
# define MAN_DISS           "Dissociate which player?"
# define MAN_ZAP            "Really clear all pairings (Y/N)?"
# define MAN_WHICHP         "Which player?"
# define MAN_ASS_BLACK      "  First player (%s):"
# define MAN_ASS_WHITE      "  Second player (%s):"
# define AFF_NOBODY         "No players inscribed."
# define WHICH_FEATS        "Feats for which player?"
# define HIT_ANY_KEY        "Hit any key..."
# define FIC_TEMP           "Temporary file name: %s"
#else
# define RND_SAVE           "Sauvegarde de la ronde %ld"
# define RND_RESULTS        "Resultats de la ronde %ld"
# define RND_INSC           "Inscrits pour la ronde %ld"
# define NEW_BEGIN_BLOCK    " Nouveaux joueurs, ronde %ld"
# define NEW_END_BLOCK      " Fin des nouveaux joueurs pour la ronde %ld"
# define MIGR_BEGIN_BLOCK   " Joueurs ayant change de nationalite, ronde %ld"
# define MIGR_END_BLOCK     " Fin des joueurs emigres, ronde %ld"
# define RND_SINSC          "Sauvegarde des inscrits de la ronde %ld"
# define CANT_OPEN          "Impossible d'ouvrir le fichier"
# define RND_PAIR           "Appariements de la ronde %ld"
# define INCOMPLETE_P       " (incomplets)"
# define RND_SPAIR          "Sauvegarde des appariements de la ronde %ld"
# define NOT_INSC           "Le joueur %ld n'est pas inscrit"
# define SUPP_REASON        "\nLe joueur %s (%ld) n'a joue aucune partie..."
# define SUPP_ASK           "\nVoulez-vous supprimer completement ce joueur (O/N) ?"
# define MAN_PAIRED         "Joueurs apparies"
# define MAN_UNPAIRED       "Joueurs non apparies"
# define MAN_PROMPT         "[A]pparier, [D]ecoupler, RA[Z] ou [Q]uitter ?"
# define MAN_ASS            "Apparier quels joueurs ?"
# define MAN_ASS_BLACK      "  Premier joueur (%s) :"
# define MAN_ASS_WHITE      "  Deuxieme joueur (%s) :"
# define MAN_DISS           "Decoupler quel joueur ?"
# define MAN_ZAP            "Effacer tous les appariements (O/N) ?"
# define MAN_WHICHP         "Quel joueur ?"
# define AFF_NOBODY         "Aucun joueur n'est encore inscrit."
# define WHICH_FEATS        "Fiche de quel joueur ?"
# define HIT_ANY_KEY        "Pressez une touche..."
# define FIC_TEMP           "Nom du fichier temporaire : %s"
#endif


/*
 * Gestion des rondes
 */

static Couple *historique[NMAX_RONDES];
static long couplage_modifie = 0,
ancien_nb_inscrits = 0,
presents_modifies = 0;

/*
* Historique des presences de chaque joueur
 */

#define TAILLE_ENTIERS 32    /* 16 bits : taille des entiers "unsigned long" sur le compilateur */
#define NB_SLOTS_BITS  ( 1 + ((NMAX_RONDES - 1) / TAILLE_ENTIERS))

static unsigned long historique_presences[MAX_INSCRITS][NB_SLOTS_BITS];


/* Initialisations avant la premiere ronde */

void premiere_ronde() {
    static long historique_initialise = 0;
    long i,j;


    /* Initialisation de l'historique des couplages */
    if (historique_initialise)
        for (ronde = 0; ronde < NMAX_RONDES; ronde ++)
            raz_couplage();
    for (i=0; i<NMAX_RONDES; i++) historique[i] = NULL;
    historique_initialise = 1;

    /* Initialisation de l'historique des presences */
    for (i=0; i<MAX_INSCRITS; i++)
        for (j=0; j<NB_SLOTS_BITS; j++)
            historique_presences[i][j] = 0;

    /* remettons a zero les scores, pions, departages, etc */
    raz_scores();

    ronde = 0;
    couplage_modifie = 0;
}

/* Passage a la ronde suivante */

void ronde_suivante() {
    long i;

    assert(0 <= ronde && ronde < NMAX_RONDES-1);

    /* on enregistre les presents de la ronde dans l'historique */
    for (i = 0; i < joueurs_inscrits->n; i++)
        set_historique_presence(joueurs_inscrits->liste[i]->numero, ronde, present[i]);

    ++ronde;
    couplage_modifie = 1;
}

/*
 * Sauve les resultats de la ronde courante; les signaux sont supposes
 * etre masques. Aucun controle d'erreur pour l'instant!
 *
 * S'il s'agit de la premiere ronde et que le nombre de joueurs permet un
 * toutes-rondes, alors on sauvegarde la `table' de numerotation
 * des joueurs.
 */

void _sauve_ronde() {
    long n1, n2;
    pions_t v;
    FILE *fp;

    fp = myfopen_dans_sous_dossier(nom_fichier_inter,"a","",0, 1) ;
#ifdef DEBUG
    fprintf(stderr, RND_SAVE "\n", ronde+1);
#endif
    /* Nous en profitons pour sauvegarder la table du toutes-rondes */
    if (ronde == 0) {
        init_ttrondes();
        sauve_table_ttr(fp);
    }
    /* Puis les resultats proprement dits */
    fprintf(fp,"%% " RND_RESULTS "\n\n" "raz-couplage;\n", ronde+1);
    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v)) {
        assert(SCORE_EST_LEGAL(v));
        fprintf(fp,"(%06ld %-5s %06ld %s);\n", n1, pions_en_chaine(v),
                n2, pions_en_chaine(SCORE_ADVERSE(v)));
    }
    fprintf(fp,"ronde-suivante;\n\n");
    fclose(fp);
    backup_inter() ;
    couplage_modifie = 0;
}

/*
 * Si de nouveaux joueurs se sont inscrits, ou si des joueurs ont quitte
 * ou reintegre le tournoi, ou s'ils ont change de nationalite(!),
 * alors cette fonction effectue les sauvegardes necessaires dans le
 * fichier intermediaire et/ou le fichier des nouveaux. Les signaux sont
 * supposes bloques.
 */

void _sauve_inscrits() {
    long i;
    FILE *fp;
    Joueur *j;

    if (ancien_nb_inscrits == joueurs_inscrits->n /* pas de nouveaux inscrits */
        && presents_modifies == 0           /* personne n'est entre ou sorti */
        && joueurs_emigres->n == 0)        /* ou n'a change de pays */
        return;

    /* ecriture dans le fichier intermediaire 'papp-internal-workfile.txt' */
    fp = myfopen_dans_sous_dossier(nom_fichier_inter,"a","",0, 1) ;
#ifdef DEBUG
    fprintf(stderr, RND_SINSC "\n", ronde+1);
#endif
    fprintf(fp,"%% " RND_INSC "\n\n&", ronde+1);
    for (i=0; i<joueurs_inscrits->n; i++)
        if (joueurs_inscrits->liste[i]->numero > 0)
            fprintf(fp,"%s%c%06ld", i!=0 && i%10==0 ? "\n  " : " ",
                    present[i]?'+':'-',
                    joueurs_inscrits->liste[i]->numero);
    fprintf(fp,";\n\n");
    fclose(fp);
    backup_inter() ;

    /* Mais il faut egalement sauvegarder les nouveaux joueurs */
    fp = myfopen_dans_sous_dossier(nom_fichier_nouveaux,"a","",0, 1) ;
    for (i=0; i<joueurs_nouveaux->n; i++) {
        j = joueurs_nouveaux->liste[i];
        if (j->numero > 0)
            fprintf(fp,"%6ld %s, %s\t{%s}\n", j->numero, j->nom_famille, j->prenom, j->pays);
    }
    fclose(fp);

    /* On met les nouveaux en commentaires dans le fichier intermediaire */
    fp = myfopen_dans_sous_dossier(nom_fichier_inter,"a","",0, 1) ;
    if (joueurs_nouveaux->n > 0) {
        fprintf(fp,"%%%%  ");
        fprintf(fp, NEW_BEGIN_BLOCK , (long)(ronde+1));
        fprintf(fp,"\n%%\n");
        for (i=0; i<joueurs_nouveaux->n; i++) {
            j = joueurs_nouveaux->liste[i];
            if (j->numero > 0)
                fprintf(fp,"%%_%% %6ld %s, %s\t{%s}\n", j->numero, j->nom_famille, j->prenom, j->pays);
        }
        fprintf(fp,"%%\n%%");
        fprintf(fp, NEW_END_BLOCK, (ronde+1));
        fprintf(fp,"\n\n\n");
    }
    fclose(fp);
    backup_inter() ;

    /* Les nouveaux sont connus maintenant */
    vider_liste(joueurs_nouveaux);

    /* Ainsi que les joueurs emigres */
    fp = myfopen_dans_sous_dossier(nom_fichier_nouveaux,"a","",0, 1) ;
    for (i=0; i<joueurs_emigres->n; i++) {
        j = joueurs_emigres->liste[i];
        if (j->numero > 0)
            fprintf(fp,"%%_ %6ld {%s}\n", j->numero, j->pays);
    }
    fclose(fp);
    /* On met aussi les emmigres en commentaires dans le fichier intermediaire */
    fp = myfopen_dans_sous_dossier(nom_fichier_inter,"a","",0, 1) ;
    if (joueurs_emigres->n > 0) {
        fprintf(fp,"%%%%  ");
        fprintf(fp, MIGR_BEGIN_BLOCK, (ronde+1));
        fprintf(fp,"\n%%\n");
        for (i=0; i<joueurs_emigres->n; i++) {
            j = joueurs_emigres->liste[i];
            if (j->numero > 0)
                fprintf(fp,"%% %6ld %s {%s}\n", j->numero, j->nom, j->pays);
        }
        fprintf(fp,"%%\n%%");
        fprintf(fp, MIGR_END_BLOCK, (ronde+1));
        fprintf(fp,"\n\n\n");
    }
    fclose(fp);
    backup_inter() ;
    /* Les emmigres sont connus maintenant */
    vider_liste(joueurs_emigres);

    ancien_nb_inscrits = joueurs_inscrits->n;
    presents_modifies = 0;
}

/*
 * Sauver les appariements s'ils ont change depuis la derniere fois, i.e.
 * si drapeau_modifie != 0. Cette fonction n'est utilisee qu'en mode
 * de sauvegarde immediate. Les signaux sont supposes bloques.
 */

void _sauve_appariements() {
    long n1, n2;
    pions_t v;
    FILE *fp;

    if (couplage_modifie == 0)
        return;
    fp = myfopen_dans_sous_dossier(nom_fichier_inter,"a","",0, 1) ;
#ifdef DEBUG
    fprintf(stderr, RND_SPAIR "\n", ronde+1);
#endif
    assert(fp);
    fprintf(fp,"%% " RND_PAIR "\n\n" "raz-couplage;\n", ronde+1);
    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v))
        fprintf(fp, COUPON_EST_VIDE(v) ? "(%06ld %06ld);\n" : "(%06ld %06ld %s);\n",
                n1, n2, pions_en_chaine(v));
    fprintf(fp,"\n");
    fclose(fp);
    backup_inter() ;
    couplage_modifie = 0;
}


/*
 * Recreer totalement un fichier 'papp-internal-workfile.txt' a partir de l'historique.
 * Utilisee par exemple apres la correction d'un resultat, ou
 * lorque l'on desinscrit un joueur qui n'a joue aucune partie.
 */

void _recreer_fichier_intermediaire() {
    long cur_ronde;
    long cur_inscrits;
    long cur_couplage_modifie;
    long cur_presents_modifies;
    long cur_present[MAX_INSCRITS];
    char *cur_nom_fichier_inter = NULL;
    char fic_temporaire[50] = "__papp.tmp";
    char fic_sauvegarde[50] = "__papp.bak";
    long error;
    FILE *papp_file ;

    long i, joueur_est_present;
    long present_avant[MAX_INSCRITS];

    /* Sauvegarder les variables globales de Papp */
    cur_ronde             = ronde;
    cur_inscrits          = ancien_nb_inscrits;
    cur_couplage_modifie  = couplage_modifie;
    cur_presents_modifies = presents_modifies;
    for (i = 0; i < joueurs_inscrits->n; i++)
        cur_present[i]    = present[i];
    COPIER(nom_fichier_inter, &cur_nom_fichier_inter);


    /* On ecrase le fichier "__papp.tmp" (s'il y en a un) */
    error = remove(fic_temporaire);
    /* On fait les sauvegardes intermediaires dans "__papp.tmp" */
    COPIER(fic_temporaire, &nom_fichier_inter);

    /* d'abord les infos du tournoi*/
    init_fichier_intermediaire(INEXISTANT) ;
    /* Puis le fichier des nouveaux */
    if ((papp_file = myfopen_dans_sous_dossier(nom_fichier_inter, "ab", "", 0, 0)) != NULL) {
        copier_fichier_nouveaux(papp_file) ;
        fclose(papp_file) ;
    }

#ifdef DEBUG
    fprintf(stderr, FIC_TEMP "\n", nom_fichier_inter);
#endif

    /* on initialise le tableau des presents */
    for (i = 0; i < joueurs_inscrits->n; i++)
        present[i] = 0;

    /* reconstitution du tournoi d'apres l'historique */
    for (ronde = 0; ronde <= cur_ronde - 1 ; ronde++) {

        for (i = 0; i < joueurs_inscrits->n; i++)  {
            present_avant[i] = present[i];
            present[i] = joueur_etait_present(joueurs_inscrits->liste[i]->numero, ronde);
        }

        presents_modifies = 0;
        for (i = 0; i < joueurs_inscrits->n; i++)
            presents_modifies |= (present[i] != present_avant[i]);
        /* toujours sauver les inscrits de la ronde 1 */
        if (ronde == 0) presents_modifies = 1;

        /* on sauve les presents/absents de cette ronde passee... */
        if (presents_modifies)
            _sauve_inscrits();
        /* ... et les resultats */
        _sauve_ronde();
    }

    ronde = cur_ronde;

    /* presents de la ronde en cours */
    presents_modifies = 0;
    for (i = 0; i < joueurs_inscrits->n; i++)  {
        joueur_est_present = cur_present[i];
        if (joueur_est_present != present[i]) {
            present[i] = joueur_est_present;
            presents_modifies = 1;
        }
    }
    /* toujours sauver les inscrits de la ronde 1 */
    if (ronde == 0) presents_modifies = 1;

    /* on sauve les presents/absents de la ronde en cours... */
    if (presents_modifies)
        _sauve_inscrits();
    /* ... et les appariements eventuels */
    assert((ronde >= 0) && (ronde < NMAX_RONDES));
    if (historique[ronde] != NULL)  {
        couplage_modifie = 1;
        _sauve_appariements();
    }


    /* on renomme l'ancien fichier "papp-internal-workfile.txt" en "__papp.bak" */
    error = remove(fic_sauvegarde);
    error = rename(cur_nom_fichier_inter,fic_sauvegarde);
    /* puis le fichier "__papp.tmp" en "papp-internal-workfile.txt" */
    error = rename(nom_fichier_inter,cur_nom_fichier_inter);

    /* Termine, retablir les variables globales de papp */
    ronde              = cur_ronde;
    ancien_nb_inscrits = cur_inscrits;
    couplage_modifie   = cur_couplage_modifie;
    presents_modifies  = cur_presents_modifies;
    for (i = 0; i < joueurs_inscrits->n; i++)
        present[i]     = cur_present[i];
    COPIER(cur_nom_fichier_inter, &nom_fichier_inter);
    backup_inter() ;
}


/*
 * Les deux fonctions suivantes signalent a PAPP que la liste des
 * inscrits/presents (resp. des appariements) est a jour et ne necessite
 * pas de sauvegarde -- voir main.c.
 */

void ne_pas_sauver_inscrits() {
    ancien_nb_inscrits = joueurs_inscrits->n;
    presents_modifies = 0;
}

void ne_pas_sauver_appariements() {
    couplage_modifie = 0;
}

/*
 * Operations sur les couplages
 */

void raz_couplage() {
    Couple *p, *q;

    for (p = historique[ronde]; p; p = q) {
        q = p->next;
        free(p);
        couplage_modifie = 1;
    }
    historique[ronde] = NULL;
}

long decoupler(long n) {
    Couple **p, *q;

    for (p = &historique[ronde]; (q = *p) != NULL; p = &(q->next))
        if (q->premier == n || q->second == n) {
            /* enlever cette paire */
            *p = q->next;
            free(q);
            couplage_modifie = 1;
            return 1;
        }
        return 0;
}

void accoupler(long n1, long n2, pions_t valeur) {
    Couple *q, *p;

    decoupler(n1);
    decoupler(n2);
    if (n1 == n2)
        return;
    CALLOC(q, 1, Couple);
    q->premier = n1;
    q->second  = n2;
    q->valeur  = valeur;
    q->table   = 0;
    q->next    = NULL;

    /* ajouter q a la fin de la liste */
    if (historique[ronde] == NULL)
        historique[ronde] = q;
    else {
        p = historique[ronde];
        while (p->next)
            p = p->next;
        p->next = q;
    }

    couplage_modifie = 1;
}

/*
 * polarite(n): renvoie 0 si le joueur n'est pas apparie, 1 s'il a les noirs
 * et 2 s'il a les blancs. La fonction polar2() prend un argument
 * supplementaire qui est le numero de la ronde.
 */

long polarite (long n) {
    return polar2 (n, ronde);
}

long polar2 (long n, long ronde) {
    Couple *q;

    if (ronde < 0 || ronde >= NMAX_RONDES)
        return 0;
    for (q = historique[ronde]; q; q = q->next) {
        if (q->premier == n)
            return 1;
        if (q->second == n)
            return 2;
    }
    return 0;
}

/*
 * renvoie un pointeur sur la variable contenant le "score" de n1 contre
 * n2, ou NULL si n1 n'a pas joue contre n2 avec les noirs. Comme ce
 * pointeur peut etre utilise pour modifier cette variable, nous levons
 * le drapeau couplage_modifie. Usage typique:
 *
 *    long n1, n2, v;
 *    *valeur_couple(n1,n2) = v;
 */

pions_t *valeur_couple(long n1, long n2) {
    Couple *q = historique[ronde];

    while (q)
        if (q->premier == n1 && q->second == n2) {
            /*
             * Quelqu'un peut se servir de cette adresse pour
             * modifier les resultats des appariements.
             */
            couplage_modifie = 1;
            return &(q->valeur);
        }
            else q = q->next;
    return NULL;
}

/*
*  nb_couples(nro_ronde) :
 *  renvoie le nombre d'appariements de la ronde <nro_ronde>
 */

long nb_couples(long nro_ronde) {
    long compteur;
    Couple *q;

    if (nro_ronde < 0 || nro_ronde >= NMAX_RONDES || nro_ronde > ronde)
        return 0;

    compteur = 0;
    for (q = historique[nro_ronde]; q; q = q->next)
        compteur++;
    return compteur;
}

/*
 * Pour iterer sur l'ensemble des couples, on ecrira:
 *
 *    long n1, n2;
 *    pions_t v;
 *    iterer_ronde(numero_de_ronde);
 *    while (couple_suivant(&n1,&n2,&v)) {
     *        ... Utiliser n1,n2,v ...
     *        }
 *
 * On pourra tester la valeur v == SCORE_INCONNU pour indiquer
 * que le score n'est pas encore connu.
 */

static Couple *pointeur;

void iterer_ronde (long nro_ronde) {
    pointeur = (nro_ronde >= 0 && nro_ronde < NMAX_RONDES) ?
    historique[nro_ronde] : NULL;
}

long couple_suivant (long *n1, long *n2, pions_t *valeur) {
    if (pointeur == NULL)
        return 0;
    *n1 = pointeur->premier;
    *n2 = pointeur->second;
    *valeur = pointeur->valeur;
    pointeur = pointeur->next;
    return 1;
}

/*
 * Fonctions diverses pour savoir si un joueur est present,
 * ainsi que son score
 */

long         present[MAX_INSCRITS];
long         score[MAX_INSCRITS];
pions_t      nb_pions[MAX_INSCRITS];
double       departage[MAX_INSCRITS];
long         dern_flot[MAX_INSCRITS];
long         score_equipe[MAX_INSCRITS];

/*
 * Fait entrer (direction == 1) ou sortir (direction == 0) le joueur de
 * numero Elo no_joueur. Si c'est impossible, les erreurs sont affichees
 * a l'ecran.
 */

void es_joueur(long no_joueur, long direction) {    /* 0=sortir, 1=entrer */
    long i,ronde0,n1,n2,jamais_joue, ret;
    pions_t v;
    char *nom;
    char chaine[500];

    for (i=0; i<joueurs_inscrits->n; i++)
        if (joueurs_inscrits->liste[i]->numero == no_joueur) {
            nom = joueurs_inscrits->liste[i]->nom;
            /* Tester l'etat actuel du joueur */

            if (present[i] == direction)
#ifdef ENGLISH
                printf("Player %s (%ld) was already %s\n", nom, no_joueur,
                       direction ? "in" : "out");
            else
                printf("Player %s (%ld) %s the tournament\n", nom, no_joueur,
                       direction ? "enters" : "leaves");
#else
                printf("Le joueur %s (%ld) etait deja %s\n", nom, no_joueur,
                       direction ? "present" : "absent");
            else
                printf("Le joueur %s (%ld) %s\n", nom, no_joueur,
                       direction ? "entre" : "sort");
#endif

            present[i] = direction;
            decouplage_absents();
            presents_modifies = 1;

            /* En cas de sortie, et si le joueur n'a joue aucune partie,
             * on propose de le supprimer completement du tournoi
             */
            jamais_joue = 1;
        if (direction == 0) {
            for (ronde0 = 0; ronde0 < ronde; ronde0++) {
                iterer_ronde(ronde0);
                while (couple_suivant(&n1,&n2,&v))
                    if ((n1 == no_joueur) || (n2 == no_joueur)) jamais_joue = 0;
            }
            if (jamais_joue) {
                printf(SUPP_REASON, nom, no_joueur);
                sprintf(chaine, SUPP_ASK);
                if (oui_non(chaine)) {

                    /* on met temporairement le numero du joueur a zero,
                    de maniere a ce qu'il ne soit pas mis dans le fichier
                    intermediaire "papp-internal-workfile.txt" que l'on regenere. */
                    joueurs_inscrits->liste[i]->numero = 0;
                    recreer_fichier_intermediaire();
                    joueurs_inscrits->liste[i]->numero = no_joueur;

                    /* on relit immediatement le nouveau fichier "papp-internal-workfile.txt" */
                    vider_liste(joueurs_inscrits);
                    premiere_ronde();
                    ret = lire_fichier(nom_fichier_inter, F_CONFIG);
                    if (ret > 0)
                        erreur_fatale("ERREUR DE FICHIER dans la fonction es_joueur");
                }
                printf("\n"HIT_ANY_KEY);
            }
        }
        return;
    }
/* Pas trouve ? */
    printf(NOT_INSC " !?\n", no_joueur);
    beep();
}

/*
 * Faire sortir le joueur du tournoi avec present[i]=0 ne suffit pas a
 * detruire son appariement; il faut invoquer la fonction suivante,
 * qui decouple tous les joueurs absents:
 */

void decouplage_absents() {
    long i;

    for (i=0; i<joueurs_inscrits->n; i++)
        if (!present[i])
            decoupler(joueurs_inscrits->liste[i]->numero);
}

/*
 * La fonction suivante permet d'enregistrer dans l'historique la presence
 * (presence =1) ou l'absence (presence=0) d'un joueur a une ronde donnee
 */

void set_historique_presence(long numero_elo, long quelle_ronde, long presence) {
    long i, which_bit_field;
    unsigned long mask;


    assert(quelle_ronde >= 0 && quelle_ronde <= ronde);
    assert(numero_elo >= 0);

    which_bit_field = (quelle_ronde / TAILLE_ENTIERS);
    mask = ((unsigned long)1) << (quelle_ronde % TAILLE_ENTIERS);

    assert(which_bit_field >= 0  &&  which_bit_field < NB_SLOTS_BITS);

    for (i=0; i<joueurs_inscrits->n; i++)
        if (joueurs_inscrits->liste[i]->numero == numero_elo)  {
            if (presence != ((historique_presences[i][which_bit_field] & mask) != 0))
                historique_presences[i][which_bit_field] ^= mask;
        }
}

/*
* La fonction suivante permet de tester si un joueur etait present a une
 * ronde passee. Note : pour la ronde courante, utiliser plutot le tableau
 * global "present" (car la variable 'ronde' peut avoir ete  modifiee).
 */

long joueur_etait_present(long numero_elo, long quelle_ronde) {
    long i, which_bit_field;
    unsigned long mask;


    assert(quelle_ronde >= 0);
    assert(numero_elo >= 0);

    which_bit_field = (quelle_ronde / TAILLE_ENTIERS);
    mask = ((unsigned long)1) << (quelle_ronde % TAILLE_ENTIERS);

    assert(which_bit_field >= 0 && which_bit_field < NB_SLOTS_BITS);

    for (i=0; i<joueurs_inscrits->n; i++)
        if (joueurs_inscrits->liste[i]->numero == numero_elo) {
	        return ((historique_presences[i][which_bit_field] & mask) != 0);
        }

    /* le joueur n'est meme pas inscrit ! */
    return 0;
}


/*
 * Maintenant la vraie routine de manipulation des appariements
 */

void manipule_appariements() {
    char chaine[256], c , *ligne;
    long l, lmax, nbc, i, k, n1, n2, _n1, _n2, passe;
    pions_t v;

    for(;;) {
        /* Afficher la liste des joueurs apparies */
        eff_ecran();
        printf(MAN_PAIRED " :\n");
        lmax = nbc = 0;
        for (passe=0; passe<2; passe++) {
            k = 0;
            iterer_ronde(ronde);
            while (couple_suivant(&n1,&n2,&v)) {

#ifdef AFFICH_COUPLAGES_ALPHA
                sprintf(chaine,"%-8.8s(%6ld)--%-8.8s(%6ld) ",
                        trouver_joueur(n1)->nom,n1,trouver_joueur(n2)->nom,n2);
#else
                sprintf(chaine,"%ld--%ld",n1,n2);
#endif

                if (passe) {
                    /* Deuxieme passe, faire l'affichage */
                    assert(nbc > 0);
                    printf("%c%-*s", k++%nbc ? ' ':'\n', (int)lmax, chaine);
                } else {
                    /* Premiere passe, calculer la largeur maximum */
                    l = strlen(chaine);
                    if (l > lmax)
                        lmax = l;
                }
            }
            if (lmax == 0)
                break;
            nbc = nb_colonnes / (lmax + 1);
        }
        /* Afficher la liste des joueurs non apparies */
        printf("\n\n" MAN_UNPAIRED " :\n");
        lmax = nbc = 0;
        for (passe=0; passe<2; passe++) {
            k = 0;
            for (i=0; i<joueurs_inscrits->n; i++)
                if (present[i] && !polarite(joueurs_inscrits->liste[i]->numero)) {
#ifdef AFFICH_COUPLAGES_ALPHA
                    sprintf(chaine,"%-8.8s(%ld)",
                            joueurs_inscrits->liste[i]->nom,joueurs_inscrits->liste[i]->numero);
#else
                    sprintf(chaine,"%ld",joueurs_inscrits->liste[i]->numero);
#endif

                    if (passe) {
                        /* Deuxieme passe */
                        assert(nbc > 0);
                        printf("%c%-*s", k++%nbc ? ' ':'\n', (int)lmax, chaine);
                    } else {
                        /* Premiere passe */
                        l = strlen(chaine);
                        if (l > lmax)
                            lmax = l;
                    }
                }
            if (lmax == 0)
                break;
            nbc = nb_colonnes / (lmax + 1);
        }
        printf("\n\n");
        /* Et afficher le prompt */
        printf(MAN_PROMPT);
        c = lire_touche(); c = tolower(c);
        switch(c) {
            case 'v':
                affiche_appariements(NULL, 0);
                break;
            case 'l':
                affiche_inscrits(NULL);
                break;
            case 'f':
                eff_ligne();
                printf("\n");
                if ((n1 = choix_d_un_joueur_au_clavier(WHICH_FEATS, joueurs_inscrits, &ligne)) >= 0)
                    /*
                     * Il n'est pas necessaire de verifier que le joueur est
                     * inscrit puisque fiche_individuelle() le verifie elle-meme.
                     */
                    fiche_individuelle(n1,NULL);
                    break;
            case 'z':
                eff_ligne();
                printf("\n");
                if (oui_non(MAN_ZAP))
                    raz_couplage();
                    break;
            case 'a':
                eff_ligne();
                printf("\n");
                printf(MAN_ASS);
                printf("\n");

                sprintf(chaine, MAN_ASS_BLACK, couleur_1);
                if (((n1 = choix_d_un_joueur_au_clavier(chaine, joueurs_inscrits, &ligne)) >= 0)
                    && ((_n1=numero_inscription(n1)) >=0) && present[_n1])
                    puts(coupon(trouver_joueur(n1), NULL, SCORE_INCONNU));
                else { beep(); break;}

                    sprintf(chaine, MAN_ASS_WHITE, couleur_2);
                if (((n2 = choix_d_un_joueur_au_clavier(chaine, joueurs_inscrits, &ligne)) >= 0)
                    && ((_n2=numero_inscription(n2)) >=0) && present[_n2])
                    puts(coupon(trouver_joueur(n2), NULL, SCORE_INCONNU));
                else { beep(); break;}

                accoupler(n1, n2, SCORE_INCONNU);
                break;
            case 'd':
                eff_ligne();
                printf("\n");
                if (((n1 = choix_d_un_joueur_au_clavier(MAN_DISS, joueurs_inscrits, &ligne)) >= 0) &&
                    ((_n1=numero_inscription(n1)) >=0 ) &&
                    present[_n1] )
                    decoupler(n1);
                else beep();
                break;
            case 'q':
            case 3:        /* ^C */
                return;
            default:
                beep();
                break;
        } /* switch */
    } /* for(;;) */
}

/*
 * changer_resultat(nro_ronde, nro_joueur1, nro_joueur2, valeur)
 * Cherche le couplage de la ronde <nro_ronde>
 * impliquant les joueurs <nro_joueur1> et <nro_joueur2>,
 * les reordonne eventuellement (changement de couleur)
 * et met le score <valeur> au premier joueur (changement de score)
 * Renvoie :
 *   1  si le changement a ete effectue
 *   0  si le changement n'a pas ete effectue parce que
 *      les joueurs et le bon score etait deja dans l'historique
 *  -1  si le changement n'a pas ete effectue parce que
 *      les joueurs n'ont pas joue ensemble a la ronde <nro_ronde>
 */

long changer_resultat(long nro_ronde, long nro_joueur1, long nro_joueur2, pions_t valeur) {
    Couple *q;

    assert(nro_ronde >= 0 && nro_ronde <= ronde && nro_ronde < NMAX_RONDES);
    assert(nro_joueur1 > 0);
    assert(nro_joueur2 > 0);
    assert(SCORE_EST_LEGAL(valeur));


    for (q = historique[nro_ronde]; q; q = q->next) {

        if ((q->premier == nro_joueur1) &&
            (q->second  == nro_joueur2) &&
            EGALITE_SCORES(q->valeur, valeur))
            return 0;  /* pas besoin de changer ! */

        if (((q->premier == nro_joueur1) && (q->second == nro_joueur2)) ||
            ((q->premier == nro_joueur2) && (q->second == nro_joueur1))) {
            /* changer les joueurs et le score */
            q->premier = nro_joueur1;
            q->second  = nro_joueur2;
            q->valeur  = valeur;
            return 1;
        }
    }

    /* pas trouve */
    return -1;
}


/*
 * Affecte un numero de table au match opposant n1 et n2 a la ronde nro_ronde.
 */

void mettre_numero_de_table(long nro_ronde, long n1, long n2, long num_table) {
    Couple *q;

    assert(nro_ronde >= 0 && nro_ronde <= ronde && nro_ronde < NMAX_RONDES);
    assert(n1 > 0);
    assert(n2 > 0);

    for (q = historique[nro_ronde]; q; q = q->next) {
        if (((q->premier == n1) && (q->second == n2)) ||
            ((q->premier == n2) && (q->second == n1))) {
            /* changer le numero de la table */
            q->table = num_table;
        }
    }
}

/*
 * Renvoie le numero de table du match entre n1 et n2 de la ronde nro_ronde,
 * ou 0 si on ne trouve pas la table (pas de match, ou pas encore de table
 * affectee, etc)
 */

long numero_de_table(long nro_ronde, long n1, long n2) {
    Couple *q;

    assert(nro_ronde >= 0 && nro_ronde <= ronde && nro_ronde < NMAX_RONDES);
    assert(n1 > 0);
    assert(n2 > 0);

    for (q = historique[nro_ronde]; q; q = q->next) {
        if (((q->premier == n1) && (q->second == n2)) ||
            ((q->premier == n2) && (q->second == n1))) {
            return (q->table);
        }
    }
    return 0;
}

/*
 * Reaffecte tous les numeros de tables de la ronde nro_ronde.
 * On utilise le meme tri que pour l'affichage des coupons.
 */

void numeroter_les_tables() {
    long i, n1, n2;
    pions_t v;
    Paire *liste;
    long nb_paires;

    assert(ronde >= 0 && ronde < NMAX_RONDES);


    CALLOC(liste, 1 + joueurs_inscrits->n / 2, Paire);
    nb_paires = 0;

    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v)) {
        i = nb_paires++;
        liste[i].j1 = trouver_joueur(n1);
        liste[i].j2 = trouver_joueur(n2);
        liste[i].score = v;
        liste[i].valeur_tri = critere_tri_tables(n1, n2);
    }
    if (ronde >= 1)
        SORT(liste, nb_paires, sizeof(Paire), tri_paires);

    for (i = 0; i < nb_paires; i++)
        mettre_numero_de_table(ronde, (liste[i].j1)->numero, (liste[i].j2)->numero, i+1);

    free(liste);
}


/*
 * calcule le nombre de joueurs presents et non apparies
 */

long nb_joueurs_napp() {
    long n, i;

    for (i = n = 0; i < joueurs_inscrits->n; i++)
        if (present[i] && polarite(joueurs_inscrits->liste[i]->numero)==0)
            ++n;
    return n;
}


/*
 * Affichage des appariements
 */

int tri_paires(const void *ARG1, const void *ARG2) {
	const Paire *p1 = (const Paire *) ARG1 ;
	const Paire *p2 = (const Paire *) ARG2 ;
    double diff;

    diff = (p2->valeur_tri) - (p1->valeur_tri);
    if (diff > 0)
        return 1;
    else if (diff < 0)
        return -1;
    else
        return 0;
}


/*
 * Les paires de joueurs apparies sont triees par ordre decroissant de
 * "valeur", cette valeur etant le score du joueur le mieux classe, avec
 * comme critere secondaire la somme des scores des joueurs.
 * Ceci est susceptible de changer dans les prochaines versions.
 */

double critere_tri_tables(long n1, long n2) {

    return 100000.0 * le_max_de(score[numero_inscription(n1)],score[numero_inscription(n2)])
    + 100.0*(score[numero_inscription(n1)] + score[numero_inscription(n2)] )
    + numero_inscription(n1)/10.0;
}


void affiche_appariements(const char *filename, long resultats_complets) {
    char chaine[256];
    char *nomTrn ;
    long i, n1, n2;
    pions_t v;
    Joueur *j;
    Paire *liste;
    long nb_paires;

    /* Peut-etre devons-nous sauvegarder l'etat */
    if (sauvegarde_immediate) {
        sauve_inscrits();
        sauve_appariements();
    }
    more_init(filename);
    if (joueurs_inscrits->n == 0) {
        more_line(AFF_NOBODY);
        more_close();
        return;
    }

    /* Pour afficher le nom du tournoi en tete */
    nomTrn = malloc(strlen(nom_du_tournoi)+15) ;
    if (nomTrn != NULL) {
        sprintf(nomTrn, "*** %s ***\n", nom_du_tournoi) ;
        more_line(nomTrn) ;
        free(nomTrn) ;
    }
    sprintf(chaine, resultats_complets ? (RND_RESULTS "%s :") : (RND_PAIR "%s :"), ronde+1,
            nb_joueurs_napp() > 1 ? INCOMPLETE_P : "");
    more_line(chaine);
    more_line("");

    /* D'abord afficher les joueurs apparies */
    CALLOC(liste, 1 + joueurs_inscrits->n / 2, Paire);
    nb_paires = 0;

    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v)) {
        i = nb_paires++;
        liste[i].j1 = trouver_joueur(n1);
        liste[i].j2 = trouver_joueur(n2);
        liste[i].score = v;
        liste[i].valeur_tri = critere_tri_tables(n1, n2);
    }
    if (ronde >= 1)
        SORT(liste, nb_paires, sizeof(Paire), tri_paires);

#ifdef NUMEROS_TABLES
    {
        char tampon[400];
        for (i = 0; i < nb_paires; i++){

            if (1)  /* afficher tous les numeros de table */
                /* pour afficher un numero de table sur 5, utiliser la ligne ci-dessous */
                /* if (((i % 5) == 4) || (i == 0)) */
                sprintf(tampon, "%2ld %s", i+1 , coupon(liste[i].j1, liste[i].j2, liste[i].score));
            else
                sprintf(tampon, "   %s" , coupon(liste[i].j1, liste[i].j2, liste[i].score));
            more_line(tampon);
        }
    }
#else
    for (i = 0; i < nb_paires; i++)
        more_line(coupon(liste[i].j1, liste[i].j2, liste[i].score));
#endif
    free(liste);

    /* Puis les joueurs non apparies */
    more_line("");
    for (i = 0; i < joueurs_inscrits->n; i++)
        if (present[i]) {
            j = (joueurs_inscrits->liste)[i];
            if (polarite(j->numero) == 0)
                more_line(coupon(j, NULL, SCORE_INCONNU));
        }
    /* Termine */
    more_close();
    /* Creons les numeros de tables, si ce n'est deja fait */
    numeroter_les_tables();
}


/*  sauvegardes dans les fichiers "ronde###.txt"  */

void sauver_fichier_appariements() {
    char    *filename;
    long     i;

    if (sauvegarde_fichier_appariements) {
        filename = nom_fichier_numerote(nom_fichier_appariements, ronde+1);
        affiche_appariements(filename,0);
        if (impression_automatique)
            for (i = 0; i < nb_copies_impression; i++)
                imprime_fichier(filename);
    }

	/* Faut-il generer le fichier XML des resultats ? */
	if (generer_fichiers_XML) {
		creer_ronde_XML();
	}
    printf("*ICI apres XML*\n");fflush(stdout);
}
