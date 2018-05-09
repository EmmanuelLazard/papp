/*
 * crosstable.c: pour afficher et sauvegarder un tableau avec tous les resulats
 * ainsi qu'une page web avec un tableau complet des resultats
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 19/11/2007 : v1.32 : La sortie en HTML prend un fichier de template qui definit toute
                     la page HTML. On doit y trouver la definition de la cellule individuelle
                     ainsi que l'emplacement de la crosstable. Nouvelles fonction 'RecordCell()',
                     'ChercherFichierCellTMPL()' et 'void OutputCrosstable()'.
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 12/02/2007 : Modification de 'sortie_tableau_croise_HTML()', 'sortie_tableau_croise_texte()'
 *                   pour qu'ils affichent le nom du tournoi au debut.
 * (EL) 02/02/2007 : changement du type de 'Departage' en double
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include "changes.h"
#include "global.h"
#include "joueur.h"
#include "couplage.h"
#include "version.h"
#include "more.h"
#include "pions.h"
#include "departage.h"
#include "crosstable.h"



#ifdef ENGLISH

  #define HTML_PROMPT_1   "Enter name of HTML file (return = no save) : "
  #define HTML_PROMPT_2   "Saving crosstable in HTML...\n"
  #define CROSS_PROMPT_1  "Print the ASCII cross-table after which round (1-%ld)? "
  #define CROSS_PROMPT_2  "Enter name of cross-table text file (return = no save) : "
  #define CROSS_APPEND    "Append to existing file (Y/N) ? "
  #define CANT_OPEN       "Can't open file "
  #define BAD_TMPL_FILE   "Bad cell.tmpl file. Cannot find <!--CELL-BEGIN or END-->.\n"
  #define HIT_ANY_KEY     "Hit any key to return to the main menu."
  #define BIP             "BYE"
  #define BIP5            " BYE "

#else

  #define HTML_PROMPT_1   "Entrez le nom du fichier HTML (return = pas de sauvegarde) : "
  #define HTML_PROMPT_2   "Sauvegarde du tableau complet en HTML...\n"
  #define CROSS_PROMPT_1  "Afficher le tableau croise ASCII apres quelle ronde (1-%ld) ? "
  #define CROSS_PROMPT_2  "Entrez le nom du fichier texte (return = pas de sauvegarde) : "
  #define CROSS_APPEND    "Ajout a la fin du fichier (O/N) ? "
  #define CANT_OPEN       "Impossible d'ouvrir le fichier "
  #define BAD_TMPL_FILE   "Mauvais fichier cell.tmpl. <!--CELL-BEGIN or END--> introuvable.\n"
  #define HIT_ANY_KEY     "Pressez une touche pour revenir au menu principal."
  #define BIP             "BIP"
  #define BIP5            " BIP "

#endif


/* Variables globales pour stocker tous les resultats */

typedef struct {
    long      ronde;           /* numero de la ronde */
    long      table;           /* numero de la table ou a eu lieu la partie */
    long      present ;        /* 1 si le joueur etait present a cette ronde, 0 sinon */
    long      bip ;            /* 1 si le joueur a joue contre bip, 0 sinon */
    long      adv ;            /* numero FFO (Elo) de l'adversaire */
    pions_t  pions ;          /* score de la partie, du point de vue du joueur */
    char     couleur ;        /* couleur dans la partie */
    long      points ;         /* 2=victoire, 1=nulle, 0=defaite */
    long      cumul_points ;   /* depuis le debut du tournoi */
    long      rang;            /* rang dans le classement : 1=premier, 2=deuxieme, etc. */
} structure_ronde ;

#define NOIR couleur_1[0]
#define BLANC couleur_2[0]
#define JOUEUR_INCONNU -1

#define TAILLE_MAX_CELLULE 10000   /* Taille maximum de la cellule utilise pour chaque case du tableau HTML */
#define TAILLE_MAX_LIGNE    2500   /* Longueur max d'une ligne du fichier HTML */

/* Les mots-clefs du fichier de template pour la crosstable */
#define MC_CROSSTABLE "$PAPP_CROSSTABLE"
#define CELLBEGIN "<!-- CELL-BEGIN -->\n"
#define CELLEND   "<!-- CELL-END -->\n"

typedef struct {
    long                 Num_FFO ;
    structure_ronde     TabRondes[NMAX_RONDES] ;
    long                 Total_Points ;
    pions_t             Total_Pions ;
    long                 Bucholtz ;
    double              Departage ;
} structure_joueur;


structure_joueur *TabJoueurs; /* tableau des joueurs du tournoi */
long partial_ronde;            /* utilisee pour trier les joueurs dans compar() */


/* prototypes des fonctions locales */

long local_trouver_joueur(long num_FFO);
long PresentToutTournoi(long num_FFO);
long Point_Par_Partie(pions_t v);
void local_calcul_departage(long rr);
void recuperation_resultats(void);
int  compar (const void *p, const void *q);
long score_de_machin_contre_bidule(long num_ffo_1, long num_ffo_2);
void calculer_classements_jusque(long quelle_ronde);
void RemplaceStr(char *str, char *coupon, char coul, char *adv, char *nom_adv,
                 char *mon_nom, char *score_partie, char *score_partie_relatif,
                 char *my_score, char *opp_score, long pts, long cumul_pts,
                 char *ronde, char *table,char *rang);
void RemplaceCellule (char *the_template, char *cellule, structure_ronde *pr, structure_joueur *pj);
FILE *GenerateCellFile(void);
void RecordCell(char [],  long, FILE *) ;
short ChercherFichierCellTMPL(char [], long) ;
void OutputCrosstable(char [], FILE *) ;
void TraiterLigneHTML(char []) ;
void sortie_tableau_croise_HTML(FILE *out);
void sortie_tableau_croise_texte(long ronde_aff, FILE *fp);





/* long local_trouver_joueur(long num_FFO)
 *
 * Trouve le numero d'un joueur dans le tableau local de tous les resultats
 * a partir de son numero FFO. Renvoie le numero ou JOUEUR_INCONNU si on ne le
 * trouve pas.
 *
 */
long local_trouver_joueur(long num_FFO) {
    long i ;

    assert(num_FFO >= 0) ;
    if (!joueurs_inscrits->n || !num_FFO)
        return JOUEUR_INCONNU ;
    for (i = 0 ; i < joueurs_inscrits->n  ; i++)
        if (TabJoueurs[i].Num_FFO == num_FFO) return i ;
    return JOUEUR_INCONNU ;
}


/* long PresentToutTournoi(long num_FFO)
 *
 * Indique si un joueur a fait tout le tournoi : 1 si vrai, 0 sinon.
 *
 */
long PresentToutTournoi(long num_FFO) {
    long i, n ;
    structure_joueur *pj ;

    assert(num_FFO >= 0) ;
    if (!num_FFO)
        return 0 ;
    n = local_trouver_joueur(num_FFO) ;
    if (n == JOUEUR_INCONNU)
        return 0 ;
    pj = &TabJoueurs[n] ;
    for (i = 0 ; i < ronde ; i++)
        if (!pj->TabRondes[i].present)
            return 0 ;
    return 1 ;
}


/* long Point_Par_Partie(long num_FFO)
 *
 * Renvie le nombre de point par partie :
 * victoire = 2, nulle = 1, defaite = 0
 *
 */
long Point_Par_Partie(pions_t v) {
    assert(SCORE_EST_LEGAL(v)) ;
    if (EST_UNE_VICTOIRE(v))
        return 2 ;
    else if (EST_UNE_DEFAITE(v))
        return 0 ;
    else
        return 1 ;
}


/* long score_de_machin_contre_bidule(long num_ffo_1, long num_ffo_2)
 *
 * Renvoie le score, en demi-points, du joueur num_ffo_1 contre
 * le joueur num_ffo_2, pendant tout le tournoi.
 *
 * Si les deux joueurs n'ont pas joue ensemble, on renvoie -1.
 *
 */
long score_de_machin_contre_bidule(long num_ffo_1, long num_ffo_2) {
    long n1, n2, i, somme;
    long ont_joue_ensemble = 0;
    structure_joueur *pj ;
    structure_ronde *pr ;

    assert(num_ffo_1 >= 0);
    assert(num_ffo_2 >= 0);

    n1 = local_trouver_joueur(num_ffo_1);
    n2 = local_trouver_joueur(num_ffo_2);

    if ((n1 == JOUEUR_INCONNU) || (n2 == JOUEUR_INCONNU))
        return -1;

    somme = 0;
    pj = &TabJoueurs[n1] ;
    for (i = 0 ; i < ronde ; i++) {
        pr = &pj->TabRondes[i] ;
        if ((pr->adv == num_ffo_2) && !COUPON_EST_VIDE(pr->pions)) {
            ont_joue_ensemble = 1;
            somme += Point_Par_Partie(pr->pions);
        }
    }

    if (!ont_joue_ensemble)
        return -1;
    else
        return somme;
}



/* void local_calcul_departage(long rr)
 *
 * Calcul du departage apres la ronde rr (>=0)
 * Appelle la fonction DepartageJoueur() (departage.c)
 * pour tous les joueurs.
 */
void local_calcul_departage(long rr) {
    long i, j, n;
    BQ_resultats TabJ ;
    structure_joueur *pj ;
    structure_ronde *pr ;

    assert ((rr >= 0) && (rr < ronde));
    for (i = 0 ; i < joueurs_inscrits->n ; i++) {
        pj = &TabJoueurs[i] ;
        TabJ.points_joueur = pj->TabRondes[rr].cumul_points ;
        for (j = 0 ; j <= rr ; j++) {
            pr = &pj->TabRondes[j] ;
            TabJ.score[j] = pr->pions ;
            if ((!pr->present) || (pr->bip)) {
                TabJ.adv[j] = 0 ;
                TabJ.points_adv[j] = 0 ;
            } else {
                TabJ.adv[j] = pr->adv ;
                n = local_trouver_joueur(pr->adv) ;
                TabJ.points_adv[j] = TabJoueurs[n].TabRondes[rr].cumul_points ;
            }
        }
        DepartageJoueur(&TabJ, rr, &pj->Departage, &pj->Total_Pions, &pj->Bucholtz) ;
    }
}



/* void recuperation_resultats(void)
 *
 * Recuperation de tous les resultats deja connus
 * et remplissage des structures pour l'affichage
 * du tableau complet et de la page html.
 *
 */
void recuperation_resultats(void) {
    long i,j, Num_n1, Num_n2, n1, n2, num_elo, nro_table;
    pions_t v ;
    structure_joueur *pj ;
    structure_ronde *pr ;

/* Initialisation des variables */
    for (i = 0 ; i < joueurs_inscrits->n ; i++) {
        pj = &TabJoueurs[i] ;
        pj->Num_FFO = joueurs_inscrits->liste[i]->numero ;
        pj->Total_Points = pj->Bucholtz = 0 ;
        pj->Total_Pions = ZERO_PION ;
        pj->Departage = 0.0 ;
        for (j = 0 ; j < ronde ; j++) {
            pr = &(pj->TabRondes[j]) ;
            pr->ronde = j;
            pr->table = 0;
            pr->present = joueur_etait_present(pj->Num_FFO, j) ;
            pr->bip = pr->adv = pr->points = pr->cumul_points = 0 ;
            pr->couleur = '\0' ;
            pr->pions = SCORE_INCONNU ;
            pr->rang = 0;
        }
    }
/* Recuperation des resultats ronde par ronde */
    for (i = 0 ; i < ronde ; i++) {
        iterer_ronde(i);
        while (couple_suivant(&n1,&n2,&v)) {
            assert(SCORE_EST_LEGAL(v)) ;
            Num_n1 = local_trouver_joueur(n1) ;
            Num_n2 = local_trouver_joueur(n2) ;
            nro_table = numero_de_table(i, n1, n2);
            /* Donnees pour Noir */
            pj = &TabJoueurs[Num_n1] ;
            pr = &(pj->TabRondes[i]) ;
            pr->ronde = i;
            pr->table = nro_table;
            pr->adv = n2 ;
            pr->pions = v ;
            pr->couleur = NOIR ;
            pr->points = Point_Par_Partie(v) ;
            pr->cumul_points = (i == 0 ? pr->points : pr->points + pj->TabRondes[i-1].cumul_points) ;
            pj->Total_Points += pr->points ;
            AJOUTE_SCORE(pj->Total_Pions, (v));
            /* Donnees pour Blanc */
            pj = &TabJoueurs[Num_n2] ;
            pr = &(pj->TabRondes[i]) ;
            pr->ronde = i;
            pr->table = nro_table;
            pr->adv = n1 ;
            pr->pions = SCORE_ADVERSE(v) ;
            pr->couleur = BLANC ;
            pr->points = Point_Par_Partie(SCORE_ADVERSE(v)) ;
            pr->cumul_points = (i == 0 ? pr->points : pr->points + pj->TabRondes[i-1].cumul_points) ;
            pj->Total_Points += pr->points ;
            AJOUTE_SCORE(pj->Total_Pions, SCORE_ADVERSE(v));
        }
        /* Qui a joue contre Bip ? */
        for (j = 0 ; j < joueurs_inscrits->n; j++) {
            pj=&TabJoueurs[j] ;
            num_elo = pj->Num_FFO ;
            pr = &TabJoueurs[j].TabRondes[i] ;
            if (!pr->present) {
                pr->cumul_points = (i == 0 ? 0 : pj->TabRondes[i-1].cumul_points) ;
                pr->couleur = '-' ;
            }
            if (pr->present && (polar2(num_elo, i) == 0)) {
                pr->bip = 1 ;
                pr->pions = SCORE_ADVERSE(score_bip) ;
                pr->couleur = '-' ;
                if (EST_UNE_DEFAITE(score_bip))
                    pr->points = 2 ;
                else if (EST_UNE_VICTOIRE(score_bip))
                    pr->points = 0 ;
                else
                    pr->points = 1 ;
                pr->cumul_points = (i == 0 ? pr->points : pr->points+pj->TabRondes[i-1].cumul_points) ;
                pj->Total_Points += pr->points ;
                AJOUTE_SCORE(TabJoueurs[j].Total_Pions, SCORE_ADVERSE(score_bip));
            }
        }
    }
}


/* int compar (const void *p, const void *q)
 *
 * Fonction utilisee par qsort pour trier les joueurs pour le classement.
 * La variable globale 'partial_ronde' donne le numero de la ronde dont
 * on veut le classement trie.
 *
 * IMPORTANT : cet ordre de tri doit etre le meme que celui implemente
 *             dans la fonction tri_joueurs() de entrejou.c.
 *
 */
int compar (const void *p, const void *q) {
    structure_joueur *pj = (structure_joueur *) p ;
    structure_joueur *pq = (structure_joueur *) q ;

    if (pj->TabRondes[partial_ronde].cumul_points > pq->TabRondes[partial_ronde].cumul_points) return -1 ;
    if (pj->TabRondes[partial_ronde].cumul_points < pq->TabRondes[partial_ronde].cumul_points) return 1 ;
/*  if (SCORE_STRICT_GRAND(pj->Departage, pq->Departage)) return -1 ;
    if (SCORE_STRICT_GRAND(pq->Departage, pj->Departage)) return  1 ;
*/
    if (pj->Departage > pq->Departage) return -1 ;
    if (pq->Departage > pj->Departage) return  1 ;
    if (SCORE_STRICT_GRAND(pj->Total_Pions, pq->Total_Pions)) return -1 ;
    if (SCORE_STRICT_GRAND(pq->Total_Pions, pj->Total_Pions)) return  1 ;

    return compare_chaines_non_sentitif(trouver_joueur(pj->Num_FFO)->nom,
                                        trouver_joueur(pq->Num_FFO)->nom);

    /*if (trouver_joueur(pj->Num_FFO)->nom < trouver_joueur(pq->Num_FFO)->nom)
        return -1 ;
    else
        return 1 ;
    */
}



/* void RemplaceStr(char *str, char *coupon, char coul, char *adv, char *nom_adv,
 *               char *mon_nom, char *score_partie, char *score_partie_relatif,
 *               char *my_score, char *opp_score, long pts, long cumul_pts,
 *               char *ronde, char *table, char *rang)
 *
 * Remplace dans la chaine *str les codes speciaux par les valeurs passees
 * en parametre pour l'affichage d'une cellule dans le tableau HTML.
 * Les codes sont : PAPP_COLOR         = couleur
 *                  PAPP_OPP           = numero de l'adversaire dans le tableau
 *                  PAPP_OPP_NAME[n]   = n premiers caracteres du nom de l'adversaire
 *                  PAPP_MY_NAME[n]    = n premiers caracteres de mon nom
 *                  PAPP_BLACK_NAME[n] = n premiers caracteres du nom de Noir
 *                  PAPP_WHITE_NAME[n] = n premiers caracteres du nom de Blanc
 *                  PAPP_COUPON        = coupon de la partie (eg SEELEY Ben 12/52 SHAMAN David)
 *                  PAPP_SCORE         = score de la partie, vue de joueur (eg 52/12)
 *                  PAPP_SCORE_RELATIF = score de la partie en relatif (eg +40)
 *                  PAPP_MY_SCORE      = mon nombre de pions (eg 52)
 *                  PAPP_OPP_SCORE     = pions de l'adversaire (eg 12)
 *                  PAPP_PTS           = points gagnes dans la partie (0, 0.5 ou 1)
 *                  PAPP_TOTAL         = total de points
 *                  PAPP_RONDE         = ronde de la partie
 *                  PAPP_TABLE         = table de la partie
 *                  PAPP_RANG          = classement du joueur apres cette partie.
 *
 * Attention, on doit avoir strlen(adv)<=3, et la chaine *str
 * doit faire moins de TAILLE_MAX_CELLULE octets.
 *
 */
void RemplaceStr(char *str, char *coupon, char coul, char *adv, char *nom_adv,
                 char *mon_nom, char *score_partie, char *score_partie_relatif,
                 char *my_score, char *opp_score, long pts, long cumul_pts,
                 char *ronde, char *table,char *rang) {
    char *p ;
    char buf[200], chaine_cherchee[50];
    long i, n, len;
    char buffer2[TAILLE_MAX_CELLULE + 300];


    p = strstr(str, "PAPP_COLOR");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_COLOR"));
            *p = *(p+2) = ' ' ;
            *(p+1) = coul ;
            strcpy(p+3, buffer2);
    }

    p = strstr(str, "PAPP_SCORE_RELATIF");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_SCORE_RELATIF"));
            strcpy(buf, score_partie_relatif) ;
		/* On aimerait supprimer le 0 en tete des nombres inferieurs a 10. */
			if ((buf[1] == '0') && strlen(buf)>2) {
				memmove(&buf[1], &buf[2], strlen(buf)-1) ;
			}
            for (i = strlen(buf); i < 12 ; i++)
               buf[i] = ' ' ;
            len = le_max_de(strlen(score_partie_relatif), 3);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    p = strstr(str, "PAPP_SCORE");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_SCORE"));
            strcpy(buf, score_partie) ;
            for (i = strlen(buf); i < 12 ; i++)
               buf[i] = ' ' ;
            len = le_max_de(strlen(score_partie), 3);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    p = strstr(str, "PAPP_MY_SCORE");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_MY_SCORE"));
            strcpy(buf, my_score) ;
            for (i = strlen(buf); i < 12 ; i++)
               buf[i] = ' ' ;
            len = le_max_de(strlen(my_score), 2);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    p = strstr(str, "PAPP_OPP_SCORE");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_OPP_SCORE"));
            strcpy(buf, opp_score) ;
            for (i = strlen(buf); i < 12 ; i++)
               buf[i] = ' ' ;
            len = le_max_de(strlen(opp_score), 2);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    p = strstr(str, "PAPP_PTS");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_PTS"));
            switch(pts) {
                    case 0 : strncpy(p, " 0 ", 3) ;
                                    break ;
                    case 1 : strncpy(p, "0.5", 3) ;
                                    break ;
                    case 2 : strncpy(p, " 1 ", 3) ;
                                    break ;
                    default: strncpy(p, "   ", 3) ;
                    break ;
            }
            strcpy(p+3, buffer2);
    }

    p = strstr(str, "PAPP_TOTAL") ;
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_TOTAL"));
            sprintf(buf, "%ld%s", cumul_pts/2, (cumul_pts%2 ? ".5" : "")) ;
            strcat(buf, "      ") ;
            strncpy(p, buf, 5) ;
            strcpy(p+5, buffer2);
    }

    p = strstr(str, "PAPP_COUPON");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_COUPON"));
            strcpy(buf, coupon) ;
            for (i = strlen(coupon); i < 200 ; i++)
               buf[i] = ' ' ;
            len = strlen(coupon);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    /* on cherche les mots clefs "PAPP_MY_NAME[1]", "PAPP_MY_NAME[2]", etc. */
    for (n = 1; n < 50; n++) {
        sprintf(chaine_cherchee, "PAPP_MY_NAME[%ld]", n);
        p = strstr(str, chaine_cherchee);
        if (p) {
            strcpy(buffer2, p+(long)strlen(chaine_cherchee));
            strcpy(buf, mon_nom) ;
            for (i = strlen(buf); i < 50 ; i++)
               buf[i] = ' ' ;
            len = le_min_de(strlen(mon_nom), n);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
        }
    }

    /* on cherche les mots clefs "PAPP_OPP_NAME[1]", "PAPP_OPP_NAME[2]", etc. */
    for (n = 1; n < 50; n++) {
        sprintf(chaine_cherchee, "PAPP_OPP_NAME[%ld]", n);
        p = strstr(str, chaine_cherchee);
        if (p) {
            strcpy(buffer2, p+(long)strlen(chaine_cherchee));
            strcpy(buf, nom_adv) ;
            for (i = strlen(buf); i < 50 ; i++)
               buf[i] = ' ' ;
            len = le_min_de(strlen(nom_adv), n);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
        }
    }



    /* on cherche les mots clefs "PAPP_BLACK_NAME[1]", "PAPP_BLACK_NAME[2]", etc. */
    for (n = 1; n < 50; n++) {
        sprintf(chaine_cherchee, "PAPP_BLACK_NAME[%ld]", n);
        p = strstr(str, chaine_cherchee);
        if (p) {
            strcpy(buffer2, p+(long)strlen(chaine_cherchee));
            if (coul == NOIR) {
                strcpy(buf, mon_nom) ;
                for (i = strlen(buf); i < 50 ; i++)
                    buf[i] = ' ' ;
                len = le_min_de(strlen(mon_nom), n);
            }
            else {
                strcpy(buf, nom_adv) ;
                for (i = strlen(buf); i < 50 ; i++)
                    buf[i] = ' ' ;
                len = le_min_de(strlen(nom_adv), n);
            }
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
        }
    }

    /* on cherche les mots clefs "PAPP_WHITE_NAME[1]", "PAPP_WHITE_NAME[2]", etc. */
    for (n = 1; n < 50; n++) {
        sprintf(chaine_cherchee, "PAPP_WHITE_NAME[%ld]", n);
        p = strstr(str, chaine_cherchee);
        if (p) {
            strcpy(buffer2, p+(long)strlen(chaine_cherchee));
            if (coul == BLANC) {
                strcpy(buf, mon_nom) ;
                for (i = strlen(buf); i < 50 ; i++)
                    buf[i] = ' ' ;
                len = le_min_de(strlen(mon_nom), n);
            }
            else {
                strcpy(buf, nom_adv) ;
                for (i = strlen(buf); i < 50 ; i++)
                    buf[i] = ' ' ;
                len = le_min_de(strlen(nom_adv), n);
            }
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
        }
    }

    p = strstr(str, "PAPP_OPP");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_OPP"));
            *p = adv[0] ;
            if (strlen(adv) == 1)
                    *(p+1) = ' ' ;
            else
                    *(p+1) = adv[1] ;
            if (strlen(adv) < 3)
                    *(p+2) = ' ' ;
            else
                    *(p+2) = adv[2] ;
            strcpy(p+3, buffer2);
    }


    p = strstr(str, "PAPP_RONDE");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_RONDE"));
            strcpy(buf, ronde) ;
            len = strlen(ronde);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    p = strstr(str, "PAPP_TABLE");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_TABLE"));
            strcpy(buf, table) ;
            len = strlen(table);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }

    p = strstr(str, "PAPP_RANG");
    if (p) {
            strcpy(buffer2, p+(long)strlen("PAPP_RANG"));
            strcpy(buf, rang);
            len = strlen(rang);
            strncpy(p, buf, len);
            strcpy(p+len, buffer2);
    }
}



/* void RemplaceCellule (char *the_template, char *cellule, structure_ronde *pr, structure_joueur *pj)
 *
 * Inscrit les infos du joueur (pour une ronde donnee) dans
 * une copie de la cellule 'the_template'.
 *
 */
void RemplaceCellule (char *the_template, char *cellule, structure_ronde *pr, structure_joueur *pj) {

    char score_partie[10], score_partie_relatif[10], my_score[10], opp_score[10];
    char num_adv[5], nom_adv[50], mon_nom[50], nro_ronde[5], nro_table[5], rang[5];
    char coupon[200];
    long nb_chiffres_des_rondes, nb_chiffres_des_tables;
    long n , temp;


    strcpy(cellule, the_template) ;

    /* creation des infos generales de la partie */
    n = local_trouver_joueur(pr->adv) ;
    sprintf(rang, "%ld", pr->rang);
    sprintf(mon_nom,"%s", trouver_joueur(pj->Num_FFO)->nom);
    nb_chiffres_des_rondes = 2;
    sprintf(nro_ronde, "%0*ld", (int)nb_chiffres_des_rondes, pr->ronde + 1);

    /* deux cas particuliers : si le joueur n'a pas joue,
     * ou s'il a joue contre Bip */
    if (n == JOUEUR_INCONNU)
        n = -1 ;
    if (pr->bip) {
        RemplaceStr(cellule, BIP, '-', "-", BIP, mon_nom,
                    BIP, BIP, BIP, BIP,
                    pr->points, pr->cumul_points, "-", "-", rang) ;
    } else if (!pr->present) {
        RemplaceStr(cellule, " --- ", '-', "-", "-", mon_nom,
                    "---", "---", "--", "--",
                    pr->points, pr->cumul_points, "-", "-", rang) ;
    } else { /* cas normal */

        /* creation du numero de l'aversaire et de son nom */
        sprintf(num_adv, "%ld", n+1) ;
        sprintf(nom_adv,"%s", trouver_joueur(pr->adv)->nom);

        /* creation des numeros de table */
        nb_chiffres_des_tables = (joueurs_inscrits->n < 20 ? 1 :
                                 (joueurs_inscrits->n < 200 ? 2 : 3));
        sprintf(nro_table, "%0*ld", (int)nb_chiffres_des_tables, pr->table);

        /* creation des scores (tjrs du point de vue du joueur) */
        temp = aff_diff_scores;
        aff_diff_scores = 0; sprintf(score_partie, "%s", fscore(pr->pions));
        aff_diff_scores = 1; sprintf(score_partie_relatif, "%s", fscore(pr->pions));
        sprintf(my_score, "%s", pions_en_chaine(pr->pions));
        sprintf(opp_score, "%s", pions_en_chaine(SCORE_ADVERSE(pr->pions)));
        aff_diff_scores = temp;

        /* creation du coupon */
        temp = aff_diff_scores;
        aff_diff_scores = 0;
        if (pr->couleur == NOIR)
            sprintf(coupon, "%s  %s  %s", mon_nom, fscore(pr->pions), nom_adv);
        else
            sprintf(coupon, "%s  %s  %s", nom_adv, fscore(SCORE_ADVERSE(pr->pions)), mon_nom);
        aff_diff_scores = temp;


        RemplaceStr(cellule, coupon, pr->couleur, num_adv, nom_adv, mon_nom,
                    score_partie, score_partie_relatif, my_score, opp_score,
                    pr->points, pr->cumul_points, nro_ronde, nro_table, rang) ;
    }
}


/* FILE *GenerateCellFile(void)
 *
 * Cree un fichier Cell.tmpl pour l'affichage d'un fichier de crosstable HTML
 * ce fichier est cree s'il n'existe pas dans le repertoire.
 *
 */
FILE *GenerateCellFile(void) {
    FILE *fp ;

    if (!(fp = myfopen_dans_sous_dossier("cell.tmpl", "w", "", 0, 0)))
        return NULL ;
	printf("FICHIER TEMPLATE GENERE\n") ;

    fprintf(fp, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n") ;
    fprintf(fp, "<html>\n<head>\n<title>PAPP_TOURNAMENT_NAME : #PAPP_RONDE</title>\n") ;
	fprintf(fp, "<style type=\"text/css\">\n"
/* table#crosstable : the table itself */
				"/* table#crosstable : the table itself */\n"
				"table#crosstable {\n"
				"\tborder: 2px solid black;\n"
				"\tborder-collapse: collapse;\n"
				"\tmargin-left: auto;\n"
				"\tmargin-right: auto;\n"
				"\tpadding: 0px;\n"
				"\tbackground-color: #ffffff;\n"
				"\tfont-family: Verdana, Arial, Helvetica, sans-serif;\n}\n") ;
/* crosstable_caption : the text for the caption */
	fprintf(fp, "/* crosstable_caption : the text for the caption */\n"
				".crosstable_caption {\n"
				"\tfont-weight: bold;\n"
				"\tfont-size: 150%%;\n"
				"\ttext-align: center;\n"
				"\tmargin-bottom: 15px;\n}\n") ;
/* td : each cell of the main table */
	fprintf(fp, "/* td : each cell of the main table */\n"
				"td	{\n"
				"\tpadding: 0px;\n"
				"\tborder: 1px solid black;\n}\n") ;
/* td.crosstable_title : the title line */
	fprintf(fp, "/* td.crosstable_title : the title line */\n"
				"td.crosstable_title {\n"
				"\ttext-align: center;\n"
				"\tfont-weight: bold;\n"
				"\tpadding: 5px 5px 5px 5px;\n}\n") ;
/* td.crosstable_name : the name column */
	fprintf(fp, "/* td.crosstable_name : the name column */\n"
				"td.crosstable_name {\n"
				"\ttext-align: left;\n"
				"\tpadding: 0px 10px 0px 10px ;\n"
				"\tfont-weight: bold;\n}\n") ;
/* td.crosstable_country : the country column */
	fprintf(fp, "/* td.crosstable_country : the country column */\n"
				"td.crosstable_country {\n"
				"\ttext-align: center;\n"
				"\tpadding: 0px 3px 0px 3px ;\n"
				"\tfont-size: 90%%;\n"
				"\tfont-weight: bold;\n}\n") ;
/* td.crosstable_points : the total number of points column */
	fprintf(fp, "/* td.crosstable_points : the total number of points column */\n"
				"td.crosstable_points {\n"
				"\tpadding: 0px 3px 0px 3px ;\n"
				"\ttext-align: center;\n"
				"\tfont-weight: bold;\n"
				"\tfont-size: 115%%;\n}\n") ;
/* td.crosstable_tiebreak : the tiebreak column */
	fprintf(fp, "/* td.crosstable_tiebreak : the tiebreak column */\n"
				"td.crosstable_tiebreak {\n"
				"\ttext-align: center;\n"
				"\tpadding: 0px 3px 0px 3px ;\n"
				"\tfont-size: 90%%;\n"
				"\tfont-weight: normal;\n}\n") ;
/* table.crosstable_cell : each individual cell view as a table */
	fprintf(fp, "/* table.crosstable_cell : each individual cell view as a table */\n"
				"table.crosstable_cell {\n"
				"\tmargin-left: auto;\n"
				"\tmargin-right: auto;\n"
				"\ttext-align: center;\n}\n") ;
/* td.cell : each cell inside each individual cell */
	fprintf(fp, "/* td.cell : each cell inside each individual cell */\n"
				"td.cell {\n"
				"\tborder: 0px solid black;\n}\n") ;
	fprintf(fp, "</style>\n") ;
	fprintf(fp, "</head>\n<body bgcolor=\"#FFFFFF\" vlink=\"#CC6600\">\n\n\n\n") ;
	fprintf(fp, CELLBEGIN) ;
    fprintf(fp, "<table class=\"crosstable_cell\">\n") ;
    fprintf(fp, "<tr><td class=\"cell\" align=\"left\"><span STYLE=\"font-size: 90%%; color: #990000;\">PAPP_COLOR</span></td>\n"
				"    <td class=\"cell\" align=\"right\"><span STYLE=\"font-size: 90%%; color: #006600;\">PAPP_OPP</span></td></tr>\n") ;
    fprintf(fp, "<tr><td colspan=\"2\" class=\"cell\" align=\"center\"><span STYLE=\"font-weight: bold; text-align: center; color: #000000;\">PAPP_TOTAL</span></td></tr>\n") ;
    fprintf(fp, "<tr><td class=\"cell\" align=\"left\"><span STYLE=\"font-size: 90%%; color: #000000;\">PAPP_PTS</span></td>\n"
				"    <td class=\"cell\" align=\"right\"><span STYLE=\"font-size: 90%%; color: #660000;\">PAPP_SCORE</span></td></tr>\n") ;
    fprintf(fp, "</table>\n") ;
	fprintf(fp, CELLEND) ;
	fprintf(fp, "\n<div class=\"crosstable_caption\">\n"
				"PAPP_TOURNAMENT_NAME<br>\n"
#ifdef ENGLISH
				"Standings after round PAPP_RONDE<br>\n"
#else
				"Classement apr&egrave;s la ronde PAPP_RONDE<br>\n"
#endif
				"</div>\n" MC_CROSSTABLE "\n") ;
	fprintf(fp, "</body>\n</html>\n") ;
    fclose(fp) ;
    return myfopen_dans_sous_dossier("cell.tmpl","r", "", 0, 0) ;
}

/* Enregistre les lignes definissant une cellule du tableau  */
void RecordCell(char cellule[],  long taille, FILE *file) {
	short begin=0 ;
	char *buf = malloc(taille+1) ;
	cellule[0] = '\0' ;
	printf("RECORD CELL DANS TEMPLATE\n") ;
	while ( fgets(buf, taille, file) ) {
		if (!begin) {
			if (strstr(buf, CELLBEGIN)) /* le debut de la def. */
				begin = 1 ;
			continue ;
		} else { /* on est dans la definition */
			if (strstr(buf, CELLEND)) { /* fin de la def . */
				begin = 0 ;
				break ;
			}
			/* On recorde */
			strncat(cellule, buf, taille-strlen(cellule)) ;
		}
	}
	cellule[taille-1] = '\0' ;
}

/* Essaie de lire le fichier de template pour la crosstable  */
/* Si non trouve, genere un fichier cell.tmpl par defaut.    */
/* Ensuite, lit le fichier pour remplir le champ cellule     */
/* (dont la taille max est donnee)                           */
/* avec la definition de la cellule                          */
/* renvoie 0 si erreur                                       */
short ChercherFichierCellTMPL( char cellule[], long taille_cellule ) {
	FILE *cellulefile ;
	short begin, end ;
	char buf[TAILLE_MAX_LIGNE] ;

	if ( (cellulefile = myfopen_dans_sous_dossier("cell.tmpl","r", "", 0, 0)) != 0 ) {
		printf("TEMPLATE TROUVE\n") ;
		begin = end = 0 ;
		while ( fgets(buf, TAILLE_MAX_LIGNE, cellulefile) ) {
			if ( strstr(buf, CELLBEGIN) )
				 begin = 1; /* Debut de la definition de la cellule */
			if ( (strstr(buf, CELLEND)) && begin ) {
				end = 1 ; /* Fin de la definition de la cellule */
				break ;
			}
		}
		if (begin && end) { /* Il y avait une def. de cellule */
			printf("DEF. CELL TROUVEE\n") ;
			fseek(cellulefile, 0, SEEK_SET) ;
			RecordCell(cellule, taille_cellule, cellulefile) ;
			fclose(cellulefile) ;
			return 1 ;
		}
		printf(BAD_TMPL_FILE) ;
		return 0 ;
	} else if ( (cellulefile = GenerateCellFile()) != 0) {
		RecordCell(cellule, taille_cellule, cellulefile) ;
		fclose(cellulefile) ;
		return 1 ;
	} else
		return 0 ;
}

/* Generation du code pour la crosstable */
/* A inserer dans un fichier HTML         */
void OutputCrosstable(char cellule[], FILE *out) {
    char cellule2[TAILLE_MAX_CELLULE + 100] ;
    structure_joueur *pj ;
    structure_ronde *pr;
	long i, j ;

#ifdef ENGLISH
    fprintf(out, "<table id=\"crosstable\">\n") ;
/*    fprintf(out, "<caption>%s<br>Standings after %ld round%c<br>&nbsp</caption>\n",nom_du_tournoi, ronde, (ronde==1?' ':'s')); */

    fprintf(out, "<tr>\n"
				 "<td class=\"crosstable_title\">No.</td>\n"
			     "<td class=\"crosstable_title\" align=\"left\">Name</td>\n"
				 "<td class=\"crosstable_title\">Country</td>\n") ;
    for (j = 0 ; j < ronde ; j++)
        fprintf(out, "<td class=\"crosstable_title\">r. %ld</td>\n", j+1) ;
    fprintf(out, "<td class=\"crosstable_title\">Points</td>\n"
			     "<td class=\"crosstable_title\">Tie-break</td>\n</tr>\n") ;
    fflush(out) ;
#else
    fprintf(out, "<table id=\"crosstable\">\n") ;
/*    fprintf(out, "<caption><%s<br>Classement apr&egrave;s %ld ronde%c<br>&nbsp</caption>\n", nom_du_tournoi,ronde, (ronde==1?' ':'s'));*/

    fprintf(out, "<tr>\n"
			"<td class=\"crosstable_title\">No.</td>\n"
			"<td class=\"crosstable_title\" align=\"left\">Nom</td>\n"
			"<td class=\"crosstable_title\">Pays</td>\n") ;
    for (j = 0 ; j < ronde ; j++)
        fprintf(out, "<td class=\"crosstable_title\">r. %ld</td>\n", j+1) ;
    fprintf(out, "<td class=\"crosstable_title\">Points</td>\n"
			"<td class=\"crosstable_title\">D&eacute;partage</td>\n</tr>\n") ;
    fflush(out) ;
#endif
    for (i = 0 ; i < joueurs_inscrits->n ; i++) {
        pj = &TabJoueurs[i] ;
        fprintf(out, "<tr><td align=\"center\">%ld</td><td class=\"crosstable_name\">%s</td><td class=\"crosstable_country\">%s</td>\n", i+1,
				trouver_joueur(pj->Num_FFO)->nom, trouver_joueur(pj->Num_FFO)->pays);
        for (j = 0 ; j < ronde ; j++) {
            fprintf(out, "<td class=\"crosstable_cell\">\n") ;
            pr = &pj->TabRondes[j] ;
            RemplaceCellule(cellule, cellule2, pr, pj) ;
            fprintf(out, "%s\n</td>\n", cellule2) ;
        }
        fprintf(out, "<td class=\"crosstable_points\">%ld%s</td>", pj->Total_Points/2, (pj->Total_Points%2 ? ".5" : "" ) ) ;
        fprintf(out, "<td class=\"crosstable_tiebreak\">[%s]</td>\n</tr>\n\n", departage_en_chaine(pj->Departage)) ;
    }
    fprintf(out, "</table>\n") ;
}

/* void TraiterLigneHTML(char str[])
 *
 * Remplace, dans une ligne du code HTML de cell.tmpl, les mots-clefs
 * par leur valeur.
 * PAPP_TOURNAMENT_NAME : nom du tournoi
 * PAPP_RONDE : numero de la ronde
 */
void TraiterLigneHTML(char str[]) {
	char *p ;
	char buf_tmp[TAILLE_MAX_LIGNE] ;
	char buf2[TAILLE_MAX_LIGNE] ;
	long len ;

	p = strstr(str, "PAPP_RONDE");
    if (p) {
		strcpy(buf_tmp, p+(long)strlen("PAPP_RONDE"));
		sprintf(buf2, "%ld", ronde) ;
		len = strlen(buf2);
		strncpy(p, buf2, len);
		strcpy(p+len, buf_tmp);
    }
	p = strstr(str, "PAPP_TOURNAMENT_NAME");
    if (p) {
		strcpy(buf_tmp, p+(long)strlen("PAPP_TOURNAMENT_NAME"));
		len = strlen(nom_du_tournoi);
		strncpy(p, nom_du_tournoi, len);
		strcpy(p+len, buf_tmp);
    }
}


/* void sortie_tableau_croise_HTML(FILE *out)
 *
 * Generation d'une page HTML contenant le tableau
 * de tous les resultats connus.
 *
 */
void sortie_tableau_croise_HTML(FILE *out) {
    char cellule[TAILLE_MAX_CELLULE + 100] ;
	short begin=0 ;
	char buf[TAILLE_MAX_LIGNE] ;
    FILE *cellulefile ;


	if (!ChercherFichierCellTMPL(cellule, TAILLE_MAX_CELLULE+100)) { /* on n'a pas trouve un fichier */
		printf(CANT_OPEN ": cell.tmpl file\n");
        return ;
    }
	/* On a donc enregistre une cellule */
	if ((cellulefile = myfopen_dans_sous_dossier("cell.tmpl","r", "", 0, 0)) != 0) {
		while ( fgets(buf, TAILLE_MAX_LIGNE, cellulefile) ) {
			if (begin) {
				if (strstr(buf, CELLEND)) /* la fin de la def. */
					begin = 0 ;
				continue ;
			} else { /* on n'est pas dans la definition */
			if (strstr(buf, CELLBEGIN)) { /* debut de la def . */
				begin = 1 ;
				continue ;
			}
			/* On recopie */
				if (strstr(buf, MC_CROSSTABLE))
					OutputCrosstable(cellule, out) ;
				else {
					TraiterLigneHTML(buf) ; /* remplacement des mots-clefs */
					fputs(buf, out) ;
				}

			}
		}
	} else {
		printf(CANT_OPEN ": cell.tmpl file\n");
	}
}

/* void sortie_tableau_croise_texte(long ronde_aff, FILE *fp)
 *
 * Affiche a l'ecran le tableau complet apres la ronde
 * 'ronde_aff' (>=0) et eventuellement le sauvegarde
 * dans le fichier de pointeur fp (si fp !=NULL).
 *
 */
void sortie_tableau_croise_texte(long ronde_aff, FILE *fp) {
#ifdef ENGLISH
 #define STR_ROUND_1  "round "
 #define STR_ROUND_2  "rounds"
 #define STR_DISCS    "(Discs/SO- colours * fl.)\n"
 #define STR
#else
 #define STR_ROUND_1  "ronde "
 #define STR_ROUND_2  "rondes"
 #define STR_DISCS    "(Pions/SO- couleurs * fl.)\n"
#endif

#define LIM 10
    long i, j, n, m, diff_col, line1, reste ;
    structure_ronde *pr ;
    char PlusMoins[]  = "-=+" ;
    char ZeroEgalUn[] = "0=1" ;
    char Chiffres[]   = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
    char buf[400] ;
    char buf2[400] ;
    char virgule[4] ;
    char col[3] ;
    char c, last_float ;
    char titre[] = "No                                                                                      " ;
    long  v, suisse_simple;
    char *nomTrn ;


    assert ((ronde_aff >= 0) && (ronde_aff < ronde)) ;
    more_init(NULL) ;
    eff_ecran() ;

    /* Pour afficher le nom du tournoi en tete */
    nomTrn = malloc(strlen(nom_du_tournoi)+15) ;
    if (nomTrn != NULL) {
        sprintf(nomTrn, "*** %s ***\n", nom_du_tournoi) ;
        more_line(nomTrn) ;
        if (fp)
            fprintf(fp, "*** %s ***\n", nom_du_tournoi) ;
        free(nomTrn) ;
    }

    for (i = 0; i < joueurs_inscrits->n ; i++) {
        sprintf(buf , "%3ld. %s", i+1, trouver_joueur(TabJoueurs[i].Num_FFO)->nom) ;
        more_line(buf) ;
        if (fp)
            fprintf(fp, "%s\n", buf) ;
    }
    more_line("") ;
    if (fp)
        fprintf(fp,"\n") ;

    if ((joueurs_inscrits->n < 99) && (ronde_aff < 13)) {
        n = (5*ronde_aff-1)/2+3 ;
        m = 5*ronde_aff+9 ;
        strncpy(&titre[n], (ronde_aff == 0 ? STR_ROUND_1 : STR_ROUND_2), 6) ;
        strcpy(&titre[m], "Pts    TB") ;
        more_line(titre) ;
        if (fp)
            fprintf(fp, "%s   " STR_DISCS, titre) ;
        for (i = 0; i < joueurs_inscrits->n ; i++) {
            sprintf(buf, "%2ld ", i+1) ;
            for (j = 0 ; j <= ronde_aff ; j++) {
                pr = &TabJoueurs[i].TabRondes[j] ;
                if (pr->bip)
                    sprintf(buf2, BIP5) ;
                else if (!pr->present)
                    sprintf(buf2, " --- ") ;
                else
                    sprintf(buf2, "%c%c%2ld ", pr->couleur, PlusMoins[pr->points], local_trouver_joueur(pr->adv)+1) ;
                strcat(buf, buf2) ;
            }
            strcpy(virgule, (TabJoueurs[i].TabRondes[ronde_aff].cumul_points%2 ? ".5" : "  ")) ;
            sprintf(buf2, ":%2ld%s [%4s]", TabJoueurs[i].TabRondes[ronde_aff].cumul_points/2, virgule,
                          departage_en_chaine(TabJoueurs[i].Departage)) ;
            strcat(buf, buf2) ;
            more_line(buf) ;
            if (fp) {
                diff_col = 0 ;
                fprintf(fp, "%s ", buf) ;
                strcpy(buf2,"") ;
                for (j = 0 ; j <= ronde_aff ; j++) {
                    c = TabJoueurs[i].TabRondes[j].couleur ;
                    sprintf(col, "%c", c) ;
                    strcat(buf2, col) ;
                    if (c == NOIR)
                        diff_col-- ;
                    else if (c == BLANC)
                        diff_col++ ;
                }
                pr = &TabJoueurs[i].TabRondes[ronde_aff] ;

                if (ronde_aff == 0)
                    last_float = ' ' ;
                else if (!pr->present)
                    last_float = ' ' ;
                else if (pr->bip)
                    last_float = 'd' ;
                else {
                    n = local_trouver_joueur(pr->adv) ;
                    m = TabJoueurs[i].TabRondes[ronde_aff-1].cumul_points - TabJoueurs[n].TabRondes[ronde_aff-1].cumul_points ;
                    if (m>0)
                        last_float = 'd' ;
                    else if (m < 0)
                        last_float = 'u' ;
                    else
                        last_float = ' ' ;
                }
                fprintf(fp, "(%3s/%3ld - %s %2ld * %c)\n", pions_en_chaine(TabJoueurs[i].Total_Pions),
                        TabJoueurs[i].Bucholtz, buf2, diff_col, last_float) ;
            }
        }
    } else {
        line1 = le_min_de(LIM-1, ronde_aff) ;
        n = (3*line1)+4 ;
        m = 6*line1+11 ;
        strncpy(&titre[n], (ronde_aff == 0 ? STR_ROUND_1 : STR_ROUND_2), 6) ;
        strcpy(&titre[m], "Pts    TB") ;
        more_line(titre) ;
        if (fp)
            fprintf(fp, "%s   " STR_DISCS, titre) ;
        for (i = 0; i < joueurs_inscrits->n ; i++) {
            sprintf(buf, "%3ld ", i+1) ;
            line1 = le_min_de(LIM-1, ronde_aff) ;
            for (j = 0 ; j <= line1 ; j++) {
                pr = &TabJoueurs[i].TabRondes[j] ;
                if (pr->bip)
                    sprintf(buf2, " " BIP5) ;
                else if (!pr->present)
                    sprintf(buf2, "  --- ") ;
                else
                    sprintf(buf2, ((joueurs_inscrits->n < 99) ? " %c%c%2ld ": "%c%c%3ld"),
                            pr->couleur, PlusMoins[pr->points], local_trouver_joueur(pr->adv)+1) ;
                strcat(buf, buf2) ;
            }
            strcpy(virgule, (TabJoueurs[i].TabRondes[ronde_aff].cumul_points%2 ? ".5" : "  ")) ;
            sprintf(buf2, ":%2ld%s [%4s]", TabJoueurs[i].TabRondes[ronde_aff].cumul_points/2, virgule,
                          departage_en_chaine(TabJoueurs[i].Departage)) ;
            strcat(buf, buf2) ;
            more_line(buf) ;
            if (fp) {
                diff_col = 0 ;
                fprintf(fp, "%s ", buf) ;
                strcpy(buf2,"") ;
                for (j = 0 ; j <= ronde_aff ; j++) {
                    c = TabJoueurs[i].TabRondes[j].couleur ;
                    sprintf(col, "%c", c) ;
                    strcat(buf2, col) ;
                    if (c == NOIR)
                        diff_col-- ;
                    else if (c == BLANC)
                        diff_col++ ;
                }
                fprintf(fp, "(%3s/%3ld - %s %2ld)\n", pions_en_chaine(TabJoueurs[i].Total_Pions),
                        TabJoueurs[i].Bucholtz, buf2, diff_col) ;
            }
            while (line1 < ronde_aff) {
                reste = le_min_de (LIM, ronde_aff-line1) ;
                sprintf(buf, "    ") ;
                for (j = (line1+1) ; j <= (line1+reste) ; j++) {
                    pr = &TabJoueurs[i].TabRondes[j] ;
                    if (pr->bip)
                        sprintf(buf2, " " BIP5) ;
                    else if (!pr->present)
                        sprintf(buf2, "  --- ") ;
                    else
                        sprintf(buf2, ((joueurs_inscrits->n < 99) ? " %c%c%2ld ": "%c%c%3ld"),
                                pr->couleur, PlusMoins[pr->points], local_trouver_joueur(pr->adv)+1) ;
                    strcat(buf, buf2) ;
                }
                more_line(buf) ;
                if (fp)
                    fprintf(fp, "%s\n", buf) ;
                line1 += LIM ;
            }
        }
    }


    more_line("");
    if (fp) fprintf(fp, "\n") ;


    /* Affichage de la matrice des victoires. Notez que l'on
     * determine d'abord si chaque joueur a gagne au plus une fois
     * contre chacun de ses adversaires, auquel cas on pourra
     * afficher la matrice avec des 0,=,1 plutot qu'avec les
     * scores en demi-points */

    suisse_simple = 1;
    for (i = 0; i < joueurs_inscrits->n ; i++)
        for (j = 0; j < joueurs_inscrits->n ; j++)
            if (score_de_machin_contre_bidule(TabJoueurs[i].Num_FFO, TabJoueurs[j].Num_FFO) > 2)
                suisse_simple = 0;

    sprintf(buf, "%ld  ", 1 + joueurs_inscrits->n);
    m = strlen(buf);

    /* Ecriture de la ligne en haut de la matrice */
    for (j = 0; j < m ; j++)
        buf[j] = ' ';
    for (j = 0; j < joueurs_inscrits->n ; j++)
        buf[m + j] = Chiffres[ (j+1) % 10 ];
    buf[m + joueurs_inscrits->n] = '\0';
    more_line(buf);
    more_line("");
    if (fp) fprintf(fp, "%s\n\n", buf) ;

    /* Ecriture de la matrice */
    for (i = 0; i < joueurs_inscrits->n ; i++) {
        sprintf(buf, "%ld     ", i+1);
        for (j = 0; j < joueurs_inscrits->n ; j++)  {

            if (i == j)
                buf[m + j] = 'X';
            else {
                v = score_de_machin_contre_bidule(TabJoueurs[i].Num_FFO, TabJoueurs[j].Num_FFO);
                if (v < 0)
                    buf[m + j] = ' ';
                else
                    buf[m + j] = (suisse_simple ? ZeroEgalUn[v] : Chiffres[v]);
            }
        }

        buf[m + joueurs_inscrits->n] = '\0';
        more_line(buf);
        if (fp) fprintf(fp, "%s\n", buf) ;
    }

    more_line("");

    more_close() ;
}



/* long sauvegarder_tableau_croise_HTML(char *nom_fichier_html)
 *
 * Renvoie 1 si on arrive a sauvegarder le tableau croise dans le fichier
 * HTML de nom *nom_fichier_html, et 0 sinon.
 */
long sauvegarder_tableau_croise_HTML(char *nom_fichier_html) {
    FILE *fp;
    long result;

    assert(TabJoueurs != NULL);

    /* on veut une sortie HTML */
    result = 0;
    if (nom_fichier_html[0]) {
        bloquer_signaux();
        fp = myfopen_dans_sous_dossier(nom_fichier_html, "w", nom_sous_dossier, 1, 0);
        if (fp == NULL) {
            printf(CANT_OPEN " `%s'\n", nom_fichier_html);
            result = 0;
        }
        else {
            printf(HTML_PROMPT_2) ;
            /* Appeler la fonction HTML */
            sortie_tableau_croise_HTML(fp) ;
            fclose(fp) ;
            result = 1;
        }
        debloquer_signaux();
    }
    return result;
}


/* long sauvegarder_tableau_croise_texte(long quelle_ronde, char *nom_fichier_texte, char *mode_ouverture)
 *
 * Affiche a l'ecran une version texte du tableau croise apres la ronde "quelle_ronde".
 * On peut optionnellement demander de sauvegarder ce tableau dans le fichier texte
 * de nom *nom_fichier_texte, avec le mode d'ecriture *mode_ouverture ("a"=append, "w"=write, etc.)
 * Passer NULL dans *nom_fichier_texte pour ne pas sauvegarder dans un fichier.
 *
 * Cette fonction n'echoue jamais, sauf si on n'arrive pas a ouvrir le fichier
 * demande avec les droits d'ecriture demandes.
 *
 */
long sauvegarder_tableau_croise_texte(long quelle_ronde, char *nom_fichier_texte, char *mode_ouverture) {
    FILE *fp;
    long result;

    assert(TabJoueurs != NULL);

    /* On veut une sortie du tableau croise en texte */
    result = 1;
    fp = NULL ;
    bloquer_signaux();
    if (nom_fichier_texte[0]) {
        fp = myfopen_dans_sous_dossier(nom_fichier_texte, mode_ouverture, nom_sous_dossier, 1, 0);
        if (fp == NULL) {
            printf(CANT_OPEN " `%s'\n", nom_fichier_texte);
            result = 0;
        }
    }
    /* Affichage (et eventuellement sauvegarde) du tableau */
    sortie_tableau_croise_texte(quelle_ronde, fp) ;
    if (fp)
        fclose(fp) ;
    debloquer_signaux();

    return result;
}

/* void calculer_classements_jusque(long quelle_ronde)
 *
 * On calcule les rangs de chaque joueur (ses classements
 * successifs dans le tournoi) jusqu'a la ronde "quelle_ronde".
 * NOTE : puisque l'on utilise quicksort a chaque ronde pour
 * trier les N joueurs du tournoi, le temps de cette fonction
 * est en O(quelle_ronde*N*log(N)). Cela ne semble pas un probleme
 * en pratique.
 */
void calculer_classements_jusque(long quelle_ronde) {
    long rr, i;
    structure_joueur *pj ;
    structure_ronde *pr;

    assert(TabJoueurs != NULL);


    for (rr = 0; rr <= quelle_ronde; rr++)  {

        /* Trier les joueurs apres la ronde rr */
        partial_ronde = rr ;
        local_calcul_departage(partial_ronde) ;
        SORT(TabJoueurs, joueurs_inscrits->n, sizeof(structure_joueur), compar);

        /* Mettre dans le grand tableau le rang de chaque joueur a la ronde rr */
        for (i = 0 ; i < joueurs_inscrits->n ; i++) {
            pj = &TabJoueurs[i] ;
            pr = &pj->TabRondes[rr] ;
            pr->rang = i+1;
        }
    }
}


/* void preparer_calculs_tableau_croise(long quelle_ronde)
 *
 * Recupere tous les resultats et calcule les departages,
 * les totaux de pions, etc. jusqu'a la ronde "quelle_ronde".
 */
void preparer_calculs_tableau_croise(long quelle_ronde) {
    assert(TabJoueurs != NULL);


    recuperation_resultats() ;
    calculer_classements_jusque(quelle_ronde);

    partial_ronde = quelle_ronde ;
    local_calcul_departage(partial_ronde) ;
    SORT(TabJoueurs, joueurs_inscrits->n, sizeof(structure_joueur), compar);
}



/* void afficher_cross_table(void)
 *
 * La fonction principale du module crosstable.c : propose
 * a l'utilisateur de choisir les noms des fichiers HTML
 * et texte pour enregistrer un tableau croise complet
 * contenant tous les resultats du tournoi.
 *
 */
void afficher_cross_table(void) {
    char nom_fichier_html[256], nom_fichier_texte[256], mode_ouverture[2];
    long append ;
    long ronde_affichee ;


    if (ronde < 1)
        return;

    assert(TabJoueurs != NULL);
    preparer_calculs_tableau_croise(ronde-1);

    eff_ecran();
    /* Veut-on une sortie HTML ? */
    printf(HTML_PROMPT_1) ;
    strcpy(nom_fichier_html, lire_ligne());
    putchar('\n');

    if (nom_fichier_html[0])
        sauvegarder_tableau_croise_HTML(nom_fichier_html);
    else
        eff_ecran();


    /* A quelle ronde faudra-t-il fabriquer le tableau croise texte ? */
    printf(CROSS_PROMPT_1, ronde) ;
    if (sscanf(lire_ligne(), "%ld", &ronde_affichee) != 1)
        return ;
    putchar('\n');
    if ((ronde_affichee < 1) || (ronde_affichee>ronde))
        return ;
    assert( ronde_affichee >= 1 && ronde_affichee <= ronde);


    ronde_affichee-- ;
    partial_ronde = ronde_affichee;
    local_calcul_departage(partial_ronde);
    SORT(TabJoueurs, joueurs_inscrits->n, sizeof(structure_joueur), compar);


    /* Veut-on une sortie dans un fichier texte ? */
    printf(CROSS_PROMPT_2) ;
    strcpy(nom_fichier_texte, lire_ligne());
    putchar('\n');

    /* Si oui, veut-on ajouter a la fin du fichier
       ou ecraser le contenu actuel du fichier ? */
    if (nom_fichier_texte[0]) {
        append = oui_non(CROSS_APPEND) ;
        if (append)
            sprintf(mode_ouverture, "a");
        else
            sprintf(mode_ouverture, "w");
    }

    sauvegarder_tableau_croise_texte(ronde_affichee, nom_fichier_texte, mode_ouverture);
}



/* Les deux fonctions ci-dessous permettent la gestion
 * en memoire dynamique du (gros) tableau TabJoueurs, qui
 * est utilise pour stocker les infos du module crosstable.
 * On evite ainsi de bloquer Papp dans les environnements
 * a memoire limitee.
 */

long peut_allouer_memoire_cross_table(void) {
    TabJoueurs = NULL;
    TabJoueurs = (structure_joueur*)malloc((MAX_INSCRITS+1)*sizeof(structure_joueur));

    if (!TabJoueurs)
        return 0;
    else
        return 1;
}

void liberer_memoire_cross_table(void) {
    if (TabJoueurs)
        free(TabJoueurs);
    TabJoueurs = NULL;
}
