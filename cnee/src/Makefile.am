bin_PROGRAMS = cnee
cnee_SOURCES = cnee_fake.c  cnee_printer.c  cnee_strings.c  cnee_demo.c  main.c  parse.c  cnee_demo.c
man_MANS = cnee.1

BUILT_SOURCES = cnee.info cnee.texi
CLEAN_FILES   = cnee.info cnee.texi

texi_TEXINFOS = cnee.texi
#info_TEXINFOS = cnee.texi

EXTRA_DIST=$(man_MANS) ../include/cnee_fake.h  ../include/cnee_demo.h ../include/cnee_printer.h  ../include/cnee_strings.h ../include/parse.h 



cnee.texi: cnee$(EXEEXT)
	./cnee --texipage > $@

cnee.info: cnee.texi
	$(MAKEINFO) $< 

LIBXNEE_DIR = ../../libxnee

if VERBOSE
VERBOSE_FLAG = -DUSE_VERBOSE
else
VERBOSE_FLAG = -DNO_VERBOSE
endif

if GCOV
GCOV_FLAG = -fprofile-arcs -ftest-coverage  -g
else
GCOV_FLAG = 
endif

if GPROF
GPROF_FLAG = -pg -g
else
GPROF_FLAG = 
endif

if XOSD
XOSD_FLAGS = -DHAVE_XOSD
else
XOSD_FLAGS = -DNO_XOSD
endif

if BUF_VERBOSE
BUF_VERBOSE_FLAG = -DUSE_BUF_VERBOSE
else
BUF_VERBOSE_FLAG = -DNO_BUF_VERBOSE
endif

if PEDANTIC
PEDANTIC_FLAGS =  -W -Wall -pedantic -pedantic-errors  -Werror
else
PEDANTIC_FLAGS = 
endif


AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG}

cnee_LDADD =   -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} $(LIBSEMA)   

cnee_LDFLAGS = ${GCOV_FLAG} ${GPROF_FLAG}

cnee_DEPENDENCIES = ${LIBXNEE_DIR}/src/libxnee.a

test-cli:
	(cd test ; ./test_all.sh)

.info.texi:
	makeinfo 

info: cnee.info 

man: cnee.1

cnee.1: cnee
	./cnee --manpage > $@
