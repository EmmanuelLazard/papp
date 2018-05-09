/*
 * joueurs.c: Toutes les operations sur les joueurs
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change
 * (EL) 16/07/2012 : v1.34, no change
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32 Ajout des nom de famille et prenom lors de l'enregistrement d'un joueur.
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include "changes.h"
#include "global.h"
#include "joueur.h"
#include "couplage.h"



/* Variables privees (enfin...) */

static Joueur *base_joueurs = NULL;
static Zone_Insertion *zi = NULL, **tail_zi = &zi;

Joueur *premier_joueur(void)        { return base_joueurs; }
Zone_Insertion *premiere_zone(void) { return zi; }

/*
 * Insertion d'un nouveau joueur dans la base. REMARQUE: il n'est pas
 * necessaire d'initialiser la base!
 * Une nouvelle structure est allouee pour le joueur, et ses champs sont
 * recopies en "memoire permanente" (i.e. il est impossible de la
 * desallouer; voir la definition de la fonction COPIER dans global.c).
 * Cette fonction renvoie un pointeur sur le nouveau joueur, ou NULL
 * si le joueur est deja dans la base.
 */

#ifndef HASHSIZE
# define HASHSIZE 512
#endif

static Joueur* hashtbl[HASHSIZE];

Joueur *nv_joueur (long numero, const char *nom, const char *prenom,
                   const char *programmeur, const char *pays, long classement,
                   const char *commentaire, const long nouveau) {
    Joueur *j;
    long i;
    char *buffer;

    assert(nom != NULL && pays != NULL);
    assert(numero > 0);
    if (trouver_joueur(numero))
        return NULL;
    CALLOC(j, 1, Joueur);
    buffer = strcpy(new_string(), nom);
    if (programmeur) {
        strcat(buffer, " (");
        strcat(buffer, programmeur);
        strcat(buffer, ")");
    } else if (prenom && *prenom) {
        strcat(buffer, " ");
        strcat(buffer, prenom);
    }
    COPIER(nom, &j->nom_famille);
	if (prenom != NULL) {
		COPIER(prenom, &j->prenom);
	} else {
		COPIER("", &j->prenom);
	}
    COPIER(buffer, &j->nom);
    COPIER(pays, &j->pays);
    j->numero  = numero;
	j->nouveau = nouveau;
    j->cl_jech = classement;
    if (commentaire && *commentaire)
        COPIER(commentaire, &j->comment);
    else
        j->comment = NULL;
    /* Initialiser la table de hachage si necessaire */
    if (base_joueurs == NULL) {
        for (i=0; i<HASHSIZE; i++)
            hashtbl[i]=NULL;
    }
    /* Puis il faut inserer le joueur dans la base */
    j->next = base_joueurs;
    base_joueurs = j;

    /* Et l'inserer dans la table de hachage */
    i = (unsigned)numero % HASHSIZE;
    j->nxt2 = hashtbl[i];
    hashtbl[i] = j;

    return j;
}

long nombre_joueurs_base() {
    long nb = 0;
    Joueur *j;

    for (j = base_joueurs; j; j = j->next)
        ++nb;
    return nb;
}

/*
 * Recherche d'un joueur connaissant son numero Elo; pour des raisons
 * d'efficacite, nous utilisons le hachage
 */
Joueur *trouver_joueur (long numero) {
    long i; Joueur *j;

    if (numero <= 0 || base_joueurs == NULL)
        return NULL;
    i = (unsigned)numero % HASHSIZE;
    for (j=hashtbl[i]; j; j=j->nxt2)
        if (j->numero == numero)
            return j;
    /* pas trouve */
    return NULL;
}

/* Creation d'une liste de joueurs (initialement vide) */
Liste_joueurs *creer_liste() {
    Liste_joueurs *liste;

    CALLOC(liste, 1, Liste_joueurs);
    liste->n = 0;
    liste->nb_slots = 25;
    CALLOC(liste->liste, liste->nb_slots, Joueur*);
    return liste;
}

/*
 * Ajout d'un element a une liste. Il peut etre necessaire pour
 * cela de reallouer le bloc de memoire.
 */
Liste_joueurs *ajouter_joueur(Liste_joueurs *lj, Joueur *j) {
    assert(lj->n <= lj->nb_slots && lj->liste);
    if (lj->n >= lj->nb_slots) {
        lj->nb_slots += 25;
        REALLOC(lj->liste, lj->nb_slots, Joueur*);
    }
    assert(lj->n < lj->nb_slots  && lj->liste);
    (lj->liste)[lj->n++] = j;
    return lj;
}

void vider_liste(Liste_joueurs *lj) {
    lj->n = 0;
}

static int tri_alphabetique (const void *ARG1, const void *ARG2) {
	Joueur **j1 = (Joueur **) ARG1 ;
	Joueur **j2 = (Joueur **) ARG2 ;

    assert(j1 && *j1 && j2 && *j2);
    return compare_chaines_non_sentitif((*j1)->nom, (*j2)->nom);
}

/*
 * Recherche dans la base les joueurs dont le nom commence par "chaine"
 * sans distinction min/maj, et renvoie les reponses dans la liste
 * pointee par "lj"; le resultat renvoye est l'adresse de la liste, soit
 * lj, sauf si lj contenait 0.
 */
Liste_joueurs *recherche_nom(Liste_joueurs *lj, const char *chaine) {
    Joueur *j;
    long len = strlen(chaine);

    if (lj==NULL)
        lj = creer_liste();
    else
        vider_liste(lj);

    for (j=base_joueurs; j; j=j->next) {
        if (!debuts_differents(chaine,j->nom,len))
            ajouter_joueur(lj,j);
    }
    /* Puis trier la liste par ordre alphabetique */
    SORT(lj->liste, lj->n, sizeof(Joueur*), tri_alphabetique);
    return lj;
}

/*
 *  Recherche dans une liste les joueurs dont le nom commence par "chaine"
 *  sans distinction min/maj, et renvoie les reponses dans la liste
 *  pointee par "lj"; le resultat renvoye est l'adresse de la liste, soit
 *  lj, sauf si lj contenait 0.
 *  Similaire a la fonction recherche_nom, mais recherche les completions
 *  possibles parmi les joueurs de la liste *liste_completion au lieu de
 *  les chercher dans toute la base.
 *
 */
Liste_joueurs *recherche_nom_dans_liste(Liste_joueurs *lj, const char *chaine, Liste_joueurs *liste_completion) {
    Joueur *j;
    long len = strlen(chaine);
    long nb_joueurs,i ;

    if (lj == NULL)
        lj = creer_liste();
    else
        vider_liste(lj);

    if (liste_completion != NULL) {
        nb_joueurs = liste_completion->n;
        if (nb_joueurs > 0)
            for (i = 0; i < nb_joueurs; i++) {
                assert(liste_completion->liste[i]);
                j = liste_completion->liste[i];
                if (!debuts_differents(chaine,j->nom,len))
                    ajouter_joueur(lj,j);
            }
    }

    /* Puis trier la liste par ordre alphabetique */
    if (lj->n > 0)
        SORT(lj->liste, lj->n, sizeof(Joueur*), tri_alphabetique);
    return lj;
}

/*
 * Cree une nouvelle zone d'insertion: [min_ins,max_ins]
 */
void nv_zone (const char *pays, long min_ins, long max_ins) {
    Zone_Insertion *z;

    if (min_ins < 0)
        min_ins = 0;
    if (min_ins > max_ins)
        return;     /* La zone est vide! */

    CALLOC(z, 1, Zone_Insertion);
    if (pays && *pays)
        COPIER(pays, &z->pays);
    else
        z->pays = NULL;

    z->min_ins = min_ins;
    z->max_ins = max_ins;
    z->next = NULL;
    *tail_zi = z;
    tail_zi = &(z->next);
}

/*
 * Cherche un numero libre entre <bas> et <haut>, renvoie -1 si
 * aucun n'est disponible
 */
static long _inserer_joueur(long bas, long haut) {
    long i;

    assert(0 <= bas && bas <= haut);
    for (i = bas; i <= haut; i++)
        if (trouver_joueur(i) == NULL)
            return i;
    /* pas trouve */
    return -1;
}

/*
 * Cherche un numero libre pour un joueur du pays indique. Contrairement
 * a la precedente, cette fonction ne doit jamais echouer (cela ne pourrait
 * se produire que si tous les numeros entre 1 et LONG_MAX etaient
 * occupes). Les zones nationales sont toujours scrutees avant les zones
 * internationales.
 */
long inserer_joueur(const char *pays) {
    long ret;
    Zone_Insertion *z;

    /* Essayer d'abord les zones nationales */
    for (z = zi; z; z = z->next)
        if (z->pays && !compare_chaines_non_sentitif(z->pays, pays) &&
            (ret = _inserer_joueur(z->min_ins, z->max_ins)) >= 0)
            return ret;

    /* Puis les zones internationales */
    for (z = zi; z; z = z->next)
        if (z->pays == NULL &&
            (ret = _inserer_joueur(z->min_ins, z->max_ins)) >= 0)
            return ret;

    /* En cas d'echec, attribuer le premier numero libre >= 1 */
    ret = _inserer_joueur(1, LONG_MAX);
    assert (ret > 0);
    return ret;
}

/*
 * char *coupon(j1, j2, score): affiche le coupon donnant le score de j1
 * contre j2; renvoie une chaine allouee statiquement. Si (j2=NULL), le
 * second joueur et le score ne sont pas affiches. Si (score<0) alors
 * le score n'est pas affiche.
 */
char *coupon (const Joueur *j1, const Joueur *j2, pions_t score) {
    static char tampon[200], tmp[80];
    static long inscrits = -1;
    static long maxlen;
    long i, v, padding;
    char *fs;
    Joueur *j;

    /*
     * S'il y a eu de nouveaux inscrits depuis la derniere fois ou
     * cette fonction a ete appelee, nous recalculons `maxlen',
     * la longueur maximum des demi-coupons.
     */
    if (joueurs_inscrits->n != inscrits) {
        inscrits = joueurs_inscrits->n;
        maxlen = 0;
        for (i = 0; i < inscrits; i++) {
            j = joueurs_inscrits->liste[i];
            sprintf(tmp, "%s (%ld)", j->nom, j->numero);
            if ((v = strlen(tmp)) > maxlen)
                maxlen = v;
        }
    }
    assert(maxlen > 0);
    assert(j1 != NULL);

    sprintf(tmp, "%s(%ld)", j1->nom, j1->numero);
    padding = maxlen - strlen(tmp);
    sprintf(tampon, " %s%*s(%ld)", j1->nom, (int)padding, "", j1->numero);
    if (j2 == NULL)
        return tampon;
    strcat(tampon, " -- ");
    sprintf(tmp, "%s(%ld)", j2->nom, j2->numero);
    padding = maxlen - strlen(tmp);
    sprintf(tmp, "%s%*s(%ld)", j2->nom, (int)padding, "", j2->numero);
    strcat(tampon, tmp);
    if ((fs = fscore(score)) != NULL) {
        strcat(tampon, "  ");
        strcat(tampon, fs);
    }
    return tampon;
}

#ifdef ENGLISH
#define ERES_I_COMPL    "Player < %s > didn't play this round"
#define ERES_I_AMBIG    "Several players begin with < %s >"
#define ERES_I_INCONNU  "No player begins with < %s > , type TAB to get help"
#else
#define ERES_I_COMPL    "Le joueur < %s > n'a pas joue a cette ronde"
#define ERES_I_AMBIG    "Plusieurs joueurs commencent par < %s >"
#define ERES_I_INCONNU  "Aucun joueur ne commence par < %s > , tapez TAB pour de l'aide"
#endif

/*
 * long nb_completions_dans_coupons(ligne,&nroJoueur,chaineScore) :
 * Entree d'un resultat de coupon avec le nom en toutes lettres. On cherche
 * d'abord dans les coupons non remplis, puis dans les autres.
 *
 * parametre d'entree   : *ligne
 * parametres de sortie : *nroJoueur
 *                        *chaineScore : doit etre allouee a l'exterieur de la fonction.
 *
 * valeurs possibles renvoyees :
 *    -1      pas de nom de joueur trouve, l'utilisateur a peut-etre utilise les numeros ELOS.
 *     0      le joueur n'est pas dans le tournoi; message d'erreur.
 *     1      reussite :
 *            *nroJoueur contient le numero ELO du joueur
 *            *chaineScore contient la fin de la ligne (apres le nom du joueur).
 *     >=2    nombre de joueurs dans les coupons commencant par ce prefixe;
 *            on en affiche la liste, puis un message d'erreur.
 *
 */
long nb_completions_dans_coupons(char *ligne, long *nroJoueur, char *chaineScore) {
    long n,i;
    long premier,longueur_nom,debut_chaine_score;
    char nom_joueur[TAILLE_TAMPON+1];
    char c,previous=0;
    long n1,n2 ;
	long nb_completions;
    pions_t v;
    Joueur *j1, *j2, *result=NULL;

    *nroJoueur = 0;


     /* on essaie de separer "ligne" entre un nom de joueur (nom_joueur)
        et une chaine correspondant au score (chaineScore) */

    n = strlen(ligne);
    if (n>TAILLE_TAMPON) n=TAILLE_TAMPON;

    i = 0;
    c = 0;
    premier = -1;  /* sera l'index du premier caractere different d'une espace */
    while (i<n) {
        previous = c;
        c = ligne[i];
        if ((premier < 0) && (c != ' '))
            premier = i;
        if ( isdigit(c) ||
             (c == '=') ||
             (c == '+') ||
             ((c == '-') && (isdigit(ligne[i+1]))) ||
             ((c == '-') && (previous == ' ')))
            break;
        i++;
    }
    if (premier < 0)
        premier = 0;
    debut_chaine_score = i;
    longueur_nom = i - premier;
    if ((previous == ' ') && (i<n))
        longueur_nom--;

    for (i=0;i<longueur_nom;i++)
        nom_joueur[i] = ligne[i+premier];
    nom_joueur[longueur_nom] = 0;
    longueur_nom = longueur_nom - enleve_espaces_de_droite(nom_joueur);

    for (i = debut_chaine_score; i < n; i++)
        chaineScore[i-debut_chaine_score] = ligne[i];
    chaineScore[n-debut_chaine_score] = 0;

    /* si le resultat n'a pas ete rentre en toutes lettres, on s'arrete la */
    if (longueur_nom <= 0)
        return -1;

    /* on recherche si, parmi les coupons non remplis,
        un joueur commence par "nom_joueur".
        Des qu'il y en a 2 ou plus, on affiche la liste */
    nb_completions = 0;
    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v))
        if (COUPON_EST_VIDE(v)) {
            j1 = trouver_joueur(n1);
            if ((j1 != NULL) &&
                !debuts_differents(nom_joueur, j1->nom, longueur_nom)) {
                nb_completions++;
                if (nb_completions == 2) puts(coupon(result, NULL, SCORE_INCONNU));
                if (nb_completions >= 2) puts(coupon(j1, NULL, SCORE_INCONNU));
                result = j1;
            }
            j2 = trouver_joueur(n2);
            if ((j2 != NULL) &&
                !debuts_differents(nom_joueur, j2->nom, longueur_nom)) {
                nb_completions++;
                if (nb_completions == 2) puts(coupon(result, NULL, SCORE_INCONNU));
                if (nb_completions >= 2) puts(coupon(j2, NULL, SCORE_INCONNU));
                result = j2;
            }
        }

    if (nb_completions == 1) {
        *nroJoueur = result->numero;
        return nb_completions;
    }

    /* Idem parmi les coupons remplis */
    iterer_ronde(ronde);
    while (couple_suivant(&n1,&n2,&v))
        if (COUPON_EST_REMPLI(v)) {
            j1 = trouver_joueur(n1);
            if ((j1 != NULL) &&
              !debuts_differents(nom_joueur,j1->nom,longueur_nom)) {
                nb_completions++;
                if (nb_completions == 2) puts(coupon(result, NULL, SCORE_INCONNU));
                if (nb_completions >= 2) puts(coupon(j1, NULL, SCORE_INCONNU));
                result = j1;
            }
            j2 = trouver_joueur(n2);
            if ((j2 != NULL) &&
              !debuts_differents(nom_joueur,j2->nom,longueur_nom)) {
                nb_completions++;
                if (nb_completions == 2) puts(coupon(result, NULL, SCORE_INCONNU));
                if (nb_completions >= 2) puts(coupon(j2, NULL, SCORE_INCONNU));
                result = j2;
            }
        }

    if (nb_completions >= 2)
        printf(ERES_I_AMBIG "\n", nom_joueur);  /* ambiguite */
    if (nb_completions <= 0)
        printf(ERES_I_COMPL "\n", nom_joueur);  /* aucune completion possible ! */

    if ((nb_completions >= 1) && (result != NULL))
        *nroJoueur = result->numero;
    return nb_completions;
}

#ifdef ENGLISH
#define BASE_JOUEURS_GROSSE  "I will not show you all the players in the database...\n" \
                             "Please type at least one letter before using the TAB\n"
#define NOM_TROP_LONG "That player name is too long ! (max %d caracters)\n"
#define ILLEGAL_FIRST "Player names must begin with a letter or a '_'\n"
#define ILLEGAL_ESPACE "Player names must begin with a letter or a '_'\n"
#define ILLEGAL_CARACT "That character is illegal in a player's name : %c\n"
#else
#define BASE_JOUEURS_GROSSE  "Je ne vais pas vous montrer toute la base des joueurs...\n"\
                             "Utilisez la touche TAB apres avoir tape au moins une lettre\n"
#define NOM_TROP_LONG "Ce nom de joueur est trop long ! (max %d caracteres)\n"
#define ILLEGAL_FIRST "Les noms des joueurs doivent commencer par une lettre ou par '_'\n"
#define ILLEGAL_ESPACE "Les noms de joueur ne peuvent pas etre composes que d'espaces...\n"
#define ILLEGAL_CARACT "Ce caractere est illegal dans un nom de joueur : %c\n"
#endif


/*
 * long enleve_espaces_de_gauche(chaine) :
 * Une fonction pour enlever les espaces de tete d'une chaine ;
 * renvoie le nombre d'espaces supprimees ; la chaine est modifiee en place.
 */
long enleve_espaces_de_gauche(char *chaine) {
    long nb_espaces,j,longueur;

    nb_espaces = 0;
    longueur = strlen(chaine);

    if ( longueur > 0 ) {
        while (chaine[nb_espaces] == ' ')
            nb_espaces++;
        if (nb_espaces > 0) {
            if (nb_espaces == longueur) { /* seulement des espaces ? => les effacer */
                for ( j=0 ; j < nb_espaces ; j++)
                    chaine[j]='\0';
            } else {     /* sinon, decaler la chaine pour annuler les espaces de tete */
                for (j=0 ; j <= longueur-nb_espaces ; j++)
                    chaine[j]=chaine[j+nb_espaces];
            }
        }
    }
    return nb_espaces;
}

/*
 * long enleve_espaces_de_droite(chaine) :
 * Une fonction pour enlever les espaces de queue d'une chaine ;
 * renvoie le nombre d'espaces supprimees ; la chaine est modifiee en place.
 */
long enleve_espaces_de_droite(char *chaine) {
    long nb_espaces,j,longueur;

    nb_espaces = 0;
    longueur = strlen(chaine);

    if ( longueur > 0 ) {
        j = longueur - 1;
        while ((j >= 0) && (chaine[j] == ' '))  {
            nb_espaces++;
            chaine[j]='\0';
            j--;
        }
    }
    return nb_espaces;
}

/*
 * long est_un_nom_de_joueur_valide(tampon) :
 * Verification grossiere de la legalite apparente du nom d'un nouveau joueur
 * que l'utilisateur rentre au clavier. Il faudrait idealement reconnaitre les
 * memes expressions regulieres que dans PAP.L ; ici on se contente de verifier
 * que le nom n'est pas trop long, que la premiere lettre est une lettre (a-z et A-Z)
 * ou un underscore (_) , et que les caracteres suivants n'ont pas l'air debiles.
 * Cette fonction pourrait etre rafinee !
 * Renvoie 1 si le nom a l'air valide, 0 sinon.
 */

long est_un_nom_de_joueur_valide(char *tampon) {
    long longueur, k;
    char c;

    longueur = strlen(tampon);

    if ( (longueur >= (ALLOC_RING_LENGTH-10)) || (longueur >= (TAILLE_TAMPON-1)) ) {
        printf(NOM_TROP_LONG,TAILLE_TAMPON);
        return 0;
    }

    if ( longueur > 0 ) {
        /* seulement des espaces ? */
        if (enleve_espaces_de_gauche(tampon) == longueur) {
            printf(ILLEGAL_ESPACE);
            return 0;
        }
        /* le premier caractere a-t-il l'air valide ? */
        c = tampon[0];
        if ( !isalpha(c) && (c != '_') && (c != ' ')) {
            printf(ILLEGAL_FIRST);
            return 0;
        }

        /* et les autres ? */
        longueur = strlen(tampon);
        for (k=1 ; k<longueur ; k++) {
            c = tampon[k];
            if ( !isalnum(c) && (c != '_') && (c != ' ') && (c != '(') && (c != ')') &&
                 (c != '\'') && (c != '-') && (c != '.')) {
                printf(ILLEGAL_CARACT , c);
                return 0;
            }
        }
    }

    return 1;
}

/*
 * long choix_d_un_joueur_au_clavier(prompt, parmi_ces_joueurs, &chaine) :
 * fonction gerant la specification d'un joueur par l'utilisateur, sous
 * la forme de son nom, d'un prefixe de son nom ou de son numero Elo.
 *
 * <prompt>             est le prompt affiche en debut de ligne.
 * <parmi_ces_joueurs>  est une liste de joueurs dans laquelle la fonction
 *                      cherche les completions quand l'utilisateur tape
 *                      sur [TAB]. Passer NULL dans <parmi_ces_joueurs>
 *                      pour chercher dans toute la base.
 * <&chaine>            est un POINTEUR sur la chaine de caractere que
 *                      l'utilisateur a rentree. Cette chaine est statique,
 *                      et doit donc etre copie quelque part avant que la
 *                      fonction ne soit invoquee une nouvelle fois.
 *
 * La fonction regarde d'abord si l'utilisateur a tape "*", qui est un joker
 * signifiant "tous les joueurs". La fonction renvoie alors TOUS_LES_JOUEURS.
 *
 * Puis on cherche dans la liste <parmi_ces_joueurs> (ou dans toute la base
 * si <parmi_ces_joueurs> est NULL) les joueurs dont le nom commence par ce qu'a
 * tape l'utilisateur:
 * -- s'il n'y qu'un seul joueur correspondant, on renvoie son numero
 * -- s'il y en a 2 ou plus, on affiche les completions possibles et on recommence
 * -- s'il n'y en a pas, on regarde si l'utilisateur n'aurait pas rentre directement
 *    un numero, que l'on renvoie si c'est le cas (attention : on ne verifie pas que
 *    ce numero fait bien partie de la liste <parmi_ces_joueurs>)
 * -- si rien ne marche, on affiche un petit message d'erreur et on recommence.
 *
 * La fonction renvoie -2 si l'utilisateur n'a rien tape.
 */

long choix_d_un_joueur_au_clavier(const char *prompt, Liste_joueurs *parmi_ces_joueurs, char **chaine) {
    long nro_joueur, i ,len;
    char *ligne;
    Liste_joueurs *completions = NULL;

    *chaine = "";
    while ((ligne = entree_clav_nom_joueur(prompt, *chaine, parmi_ces_joueurs))[0]) {

        len = strlen(ligne);
        if (enleve_espaces_de_gauche(ligne) == len) {
            printf(ILLEGAL_ESPACE);
            continue;
        }

        *chaine = ligne;

        /* joker '*' pour designer tous les joueurs */
        if ((ligne[0] == '*') && (strlen(ligne) == 1))
            return TOUS_LES_JOUEURS;

        if ((!isdigit(ligne[0])) && (!est_un_nom_de_joueur_valide(ligne))) {
            beep();
            continue;
        }

        if (parmi_ces_joueurs == NULL) {
            if ( (strlen(ligne) == 0) && (nombre_joueurs_base() >= 100) ) {
                /* ne pas afficher toute la base des joueurs  :-/   */
                printf(BASE_JOUEURS_GROSSE);
                if (completions==NULL)
                    completions = creer_liste();
                else
                    vider_liste(completions);
            } else
                completions = recherche_nom(completions, ligne);
        } else
            completions = recherche_nom_dans_liste(completions, ligne, parmi_ces_joueurs);

        assert( completions != NULL );

        if (completions->n == 1)
            return completions->liste[0]->numero;

        if (completions->n >= 2) {
            for (i=0 ; i < completions->n ; i++)
                printf(" %s\n",completions->liste[i]->nom);
            printf(ERES_I_AMBIG "\n", ligne);  /* ambiguite */
                   continue;
        }

        assert( completions->n == 0);

        /* est-ce directement un numero de joueur ? */
        if (sscanf(ligne,"%ld", &nro_joueur) == 1)
        return nro_joueur;

        printf(ERES_I_INCONNU "\n", ligne);  /* aucune completion possible ! */
       *chaine = "";
    }

return -2;  /* non trouve si la chaine est vide */
}
