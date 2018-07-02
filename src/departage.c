/*
 * tiebreak.c: computes player's tiebreak with Brightwell quotient rule.
 *             calcule le departage d'un joueur avec les regles du Brightwell
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33 All 'int' become 'long' to force 4 bytes storage.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 02/02/2007 : Le departage se calcule maintenant en double
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "changes.h"
#include "global.h"
#include "pions.h"
#include "joueur.h"
#include "tiebreak.h"


/* Note: si (total_pions <= 1), cela signifie que nous jouons un jeu sans
 * "nombre de pions" tel que les echecs. Le nombre de pions n'est alors
 * rien d'autre que le score et n'apporte donc pas d'information supple-
 * mentaire pour le departage. Nous prenons alors comme departage le
 * "Buchholz", c'est-a-dire la somme des points des adversaires.
 */
#define BUCHHOLZ_ONLY   (total_pions <= 1)



/*
 * void DepartageJoueur(BQ_results *TabJ,
 *                      long BQ_ronde,
 *                      double *Dep,
 *                      discs_t *TotalPions,
 *                      long *SO)
 *
 * Calcule le departage d'un joueur.
 *
 * tabJ       : tableau des resultats du joueur avec le ID FFO
 *              de l'adversaire (0 si Bip ou pas joue), le score et
 *              le total de points actuel de l'adversaire;
 * BQ_ronde   : ID de la ronde a laquelle calculer le departage (>=0);
 * Dep        : resultat du departage suivant les regles de Brightwell;
 * TotalPions : total de pions du joueur en comptant toutes les parties
 *              (meme celles contre Bip ou un adversaire ayant abandonne);
 * SO         : Bucholtz du joueur en comptant toutes les parties
 *              (meme celles contre Bip ou un adversaire ayant abandonne).
 *
 *
 *
 * Rules to calculate the Brightwell tie-breaking score : each game
 * is counted normally (using actual disc-count and actual number
 * of points of opponent) unless :
 *   - your opponent withdrew before the end or
 *   - you played Bye or
 *   - you didn't play at all that round because you
 *     were late or withdrew;
 *
 * In these cases the Brightwell tie-break is raised using a draw
 * against yourself.
 *
 */

void DepartageJoueur(
    BQ_results *TabJ, long BQ_ronde, double *Dep, discs_t *TotalPions, long *SO) {
    long i, adv, PartiesPasJouees ;

    PartiesPasJouees = 0;
    if (BUCHHOLZ_ONLY)
        coef_brightwell = 1;
    /* mais voir egalement plus bas: le nombre de pions est ignore */
    *SO = 0 ;
    *Dep = 0.0 ;
    *TotalPions = ZERO_DISC ;
    assert ((BQ_ronde >= 0) && (BQ_ronde < current_round));
    for (i=0 ; i<=BQ_ronde ; i++) {
        if (!SCORES_EQUALITY(TabJ->score[i], UNKNOWN_SCORE))
            ADD_SCORE(*TotalPions, TabJ->score[i]) ;
        *SO += TabJ->opp_points[i] ;
        adv = numero_inscription(TabJ->opp[i]) ;
        if ((adv != -1) && present[adv]) {
        /* on a joue un adversaire reel et il est encore present dans le tournoi */
            *Dep += coef_brightwell * (float) TabJ->opp_points[i];
            if (!BUCHHOLZ_ONLY)
                *Dep += SCORE_TO_FLOAT(TabJ->score[i]);
        } else {
        /* on n'a pas joue ou joue Bip ou un adv qui a abandonne */
            PartiesPasJouees++ ;
        }
    }
    /*
     * Ajouter (PartiesPasJouees) parties nulles contre soi,
     * pour toutes les parties jouees contre BIP, contre un adversaire
     * ayant abandonne ou les parties non jouees (retard ou abandon).
     *   */

    *Dep += coef_brightwell * ((float) PartiesPasJouees *  TabJ->player_points) ;
    if (!BUCHHOLZ_ONLY)
        *Dep += (float) (PartiesPasJouees * total_pions/2);

    if (coef_brightwell == 0) { /* Departage aux pions */
        *Dep = SCORE_TO_FLOAT(*TotalPions) ;
    }
}
