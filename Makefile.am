SUBDIRS = ${LIB_DIR}  ${CLI_DIR} ${GUI_DIR} ${DOC_DIR}

AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = Makefile.xnee Makefile.cvs autotools/install-sh projects/test.xnp sessions/example1.xns README BUGS FAQ EXAMPLES




manual: 
	cd ./doc/ && $(MAKE) all

man:
	cd ./cnee/src && make man
