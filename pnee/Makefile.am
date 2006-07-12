## Process this file with automake to produce Makefile.in

SUBDIRS = pics src

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
	mkdir -p $(PANEL_SERVER_DIR)
	cp pnee.server   $(PANEL_SERVER_DIR)/

