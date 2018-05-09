/* joueur.h
 *
 *  - Gestion des joueurs
 *  - Players management
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
					 Ajout du champ 'nouveau' dans la structure _Joueur. Cela permet de savoir
                     si le joueur est dans le fichier "nouveaux". Important pour le fichier ELO.
 * (EL) 21/04/2008 : v1.32 Ajout du nom de famille et prenom dans la structure _Joueur
                     Cela est necessaire pour separer ces informations dans le fichier nouveaux.
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 02/02/2007 : changement du type 'departage[]' en double
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef  __Joueur_h__
#define  __Joueur_h__


#include "changes.h"
#include "pions.h"


typedef struct _Joueur {
	char *nom_famille;
	char *prenom;           /* on distingue ces infos pour le fichier nouveaux */
    char *nom;              /* Nom complet du joueur   */
    char *pays;             /* Sa nationalite          */
	long  numero;           /* Son numero Elo          */
    long  cl_jech;          /* Son classement de Jech  */
    char *comment;          /* Commentaires divers     */
	long nouveau;			/* nouveau joueur ?        */
    struct _Joueur *next;   /* suivant dans la liste   */
    struct _Joueur *nxt2;   /* suivant pour le hachage */
} Joueur;

typedef struct {
    long nb_slots;          /* nombre de slots alloues */
    long n;                 /* longueur de la liste    */
    Joueur **liste;         /* tableau de pointeurs    */
} Liste_joueurs;

typedef struct _zone_i {
    char *pays;             /* nom du pays                */
    long  min_ins;          /* plus petit numero autorise */
	long  max_ins;          /* plus grand numero autorise */
    struct _zone_i *next;   /* suivant dans la liste      */
} Zone_Insertion;

#define MAX_INSCRITS    256

extern Liste_joueurs
            *joueurs_inscrits,
            *joueurs_nouveaux,
            *joueurs_emigres,
            *capitaines_equipes;

extern long         present[MAX_INSCRITS];
extern long         score[MAX_INSCRITS];
extern pions_t      nb_pions[MAX_INSCRITS];
extern double       departage[MAX_INSCRITS];
extern long         dern_flot[MAX_INSCRITS];
extern long         score_equipe[MAX_INSCRITS];

/* Operations, definies dans joueurs.c et entrejou.c */

Joueur *nv_joueur (long numero, const char *nom, const char *prenom,
    const char *programmeur, const char *pays, long classement,
    const char *commentaire, const long nv);
Joueur *trouver_joueur (long numero);
Joueur *premier_joueur(void);   /* premier joueur de la base */
long nombre_joueurs_base(void);  /* nb de joueurs dans la base*/
long inserer_joueur (const char *pays);
void nv_zone(const char *pays, long min_ins, long max_ins);
Zone_Insertion *premiere_zone(void);
char *coupon(const Joueur *j1, const Joueur *j2, pions_t score);
char *entree_clav_nom_joueur(const char *prompt, char *tampon_initial, Liste_joueurs *parmi_ces_joueurs);
void entree_joueurs(void);
void affiche_inscrits(const char *filename);
void affiche_equipes(const char *filename);
long numero_inscription(long numero_elo);
long est_nouveau(long numero_elo) ;
long inscrire_joueur(long numero_elo);
long change_nationalite (long numero, const char *pays);
 int tri_joueurs(const void *ARG1, const void *ARG2) ;

/* Gestion des listes */

Liste_joueurs *creer_liste(void);
Liste_joueurs *ajouter_joueur(Liste_joueurs *l, Joueur *j);
Liste_joueurs *recherche_nom(Liste_joueurs *l, const char *chaine);
Liste_joueurs *recherche_nom_dans_liste(Liste_joueurs *lj, const char *chaine, Liste_joueurs *liste_completion);
void vider_liste(Liste_joueurs *l);

#define TOUS_LES_JOUEURS (-1)
long choix_d_un_joueur_au_clavier(const char *prompt, Liste_joueurs *parmi_ces_joueurs, char **chaine);
long nb_completions_dans_coupons(char *chaine, long *nroJoueur, char *chaineScore);


/* Utilitaires sur les chaines de caracteres des noms de joueurs */

long enleve_espaces_de_gauche(char *chaine);
long enleve_espaces_de_droite(char *chaine);
long est_un_nom_de_joueur_valide(char *tampon);


#endif    /*  #ifndef __Joueur_h__  */
