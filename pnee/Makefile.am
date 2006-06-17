## Process this file with automake to produce Makefile.in

SUBDIRS = src 

EXTRA_DIST = \
	autogen.sh \
	pnee.glade \
	pnee.gladep \
        data/pnee.server.in 

CLEAN_FILES=pnee.server

clean-local:
	rm -f pnee.server

all: pnee.server

pnee.server: data/pnee.server.in
	cat data/pnee.server.in | \
	sed 's,APPLET_DIR,$(PANEL_APPLET_DIR)/pnee,g' \
	> pnee.server

install-data-local: pnee.server
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi
	mkdir -p $(PANEL_SERVER_DIR)
	cp pnee.server   $(PANEL_SERVER_DIR)/

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

