bin_PROGRAMS = xnee
xnee_SOURCES = main.c parse.c parse.h 
xnee_LDADD = ../../libxnee/src/libxnee.a  ${X_LIBS} -lXext  -lXtst -lX11 -lpthread -ldl

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

if BUF_VERBOSE
BUF_VERBOSE_FLAG = -DUSE_BUF_VERBOSE
else
BUF_VERBOSE_FLAG = -DNO_BUF_VERBOSE
endif



CFLAGS = -I../../libxnee/include ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${GCOV_FLAG} ${GPROF_FLAG} ${BUF_VERBOSE_FLAG}
