AUTOMAKE_OPTIONS = gnu

lib_LIBRARIES = libxnee.a
libxnee_a_SOURCES = xnee.c print.c print_varargs.c xnee_record.c xnee_replay.c  datastrings.c xnee_sem.c xnee_dl.c

EXTRA_DIST = ../include/libxnee/xnee.h ../include/libxnee/print.h ../include/libxnee/print_varargs.h ../include/libxnee/xnee_record.h ../include/libxnee/xnee_replay.h ../include/libxnee/datastrings.h ../include/libxnee/xnee_sem.h  ../include/libxnee/xnee_dl.h 

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

CFLAGS = -I../include ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${GCOV_FLAG} ${GPROF_FLAG} ${BUF_VERBOSE_FLAG} 
