## Process this file with automake to produce Makefile.in

SUBDIRS = src 
#po

EXTRA_DIST = \
	autogen.sh \
	pnee.glade \
	pnee.gladep 

@INTLTOOL_DESKTOP_RULE@
@INTLTOOL_SCHEMAS_RULE@

#INTLTOOL_BUILT = \
#        intltool-extract \
#        intltool-merge \
#        intltool-update

Applicationsdir = $(datadir)/applications
Applications_in_files = pnee.desktop.in 
Applications_DATA = $(Applications_in_files:.desktop.in=.desktop)

schemadir=$(GCONF_SCHEMA_FILE_DIR)
schema_DATA=pnee.schemas

CLEANFILES=$(Applications_DATA) $(schema_DATA)
install-schemas:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(schema_DATA) || true

install-data-local_sub:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

install-data-local: install-schemas install-data-local_sub



dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

