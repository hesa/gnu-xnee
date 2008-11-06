SUBDIRS = ${LIB_DIR}  ${CLI_DIR} ${GUI_DIR} ${GNOME_APPLET_DIR} ${DOC_DIR} share examples sessions ${PIXMAP_DIR} man

AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = Makefile.xnee Makefile.cvs autotools/* projects/test.xnp sessions/example1.xns README README.debian BUGS FAQ EXAMPLES doc/Makefile.am doc/Makefile.in ChangeLog 



devel-doc:
	-rm -fr src-html/
	mkdir src-html/
	DATE="$(shell date)" && \
	grep AM_INIT_AUTOMAKE configure.in |  sed -e 's/AM_INIT_AUTOMAKE//g' -e 's/\,[a-zA-Z0-9\.]*//g' -e 's,[()]*,,g'  && \
	XNEE_NAME="$(shell grep AM_INIT_AUTOMAKE configure.in |  sed -e 's/AM_INIT_AUTOMAKE//g' -e 's/\,[a-zA-Z0-9\.]*//g' -e 's,[()]*,,g') " && \
	XNEE_VERSION="$(shell grep AM_INIT_AUTOMAKE configure.in |  sed -e 's/AM_INIT_AUTOMAKE//g' -e 's/[a-zA-Z0-9\.]*\,//g' -e 's,[()]*,,g') " && \
	echo "Date: \"$$DATE\"  $$XNEE_NAME  $$XNEE_VERSION  "  && \
	cat tmpl/header.html.tmpl | sed \
	-e "s,PRODUCT_TMPL,$$XNEE_NAME (libxnee),g"  \
	-e "s,PRODUCT_VERSION_TMPL,$$XNEE_VERSION,g"  \
	-e "s,PRODUCT_BUILD_TMPL,$$DATE,g"  \
	-e "s,GNU_WWW_TMPL,http://www\.gnu\.org,g"  \
	-e "s,XNEE_WWW_TMPL,http://www\.gnu\.org/software/xnee,g"  \
	> libxnee/src/header.html 
	cat tmpl/header.html.tmpl | sed \
	-e 's,PRODUCT_TMPL,$$XNEE_NAME (cnee),g'  \
	-e 's,PRODUCT_VERSION_TMPL,$$XNEE_VERSION,g'  \
	-e 's,PRODUCT_BUILD_TMPL,$$DATE,g'  \
	-e "s,GNU_WWW_TMPL,http://www\.gnu\.org,g"  \
	-e "s,XNEE_WWW_TMPL,http://www\.gnu\.org/software/xnee,g"  \
	> cnee/src/header.html && \
	mkdir -p src-html/libxnee/src
	mkdir -p src-html/libxnee/include/libxnee
	mkdir -p src-html/cnee/src
	cd libxnee/src/     && ${MAKE} devel-doc 
#	cd cnee/src/ && ${MAKE} src-html && cp *.html ../../src-html/cnee/src


manual: 
	cd ./doc/ && $(MAKE) all manual

man:
	cd ./cnee/src && make man
	cd pnee/man && make
	cd gnee/man && make

test:
	cd libxnee/test && make clean all test


$(srcdir)/package.m4 : $(topdir)/configure.in
	:;{\
	echo '# ' && \
	echo 'm4_define([AT_PACKAGE_NAME],  [@PACKAGE_NAME@])' ; \
	} > '$$(srcdir)/package.m4'