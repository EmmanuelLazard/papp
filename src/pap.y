%{
/*
 * pap.y: Analyse syntaxique du fichier de configuration
 * ainsi que des fichiers de joueurs.
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 25/06/2012 : v1.34, Ajout du mot-clef 'KW_XML' indiquant s'il faut generer
                     les fichiers XML de sortie.
 * (EL) 20/07/2008 : v1.33, Ajout du token 'DIESE_DATE' pour lire la ligne du fichier intermediaire.
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, no change
 * (EL) 29/04/2007 : v1.31, no change
 * (EL) 30/03/2007 : Ajout des mots-clefs 'KW_DOSSIER', 'KW_TRUE' et 'KW_FALSE' indiquant
 *                   s'il faut regrouper tous les fichiers class/ronde/result...
 *                   dans un meme sous-dossier portant le nom du tournoi.
 * (EL) 07/02/2007 : Ajout des tokens 'DIESE_NOM', 'DIESE_RONDES' et
 *                   'DIESE_BRIGHTWELL_DBL' pour lire les lignes correspondantes
 *                   dans le fichier intermediaire.
 *                   Ajout des regles dans les commandes internes.
 * (EL) 04/02/2007 : Ajout du token 'KW_BRIGHTWELL_DBL' qui donne
 *                   la constante lorsqu'on compte 1 point par victoire.
 * (EL) 04/02/2007 : Test de la presence du type precedent (<= 1.29)
 *                   du fichier de config (avec Brightwell ou toutes-rondes)
 *                   -> memoriser dans variable 'type_fichier_config'
 * (EL) 02/02/2007 : Ajout du token 'INT_ET_DEMI' pour distinguer
 *                   du token 'DOUBLE' utilise uniquement pour
 *                   definir la constante de Brightwell.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "appari.h"
#include "couplage.h"
#include "global.h"
#include "joueur.h"
#include "pions.h"

#define yyerror(x)

#if defined(__MSDOS__) && !defined(MSDOS)
# define MSDOS
#endif

#if defined(__THINK_C__) || defined(PAPP_MAC_METROWERKS)
# define alloca malloc
#endif

#ifdef __BORLANDC__
#include <malloc.h>
#endif

#ifdef ENGLISH
# define NUM_ZERO               "player %s has number 0"
# define FOUND_DUP              "player %ld already exists"
# define BIP_ERROR              "impossible Bye score"
# define BIP_WINS               "Bye shouldn't win games"
# define BIP_TOTAL              "the total number of discs must be >0"
# define TTR_BADP               "bad parameters for toutes-rondes"
# define ZIS_BADP               "bad parameters for zone-insertion"
# define PEN_NEGATIVE           "negative penalty, raised to zero"
# define PEN_INFINITE           "huge penalty truncated"
# define PENC_NULL              "null color difference, penalty ignored"
# define PENF_NULL              "null float, penalty ignored"
# define PENCH_NULL             "null round, penalty ignored"
# define PENEL_NULL             "null round, penalty ignored"
# define PENCH_TOOFAR           "round number too great, penalty ignored"
# define PENEL_TOOFAR           "round number too great, penalty ignored"
# define ICMD_BPAIR             "bad pairing"
# define ICMD_APAIR             "player(s) already paired"
# define ICMD_TOTS              "bad total of discs"
# define ICMD_BRES              "bad result"
# define ICMD_DEMI              "impossible half-disc result"
# define TTR_TOOSMALL           "table-toutes-rondes is too small"
# define TTR_TOOBIG             "table-toutes-rondes is too big"
# define PLR_UNKNOWN            "unknown player %ld"
# define CMD_UNKNOWN            "unknown command name"
# define ERR_SYNTAX             "syntax error"
# define M_SEMICOLON            "missing semicolon"
# define ICMD_BAD_RND           "Bad number of rounds in 'papp-workfile' file"
# define ICMD_BAD_BC            "Bad Brightwell constant in 'papp-workfile' file"
#else
# define NUM_ZERO               "Le joueur %s a le numero 0"
# define FOUND_DUP              "le joueur %ld existe deja"
# define BIP_ERROR              "le score de Bip est incorrect"
# define BIP_WINS               "Bip ne devrait pas gagner"
# define BIP_TOTAL              "le total de pions doit etre >0"
# define TTR_BADP               "mauvais parametres pour toutes-rondes"
# define ZIS_BADP               "mauvais parametres pour zone-insertion"
# define PEN_NEGATIVE           "penalite negative, ramenee a zero"
# define PEN_INFINITE           "penalite trop grande, tronquee"
# define PENC_NULL              "ecart chromatique nul, penalite ignoree"
# define PENF_NULL              "flottement nul, penalite ignoree"
# define PENCH_NULL             "ronde zero, penalite ignoree"
# define PENEL_NULL             "ronde zero, penalite ignoree"
# define PENCH_TOOFAR           "ronde trop eloignee, penalite ignoree"
# define PENEL_TOOFAR           "ronde trop eloignee, penalite ignoree"
# define ICMD_BPAIR             "mauvais appariement"
# define ICMD_APAIR             "joueur(s) deja apparie(s)"
# define ICMD_TOTS              "mauvais total de pions"
# define ICMD_BRES              "mauvais resultat"
# define ICMD_DEMI              "resulat en demi pions impossible"
# define TTR_TOOSMALL           "table-toutes-rondes trop petite"
# define TTR_TOOBIG             "table-toutes-rondes trop grande"
# define PLR_UNKNOWN            "joueur %ld inconnu"
# define CMD_UNKNOWN            "nom de commande inconnu"
# define ERR_SYNTAX             "erreur de syntaxe"
# define M_SEMICOLON            "point-virgule attendu"
# define ICMD_BAD_RND           "Mauvais nombre de rondes dans le fichier intermediaire"
# define ICMD_BAD_BC            "Mauvaise constante de Brightwell dans le fichier intermediaire"
#endif

static char buffer[80];
static long i, taille_ttr, *table_ttr;
int yylex (void);

static char *
bout_a_bout (char *s1, char *s2) {
    char *s3, *p, *q;

    s3 = new_string();
    p = s1; q = s3;
    while ((*q++ = *p++))
        ;
    q[-1] = ' ';
    p = s2;
    while ((*q++ = *p++))
        ;
    return s3;
}

static void Nouveau_joueur (long numero, const char *nom, const char *firstname,
        const char *programmeur, const char *pays, long classement,
        const char *commentaire, const long nv) {
    if (!numero) {
        char *s = new_string();
        sprintf(s, NUM_ZERO, nom);  /* numero 0 interdit */
        erreur_syntaxe(s);
    }
    if (nv_joueur(numero, nom, firstname, programmeur, pays, classement, commentaire, nv) == NULL) {
        char *s = new_string();
        sprintf(s, FOUND_DUP, numero);  /* doublon */
        erreur_syntaxe(s);
    }
}

static void elargir_table (pen_t **table, long *taille, long nv_taille) {
    long i, o_taille;

    o_taille = *taille;
    assert(0 < o_taille && o_taille < nv_taille);
    REALLOC(*table, nv_taille, pen_t);
    for (i = o_taille; i < nv_taille; i++) {
        (*table)[i] = (*table)[o_taille-1];
    }
    *taille = nv_taille;
}

static void raz_toutes_penalites (void) {
    long i;

    /* Penalites de couleur */
    for (i = 0; i < nmax_couleur; i++)
        penalite_couleur[i] = 0;
    penalite_repcoul = 0;

    /* Penalites de flottement */
    for (i = 0; i < nmax_flottement; i++)
        penalite_flottement[i] = 0;
    penalite_flcum = minoration_fac = 0;

    /* Penalites de repetition */
    penalite_mcoul   = penalite_copp   =
    penalite_desuite = penalite_bipbip = 0;

    /* Penalites de chauvinisme */
    for (i = 0; i < NMAX_ROUNDS; i++)
        penalite_chauvinisme[i] = 0;

    /* Penalites d'elitisme */
#ifdef ELITISM
    for (i = 0; i < NMAX_ROUNDS; i++)
        penalite_elitisme[i] = 0;
#endif
}

%}

%union {
        long integer;
        char *string;
        double reel;
        }

%token TOK_NEWLINE KW_RAZ_COUPL KW_SCORE_BIP
%token KW_COPP KW_TTRONDES KW_TABTTR INVALID_KEYWORD
%token KW_SAUVEGARDE KW_SAUVIMM KW_SAUVDIF KW_BIPBIP
%token KW_IMPRESSION KW_MANUELLE KW_AUTOMATIQUE
%token KW_MINORATION KW_AFF_PIONS KW_AFF_NORMAL KW_AFF_DIFF
%token KW_FICHIER KW_JOUEURS KW_NOUVEAUX KW_INTER KW_LOG
%token KW_RESULT KW_CLASS KW_EQUIPES KW_APPARIEMENTS
%token KW_CROSSTABLE KW_ELO
%token KW_PENALITES KW_PAYS  KW_ZONE_INS
%token KW_FOIS KW_DPOINT KW_COULEUR  KW_FLOTTEMENT
%token KW_REPET KW_MCOUL KW_DESUITE KW_RONDESUIV
%token KW_CHAUVIN KW_RONDE KW_ELITISME
%token KW_BRIGHTWELL KW_BRIGHTWELL_DBL
%token KW_DOSSIER KW_XML KW_TRUE KW_FALSE

%token DIESE_NOM DIESE_RONDES DIESE_BRIGHTWELL_DBL DIESE_DATE

%token <string>  STRING COMMENT
%token <integer> INTEGER
%token <reel> DOUBLE
%token <reel> INT_ET_DEMI

%type  <integer> entier_signe opt_jech opt_plus v_pen
%type  <string>  chaines opt_chaines opt_pays opt2_pays opt_comm
%type  <reel>  nbr_reel

%%

entree: /* rien */
        | entree_config
        | entree_joueurs
        ;

entree_joueurs:    _entree_joueurs
        | newlines _entree_joueurs
        ;

_entree_joueurs:          ligne_joueur newlines
        | _entree_joueurs ligne_joueur newlines
        ;

newlines: TOK_NEWLINE
        | newlines TOK_NEWLINE;

ligne_joueur:
          indication_pays
        | description_joueur
        | ligne_invalide
        ;

indication_pays: KW_PAYS '=' STRING
            { COPIER($3, &pays_courant); }
        ;

description_joueur:
          INTEGER chaines STRING opt_pays opt_jech opt_comm
                { Nouveau_joueur($1,$2,$3,NULL,$4,$5,$6, fichier_des_nouveaux); }
        | INTEGER chaines ',' opt_chaines opt_pays opt_jech opt_comm
                { Nouveau_joueur($1,$2,$4,NULL,$5,$6,$7, fichier_des_nouveaux); }
        | INTEGER STRING opt_pays opt_jech opt_comm
                { Nouveau_joueur($1,$2,NULL,NULL,$3,$4,$5, fichier_des_nouveaux); }
        | INTEGER chaines '(' opt_chaines ')' opt_pays opt_jech opt_comm
                { Nouveau_joueur($1,$2,NULL,$4,$6,$7,$8, fichier_des_nouveaux); }
        | INTEGER '{' STRING '}' opt_comm
                { change_nationalite($1,$3); }
        ;

chaines:  STRING
        | chaines STRING        { $$ = bout_a_bout($1, $2);  }
        | chaines '+'           { $$ = bout_a_bout($1, "+"); }
        | chaines '/'           { $$ = bout_a_bout($1, "/"); }
        ;

opt_chaines: /* rien */         { $$ = ""; }
        | chaines               { $$ = $1; }
        ;

opt_jech: /* rien */            { $$ =  0; }
        | '<' entier_signe '>'  { $$ = $2; }
        ;

entier_signe: INTEGER           { $$ =  $1; }
        | '-' INTEGER           { $$ = -$2; }
        ;

opt_pays: /* rien */            { $$ = pays_courant; }
        | '{' STRING '}'        { $$ = $2; }
        ;

opt2_pays: /* rien */           { $$ = ""; }
        | STRING                { $$ = $1; }
        ;

opt_comm: /* rien */            { $$ = NULL; }
        | COMMENT               { $$ = $1;   }
        ;

nbr_reel: INTEGER               { $$ = (double) $1; }
        | INT_ET_DEMI           { $$ = $1 ; }
        | DOUBLE                { $$ = $1 ; }
        ;

entree_config:
          commande point_virgule
        | entree_config commande point_virgule
        ;

commande: /* rien */
        | KW_XML KW_TRUE
                { generer_fichiers_XML = 1;}
        | KW_XML KW_FALSE
                { generer_fichiers_XML = 0;}
        | KW_DOSSIER KW_TRUE
                { utiliser_sous_dossier = 1 ;}
        | KW_DOSSIER KW_FALSE
                { utiliser_sous_dossier = 0 ;}
        | KW_FICHIER KW_JOUEURS  '=' STRING
                { COPIER($4, &nom_fichier_joueurs); }
        | KW_FICHIER KW_NOUVEAUX '=' STRING
                { COPIER($4, &nom_fichier_nouveaux); }
        | KW_FICHIER KW_INTER    '=' STRING
                { COPIER($4, &nom_fichier_inter); }
        | KW_FICHIER KW_RESULT   '=' STRING
            { COPIER($4, &nom_fichier_result);
            sauvegarde_fichier_result = nom_fichier_result[0]? 1 : 0 ; }
        | KW_FICHIER KW_CLASS    '=' STRING
            { COPIER($4, &nom_fichier_classement);
            sauvegarde_fichier_classement = nom_fichier_classement[0]? 1 : 0 ; }
        | KW_FICHIER KW_EQUIPES  '=' STRING
            { COPIER($4, &nom_fichier_class_equipes);
            sauvegarde_fichier_class_equipes = nom_fichier_class_equipes[0]? 1 : 0 ; }
        | KW_FICHIER KW_APPARIEMENTS   '=' STRING
            { COPIER($4, &nom_fichier_appariements);
            sauvegarde_fichier_appariements = nom_fichier_appariements[0]? 1 : 0 ; }
        | KW_FICHIER KW_CROSSTABLE   '=' STRING
            { COPIER($4, &nom_fichier_crosstable_HTML);
            sauvegarde_fichier_crosstable_HTML = nom_fichier_crosstable_HTML[0]? 1 : 0 ; }
        | KW_FICHIER KW_ELO   '=' STRING
            { COPIER($4, &nom_fichier_elo);
            sauvegarde_fichier_elo = nom_fichier_elo[0]? 1 : 0 ; }
        | KW_FICHIER KW_LOG      '=' STRING
                { COPIER($4, &nom_fichier_log); }
        | KW_ZONE_INS opt2_pays  '=' INTEGER '-' INTEGER
            {
                /*
                 * zone-insertion [pays] = inf-sup;
                 */
                if ($4<0 || $4>$6) {
                    /* mauvais parametres */
                    erreur_syntaxe(ZIS_BADP);
                } else
                    nv_zone($2,$4,$6);
            }
        | indication_pays
        | KW_BRIGHTWELL '=' nbr_reel
            {
                coef_brightwell = $3;
                type_fichier_config = OLD ; /* Vieux fichier de config */
            }
        | KW_BRIGHTWELL_DBL '=' nbr_reel
            {
                coef_brightwell = ($3)/2;
                type_fichier_config = OLD ; /* Vieux fichier de config */
            }
        | KW_SCORE_BIP  '=' INTEGER
            {
                score_bip = INTEGER_TO_SCORE($3);
                if (SCORE_TOO_LARGE(score_bip))
                    erreur_syntaxe(BIP_ERROR);
                if (IS_VICTORY(score_bip))
                    avert_syntaxe(BIP_WINS);
            }
        | KW_SCORE_BIP  '=' INTEGER '/' INTEGER
            {
                score_bip = INTEGER_TO_SCORE($3);
                total_pions = $5;
                if (total_pions < 1)
                    erreur_syntaxe(BIP_TOTAL);
                else
                    nb_chiffres_des_scores = nombre_chiffres(total_pions);

                if (SCORE_TOO_LARGE(score_bip))
                    erreur_syntaxe(BIP_ERROR);
                else if (IS_VICTORY(score_bip))
                    avert_syntaxe(BIP_WINS);
            }
        | KW_COULEUR '=' '{' STRING ',' STRING '}'
            { COPIER($4, &couleur_1); COPIER($6, &couleur_2); }
        | KW_SAUVEGARDE KW_SAUVIMM
                { sauvegarde_immediate = 1; }
        | KW_SAUVEGARDE KW_SAUVDIF
                { sauvegarde_immediate = 0; }
        | KW_IMPRESSION KW_MANUELLE
            { impression_automatique = 0; }
        | KW_IMPRESSION KW_AUTOMATIQUE INTEGER
            { nb_copies_impression = $3;
            impression_automatique = nb_copies_impression? 1 : 0 ; }
        | KW_IMPRESSION KW_AUTOMATIQUE
            { nb_copies_impression = 1;
            impression_automatique = 1; }
        | KW_AFF_PIONS KW_AFF_DIFF
                { aff_diff_scores = 1; }
        | KW_AFF_PIONS KW_AFF_NORMAL
                { aff_diff_scores = 0; }
        | KW_TTRONDES '=' INTEGER '-' INTEGER KW_JOUEURS
            {
                /*
                 * toutes-rondes = inf-sup joueurs
                 */
                type_fichier_config = OLD ; /* Vieux fichier de config */
                if ($3 < 1 || $3 > $5) {
                    /* mauvais parametres */
                    erreur_syntaxe(TTR_BADP);
                } else {
                    ttr_minj = $3;
                    ttr_maxj = $5;
                }
            }
        | KW_PENALITES '{'
            {
                /*
                 * Remettre a zero toutes les penalites, avant de commencer
                 * a lire les differentes sections
                 */
                raz_toutes_penalites();
            }
        liste_sections '}'
        | commande_interne
        | ligne_invalide
        ;

liste_sections: /* rien */
        | liste_sections section
        ;

section:  section_couleur
        | section_flottement
        | section_repetition
        | section_chauvinisme
        | section_elitisme
        ;

section_couleur:     KW_COULEUR    ':' liste_pen_couleur;
section_flottement:  KW_FLOTTEMENT ':' liste_pen_flottement;
section_repetition:  KW_REPET      ':' liste_pen_repetition;
section_chauvinisme: KW_CHAUVIN    ':' liste_pen_chauvinisme;
section_elitisme:    KW_ELITISME   ':' liste_pen_elitisme;

liste_pen_couleur: /* rien */
        | liste_pen_couleur pen_couleur point_virgule
        ;

liste_pen_flottement: /* rien */
        | liste_pen_flottement pen_flottement point_virgule
        ;

liste_pen_repetition: /* rien */
        | liste_pen_repetition pen_repetition point_virgule
        ;

liste_pen_chauvinisme: /* rien */
        | liste_pen_chauvinisme pen_chauvinisme point_virgule
        ;

liste_pen_elitisme: /*rien */
    | liste_pen_elitisme pen_elitisme point_virgule
    ;

opt_plus: /* rien */    { $$ = 0; }
        | '+'           { $$ = 1; }
        ;

v_pen: INTEGER
        {
            /*
             * Renvoie un entier entre 0 et MAX_PEN
             */
            long valeur = $1;
            if (valeur < 0) {
                avert_syntaxe(PEN_NEGATIVE);
                valeur = 0;
            } else if (valeur > MAX_PEN) {
                /* LONG_MAX signifie l'infini */
                if (valeur != LONG_MAX) {
                    avert_syntaxe(PEN_INFINITE);
                }
                valeur = MAX_PEN;
            }
            $$ = valeur;
        }
        ;

pen_couleur: /* rien */
        | INTEGER opt_plus KW_FOIS '=' v_pen
            {
                i = $1;
                if (i < 1) {
                    avert_syntaxe(PENC_NULL);
                } else {
                    if (i+1 >= nmax_couleur) {
                        elargir_table(&penalite_couleur,
                                &nmax_couleur,i+2);
                    }
                    while (i < nmax_couleur) {
                                penalite_couleur[i++] = $5;
                                if ($2==0) break;
                    }
                }
            }
        | KW_DESUITE '=' v_pen
            { penalite_repcoul = $3; }
        ;

pen_flottement: /* rien */
        | INTEGER opt_plus KW_DPOINT '=' v_pen
            {
                i = $1;
                if (i < 1) {
                    avert_syntaxe(PENF_NULL);
                } else {
                    if (i+1 >= nmax_flottement) {
                        elargir_table(&penalite_flottement,
                                &nmax_flottement, i+2);
                    }
                    while (i < nmax_flottement) {
                        penalite_flottement[i++] = $5;
                        if ($2==0) break;
                    }
                }
            }
        | KW_MINORATION '=' v_pen
                { minoration_fac = $3; }
        | KW_DESUITE '=' v_pen
                { penalite_flcum = $3; }
        ;

pen_repetition: /* rien */
        | KW_MCOUL '=' v_pen
                { penalite_mcoul = $3; }
        | KW_COPP '=' v_pen
                { penalite_copp = $3; }
        | KW_BIPBIP '=' v_pen
                { penalite_bipbip = $3; }
        | KW_DESUITE '=' v_pen
                { penalite_desuite = $3; }
        ;

pen_chauvinisme: /* rien */
        | KW_RONDE INTEGER opt_plus '=' v_pen
            {
                i = $2;
                if (i < 1)
                        avert_syntaxe(PENCH_NULL);
                else if (i > NMAX_ROUNDS)
                        avert_syntaxe(PENCH_TOOFAR);
                else while (i <= NMAX_ROUNDS) {
                        penalite_chauvinisme[i-1] = $5;
                        i++;
                        if ($3 == 0) break;
                    }
            }
        ;

pen_elitisme: /* rien */
    | KW_RONDE INTEGER opt_plus '=' v_pen
       {
#ifdef ELITISM

                i = $2;
                if (i < 1)
                        avert_syntaxe(PENEL_NULL);
                else if (i > NMAX_ROUNDS)
                        avert_syntaxe(PENEL_TOOFAR);
                else while (i <= NMAX_ROUNDS) {
                        penalite_elitisme[i-1] = $5;
                        i++;
                        if ($3 == 0) break;
                    }

#else
        i = $2;
        if (i > 0) {
            puts("WARNING : penalite d'elitisme ignoree dans cette version de PAPP");
            /* lire_touche(); */
        }

#endif
       }
       ;


commande_interne:
		  DIESE_DATE '=' STRING
			{   COPIER($3, &date_tournoi); }

		| DIESE_NOM '=' STRING
            {
                COPIER($3, &nom_du_tournoi);
                type_fichier_intermediaire = CORRECT ;
            }
        | DIESE_RONDES '=' INTEGER
            {
                if( ($3<1) || ($3>NMAX_ROUNDS) )
                    erreur_syntaxe(ICMD_BAD_RND) ;
                else {
                    nombre_de_rondes = $3 ;
                    /* Si 2n-1 ou 2n rondes, on a 2n joueurs max pour un toutes-rondes */
                    ttr_maxj = ((nombre_de_rondes+1)/2)*2 ;
                    ttr_minj = 1 ;
                }
            }
        | DIESE_BRIGHTWELL_DBL '=' nbr_reel
            { coef_brightwell = ($3)/2; }
        | KW_RONDESUIV
            {
                tables_numbering();
                mettre_aj_scores();
                next_round();
            }
        | '&' liste_inscrits
            {
                absents_uncoupling();
            }
        | '(' INTEGER INTEGER ')'
            {
                long Jn=$2, Jb=$3, n1, n2;
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        /* mauvais appariement */
                        erreur_syntaxe(ICMD_BPAIR);
                else if (polarity(Jn) || polarity(Jb))
                        /* deja apparies */
                        erreur_syntaxe(ICMD_APAIR);
                else
                        make_couple(Jn, Jb, UNKNOWN_SCORE);
            }
        | '(' INTEGER INTEGER INTEGER ')'
            {
                long Jn=$2, Jb=$3, n1, n2;
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BPAIR);
                else if (polarity(Jn) || polarity(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        make_couple(Jn, Jb, INTEGER_TO_SCORE($4));
            }
        | '(' INTEGER INTEGER INT_ET_DEMI ')'
            {
#ifndef USE_HALF_DISCS
            beep();
            erreur_syntaxe(ICMD_DEMI);
#else
                long Jn=$2, Jb=$3, n1, n2;
                double Scn=$4;
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BPAIR);
                else if (polarity(Jn) || polarity(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        make_couple(Jn, Jb, FLOAT_TO_SCORE(Scn));
#endif
            }
        | '(' INTEGER INTEGER INTEGER INTEGER ')'
            {
                long Jn=$2, Scn=$3, Jb=$4, Scb=$5, n1, n2;
                if (BAD_TOTAL(INTEGER_TO_SCORE(Scn),INTEGER_TO_SCORE(Scb)))
                        erreur_syntaxe(ICMD_TOTS);
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BRES);
                else if (polarity(Jn) || polarity(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        make_couple(Jn, Jb, INTEGER_TO_SCORE(Scn));
            }
        | '(' INTEGER INT_ET_DEMI INTEGER INT_ET_DEMI ')'
            {
#ifndef USE_HALF_DISCS
            beep();
            erreur_syntaxe(ICMD_DEMI);
#else
        long Jn=$2, Jb=$4, n1, n2;
                double Scn=$3, Scb=$5;
                if (BAD_TOTAL(FLOAT_TO_SCORE(Scn),FLOAT_TO_SCORE(Scb)))
                        erreur_syntaxe(ICMD_TOTS);
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BRES);
                else if (polarity(Jn) || polarity(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        make_couple(Jn, Jb, FLOAT_TO_SCORE(Scn));
#endif
            }
        | KW_RAZ_COUPL
            { zero_coupling(); }
        | KW_TABTTR '[' INTEGER ']'
            {
                i = 0; taille_ttr = $3;
                CALLOC(table_ttr, taille_ttr, long);
            }
        '=' '{' liste_tabttr '}'
            {
                assert(table_ttr);
                if (i != taille_ttr)
                        erreur_syntaxe(TTR_TOOSMALL);
                else
                        charge_table_ttr(taille_ttr, table_ttr);
                free(table_ttr);
            }
        ;

liste_tabttr:   /* rien */
        | liste_tabttr entier_signe
            {
                if (i >= taille_ttr)
                        erreur_syntaxe(TTR_TOOBIG);
                else
                        table_ttr[i++] = $2;
            }
        ;

liste_inscrits: /* rien */
        | liste_inscrits '+' INTEGER
            {
                long n = inscrire_joueur($3);
                if (n < 0) {
                        sprintf(buffer, PLR_UNKNOWN, $3);
                        erreur_syntaxe(buffer);
                } else
                        present[n] = 1;
            }
        | liste_inscrits '-' INTEGER
            {
                long n = inscrire_joueur($3);
                if (n < 0) {
                        sprintf(buffer, PLR_UNKNOWN, $3);
                        erreur_syntaxe(buffer);
                } else
                        present[n] = 0;
            }
        ;

ligne_invalide: INVALID_KEYWORD error
            { erreur_syntaxe(CMD_UNKNOWN); }
        | error
            { erreur_syntaxe(ERR_SYNTAX); }
        ;

point_virgule: ';'
        | error
            { erreur_syntaxe(M_SEMICOLON); }
        ;
