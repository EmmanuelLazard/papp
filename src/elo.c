/*
 * elo.c: Creation d'un fichier Elo
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, ajout du classement ELO dans le classement du tournoi.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 18/08/2008 : v1.33, ajout de la possibilite de rajouter des parties dans le fichier ELO
                     avec la fonction "entrer_parties_supplementaires()".
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, Le format des resultats du fichier ELO se conforme
                     au nouveau standard defini par la WOF.
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 12/02/2007 : Changement de 'cree_fichier_elo()'. On ne demande plus
 *                   le fullname du tournoi puiqu'on l'a deja.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, lecture du fichier 'nouveaux' en faisant
 *                   attention aux fins de lignes
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include "couplage.h"
#include "global.h"
#include "version.h"
#include "more.h"
#include "joueur.h"


#ifdef ENGLISH
	#define ELO_PROMPT_1       "Creating an ELO file"
	#define ELO_PROMPT_2       "Enter the filename [%s]: "
/*# define ELO_PROMPT_3     "Name of the tournament : "*/
	#define ELO_PROMPT_4		"Country: "
	#define ELO_PROMPT_5		"Date (DD/MM/YYYY) [%s]: "
	#define ELO_PROMPT_6		"Who is sending the results (your name): "
	#define CANT_OPEN           "Can't open file"
	#define THANKS				"\n\nResults file %s created.\nPlease send it to Emmanuel.Lazard@katouche.fr\nfor inclusion in the rating list.\n\n"
	#define HIT_ANY_KEY        "Hit any key to return to the main menu."
	#define NEW_PLAYERS        "New players"
	#define NEW_PLAYERS_END    "End of new players"
	#define ELO_PARTIES_SUPP   "You can now add results to the ELO file.\n\n"
	#define SUPP_NOIR          "\n\nBlack player (return to exit)?"
	#define SUPP_SCORE_NOIR    "Black score?"
	#define SUPP_BLANC         "\nWhite player?"
	#define SUPP_SCORE_BLANC   "White Score?"
#else
	#define ELO_PROMPT_1       "Creation d'un fichier ELO"
	#define ELO_PROMPT_2       "Entrez le nom du fichier [%s] : "
/*# define ELO_PROMPT_3     "et le fullname complet du tournoi : "*/
	#define ELO_PROMPT_4		"Pays : "
	#define ELO_PROMPT_5		"Date (DD/MM/YYYY) [%s] : "
	#define ELO_PROMPT_6		"Qui envoie le fichier (votre nom) : "
	#define CANT_OPEN          "Impossible d'ouvrir"
	#define THANKS				"\n\nFichier de resultats %s cree.\nMerci de l'envoyer a Emmanuel.Lazard@katouche.fr\npour inclusion dans le classement.\n\n"
	#define HIT_ANY_KEY        "Pressez une touche pour revenir au menu principal."
	#define NEW_PLAYERS        "Nouveaux joueurs"
	#define NEW_PLAYERS_END    "Fin des nouveaux joueurs"
	#define ELO_PARTIES_SUPP   "Vous pouvez maintenant entrer des parties a ajouter au fichier ELO.\n\n"
	#define SUPP_NOIR          "\n\nJoueur noir (return pour quitter) ?"
	#define SUPP_SCORE_NOIR    "Score noir ?"
	#define SUPP_BLANC         "\nJoueur blanc ?"
	#define SUPP_SCORE_BLANC   "Score blanc ?"

#endif

/* Le contenu du fichier des nouveaux dans le FILE* passe en parametre*/
void copier_fichier_nouveaux(FILE *fp_dest) {
    FILE *fichier_nouveaux;
    unsigned long i, fin_ligne ;
    long c ;
    char nom_joueur_nouveau[256];

    fichier_nouveaux = myfopen_dans_sous_dossier(nom_fichier_nouveaux, "rb", "", 0, 0);
    if (fichier_nouveaux != NULL) {
        fprintf(fp_dest,"\n%% " NEW_PLAYERS " :\n%%\n");
        do {
            i=0 ; fin_ligne=0 ; /* on commence une nouvelle ligne */
            do {
                c = fgetc(fichier_nouveaux) ;
                if (c != EOF && c != 0x0D && c != 0x0A && i<(sizeof(nom_joueur_nouveau)-1))
                    nom_joueur_nouveau[i++] = c ;
                else
                    fin_ligne = 1 ;
            } while (!fin_ligne) ;
            nom_joueur_nouveau[i] = 0 ;
            if (strlen(nom_joueur_nouveau) != 0) {
                fprintf(fp_dest,"%%_%%");
                fprintf(fp_dest,"%s\n", nom_joueur_nouveau);
            }
        } while (c != EOF) ;

        fprintf(fp_dest,"%%\n%% " NEW_PLAYERS_END "\n\n");
        fclose(fichier_nouveaux);
    }
}

/* La liste des joueurs dans le FILE* passe en parametre */
void copier_liste_inscrits(FILE *fp_dest) {
	long i, _i, nbi ;

    assert(registered_players != NULL);
    assert(new_players != NULL);

    nbi = registered_players->n;

    if (nbi > 0) {
        long    table[MAX_REGISTERED];
        Player *j;

        for (i=0; i<nbi; i++)
            table[i] = i;
        SORT(table, nbi, sizeof(long), tri_joueurs);

		fprintf(fp_dest, "\n%%        ID, NAME, Firstname, COUNTRY, score, disc-count, rating\n") ;
		for (i=0; i<nbi; i++) {
			_i = table[i];
            assert(_i >= 0 && _i < nbi);
            j  = registered_players->list[_i];
			fprintf(fp_dest, "%%_%% ") ;
			fprintf(fp_dest, "%c", (j->newPlayer ? '+' : ' ')) ;
			fprintf(fp_dest, "%6ld, %s, %s, %s", j->ID, j->family_name, j->firstname, j->country) ;
			/* Sauvegarde scores */
			if ((score[_i] % 2) == 1)
				fprintf(fp_dest,", %2ld.5", (score[_i] / 2 ));
			else
				fprintf(fp_dest,", %2ld", (score[_i] / 2 ));
			/* Sauvegarde pions */
			fprintf(fp_dest, ", %s, %ld\n", pions_en_chaine(nbr_discs[_i]), j->rating) ;
		}
		fprintf(fp_dest, "\n\n") ;
	}
}

int lire_score(char *prompt) {
	int number ;
	unsigned int i ;
	char tampon[100] ;

	while (1) {
		printf("%s", prompt) ;
		strcpy(tampon, lire_ligne());
		/* Verifions qu'il n'y a que des chiffres */
		if (strlen(tampon) == 0) {
			printf("\n") ;
			continue ;
		}
		number = 1 ;
		for (i=0 ; i<strlen(tampon) ; i++) {
			if (!isdigit(tampon[i])) {
				number = 0 ;
				break ;
			}
		}
		if (!number) {
			continue ;
			printf("\n") ;
		}
		number = atoi(tampon) ;
		if ( (number<0) || (number>64)) {
			continue ;
			printf("\n") ;
		}
		return number ;
	}
}


/* Permet a l'utilisateur d'entrer des parties supplementaires
 * (ex: finales,...) dans le fichier ELO
 */
void entrer_parties_supplementaires(FILE *fp_dest) {
	long jNoir, jBlanc ;
	int scNoir, scBlanc ;
	char *tampon ;
	char symb ;
	printf(ELO_PARTIES_SUPP) ;
	fprintf(fp_dest, "\n\n%%%%Added results\n") ;
	while (1) {
		jNoir = choix_d_un_joueur_au_clavier(SUPP_NOIR, registered_players, &tampon) ;
		if ((jNoir == TOUS_LES_JOUEURS) || (jNoir<0))
			break ;
		scNoir = lire_score(SUPP_SCORE_NOIR) ;
		do {
			jBlanc = choix_d_un_joueur_au_clavier(SUPP_BLANC, registered_players, &tampon) ;
		} while (jBlanc <0) ;
		scBlanc = lire_score(SUPP_SCORE_BLANC) ;
		/* Sauvegarder le resultat */
		if (scNoir > scBlanc)
			symb = '>' ;
		else if (scNoir < scBlanc)
			symb = '<' ;
		else
			symb = '=' ;
		fprintf(fp_dest, "%6ld (%02d)%c(%02d) %6ld %c\n", jNoir, scNoir, symb, scBlanc, jBlanc, 'B');
	}
}

void cree_fichier_elo(void) {
    char nom_fichier[256] = "" ;
	/*  char nom_tournoi[256]; */
	char nom_pays[256] ="" ;
	char date_elo[256] ="" ;
	char sender[256] ="" ;
	char strtmp[256] ="" ;
    FILE *fp;
    struct tm *t;
    time_t now;
    long ronde0, n1, n2;
    discs_t v;
	char c =' ';

    if (current_round < 1)
        return;
    eff_ecran();
    /* Creation d'un fullname de fichier par defaut */
	if (strlen(date_tournoi) > 9) {
		strcpy(nom_fichier, &date_tournoi[6]) ;/* annee */
		nom_fichier[4]=date_tournoi[3] ; /* mois */
		nom_fichier[5]=date_tournoi[4] ;
		nom_fichier[6]=date_tournoi[0] ; /* jour */
		nom_fichier[7]=date_tournoi[1] ;
		nom_fichier[8] = '_' ;
		nom_fichier[9] = '\0' ;
	}
	strcat(nom_fichier, nom_sous_dossier) ;
	strcpy(&nom_fichier[strlen(nom_fichier)-1], ".ELO") ;

    printf(ELO_PROMPT_1 "\n\n" ELO_PROMPT_2, nom_fichier);
    strcpy(strtmp, lire_ligne());
	if (strtmp[0])
		strcpy(nom_fichier, strtmp) ;
	if (nom_fichier[0] == 0)
        return;                 /* Annulation */
    putchar('\n');

	/* Nom du pays */
	printf(ELO_PROMPT_4);
	strcpy(nom_pays, lire_ligne());
	if (!nom_pays[0]) /* vide */
		strcpy(nom_pays, "??") ;
	putchar('\n');

	/* Date du tournoi */
	printf(ELO_PROMPT_5, date_tournoi);
	strcpy(date_elo, lire_ligne());
	if (!date_elo[0])
		strcpy(date_elo, date_tournoi) ;
	putchar('\n');

	/* Nom de l'expediteur */
	printf(ELO_PROMPT_6);
	strcpy(sender, lire_ligne());
	if (!sender[0]) /* vide */
		strcpy(sender, "??") ;
	putchar('\n');

    bloquer_signaux();
    fp = myfopen_dans_sous_dossier(nom_fichier, "w", nom_sous_dossier, 1, 0);
    if (fp == NULL) {
        printf(CANT_OPEN " `%s'\n",nom_fichier);
        goto attendre_touche;
    }
    /* Inscrire le fullname du tournoi */
    if (nom_du_tournoi[0])
        fprintf(fp, "%%%%Tournament: %s\n", nom_du_tournoi);
	else
        fprintf(fp, "%%%%Tournament: \n");
	fprintf(fp, "%%%%Country: %s\n", nom_pays);
	fprintf(fp, "%%%%Date: %s\n", date_elo);
	fprintf(fp, "%%%%Sender: %s\n\n", sender);


    /* Le createur et la date de creation */
    fprintf(fp, "%%%%Creator: %s\n", VERSION);

    time(&now);
    t = localtime(&now);
    assert(t != NULL);
    fprintf(fp, "%%%%CreationDate: %04d/%02d/%02d %02d:%02d\n",
			t->tm_year+1900, t->tm_mon+1, t->tm_mday,
			t->tm_hour, t->tm_min);

	/* recopier le fichier nouveaux */
	/*  copier_fichier_nouveaux(fp) ;*/
	copier_liste_inscrits(fp) ;

	/* Boucle sur les rondes */
    for (ronde0 = 0; ronde0 < current_round; ronde0++) {
        fprintf(fp,"\n%%Round: %ld\n\n", ronde0+1);
        round_iterate(ronde0);
        while (next_couple(&n1, &n2, &v)) {
            if (IS_VICTORY(v))
				c = '>' ;
			if (IS_DRAW(v))
				c = '=' ;
            if (IS_DEFEAT(v))
				c = '<';
			fprintf(fp, "%6ld (%s)%c(%s) %6ld %c\n", n1, pions_en_chaine(v),c, pions_en_chaine(OPPONENT_SCORE(v)), n2, 'B');
        }
    }
	entrer_parties_supplementaires(fp) ;

    fclose(fp);
	printf(THANKS, nom_fichier) ;
    printf(HIT_ANY_KEY "\n");
	attendre_touche:
    debloquer_signaux();
    (void)lire_touche();
}
