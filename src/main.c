/*
 * main.c: module principal de PAPP
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 02/05/2007 : v1.31, changements dans les #include
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 03/04/2007 : Changement de l'affichage initial.
 * (EL) 30/03/2007 : Ecriture de la fonction 'Init_Sous_Dossier()' qui cree
                     un sous-dossier a partir du nom du tournoi et modifie les noms
                     des fichiers class/result/ronde...
 * (EL) 06/02/2007 : Test de la presence d'un ancien fichier intermediaire
                     ou de l'absence de ce fichier. Dans ce cas, demande
                     les caracteristiques et sauvegarde un nouveau fichier intermediaire.
                     Ecriture de la fonction 'Demander_caracteristiques_tournoi'
 * (EL) 04/02/2007 : Test de la presence du type precedent
 *                   du fichier de config -> affichage d'un warning.
                     Ecriture de la fonction 'warning_old_papp_cfg'.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#if defined(CYGWIN) || defined(MACOSX) || defined(LINUX)
	#include <signal.h>
#else
	#include <sys/signal.h>
#endif

#if defined(UNIX_BSD) || defined(UNIX_SYSV)
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif


#include "global.h"
#include "joueur.h"
#include "version.h"
#include "changes.h"
#include "couplage.h"
#include "more.h"
#include "crosstable.h"
#include "pions.h"


static void main_menu(void)	EXITING ;	/* decl.forward */
void messages_copyleft(void);
void warning_absence_papp_cfg(void);
void warning_old_papp_cfg(void) ;
void warning_deux_fichiers_de_joueurs(char *nom_fichier_joueurs,
                                      char *nom_fichier_joueurs_alt);
void Demander_caracteristiques_tournoi(long type_fichier) ;
void Sauver_fichier_intermediaire(long type_fichier) ;
void Init_Sous_Dossier(void) ;

/* static void printnewline(void) {} */
/* static void main_menu(void) /-- decl.forward --/ */

#ifdef ENGLISH
# define MSG_WELCOME    "%s (%s) by Thierry Bousch\n"
# define MSG_CANTLOCK   "can't lock file.\n" \
						"\nIt may be that you don't have execute and write privileges\n" \
						"in that directory.\n"
# define MSG_LOCKED     "the program seems already running.\n" \
                        "\nPerhaps there is an old lockfile (from an old PAPP session)\n" \
                        "in the current directory.\n"
# define BAD_CONFIG     "bad configuration file"
# define BAD_PLAYERS    "bad players file"
# define BAD_NEWP       "bad new-players file"
# define BAD_SAVE       "bad save file"
# define MSG_PLAYERS    "There are %ld players in the database.\n"
# define MSG_CROSS_MEMORY "Not enough memory to generate the cross-table."
# define MSG_NAME_TRN   "         * Name of tournament? "
# define MSG_NBR_ROUNDS "         * How many rounds? "
# define MSG_RROBIN     "           Round-robin between 1 and %ld players.\n"
# define MSG_BQ         "\n         * Brightwell constant used for tie-breaking\n           counting one point per victory\n           (%.0f as default, enter 0 to use disc-count as tie-break)? "
# define PRESS_KEY      "Press any key"
#else
# define MSG_WELCOME    "%s (%s) par Thierry Bousch\n"
# define MSG_CANTLOCK   "verrouillage impossible.\n" \
						"\nPeut-etre n'avez-vous pas les droits d'execution et d'ecriture\n" \
						"sur ce repertoire.\n"
# define MSG_LOCKED     "le programme semble deja en cours d'execution.\n" \
                        "\nPeut-etre y a-t-il un vieux lockfile (d'une vieille session PAPP)\n" \
                        "dans le repertoire courant.\n"
# define BAD_CONFIG     "fichier de configuration incorrect"
# define BAD_PLAYERS    "fichier des joueurs incorrect"
# define BAD_NEWP       "fichier des nouveaux incorrect"
# define BAD_SAVE       "fichier intermediaire corrompu"
# define MSG_PLAYERS    "Il y a %ld joueurs dans la base.\n"
# define MSG_CROSS_MEMORY "Pas assez de memoire pour la cross-table."
# define MSG_NAME_TRN   "         * Nom du tournoi ? "
# define MSG_NBR_ROUNDS "         * Combien de rondes ? "
# define MSG_RROBIN     "           Toutes-rondes entre 1 et %ld joueurs.\n"
# define MSG_BQ         "\n         * Constante de Brightwell pour le departage\n           en comptant un point par victoire\n           (%.0f par defaut, entrez 0 pour un departage aux pions) ? "
# define PRESS_KEY      "Appuyer sur une touche"
#endif



void messages_copyleft() {
	eff_ecran();
	printf("\n\n"
#ifdef ENGLISH
		   "        %s (pairing program)  [%s]\n"
		   "\n"
		   "        Written by Thierry Bousch <bousch@topo.math.u-psud.fr>\n"
		   "        Suggestions to <Emmanuel.Lazard-AT-katouche.fr>\n"
		   "\n"
		   "        This is free software; see the file \"COPYING\"\n"
		   "        for distribution conditions.\n\n"
		   "        You can inscribe at most %d players, and there is\n"
		   "        a maximum of %d rounds. There are %ld players in \n"
		   "        the file.\n"
#else
		   "        %s (programme d'appariements)  [%s]\n"
		   "\n"
		   "        Auteur : Thierry Bousch <bousch-AT-topo.math.u-psud.fr>\n"
		   "        Suggestions : <Emmanuel.Lazard-AT-katouche.fr>\n"
		   "\n"
		   "        Ce logiciel est librement redistribuable sous les\n"
		   "        conditions GNU decrites dans le fichier \"COPYING\".\n\n"
		   "        On peut inscrire et apparier %d joueurs au plus,\n"
		   "        avec un maximum de %d rondes. Il y a %ld joueurs \n"
		   "        dans la base.\n"
#endif
		   , VERSION, __DATE__, MAX_INSCRITS, NMAX_RONDES, nombre_joueurs_base());
}

void affiche_banniere_cfg(void) {
	printf("\n\n"
#ifdef ENGLISH
		   "   **************************************************************\n"
		   "   *                     Configuration file                     *\n"
		   "   **************************************************************\n"
#else
		   "   **************************************************************\n"
		   "   *                  Fichier de configuration                  *\n"
		   "   **************************************************************\n"
#endif
		   );
}

void warning_absence_papp_cfg(void) {
	/* eff_ecran(); */
	beep();
	printf("\n"
#ifdef ENGLISH
		   "      WARNING: I can't find the configuration file \"%s\".\n"
		   "\n"
		   "      I will try to create, in the current directory, a file\n"
		   "      called \"%s\" with default values, but these values\n"
		   "      will probably not fit exactly your requirements for the\n"
		   "      tournament rules, and you should probably try to use\n"
		   "      instead the config file from the official %s\n"
		   "      distribution [www.ffothello.org/info/appa.php].\n"
		   "      Please read the manual for details.\n"
#else
		   "      WARNING : je ne trouve pas le fichier de configuration \"%s\".\n"
		   "\n"
		   "      Je vais essayer de creer, dans le repertoire courant,\n"
		   "      un fichier \"%s\" comprenant des valeurs par\n"
		   "      defaut, mais ces valeurs ne correspondent peut-etre\n"
		   "      pas aux regles de votre tournoi et vous devriez plutot\n"
		   "      essayer d'utiliser le fichier de configuration\n"
		   "      de la distribution officielle de %s [www.ffothello.org/info/appa.php].\n"
		   "      Lisez la documentation pour les details.\n"
#endif
		   , nom_fichier_config, nom_fichier_config, VERSION);
	/* (void)lire_touche(); */
}

void warning_old_papp_cfg(void) {
	/* eff_ecran(); */
	beep();
	printf("\n"
#ifdef ENGLISH
		   "      WARNING: Your configuration file \"%s\" is the one\n"
		   "      used in Papp up to version 1.29. A new type of\n"
		   "      configuration file is available. I shall use your file\n"
		   "      but you should probably try to use instead the config\n"
		   "      file from the official %s distribution\n"
		   "      [www.ffothello.org/info/appa.php].\n"
#else
		   "      Votre fichier de configuration \"%s\" correspond\n"
		   "      a celui utilise jusqu'a la version 1.29 de Papp.\n"
		   "      Un nouveau type de fichier de configuration est\n"
		   "      maintenant disponible. Je vais utiliser le fichier indique\n"
		   "      mais je vous recommande d'utiliser le fichier de\n"
		   "      configuration de la distribution officielle de %s\n"
		   "      [www.ffothello.org/info/appa.php].\n"
#endif
		   , nom_fichier_config, VERSION );
	/* (void)lire_touche();*/
}

void affiche_param_type2(void) {
	printf("\n"
#ifdef ENGLISH
		   "      Parameters currently in use, which you may wish \n"
		   "      to change by editing the papp.cfg file.\n\n"
		   "          Default country: %s\n"
		   "          BYE score and total number of discs: %s/%ld\n"
		   "          Use a subfolder: %s\n"
#else
		   "      Parametres actuellement utilises, que vous\n"
		   "      pouvez changer en editant le fichier papp.cfg\n\n"
		   "          Pays par defaut : %s\n"
		   "          Score de BIP et total de pions : %s/%ld\n"
		   "          Utilisation d'un sous-repertoire : %s\n"
#endif
		   , pays_defaut, pions_en_chaine(score_bip), total_pions,
#ifdef ENGLISH
		   (utiliser_sous_dossier?"YES":"NO"));
#else
	(utiliser_sous_dossier?"OUI":"NON"));
#endif
	/* (void)lire_touche(); */
}

void warning_deux_fichiers_de_joueurs(char *nom_fichier_joueurs,
                                      char *nom_fichier_joueurs_alt) {
	eff_ecran();
	beep();
	printf("\n\n"
#ifdef ENGLISH
		   "      PROBLEM : you have two database files of players.\n"
		   "\n"
		   "      There seems to exist, in the current directory,\n"
		   "      both a file named     :  \"%s\" \n"
		   "      and another named     :  \"%s\" \n"
		   "      (Beware of the option in Windows or in Mac OS X\n"
		   "      to hide the file extension, btw).\n\n\n"
		   "      Please delete or rename the obsolete player file,\n"
		   "      then relaunch Papp. Thanks !\n"
#else
		   "      PROBLEM : vous avez deux fichiers de joueurs.\n"
		   "\n"
		   "      Vous semblez avoir, dans le repertoire courant,\n"
		   "      a la fois un fichier    :  \"%s\" \n"
		   "      et un autre fichier     :  \"%s\" \n"
		   "      (Peut-etre est-ce du au fait que Windows ou Mac OS X\n"
		   "      a cache les extensions de fichier, au fait).\n\n\n"
		   "      En tout cas, veuillez detruire ou renommer le fichier\n"
		   "      obsolete, puis relancer Papp. Merci !\n"
#endif
		   , nom_fichier_joueurs, nom_fichier_joueurs_alt);
	(void)lire_touche();
}

void affiche_banniere_tournoi(void) {
	printf("\n\n"
#ifdef ENGLISH
		   "   **************************************************************\n"
		   "   *                  Current tournament info                   *\n"
		   "   **************************************************************\n"
#else
		   "   **************************************************************\n"
		   "   *                Informations tournoi en cours               *\n"
		   "   **************************************************************\n"
#endif
		   );
}

/* Si c'est un nouveau tournoi ou un ancien fichier intermediaire
   On demande les caracteristiques du tournoi */
void Demander_caracteristiques_tournoi(long type_fichier) {
	char *tmp;
	double BQ_tmp ;
	struct tm *t;
    time_t now;
	char date_tmp[25];

	assert(type_fichier == INEXISTANT || type_fichier == OLD) ;
	/*	eff_ecran(); */
	beep();
	if (type_fichier == INEXISTANT) {
		printf("\n"
#ifdef ENGLISH
			   "      You are starting a new tournament.\n"
			   "      Please enter its main caracteristics.\n"
#else
			   "      Vous debutez un nouveau tournoi.\n"
			   "      Merci de m'indiquer ses caracteristiques.\n"
#endif
			   ) ;
	} else {
		printf("\n"
#ifdef ENGLISH
			   "      I found an old 'papp-workfile' file used in an old Papp version.\n"
			   "      Please enter the main caracteristics of the tournament.\n"
#else
			   "      J'ai trouve le fichier 'papp-workfile' du tournoi\n"
			   "      correspondant a une ancienne version de Papp.\n"
			   "      Merci de m'indiquer les caracteristiques du tournoi.\n"
#endif
			   ) ;
	}
	printf(MSG_NAME_TRN) ;
	tmp = lire_ligne() ;
	COPIER(tmp, &nom_du_tournoi);
	putchar('\n');
	nombre_de_rondes = 0 ;
	while( (nombre_de_rondes<1) || (nombre_de_rondes>NMAX_RONDES) ) {
		printf(MSG_NBR_ROUNDS) ;
		if (sscanf(lire_ligne(), "%ld", &nombre_de_rondes) != 1) {
			beep() ;
			nombre_de_rondes = 0 ;
		}
		putchar('\n');
	}
	/* Si 2n-1 ou 2n rondes, on a 2n joueurs max pour un toutes-rondes */
	ttr_maxj = ((nombre_de_rondes+1)/2)*2 ;
	ttr_minj = 1 ;
	printf(MSG_RROBIN, ttr_maxj) ;
	coef_brightwell = (nombre_de_rondes<8 ? 0.0 : 3.0) ; /* coeff par defaut */
	BQ_tmp = -1.0 ;
	do {
		printf(MSG_BQ, coef_brightwell*2) ;
		if (sscanf(lire_ligne(), "%lf", &BQ_tmp) < 1)
			BQ_tmp = coef_brightwell*2 ;
		putchar('\n');
	} while (BQ_tmp<0) ;
	coef_brightwell = BQ_tmp/2 ;
/* Quelle date pour le tournoi ? */
    time(&now);
    t = localtime(&now);
    assert(t != NULL);
    sprintf(date_tmp, "%02d/%02d/%04d",t->tm_mday, t->tm_mon+1, t->tm_year+1900);
	COPIER(date_tmp, &date_tournoi);

	/*	(void)lire_touche(); */
}

void affiche_params_tournoi(void) {
	printf("\n"
#ifdef ENGLISH
		   "          Name of tournament: %s\n"
		   "          Date of tournament: %s\n"
		   "          Number of rounds: %ld\n"
		   "          Brightwell constant: %f\n"
#else
		   "          Nom du tournoi : %s\n"
		   "          Date du tournoi : %s\n"
		   "          Nombre de rondes : %ld\n"
		   "          Coefficient de Brightwell : %f\n"
#endif
		   , nom_du_tournoi, date_tournoi, nombre_de_rondes, coef_brightwell*2) ;

	bas_ecran();
	inv_video(PRESS_KEY);
	(void)lire_touche();
}

void Init_Sous_Dossier(void) {
	const char *char_speciaux = "#&%:?*!/'`\"\\" ;
	unsigned long i ;

	if (utiliser_sous_dossier != 1)
		return ;
	if (nom_du_tournoi == NULL || nom_du_tournoi[0] == '\0')
		return ;
	/* On cree d'abord le nom du sous-dossier sans les espaces et caracteres speciaux */
	COPIER(nom_du_tournoi, &nom_sous_dossier) ;
	for (i=0 ; i<strlen(nom_sous_dossier) ; i++) {
		if (nom_sous_dossier[i] == ' ')
			nom_sous_dossier[i] = '_' ; /* on remplace les espaces par des '_' */
		if (strchr(char_speciaux, nom_sous_dossier[i]))
			nom_sous_dossier[i] = '-' ; /* on remplace les char speciaux par des '-' */
	}
	CONCAT(nom_sous_dossier, "/", &nom_sous_dossier) ;
	/* Le repertoire existe-t-il ? */

	if (mkdir(nom_sous_dossier, 0777) == -1)
		if (errno != EEXIST)
			/* Le repertoire ne peut etre cree et ce n'est pas parce qu'il existe */
			return ;
	/* Modifier le nom des fichiers */
	CONCAT(nom_sous_dossier, nom_fichier_appariements, &nom_fichier_appariements) ;
	CONCAT(nom_sous_dossier, nom_fichier_result, &nom_fichier_result) ;
	CONCAT(nom_sous_dossier, nom_fichier_classement, &nom_fichier_classement) ;
	CONCAT(nom_sous_dossier, nom_fichier_class_equipes, &nom_fichier_class_equipes) ;
	CONCAT(nom_sous_dossier, nom_fichier_crosstable_HTML, &nom_fichier_crosstable_HTML) ;
	CONCAT(nom_sous_dossier, nom_fichier_elo, &nom_fichier_elo) ;
	/* Pour le fichier intermediaire, il faut garder les deux noms : l'original et celui
	 * concatene avec le sous-dossier pour la sauvegarde */
	CONCAT(nom_sous_dossier, nom_fichier_inter, &nom_fichier_inter_backup) ;
	CONCAT(nom_fichier_inter_backup, "-BACKUP", &nom_fichier_inter_backup) ;
}

int main (int argc, char **argv) {
	long ret;
	char nom_fichier_joueurs_alt[2048];

 #if defined(PAPP_MAC_METROWERKS)
	init_mac_SIOUX_console();
	nom_programme = VERSION;
 #else
	nom_programme = argv[0];
 #endif

	/*
	 * L'alloc-ring est utilise pour (quasiment) toutes les allocations
	 * temporaires de chaines, mais il s'initialise tout seul!
	 */


	/* Initialisation de l'ecran, puis du clavier */
	init_ecran();
	init_clavier();

	/* Message de presentation/copyright */
	eff_ecran() ;
	printf(MSG_WELCOME, VERSION, __DATE__);


	/* Obtenir un verrou */
	ret = poser_verrou();
	if (ret < 0)
		erreur_fatale(MSG_CANTLOCK);
	if (ret > 0) {
		beep();
		erreur_fatale(MSG_LOCKED);
	}

	/* Initialisation des listes de joueurs */
	joueurs_inscrits   = creer_liste();
	joueurs_nouveaux   = creer_liste();
	joueurs_emigres    = creer_liste();
	capitaines_equipes = creer_liste();

	/* Initialisation de l'historique */
	premiere_ronde();
	/* Initialisation des tableaux de penalites */
	init_penalites_defaut();

	/* Recherche du fichier de configuration */
	if (getenv("PAPP_CFG"))
		nom_fichier_config = getenv("PAPP_CFG");
	if (argc > 2)
		erreur_fatale("usage: papp [config-file]");
	else if (argc == 2)
		nom_fichier_config = argv[1];

	/* Lecture du fichier de config */
	type_fichier_config = INEXISTANT ;
	affiche_banniere_cfg() ;
	ret = lire_fichier(nom_fichier_config, F_CONFIG);
	if (ret > 0)
		erreur_fatale(BAD_CONFIG);
	if (ret == -1 && errno == ENOENT) {
		/* Sauvegarde de la configuration par defaut, si possible */
		FILE *fp;
		if ((fp = myfopen_dans_sous_dossier(nom_fichier_config, "w", "", 0, 0)) != NULL) {
			warning_absence_papp_cfg();
			bloquer_signaux();
			grand_dump(fp);
			debloquer_signaux();
			fclose(fp);
		}
	}
	if (ret == 0 && type_fichier_config == OLD) {
		/* Prevenir l'utilisateur que c'est un ancien type de fichier cfg */
		warning_old_papp_cfg() ;
	}
	affiche_param_type2() ;

	/* Fabrication du nom de fichier "joueurs.txt" */
	(void)strcpy(nom_fichier_joueurs_alt, nom_fichier_joueurs);
	(void)strcat(nom_fichier_joueurs_alt, ".txt");

	/* Verifions que l'utilisateur ne s'est pas emmele les pinceaux
	   avec les fichiers joueurs et joueurs.txt */
	if (fichier_existe(nom_fichier_joueurs) &&
	    fichier_existe(nom_fichier_joueurs_alt))
	    {
	    warning_deux_fichiers_de_joueurs(nom_fichier_joueurs, nom_fichier_joueurs_alt);
	    terminer();
	    return (-999);
	    }

	/* Lecture du fichier des joueurs, sans l'extension .txt */
	ret = lire_fichier(nom_fichier_joueurs, F_JOUEURS);
	if (ret > 0)
		erreur_fatale(BAD_PLAYERS);

	/* Lecture du fichier des joueurs, avec l'extension .txt */
	ret = lire_fichier(nom_fichier_joueurs_alt, F_JOUEURS);
	if (ret > 0)
		erreur_fatale(BAD_PLAYERS);

	/* Lecture des nouveaux */
	ret = lire_fichier(nom_fichier_nouveaux, F_NOUVEAUX);
	if (ret > 0)
		erreur_fatale(BAD_NEWP);

	/* Si le nom du fichier intermediaire est different de '__papp__',
	   on renomme un ancien fichier '__papp__' avec ce nouveau nom.        */
	if (strcmp(nom_fichier_inter, "__papp__")) {
		if (fichier_existe("__papp__") && !fichier_existe(nom_fichier_inter)) {
			rename("__papp__", nom_fichier_inter);
		}
	}
	/* Nous pouvons maintenant lire le fichier de sauvegarde */
	type_fichier_intermediaire = INEXISTANT ;
	affiche_banniere_tournoi() ;
	ret = lire_fichier(nom_fichier_inter, F_CONFIG);
	if (ret > 0)
		erreur_fatale(BAD_SAVE);

	if (type_fichier_intermediaire != CORRECT) {
		/* On a soit un vieux fichier intermediaire, soit pas de fichier */
		Demander_caracteristiques_tournoi(type_fichier_intermediaire) ;
		init_fichier_intermediaire(type_fichier_intermediaire) ;
	} else {
		affiche_params_tournoi() ;
	}

	/* Creation eventuelle d'un sous-dossier et modification des noms de fichiers */
	if (utiliser_sous_dossier == 1) {
		Init_Sous_Dossier() ;
	}
	/* Pour eviter une sauvegarde automatique */
	ne_pas_sauver_inscrits();
	ne_pas_sauver_appariements();

	installer_signaux();
	verification_penalites();

    /* afficher les infos */
    /*
    messages_copyleft();
    (void)lire_touche();
    */

	/* Boucle principale */
	main_menu();

	return (-999);
}

#ifdef ENGLISH
# define WHICH_FILENAME		"Filename?"
# define WHICH_PLAYER		"Which player?"
# define WHICH_FEATS		"Feats for which player?"
# define WHO_COMES			"Who comes back?"
# define WHO_LEAVES			"Who leaves?"
# define OLD_NATION         "Old nationality: "
# define NEW_NATION         "New nationality: "
#else
# define WHICH_FILENAME		"Nom de fichier ?"
# define WHICH_PLAYER		"Quel joueur ?"
# define WHICH_FEATS		"Fiche de quel joueur ?"
# define WHO_COMES			"Qui revient ?"
# define WHO_LEAVES			"Qui s'en va ?"
# define OLD_NATION         "Ancienne nationalite: "
# define NEW_NATION         "Nouvelle nationalite: "
#endif


#define ASK_FILENAME(var)				\
	printf("\n\n%s ", WHICH_FILENAME);		\
	var = strcpy(new_string(), lire_ligne());	\
	putchar('\n'); if (*var == '\0')  break;

#define ASK_PLAYER(var)					\
	printf("\n\n%s ", WHICH_PLAYER);		\
	if (sscanf(lire_ligne(), "%ld", &var) != 1)	\
		break;

static void main_menu (void) {
    char c, *ligne, *fichier;
    long no_joueur, k;
#ifdef atarist
    extern int __mint;
#endif

    for(;;) {
        init_ecran();
	    eff_ecran();
#ifdef ENGLISH
        printf("\n                MAIN MENU\n\n"
        "        & About this program\n"
        "        I Inscription of new players\n"
        "        L List of all players\n"
        "        - Someone quits the tournament\n"
        "        + Someone enters the tournament\n"
        "        N Nationality of a player\n"
        "        F Feats of a player\n"
        "        T Team results\n"
		);
		printf(
		"        W Crosstable and Web page export\n"
        "        M Manual pairings\n"
        "        A Automatic pairings\n"
        "        V View pairings\n"
        "        R Results of this round\n"
        "        C Correct an old result\n"
        "        E Create an ELO file\n"
        "        S Save the current ranking\n"
        "        X Exit this program\n"
        "\n        Your choice? ");
#else
        printf("\n                MENU PRINCIPAL\n\n"
        "        & Version du programme\n"
        "        I Inscription des joueurs\n"
        "        L Liste des joueurs\n"
        "        - Depart d'un joueur\n"
        "        + Retour d'un joueur\n"
        "        N Nationalite des joueurs\n"
        "        F Fiche individuelle d'un joueur\n"
        "        T Totaux des equipes\n"
		);
		printf(
        "        W Tableaux des resultats et page Web\n"
        "        M Appariement manuel\n"
        "        A Appariement automatique\n"
        "        V Voir les appariements\n"
        "        R Resultats d'une ronde\n"
        "        C Corriger un vieux resultat\n"
        "        E Creation d'un fichier ELO\n"
        "        S Sauvegarder le classement\n"
        "        X Quitter le programme\n"
        "\n        Votre choix ? ");
#endif

lire:
        c = lire_touche();
        c = tolower(c);
        fichier = NULL;
        init_ecran();

    /* sur Mac OS X, les touches speciales F1, .., F14 et
	   les fleches (bas, haut, etc.) envoient deux caracteres
	   ascii: on  appelle deux fois de plus lire_touche() pour
	   consommer les caracteres parasites */
#if defined(MACOSX)
        if (c == '\033') {lire_touche();lire_touche();c='\n';}
#endif

        switch (c) {
	        case '&':
			    messages_copyleft();
		    touche: (void)lire_touche();
			    break;
	        case 'i':
			    entree_joueurs();
			    break;
	        case 's':
			    ASK_FILENAME(fichier);
	        case 'l':
			    affiche_inscrits(fichier);
			    break;
		    case 't':
			    affiche_equipes(fichier);
			    break;
	        case '\026':	/* Ctrl-V */
			    ASK_FILENAME(fichier);
	        case 'v':
			    affiche_appariements(fichier, 0);
			    break;
	        case '\006':	/* Ctrl-F */
			    ASK_FILENAME(fichier);
	        case 'f':
			    eff_ecran();
			    printf("\n\n");
			    if ((no_joueur = choix_d_un_joueur_au_clavier(WHICH_FEATS, joueurs_inscrits, &ligne)) <= 0)
				    break;
				fiche_individuelle(no_joueur, fichier);
			    break;
#if defined(UNIX_BSD) || defined(UNIX_SYSV)
	        case '\032':	/* Ctrl-Z */
# ifdef atarist
			    if (__mint > 0)
# endif
		        {
				bas_ecran();
				eff_ligne();
				fflush(stdout);
				reset_clavier();
				reset_ecran();
				goodbye();
				raise(SIGTSTP);  /* STOP */
				init_ecran();
				init_clavier();
		        }
			    break;
#endif
	        case 'x':
			    terminer();
			    break;
	        case '+':
			    eff_ecran();
			    no_joueur = choix_d_un_joueur_au_clavier(WHO_COMES, joueurs_inscrits, &ligne);
			    if (no_joueur == TOUS_LES_JOUEURS) {
				    for (k = 0; k < joueurs_inscrits->n; k++)
					    es_joueur(joueurs_inscrits->liste[k]->numero, 1);
				    goto touche; /* Laisser les messages a l'ecran */
			    }
				else if (no_joueur >= 0) { es_joueur(no_joueur, 1); goto touche; }
				    break;
	        case '-':
			    eff_ecran();
			    no_joueur = choix_d_un_joueur_au_clavier(WHO_LEAVES,joueurs_inscrits, &ligne);
			    if (no_joueur == TOUS_LES_JOUEURS) {
				    for (k = 0; k < joueurs_inscrits->n; k++)
					    es_joueur(joueurs_inscrits->liste[k]->numero, 0);
				    goto touche; /* Laisser les messages a l'ecran */
			    }
				else if (no_joueur >= 0) { es_joueur(no_joueur, 0); goto touche; }
				    break;
	        case 'n':
			    printf("\n\n");
			    if ((no_joueur = choix_d_un_joueur_au_clavier(WHICH_PLAYER, NULL, &ligne)) <= 0)
				    break;
				{
				Joueur *j = trouver_joueur(no_joueur);
				if (j == NULL)
					beep();
				else {
					eff_ligne();
					printf("%s (%ld)\n", j->nom, j->numero);
					printf( OLD_NATION "%s\n", j->pays);
					printf( NEW_NATION);
					ligne = lire_ligne();
					if (ligne[0]) {
						change_nationalite(no_joueur, ligne);
						ajouter_joueur(joueurs_emigres, j);
					}
				}
				}
				break;
	        case 'r':
			    entree_resultats();
			    break;
		    case 'c':
			    correction_resultat();
			    break;
		    case 'w':
			    if (!peut_allouer_memoire_cross_table()) {
				    eff_ecran();
				    printf(MSG_CROSS_MEMORY);
				    beep();
				    goto touche;
			    } else {
				    afficher_cross_table();
				    liberer_memoire_cross_table();
			    }
			    break;
	        case 'a':
			    calcul_appariements();
			    break;
		    case 'm':
			    manipule_appariements();
			    break;
	        case 'e':
			    cree_fichier_elo();
			    break;
	        case '\n':		/* Ctrl-J */
	        case '\r':		/* Ctrl-M */
	        case '\f':		/* Ctrl-L */
	        case '\022':	/* Ctrl-R */
			    break;  /* redessiner l'ecran */
	        default:
			    beep();
			    goto lire;
	    }       /* switch */
    }           /* for(;;) */
}
