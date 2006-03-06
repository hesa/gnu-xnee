XNEE_DATA_FILES = xnee.sh
EXTRA_DIST = $(XNEE_DATA_FILES) 
pkgdata_DATA = $(XNEE_DATA_FILES)


xnee.sh: xnee.sh.in1
	cp  xnee.sh.in1 xnee.sh
	cat xnee.sh.in2 | sed 's,__VERSIONTAG__,$(VERSION),g' >> xnee.sh
