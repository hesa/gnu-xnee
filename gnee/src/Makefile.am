## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = gnee

gnee_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	recordables.c recordables.h \
	gnee_xnee.c gnee_xnee.h

CFLAGS = -I/home/joda/src/xnee/libxnee/include
LDFLAGS = -L/home/joda/src/xnee/libxnee/src
gnee_LDADD = @PACKAGE_LIBS@ $(INTLLIBS) ../../libxnee/libxnee.a -lpthread ${X_LIBS} -lXext  -lXtst

