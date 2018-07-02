/* global.h
 *
 * - definitions diverses
 * - various definitions
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, add two function prototypes for xml saves
 * (EL) 05/05/2008 : v1.33 All 'int' become 'long' to force 4 bytes storage.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 30/03/2007 : Remove 'CONCAT()' macro and turn it into function.
 * (EL) 06/02/2007 : Add 'copier_fichier()' function
 * (EL) 05/02/2007 : Add 'nom_du_tournoi' and 'nombre_de_rondes' variables
 * (EL) 04/02/2007 : Add two variables to save type of config file and work file
                     (old or new version)
 * (EL) 02/02/2007 : change Brightwell coefficient type to 'double'
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef __Global_h__
#define __Global_h__

#include <stdio.h>
#include <stdlib.h>
#include "appari.h"
#include "changes.h"
#include "more.h"
#include "pions.h"

/*
 *  global.c and pap.l prototypes
 */

#define CONFIG_F   300
#define PLAYERS_F  301
#define NEWPLAYERS_F 302
long lire_fichier (char *filename, long type);
void erreur_syntaxe(const char *erreur);
void avert_syntaxe (const char *erreur);

void bloquer_signaux(void), debloquer_signaux(void);
void installer_signaux(void);
void sauve_ronde(void);
void sauve_inscrits(void);
void sauve_appariements(void);
void recreer_fichier_intermediaire(void);
void grand_dump (FILE *fp);
void init_penalites_defaut (void);
void verification_penalites (void);
long nombre_chiffres (long n);
char *trivialloc (long longueur);
void COPIER(const char *source, char **dest) ;
void CONCAT(const char *source, const char *ajout, char **dest) ;
char *fscore(discs_t score);

/* This functions terminate the program -
 * Les fonctions suivantes terminent le programme */
#ifdef __GNUC__
# define EXITING  __attribute__ ((noreturn))
/* #elif defined(__BORLANDC__) */
/* # define EXITING {} */
#else
# define EXITING
#endif

void fatal_error(const char *erreur)  EXITING;
void terminate(void)         EXITING;

/*
 *  penalite.c prototypes
 */

void    calcul_appariements(void);

/*
 *  other prototypes
 */

void    calcul_departage(void);                 /* resultat.c */
void    fiche_individuelle(long, const char*);  /* resultat.c */
void    cree_fichier_elo(void);                 /* elo.c      */
void    copier_fichier_nouveaux(FILE *fp_dest); /* elo.c      */
int     yyparse(void);                          /* pap_tab.c  */

void    creer_classement_XML(void);             /* XML.c */
void    creer_ronde_XML(void);                  /* XML.c */


/*
 *  Macros for memory allocation
 */
#ifdef ENGLISH
# define ALLOC_FAILED   ": out of memory"
#else
# define ALLOC_FAILED   ": plus de memoire"
#endif

#define CALLOC(var,size,type)   do{             \
    var = (type*)malloc((size)*sizeof(type));           \
    if (!var)  fatal_error("malloc()" ALLOC_FAILED);  \
    } while(0)

#define REALLOC(var,size,type)  do{             \
    var = (type*)realloc(var,(size)*sizeof(type));      \
    if (!var)  fatal_error("realloc()" ALLOC_FAILED); \
    } while(0)


typedef int (*Sort_function)(const void*, const void*);
#define SORT(base,n,size,f) qsort((void*)base,n,size,(Sort_function)f)

/*
 *  Entry buffer size for terminal emulation -
 *  Taille du tampon d'entree pour l'emulation de terminal
 */

#define TAILLE_TAMPON   70

/*
 *  Alloc-ring management
 *  Gestion de l'alloc-ring
 */

#define ALLOC_RING_LENGTH 256
#define ALLOC_RING_SLOTS  32    /* 32 temporary strings can be used before chaos.
                                 * On peut utiliser 32 chaines temporaires
                                 * avant de se tirer dans le pied */
extern long  alloc_ring_index;
extern char *alloc_ring[ALLOC_RING_SLOTS];
void init_alloc_ring (void);            /* Do not forget - Ne pas oublier!!!!! */
char *new_string (void);

/*
 *  Global variables
 */

#define NMAX_ROUNDS     128

/* Variables to deal with old configuration file and work file.
 * Variables pour gerer les anciens fichiers de configuration et intermediaire
 */

#define OLD        -1
#define NONEXISTING  0
#define CORRECT     1

extern long
        type_fichier_config,
        type_fichier_intermediaire ,
		fichier_des_nouveaux ; /* indicate if the new players file is being read.
                                * indique si c'est le fichier des nouveaux qu'on lit */

/* User defined parameter.
 * Parametres definis par l'utilisateur */

extern char
        *nom_du_tournoi,
        *nom_sous_dossier,
		*date_tournoi ;

extern long
        nombre_de_rondes ;

extern double
        coef_brightwell ;

extern long
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
        total_pions,
        nb_chiffres_des_scores,
        nmax_flottement,
        nmax_couleur;

extern discs_t
        score_bip;

/* User defined penalties
 * Penalites definies par l'utilisateur */

extern pen_t
        *penalite_couleur,
        *penalite_flottement,
        *penalite_chauvinisme,
        penalite_mcoul, penalite_copp, penalite_desuite,
        penalite_bipbip, penalite_flcum, minoration_fac,
#ifdef ELITISM
        *penalite_elitisme,
#endif
        penalite_repcoul;

/* User defined strings and filenames
 * Chaines et noms de fichiers definis par l'utilisateur */

extern char
        *pays_courant,
        *pays_defaut,
        *nom_fichier_joueurs,
        *nom_fichier_nouveaux,
        *nom_fichier_inter,
        *nom_fichier_inter_backup,
        *nom_fichier_log,
        *nom_fichier_config,
        *nom_fichier_result,
        *nom_fichier_classement,
        *nom_fichier_class_equipes,
        *nom_fichier_appariements,
        *nom_fichier_crosstable_HTML,
        *nom_fichier_elo,
        *nom_fichier_lu,
        *nom_programme,
        *couleur_1, *couleur_2;


#ifdef NO_RAISE
    #define raise(sig)  kill(getpid(),(sig))
#endif

/* utilities - utilitaires */
long le_max_de(long n1, long n2);
long le_min_de(long n1, long n2);
char *nom_fichier_numerote(char *pattern, long numero);
long debuts_differents(const char *s1, const char *s2, long n);
long compare_chaines_non_sentitif(const char *scan1, const char *scan2);
long copier_fichier(FILE *dest, FILE *source) ;
void backup_inter(void) ;
void init_fichier_intermediaire(long type_fichier) ;
FILE *myfopen_dans_sous_dossier(const char *filename, const char *mode, const char *folder, long utilise, long err) ;

/*
 * Variables and prototypes from ttrondes.c. Functions return 1
 * in case of success and 0 otherwise.
 * Variables et prototypes de ttrondes.c. Les fonctions renvoient 1 en
 * cas de succes et 0 en cas d'echec.
 */

extern long ttr_minj, ttr_maxj;

long sauve_table_ttr(FILE *fp);
long charge_table_ttr(long card, long *table);
long init_ttrondes(void);
long appariement_ttrondes(void);



#endif    /*  #ifndef __Global_h__   */
