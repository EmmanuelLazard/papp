/*
 * xml.c:pour sauvegarder les appariements/resultats et le classement au format xml
 *
 * (EL) 22/09/2012 : v1.36, correction d'un bug dans la taille du tableau local
 * 						gardant le fullname du fichier.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, Creation du fichier
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
#include "pions.h"
#include "joueur.h"

/* prototype */
void saveCreatorAndTime(FILE *) ;


#ifdef ENGLISH
	#define XML_PROMPT_1       "Creating XML standings file"
	#define XML_PROMPT_2       "Creating XML round file"
	#define CANT_OPEN          "Can't open file"
#else
	#define XML_PROMPT_1       "Creation d'un fichier XML de classement "
	#define XML_PROMPT_2       "Creation d'un fichier XML de rondes "
	#define CANT_OPEN          "Impossible d'ouvrir"
#endif


void saveCreatorAndTime(FILE *fp) {
	struct tm *t;
    time_t now;

    /* Le createur et la date de creation */
    fprintf(fp, "<!-- Creator: %s -->\n", VERSION);

    time(&now);
    t = localtime(&now);
    assert(t != NULL);
    fprintf(fp, "<!-- CreationDate: %04d/%02d/%02d %02d:%02d -->\n\n",
			t->tm_year+1900, t->tm_mon+1, t->tm_mday,
			t->tm_hour, t->tm_min);
}

void creer_classement_XML(void) {
    char nom_fichier[1024];
	char str[10] = "";
	FILE *fp;
	long _i, i, nbi;
	long place = 0, dernierScore = -1;

	strcpy(nom_fichier, nom_sous_dossier);
	strcat(nom_fichier, "stand");
	if (current_round < 10) {
		sprintf(str, "__%ld.xml", current_round);
		strcat(nom_fichier, str);
	} else if (current_round < 100) {
		sprintf(str, "_%ld.xml", current_round);
		strcat(nom_fichier, str);
	} else {
		sprintf(str, "%ld.xml", current_round);
		strcat(nom_fichier, str);
	}

    eff_ecran();

	printf(XML_PROMPT_1 ": %s\n\n", nom_fichier);
    bloquer_signaux();
    fp = myfopen_dans_sous_dossier(nom_fichier, "w", nom_sous_dossier, utiliser_sous_dossier, 0);
    if (fp == NULL) {
        printf(CANT_OPEN " `%s'\n",nom_fichier);
        goto theEnd;
    }

/* Debut du fichier XML */

	fprintf(fp,
			"<?xml version='1.0' encoding='UTF-8'?>\n"
			"<!DOCTYPE Tournament [\n"
			"<!ELEMENT Tournament (Name,Standing)>\n"
			"<!ELEMENT Standing (Number, Players)>\n"
			"<!ELEMENT Players (Player*)>\n"
			"<!ELEMENT Player (HasLeftTournament?, Place, Points, TieBreak?, Name, Id?, Country?)>\n"
			"<!ELEMENT HasLeftTournament EMPTY>\n"
			"<!ELEMENT Name (#PCDATA)>\n"
			"<!ELEMENT Number (#PCDATA)>\n"
			"<!ELEMENT Place (#PCDATA)>\n"
			"<!ELEMENT Points (#PCDATA)>\n"
			"<!ELEMENT TieBreak (#PCDATA)>\n"
			"<!ELEMENT Id (#PCDATA)>\n"
			"<!ELEMENT Country (#PCDATA)>\n"
		"]>\n\n\n");

	saveCreatorAndTime(fp);

	fprintf(fp,"<Tournament>\n");
    /* Pour afficher le fullname du tournoi en tete */
	fprintf(fp, "\t<Name>%s</Name>\n", nom_du_tournoi);
	fprintf(fp, "\t<Standing>\n");
	fprintf(fp, "\t\t<Number>%ld</Number>\n", current_round);
	fprintf(fp, "\t\t<Players>\n");

	nbi = registered_players->n;
    calcul_departage();

    if (nbi > 0) {
        long table[MAX_REGISTERED];
        Player *j;

        for (i=0; i<nbi; i++)
            table[i] = i;
        SORT(table, nbi, sizeof(long), tri_joueurs);

        for (i=0; i<nbi; i++) {
            _i = table[i];
            assert(_i >= 0 && _i < nbi);
            j  = registered_players->list[_i];

			fprintf(fp, "\t\t\t<Player>\n");
			if (!present[_i]) {
				fprintf(fp, "\t\t\t\t<HasLeftTournament />\n");
			}

			if (dernierScore != score[_i]) {
				dernierScore = score[_i];
				place = i+1;
			}
			fprintf(fp, "\t\t\t\t<Place>%ld</Place>\n", place);
			if ((score[_i] % 2) == 1) {
                fprintf(fp, "\t\t\t\t<Points>%ld.5</Points>\n", score[_i] / 2);
			} else {
                fprintf(fp, "\t\t\t\t<Points>%ld</Points>\n", score[_i] / 2);
			}
			fprintf(fp, "\t\t\t\t<TieBreak>%s</TieBreak>\n", departage_en_chaine(tieBreak[_i]));
			fprintf(fp, "\t\t\t\t<Name>%s</Name>\n", j->fullname);
			fprintf(fp, "\t\t\t\t<Id>%ld</Id>\n", j->ID);
			fprintf(fp, "\t\t\t\t<Country>%s</Country>\n", j->country);
			fprintf(fp, "\t\t\t</Player>\n");
		}
	}
	fprintf(fp, "\t\t</Players>\n");
	fprintf(fp, "\t</Standing>\n");
	fprintf(fp,"</Tournament>\n");
    fclose(fp);
	theEnd:
    debloquer_signaux();
/*    (void)lire_touche(); */
}

void creer_ronde_XML(void) {
    char nom_fichier[1024];
/*	char str[10] = "";*/
	FILE *fp;
    Pair *liste;
    long nb_paires, iterRonde;
    long i, n1, n2, atLeastOneNotPaired;
    discs_t v;
    Player *j;
    if (registered_players->n == 0) {
        return;
    }

	strcpy(nom_fichier, nom_sous_dossier);
/*	strcat(nom_fichier, "round");

	if (ronde < 10) {
		sprintf(str, "__%ld.xml", ronde+1);
		strcat(nom_fichier, str);
	} else if (ronde < 100) {
		sprintf(str, "_%ld.xml", ronde+1);
		strcat(nom_fichier, str);
	} else {
		sprintf(str, "%ld.xml", ronde+1);
		strcat(nom_fichier, str);
	}
 */
	strcat(nom_fichier, "rounds.xml");

    eff_ecran();

    printf(XML_PROMPT_2 ": %s\n\n", nom_fichier);
    bloquer_signaux();
    fp = myfopen_dans_sous_dossier(nom_fichier, "w", nom_sous_dossier, utiliser_sous_dossier, 0);
    if (fp == NULL) {
        printf(CANT_OPEN " `%s'\n",nom_fichier);
        goto theEnd_2;
    }

/* Debut du fichier XML */

	fprintf(fp,
			"<?xml version='1.0' encoding='UTF-8'?>\n"
			"<!DOCTYPE Tournament [\n"
			"<!ELEMENT Tournament (Name,Rounds)>\n"
			"<!ELEMENT Rounds (Round*)>\n\n"
			"<!ELEMENT Round (Number, Pairs, NotPairedPlayers)>\n\n"
			"<!ELEMENT Pairs (Pair*)>\n"
			"<!ELEMENT NotPairedPlayers (NotPairedPlayer*)>\n\n"
			"<!ELEMENT Pair (Table?, BlackPlayer, WhitePlayer)>\n\n"
			"<!ELEMENT BlackPlayer (Name, Id?, Score?)>\n"
			"<!ELEMENT WhitePlayer (Name, Id?, Score?)>\n\n"
		   );
	fprintf(fp,
			"<!ELEMENT NotPairedPlayer (Name, Id?)>\n\n"
			"<!ELEMENT Name (#PCDATA)>\n"
			"<!ELEMENT Number (#PCDATA)>\n\n"
			"<!ELEMENT Table (#PCDATA)>\n"
			"<!ELEMENT Id (#PCDATA)>\n"
			"<!ELEMENT Score (#PCDATA)>\n"
		"]>\n\n\n");

	saveCreatorAndTime(fp);

	fprintf(fp,"<Tournament>\n");
    /* Pour afficher le fullname du tournoi en tete */
	fprintf(fp, "\t<Name>%s</Name>\n", nom_du_tournoi);
	fprintf(fp, "\t<Rounds>\n");

    for (iterRonde = 0; iterRonde < current_round+1; iterRonde++) {
		fprintf(fp, "\t\t<Round>\n");
		fprintf(fp, "\t\t\t<Number>%ld</Number>\n", iterRonde+1);

        round_iterate(iterRonde); /* Aller a la bonne ronde */

	    /* D'abord afficher les joueurs apparies */
		CALLOC(liste, 1 + registered_players->n / 2, Pair);
	    nb_paires = 0;

	    while (next_couple(&n1, &n2, &v)) {
	        i = nb_paires++;
	        liste[i].j1 = trouver_joueur(n1);
	        liste[i].j2 = trouver_joueur(n2);
	        liste[i].score = v;
	        liste[i].sort_value = tables_sort_criteria(n1, n2);
	    }
	    if (iterRonde >= 1)
	        SORT(liste, nb_paires, sizeof(Pair), pairs_sort);

		fprintf(fp,"\t\t\t<Pairs>\n");
	    for (i = 0; i < nb_paires; i++) {
			fprintf(fp, "\t\t\t\t<Pair>\n");

			fprintf(fp, "\t\t\t\t\t<Table>%ld</Table>\n", i+1);

			fprintf(fp, "\t\t\t\t\t<BlackPlayer>\n");
			fprintf(fp, "\t\t\t\t\t\t<Name>%s</Name>\n", liste[i].j1->fullname);
			fprintf(fp, "\t\t\t\t\t\t<Id>%ld</Id>\n", liste[i].j1->ID);
			if (SCORE_IS_LEGAL(liste[i].score)) {
				fprintf(fp, "\t\t\t\t\t\t<Score>%s</Score>\n", pions_en_chaine(liste[i].score));
			}
			fprintf(fp, "\t\t\t\t\t</BlackPlayer>\n");

			fprintf(fp, "\t\t\t\t\t<WhitePlayer>\n");
			fprintf(fp, "\t\t\t\t\t\t<Name>%s</Name>\n", liste[i].j2->fullname);
			fprintf(fp, "\t\t\t\t\t\t<Id>%ld</Id>\n", liste[i].j2->ID);
			if (SCORE_IS_LEGAL(liste[i].score)) {
				fprintf(fp, "\t\t\t\t\t\t<Score>%s</Score>\n", pions_en_chaine(OPPONENT_SCORE(liste[i].score)));
			}
			fprintf(fp,"\t\t\t\t\t</WhitePlayer>\n");

			fprintf(fp,"\t\t\t\t</Pair>\n");
	    }
	    free(liste);

	    fprintf(fp,"\t\t\t</Pairs>\n");

	    /* Puis les joueurs non apparies */

		atLeastOneNotPaired = 0;

	    for (i = 0; i < registered_players->n; i++) {
	        j = (registered_players->list)[i];
	        if ((polar2(j->ID, iterRonde) == 0) && (iterRonde == current_round ? present[j->ID] : player_was_present(j->ID, iterRonde))) {
				if (atLeastOneNotPaired == 0) {
					atLeastOneNotPaired = 1;
					fprintf(fp,"\t\t\t<NotPairedPlayers>\n");
				}
				fprintf(fp, "\t\t\t\t<NotPairedPlayer>\n");
				fprintf(fp, "\t\t\t\t\t<Name>%s</Name>\n", j->fullname);
				fprintf(fp, "\t\t\t\t\t<Id>%ld</Id>\n", j->ID);
				fprintf(fp, "\t\t\t\t</NotPairedPlayer>\n");
	        }
		}
		if (atLeastOneNotPaired == 1) {
			fprintf(fp,"\t\t\t</NotPairedPlayers>\n");
		} else {
			fprintf(fp,"\t\t\t<NotPairedPlayers />\n");
		}
		fprintf(fp,"\t\t</Round>\n");
	}
	fprintf(fp,"\t</Rounds>\n");
	fprintf(fp,"</Tournament>\n");

    fclose(fp);
	theEnd_2:
    debloquer_signaux();
/*    (void)lire_touche(); */
}
