SUBDIRS = src doc 

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

test: 
	cd xnee/test && ./test_all.sh

.PHONY: html pdf text info xnee.1 devel 

## -------------------- ##
## EO Forwarding targets##
## -------------------- ##
