## Process this file with automake to produce Makefile.in


if HAVE_LIBXNEE_DIR
LIBXNEE_DIR=@libxnee_dir@
else
LIBXNEE_DIR="./"
endif




INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-I${LIBXNEE_DIR}/include \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = gnee

gnee_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	recordables.c recordables.h \
	gnee_xnee.c gnee_xnee.h

gnee_LDADD = @PACKAGE_LIBS@ $(INTLLIBS) \
	-L${LIBXNEE_DIR}/ -L${LIBXNEE_DIR}/.. -lxnee \
	 ${X_LIBS} -lXext  -lXtst -lX11 -lpthread -ldl -lxosd

