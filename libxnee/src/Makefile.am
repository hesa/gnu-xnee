AUTOMAKE_OPTIONS = gnu

lib_LIBRARIES = libxnee.a
libxnee_a_SOURCES = xnee.c  \
	print.c  \
	print_varargs.c  \
	xnee_record.c  \
	xnee_replay.c   \
	datastrings.c  \
	xnee_sem.c  \
	xnee_dl.c \
	xnee_setget.c \
	xnee_resolution.c \
	xnee_fake.c \
	xnee_km.c \
	xnee_grab.c \
	xnee_resource.c \
	xnee_time.c \
	xnee_callback.c \
	xnee_buffer.c   \
	xnee_threshold.c \
	feedback.c 

EXTRA_DIST = ../include/libxnee/xnee.h     \
	../include/libxnee/print.h         \
	../include/libxnee/print_varargs.h \
	../include/libxnee/xnee_record.h   \
	../include/libxnee/xnee_replay.h   \
	../include/libxnee/datastrings.h   \
	../include/libxnee/xnee_sem.h      \
	../include/libxnee/xnee_dl.h       \
	../include/libxnee/xnee_setget.h   \
	../include/libxnee/xnee_resolution.h  \
	../include/libxnee/xnee_resource.h \
	../include/libxnee/xnee_time.h     \
	../include/libxnee/xnee_km.h       \
	../include/libxnee/xnee_callback.h \
	../include/libxnee/xnee_buffer.h   \
	../include/libxnee/xnee_fake.h     \
	../include/libxnee/xnee_grab.h     \
	../include/libxnee/xnee_threshold.h \
	../include/libxnee/feedback.h 


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
