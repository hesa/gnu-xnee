bin_PROGRAMS = cnee
cnee_SOURCES = main.c parse.c parse.h 

if HAVE_LIBXNEE_DIR
LIBXNEE_DIR=@libxnee_dir@
else
LIBXNEE_DIR="../../libxnee"
endif

cnee_LDADD =  -L${LIBXNEE_DIR}/src  -lxnee ${X_LIBS} -lXext  -lXtst -lX11 -lpthread -ldl

if VERBOSE
VERBOSE_FLAG = -DUSE_VERBOSE
else
VERBOSE_FLAG = -DNO_VERBOSE
endif

if GCOV
GCOV_FLAG = -fprofile-arcs -ftest-coverage 
else
GCOV_FLAG = 
endif

if GPROF
GPROF_FLAG = -pg
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

CFLAGS =  -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${GCOV_FLAG} ${GPROF_FLAG} ${BUF_VERBOSE_FLAG}  ${XOSD_FLAGS}
