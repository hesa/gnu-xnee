SUBDIRS = libxnee cnee gnee doc

AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = Makefile.xnee Makefile.cvs autotools/install-sh 

doc: html man pdf ps text info xnee.1 
	cd ./doc/ && $(MAKE) $(AM_MAKEFLAGS) $@

