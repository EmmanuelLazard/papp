/* appari.h
 *
 * - entete pour l'algorithme d'appariement
 * - header for the pairing algorithm
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 */

#ifndef __Appari_h__
#define __Appari_h__

/* j'ai ajoute le #if defined(UNIX_BSD) || defined(UNIX_SYSV) */
/* change by Stephane Nicolet , 17 march 2000 */
#if defined(UNIX_BSD) || defined(UNIX_SYSV)
    #include <sys/types.h>
#endif

#include <limits.h>

typedef signed long pen_t;
#define _PENALITE_MAX_  LONG_MAX
#define MAX_PEN         10000000L

pen_t apparie (long n, pen_t **penalites, long *solution);

#endif  /* #ifndef __Appari_h__ */
