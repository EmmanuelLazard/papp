#########################################################################################
#
# Makefile pour PAPP (programme d'appariements)
# par Thierry Bousch, Emmanuel Lazard & Stephane Nicolet
#
# Makefile v1.36                                                                        #
# 17/07/2012 (Lazard) : Ajout des options 'pedantic' et 'ansi' pour se conformer au C90 #
# 12/01/2007 (Lazard) : Ajout d'une option de compilation pour supprimer les warnings   #
#########################################################################################

# POUR COMPILER, IL FAUT LANCER LA CIBLE 'papp' EN TAPANT 'make papp'

# Le compilateur utilise doit etre ANSI
CC = gcc

# PAPP a ete compile au moins une fois sur des machines variees, et
# utilise les symboles suivants exportes par les compilateurs:
# (Il ne faut donc pas les definir)
#
# `atarist' pour l'Atari,
# `__GNUC__' pour le GCC,
# `__MSDOS__' pour MS-DOS,
# `__THINK_C__' pour le Think-C (Macintosh),
# `__MWERKS__' pour le CodeWarrior-C (Macintosh),
# `__BORLANDC__' pour Borland C++ pour Windows
# `__GO32__' pour le cross- compilateur MS-DOS (sous linux).
#
#-------------------------------------------------------------------------------
#
# On peut, par ailleurs, definir les symboles suivants
# soit pour faciliter la compilation en fonction de l'OS,
# soit pour activer/desactiver certaines options
# (voir aussi le fichier changes.h, qui definit
# d'autres symboles plus recents).
#
#
#----------- Symboles caracterisant le systeme d'exploitation ------------------
#
# UNIX_BSD
#   si vous utilisez un Unix de type BSD, et
#
# UNIX_SYSV
#   si vous utilisez un Unix de type Systeme 5.
#
# MACOSX
#   si vous utilisez un Unix MacOS X
#
# CYGWIN
#       pour la compilation sous Windows avec l'emulateur Unix cygwin
#
# LINUX
#      si vous utilisez un Unix Linux
#
#----------- Symboles a definir ou non suivant votre OS ------------------------
#
# TERMCAP
#   si vous avez la librairie Termcap, sinon les commandes de gestion
#   d'ecran (effacement, etc.) devront etre codees en dur dans le
#   programme. Unix surtout.
#
# NO_TERMCAP_H
#   si vous avez TERMCAP mais pas de fichier d'en-tete <termcap.h>.
#
# SIGNAUX_POSIX
#   si vous voulez utiliser les fonctions POSIX pour manipuler les
#   signaux (sigaction, sigprogmask, sigemptyset,...)
#
# NO_RAISE
#   si raise() n'est pas dans les librairies -- c'est pourtant une
#   fonction ANSI.
#
# P_OPEN
#   si votre compilateur connait les fonctions popen()/pclose()
#       et si elles font quelque chose de sense, evidemment.
#   Si ce symbole est defini, il est possible de "piper" le classement
#   des joueurs vers une commande (less, par exemple).
#
#----------- Options de compilation -----------
#
# ENGLISH
#   define this symbol if you want messages and menus in English.
#   The translations are not very good. Note that all key bindings
#   are the same as in the french version (at least for now),
#   except for the yes/no questions. The config file must
#   still be in French (well, not the comments, of course)
#   for compatibility with the french version.
#
# CURSEUR (defaut : OFF)
#   sur certains terminaux (que nous ne nommerons pas), le curseur est
#   invisible, ce qui est incommode pour entrer du texte. Si ce symbole
#   est defini, le curseur sera materialise par une tilde.
#
# LOCK (defaut : ON)
#   cree un fichier-verrou dans le repertoire courant pour eviter
#   que deux invocations de papp ne modifient le meme fichier intermediaire.
#
# DEBUG_PEN (defaut : OFF)
#   Si ce symbole est defini, PAPP affiche les penalites trouvees (ainsi
#   que leur total) dans les appariements optimaux.
#
# SAVE_MATRIX (defaut : OFF)
#   sauve la matrice des penalites dans `matrice.pen' avant de lancer
#   l'algorithme d'Edmonds, sous une forme ou elle peut etre relue par
#   le programme `test'. Peut etre utile pour la mise au point du
#   module edmonds.c. Les resultats de l'optimisation (si on arrive
#   jusque la) seront stockes dans `matrice.out'.
#
# USE_LONG_LONG (defaut : OFF)
#   si vous voulez utiliser des "long long" dans edmonds.c (entiers
#   sur 64 bits) au lieu des "double" (flottants sur 64 bits).
#   Seul le GCC supporte ce type. Peut etre necessaire si la librairie
#   flottante est boguee ou imprecise. (Ce symbole est automatiquement
#   defini si __GO32__ est defini).
#
# EDEBUG (defaut : OFF)
#   affiche enormement de details sur le deroulement de l'algorithme
#   d'Edmonds. N'est utile que pour la mise au point.
#
# MELANGE (defaut : ON)
#   si vous voulez melanger les joueurs avant toute optimisation,
#   c.a.d. pas uniquement a la premiere ronde. Ceci a pour consequence
#   de rendre les appariements non deterministes.
#
# HUIT_BITS (defaut : OFF)
#   si vous voulez autoriser l'utilisateur a entrer des caracteres
#   huit bits au clavier. A utiliser avec circonspection, parce que
#   cela diminue la portabilite vers d'autres machines. Seules les
#   machines utilisant le jeu ISO-Latin-1 (windows, unix) devraient
#   avoir le droit de definir ce symbole.
#
# Voici les options rajoutees a la compilation
#
EXTRA  = #-DENGLISH
#
#-------------------------------------------------------------------------------
#
# Voici les commmandes de compilation pour divers compilateurs
# Il faut toutes les commenter sauf celle qui vous correspond.
#
#------------------------------------
#
# Options de compilation pour MacOS X

CFLAGS = -DMACOSX -DUNIX_BSD \
		 -DSIGNAUX_POSIX -DP_OPEN -DTERMCAP -DTERMCAP_H \
         -DMELANGE  $(EXTRA) \
         -O2 -pipe -arch x86_64 -mmacosx-version-min=10.10 \
         -Wstrict-prototypes -Wall -Wextra -Wno-unused-function \
         -pedantic -ansi
LDFLAGS = -mmacosx-version-min=10.10
LIBS = -lm -ltermcap -arch x86_64

#----------

# Options de compilation pour CygWin sous Windows avec ncurses

#CFLAGS = -DCYGWIN -DUNIX_BSD \
#         -DSIGNAUX_POSIX \
#         -DMELANGE  $(EXTRA) \
#         -O2 -pipe \
#         -Wstrict-prototypes -Wall -Wextra -Wno-unused-function \
#         -Wno-char-subscripts -pedantic -ansi
#LDFLAGS =
#LIBS = -lm -lncurses

#----------

# Options de compilation pour Linux
# avec la librairie Termcap
# (si vous ne l'avez pas au bon endroit, copiez-la dans le repertoire papp)

#CFLAGS = -DLINUX -DUNIX_SYSV \
#        -DSIGNAUX_POSIX -DP_OPEN -DTERMCAP -DTERMCAP_H \
#        -DMELANGE  $(EXTRA) \
#        -O2 -pipe \
#        -Wstrict-prototypes -Wall -Wextra -Wno-unused-function \
#        -fomit-frame-pointer -pedantic -ansi
#LDFLAGS = -L.
#LIBS = -lm -ltermcap


#----------

# Options de compilation pour Linux
# sans la libraire Termcap

#CFLAGS = -DLINUX -DUNIX_SYSV \
#         -DSIGNAUX_POSIX -DP_OPEN -DNO_TERMCAP_H \
#         -DMELANGE  $(EXTRA) \
#         -O2 -pipe \
#         -Wstrict-prototypes -Wall -Wextra -Wno-unused-function \
#         -fomit-frame-pointer
#LDFLAGS =
#LIBS = -lm

#----------

# Options de compilation pour SunOS (ou autres versions BSD d'Unix)
# avec la librairie Termcap
#
#CFLAGS = -DUNIX_BSD \
#         -DSIGNAUX_POSIX -DP_OPEN -DTERMCAP -DTERMCAP_H -DNO_RAISE \
#         -DMELANGE  $(EXTRA) \
#         -O2 -pipe \
#         -Wstrict-prototypes -Wall -Wextra -Wno-unused-function \
#LDFLAGS =
#LIBS = -lm -ltermcap

#----------

# Options de compilation pour SunOS (ou autres versions BSD d'Unix)
# sans la librairie Termcap
#
#CFLAGS = -DUNIX_BSD \
#         -DSIGNAUX_POSIX -DP_OPEN -DNO_TERMCAP_H -DNO_RAISE \
#         -DMELANGE  $(EXTRA) \
#         -O2 -pipe \
#         -Wstrict-prototypes -Wall -Wextra -Wno-unused-function \
#LDFLAGS =
#LIBS = -lm

#----------

# Options de cross-compilation pour Ms-Dos
#
#GO32_CFLAGS = \
#       -DMELANGE -DVERROU $(EXTRA) \
#       -O2 -fomit-frame-pointer -pipe -m486 -Wall
#GO32_LDFLAGS = -s
#GO32_LIBS = -lm -lpc
#GO32_CC = go32gcc
#GO32_STUB = /usr/local/go32/lib/stub.exe

# Compilation options for Windows:
#
# When compiling with the Borland C++ compiler,
# you should set Large model in the Code Generation dialog box.
# Then create a project with the C files specified below (after
# the CFILE keyword), then Build All, and Run.

# Les regles suivantes permettent de verifier le code assembleur
# genere, ainsi que de cross-compiler pour Ms-Dos. Commentez-les si
# elles posent probleme avec votre version de `make'.
#
%.s: %.c
	$(CC) $(CFLAGS) -S $< -o $@
%.oms: %.c
	$(GO32_CC) $(GO32_CFLAGS) -c $< -o $@
%.sms: %.c
	$(GO32_CC) $(GO32_CFLAGS) -S $< -o $@

#-------------------------------------------------------------------------------------
#
# Voici la liste de tous les fichiers sources. Ces fichiers peuvent
# etre sauvegardes avec `make archive'
#
HEADERS = changes.h couplage.h crosstable.h pairings.h discs.h global.h \
          more.h pairings.h player.h tiebreak.h version.h

CFILES  = couplage.c crosstable.c discs.c edmonds.c elo.c \
    enterPlayer.c global.c main.c more.c penalties.c player.c \
    results.c roundrobin.c tiebreak.c XML.c

SFILES  = Makefile pap.l pap.y
DOCUS   = papp_doc.tex Bugs A_faire Changes
AUTRES  = pap_tab.h pap_tab.c lexyy.c joueurs

RCSFILES = $(HEADERS) $(CFILES) $(SFILES) $(DOCUS)
SOURCES = $(RCSFILES) $(AUTRES)

#
# Fichiers objets requis par PAPP
#

OBJETS  = \
    lexyy.o pap_tab.o \
    couplage.o crosstable.o tiebreak.o edmonds.o elo.o \
    enterPlayer.o global.o player.o main.o more.o  penalties.o \
    discs.o results.o roundrobin.o XML.o

OBJETS_DOS = \
    lexyy.oms pap_tab.oms \
    edmonds.oms global.oms player.oms main.oms elo.oms more.oms \
    enterPlayer.oms roundrobin.oms couplage.oms results.oms penalties.oms \
    discs.oms crosstable.oms tiebreak.oms XML.oms

#
# Les programmes `exemple' et `geom' produisent des matrices
# d'exemples pour le programme `testp'. Le programme proprement dit
# est `papp', sa documentation est `papp_doc.tex', et `changes' contient
# l'historique des versions.
#

all:
	$(MAKE) papp
	$(MAKE) papp_GB_PHONY

papp : LANGUE=
papp : $(OBJETS)
	$(CC) $(LDFLAGS) -o papp $(OBJETS) $(LIBS)
#	cp papp ..

.PHONY : papp_GB_PHONY
papp_GB_PHONY :
	rm -f *.o
	$(MAKE) papp_GB

papp_GB : LANGUE=-DENGLISH
papp_GB : $(OBJETS)
	$(CC) $(LDFLAGS) -o papp_GB $(OBJETS) $(LIBS)
	rm -f *.o
	cp papp_GB ..

%.o : %.c
	$(CC) -c $< $(CFLAGS) $(EXTRA) $(LANGUE)

.PHONY : clean
clean:
	rm -f core *.s *.sms *.o *.oms *.bak *.go32
	rm -f papp_doc.log papp_doc.aux papp_doc.toc

.PHONY : veryclean
veryclean: clean
	rm -f *.exe $(BINAIRES) lexyy.c pap_tab.c pap_tab.h
	rm -f papp_doc.dvi papp_doc.ps
	rm -f class_*.txt ronde_*.txt result_*.txt resul_*.txt
	rm -f team_*.txt crosstable_*.htm crosstable_*.html

#
# Ces targets ne sont pas utilises sauf en test
#

BINAIRES_TEST = testp geom exemple edtest contest papp

msdos: edtest.exe papp.exe

testp: discs.o edmonds.o test.o
	$(CC) $(LDFLAGS) -o testp $^ $(LIBS)

edtest: discs.o edmonds.o edtest.o
	$(CC) $(LDFLAGS) -o edtest $^ $(LIBS)

contest: discs.o edmonds.o contest.o
	$(CC) $(LDFLAGS) -o contest $^ $(LIBS)

papp.exe: $(OBJETS_DOS)
	$(GO32_CC) $(GO32_LDFLAGS) -o papp.go32 $(OBJETS_DOS) $(GO32_LIBS)
	cat $(GO32_STUB) papp.go32 >papp.exe

edtest.exe: discs.oms edmonds.oms edtest.oms
	$(GO32_CC) $(GO32_LDFLAGS) -o edtest.go32 $^ $(GO32_LIBS)
	cat $(GO32_STUB) edtest.go32 >edtest.exe

exemple: exemple.c
	$(CC) $(CFLAGS) $(LDFLAGS) exemple.c -o exemple $(LIBS)

geom: geom.c
	$(CC) $(CFLAGS) $(LDFLAGS) geom.c -o geom $(LIBS)


#
# Archivage
#
archive: appari.tgz
	@echo Sauvegarde du dossier RCS
	tar -czf $$HOME/papp_rcs.tgz RCS

appari.tgz: $(SOURCES)
	tar -czf appari.tgz $(SOURCES)

distrib: appari.tgz
	./makedist

#
# Maintenant que la documentation est en LaTeX, nous ajoutons une `cible'
# pour previsualiser le document
#

preview: papp_doc.dvi
	xdvi papp_doc.dvi &

papp_doc.dvi: papp_doc.tex
	latex papp_doc.tex
	latex papp_doc.tex


#
# Dependances pour Lex et Yacc.
#
lexyy.c: pap.l
	flex -8 pap.l
	mv lex.yy.c lexyy.c

lexyy.o: pap_tab.h

pap_tab.c: pap_tab.h
	touch pap_tab.c

pap_tab.h: pap.y
	@echo "pap.y devrait contenir 2 conflits de type shift/reduce"
	bison -d pap.y
	mv pap.tab.c pap_tab.c
	mv pap.tab.h pap_tab.h

#
# Autres dependances (on n'est jamais trop prudent):
#
$(OBJETS) $(OBJETS_DOS): $(HEADERS)
test.o edtest.o contest.o: pairings.h discs.h $(HEADERS)
test.oms edtest.oms contest.oms: pairings.h discs.h $(HEADERS)
