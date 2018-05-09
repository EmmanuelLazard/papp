/* pions.h
 *
 *  - macros pour abstraire le type des pions dans Papp
 *  - macros to define an abstraction for discs management in Papp
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 02/02/2007 : Ajout d'une macro 'SCORE_EN_FLOTTANT' qui renvoie
 *                   le nombre exact de pions en flottant (pour le departage)
 * (EL) 13/01/2007 : v1.30 E. Lazard, no change
 */

#ifndef __Pions_h__
#define __Pions_h__

#include <stdlib.h>
#include "changes.h"


extern long total_pions;

/*
 * Les macros et fonctions generales suivantes sont abstraites
 * de la representation interne du type "pions_t"
 */

#define TOTAL_PIONS					(ENTIER_EN_SCORE(total_pions))
#define SCORE_INCONNU				(ENTIER_EN_SCORE(-1))
#define ZERO_PION					(ENTIER_EN_SCORE(0))

#define SCORES_DIFFERENTS(v1,v2)	(!EGALITE_SCORES((v1),(v2)))
#define COUPON_EST_VIDE(v)			(EGALITE_SCORES((v),SCORE_INCONNU))
#define COUPON_EST_REMPLI(v)		(SCORE_EST_POSITIF((v)))

#define SCORE_EST_POSITIF(v) 		(SCORE_EST_PLUS_GRAND((v), ZERO_PION))
#define SCORE_EST_LEGAL(v)			(SCORE_EST_POSITIF(v) && (SCORE_PLUS_PETIT((v),TOTAL_PIONS)))
#define EST_UNE_VICTOIRE(v)			(SCORE_STRICT_GRAND(DOUBLE_SCORE(v), TOTAL_PIONS))
#define EST_UNE_DEFAITE(v)			(SCORE_STRICT_PETIT(DOUBLE_SCORE(v), TOTAL_PIONS))
#define EST_PARTIE_NULLE(v)			(EGALITE_SCORES(DOUBLE_SCORE(v), TOTAL_PIONS))

#define SCORE_EN_RELATIF(v)			(DIFF_SCORES(DOUBLE_SCORE(v),TOTAL_PIONS))
#define SCORE_ADVERSE(v)			(DIFF_SCORES(TOTAL_PIONS, (v)))
#define SCORE_TROP_GRAND(v)			(SCORE_STRICT_GRAND((v),TOTAL_PIONS))
#define MAUVAIS_TOTAL(v1,v2)		(SCORES_DIFFERENTS(SOMME_SCORES((v1),(v2)),TOTAL_PIONS))


/*
 * Voici maintenant la representation concrete
 * des operations sur le type "pions_t"
 */

#ifndef USE_DEMI_PIONS

/*
 * On utilise la definition normale de PAPP des pions
 * par des entiers, et on implemente les operations par
 * des macros
 */
	typedef long pions_t;

	#define	SCORE_EST_PLUS_GRAND(v1,v2)	((v1) >= (v2))
	#define SCORE_STRICT_GRAND(v1,v2)	((v1) >  (v2))
	#define EGALITE_SCORES(v1,v2)		((v1) == (v2))
	#define	SCORE_PLUS_PETIT(v1,v2)		((v1) <= (v2))
	#define SCORE_STRICT_PETIT(v1,v2)	((v1) <  (v2))

	#define SCORE_RELATIF_EN_ABSOLU(v)  (((v) + total_pions) / 2)
	#define ENTIER_EN_SCORE(v)          (v)
	#define FLOTTANT_EN_SCORE(v)        ((long)((v) + 0.49999999)) /* entier le plus proche */
	#define AJOUTE_SCORE(v,x)			(v) += (x)
	#define SOMME_SCORES(v1,v2)			((v1) + (v2))
	#define DIFF_SCORES(v1,v2)			((v1) - (v2))
	#define DOUBLE_SCORE(v)				(2*(v))
	#define	VALEUR_ABSOLUE_SCORE(v)		(abs(v))
    #define SCORE_EN_FLOTTANT(v)        (v)

#else

/*
 * Definition speciale pour pouvoir gerer dans Papp
 * des scores en demi-pions: on stoque les demi-pions
 * dans une structure pour les cacher, et certaines
 * operations sont implementees par des fonctions
 */
	typedef struct { long demi_pions; } pions_t;

	#define	SCORE_EST_PLUS_GRAND(v1,v2)	((v1).demi_pions >= (v2).demi_pions)
	#define SCORE_STRICT_GRAND(v1,v2)	((v1).demi_pions >  (v2).demi_pions)
	#define EGALITE_SCORES(v1,v2)		((v1).demi_pions == (v2).demi_pions)
	#define	SCORE_PLUS_PETIT(v1,v2)		((v1).demi_pions <= (v2).demi_pions)
	#define SCORE_STRICT_PETIT(v1,v2)	((v1).demi_pions <  (v2).demi_pions)


	static pions_t ENTIER_EN_SCORE(long n)
		{	pions_t aux;
			aux.demi_pions = 2*n;
			return aux; }

	static pions_t FLOTTANT_EN_SCORE(double n)
		{	pions_t aux;
			aux.demi_pions = 2*n + 0.49999999;
			return aux; }

	static pions_t SCORE_RELATIF_EN_ABSOLU(pions_t v)
		{	pions_t aux;
			aux.demi_pions = (v.demi_pions + ENTIER_EN_SCORE(total_pions).demi_pions) / 2 ;
			return aux; }

	static pions_t SOMME_SCORES(pions_t v1, pions_t v2)
		{	pions_t aux;
			aux.demi_pions = v1.demi_pions + v2.demi_pions;
			return aux; }

	static pions_t DIFF_SCORES(pions_t v1, pions_t v2)
		{	pions_t aux;
			aux.demi_pions = v1.demi_pions - v2.demi_pions;
			return aux; }

	static pions_t DOUBLE_SCORE(pions_t v)
		{	pions_t aux;
			aux.demi_pions = 2 * v.demi_pions;
			return aux; }

	static pions_t VALEUR_ABSOLUE_SCORE(pions_t v)
		{	pions_t aux;
			aux.demi_pions = abs(v.demi_pions);
			return aux; }

	static double  SCORE_EN_FLOTTANT(pions_t v)
		{   if (v.demi_pions%2 == 0)
				return (float) v.demi_pions/2 ;
			else
				return ((float) v.demi_pions/2) + 0.5 ;
		}

	#define AJOUTE_SCORE(v,x)			\
		((v).demi_pions) += ((x).demi_pions)

#endif    /*  #else */


/*
 * fonctions pour l'affichage
 */

char *pions_en_chaine(pions_t v);
char *departage_en_chaine(double v);

/*
 * conversion d'une chaine en pions pour l'entree des resultats
 */

long comprend_score(const char *chaine, pions_t *valeur, long *relatif);


#endif  /*  #ifndef __Pions_h__ */
