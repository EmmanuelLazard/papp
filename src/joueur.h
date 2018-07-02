/* joueur.h
 *
 *  - Gestion des joueurs
 *  - Players management
 *
 * (EL) 15/06/2018 : v1.37, English version for code.
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 All 'int' become 'long' to force 4 bytes storage.
 					 Addition of field 'newPlayer' in _Player structure.
 					 Enable to know if the player is in the "nouveaux" file. Important for ELO.
					 Ajout du champ 'newPlayer' dans la structure _Player. Cela permet de savoir
                     si le joueur est dans le fichier "nouveaux". Important pour le fichier ELO.
 * (EL) 21/04/2008 : v1.32 Ajout du fullname de famille et prenom dans la structure _Player
 					 Addition of name and firstname in _Player structure.
 					 Necessary to separate these information in the "nouveaux" file
                     Cela est necessaire pour separer ces informations dans le fichier nouveaux.
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 02/02/2007 : changement du type 'tieBreak[]' en double
                      tieBreak[] is changed to 'double' type
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef  __Joueur_h__
#define  __Joueur_h__


#include "changes.h"
#include "pions.h"


typedef struct _Player {
	char *family_name;
	char *firstname;        /* Used in 'nouveaux' file */
    char *fullname;         /* Player's full name      */
    char *country;          /* Nationality             */
	long  ID;               /* ID in WOF database      */
    long  rating;           /* World rating            */
    char *comment;          /* Comments                */
	long newPlayer;			/* is a new player ?       */
    struct _Player *next;   /* next in list            */
    struct _Player *nxt2;   /* next for hashing        */
} Player;

typedef struct {
    long slots_number;     /* number of allocated slots */
    long n;                /* list length               */
    Player **list;         /* pointers array            */
} Players_list;

typedef struct _i_zone {
    char *country;          /* country fullname                                */
    long  min_ins;          /* smallest authorized ID - plus petit ID autorise */
	long  max_ins;          /* largest authorized ID - plus grand ID autorise  */
    struct _i_zone *next;   /* next in list - suivant dans la liste            */
} Insertion_Zone;

#define MAX_REGISTERED    256

extern Players_list
            *registered_players,
            *new_players,
            *emigrant_players,
            *team_captain;

extern long         present[MAX_REGISTERED];
extern long         score[MAX_REGISTERED];
extern discs_t      nbr_discs[MAX_REGISTERED];
extern double       tieBreak[MAX_REGISTERED];
extern long         last_float[MAX_REGISTERED];
extern long         team_score[MAX_REGISTERED];

/* Operations, definies dans joueurs.c et entrejou.c */

Player *nv_joueur (long numero, const char *nom, const char *prenom,
    const char *programmeur, const char *pays, long classement,
    const char *commentaire, const long nv);
Player *trouver_joueur (long numero);
Player *premier_joueur(void);   /* premier joueur de la base */
long nombre_joueurs_base(void);  /* nb de joueurs dans la base*/
long inserer_joueur (const char *pays);
void nv_zone(const char *pays, long min_ins, long max_ins);
Insertion_Zone *premiere_zone(void);
char *coupon(const Player *j1, const Player *j2, discs_t score);
char *entree_clav_nom_joueur(const char *prompt, char *tampon_initial, Players_list *parmi_ces_joueurs);
void entree_joueurs(void);
void affiche_inscrits(const char *filename);
void affiche_equipes(const char *filename);
long numero_inscription(long numero_elo);
long est_nouveau(long numero_elo) ;
long inscrire_joueur(long numero_elo);
long change_nationalite (long numero, const char *pays);
 int tri_joueurs(const void *ARG1, const void *ARG2) ;

/* Gestion des listes */

Players_list *creer_liste(void);
Players_list *ajouter_joueur(Players_list *l, Player *j);
Players_list *recherche_nom(Players_list *l, const char *chaine);
Players_list *recherche_nom_dans_liste(Players_list *lj, const char *chaine, Players_list *liste_completion);
void vider_liste(Players_list *l);

#define TOUS_LES_JOUEURS (-1)
long choix_d_un_joueur_au_clavier(const char *prompt, Players_list *parmi_ces_joueurs, char **chaine);
long nb_completions_dans_coupons(char *chaine, long *nroJoueur, char *chaineScore);


/* Utilitaires sur les chaines de caracteres des noms de joueurs */

long enleve_espaces_de_gauche(char *chaine);
long enleve_espaces_de_droite(char *chaine);
long est_un_nom_de_joueur_valide(char *tampon);


#endif    /*  #ifndef __Joueur_h__  */
