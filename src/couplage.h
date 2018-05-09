/* couplage.h
 *
 *  - prototypes pour couplage.c
 *  - prototypes for couplage.c
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef __Couplage_h__
#define __Couplage_h__


#include "pions.h"
#include "joueur.h"

void    premiere_ronde(void), ronde_suivante(void);
void    raz_scores(void);
void    _sauve_ronde(void), mettre_aj_scores(void);
void    _sauve_inscrits(void), _sauve_appariements(void);
void    _recreer_fichier_intermediaire(void);
void    ne_pas_sauver_inscrits(void), ne_pas_sauver_appariements(void);
void    raz_couplage(void);


typedef struct paire_joueurs {
    Joueur    *j1;
    Joueur    *j2;
    pions_t   score;
    double    valeur_tri;
} Paire;


long    decoupler(long n);
void    accoupler(long n1, long n2, pions_t valeur);
long    polarite(long n);
long    polar2(long n, long ronde);
pions_t *valeur_couple(long n1, long n2);
long    nb_couples(long nro_ronde);
void    iterer_ronde(long nro_ronde);
long    couple_suivant(long *n1, long *n2, pions_t *val);
void    es_joueur(long numero, long direction);
void    set_historique_presence(long numero_elo, long quelle_ronde, long presence);
long    joueur_etait_present(long numero_elo, long quelle_ronde);
long    changer_resultat(long nro_ronde, long nro_joueur1, long nro_joueur2, pions_t valeur);
void    mettre_numero_de_table(long nro_ronde, long n1, long n2, long num_table);
long    numero_de_table(long nro_ronde, long n1, long n2);
void    numeroter_les_tables(void);
void    manipule_appariements(void);
void    decouplage_absents(void);
long    nb_joueurs_napp(void);
int    tri_paires(const void *p1, const void *p2);
double  critere_tri_tables(long n1, long n2);
void    affiche_appariements(const char *filename, long resultats_complets);
void    afficher_remplissage_coupons(void);
void    entree_resultats(void);
void    sauver_fichier_appariements(void);
void    sauver_fichier_resultats(void);
void    sauver_fichier_classement(void);
void    sauver_fichier_equipes(void);
void    sauver_fichier_tableau_croise_html(void);
void    imagine_resultats(void);
void    validation_resultats(long sauve_results_en_clair);
void    correction_resultat(void);


#endif  /*  #ifndef __Couplage_h__ */
