SUBDIRS = src doc 

## -------------------- ##
## Forwarding targets.  ##
## -------------------- ##
doc: html pdf info text man txt ps  

html pdf info text man txt ps :
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@

test: 
	cd test && ./test_all.sh

.PHONY: html pdf text info xnee.1 devel 

## -------------------- ##
## EO Forwarding targets##
## -------------------- ##
