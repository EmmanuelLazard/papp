/*
 * ttrondes.c: Organisation d'un tournoi toutes rondes (systeme Tastet)
 *
 * Le principe est le suivant: on place les 2n joueurs autour d'une table,
 * et a chaque ronde on fait tourner tous les joueurs, sauf un (le pivot),
 * situe a l'une des extremites de la table. Exemple:
 *
 *   Ronde 1     Ronde 2     Ronde 3
 *
 *  0 1 2 3 4   0 2 3 4 9   0 3 4 9 8     etc...
 *  5 6 7 8 9   1 5 6 7 8   2 1 5 6 7
 *
 * Chaque chaise a sa couleur (mises a part celle du pivot et celle d'en face)
 * et les couleurs alternent:
 *
 *  ? B N B N
 *  ? N B N B
 *
 * Enfin, le pivot commence avec les noirs, et change de couleur
 * a chaque ronde. Les couleurs sont donc initialement
 *
 *  N B N B N
 *  B N B N B
 *
 * S'il y a un nombre impair de joueurs, c'est Bip qui est le pivot,
 * ainsi chaque joueur jouera exactement le meme nombre de fois
 * chaque couleur.
 * Dans le cas d'un plusieurs fois toutes-rondes, on inverse la couleur
 * des chaises apres chaque rotation complete; les appariements sont
 * alors exactement les memes qu'au "tour" precedent, sauf que les
 * couleurs sont inversees.
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "couplage.h"
#include "global.h"
#include "joueur.h"

long     ttr_minj = 1,   /* Nb minimum de joueur pour un toutes-rondes */
        ttr_maxj = 6;   /* Nb maximum de joueurs */

static long ttr_card = 0;        /* Pas encore de table */
static long *ttr_table;

/*
 * Sauvegarde de la table. Cette fonction est normalement appellee par
 * _sauve_ronde(), a la fin de la premiere ronde, immediatement
 * apres init_ttrondes().
 */

long sauve_table_ttr (FILE *fp) {
    long i;

    assert(0 < ttr_minj && ttr_minj <= ttr_maxj);
    if (ttr_card < ttr_minj || ttr_card > ttr_maxj)
        return 0;       /* rien a sauver */
    assert(ttr_table);
    /* Est-ce bien la premiere ronde? */
    assert(ronde == 0);
    fprintf(fp,"table-toutes-rondes [%ld] = {", ttr_card);
    for (i = 0; i < ttr_card; i++) {
        fprintf(fp, i % (ttr_card/2) ? " " : "\n\t");
        fprintf(fp, "%06ld", ttr_table[i]);
    }
    fprintf(fp, "\n};\n\n");
    return 1;
}

/*
 * Initialisation de la table a partir d'un tableau; cette fonction est
 * utilisee pour relire la table qui a ete sauvegardee dans le fichier intermediaire.
 */

long charge_table_ttr (long card, long *table) {
    long i;

    assert(0 < ttr_minj && ttr_minj <= ttr_maxj);
    if (card < ttr_minj && card > ttr_maxj)
        return 0;
    /* Est-ce bien la premiere ronde? */
    assert(ronde == 0);
    CALLOC(ttr_table, ttr_card=card, long);
    for (i = 0; i < card; i++)
        ttr_table[i] = table[i];
    return 1;
}

/*
 * Initialisation de la table a l'aide des appariements de la
 * premiere ronde. Sans effet s'il y a trop ou trop peu de joueurs.
 */

long init_ttrondes (void) {
    long i, j, nb_presents, nb_napp, p0, p1, n1, n2;
    pions_t v;

    assert(0 < ttr_minj && ttr_minj <= ttr_maxj);
    /* Table deja initialisee? */
    if (ttr_card > 0)
        return 0;
    /* Est-ce bien la premiere ronde? */
    assert(ronde == 0);
    /* Compter le nombre de joueurs presents */
    nb_presents = 0;
    for (i = 0; i < joueurs_inscrits->n; i++)
        if (present[i])
            ++nb_presents;
    if (nb_presents < ttr_minj || nb_presents > ttr_maxj)
        return 0;
    /* Verifier que (presque) tous les joueurs sont apparies */
    nb_napp = nb_joueurs_napp();
    if (nb_napp > 1)
        return 0;       /* erreur */
    ttr_card = nb_napp + nb_presents;
    assert(ttr_card % 2 == 0);
    CALLOC(ttr_table, ttr_card, long);
    p0 = 0;
    p1 = ttr_card/2;
    if (nb_napp) {
        /* Il y a un Bip */
        /* Quel est le joueur qui passe ? */
        for (i = 0; i < joueurs_inscrits->n; i++)
            if (present[i]) {
                j = joueurs_inscrits->liste[i]->numero;
                if (polarite(j) == 0) {
                    ttr_table[p0++] = -1;
                    ttr_table[p1++] = j;
                }
            }
        assert(p0 == 1);
    }
    /*
     * Mettre les joueurs des deux cotes de la table:
     *
     *  N B N B N
     *  B N B N B   pour la premiere ronde.
     */
    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v)) {
        if (p0%2 == 0) {
            ttr_table[p0++] = n1;   /* Noir  */
            ttr_table[p1++] = n2;   /* Blanc */
        } else {
            ttr_table[p0++] = n2;   /* Blanc */
            ttr_table[p1++] = n1;   /* Noir  */
        }
    }
    /* Verifier que la table est pleine */
    assert(p0 == ttr_card/2);
    assert(p1 == ttr_card);
    return 1;       /* Ok! */
}

static void rotation(void) {
    /*
     * Realise la rotation de tous les joueurs sauf le pivot:
     * 1 -> 2 -> 3 -> 4 -> 9 -> 8 -> 7 -> 6 -> 5 -> 1.
     *
     * Cette rotation est dans le mauvais sens (pour des raisons
     * historiques...); ce n'est pas trop grave puisqu'elle est
     * d'ordre (ttr_card-1).
     */
    long i, k, tmp;

    assert(ttr_card > 0 && ttr_card%2 == 0);
    k = ttr_card / 2;
    tmp = ttr_table[k];
    for (i = k; i < 2*k-1; i++)
        ttr_table[i] = ttr_table[i+1];
    ttr_table[2*k-1] = ttr_table[k-1];
    for (i = k-1; i > 1; i--)
        ttr_table[i] = ttr_table[i-1];
    ttr_table[1] = tmp;
}

/*
 * Realise les appariements pour la ronde dont le numero est donne par
 * la variable globale `ronde' (plus un). Renvoie 0 en cas d'erreur et 1
 * si tout s'est bien passe.
 */

long appariement_ttrondes (void) {
    long i, j, k, nb_presents;

    if (ttr_card == 0)
        return 0;       /* Infos non disponibles */
    nb_presents = 0;
    for (i = 0; i < joueurs_inscrits->n; i++)
        if (present[i])
            ++nb_presents;
    /* Verifier qu'aucun joueur n'est deja inscrit */
    if (nb_joueurs_napp() != nb_presents)
        return 0;
    /*
     * Verifier qu'il y a le bon nombre de joueurs. REMARQUE: le code
     * ci-dessous suppose que Bip est en 0 (i.e. c'est le pivot)
     */
    if (nb_presents != ttr_card - (ttr_table[0] < 0))
        return 0;
    /* Verifier que tous les joueurs sont dans la table */
    for (i = 0; i < joueurs_inscrits->n; i++)
        if (present[i]) {
            j = joueurs_inscrits->liste[i]->numero;
            for (k = 0; k < ttr_card; k++)
                if (ttr_table[k] == j)
                    break;
            if (k == ttr_card)
                return 0;       /* pas trouve */
        }
    /*
     * On fait tourner la table de `ronde' crans dans le sens direct
     * donc de (ttr_card-1-ronde) crans dans le `mauvais' sens...
     */
    for (i = ronde; i % (ttr_card-1); i++)
        rotation();
    /* Et on apparie les joueurs. Le pivot a commence avec les noirs */
    k = ttr_card / 2;
    if (ttr_table[0] >= 0) {
        /*
         * Le pivot n'est pas Bip; sa chaise change de couleur a
         * chaque ronde, tout comme celle qui est en face de lui.
         */
        if (ronde%2 == 0)
          accoupler(ttr_table[0], ttr_table[k], SCORE_INCONNU);
        else
          accoupler(ttr_table[k], ttr_table[0], SCORE_INCONNU);
    }
    for (i = 1; i < k; i++) {
        /*
         * Les autres chaises ne changent de couleur que dans le cas
         * d'un plusieurs-fois-toutes-rondes, apres chaque rotation
         * complete de la table.
         */
        if (i%2 == (ronde/(ttr_card-1))%2)
          accoupler(ttr_table[i], ttr_table[i+k], SCORE_INCONNU);
        else
          accoupler(ttr_table[i+k], ttr_table[i], SCORE_INCONNU);
    }
    /* Et on remet la table en place */
    for (i = 0; i < ronde; i++)
        rotation();
    return 1;       /* Ok! */
}
