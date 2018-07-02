/*
 * global.c: fonctions qu'on ne sait pas ou mettre
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, Ajout de la variable 'generer_fichiers_XML' indiquant
                     s'il faut generer des fichiers xml en sortie.
                     Ajout du drapeau _POSIX_C_SOURCE pour permettre la compilation
                     avec les options -ansi et -pedantic sur Linux.
 * (EL) 20/07/2008 : v1.33, On sauve la date du tournoi dans le fichier intermediaire.
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 06/04/2007 : Ecriture de 'myfopen_dans_sous_dossier()' qui essaie de creer le
                     sous-dossier avant d'ouvrir un fichier.
 * (EL) 05/04/2007 : Ecriture de la fonction 'init_fichier_intermediaire' qui initialise
                     un fichier intermediaire (en recopiant evnt un fichier a l'ancien format)
 * (EL) 05/04/2007 : Ecriture de la fonction 'backup_inter()' qui effectue une sauvegarde
                     du fichier intermediaire dans un sous-dossier si on l'utilise.
 * (EL) 05/04/2007 : Ajout de la variable 'nom_fichier_inter_backup' permettant de stocker
                     le fullname du fichier intermediaire AVEC le fullname du sous-dossier pour la
                     sauvegarde dans ce sous-dossier.
 * (EL) 30/03/2007 : Tranformation de 'COPIER()' en fonction et changement de son code
                     ainsi que des parametres.
                     Ecriture de 'CONCAT()' permettant de concatener 2 chaines.
 * (EL) 30/03/2007 : Ajout des variables 'nom_sous_dossier' et 'utiliser_sous_dossier'
                     qui servent a regrouper tous les fichiers ronde/class/result...
                     dans un sous-dossier portant le fullname du tournoi.
 * (EL) 29/03/2007 : Reecriture de la fonction 'grand_dump()' pour coller avec
                     le nouveau format du fichier de configuration.
 * (EL) 06/02/2007 : Ajout de la fonction 'copier_fichier()'
 * (EL) 05/02/2007 : Ajout des variables 'nom_du_tournoi' et 'nombre_de_rondes'
 * (EL) 04/02/2007 : Ajout de deux variables pour memoriser le type des fichiers
                     de config et intermediaire (ancienne ou nouvelle version)
 * (EL) 02/02/2007 : changement du type du coefficient de Brightwell en double.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#define  _POSIX_C_SOURCE 1L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>

#include "appari.h"
#include "global.h"
#include "couplage.h"
#include "more.h"
#include "version.h"

#ifdef atarist
# include <mintbind.h>
#endif

#if defined(UNIX_BSD) || defined(UNIX_SYSV)
    #include <sys/signal.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/stat.h>
#endif

#if defined(CYGWIN) || defined(MACOSX) || defined(LINUX)
    #include <signal.h>
#endif

/* Variables pour gerer les anciens fichiers
   de configuration et intermediaire
*/

long
    type_fichier_config,
    type_fichier_intermediaire,
	fichier_des_nouveaux ; /* indique si c'est le fichier des nouveaux qu'on lit */

char
    *nom_du_tournoi ="",
    *nom_sous_dossier ="",
	*date_tournoi = "" ;

long
    nombre_de_rondes = 5 ;

/* Penalites et tableaux de penalites elementaires */

double
    coef_brightwell ; /* Le coeff de brightwell en flottant, 2 pts par victoire */

long
    current_round,
    sauvegarde_immediate,
    sauvegarde_fichier_result,
    sauvegarde_fichier_classement,
    sauvegarde_fichier_class_equipes,
    sauvegarde_fichier_appariements,
    sauvegarde_fichier_crosstable_HTML,
    sauvegarde_fichier_elo,
    utiliser_sous_dossier,
    generer_fichiers_XML,
    impression_automatique,
    nb_copies_impression,
    aff_diff_scores,
    nb_chiffres_des_scores,
    nmax_flottement,
    nmax_couleur;

discs_t
    score_bip;

pen_t
    *penalite_couleur,
    penalite_repcoul,
    *penalite_flottement,
    *penalite_chauvinisme,
    penalite_flcum,
    minoration_fac,
#ifdef ELITISM
    *penalite_elitisme,
#endif
    penalite_mcoul,
    penalite_copp,
    penalite_desuite,
    penalite_bipbip;

char
    *pays_defaut                 = "",
    *nom_fichier_joueurs         = "joueurs",
    *nom_fichier_nouveaux        = "nouveaux.txt",
    *nom_fichier_inter           = "papp-internal-workfile.txt",
    *nom_fichier_inter_backup    = "papp-internal-workfile-BACKUP.txt", /* backup dans le sous-dossier si necessaire */
    *nom_fichier_log             = "papp.log",
    *nom_fichier_config          = "papp.cfg",
    *nom_fichier_appariements    = "round###.txt",
    *nom_fichier_result          = "stand###.txt",
    *nom_fichier_classement      = "stand###.txt",
    *nom_fichier_class_equipes   = "team###.txt",
    *nom_fichier_crosstable_HTML = "cross###.htm",
    *nom_fichier_elo             = "papp.elo",
    *nom_programme,
#ifdef ENGLISH
    *couleur_1                 = "Black",
    *couleur_2                 = "White";
#else
    *couleur_1                 = "Noir",
    *couleur_2                 = "Blanc";
#endif


void init_penalites_defaut (void) {
    long i;

    /* Departage de Brightwell */
    coef_brightwell = 3.0; /* On compte 2 points par victoire */

        /* Ne faire les sauvegardes que si necessaire */
    sauvegarde_immediate = 1;
    sauvegarde_fichier_result = 0;
    sauvegarde_fichier_classement = 0;
    sauvegarde_fichier_class_equipes = 0;
    sauvegarde_fichier_appariements = 0;
    sauvegarde_fichier_crosstable_HTML = 0;
    sauvegarde_fichier_elo = 0;
    utiliser_sous_dossier = 1 ;
    generer_fichiers_XML = 0 ;
    impression_automatique = 0;
    nb_copies_impression = 1;

    /* Afficher le nombre de pions, pas leur difference */
    aff_diff_scores = 0;

    /* Bip fait 24 pions sur 64 */
    score_bip   = INTEGER_TO_SCORE(24);
    total_pions = 64;
    nb_chiffres_des_scores = nombre_chiffres(total_pions);

    /*
     * Penalites de couleur par defaut
     */
    nmax_couleur = 6;
    CALLOC(penalite_couleur, nmax_couleur, pen_t);
    penalite_couleur[0] =     0;
    penalite_couleur[1] =    20;
    penalite_couleur[2] =   105;
    penalite_couleur[3] =   225;
    penalite_couleur[4] =   530;
    penalite_couleur[5] =  1075;
    penalite_repcoul    =    15;

    /*
     * Penalites de flottement par defaut
     */
    nmax_flottement = 25;
    CALLOC(penalite_flottement, nmax_flottement, pen_t);
    penalite_flottement [0] =           0;
    penalite_flottement [1] =       20000;
    penalite_flottement [2] =       40588;
    penalite_flottement [3] =       62785;
    penalite_flottement [4] =       87119;
    penalite_flottement [5] =      114041;
    penalite_flottement [6] =      143998;
    penalite_flottement [7] =      177453;
    penalite_flottement [8] =      214908;
    penalite_flottement [9] =      256910;
    penalite_flottement[10] =      304062;
    penalite_flottement[11] =      357037;
    penalite_flottement[12] =      416582;
    penalite_flottement[13] =      483534;
    penalite_flottement[14] =      558828;
    penalite_flottement[15] =      643514;
    penalite_flottement[16] =      738767;
    penalite_flottement[17] =      845906;
    penalite_flottement[18] =      966413;
    penalite_flottement[19] =     1101949;
    penalite_flottement[20] =     1254381;
    penalite_flottement[21] =     1425805;
    penalite_flottement[22] =     1618578;
    penalite_flottement[23] =     1835345;
    penalite_flottement[24] =     2079081;
    penalite_flcum          =          25;
    minoration_fac          =          10;

    /*
     * Les penalites de repetition par defaut sont infinies
     */
    penalite_bipbip  =  MAX_PEN;
    penalite_copp    =  5000000;
    penalite_mcoul   =  MAX_PEN;
    penalite_desuite =        1;

    /*
     * Penalites de chauvinisme par defaut
     */
    CALLOC(penalite_chauvinisme, NMAX_ROUNDS, pen_t);
    for (i = 0; i < NMAX_ROUNDS; i++)
            penalite_chauvinisme[i] = 0;
    penalite_chauvinisme[0] = 2;

    /*
     * Penalite d'elitisme par defaut
     */
#ifdef ELITISM
    CALLOC(penalite_elitisme, NMAX_ROUNDS, pen_t);
    penalite_elitisme[0] = 100;
    penalite_elitisme[1] = 100;
    penalite_elitisme[2] = 100;
    penalite_elitisme[3] = 200;
    penalite_elitisme[4] = 200;
    penalite_elitisme[5] = 300;
    penalite_elitisme[6] = 300;
    penalite_elitisme[7] = 300;
    penalite_elitisme[8] = 300;
        /* on veut favoriser les bons appariements pour
           le haut du classement vers la fin du tournoi */
        for (i = 9; i < NMAX_ROUNDS; i++)
                penalite_elitisme[i] = 400;
#endif

}

#ifdef ENGLISH
# define VPEN_COLORS    "non-increasing colour penalties"
# define VPEN_FLOAT     "non-increasing float penalties"
# define VPEN_MINFAC    "value of `min_fac' is too big"
# define FATAL_OPEN     "ne peut etre ouvert (pb de droits ?) : "
# define FATAL_MSG      "%s: fatal error: %s\n"
# define FATAL_HAK      "Hit any key to quit."
#else
# define VPEN_COLORS    "penalites de couleur non croissantes"
# define VPEN_FLOAT     "penalites de flottement non croissantes"
# define VPEN_MINFAC    "la valeur de `min_fac' est trop grande"
# define FATAL_OPEN     "ne peut etre ouvert (pb de droits ?) : "
# define FATAL_MSG      "%s: erreur fatale: %s\n"
# define FATAL_HAK      "Pressez une touche pour quitter."
#endif

void verification_penalites (void) {
    long i;

    assert(nmax_couleur >= 0 && nmax_flottement >= 0);
    assert(penalite_couleur[0] == 0);
    assert(penalite_flottement[0] == 0);

    /* Monotonie des penalites de couleur */
    for (i = 0; i < nmax_couleur-1; i++)
        if (penalite_couleur[i] > penalite_couleur[i+1])
            fatal_error(VPEN_COLORS);

    /* Penalites de flottement */
    if (2 * minoration_fac > penalite_flottement[1])
        fatal_error(VPEN_MINFAC);
    for (i = 0; i < nmax_flottement-1; i++)
        if (penalite_flottement[i] > penalite_flottement[i+1])
            fatal_error(VPEN_FLOAT);
}

/*
 * Gestion des signaux
 */
static void _terminer(void) EXITING;    /* declaration forward */

#if defined(UNIX_BSD) || defined(UNIX_SYSV)
typedef void (*Signal_handler)(int);
static sigset_t old_sigmask;

#ifndef SIGNAUX_POSIX
void bloquer_signaux() {
    old_sigmask = sigblock( (1L<<SIGHUP) | (1L<<SIGINT) |
                            (1L<<SIGQUIT) | (1L<<SIGTERM) );
}

void debloquer_signaux() {
    sigsetmask(old_sigmask);
}
#else
void bloquer_signaux() {
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    sigaddset(&set, SIGTERM);
    sigprocmask(SIG_BLOCK, &set, &old_sigmask);
}

void debloquer_signaux() {
    sigprocmask(SIG_SETMASK, &old_sigmask, NULL);
}
#endif

#ifndef SIGNAUX_POSIX
    #define handle_signal(sig, fn)  signal((sig),(Signal_handler)(fn))
#else
    #define handle_signal(sig, fn)  do {           \
        struct sigaction act;                   \
        act.sa_handler = (Signal_handler)(fn);  \
        sigemptyset(&act.sa_mask);              \
        act.sa_flags = 0;                       \
        sigaction((sig), &act, NULL);           \
    } while(0)
#endif

/* changement par Stephane Nicolet, 27/05/2000
 * je ne sais pas ou est defini winsize sur Unix,
 * tant pis on ne fera pas de changement de taille
 * de fenetre
 */
static void changement_taille(void) {
    /*
     struct winsize ecran;

     if (ioctl(1, TIOCGWINSZ, &ecran) < 0)
     return;
     nb_lignes   = ecran.ws_row;
     nb_colonnes = ecran.ws_col;
# if defined(UNIX_SYSV) && !defined(SIGNAUX_POSIX)
     handle_signal(SIGWINCH, changement_taille);
# endif
     */
}

void installer_signaux() {
    handle_signal(SIGHUP, _terminer);
    handle_signal(SIGINT, SIG_IGN);
    handle_signal(SIGQUIT, terminate);
    handle_signal(SIGTERM, _terminer);
    /*      handle_signal(SIGWINCH, changement_taille);    cf plus haut, SN */
}

#else

/* pas de signaux */
void bloquer_signaux()          { }
void debloquer_signaux()        { }
void installer_signaux()        { }

#endif

/*
 * Terminaison, normale ou anormale, de PAPP.
 */
void fatal_error(const char *erreur) {
    fprintf(stdout, "\n" FATAL_MSG,
            nom_programme[0] ? nom_programme : "Papp", erreur);
#if defined(__THINK_C__) || defined(PAPP_MAC_METROWERKS)
    /*
     * Attendre une pression de touche avant de quitter (afin de pouvoir
     * lire les messages d'erreur)
     */
    fprintf(stdout, FATAL_HAK);
    (void)lire_touche();
#endif

    oter_verrou();
    reset_clavier() ;
    reset_ecran();
    exit(1);
}

void terminate() {
#if defined(UNIX_BSD) || defined(UNIX_SYSV)
    /* peut-etre ne pouvons-nous plus ecrire sur ce terminal */
    handle_signal(SIGTTIN, _terminer);
    handle_signal(SIGTTOU, _terminer);
#endif
#ifndef PAPP_MAC_METROWERKS
    bas_ecran();
    eff_ligne();
#endif
    eff_ecran();
    fflush(stdout);
    reset_clavier();
    reset_ecran();
    goodbye();
    _terminer();
}

static void _terminer() {
    /* Nous devons sauvegarder l'etat */
    sauve_inscrits();
    sauve_appariements();
#ifdef atarist
    Psigreturn();
#endif
    oter_verrou();
    exit(0);
}

/*
 * Les fonctions suivantes sont protegees contre les signaux
 */

void sauve_ronde() {
    bloquer_signaux();
    _save_round();
    debloquer_signaux();
}

void sauve_inscrits() {
    bloquer_signaux();
    _save_registered();
    debloquer_signaux();
}

void sauve_appariements() {
    bloquer_signaux();
    _save_pairings();
    debloquer_signaux();
}

void recreer_fichier_intermediaire() {
    bloquer_signaux();
    _recreate_workfile();
    debloquer_signaux();
}

/*
 * grand_dump(): affichage de tous les parametres a un moment donne
 * chpen(): convertit une penalite en une chaine de sept caracteres
 */

char *fscore (discs_t score) {
    char *chaine;

    if (!SCORE_IS_LEGAL(score))
        return NULL;

    chaine = new_string();
    if (aff_diff_scores) {

        if (IS_VICTORY(score)) chaine[0] = '+';
        if (IS_DEFEAT(score))  chaine[0] = '-';
        if (IS_DRAW(score)) chaine[0] = '=';

        sprintf(chaine+1, "%s", pions_en_chaine(ABSOLUTE_VALUE_SCORE(RELATIVE_SCORE(score))));
    } else {
        sprintf(chaine, "%s/%s", pions_en_chaine(score), pions_en_chaine(OPPONENT_SCORE(score)));
    }
    return chaine;
}

static char *fpen (pen_t p) {
    static char buf[20];

    if (p >= MAX_PEN)
        return " INFINI";
    sprintf(buf, "%7ld", (long)p);
    return buf;
}

long nombre_chiffres (long n) {
    long k = 0;

    if (n == 0)
        return 1;
    else if (n < 0)
        ++k, n = -n;

    while(n)
        ++k, n /= 10;
    return k;
}

/*
 * grand_dump(): sauve les parametres courants dans le fichier pointe
 * par fp. Je laisse les choses en francais pour le moment.
 */

void grand_dump (FILE *fp) {
    long i, l;
    char *buf;

    buf = new_string();

    fprintf(fp,
            "#! papp\n"
"#\n"
"# Ce fichier de configuration a ete cree automatiquement par PAPP parce\n"
"# qu'il n'y en avait aucun dans votre repertoire. Il indique quels sont\n"
"# les parametres pris par defaut. Libre a vous de modifier ces parametres\n"
"# pour adapter PAPP a vos besoins.\n\n"
                ) ;


    fprintf(fp,
"# Fichier de configuration pour Papp 1.34\n"
"# Configuration file for Papp 1.34\n"
"###############################################################################\n\n"
"###############################################################################\n"
"## Ces parametres vont aller dans le fichier \"papp-internal-workfile.txt\" et sont\n"
"## modifies a chaque nouveau tournoi.\n"
		   );
	fprintf(fp,
"## These parameters are now in the \"papp-internal-workfile.txt\" file and are modified\n"
"## for each new tournament.\n"
"##\n"
"##  Brightwell  = 3;                    # Coefficient de Brightwell\n"
"##\n"
		   );
	fprintf(fp,
"## On organise un toutes-rondes s'il y a entre 1 et 12 joueurs:\n"
"## A round-robin is organized if there are between 1 and 12 players:\n"
"##  Toutes-rondes = 1-12 joueurs;\n"
"###############################################################################\n\n\n"
"###############################################################################\n"
		   );
	fprintf(fp,
"###############################################################################\n"
"## Parametres que l'utilisateur peut vouloir changer:\n"
"## The user may want to change these parameters:\n\n"
            ) ;

    fprintf(fp,"# Pays par defaut / default country\n\tPays        = \"%s\";\n\n", pays_defaut);
    fprintf(fp,"# Bip fait 24 pions sur 64 / BYE score and total number of discs\n"
            "\tScore-bip   = %s/%ld;\n\n", pions_en_chaine(score_bip), total_pions);
    fprintf(fp,"# Utilisation d'un sous-dossier / use a folder for all files (true/false)\n"
            "\tdossier %s;\n\n", utiliser_sous_dossier ? "true" : "false") ;
    fprintf(fp,"# Generation des fichiers XML en sortie / generate XML output files (true/false)\n"
            "\tXML %s;\n\n", generer_fichiers_XML ? "true" : "false") ;
    fprintf(fp,"# Fichiers des appariements de chaque ronde / round pairings files\n"
            "\tFichier appariements   = \"%s\";\n\n", nom_fichier_appariements);
    fprintf(fp,"# Fichiers des resultats de chaque ronde / round results files\n"
            "\tFichier resultats      = \"%s\";\n\n", nom_fichier_result);
    fprintf(fp,"# Fichiers du classement de chaque ronde / round standings files\n"
            "\tFichier classement     = \"%s\";\n\n", nom_fichier_classement);
    fprintf(fp,"# Fichiers des tableaux HTML / HTML crosstables files\n"
            "\tFichier tableau-croise = \"%s\";\n\n", nom_fichier_crosstable_HTML);
    fprintf(fp,"# Fichiers resultats par equipes / team standings files\n"
            "#\tFichier equipes        = \"%s\";\n", nom_fichier_class_equipes);
    fprintf(fp,
"###############################################################################\n"
"###############################################################################\n\n\n"
"###############################################################################\n"
"## Parametres normalement fixes.\n"
"## Parameters that normally should not be changed.\n\n"
"# Les fichiers utilises par Papp / files used by Papp\n"
            );
    sprintf(buf, "\tFichier joueurs        = \"%s\";", nom_fichier_joueurs);
    fprintf(fp, "%-42s # Fichier principal des joueurs\n", buf);
    sprintf(buf, "\tFichier nouveaux       = \"%s\";", nom_fichier_nouveaux);
    fprintf(fp, "%-42s # Fichier des nouveaux joueurs\n", buf);
    sprintf(buf, "\tFichier inter          = \"%s\";", nom_fichier_inter);
    fprintf(fp, "%-42s # Fichier intermediaire\n\n", buf);

    fprintf(fp,"# Actuellement inutilise / currently unused\n") ;
    sprintf(buf, "#\t Fichier elo           = \"%s\";", nom_fichier_elo);
    fprintf(fp, "%-42s # Fichiers Elo de chaque ronde\n", buf);
    sprintf(buf, "#\t Fichier log           = \"%s\";", nom_fichier_log);
    fprintf(fp, "%-42s # Fichier de log\n", buf);
    sprintf(buf, "#\t Fichier config        = \"%s\";", nom_fichier_config);
    fprintf(fp, "%-42s # Ce fichier meme...\n\n\n", buf);
    fprintf(fp, "# Frequence des sauvegardes / saves frequency\n\n"
            "\tSauvegarde %s;\n\n\n", sauvegarde_immediate ? "immediate" : "differee");

    fprintf(fp, "# Type d'impressions des fichiers de resultats et de classement.\n");
    fprintf(fp, "# Deux types d'impression possibles : automatique NN si vous voulez\n");
    fprintf(fp, "# que PAPP imprime lui-meme NN copies des fichiers, manuelle si \n");
    fprintf(fp, "# vous preferez utiliser un editeur de texte.\n\n");
    fprintf(fp, "\tImpression %s;\n\n\n",impression_automatique ? "automatique 1" : "manuelle");
    fprintf(fp, "# Couleurs du premier et du second joueur\n\n"
            "\tCouleurs = { \"%s\", \"%s\" };\n\n\n",
            couleur_1, couleur_2 );
    fprintf(fp, "# Doit-on afficher les nombres de pions ou leur difference ?\n"
            "# Should we print absolute score or score difference?\n\n"
            "\tAffichage-pions %s;\n\n", aff_diff_scores ? "relatif" : "absolu" ) ;
    fprintf(fp,
            "# Zone d'insertion pour les nouveaux joueurs\n"
            "# Number ranges for new players\n"
			"\tzone-insertion \"ARG\" = 280000-289999;  # argentins\n"
			"\tzone-insertion \"AUS\" = 260000-269999;  # australiens\n"
			"\tzone-insertion \"A\"   = 310000-319999;  # autrichiens\n"
			"\tzone-insertion \"B\"   = 270000-279999;  # belges\n"
			"\tzone-insertion \"BLR\" = 360000-369999;  # bielorusses\n"
		   );
	fprintf(fp,
			"\tzone-insertion \"BR\"  = 290000-299999;  # bresiliens\n"
			"\tzone-insertion \"BG\"  = 400000-409999;  # bulgares\n"
			"\tzone-insertion \"CDN\" = 110000-119999;  # canadiens\n"
			"\tzone-insertion \"CN\"  =  60000-69999;   # chinois\n"
			"\tzone-insertion \"CRO\" = 230000-234999;  # croates\n"
			"\tzone-insertion \"CZ\"  = 210000-219999;  # tcheques\n"
		   );
	fprintf(fp,
			"\tzone-insertion \"D\"   =  70000-79999;   # allemands\n"
			"\tzone-insertion \"DK\"  = 220000-229999;  # danois\n"
			"\tzone-insertion \"E\"   =  90000-99999;   # espagnols\n"
			"\tzone-insertion \"F\"   =  50000-59999;   # francais\n"
			"\tzone-insertion \"FIN\" =  80000-89999;   # finlandais\n"
			"\tzone-insertion \"GR\"  = 300000-309999;  # grecs\n"
		   );
	fprintf(fp,
			"\tzone-insertion \"GB\"  = 100000-109999;  # anglais\n"
			"\tzone-insertion \"HK\"  = 170000-179999;  # hong-kong\n"
			"\tzone-insertion \"I\"   =  40000-49999;   # italiens\n"
			"\tzone-insertion \"RI\"  = 180000-189999;  # indonesiens\n"
			"\tzone-insertion \"IL\"  = 200000-209999;  # israeliens\n"
			"\tzone-insertion \"IND\" = 350000-359999;  # indiens\n"
		   );
	fprintf(fp,
			"\tzone-insertion \"IRA\" = 390000-399999;  # iraniens\n"
			"\tzone-insertion \"ISL\" = 330000-339999;  # islandais\n"
			"\tzone-insertion \"J\"   =  10000-19999;   # japonais\n"
			"\tzone-insertion \"MAL\" = 160000-169999;  # malais\n"
			"\tzone-insertion \"MEX\" = 340000-349999;  # mexicains\n"
			"\tzone-insertion \"N\"   = 140000-149999;  # norvegiens\n"
		   );
	fprintf(fp,
			"\tzone-insertion \"NL\"  = 120000-129999;  # hollandais\n"
			"\tzone-insertion \"PL\"  = 130000-139999;  # polonais\n"
			"\tzone-insertion \"PK\"  = 380000-389999;  # pakistanais\n"
			"\tzone-insertion \"RO\"  = 370000-379999;  # roumains\n"
			"\tzone-insertion \"ROK\" =  30000-39999;   # sud-coreens\n"
			"\tzone-insertion \"RUS\" = 320000-329999;  # russes\n"
		   );
	fprintf(fp,
			"\tzone-insertion \"SRB\" = 235000-239999;  # serbes\n"
			"\tzone-insertion \"S\"   = 150000-159999;  # suedois\n"
			"\tzone-insertion \"CH\"  = 240000-249999;  # suisses\n"
			"\tzone-insertion \"SGP\" = 250000-259999;  # singapour\n"
			"\tzone-insertion \"T\"   = 190000-199999;  # thai\n"
			"\tzone-insertion \"USA\" =  20000-29999;   # americains\n"
			"\tzone-insertion \"PG\"  =   3200-3349;    # programmes\n"
			"\tzone-insertion         =   9000-9500;    # autres\n\n\n"
            );
    /*
     * Table des penalites
     */

    fprintf(fp,
"# La table des penalites est divisee en cinq sections: couleur,\n"
"# flottement, repetition, chauvinisme, elitisme. Ces sections\n"
"# peuvent apparaitre dans un ordre quelconque. Si une section\n"
"# n'apparait pas, les valeurs par defaut des penalites de cette\n"
"# section sont conservees. Si elle apparait, toutes les penalites\n"
"# de la section sont initialisees a zero.\n\n"
            );
    fprintf(fp,"Penalites {\n");

    /*
     * Couleur
     */

    fprintf(fp,"\tCouleur:\n");
    for (i = 1; i < nmax_couleur; i++) {
        sprintf(buf, "%ld", i);
        if (i == nmax_couleur - 1)
            strcat(buf, "+");
        fprintf(fp,"\t\t%-3sfois  = %s;\n",
                buf, fpen(penalite_couleur[i]));
    }
    fprintf(fp,"\t\tde-suite = %s;\n", fpen(penalite_repcoul));

    /*
     * Flottement
     */

    fprintf(fp,"\n\tFlottement:\n");
    for (i = 1; i < nmax_flottement; i++) {
        sprintf(buf, "%ld", i);
        if (i == nmax_flottement - 1)
            strcat(buf, "+");
        fprintf(fp,"\t\t%-3sdemi-point%c  =     %s;\n",
                buf, i==1 ? ' ':'s', fpen(penalite_flottement[i]));
    }
    fprintf(fp,"\t\tde-suite        = %s;\n", fpen(penalite_flcum));
    fprintf(fp,"\t\tminoration      = %s;\n", fpen(minoration_fac));

    /*
     * Repetition
     */

    fprintf(fp,"\n\tRepetition:\n");
    fprintf(fp,"\t\tbip-bip           = %s;\n", fpen(penalite_bipbip));
    fprintf(fp,"\t\tcouleurs-opposees = %s;\n", fpen(penalite_copp));
    fprintf(fp,"\t\tmemes-couleurs    = %s;\n", fpen(penalite_mcoul));
    fprintf(fp,"\t\tde-suite          = %s;\n", fpen(penalite_desuite));

    /*
     * Chauvinisme
     */

    for (l = i = NMAX_ROUNDS-1; l >= 0; l--)
        if (penalite_chauvinisme[l] != penalite_chauvinisme[i])
        { ++l; break; }

            fprintf(fp,"\n\tChauvinisme:\n");
    for (i = 0; i <= l; i++) {
        sprintf(buf, "%ld", i+1);
        if (i == l)
            strcat(buf, "+");
        fprintf(fp,"\t\tronde  %-10s = %s;\n", buf,
                fpen(penalite_chauvinisme[i]));
    }
    fprintf(fp, "\t\tronde  12+        =  0; # Mettre INFINI pour le championnat du monde.\n"
            "                                        # INFINI should be used for the WOC.\n");

#ifdef ELITISM
    /*
     * Elitisme
     */
    for (l = i = NMAX_ROUNDS-1; l >= 0; l--)
        if (penalite_elitisme[l] != penalite_elitisme[i])
        { ++l; break; }

            fprintf(fp,"\n\tElitisme:\n");
    for (i = 0; i <= l; i++) {
        sprintf(buf, "%ld", i+1);
        if (i == l)
            strcat(buf, "+");
        fprintf(fp,"\t\tronde  %-10s = %s;\n", buf,
                fpen(penalite_elitisme[i]));
    }
#endif

    fprintf(fp,"};\n");
}

/* Initialisation de l'alloc-ring */
static long allo_ring_initialise = 0;
long  alloc_ring_index = 0;
char *alloc_ring[ALLOC_RING_SLOTS];

void init_alloc_ring (void) {
    long i;

    if (!allo_ring_initialise) {
        for (i = 0; i < ALLOC_RING_SLOTS; i++)
            CALLOC(alloc_ring[i], ALLOC_RING_LENGTH, char);
        allo_ring_initialise = 1;
    }
}

char *new_string (void) {
    /* faut-il initialise l'alloc-ring ? */
    if (!allo_ring_initialise)
        init_alloc_ring();
    /*
     * renvoyer la premiere chaine vierge,
     * ou la plus ancienne si elles sont toutes prises
     */
    if (++alloc_ring_index >= ALLOC_RING_SLOTS)
        alloc_ring_index = 0;
    return alloc_ring[alloc_ring_index];
}

/*
 * trivialloc: allocation triviale de memoire, sans alignement particulier,
 * et cette memoire ne pourra jamais etre liberee! Tres utile cependant
 * pour allouer un grand nombre de petites chaines de caracteres.
 */

#ifdef __BORLANDC__
  #define MINIMAL_BLOCK_SIZE 8192
  #include <malloc.h>
#else
  #define MINIMAL_BLOCK_SIZE 8192
#endif

char *trivialloc (long longueur) {
    static char *bloc;
    char *allo;
    static long libre = 0;

    assert(longueur > 0);
    if (libre < longueur)
        CALLOC(bloc, libre = MINIMAL_BLOCK_SIZE, char);
    allo = bloc;
    bloc  += longueur;
    libre -= longueur;
    assert(libre >= 0);
    return allo;
}

/* Fonctions pour copier deux chaines de caracteres
 * et concatener deux chaines dans une troisieme
 */

void COPIER(const char *source, char **dest) {
    char *tmp ;

    assert((dest != NULL) && (source != NULL)) ;

    tmp = trivialloc(strlen(source)+1) ;
    strcpy(tmp, source) ;
    *dest = tmp ;
}

void CONCAT(const char *source, const char *ajout, char **dest) {
    char * tmp ;

    assert((dest != NULL) && ((source != NULL) || (ajout != NULL))) ;

    if (source == NULL) {
        COPIER(ajout, dest) ;
    } else if (ajout == NULL) {
        COPIER(source, dest) ;
    } else {
        tmp=trivialloc(strlen(source)+strlen(ajout)+1);
        strcpy(tmp,source) ;
        strcat(tmp,ajout) ;
        *dest = tmp ;
    }
}

/*
 * le_max_de et le_min_de : fonctions retournant le max et le min
 * de deux entiers longs. Il vaut mieux les nommer ainsi pour eviter
 * des problemes dans les environnements definissant leurs propres
 * fonctions max et min
 *
 */

long le_max_de(long n1, long n2) {
    return ((n1 > n2) ? n1 : n2);
}

long le_min_de(long n1, long n2) {
    return ((n1 > n2) ? n2 : n1);
}


/*
 * Une petite fonction pour generer des noms de fichiers qui se suivent.
 * Si pattern contient la chaine "###", alors ### est remplace par ID;
 * sinon, on ajoute seulement ID a la fin de pattern.
 * Ex : pattern == "class###.txt" et ID == 11   -> on renvoie class_11.txt
 *      pattern == "classement" et ID == 11   -> on renvoie classement_11
 * Cette fonction alloue de la memoire pour la chaine resultat, qui ne sera jamais
 * liberee.
 */
char *nom_fichier_numerote(char *pattern, long numero) {
    char *nom, *occur_dieses;
    long len;

    assert( (numero >= 0) && (numero <= 999));

    len = strlen(pattern);
    nom = trivialloc( len + 4 );
    strcpy(nom, pattern);
    nom[len]   = '\0';
    nom[len+1] = '\0';
    nom[len+2] = '\0';
    nom[len+3] = '\0';
    occur_dieses = strstr(nom, "###");

    if (occur_dieses == NULL)
        occur_dieses = nom + len;

    occur_dieses[0] = ((numero < 100)? '_' : '0' + ((numero/100) % 10));
    occur_dieses[1] = ((numero < 10) ? '_' : '0' + ((numero/10) % 10));
    occur_dieses[2] = '0' + ((numero) % 10);

    return nom;
}

/* long debuts_differents(s1,s2,n);
 *  Renvoie 0 si
 *    - la longueur de s1 est plus petite que celle de s2, et
 *    - les n premiers caracteres des 2 chaines coincident;
 *  Renvoie 1 sinon.
 *  Pas de distinction majuscules/minuscules.
 */
long debuts_differents(const char *s1, const char *s2, long n) {
    long len1,len2,i;
    char c1,c2;

    assert((s1 != NULL) && (s2 != NULL));
    len1 = strlen(s1);
    len2 = strlen(s2);

    if (len1 > len2) return 1;

    if (n>len1) n=len1;
    if (n>len2) n=len2;

    for (i=0;i<n;i++)
    {
        c1=toupper(s1[i]);
        c2=toupper(s2[i]);
        if (c1 != c2) return 1;
    }
    return 0;
}

 /*
  * long compare_chaines_non_sentitif(const char *scan1, const char *scan2);
  * <0 for <, 0 for ==, >0 for >
  */
long compare_chaines_non_sentitif(const char *scan1, const char *scan2) {
    register char c1, c2;

    if (!scan1)
        return scan2 ? -1 : 0;
    if (!scan2) return 1;

    do {
        c1 = *scan1++; c1=toupper(c1);
        c2 = *scan2++; c2=toupper(c2);
    } while (c1 && c1 == c2);

    /*
     * The following case analysis is necessary so that characters
     * which look negative collate low against normal characters but
     * high against the end-of-string NUL.
     */
    if (c1 == c2)
        return(0);
    else if (c1 == '\0')
        return(-1);
    else if (c2 == '\0')
        return(1);
    else
        return(c1 - c2);
}


/* Recopie un fichier dans un autre */
/* Les fichiers sont deja ouverts   */
/* renvoie -1 si erreur             */
/* Convertit les fins de ligne      */
long copier_fichier(FILE *dest, FILE *source) {
    long c, cc ;
    unsigned long i, fin_ligne ;
    char str_ligne[256] ;

    if (dest == NULL || source == NULL)
        return -1 ;
    do {
        i=0 ; fin_ligne=0 ; /* on commence une nouvelle ligne */
        do {
            c = fgetc(source) ;
            if (c != EOF && c != 0x0D && c != 0x0A && i<(sizeof(str_ligne)-1))
                str_ligne[i++] = c ;
            else {
                fin_ligne = 1 ;
                if (c == 0x0D) {
                    cc = fgetc(source) ;
                    if (cc != 0x0A)
                        ungetc(cc, source) ;
                }
            }
        } while (!fin_ligne) ;
        str_ligne[i] = '\0' ;
        if (fprintf(dest,"%s\n", str_ligne) <0)
            return -1 ;
    } while (c != EOF) ;
    return 0 ;
}

/* Effectue une sauvegarde du fichier intermediaire */
/* dans le sous-dossier si on l'utilise */
void backup_inter(void) {
    FILE *papp_file ;
    FILE *papp_backup_file ;
    if (utiliser_sous_dossier != 1)
        return ;
    if ((papp_file = myfopen_dans_sous_dossier(nom_fichier_inter, "rb", "", 0, 0)) == NULL)
        return ;
    if ((papp_backup_file = myfopen_dans_sous_dossier(nom_fichier_inter_backup, "wb", nom_sous_dossier, utiliser_sous_dossier, 0)) == NULL) {
        fclose(papp_file) ;
        return ;
    }
    copier_fichier(papp_backup_file, papp_file) ;
    fclose(papp_file) ;
    fclose(papp_backup_file) ;
}

/* Sauvegarde les infos du tournoi dans le fichier intermediaire */
/* EN ECRASANT CE QU'IL Y A !! */
void sauver_infos_tournoi(void) {
    FILE *papp_file ;

    papp_file = myfopen_dans_sous_dossier(nom_fichier_inter, "wb", "", 0, 1) ;
    /* Sauvegarde des informations dans "papp-internal-workfile.txt"        */
    /* Attention, les mots-clefs doivent etre les memes                     */
    /* que dans l'analyse lexicale (pap.l).                                 */
    fprintf(papp_file, "#########################################\n") ;
    fprintf(papp_file, "# %s, %s\n", VERSION, __DATE__) ;
    fprintf(papp_file, "#########################################\n") ;
    fprintf(papp_file, "#_Nom-Tournoi   = \"%s\" ;\n", nom_du_tournoi) ;
    fprintf(papp_file, "#_Nombre-Rondes = %ld ;\n", nombre_de_rondes) ;
    fprintf(papp_file, "#_BQ-double = %f ;\n", coef_brightwell*2) ;
    fprintf(papp_file, "#_Date = \"%s\" ;\n", date_tournoi) ;
    fprintf(papp_file, "#########################################\n\n") ;
    fclose(papp_file) ;
}


/* Initialise un fichier intermediaire avec les infos du tournoi. */
/* Si type_fichier = OLD, on recopie a la suite l'ancien fichier */
void init_fichier_intermediaire(long type_fichier) {
    char tmp_filename[] = "temp.XXXXXX" ;
    FILE *tmpfile ;
    FILE *papp_file ;
    long err ;

    if (type_fichier == OLD) {
        /* On sauvegarde l'ancien "papp-internal-workfile.txt" */
        if (mktemp(tmp_filename) == NULL)
        	return ;
        papp_file = myfopen_dans_sous_dossier(nom_fichier_inter, "rb", "", 0, 1) ;
        if ((tmpfile = myfopen_dans_sous_dossier(tmp_filename, "wb", "", 0, 0)) == NULL) {
            fclose(papp_file) ;
            return ;
        }
        err = copier_fichier(tmpfile, papp_file) ;
        fclose(papp_file) ;
        fclose(tmpfile) ;
        if (err == -1)
            return ;

    }
    sauver_infos_tournoi() ; /* recree un fichier intermediaire */
    if (type_fichier == OLD) {
        papp_file = myfopen_dans_sous_dossier(nom_fichier_inter, "ab", "", 0, 1) ;
        /* On recopie l'ancien "papp-internal-workfile.txt" */
        if ((tmpfile = myfopen_dans_sous_dossier(tmp_filename, "rb", "", 0, 0)) != NULL) {
            copier_fichier(papp_file, tmpfile) ;
            fclose(tmpfile) ;
            remove(tmp_filename) ;
        }
        fclose(papp_file) ;

    }
}

/* fopen() ne sait pas ouvrir un fichier se trouvant dans un sous-dossier ("dir/truc.txt")  */
/* si le sous-dossier n'existe pas. Il faut d'abord le creer puis ouvrir le fichier         */
/* Cette fonction verifie
 * 1. qu'on utilise le sous-dossiers (variable 'utilise' vaut 1) ;
 * 2. que le sous-dossier de fullname indique existe ou peut etre creer
 * puis ouvre le fichier indique avec son mode d'ouverture                                  */
/* -> Affiche une erreur fatale si le fichier ne s'ouvre pas et que le param 'err' est a 1  */

FILE *myfopen_dans_sous_dossier(const char *filename, const char *mode, const char *folder, long utilise, long err) {
    FILE *fp ;
    char ligne[512] ;

    assert(filename && folder) ;
    assert(filename[0] != '\0') ;
    if (utilise == 1) {
        /* On essaie de creer le sous-dossier */
        /* S'il existe, tant mieux !          */
        mkdir(folder, 0777) ;
    }
    fp = fopen(filename, mode) ;
	if ((fp == NULL) && (err == 1)) {
        sprintf(ligne, "%s " FATAL_OPEN "%s", filename, strerror(errno)) ;
        fatal_error(ligne);
    }
    return fp ;
}
