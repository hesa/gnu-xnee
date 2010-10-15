## Process this file with automake to produce Makefile.in


LIBXNEE_DIR="../../libxnee"


INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-I${LIBXNEE_DIR}/include \
	-I../../ 

bin_PROGRAMS = gnee

gnee_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	recordables.c recordables.h \
	gnee_xnee.c gnee_xnee.h 

if XNEE_XINPUT_SUPPORT
XNEE_XINPUT_SUPPORT_FLAGS = -DXNEE_XINPUT_SUPPORT
else
XNEE_XINPUT_SUPPORT_FLAGS =
endif

gnee_CFLAGS = ${PEDANTIC_FLAGS} $(libgnomeui_CFLAGS) $(gtk_CFLAGS)  ${XNEE_XINPUT_SUPPORT_FLAGS} 
AM_CFLAGS = ${PEDANTIC_FLAGS} 

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif


gnee_LDFLAGS =  $(STATIC_FLAGS) 

gnee_LDADD =    \
	-L${LIBXNEE_DIR}/ -L${LIBXNEE_DIR}/.. -L${LIBXNEE_DIR}/src -lxnee \
	 ${X_LIBS} ${X11_LIBS} ${LIBDL} $(LIBSEMA)  \
         $(libgnomeui_LIBS) $(gtk_LIBS) 



