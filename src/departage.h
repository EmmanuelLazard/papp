/* departage.h
 *
 *  - prototypes pour departage.c
 *  - prototypes for departage.c
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 02/02/2007 : Modification du type de '*Dep' qui passe en double
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef __Departage_h__
#define __Departage_h__


#include "global.h"
#include "pions.h"


typedef struct {
    long adv[NMAX_RONDES] ;        /* Numero FFO de l'adversaire */
    long points_adv[NMAX_RONDES] ; /* nombre de points de l'adversaire de la ronde */
    pions_t score[NMAX_RONDES] ;   /* Score de la partie */
    long points_joueur ;           /* Nombre de points du joueur pour les nulles */
} BQ_resultats ;


void DepartageJoueur(BQ_resultats *TabJ, long BQ_ronde, double *Dep, pions_t *Total, long *SO) ;


#endif  /*  #ifndef __Departage_h__ */
