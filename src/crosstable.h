/* crosstable.h
 *
 *  - prototypes pour crosstable.c
 *  - prototypes for crosstable.c
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef __Crosstable_h__
#define __Crosstable_h__

long sauvegarder_tableau_croise_texte(long ronde_affichee, char *nom_fichier_texte, char *mode_ouverture);
long sauvegarder_tableau_croise_HTML(char *nom_fichier_html);

void preparer_calculs_tableau_croise(long quelle_ronde);
void afficher_cross_table(void);
long  peut_allouer_memoire_cross_table(void);
void liberer_memoire_cross_table(void);


#endif  /*  #ifndef __Crosstable_h__ */
