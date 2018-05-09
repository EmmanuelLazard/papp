/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_NEWLINE = 258,
     KW_RAZ_COUPL = 259,
     KW_SCORE_BIP = 260,
     KW_COPP = 261,
     KW_TTRONDES = 262,
     KW_TABTTR = 263,
     INVALID_KEYWORD = 264,
     KW_SAUVEGARDE = 265,
     KW_SAUVIMM = 266,
     KW_SAUVDIF = 267,
     KW_BIPBIP = 268,
     KW_IMPRESSION = 269,
     KW_MANUELLE = 270,
     KW_AUTOMATIQUE = 271,
     KW_MINORATION = 272,
     KW_AFF_PIONS = 273,
     KW_AFF_NORMAL = 274,
     KW_AFF_DIFF = 275,
     KW_FICHIER = 276,
     KW_JOUEURS = 277,
     KW_NOUVEAUX = 278,
     KW_INTER = 279,
     KW_LOG = 280,
     KW_RESULT = 281,
     KW_CLASS = 282,
     KW_EQUIPES = 283,
     KW_APPARIEMENTS = 284,
     KW_CROSSTABLE = 285,
     KW_ELO = 286,
     KW_PENALITES = 287,
     KW_PAYS = 288,
     KW_ZONE_INS = 289,
     KW_FOIS = 290,
     KW_DPOINT = 291,
     KW_COULEUR = 292,
     KW_FLOTTEMENT = 293,
     KW_REPET = 294,
     KW_MCOUL = 295,
     KW_DESUITE = 296,
     KW_RONDESUIV = 297,
     KW_CHAUVIN = 298,
     KW_RONDE = 299,
     KW_ELITISME = 300,
     KW_BRIGHTWELL = 301,
     KW_BRIGHTWELL_DBL = 302,
     KW_DOSSIER = 303,
     KW_XML = 304,
     KW_TRUE = 305,
     KW_FALSE = 306,
     DIESE_NOM = 307,
     DIESE_RONDES = 308,
     DIESE_BRIGHTWELL_DBL = 309,
     DIESE_DATE = 310,
     STRING = 311,
     COMMENT = 312,
     INTEGER = 313,
     DOUBLE = 314,
     INT_ET_DEMI = 315
   };
#endif
/* Tokens.  */
#define TOK_NEWLINE 258
#define KW_RAZ_COUPL 259
#define KW_SCORE_BIP 260
#define KW_COPP 261
#define KW_TTRONDES 262
#define KW_TABTTR 263
#define INVALID_KEYWORD 264
#define KW_SAUVEGARDE 265
#define KW_SAUVIMM 266
#define KW_SAUVDIF 267
#define KW_BIPBIP 268
#define KW_IMPRESSION 269
#define KW_MANUELLE 270
#define KW_AUTOMATIQUE 271
#define KW_MINORATION 272
#define KW_AFF_PIONS 273
#define KW_AFF_NORMAL 274
#define KW_AFF_DIFF 275
#define KW_FICHIER 276
#define KW_JOUEURS 277
#define KW_NOUVEAUX 278
#define KW_INTER 279
#define KW_LOG 280
#define KW_RESULT 281
#define KW_CLASS 282
#define KW_EQUIPES 283
#define KW_APPARIEMENTS 284
#define KW_CROSSTABLE 285
#define KW_ELO 286
#define KW_PENALITES 287
#define KW_PAYS 288
#define KW_ZONE_INS 289
#define KW_FOIS 290
#define KW_DPOINT 291
#define KW_COULEUR 292
#define KW_FLOTTEMENT 293
#define KW_REPET 294
#define KW_MCOUL 295
#define KW_DESUITE 296
#define KW_RONDESUIV 297
#define KW_CHAUVIN 298
#define KW_RONDE 299
#define KW_ELITISME 300
#define KW_BRIGHTWELL 301
#define KW_BRIGHTWELL_DBL 302
#define KW_DOSSIER 303
#define KW_XML 304
#define KW_TRUE 305
#define KW_FALSE 306
#define DIESE_NOM 307
#define DIESE_RONDES 308
#define DIESE_BRIGHTWELL_DBL 309
#define DIESE_DATE 310
#define STRING 311
#define COMMENT 312
#define INTEGER 313
#define DOUBLE 314
#define INT_ET_DEMI 315




/* Copy the first part of user declarations.  */
#line 1 "pap.y"

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

static void Nouveau_joueur (long numero, const char *nom, const char *prenom,
        const char *programmeur, const char *pays, long classement,
        const char *commentaire, const long nv) {
    if (!numero) {
        char *s = new_string();
        sprintf(s, NUM_ZERO, nom);  /* numero 0 interdit */
        erreur_syntaxe(s);
    }
    if (nv_joueur(numero, nom, prenom, programmeur, pays, classement, commentaire, nv) == NULL) {
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
    for (i = 0; i < NMAX_RONDES; i++)
        penalite_chauvinisme[i] = 0;

    /* Penalites d'elitisme */
#ifdef ELITISME
    for (i = 0; i < NMAX_RONDES; i++)
        penalite_elitisme[i] = 0;
#endif
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 194 "pap.y"
{
        long integer;
        char *string;
        double reel;
        }
/* Line 193 of yacc.c.  */
#line 415 "pap.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 428 "pap.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  79
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   308

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNRULES -- Number of states.  */
#define YYNSTATES  278

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,     2,
      63,    64,     2,    67,    62,    71,     2,    68,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    72,    76,
      69,    61,    70,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    74,     2,    75,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,     2,    66,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    10,    13,    16,    20,
      22,    25,    27,    29,    31,    35,    42,    50,    56,    65,
      71,    73,    76,    79,    82,    83,    85,    86,    90,    92,
      95,    96,   100,   101,   103,   104,   106,   108,   110,   112,
     115,   119,   120,   123,   126,   129,   132,   137,   142,   147,
     152,   157,   162,   167,   172,   177,   182,   189,   191,   195,
     199,   203,   209,   217,   220,   223,   226,   230,   233,   236,
     239,   246,   247,   253,   255,   257,   258,   261,   263,   265,
     267,   269,   271,   275,   279,   283,   287,   291,   292,   296,
     297,   301,   302,   306,   307,   311,   312,   316,   317,   319,
     321,   322,   328,   332,   333,   339,   343,   347,   348,   352,
     356,   360,   364,   365,   371,   372,   378,   382,   386,   390,
     394,   396,   399,   404,   410,   416,   423,   430,   432,   433,
     443,   444,   447,   448,   452,   456,   459,   461,   463
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      78,     0,    -1,    -1,    93,    -1,    79,    -1,    80,    -1,
      81,    80,    -1,    82,    81,    -1,    80,    82,    81,    -1,
       3,    -1,    81,     3,    -1,    83,    -1,    84,    -1,   119,
      -1,    33,    61,    56,    -1,    58,    85,    56,    89,    87,
      91,    -1,    58,    85,    62,    86,    89,    87,    91,    -1,
      58,    56,    89,    87,    91,    -1,    58,    85,    63,    86,
      64,    89,    87,    91,    -1,    58,    65,    56,    66,    91,
      -1,    56,    -1,    85,    56,    -1,    85,    67,    -1,    85,
      68,    -1,    -1,    85,    -1,    -1,    69,    88,    70,    -1,
      58,    -1,    71,    58,    -1,    -1,    65,    56,    66,    -1,
      -1,    56,    -1,    -1,    57,    -1,    58,    -1,    60,    -1,
      59,    -1,    94,   120,    -1,    93,    94,   120,    -1,    -1,
      49,    50,    -1,    49,    51,    -1,    48,    50,    -1,    48,
      51,    -1,    21,    22,    61,    56,    -1,    21,    23,    61,
      56,    -1,    21,    24,    61,    56,    -1,    21,    26,    61,
      56,    -1,    21,    27,    61,    56,    -1,    21,    28,    61,
      56,    -1,    21,    29,    61,    56,    -1,    21,    30,    61,
      56,    -1,    21,    31,    61,    56,    -1,    21,    25,    61,
      56,    -1,    34,    90,    61,    58,    71,    58,    -1,    83,
      -1,    46,    61,    92,    -1,    47,    61,    92,    -1,     5,
      61,    58,    -1,     5,    61,    58,    68,    58,    -1,    37,
      61,    65,    56,    62,    56,    66,    -1,    10,    11,    -1,
      10,    12,    -1,    14,    15,    -1,    14,    16,    58,    -1,
      14,    16,    -1,    18,    20,    -1,    18,    19,    -1,     7,
      61,    58,    71,    58,    22,    -1,    -1,    32,    65,    95,
      96,    66,    -1,   115,    -1,   119,    -1,    -1,    96,    97,
      -1,    98,    -1,    99,    -1,   100,    -1,   101,    -1,   102,
      -1,    37,    72,   103,    -1,    38,    72,   104,    -1,    39,
      72,   105,    -1,    43,    72,   106,    -1,    45,    72,   107,
      -1,    -1,   103,   110,   120,    -1,    -1,   104,   111,   120,
      -1,    -1,   105,   112,   120,    -1,    -1,   106,   113,   120,
      -1,    -1,   107,   114,   120,    -1,    -1,    67,    -1,    58,
      -1,    -1,    58,   108,    35,    61,   109,    -1,    41,    61,
     109,    -1,    -1,    58,   108,    36,    61,   109,    -1,    17,
      61,   109,    -1,    41,    61,   109,    -1,    -1,    40,    61,
     109,    -1,     6,    61,   109,    -1,    13,    61,   109,    -1,
      41,    61,   109,    -1,    -1,    44,    58,   108,    61,   109,
      -1,    -1,    44,    58,   108,    61,   109,    -1,    55,    61,
      56,    -1,    52,    61,    56,    -1,    53,    61,    58,    -1,
      54,    61,    92,    -1,    42,    -1,    73,   118,    -1,    63,
      58,    58,    64,    -1,    63,    58,    58,    58,    64,    -1,
      63,    58,    58,    60,    64,    -1,    63,    58,    58,    58,
      58,    64,    -1,    63,    58,    60,    58,    60,    64,    -1,
       4,    -1,    -1,     8,    74,    58,    75,   116,    61,    65,
     117,    66,    -1,    -1,   117,    88,    -1,    -1,   118,    67,
      58,    -1,   118,    71,    58,    -1,     9,     1,    -1,     1,
      -1,    76,    -1,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   228,   228,   229,   230,   233,   234,   237,   238,   241,
     242,   245,   246,   247,   250,   255,   257,   259,   261,   263,
     267,   268,   269,   270,   273,   274,   277,   278,   281,   282,
     285,   286,   289,   290,   293,   294,   297,   298,   299,   303,
     304,   307,   308,   310,   312,   314,   316,   318,   320,   322,
     325,   328,   331,   334,   337,   340,   342,   353,   354,   359,
     364,   372,   386,   388,   390,   392,   394,   397,   400,   402,
     404,   419,   418,   427,   428,   431,   432,   435,   436,   437,
     438,   439,   442,   443,   444,   445,   446,   448,   449,   452,
     453,   456,   457,   460,   461,   464,   465,   468,   469,   472,
     492,   493,   509,   513,   514,   530,   532,   536,   537,   539,
     541,   543,   547,   548,   563,   564,   592,   595,   600,   611,
     613,   619,   623,   637,   649,   667,   681,   701,   704,   703,
     719,   720,   729,   730,   739,   750,   752,   756,   757
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_NEWLINE", "KW_RAZ_COUPL",
  "KW_SCORE_BIP", "KW_COPP", "KW_TTRONDES", "KW_TABTTR", "INVALID_KEYWORD",
  "KW_SAUVEGARDE", "KW_SAUVIMM", "KW_SAUVDIF", "KW_BIPBIP",
  "KW_IMPRESSION", "KW_MANUELLE", "KW_AUTOMATIQUE", "KW_MINORATION",
  "KW_AFF_PIONS", "KW_AFF_NORMAL", "KW_AFF_DIFF", "KW_FICHIER",
  "KW_JOUEURS", "KW_NOUVEAUX", "KW_INTER", "KW_LOG", "KW_RESULT",
  "KW_CLASS", "KW_EQUIPES", "KW_APPARIEMENTS", "KW_CROSSTABLE", "KW_ELO",
  "KW_PENALITES", "KW_PAYS", "KW_ZONE_INS", "KW_FOIS", "KW_DPOINT",
  "KW_COULEUR", "KW_FLOTTEMENT", "KW_REPET", "KW_MCOUL", "KW_DESUITE",
  "KW_RONDESUIV", "KW_CHAUVIN", "KW_RONDE", "KW_ELITISME", "KW_BRIGHTWELL",
  "KW_BRIGHTWELL_DBL", "KW_DOSSIER", "KW_XML", "KW_TRUE", "KW_FALSE",
  "DIESE_NOM", "DIESE_RONDES", "DIESE_BRIGHTWELL_DBL", "DIESE_DATE",
  "STRING", "COMMENT", "INTEGER", "DOUBLE", "INT_ET_DEMI", "'='", "','",
  "'('", "')'", "'{'", "'}'", "'+'", "'/'", "'<'", "'>'", "'-'", "':'",
  "'&'", "'['", "']'", "';'", "$accept", "entree", "entree_joueurs",
  "_entree_joueurs", "newlines", "ligne_joueur", "indication_pays",
  "description_joueur", "chaines", "opt_chaines", "opt_jech",
  "entier_signe", "opt_pays", "opt2_pays", "opt_comm", "nbr_reel",
  "entree_config", "commande", "@1", "liste_sections", "section",
  "section_couleur", "section_flottement", "section_repetition",
  "section_chauvinisme", "section_elitisme", "liste_pen_couleur",
  "liste_pen_flottement", "liste_pen_repetition", "liste_pen_chauvinisme",
  "liste_pen_elitisme", "opt_plus", "v_pen", "pen_couleur",
  "pen_flottement", "pen_repetition", "pen_chauvinisme", "pen_elitisme",
  "commande_interne", "@2", "liste_tabttr", "liste_inscrits",
  "ligne_invalide", "point_virgule", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,    61,    44,    40,    41,   123,   125,    43,    47,    60,
      62,    45,    58,    38,    91,    93,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    77,    78,    78,    78,    79,    79,    80,    80,    81,
      81,    82,    82,    82,    83,    84,    84,    84,    84,    84,
      85,    85,    85,    85,    86,    86,    87,    87,    88,    88,
      89,    89,    90,    90,    91,    91,    92,    92,    92,    93,
      93,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    95,    94,    94,    94,    96,    96,    97,    97,    97,
      97,    97,    98,    99,   100,   101,   102,   103,   103,   104,
     104,   105,   105,   106,   106,   107,   107,   108,   108,   109,
     110,   110,   110,   111,   111,   111,   111,   112,   112,   112,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   116,   115,
     117,   117,   118,   118,   118,   119,   119,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     1,     2,     2,     3,     1,
       2,     1,     1,     1,     3,     6,     7,     5,     8,     5,
       1,     2,     2,     2,     0,     1,     0,     3,     1,     2,
       0,     3,     0,     1,     0,     1,     1,     1,     1,     2,
       3,     0,     2,     2,     2,     2,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     6,     1,     3,     3,
       3,     5,     7,     2,     2,     2,     3,     2,     2,     2,
       6,     0,     5,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     1,     1,
       0,     5,     3,     0,     5,     3,     3,     0,     3,     3,
       3,     3,     0,     5,     0,     5,     3,     3,     3,     3,
       1,     2,     4,     5,     5,     6,     6,     1,     0,     9,
       0,     2,     0,     3,     3,     2,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   136,     9,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,     0,     4,
       0,     0,     0,    57,    12,     0,     0,    73,    74,     0,
       0,     0,   135,    63,    64,    65,    67,    69,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    33,     0,     0,     0,     0,    44,    45,    42,    43,
       0,     0,     0,     0,    20,     0,     0,     0,   121,     1,
       0,    11,    13,    10,     0,     7,    57,     0,    74,   138,
     137,    39,    60,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    14,     0,     0,
      36,    38,    37,    58,    59,   117,   118,   119,   116,     0,
      26,     0,    21,    24,    24,    22,    23,     0,     0,     0,
       0,     8,    40,     0,     0,   128,    46,    47,    48,    55,
      49,    50,    51,    52,    53,    54,     0,     0,     0,     0,
       0,    34,    34,    26,    20,    25,    30,     0,     0,     0,
     122,     0,   133,   134,    61,     0,     0,     0,     0,     0,
       0,     0,    72,    76,    77,    78,    79,    80,    81,     0,
       0,    31,    28,     0,     0,    35,    17,    19,    34,    21,
      26,    30,     0,   123,   124,     0,    70,     0,    87,    89,
      91,    93,    95,    56,     0,    29,    27,    15,    34,    26,
     125,   126,   130,    82,    83,    84,    85,    86,    62,    16,
      34,     0,     0,    97,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,   129,
     131,     0,    98,     0,    88,     0,     0,     0,    90,     0,
       0,     0,     0,    92,    97,    94,    97,    96,    99,   102,
       0,   105,   106,     0,   109,   110,   108,   111,     0,     0,
       0,     0,     0,     0,   101,   104,   113,   115
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,    31,    32,    81,    34,   155,   156,
     151,   184,   120,    62,   186,   113,    35,    36,   106,   146,
     173,   174,   175,   176,   177,   178,   213,   214,   215,   216,
     217,   243,   259,   224,   228,   233,   235,   237,    37,   166,
     221,    78,    82,    91
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -131
static const yytype_int16 yypact[] =
{
       1,  -131,  -131,  -131,   -45,   -40,    31,   120,   145,   158,
     159,   169,    69,    75,    93,   104,  -131,   114,   124,   130,
     133,   125,   140,   141,   142,    41,   146,  -131,   205,  -131,
      28,   119,   203,   204,  -131,    62,    12,  -131,   206,   150,
     152,   153,  -131,  -131,  -131,  -131,   154,  -131,  -131,   155,
     156,   157,   160,   161,   162,   163,   164,   165,   166,  -131,
     172,  -131,   168,   148,   112,   112,  -131,  -131,  -131,  -131,
     174,   173,   112,   176,    33,   177,    77,    71,    76,  -131,
     203,  -131,  -131,  -131,   118,   211,  -131,    12,  -131,  -131,
    -131,  -131,   147,   149,   144,  -131,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,  -131,  -131,   188,   189,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,   191,
     175,   190,    33,   192,   192,  -131,  -131,   100,   193,   194,
     195,   211,  -131,   196,   197,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,   116,   198,   199,   200,
     -31,   201,   201,   175,  -131,    56,   202,   207,   -32,   208,
    -131,   210,  -131,  -131,  -131,   227,   212,   209,   213,   214,
     215,   216,  -131,  -131,  -131,  -131,  -131,  -131,  -131,   217,
     218,  -131,  -131,   219,   220,  -131,  -131,  -131,   201,  -131,
     175,   202,   225,  -131,  -131,   228,  -131,   226,  -131,  -131,
    -131,  -131,  -131,  -131,   229,  -131,  -131,  -131,   201,   175,
    -131,  -131,  -131,    17,    24,    11,     2,    13,  -131,  -131,
     201,   -27,   221,   230,    12,   222,   223,   230,    12,   232,
     233,   235,   237,    12,   241,    12,   242,    12,  -131,  -131,
    -131,   243,  -131,   224,  -131,   243,   243,   240,  -131,   243,
     243,   243,   243,  -131,   230,  -131,   230,  -131,  -131,  -131,
     244,  -131,  -131,   245,  -131,  -131,  -131,  -131,   246,   247,
     243,   243,   243,   243,  -131,  -131,  -131,  -131
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -131,  -131,  -131,   231,   -12,     0,     7,  -131,   238,   126,
    -130,    36,   -49,  -131,  -107,    20,  -131,   267,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -124,   -83,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,    91,   -87
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -115
static const yytype_int16 yytable[] =
{
     132,    -2,     1,  -112,     2,     3,     4,    33,     5,     6,
       7,     8,  -107,    89,  -114,     9,    39,   229,  -100,    10,
      85,    40,    11,   188,   230,  -103,   192,   182,    -5,     1,
      80,   182,   193,    12,    13,    14,   -30,     7,    15,   239,
     183,   225,    86,    16,   183,   187,   234,    17,    18,    19,
      20,   231,   232,    21,    22,    23,    24,   236,   222,    25,
     208,    13,    -3,     1,    26,   226,     3,     4,   131,     5,
       6,     7,     8,   153,    27,   223,     9,   -41,  -112,   220,
      10,   207,   227,    11,    80,   114,    25,  -107,    90,  -114,
     -30,    38,   117,  -100,    12,    13,    14,    74,   119,    15,
    -103,   219,   -30,   247,    16,    41,    75,   190,    17,    18,
      19,    20,   189,   238,    21,    22,    23,    24,    -6,     1,
       1,    42,    83,   125,   126,    26,    88,     7,     7,   127,
     268,   128,   269,   122,    59,    27,    60,   244,   -41,   123,
     124,   248,   209,   129,   125,   126,   253,   130,   255,    61,
     257,    13,    13,   167,   168,   169,    43,    44,   158,   170,
     159,   171,   261,   262,   160,    63,   264,   265,   266,   267,
     110,   111,   112,    45,    46,    64,    25,    25,    47,    48,
      66,    67,   172,    68,    69,    65,    70,   274,   275,   276,
     277,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    71,    72,    73,    77,    79,     2,   -11,    92,   -13,
      93,    94,    95,   109,    83,   133,    96,    97,    98,   135,
     134,    99,   100,   101,   102,   103,   104,   105,   107,   108,
     115,   116,   118,   121,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   150,   148,   147,   149,   154,   196,
     157,   161,   162,   163,   164,   165,   152,   240,   185,   260,
       0,   180,    84,    76,     0,     0,   181,   119,     0,   179,
     195,   191,   194,   197,   204,   203,   263,   205,     0,     0,
       0,   198,   241,   245,   246,   199,   200,   201,   202,   210,
     206,   212,   211,   249,   250,   218,   251,   242,   252,   254,
     256,   258,    87,     0,     0,   270,   271,   272,   273
};

static const yytype_int16 yycheck[] =
{
      87,     0,     1,     1,     3,     4,     5,     0,     7,     8,
       9,    10,     1,     1,     1,    14,    61,     6,     1,    18,
      32,    61,    21,   153,    13,     1,    58,    58,     0,     1,
      30,    58,    64,    32,    33,    34,     3,     9,    37,    66,
      71,    17,    35,    42,    71,   152,    44,    46,    47,    48,
      49,    40,    41,    52,    53,    54,    55,    44,    41,    58,
     190,    33,     0,     1,    63,    41,     4,     5,    80,     7,
       8,     9,    10,   122,    73,    58,    14,    76,    76,   209,
      18,   188,    58,    21,    84,    65,    58,    76,    76,    76,
      57,     0,    72,    76,    32,    33,    34,    56,    65,    37,
      76,   208,    69,   227,    42,    74,    65,   156,    46,    47,
      48,    49,    56,   220,    52,    53,    54,    55,     0,     1,
       1,     1,     3,    67,    68,    63,    35,     9,     9,    58,
     254,    60,   256,    56,    65,    73,    61,   224,    76,    62,
      63,   228,   191,    67,    67,    68,   233,    71,   235,    56,
     237,    33,    33,    37,    38,    39,    11,    12,    58,    43,
      60,    45,   245,   246,    64,    61,   249,   250,   251,   252,
      58,    59,    60,    15,    16,    61,    58,    58,    19,    20,
      50,    51,    66,    50,    51,    61,    61,   270,   271,   272,
     273,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    61,    61,    61,    58,     0,     3,     3,    58,     3,
      58,    58,    58,    65,     3,    68,    61,    61,    61,    75,
      71,    61,    61,    61,    61,    61,    61,    61,    56,    61,
      56,    58,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    69,    56,    58,    56,    56,    22,
     124,    58,    58,    58,    58,    58,    66,   221,    57,    35,
      -1,    62,    31,    25,    -1,    -1,    66,    65,    -1,    71,
      60,    64,    64,    61,    56,    58,    36,    58,    -1,    -1,
      -1,    72,    61,    61,    61,    72,    72,    72,    72,    64,
      70,    65,    64,    61,    61,    66,    61,    67,    61,    58,
      58,    58,    35,    -1,    -1,    61,    61,    61,    61
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     7,     8,     9,    10,    14,
      18,    21,    32,    33,    34,    37,    42,    46,    47,    48,
      49,    52,    53,    54,    55,    58,    63,    73,    78,    79,
      80,    81,    82,    83,    84,    93,    94,   115,   119,    61,
      61,    74,     1,    11,    12,    15,    16,    19,    20,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    65,
      61,    56,    90,    61,    61,    61,    50,    51,    50,    51,
      61,    61,    61,    61,    56,    65,    85,    58,   118,     0,
      82,    83,   119,     3,    80,    81,    83,    94,   119,     1,
      76,   120,    58,    58,    58,    58,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    95,    56,    61,    65,
      58,    59,    60,    92,    92,    56,    58,    92,    56,    65,
      89,    56,    56,    62,    63,    67,    68,    58,    60,    67,
      71,    81,   120,    68,    71,    75,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    96,    58,    56,    56,
      69,    87,    66,    89,    56,    85,    86,    86,    58,    60,
      64,    58,    58,    58,    58,    58,   116,    37,    38,    39,
      43,    45,    66,    97,    98,    99,   100,   101,   102,    71,
      62,    66,    58,    71,    88,    57,    91,    91,    87,    56,
      89,    64,    58,    64,    64,    60,    22,    61,    72,    72,
      72,    72,    72,    58,    56,    58,    70,    91,    87,    89,
      64,    64,    65,   103,   104,   105,   106,   107,    66,    91,
      87,   117,    41,    58,   110,    17,    41,    58,   111,     6,
      13,    40,    41,   112,    44,   113,    44,   114,    91,    66,
      88,    61,    67,   108,   120,    61,    61,   108,   120,    61,
      61,    61,    61,   120,    58,   120,    58,   120,    58,   109,
      35,   109,   109,    36,   109,   109,   109,   109,   108,   108,
      61,    61,    61,    61,   109,   109,   109,   109
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 14:
#line 251 "pap.y"
    { COPIER((yyvsp[(3) - (3)].string), &pays_courant); ;}
    break;

  case 15:
#line 256 "pap.y"
    { Nouveau_joueur((yyvsp[(1) - (6)].integer),(yyvsp[(2) - (6)].string),(yyvsp[(3) - (6)].string),NULL,(yyvsp[(4) - (6)].string),(yyvsp[(5) - (6)].integer),(yyvsp[(6) - (6)].string), fichier_des_nouveaux); ;}
    break;

  case 16:
#line 258 "pap.y"
    { Nouveau_joueur((yyvsp[(1) - (7)].integer),(yyvsp[(2) - (7)].string),(yyvsp[(4) - (7)].string),NULL,(yyvsp[(5) - (7)].string),(yyvsp[(6) - (7)].integer),(yyvsp[(7) - (7)].string), fichier_des_nouveaux); ;}
    break;

  case 17:
#line 260 "pap.y"
    { Nouveau_joueur((yyvsp[(1) - (5)].integer),(yyvsp[(2) - (5)].string),NULL,NULL,(yyvsp[(3) - (5)].string),(yyvsp[(4) - (5)].integer),(yyvsp[(5) - (5)].string), fichier_des_nouveaux); ;}
    break;

  case 18:
#line 262 "pap.y"
    { Nouveau_joueur((yyvsp[(1) - (8)].integer),(yyvsp[(2) - (8)].string),NULL,(yyvsp[(4) - (8)].string),(yyvsp[(6) - (8)].string),(yyvsp[(7) - (8)].integer),(yyvsp[(8) - (8)].string), fichier_des_nouveaux); ;}
    break;

  case 19:
#line 264 "pap.y"
    { change_nationalite((yyvsp[(1) - (5)].integer),(yyvsp[(3) - (5)].string)); ;}
    break;

  case 21:
#line 268 "pap.y"
    { (yyval.string) = bout_a_bout((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string));  ;}
    break;

  case 22:
#line 269 "pap.y"
    { (yyval.string) = bout_a_bout((yyvsp[(1) - (2)].string), "+"); ;}
    break;

  case 23:
#line 270 "pap.y"
    { (yyval.string) = bout_a_bout((yyvsp[(1) - (2)].string), "/"); ;}
    break;

  case 24:
#line 273 "pap.y"
    { (yyval.string) = ""; ;}
    break;

  case 25:
#line 274 "pap.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 26:
#line 277 "pap.y"
    { (yyval.integer) =  0; ;}
    break;

  case 27:
#line 278 "pap.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 28:
#line 281 "pap.y"
    { (yyval.integer) =  (yyvsp[(1) - (1)].integer); ;}
    break;

  case 29:
#line 282 "pap.y"
    { (yyval.integer) = -(yyvsp[(2) - (2)].integer); ;}
    break;

  case 30:
#line 285 "pap.y"
    { (yyval.string) = pays_courant; ;}
    break;

  case 31:
#line 286 "pap.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 32:
#line 289 "pap.y"
    { (yyval.string) = ""; ;}
    break;

  case 33:
#line 290 "pap.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 34:
#line 293 "pap.y"
    { (yyval.string) = NULL; ;}
    break;

  case 35:
#line 294 "pap.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string);   ;}
    break;

  case 36:
#line 297 "pap.y"
    { (yyval.reel) = (double) (yyvsp[(1) - (1)].integer); ;}
    break;

  case 37:
#line 298 "pap.y"
    { (yyval.reel) = (yyvsp[(1) - (1)].reel) ; ;}
    break;

  case 38:
#line 299 "pap.y"
    { (yyval.reel) = (yyvsp[(1) - (1)].reel) ; ;}
    break;

  case 42:
#line 309 "pap.y"
    { generer_fichiers_XML = 1;;}
    break;

  case 43:
#line 311 "pap.y"
    { generer_fichiers_XML = 0;;}
    break;

  case 44:
#line 313 "pap.y"
    { utiliser_sous_dossier = 1 ;;}
    break;

  case 45:
#line 315 "pap.y"
    { utiliser_sous_dossier = 0 ;;}
    break;

  case 46:
#line 317 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_joueurs); ;}
    break;

  case 47:
#line 319 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_nouveaux); ;}
    break;

  case 48:
#line 321 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_inter); ;}
    break;

  case 49:
#line 323 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_result);
            sauvegarde_fichier_result = nom_fichier_result[0]? 1 : 0 ; ;}
    break;

  case 50:
#line 326 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_classement);
            sauvegarde_fichier_classement = nom_fichier_classement[0]? 1 : 0 ; ;}
    break;

  case 51:
#line 329 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_class_equipes);
            sauvegarde_fichier_class_equipes = nom_fichier_class_equipes[0]? 1 : 0 ; ;}
    break;

  case 52:
#line 332 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_appariements);
            sauvegarde_fichier_appariements = nom_fichier_appariements[0]? 1 : 0 ; ;}
    break;

  case 53:
#line 335 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_crosstable_HTML);
            sauvegarde_fichier_crosstable_HTML = nom_fichier_crosstable_HTML[0]? 1 : 0 ; ;}
    break;

  case 54:
#line 338 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_elo);
            sauvegarde_fichier_elo = nom_fichier_elo[0]? 1 : 0 ; ;}
    break;

  case 55:
#line 341 "pap.y"
    { COPIER((yyvsp[(4) - (4)].string), &nom_fichier_log); ;}
    break;

  case 56:
#line 343 "pap.y"
    {
                /*
                 * zone-insertion [pays] = inf-sup;
                 */
                if ((yyvsp[(4) - (6)].integer)<0 || (yyvsp[(4) - (6)].integer)>(yyvsp[(6) - (6)].integer)) {
                    /* mauvais parametres */
                    erreur_syntaxe(ZIS_BADP);
                } else
                    nv_zone((yyvsp[(2) - (6)].string),(yyvsp[(4) - (6)].integer),(yyvsp[(6) - (6)].integer));
            ;}
    break;

  case 58:
#line 355 "pap.y"
    {
                coef_brightwell = (yyvsp[(3) - (3)].reel);
                type_fichier_config = OLD ; /* Vieux fichier de config */
            ;}
    break;

  case 59:
#line 360 "pap.y"
    {
                coef_brightwell = ((yyvsp[(3) - (3)].reel))/2;
                type_fichier_config = OLD ; /* Vieux fichier de config */
            ;}
    break;

  case 60:
#line 365 "pap.y"
    {
                score_bip = ENTIER_EN_SCORE((yyvsp[(3) - (3)].integer));
                if (SCORE_TROP_GRAND(score_bip))
                    erreur_syntaxe(BIP_ERROR);
                if (EST_UNE_VICTOIRE(score_bip))
                    avert_syntaxe(BIP_WINS);
            ;}
    break;

  case 61:
#line 373 "pap.y"
    {
                score_bip = ENTIER_EN_SCORE((yyvsp[(3) - (5)].integer));
                total_pions = (yyvsp[(5) - (5)].integer);
                if (total_pions < 1)
                    erreur_syntaxe(BIP_TOTAL);
                else
                    nb_chiffres_des_scores = nombre_chiffres(total_pions);

                if (SCORE_TROP_GRAND(score_bip))
                    erreur_syntaxe(BIP_ERROR);
                else if (EST_UNE_VICTOIRE(score_bip))
                    avert_syntaxe(BIP_WINS);
            ;}
    break;

  case 62:
#line 387 "pap.y"
    { COPIER((yyvsp[(4) - (7)].string), &couleur_1); COPIER((yyvsp[(6) - (7)].string), &couleur_2); ;}
    break;

  case 63:
#line 389 "pap.y"
    { sauvegarde_immediate = 1; ;}
    break;

  case 64:
#line 391 "pap.y"
    { sauvegarde_immediate = 0; ;}
    break;

  case 65:
#line 393 "pap.y"
    { impression_automatique = 0; ;}
    break;

  case 66:
#line 395 "pap.y"
    { nb_copies_impression = (yyvsp[(3) - (3)].integer);
            impression_automatique = nb_copies_impression? 1 : 0 ; ;}
    break;

  case 67:
#line 398 "pap.y"
    { nb_copies_impression = 1;
            impression_automatique = 1; ;}
    break;

  case 68:
#line 401 "pap.y"
    { aff_diff_scores = 1; ;}
    break;

  case 69:
#line 403 "pap.y"
    { aff_diff_scores = 0; ;}
    break;

  case 70:
#line 405 "pap.y"
    {
                /*
                 * toutes-rondes = inf-sup joueurs
                 */
                type_fichier_config = OLD ; /* Vieux fichier de config */
                if ((yyvsp[(3) - (6)].integer) < 1 || (yyvsp[(3) - (6)].integer) > (yyvsp[(5) - (6)].integer)) {
                    /* mauvais parametres */
                    erreur_syntaxe(TTR_BADP);
                } else {
                    ttr_minj = (yyvsp[(3) - (6)].integer);
                    ttr_maxj = (yyvsp[(5) - (6)].integer);
                }
            ;}
    break;

  case 71:
#line 419 "pap.y"
    {
                /*
                 * Remettre a zero toutes les penalites, avant de commencer
                 * a lire les differentes sections
                 */
                raz_toutes_penalites();
            ;}
    break;

  case 97:
#line 468 "pap.y"
    { (yyval.integer) = 0; ;}
    break;

  case 98:
#line 469 "pap.y"
    { (yyval.integer) = 1; ;}
    break;

  case 99:
#line 473 "pap.y"
    {
            /*
             * Renvoie un entier entre 0 et MAX_PEN
             */
            long valeur = (yyvsp[(1) - (1)].integer);
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
            (yyval.integer) = valeur;
        ;}
    break;

  case 101:
#line 494 "pap.y"
    {
                i = (yyvsp[(1) - (5)].integer);
                if (i < 1) {
                    avert_syntaxe(PENC_NULL);
                } else {
                    if (i+1 >= nmax_couleur) {
                        elargir_table(&penalite_couleur,
                                &nmax_couleur,i+2);
                    }
                    while (i < nmax_couleur) {
                                penalite_couleur[i++] = (yyvsp[(5) - (5)].integer);
                                if ((yyvsp[(2) - (5)].integer)==0) break;
                    }
                }
            ;}
    break;

  case 102:
#line 510 "pap.y"
    { penalite_repcoul = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 104:
#line 515 "pap.y"
    {
                i = (yyvsp[(1) - (5)].integer);
                if (i < 1) {
                    avert_syntaxe(PENF_NULL);
                } else {
                    if (i+1 >= nmax_flottement) {
                        elargir_table(&penalite_flottement,
                                &nmax_flottement, i+2);
                    }
                    while (i < nmax_flottement) {
                        penalite_flottement[i++] = (yyvsp[(5) - (5)].integer);
                        if ((yyvsp[(2) - (5)].integer)==0) break;
                    }
                }
            ;}
    break;

  case 105:
#line 531 "pap.y"
    { minoration_fac = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 106:
#line 533 "pap.y"
    { penalite_flcum = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 108:
#line 538 "pap.y"
    { penalite_mcoul = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 109:
#line 540 "pap.y"
    { penalite_copp = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 110:
#line 542 "pap.y"
    { penalite_bipbip = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 111:
#line 544 "pap.y"
    { penalite_desuite = (yyvsp[(3) - (3)].integer); ;}
    break;

  case 113:
#line 549 "pap.y"
    {
                i = (yyvsp[(2) - (5)].integer);
                if (i < 1)
                        avert_syntaxe(PENCH_NULL);
                else if (i > NMAX_RONDES)
                        avert_syntaxe(PENCH_TOOFAR);
                else while (i <= NMAX_RONDES) {
                        penalite_chauvinisme[i-1] = (yyvsp[(5) - (5)].integer);
                        i++;
                        if ((yyvsp[(3) - (5)].integer) == 0) break;
                    }
            ;}
    break;

  case 115:
#line 565 "pap.y"
    {
#ifdef ELITISME

                i = (yyvsp[(2) - (5)].integer);
                if (i < 1)
                        avert_syntaxe(PENEL_NULL);
                else if (i > NMAX_RONDES)
                        avert_syntaxe(PENEL_TOOFAR);
                else while (i <= NMAX_RONDES) {
                        penalite_elitisme[i-1] = (yyvsp[(5) - (5)].integer);
                        i++;
                        if ((yyvsp[(3) - (5)].integer) == 0) break;
                    }

#else
        i = (yyvsp[(2) - (5)].integer);
        if (i > 0) {
            puts("WARNING : penalite d'elitisme ignoree dans cette version de PAPP");
            /* lire_touche(); */
        }

#endif
       ;}
    break;

  case 116:
#line 593 "pap.y"
    {   COPIER((yyvsp[(3) - (3)].string), &date_tournoi); ;}
    break;

  case 117:
#line 596 "pap.y"
    {
                COPIER((yyvsp[(3) - (3)].string), &nom_du_tournoi);
                type_fichier_intermediaire = CORRECT ;
            ;}
    break;

  case 118:
#line 601 "pap.y"
    {
                if( ((yyvsp[(3) - (3)].integer)<1) || ((yyvsp[(3) - (3)].integer)>NMAX_RONDES) )
                    erreur_syntaxe(ICMD_BAD_RND) ;
                else {
                    nombre_de_rondes = (yyvsp[(3) - (3)].integer) ;
                    /* Si 2n-1 ou 2n rondes, on a 2n joueurs max pour un toutes-rondes */
                    ttr_maxj = ((nombre_de_rondes+1)/2)*2 ;
                    ttr_minj = 1 ;
                }
            ;}
    break;

  case 119:
#line 612 "pap.y"
    { coef_brightwell = ((yyvsp[(3) - (3)].reel))/2; ;}
    break;

  case 120:
#line 614 "pap.y"
    {
                numeroter_les_tables();
                mettre_aj_scores();
                ronde_suivante();
            ;}
    break;

  case 121:
#line 620 "pap.y"
    {
                decouplage_absents();
            ;}
    break;

  case 122:
#line 624 "pap.y"
    {
                long Jn=(yyvsp[(2) - (4)].integer), Jb=(yyvsp[(3) - (4)].integer), n1, n2;
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        /* mauvais appariement */
                        erreur_syntaxe(ICMD_BPAIR);
                else if (polarite(Jn) || polarite(Jb))
                        /* deja apparies */
                        erreur_syntaxe(ICMD_APAIR);
                else
                        accoupler(Jn, Jb, SCORE_INCONNU);
            ;}
    break;

  case 123:
#line 638 "pap.y"
    {
                long Jn=(yyvsp[(2) - (5)].integer), Jb=(yyvsp[(3) - (5)].integer), n1, n2;
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BPAIR);
                else if (polarite(Jn) || polarite(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        accoupler(Jn, Jb, ENTIER_EN_SCORE((yyvsp[(4) - (5)].integer)));
            ;}
    break;

  case 124:
#line 650 "pap.y"
    {
#ifndef USE_DEMI_PIONS
            beep();
            erreur_syntaxe(ICMD_DEMI);
#else
                long Jn=(yyvsp[(2) - (5)].integer), Jb=(yyvsp[(3) - (5)].integer), n1, n2;
                double Scn=(yyvsp[(4) - (5)].reel);
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BPAIR);
                else if (polarite(Jn) || polarite(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        accoupler(Jn, Jb, FLOTTANT_EN_SCORE(Scn));
#endif
            ;}
    break;

  case 125:
#line 668 "pap.y"
    {
                long Jn=(yyvsp[(2) - (6)].integer), Scn=(yyvsp[(3) - (6)].integer), Jb=(yyvsp[(4) - (6)].integer), Scb=(yyvsp[(5) - (6)].integer), n1, n2;
                if (MAUVAIS_TOTAL(ENTIER_EN_SCORE(Scn),ENTIER_EN_SCORE(Scb)))
                        erreur_syntaxe(ICMD_TOTS);
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BRES);
                else if (polarite(Jn) || polarite(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        accoupler(Jn, Jb, ENTIER_EN_SCORE(Scn));
            ;}
    break;

  case 126:
#line 682 "pap.y"
    {
#ifndef USE_DEMI_PIONS
            beep();
            erreur_syntaxe(ICMD_DEMI);
#else
        long Jn=(yyvsp[(2) - (6)].integer), Jb=(yyvsp[(4) - (6)].integer), n1, n2;
                double Scn=(yyvsp[(3) - (6)].reel), Scb=(yyvsp[(5) - (6)].reel);
                if (MAUVAIS_TOTAL(FLOTTANT_EN_SCORE(Scn),FLOTTANT_EN_SCORE(Scb)))
                        erreur_syntaxe(ICMD_TOTS);
                n1 = numero_inscription(Jn);
                n2 = numero_inscription(Jb);
                if (n1<0 || n2<0 || !present[n1] || !present[n2])
                        erreur_syntaxe(ICMD_BRES);
                else if (polarite(Jn) || polarite(Jb))
                        erreur_syntaxe(ICMD_APAIR);
                else
                        accoupler(Jn, Jb, FLOTTANT_EN_SCORE(Scn));
#endif
            ;}
    break;

  case 127:
#line 702 "pap.y"
    { raz_couplage(); ;}
    break;

  case 128:
#line 704 "pap.y"
    {
                i = 0; taille_ttr = (yyvsp[(3) - (4)].integer);
                CALLOC(table_ttr, taille_ttr, long);
            ;}
    break;

  case 129:
#line 709 "pap.y"
    {
                assert(table_ttr);
                if (i != taille_ttr)
                        erreur_syntaxe(TTR_TOOSMALL);
                else
                        charge_table_ttr(taille_ttr, table_ttr);
                free(table_ttr);
            ;}
    break;

  case 131:
#line 721 "pap.y"
    {
                if (i >= taille_ttr)
                        erreur_syntaxe(TTR_TOOBIG);
                else
                        table_ttr[i++] = (yyvsp[(2) - (2)].integer);
            ;}
    break;

  case 133:
#line 731 "pap.y"
    {
                long n = inscrire_joueur((yyvsp[(3) - (3)].integer));
                if (n < 0) {
                        sprintf(buffer, PLR_UNKNOWN, (yyvsp[(3) - (3)].integer));
                        erreur_syntaxe(buffer);
                } else
                        present[n] = 1;
            ;}
    break;

  case 134:
#line 740 "pap.y"
    {
                long n = inscrire_joueur((yyvsp[(3) - (3)].integer));
                if (n < 0) {
                        sprintf(buffer, PLR_UNKNOWN, (yyvsp[(3) - (3)].integer));
                        erreur_syntaxe(buffer);
                } else
                        present[n] = 0;
            ;}
    break;

  case 135:
#line 751 "pap.y"
    { erreur_syntaxe(CMD_UNKNOWN); ;}
    break;

  case 136:
#line 753 "pap.y"
    { erreur_syntaxe(ERR_SYNTAX); ;}
    break;

  case 138:
#line 758 "pap.y"
    { erreur_syntaxe(M_SEMICOLON); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2577 "pap.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



