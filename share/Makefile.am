XNEE_DATA_FILES = xnee.sh
EXTRA_DIST = xnee.sh.in1 xnee.sh.in2
pkgdata_DATA = xnee.sh

CLEAN_FILES=xnee.sh

xnee.sh: xnee.sh.in1 xnee.sh.in2
	cp  xnee.sh.in1 xnee.sh
	cat xnee.sh.in2 | sed 's,__VERSIONTAG__,$(VERSION),g' >> xnee.sh

