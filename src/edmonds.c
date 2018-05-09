/*
 * edmonds.c: Algorithme d'Edmonds-Johnson pour la recherche d'un
 * couplage de poids maximum dans un graphe.
 *
 * Les notations utilisees sont celles du livre d'Alan Gibbons,
 * "Algorithmic Graph Theory", Cambridge University Press (1985),
 * chapitre 5, pp 125--152.
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 06/04/2007 : changement des 'fopen()' en 'myfopen_dans_sous_dossier()'
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */


/* Interface avec le reste du programme */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "global.h"
#include "appari.h"
#include "changes.h"

/*
 * Les poids des aretes et les variables y et z sont stockes dans des
 * variables de type "weight". On pourrait utiliser des "long long"
 * (entiers sur 64 bits), mais tous les compilateurs n'ont pas ca,
 * et le GCC ne semble pas aller plus vite qu'avec des "double"
 * (virgule flottante, double precision). On n'utilise donc les "long long"
 * que dans le cas de DJGPP, car l'emulation 387 semble avoir des
 * problemes de precision.
 */

#if defined(__GO32__)
# define USE_LONG_LONG
# define LONG_LONG_MAX  9223372036854775807LL
#endif

#ifndef USE_LONG_LONG
# include <float.h>
# define weight     double
# define pen2weight(x)  ((weight)(x))
# define INFINI     (DBL_MAX/2)
#else
# define weight     long long
# define pen2weight(x)  ((weight)(x) << 32)
# define INFINI     LONG_LONG_MAX
#endif

#define ED_CALLOC(var,size,type)   do {            \
    var=(type*)malloc((size)*sizeof(type)); \
    if (var==NULL) abort();                 \
    } while (0)

/*#define EDEBUG*/

#ifdef EDEBUG
# define TRACE(x)       fprintf x
#else
# define TRACE(x)
#endif

static weight **p;
static long nb_sommets, *T;

static void edmonds_init(void), edmonds(void), edmonds_end(void);

/*
 * La fonction apparie(n, _p, v) prend comme parametres la _moitie_ du
 * nombre de joueurs, l'adresse de la matrice des penalites, et l'adresse
 * d'un tableau de longueur 2*n pour stocker le vecteur resultat, sous
 * la forme (i1 j1 i2 j2 ... in jn), si (i1,j1)...(in,jn) est l'appariement
 * optimal. Le resultat est alors la penalite de l'appariement optimal.
 *
 * L'algorithme utilise dans edmonds() attend, lui, le nombre de joueurs
 * (nb_sommets), la matrice symetrisee des poids (et non pas des penalites),
 * et au retour remplit le tableau T sous la forme (T0 T1 ... T2N), ou
 * T[i] est l'adversaire de i. Il faut encore remettre ce resultat en forme
 * pour obtenir v, en particulier determiner dans quel sens les joueurs
 * seront apparies (i.e. qui aura les noirs).
 */

pen_t apparie (long n, pen_t **_p, long *v) {
    long i, j, k, tronquees;
    pen_t total, p1, p2;

#ifdef SAVE_MATRIX
    /*
     * Sauve la matrice dans un fichier qui peut etre relu par `test'.
     * Ainsi, si une erreur se produit dans ce module, elle peut etre
     * facilement reproduite.
     */
    FILE *fp = myfopen_dans_sous_dossier("matrice.pen","w", "", 0, 0);
    if (fp) {
        fprintf(fp,"%ld\n\n", n);
        for (i = 0; i < 2*n; i++) {
            for (j = 0; j < 2*n; j++)
                fprintf(fp," %8lu", _p[i][j]);
            fprintf(fp,"\n");
        }
        fclose(fp);
    }
#endif
    nb_sommets = 2*n;
    tronquees = 0;

    /*
     * Verifier que la precision des calculs est suffisante (il est
     * bien tard pour verifier, mais vieux motard que jamais).
     */
    assert(sizeof(weight) >= 6);

    /* Rendre la matrice _p symetrique */
    ED_CALLOC(p, nb_sommets, weight*);
    for (i = 0; i < nb_sommets; i++)
        ED_CALLOC(p[i], nb_sommets, weight);
    ED_CALLOC(T, nb_sommets, long);

    for (i = 0; i < nb_sommets; i++)
        for (j = 0; j < i; j++) {
            p1 = _p[i][j];
            p2 = _p[j][i];
            if (p1 > p2)
                p1 = p2;
            if (p1 > MAX_PEN) {
                p1 = MAX_PEN;
                ++tronquees;
            }
            if (p1 < 0) {
                p1 = 0;
                ++tronquees;
            }
            p[i][j] = p[j][i] = pen2weight(MAX_PEN + 1 - p1);
        }
    if (tronquees)
        fprintf(stderr,
#ifdef ENGLISH
        "Warning: %ld penalties have been truncated,\n"
        "since they weren't between 0 and %ld.\n"
#else
        "Attention, %ld penalites ont ete tronquees,\n"
        "c.a.d. ramenees dans l'intervalle 0--%ld.\n"
#endif
        , tronquees, MAX_PEN);

    /* Annulons a tout hasard les elements diagonaux */
    for (i = 0; i < nb_sommets; i++)
        p[i][i] = (weight)0;

    edmonds_init();
    edmonds();
    edmonds_end();

    /* Tout le monde devrait etre apparie */
    for (i = 0; i < nb_sommets; i++) {
        assert(T[i] != i && T[T[i]] == i);
        v[i] = i;
    }
    total = (pen_t)0;
    for (i = 0; i < nb_sommets; i+= 2) {
        j = T[v[i]];    /* A mettre en position i+1 */
        for (k = i+2; k < nb_sommets; k++)
            if (v[k] == j)
                v[k]=v[i+1], v[i+1]=j;
    /* Il reste a ordonner les paires */
        if (_p[v[i]][v[i+1]] > _p[v[i+1]][v[i]])
            k=v[i], v[i]=v[i+1], v[i+1]=k;
        total += _p[v[i]][v[i+1]];
    }
    /* Et a liberer la memoire */
    for (i = 0; i < nb_sommets; i++)
        free(p[i]);
    free(p);
    free(T);

#ifdef SAVE_MATRIX
    fp = myfopen_dans_sous_dossier("matrice.out","w", "", 0, 0);
    if (fp) {
        for (i=0; i<n; i++) {
            j = v[2*i];
            k = v[2*i+1];
            fprintf(fp, "%ld-%ld\t%lu\n", j, k, _p[j][k]);
        }
        fprintf(fp,"\nTotal =\t%lu\n", total);
        fclose(fp);
    }
#endif
    return total;
}

/*
 * Structures de donnees utilisees
 */

#define UNLABELLED      0
#define LBL_INNER       1
#define LBL_OUTER       2

typedef struct _pseudo_sommet {
    long cardinal;
    long label;
    weight z;
    struct _pseudo_sommet *suivant;
    struct _pseudo_sommet *parent;
    struct _pseudo_sommet *aine;
    struct _pseudo_sommet *conjoint;
    struct _pseudo_sommet *vers_racine;
    struct _pseudo_sommet **voisins;
} pseudo_sommet;

typedef struct _sommet {
    weight y;
    pseudo_sommet *interne, *externe;
} sommet;

static pseudo_sommet *liste_pseudo_sommets;
static sommet *liste_sommets;
static long **e_star;

/*
 * Cree un pseudo_sommet, sans le lier aux autres
 */

static pseudo_sommet *
creer_pseudo_sommet (long cardinal, long label) {
    pseudo_sommet *ps;

    ED_CALLOC(ps, 1, pseudo_sommet);
    ps->cardinal = cardinal;
    ps->label = label;
    ps->z = (weight)0;
    ps->parent = ps->aine = ps->conjoint = ps->vers_racine = NULL;
    ED_CALLOC(ps->voisins, nb_sommets, pseudo_sommet*);
    ps->voisins[0] = NULL;
    return ps;
}

/*
 * Detruit un pseudo-sommet, sans le delier des autres
 */

static void detruire_pseudo_sommet (pseudo_sommet *ps) {
    assert(ps);
    assert(ps->voisins);
    free(ps->voisins);
    free(ps);
}

/*
 * Initialisation des structures de donnees. Nous affichons une "regle" de
 * [nb_sommets/2] signes "-" entre crochets; c'est le nombre maximum
 * d'augmentations fortes qui peuvent avoir lieu. Pour nous, le maximum
 * sera toujours atteint (tout couplage optimal est parfait).
 */

static void edmonds_init (void) {
    long i;
    sommet *s;
    pseudo_sommet *ps;

#ifndef EDEBUG
    fprintf(stderr, "[");
    for (i = 0; i < nb_sommets/2; i++)
        fprintf(stderr, "-");
    fprintf(stderr, "]\r[");
#endif

    /* D'abord initialiser la liste des sommets */
    liste_pseudo_sommets = NULL;
    ED_CALLOC(liste_sommets, nb_sommets, sommet);
    for (i = 0; i < nb_sommets; i++) {
        ps = creer_pseudo_sommet(1, UNLABELLED);
        s = liste_sommets + i;
        s->y = (weight)0;
        s->interne = s->externe = ps;
        ps->suivant = liste_pseudo_sommets;
        liste_pseudo_sommets = ps;
    }
    /* Puis allouer de la memoire pour le graphe G' */
    ED_CALLOC(e_star, nb_sommets, long*);
    for (i = 0; i < nb_sommets; i++)
        ED_CALLOC(e_star[i], nb_sommets, long);
}

/*
 * Liberation de ces memes structures de donnees. A la fin de l'execution
 * de l'algorithme, il ne doit pas rester de blossoms (il y a donc
 * exactement autant de pseudo-sommets que de sommets)
 */

static void edmonds_end (void) {
    long i;
    sommet *s;
    pseudo_sommet *ps;

#ifndef EDEBUG
    fprintf(stderr, "\n");
#endif

    for (i = 0; i < nb_sommets; i++)
        free(e_star[i]);
    free(e_star);

    /* Verifier le nombre de pseudo-sommets */
    for (i=0, ps=liste_pseudo_sommets; ps; ps=ps->suivant) {
        assert(ps->cardinal == 1);
        ++i;
    }
    assert(i == nb_sommets);

    for (i = 0; i < nb_sommets; i++) {
        s = liste_sommets + i;
        ps = s->interne;
        assert(ps); assert(ps == s->externe);
        detruire_pseudo_sommet(ps);
    }
    free(liste_sommets);
}

/*
 * Ajoute v a la liste des voisins de u. Renvoie 1 en cas de succes, sinon 0
 * (si v etait deja voisin de u)
 */

static long ajoute_voisin (pseudo_sommet *u, pseudo_sommet *v) {
    pseudo_sommet **u1;

    assert(u); assert(v);
    for (u1 = u->voisins; *u1; ++u1)
        if (*u1 == v)
            return 0;
    *u1++ = v;
    *u1 = NULL;
    return 1;
}

#ifdef EDEBUG
/*
 * Verifie que deux pseudo-sommets sont voisins
 */
static long sont_voisins(pseudo_sommet *u, pseudo_sommet *v) {
    long i, j;

    for (i = 0; i < nb_sommets; i++)
        if (liste_sommets[i].externe == u)
        for (j = 0; j < nb_sommets; j++)
            if (liste_sommets[j].externe == v && e_star[i][j])
            return 1;
    return 0;
}
#endif

/*
 * Mise a jour des voisins, apres la contraction ou l'expansion
 * d'un blossom, ou apres augmentation de E*
 */

static void recalculer_voisins (void) {
    pseudo_sommet *t, *v;
    long i, j;

    /* D'abord purger toutes les listes de voisins */
    for (t = liste_pseudo_sommets; t; t = t->suivant)
        t->voisins[0] = NULL;

    for (i = 0; i < nb_sommets; i++) {
        t = liste_sommets[i].externe;
        for (j = 0; j < nb_sommets; j++)
            if ((v = liste_sommets[j].externe)!=t && e_star[i][j])
                ajoute_voisin(t, v);
    }
}

/*
 * Mise a jour des pseudo-sommets externes, i.e. determiner quel est le
 * pseudo-sommet le plus externe contenant un sommet donne.
 */

static void recalculer_ps_externes (void) {
    long i;
    sommet *s;
    pseudo_sommet *t;

    for (i = 0; i < nb_sommets; i++) {
        s = liste_sommets + i;
        for (t = s->interne; t->parent; )
            t = t->parent;
        s->externe = t;
    }
}

/*
 * Expansion d'un blossom externe, en induisant un couplage maximum parmi
 * les pseudo-sommets ainsi liberes.
 */

static void expande_blossom (pseudo_sommet *u) {
    pseudo_sommet **old, *t, *c, *ia, *ib, *t1;
    long i, j;

    TRACE((stderr, "Expansion d'un blossom [\n"));
    /* Verifier que le sommet est bien externe */
    assert(u); assert(u->cardinal > 1 && u->cardinal % 2);
    assert(u->parent == NULL); assert(u->aine);

    TRACE((stderr, "Recherche d'un conjoint\n"));
    ia = u->aine;
    ia->conjoint = NULL;
    /* Avons-nous un conjoint ? */
    if ((c = u->conjoint) != NULL) {
        assert(c->conjoint == u);
        assert((c->label == LBL_OUTER  && u->label == LBL_INNER)
            || (c->label == UNLABELLED && u->label == UNLABELLED));
        /* Lequel de nos enfants va recuperer le conjoint ? */
        for (i = 0; i < nb_sommets; i++)
            if (liste_sommets[i].externe == c)
                for (j = 0; j < nb_sommets; j++)
                    if (liste_sommets[j].externe ==u && e_star[i][j]) {
                        /* Trouver le fils */
                        for (t = liste_sommets[j].interne;
                            t && t->parent != u; t = t->parent)  ;
                        assert(t->parent == u);
                        t->conjoint = c;
                        c->conjoint = ia = t;
                        if (c->vers_racine == u)
                            c->vers_racine = ia;
                        goto suite;
                    }
        assert(0);  /* le conjoint est introuvable?? */
suite: ;
    }

    /*
     * Si le sommet est etiquete 'inner', nous devons faire attention
     * a etiqueter correctement les pseudo-sommets liberes. Pour cela
     * nous cherchons s'il existe un pseudo-sommet 'ib' fils de u tel
     * que ib->vers_racine soit hors de u. Nous numerotons alors
     * alternativement les fils afin que ia et ib soient tous les deux
     * inner.
     */
    ib = NULL;
    if (u->label == LBL_INNER) {
        TRACE((stderr, "Recherche d'un point d'aboutissement\n"));
        c = u->vers_racine; assert(c);
        assert(c->label == LBL_OUTER);
        for (i = 0; i < nb_sommets; i++)
            if (liste_sommets[i].externe == c)
                for (j = 0; j < nb_sommets; j++)
                    if (liste_sommets[j].externe ==u && e_star[i][j]) {
                        /* Trouver le fils */
                        for (t = liste_sommets[j].interne;
                            t && t->parent != u; t = t->parent)  ;
                        assert(t->parent == u);
                        ib = t;
                        ib->vers_racine = c;
                        goto st2;
                    }
        assert(0);  /* point d'aboutissement introuvable?? */
st2:    assert(ib);
    }

    /* Nous devons maintenant apparier les enfants autres que 'ia' */
    TRACE((stderr, "Appariement des fils\n"));
    for (t = ia, i = -1; ; ) {
        t = t->suivant ? t->suivant : u->aine;
        if (t == ia)
            break;
        t1 = t;
        t = t->suivant ? t->suivant : u->aine;
        t1->conjoint = t;
        t->conjoint = t1;
        if (t1 == ib)
            i = 1;
        else if (t == ib)
            i = 0;
    }
    if (ia == ib)
        i = 0;

    /* Et labeller nos enfants */
    TRACE((stderr, "Etiquetage des fils\n"));
    ia->label = u->label;
    if (i == 0) {
        assert(ib);
        t = ia;
        while (1) {
            t->label = LBL_INNER;
            if (t == ib) break;
            t1 = t;
            t = t->suivant ? t->suivant : u->aine;
            t ->label = LBL_OUTER;
            t1->vers_racine = t;
            t1 = t;
            t = t->suivant ? t->suivant : u->aine;
            t1->vers_racine = t;
        }
        assert(t == ib);
        t = t->suivant ? t->suivant : u->aine;
        while (t != ia) {
            t->label = UNLABELLED;
            t->vers_racine = NULL;
            t = t->suivant ? t->suivant : u->aine;
        }
    } else if (i == 1) {
        assert(ib);
        t = ib;
        while (1) {
            t->label = LBL_INNER;
            if (t == ia) break;
            t1 = t;
            t = t->suivant ? t->suivant : u->aine;
            t->label = LBL_OUTER;
            t->vers_racine = t1;
            t1 = t;
            t = t->suivant ? t->suivant : u->aine;
            t->vers_racine = t1;
        }
        assert(t == ia);
        t = t->suivant ? t->suivant : u->aine;
        while (t != ib) {
            t->label = UNLABELLED;
            t->vers_racine = NULL;
            t = t->suivant ? t->suivant : u->aine;
        }
    } else {
        assert(ib == NULL);
        /*
         * Si nous arrivons ici, c'est que le blossom n'est pas
         * inner, donc que nous sommes dans la phase finale
         * d'expansion des blossoms. Il ne faut surtout pas
         * que les fils soient etiquetes 'inner', sinon
         * expande_blossom() rechercherait un point d'aboutissement
         */
        for (t = u->aine; t; t = t->suivant) {
            t->label = UNLABELLED;
            t->vers_racine = NULL;
        }
    }

    assert(ib == NULL || ia->label == LBL_INNER);
    assert(ib == NULL || ib->label == LBL_INNER);

    /* Unlinker u de la liste des sommets externes */
    TRACE((stderr, "Mise a jour de la liste des pseudo-sommets\n"));
    old = &liste_pseudo_sommets;
    for (t = *old; t && t != u; t = t->suivant)
        old = &(t->suivant);
    assert(*old == u);
    *old = u->aine;

    /* Suivre la liste des fils jusqu'au bout */
    old = &(u->aine);
    for (t = *old; t; t = t->suivant) {
        t->parent = NULL;
        old = &(t->suivant);
    }
    assert(*old == NULL);
    *old = u->suivant;

    /* Nous n'avons plus besoin de u */
    detruire_pseudo_sommet(u);

    recalculer_ps_externes();
    TRACE((stderr, "Blossom totalement detruit ]\n"));
}

/*
 * Contracte un blossom delimite par deux pseudo-sommets etiquetes 'outer'
 * dans l'arbre de recherche, et renvoie un pointeur sur le pseudo-sommet
 * obtenu.
 */

static pseudo_sommet *contracte_blossom (pseudo_sommet *u, pseudo_sommet *v) {
    long i, alpha, card, dist_u, dist_v;
    pseudo_sommet *up, *vp, *s, *t, *blossom, **old;

    TRACE((stderr, "Contraction d'un blossom [\n"));

    assert(u); assert(u->cardinal % 2); assert(u->parent == NULL);
    assert(u->label == LBL_OUTER);
    assert(v); assert(v->cardinal % 2); assert(v->parent == NULL);
    assert(v->label == LBL_OUTER);
    /*
     * La premiere etape consiste a trouver le point de rencontre
     * des chemins u->racine et v->racine; on aura une situation du
     * genre suivant:
     *
     *                    *- ... -- U
     *                   /
     * Racine -- ... -- S
     *                   \
     *                    *- ... -- V' -- ... -- V
     *
     * Le blossom sera alors U -- ... -- S -- ... -- V' -- ... -- V.
     */
    for (dist_u = 0, t = u; t; t = t->vers_racine)
        ++dist_u;
    for (dist_v = 0, t = v; t; t = t->vers_racine)
        ++dist_v;
    if (dist_v > dist_u) {
        up = u;
        for (vp = v, i = 0; i < dist_v - dist_u; ++i) {
            assert(vp);
            vp = vp->vers_racine;
        }
    } else {
        vp = v;
        for (up = u, i = 0; i < dist_u - dist_v; ++i) {
            assert(up);
            up = up->vers_racine;
        }
    }
    /* Calcul de s et alpha=dist(s,u')=dist(s,v') */
    TRACE((stderr, "Recherche du point de jonction\n"));
    s = up;
    t = vp;
    for (alpha = 0; s != t; ++alpha) {
        assert(s != NULL && t != NULL);
        s = s->vers_racine;
        t = t->vers_racine;
    }
    assert(s->label == LBL_OUTER);
    assert(s->vers_racine == s->conjoint);
    /*
     * Nous pouvons maintenant creer le pseudo-sommet et faire pointer
     * ses fils sur lui
     */
    TRACE((stderr, "Creation du parent\n"));
    card = 2*alpha + 1 + abs(dist_u - dist_v);
    assert(card%2 == 1);
    blossom = creer_pseudo_sommet (card, LBL_OUTER);
    TRACE((stderr, "cardinal == %ld\n", card));
    for (t = u; t != s; t = t->vers_racine)
        t->parent = blossom;
    for (t = v; t != s; t = t->vers_racine)
        t->parent = blossom;
    s->parent = blossom;
    /*
     * Puis les problemes de conjoint; seul S peut en avoir un
     */
    TRACE((stderr, "Contraction du conjoint\n"));
    if ((t = s->conjoint) != NULL) {
        assert(t->conjoint == s);
        assert(t->parent == NULL);
        t->conjoint = blossom;
        blossom->conjoint = t;
    }
    /* Retirer les fils de la liste des sommets externes */
    TRACE((stderr, "Mise a jour des liens\n"));
    old = &liste_pseudo_sommets;
    for (t = *old; t; t = t->suivant)
        if (t->parent == NULL) {
            *old = t;
            old = &(t->suivant);
        }
    *old = NULL;
    /* Puis ajouter "blossom" au debut de la liste */
    blossom->suivant = liste_pseudo_sommets;
    liste_pseudo_sommets = blossom;

    /*
     * Maintenant que les fils ont ete unlinkes de la liste des
     * pseudo-sommets externes, nous pouvons utiliser le champ
     * "suivant" pour les chainer, et initialiser le champ "aine"
     * du parent
     */
    TRACE((stderr, "Chainage des fils\n"));
    blossom->aine = u;
    for (t = u; t != s; t = t->vers_racine)
        t->suivant = t->vers_racine;
    for (t = v; t != s; t = t->vers_racine)
        t->vers_racine->suivant = t;
    v->suivant = NULL;

    /*
     * Si un pseudo-sommet pointait (via ->vers_racine) vers un fils,
     * il faut le faire pointer sur blossom.
     * Egalement, initialiser le champ blossom->vers_racine.
     */
    TRACE((stderr, "Contraction de l'arbre de recherche\n"));
    for (t = liste_pseudo_sommets; t; t = t->suivant)
        if (t->vers_racine && t->vers_racine->parent) {
            assert(t->vers_racine->parent == blossom);
            t->vers_racine = blossom;
        }
    blossom->vers_racine = s->vers_racine;
    assert(blossom->vers_racine == blossom->conjoint);

    recalculer_ps_externes();
    TRACE((stderr, "Blossom entierement cree ]\n"));
    return blossom;
}

/*
 * Enleve toutes les etiquettes et efface l'arbre de recherche
 */

static void enleve_etiquettes (void) {
    pseudo_sommet *t;

    TRACE((stderr, "Effacement de toutes les etiquettes\n"));

    for (t = liste_pseudo_sommets; t; t = t->suivant) {
        t->label = UNLABELLED;
        t->vers_racine = NULL;
    }
}

/*
 * Choix d'une racine. Renvoie 0 si ce n'est pas possible, cad si tous les
 * sommets verifient la "vertex slackness condition". Dans ce cas
 * l'algorithme se termine. Renvoie 1 sinon.
 *
 * IMPORTANT: Cet algorithme n'est pas parfait, il peut occasionnellement
 * renvoyer un sommet verifiant deja la v.s.c., mais jamais deux fois.
 * Au moins, cela evitera les boucles infinies. (Un pseudo-sommet non
 * apparie contient exactement un sommet non apparie, mais les structures
 * de donnees presentes ne stockent pas cette information; il faudra
 * vraiment corriger ca un jour.)
 */

static sommet *racine;

static long choisir_racine (void) {
    long i;
    sommet *s;
    static long last = -1;

    for (i = last+1; i < nb_sommets; i++) {
        s = liste_sommets + i;
        if (s->externe->conjoint == NULL && (s->y) > (weight)0) {
            racine = s;
            racine->externe->label = LBL_OUTER;
            TRACE((stderr,"Nouvelle racine choisie: %ld\n",i));
            last = i;
            return 1;
        }
    }
    TRACE((stderr, "Tous les sommets sont satures\n"));
    last = -1;
    return 0;
}

/*
 * Expansion de tous les blossoms externes etiquetes 'inner' dont la
 * variable z est egale a zero
 */

static void expande_ps_internes (void) {
    pseudo_sommet *t;

    TRACE((stderr, "Disparition d'un inner-blossom (au moins)\n"));
    do {
        for (t = liste_pseudo_sommets; t; t = t->suivant)
            if (t->label == LBL_INNER &&
            t->cardinal > 1 && t->z == (weight)0) {
                expande_blossom(t);
                break;
            }
    } while (t);
}

/*
 * Expansion de tous les blossoms restants
 */

static void expande_tous_blossoms (void) {
    pseudo_sommet *t;

    TRACE((stderr, "Disparition de tous les blossoms restants\n"));
    do {
        for (t = liste_pseudo_sommets; t; t = t->suivant)
            if (t->parent == NULL && t->cardinal > 1) {
                expande_blossom(t);
                break;
            }
    } while (t);
}

/*
 * Ajoute des aretes au graphe G', et renvoie le nombre d'aretes ajoutees.
 * Attention, certaines aretes de G' peuvent aussi disparaitre (mais pas
 * celles de l'arbre de recherche, ni celles qui sont enfermees dans un
 * blossom).
 *
 * Cette fonction n'est pas utilisable pour initialiser E*, mais uniquement
 * pour le recalculer apres DVC. La valeur de delta doit etre STRICTEMENT
 * POSITIVE.
 */

static long recalcule_e_star (void) {
    long i, j, newa;
    sommet *u, *v;
    pseudo_sommet *u1, *v1;
    weight dif;

    TRACE((stderr, "Mise a jour de E*\n"));

#ifdef EDEBUG
    for (u1 = liste_pseudo_sommets; u1; u1 = u1->suivant)
        if (u1->conjoint)
            assert(sont_voisins(u1,u1->conjoint));
#endif
    newa = 0;
    for (i = 0; i < nb_sommets; i++) {
        u = liste_sommets + i;
        for (j = 0; j < i; j++) {
            v = liste_sommets + j;
            /*
             * Si l'un des sommets est inner et l'autre outer, ou
             * s'ils sont non etiquetes, alors la valeur de e*(i,j)
             * ne change pas. Meme chose si les deux sommets sont
             * contenus dans un meme pseudo-sommet.
             */
            u1 = u->externe;
            v1 = v->externe;
            if ((u1 == v1) ||
                (u1->label == LBL_INNER  && v1->label == LBL_OUTER) ||
                (u1->label == LBL_OUTER  && v1->label == LBL_INNER) ||
                (u1->label == UNLABELLED && v1->label == UNLABELLED))
                continue;
            /*
             * Si l'un des sommets est inner, alors l'autre est inner
             * ou unlabelled; et on peut etre certain que l'arete va
             * sortir du graphe, car la quantite "dif" sera augmentee
             * de (delta) ou (2*delta).
             */
            if ((u1->label == LBL_INNER) || (v1->label == LBL_INNER)) {
                if (e_star[i][j])
                    e_star[i][j] = e_star[j][i] = 0;
                continue;
            }
            /*
             * Sinon, l'un des label est outer et l'autre est outer
             * ou bien unlabelled, et il est possible qu'une arete
             * soit ajoutee au graphe; et on peut etre certain qu'elle
             * n'y etait pas auparavant, car "dif" a strictement
             * diminue.
             */
            dif = (u->y) + (v->y) - p[i][j];
            assert(dif >= (weight)0);
            assert(e_star[i][j] == 0);

            if (dif == (weight)0) {
                ++newa;
                e_star[i][j] = e_star[j][i] = 1;
            }
        } /* for (j...) */
    } /* for (i...) */
    TRACE((stderr, "Ajout de %ld arete(s) a E*\n", newa));

#ifdef EDEBUG
    for (u1 = liste_pseudo_sommets; u1; u1 = u1->suivant)
        if (u1->conjoint)
            assert(sont_voisins(u1,u1->conjoint));
#endif
    return newa;
}

/*
 * Initialisation du graphe G'
 */

static void initialise_e_star (void) {
    long i, j;
    weight W;

    W = -INFINI;
    for (i = 0; i < nb_sommets; i++)
        for (j = 0; j < i; j++)
            if (p[i][j] > W)
                W = p[i][j];

    for (i = 0; i < nb_sommets; i++)
        for (j = 0; j < i; j++)
            e_star[i][j] = e_star[j][i] = (p[i][j] == W ? 1 : 0);

    W /= 2;
    TRACE((stderr, "W = %f\n", W));

    for (i = 0; i < nb_sommets; i++) {
        liste_sommets[i].y = W;
        e_star[i][i] = 0;   /* On ne sait jamais... */
    }
}

/*
 * Les trois procedures suivantes (DEV, DVC, HUT) font reference a des
 * variables communes delta, delta_[1-4], donc nous les definissons ici
 */

static weight delta, delta_1, delta_2, delta_3, delta_4;

/*
 * Delta EValuation procedure
 */

static void DEV (void) {
    long i, j;
    weight mu;
    pseudo_sommet *z;

    TRACE((stderr, "DEV\n"));

    delta_1 = INFINI;
    for (z = liste_pseudo_sommets; z; z = z->suivant)
        if (z->cardinal > 1 && z->label == LBL_INNER)
            if (z->z < delta_1)
                delta_1 = z->z;
    delta_1 /= 2;

    delta_2 = INFINI;
    for (i = 0; i < nb_sommets; i++)
        if (liste_sommets[i].externe->label == LBL_OUTER)
            if (liste_sommets[i].y < delta_2)
                delta_2 = liste_sommets[i].y;

    delta_3 = INFINI;
    for (i = 0; i < nb_sommets; i++)
        if (liste_sommets[i].externe->label == LBL_OUTER)
        for (j = 0; j < nb_sommets; j++)
            if (liste_sommets[j].externe->label == UNLABELLED) {
                mu = liste_sommets[i].y + liste_sommets[j].y - p[i][j];
                if (mu < delta_3)
                    delta_3 = mu;
            }

    delta_4 = INFINI;
    for (i = 0; i < nb_sommets; i++)
        if (liste_sommets[i].externe->label == LBL_OUTER)
        for (j = 0; j < nb_sommets; j++)
            if (liste_sommets[j].externe->label == LBL_OUTER &&
            liste_sommets[i].externe < liste_sommets[j].externe) {
                mu = liste_sommets[i].y + liste_sommets[j].y
                - p[i][j];
                if (mu < delta_4)
                delta_4 = mu;
            }
    delta_4 /= 2;

    delta = delta_1;
    if (delta_2 < delta)
        delta = delta_2;
    if (delta_3 < delta)
        delta = delta_3;
    if (delta_4 < delta)
        delta = delta_4;

    TRACE((stderr, "delta_1 = %e\n", delta_1));
    TRACE((stderr, "delta_2 = %e\n", delta_2));
    TRACE((stderr, "delta_3 = %e\n", delta_3));
    TRACE((stderr, "delta_4 = %e\n", delta_4));
    TRACE((stderr, "delta   = %e\n", delta  ));
}

/*
 * Dual Variables Change
 */

static void DVC (void) {
    long i;
    pseudo_sommet *t, *u;

    TRACE((stderr, "DVC\n"));

    /* D'abord verifier l'integrite des labels */
    for (t = liste_pseudo_sommets; t; t = t->suivant)
        if (t->conjoint)
        assert(t->conjoint != t && t->conjoint->conjoint == t);

    for (t = liste_pseudo_sommets; t; t = t->suivant)
        if ((u = t->conjoint) != NULL) {
        assert((t->label == LBL_INNER  && u->label == LBL_OUTER)
            || (t->label == LBL_OUTER  && u->label == LBL_INNER)
            || (t->label == UNLABELLED && u->label == UNLABELLED));
        }
    /* Ok */

    assert(delta >= (weight)0);
    if (delta == (weight)0)
        return;  /* Ne rien faire */

    for (i = 0; i < nb_sommets; i++)
        if (liste_sommets[i].externe->label == LBL_OUTER) {
            liste_sommets[i].y -= delta;
            assert(liste_sommets[i].y >= (weight)0);
        } else if (liste_sommets[i].externe->label == LBL_INNER)
            liste_sommets[i].y += delta;

    for (t = liste_pseudo_sommets; t; t = t->suivant)
        if (t->cardinal > 1) {
            if (t->label == LBL_OUTER)
                t->z += 2*delta;
            else if (t->label == LBL_INNER) {
                t->z -= 2*delta;
                assert(t->z >= (weight)0);
            }
        }
}

/*
 * Les fonctions augmentation_neutre(u) et augmentation_forte(u) prennent
 * en argument un pseudo-sommet u (deja apparie dans le premier cas,
 * libre dans le second) contenant un sommet en lequel y=0. On realise
 * alors l'augmentation en echangeant, entre ce sommet et la racine, les
 * aretes qui appartiennent au couplage, et celles qui n'y appartiennent pas.
 *
 * Si l'argument u est NULL, ces fonctions cherchent un sommet u verifiant
 * les conditions ci-dessus.
 */

static void augmentation_neutre (pseudo_sommet *u) {
    long i;
    sommet *s;
    pseudo_sommet *v;

    if (u) {
        TRACE((stderr,"Augmentation neutre\n"));
        assert(u->label == LBL_OUTER);
        assert(u->conjoint == u->vers_racine);
        u->conjoint = NULL;
        u = u->vers_racine;
        while (u) {
            v = u->vers_racine;
            assert(v);
#ifdef EDEBUG
            assert(sont_voisins(u,v));
#endif
            u->conjoint = v;
            v->conjoint = u;
            u = v->vers_racine;
        }
        return;
    }
    /* Nous devons nous-memes trouver u */
    for (i = 0; i < nb_sommets; i++) {
        s = liste_sommets + i;
        u = s->externe;
        if (u->label == LBL_OUTER && s->y == (weight)0) {
            augmentation_neutre(u);
            return;
        }
    }
    assert(0);
}

/*
 * Nous affichons un "+" a chaque augmentation forte; si le couplage de
 * poids maximum est un couplage parfait (ce sera toujours le cas pour nous)
 * alors il y aura exactement [nb_sommets/2] augmentations fortes.
 */

static void augmentation_forte (pseudo_sommet *u) {
    pseudo_sommet *v;

    if (u) {
        TRACE((stderr,"Augmentation forte\n"));
#ifndef EDEBUG
        fprintf(stderr, "+");
# ifdef __THINK_C__
        /*
         * Think-C bufferise stderr `par ligne', il est donc
         * necessaire de vider explicitement le tampon ici
         */
        fflush(stderr);
# endif
#endif
        assert(u->conjoint == NULL);
        v = u->vers_racine;
        assert(v != NULL);
        augmentation_neutre(v);
        assert(v->conjoint == NULL);
#ifdef EDEBUG
        assert(sont_voisins(u,v));
#endif
        u->conjoint = v;
        v->conjoint = u;
        return;
    }
    /* Sinon, nous devons nous-memes trouver u */
    u = liste_pseudo_sommets;
    while (u) {
        if (u->conjoint == NULL && u->vers_racine != NULL) {
            augmentation_forte(u);
            return;
        }
        u = u->suivant;
    }
    assert(0);
}


/*
 * La fonction HUT renvoie HUT_NEWROOT ou bien HUT_MAPS selon que
 * delta == delta_2 ou non
 */

#define HUT_NEWROOT     0
#define HUT_MAPS        1

static long HUT (void) {
    long newa;       /* Nb de nouvelles aretes de (E*) */

    TRACE((stderr, "HUT\n"));
    DEV();
    DVC();
    newa = (delta == (weight)0) ? 0 : recalcule_e_star();

    if (delta == delta_1)
        expande_ps_internes();
    if (delta == delta_3 || delta == delta_4)
        assert(newa > 0);
    if (delta == delta_2 && racine->y)
        augmentation_neutre(NULL);
    if (delta == delta_2) {
        /* La racine verifie la vertex slackness condition */
        enleve_etiquettes();
        return HUT_NEWROOT;
    }
    return HUT_MAPS;
}

/*
 * La procedure d'exploration, MAPS. Renvoie MAPS_AUGMENT si un chemin
 * augmentant a ete trouve, et MAPS_HUT si on a decouvert un arbre
 * hongrois.
 *
 * NB: la contraction des blossoms a lieu ici, et non pas dans la boucle
 * principale. Nous n'avons donc pas a indiquer un troisieme code de sortie
 * pour signaler qu'un blossom a ete trouve.
 */

#define MAPS_AUGMENT    0
#define MAPS_HUT        1

static long MAPS (void) {
    pseudo_sommet *x, **xv, *y, *z;
    long explored;

    TRACE((stderr, "MAPS\n"));
    recalculer_voisins();
redo:
    explored = 0;

    for (x = liste_pseudo_sommets; x; x = x->suivant)
        if (x->label == LBL_OUTER)
            for (xv = x->voisins; (y = *xv) != NULL; ++xv) {
                if (y->label == LBL_INNER)
                    continue;
                if (y->conjoint == NULL && y->label == UNLABELLED) {
                    y->vers_racine = x;
                    return MAPS_AUGMENT;
                }
                if (y->label == LBL_OUTER) {
                    contracte_blossom(x,y);
                    TRACE((stderr, "MAPS again\n"));
                    recalculer_voisins();
                    goto redo;
                }
                assert(y->label == UNLABELLED && y->conjoint != NULL);
                z = y->conjoint;
                assert(z->label == UNLABELLED && z->conjoint == y);
#ifdef EDEBUG
                assert(sont_voisins(y,z));
#endif
                y->vers_racine = x;
                z->vers_racine = y;
                y->label = LBL_INNER;
                z->label = LBL_OUTER;
                ++explored;
            }
    if (explored) {
        TRACE((stderr, "MAPS again\n"));
        goto redo;
    }
    return MAPS_HUT;
}

/*
 * Une fois tous les blossoms expandes, nous copions M dans le tableau t.
 */

static void remplir_t (void) {
    long i;
    pseudo_sommet *s;

    for (i = 0; i < nb_sommets; i++)
        liste_sommets[i].externe->label = i;
    for (i = 0; i < nb_sommets; i++) {
        s = liste_sommets[i].externe;
        if (s->conjoint)
            T[i] = s->conjoint->label;
        else    T[i] = i;
    }
}

/*
 * Routine principale
 */

static void edmonds (void) {
    initialise_e_star();

    while (choisir_racine()) {
        while (1) {
            if (MAPS() == MAPS_AUGMENT) {
                augmentation_forte(NULL);
                enleve_etiquettes();
                break;
            } else if (HUT() == HUT_NEWROOT)
                break;
        }
    }
    enleve_etiquettes();
    expande_tous_blossoms();
    remplir_t();
}
