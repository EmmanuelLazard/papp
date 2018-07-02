/*
 * score.c: Toutes les operations sur les joueurs
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 04/02/2007 : Reecriture de 'departage_en_chaine' pour afficher
 *                   un flottant avec une eventuelle decimale.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <assert.h>
#include "changes.h"
#include "pions.h"
#include "global.h"


long total_pions = 64;

/*
 * fonctions pour l'affichage
 */
char *pions_en_chaine(discs_t v) {
    char *result;

    result = new_string();
    assert( result != NULL);

#ifdef USE_HALF_DISCS
    if (v.half_discs % 2)
        sprintf(result, "%0*ld.5" , (int)nb_chiffres_des_scores , v.half_discs / 2);
    else
        sprintf(result, "%0*ld" , (int)nb_chiffres_des_scores , v.half_discs / 2);
#else
    sprintf(result, "%0*ld" , (int)nb_chiffres_des_scores , v);
#endif
    return result;
}


char *departage_en_chaine(double Dep) {
    char *result;
    long   arrondi ;

    result = new_string();
    assert( result != NULL);

/* #ifdef USE_HALF_DISCS
    if (v.half_discs % 2)
        sprintf(result, "%ld.5" , v.half_discs / 2);
    else
        sprintf(result, "%ld" , v.half_discs / 2);
#else
    sprintf(result, "%ld" ,  v);
#endif
*/
    /* On arrondit d'abord a une decimale
       En multipliant par 10 le departage */
    arrondi = (long) 10*(Dep+0.049999) ;
    if ( arrondi%10 == 0 )
        sprintf(result, "%.0f", (float) arrondi/10.0);  /* on n'affiche pas les decimales */
    else
        sprintf(result, "%.1f", (float) arrondi/10.0); /* une decimale arrondie */
    return result;
}


/*
 * comprend_score : conversion d'une chaine en pions pour l'entree des resultats
 * Renvoie : 1 si PAPP a reussi a interpreter la chaine comme un score.
 *           0 sinon
 * Entree :
 *     <chaine> : chaine de caractere
 * Sorties :
 *     <valeur> et <relatif> : quand la fonction reussit, <valeur> contient
 *     le score lu et <relatif> vaut 1 si le score etait rentre en relatif,
 *     0 sinon.
 */
long comprend_score(const char *chaine, discs_t *valeur, long *relatif) {
    discs_t score;
    long n, lu_en_relatif;
    double f;
    char parasites[100];


    assert(chaine != NULL);

    switch (chaine[0]) {
    case '=':
        score = RELATIVE_SCORE_TO_ABSOLUTE(INTEGER_TO_SCORE(0));
        lu_en_relatif = 1;
        break;
    case '+':
        if (sscanf(chaine+1, "%ld%s" , &n, parasites) == 1) {
            score = RELATIVE_SCORE_TO_ABSOLUTE(INTEGER_TO_SCORE(n));
            lu_en_relatif = 1;
            break;
        } else
            return 0;  /* pas de score, ou des caracteres parasites */
        break ;
    case '-':
        if (sscanf(chaine+1, "%ld%s" , &n , parasites) == 1) {
            score = RELATIVE_SCORE_TO_ABSOLUTE(INTEGER_TO_SCORE(-n));
            lu_en_relatif = 1;
            break;
        } else
            return 0;  /* pas de score, ou des caracteres parasites */
        break ;
    default:
        if (sscanf(chaine, "%le%s" , &f, parasites) == 1)
          { score = FLOAT_TO_SCORE(f); lu_en_relatif = 0; }
        else
          return 0;  /* pas de score, ou des caracteres parasites */
    }

    *valeur = score;
    *relatif = lu_en_relatif;
    return 1;
}
