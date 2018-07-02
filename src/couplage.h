/* couplage.h
 *
 *  - prototypes pour couplage.c
 *  - prototypes for couplage.c
 *
 * (EL) 15/06/2018 : v1.37, English version for code.
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 All 'int' become 'long' to force 4 bytes storage.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef __Couplage_h__
#define __Couplage_h__


#include "pions.h"
#include "joueur.h"

void    first_round(void), next_round(void);
void    raz_scores(void);
void    _save_round(void), mettre_aj_scores(void);
void    _save_registered(void), _save_pairings(void);
void    _recreate_workfile(void);
void    do_not_save_registered(void), do_not_save_pairings(void);
void    zero_coupling(void);


typedef struct players_pair {
    Player    *j1;
    Player    *j2;
    discs_t   score;
    double    sort_value;
} Pair;


long    uncouple(long n);
void    make_couple(long n1, long n2, discs_t value);
long    polarity(long n);
long    polar2(long n, long round);
discs_t *couple_value(long n1, long n2);
long    couples_nbr(long round_nbr);
void    round_iterate(long round_nbr);
long    next_couple(long *n1, long *n2, discs_t *value);
void    player_InOut(long ID_player, long direction);
void    set_history_presence(long ID, long which_round, long presence);
long    player_was_present(long ID, long which_round);
long    change_result(long round_nbr, long nbr_player1, long nbr_player2, discs_t value);
void    mettre_numero_de_table(long round_nbr, long n1, long n2, long table_ID);
long    numero_de_table(long round_nbr, long n1, long n2);
void    tables_numbering(void);
void    pairings_manipulate(void);
void    absents_uncoupling(void);
long    nbr_unpaired_players(void);
int     pairs_sort(const void *p1, const void *p2);
double  tables_sort_criteria(long n1, long n2);
void    display_pairings(const char *filename, long full_results);
void    afficher_remplissage_coupons(void);
void    entree_resultats(void);
void    save_pairings_file(void);
void    sauver_fichier_resultats(void);
void    sauver_fichier_classement(void);
void    sauver_fichier_equipes(void);
void    sauver_fichier_tableau_croise_html(void);
void    imagine_resultats(void);
void    validation_resultats(long sauve_results_en_clair);
void    correction_resultat(void);


#endif  /*  #ifndef __Couplage_h__ */
