%{

/*
 * pap.l: Analyse lexicale pour PAPP
 *
 * (EL) 22/09/2012 : v1.36, no change.
 * (EL) 12/09/2012 : v1.35, no change.
 * (EL) 25/06/2012 : v1.34, Aout du mot-clef 'XML' indiquant la generation des fichiers XML
 * (EL) 20/07/2008 : v1.33, Ajout de la reconnaissance de la phrase '#_Date'
                     dans le fichier intermediaire.
 * (EL) 05/05/2008 : v1.33, Tous les 'int' deviennent 'long' pour etre sur d'etre sur 4 octets.
 * (EL) 21/04/2008 : v1.32, ajout de '%option noyywrap' et suppression d'un #define yywrap
                     pour eviter une erreur de compilation.
 * (EL) 29/04/2007 : v1.31. Ajout de la regle mettant de cote les caracteres indiquant
                     un codage Unicode (rajoutes par Notepad Windows par exemple)
 * (EL) 30/03/2007 : Ajout des mots-clefs 'dossier', 'true' et 'false' indiquant
                     s'il faut regrouper tous les fichiers class/ronde/result...
                     dans un meme sous-dossier portant le nom du tournoi.
 * (EL) 07/02/2007 : Ajout de la reconnaissance des phrases '#_Nom-Tournoi'
                     '#_Nombre-Rondes' et '#_BQ-double' (avec les tokens correspondants)
                     permettant de mettre ces informations dans le fichier intermediaire.
 * (EL) 04/02/2007 : Ajout du mot-clef 'BQ-double' qui donne
 *                   la constante lorsqu'on compte 1 point par victoire.
 * (EL) 04/02/2007 : initialisation des variables 'type_fichier_config'
 *                   et 'type_fichier_intermediaire' au debut de la lecture
 * (EL) 02/02/2007 : Ajout de la reconnaissance des nombres flottants et demi_entiers.
 * (EL) 13/01/2007 : v1.30 by E. Lazard, no change
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "changes.h"

#ifdef __THINK_C__
# include <unix.h>
#elif defined(__BORLANDC__) || defined(PAPP_WINDOWS_METROWERKS)
# include <io.h>
#else
# include <unistd.h>
#endif

#include "pap_tab.h"
#include "global.h"

/*
 * Pour Flex : il est necessaire de caster "yytext", qui est de type
 * "unsigned char *", en un simple "char *". La definition de yywrap()
 * permet d'eviter l'edition de liens avec libfl.a.
 */

#define texte		((char *)yytext)
/*#define yywrap()	1*/

#ifdef ENGLISH
# define BAD_KEYWORD		"unknown keyword `%s'"
# define BAD_BYTE			"found byte 0x%02x (binary data?)"
# define BAD_QUOTE			"unmatched quotes"
# define TOO_MANY_ERRS		"too many syntax errors"
# define AT_LINE			"%s, line %ld: %s\n"
#else
# define BAD_KEYWORD		"mot-cle `%s' inconnu"
# define BAD_BYTE			"caractere 0x%02x (donnees binaires?)"
# define BAD_QUOTE			"guillemets non refermes"
# define TOO_MANY_ERRS		"trop d'erreurs de syntaxe"
# define AT_LINE			"%s, ligne %ld: %s\n"
#endif

/* Etat de l'analyse lexicale */
static long erreur_constatee = 0;
static long numero_ligne;
/* static */char *nom_fichier_lu;
char *pays_courant;

/* Table des mots-cles */
static struct mot_cle {
	const char    *nom;
	long     valeur;
} mots_cles[] = {
	{"fichier",                      KW_FICHIER},
	{"joueurs",                      KW_JOUEURS},
	{"nouveaux",                     KW_NOUVEAUX},
	{"intermediaire",                KW_INTER},
	{"inter",                        KW_INTER},
	{"resultats",                    KW_RESULT},
	{"classement",                   KW_CLASS},
	{"equipes",                      KW_EQUIPES},
	{"appariements",                 KW_APPARIEMENTS},
	{"tableau-croise",               KW_CROSSTABLE},
	{"elo",                          KW_ELO},
	{"log",                          KW_LOG},
	{"trace",                        KW_LOG},
	{"dossier",                      KW_DOSSIER},
	{"true",                         KW_TRUE},
	{"false",                        KW_FALSE},
	{"XML",							 KW_XML},

	{"penalites",                    KW_PENALITES},
	{"penalite",                     KW_PENALITES},
	{"pays",                         KW_PAYS},
	{"zone-insertion",               KW_ZONE_INS},
	{"brightwell",                   KW_BRIGHTWELL},
	{"BQ-double",				     KW_BRIGHTWELL_DBL},
	{"sauvegarde",                   KW_SAUVEGARDE},
	{"immediate",                    KW_SAUVIMM},
	{"differee",                     KW_SAUVDIF},
	{"impression",                   KW_IMPRESSION},
	{"manuelle",                     KW_MANUELLE},
	{"automatique",                  KW_AUTOMATIQUE},
	{"affichage-pions",              KW_AFF_PIONS},
	{"normal",                       KW_AFF_NORMAL},
	{"absolu",                       KW_AFF_NORMAL},
	{"differentiel",                 KW_AFF_DIFF},
	{"relatif",                      KW_AFF_DIFF},
	{"score-bip",                    KW_SCORE_BIP},
	{"ronde-suivante",               KW_RONDESUIV},
	{"raz-couplage",                 KW_RAZ_COUPL},
	{"toutes-rondes",                KW_TTRONDES},
	{"table-toutes-rondes",          KW_TABTTR},

	{"fois",                         KW_FOIS},
	{"demi-point",                   KW_DPOINT},
	{"demi-points",                  KW_DPOINT},
	{"couleur",                      KW_COULEUR},
	{"couleurs",                     KW_COULEUR},
	{"flottement",                   KW_FLOTTEMENT},
	{"cumule",                       KW_DESUITE},
	{"minoration",                   KW_MINORATION},
	{"anti-cumule",                  KW_MINORATION},
	{"elitisme",                     KW_ELITISME},
	{"repetition",                   KW_REPET},
	{"memes-couleurs",               KW_MCOUL},
	{"couleurs-opposees",            KW_COPP},
	{"de-suite",                     KW_DESUITE},
	{"bip-bip",                      KW_BIPBIP},
	{"chauvinisme",                  KW_CHAUVIN},
	{"ronde",                        KW_RONDE},
	{"rondes",                       KW_RONDE},

	{(char*) 0,                      0}
}, *mc;

%}
%option noyywrap


UTF_BOM (\xEF\xBB\xBF)|((\x00\x00)?(\xFE\xFF))|((\xFF\xFE)(\x00\x00)?)|(\x0E\xFE\xFF)|(\xFB\xEE\x28)|(\xDD\x73\x66\x73)|((\x2B\x2F\x76)([\x38\x39\x2B\x2F]|(\x38\x2D)))
CRLF \x0D\x0A
EOL [\x0A\x0D]
NOT_EOL [^\x0A\x0D]

%s UNQUOTED

%%

__eof__         return 0;

{UTF_BOM}       ;
[0-9]+          { yylval.integer = strtol(texte, NULL, 10); return INTEGER; }
[0-9]+\.[05]    { yylval.reel    = atof(texte); return INT_ET_DEMI; }
[0-9]*\.[0-9]+  { yylval.reel    = atof(texte); return DOUBLE; }
[ \t\000]+      ;
\%              ;
\%_             ;
\%[^_!\x0A\x0D]{NOT_EOL}*   ;
#               ;
#[^_\x0A\x0D]{NOT_EOL}*       ;
#_Nom-Tournoi   { return DIESE_NOM ; }
#_Nombre-Rondes { return DIESE_RONDES ; }
#_BQ-double     { return DIESE_BRIGHTWELL_DBL ; }
#_Date          { return DIESE_DATE ; }
\%!{NOT_EOL}*           { fprintf(stderr, "%s\n", texte); }

\"[^\x0A\x0D\"]*\"    {
		yylval.string = strcpy(new_string(), texte+1);
		yylval.string[yyleng-2] = 0;
		return STRING;
		}

\"[^\x0A\x0D\"]*{EOL}    {
		erreur_syntaxe(BAD_QUOTE);
		++numero_ligne;
		yylval.string = strcpy(new_string(), texte+1);
		yylval.string[yyleng-2] = 0;
		return STRING;
		}

<UNQUOTED>{CRLF}[Pp]ays    { ++numero_ligne; return KW_PAYS; }
<UNQUOTED>{EOL}[Pp]ays     { ++numero_ligne; return KW_PAYS; }
<UNQUOTED>^[Pp]ays          return KW_PAYS;
<UNQUOTED>{CRLF}[Ff]ederation{NOT_EOL}*   {++numero_ligne;}
<UNQUOTED>{EOL}[Ff]ederation{NOT_EOL}*    {++numero_ligne;}
<UNQUOTED>^[Ff]ederation{NOT_EOL}*         ;

<INITIAL>{CRLF}    { ++numero_ligne; }
<INITIAL>{EOL}     { ++numero_ligne; }
<UNQUOTED>{CRLF}   { ++numero_ligne; return TOK_NEWLINE; }
<UNQUOTED>{EOL}    { ++numero_ligne; return TOK_NEWLINE; }

<UNQUOTED>[a-z_A-Z][a-z_A-Z'0-9\-\.]* {
		yylval.string = strcpy(new_string(), texte);
		return STRING;
		}

<UNQUOTED>`[^\x0A\x0D\000]*	{
		yylval.string = strcpy(new_string(), texte+1);
		return COMMENT;
		}

<INITIAL>[a-zA-Z][a-zA-Z\-]* {
		/* chercher le mot dans la table */
		for (mc = mots_cles; mc->nom; mc++)
			if (!compare_chaines_non_sentitif(mc->nom,texte))
				return mc->valeur;
		/* il n'y est pas */
		if (!compare_chaines_non_sentitif("infini",texte)) {
			yylval.integer = LONG_MAX;
			return INTEGER;
		} else {
			char *buf = new_string();
			sprintf(buf, BAD_KEYWORD, texte);
			erreur_syntaxe(buf);
			return INVALID_KEYWORD;
			}
		}

.               {
		if (yytext[0] & 0x80) {
			char *buf = new_string();
			sprintf(buf, BAD_BYTE, yytext[0]);
			erreur_syntaxe(buf);
		}
		return yytext[0];
		}




%%

/*
 *  Fonction de conversion des CR du Mac en CR/LF (en conservant
 *  les CR/LF du DOS) pour la version PC de Papp, car le parser
 *  genere par Flex sur PC n'aime pas les lignes tres longues.
 *  Le fichier d'origine n'est pas touche, on cree un fichier
 *  temporaire qu'il faudra penser a effacer apres l'avoir parse.
 *  Retourne 0 si tout se passe bien, -1 sinon.
 */


long Macintosh_to_PC(char *filename, char **tempfile) {
	FILE *old,*temp;
	long c1,c2;
    char nom_temp[]="temp.XXXXXX" ;

    *tempfile = "";
	old = fopen( filename, "rb" );
	if ( old == NULL )
		return -1;

	/* On essaie d'avoir un nom temporaire */
	if (mktemp(nom_temp)==NULL)
		return -1 ;

	temp = fopen( nom_temp, "wb" );
	if ( temp == NULL )
		return -1;
	c1 = fgetc(old);
    if ( c1 == EOF )
    	return -1;

	c2 = 0;
	while( (c2=fgetc(old)) != EOF ) {
	    if ((c1 == 13) && (c2 != 10)) {
	       fputc( 13, temp );
	       fputc( 10, temp );
	    } else
	       fputc( c1, temp );

	    c1 = c2;
	}

	fputc(c1,temp);
	if (c1 == 13)
		fputc(10,temp);
	fclose( old );
	fclose( temp );
	strcpy(*tempfile,nom_temp);
	return 0;
}

/*
 * Fonction de lecture; type==0 pour le fichier de config, et ==1 pour
 * un fichier de joueurs.
 */


/*
 * Sur les anciens Macintosh, les fonctions de lecture de la librairie
 * standard sont trop intelligentes pour les fichiers textes et
 * inversent carrement les CR et les LF ! On ouvre donc les fichiers
 * en binaire.
 * Question : est-ce que les parseurs marchent encore sur les autres
 * plate-formes si on fait la meme chose ?
 */

#if defined(__THINK_C__) || defined(PAPP_MAC_METROWERKS)
   #define MODE_OUVERTURE_FICHIER "rb"
#else
   #define MODE_OUVERTURE_FICHIER "r"
#endif

long lire_fichier (char *filename, long type) {
	static long nb_lectures = 0;
	long ret;
#ifdef DUPLIQUE_FICHIER_PARSER
	long use_temp_file;
	char *tempfile;
#endif

	assert(type == CONFIG_F || type == PLAYERS_F || type == NEWPLAYERS_F);
#ifdef DEBUG
	fprintf(stderr,"Lecture de '%s' (%s)\n", filename,
		type == CONFIG_F ? "config" : "joueurs ou nouveaux");
#endif

#ifdef DUPLIQUE_FICHIER_PARSER
    tempfile = trivialloc(250);
    if (Macintosh_to_PC(filename,&tempfile) == 0) {
       yyin = fopen(tempfile, MODE_OUVERTURE_FICHIER);
       use_temp_file = 1;
    } else {
       yyin = fopen(filename, MODE_OUVERTURE_FICHIER);
       use_temp_file = 0;
    }
#else
    yyin = fopen(filename, MODE_OUVERTURE_FICHIER);
#endif

	if (yyin == NULL)
		return -1;
	if (nb_lectures++)
		yyrestart(yyin);
	erreur_constatee = 0;
	numero_ligne = 1;
	nom_fichier_lu = filename;
	pays_courant = pays_defaut;
	/* Type d'analyse lexicale? */
	if (type == CONFIG_F) {
		BEGIN(INITIAL);
		type_fichier_config = CORRECT ;
		type_fichier_intermediaire = OLD ;
	} else {
		BEGIN(UNQUOTED) ;
		fichier_des_nouveaux = ((type == NEWPLAYERS_F) ? 1 : 0) ;
	}
	ret = yyparse();
	fclose(yyin);

#ifdef DUPLIQUE_FICHIER_PARSER
    if (use_temp_file)
       { remove(tempfile); }
#endif


    if (type == CONFIG_F)
		pays_defaut = pays_courant;
	return (ret + erreur_constatee);
}

/* Fonctions d'erreur */

void erreur_syntaxe(const char *erreur) {
	avert_syntaxe(erreur);
	if (++erreur_constatee >= 15)
		fatal_error(TOO_MANY_ERRS);
}

void avert_syntaxe(const char *erreur) {
	fprintf(stderr, AT_LINE, nom_fichier_lu, numero_ligne, erreur);
}
