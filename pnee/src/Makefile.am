## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@ -I../../libxnee/include

PNEE=pnee
noinst_PROGRAMS = $(PNEE)
pnee_CFLAGS = -DHAVE_STDARG $(PANEL_FLAGS) -Wall -g
pnee_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h

pnee_LDADD =  -L../../ -L../../libxnee/src @PACKAGE_LIBS@ $(INTLLIBS) $(PANEL_LD_FLAGS) -lxnee -lXtst

install-exec-local: $(PNEE)	
	mkdir -p $(PANEL_APPLET_DIR)
	cp $(PNEE)  $(PANEL_APPLET_DIR)

