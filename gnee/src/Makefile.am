## Process this file with automake to produce Makefile.in


if HAVE_LIBXNEE_DIR
LIBXNEE_DIR=@libxnee_dir@
else
LIBXNEE_DIR="../../libxnee"
endif

if XOSD
XOSD_FLAGS = -DHAVE_XOSD
XOSD_LIBS = -lxosd
else
XOSD_FLAGS = -DNO_XOSD
XOSD_LIBS = 
endif


INCLUDES = \
	${XOSD_FLAGS} \
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

if PEDANTIC
PEDANTIC_FLAGS =  -W -Wall
else
PEDANTIC_FLAGS = 
endif

gnee_CFLAGS = ${PEDANTIC_FLAGS}

gnee_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)  \
	-L${LIBXNEE_DIR}/ -L${LIBXNEE_DIR}/.. -L${LIBXNEE_DIR}/src -lxnee \
	${XOSD_LIBS} \
	 ${X_LIBS} -lXext  -lXtst -lX11 -lpthread -ldl 

