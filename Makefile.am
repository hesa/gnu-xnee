AUTOMAKE_OPTIONS = gnu
SUBDIRS = libxnee xnee 

## -------------------- ##
## Forwarding targets.  ##
## -------------------- ##
doc: html pdf info text man txt ps  

html pdf info text man txt ps :
	cd xnee/doc && $(MAKE) $(AM_MAKEFLAGS) $@

api-doc:
	cd libxnee/doc && $(MAKE) $(AM_MAKEFLAGS) $@

devel-doc: doc
	cd libxnee/doc && $(MAKE) $(AM_MAKEFLAGS) $@

.PHONY: html pdf text info xnee.1 devel 

## -------------------- ##
## EO Forwarding targets##
## -------------------- ##

EXTRA_DIST = MANUAL \
	DEVELOPMENT EXAMPLES USAGE \
	TODO DESCRIPTION FAQ BUGS \
	build*.sh depcomp \
	resources/*.xns  sessions/*.xnr \
	Makefile.xnee Makefile.openbsd Makefile.cvs 

