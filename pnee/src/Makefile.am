## Process this file with automake to produce Makefile.in


INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@ -I../../libxnee/include


PNEE=pnee
noinst_PROGRAMS = $(PNEE)
pnee_CFLAGS = -DHAVE_STDARG $(PANEL_FLAGS) -Wall -g -DPNEE_PIXDIR=\""$(datadir)/xnee/pixmaps"\"
pnee_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	pnee_impl.c pnee_impl.h pnee_types.h 

pnee_LDADD =  -L../../ -L../../libxnee/src @PACKAGE_LIBS@ $(INTLLIBS) $(PANEL_LD_FLAGS) -lxnee -lXtst $(LIBSEMA)   

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif
pnee_LDFLAGS =  $(STATIC_FLAGS)

install-exec-local: $(PNEE)	
	mkdir -p $(DESTDIR)$(PANEL_APPLET_DIR)
	cp $(PNEE)  $(DESTDIR)$(PANEL_APPLET_DIR)

