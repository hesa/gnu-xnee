AUTOMAKE_OPTIONS = gnu

lib_LIBRARIES = libxnee.a

libxnee_INCLUDES = -I../include 

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
	xnee_range.c \
	xnee_expr.c \
	xnee_error.c \
	xnee_keysym.c \
	feedback.c 

EXTRA_DIST = ../include/libxnee/xnee.h     \
	../include/libxnee/print.h         \
	../include/libxnee/xnee_error.h    \
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
	../include/libxnee/xnee_range.h \
	../include/libxnee/xnee_expr.h \
	../include/libxnee/xnee_keysym.h \
	../include/libxnee/feedback.h 


if VERBOSE
VERBOSE_FLAG = -DUSE_VERBOSE
else
VERBOSE_FLAG = -DNO_VERBOSE
endif

if GCOV
GCOV_FLAG = -fprofile-arcs -ftest-coverage -g
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

libxnee_a_CFLAGS = -I../include ${X_CFLAGS} ${PEDANTIC_FLAGS} -g ${VERBOSE_FLAG} ${GCOV_FLAG} ${GPROF_FLAG} ${BUF_VERBOSE_FLAG}  ${XOSD_FLAGS}  



HTML_SRC_FILES = $(libxnee_a_SOURCES:%.c=%.c.html)
HTML_INC_FILES = $(EXTRA_DIST:%.h=%.h.html)

%.c.html:%.c
	$(SRC_HIGHLIGHT)  -f html -n  --line-number-ref -d -t "$<" -H header.html   $<

%.h.html:%.h
	$(SRC_HIGHLIGHT)  -f html -n  --line-number-ref -d -t "$<" -H header.html   $<

#${HTML_SRC_FILES}:
#	$(SRC_HIGHLIGHT)  -f html -n  --line-number-ref -d -t "$<" -H header.html   $<

devel-doc-inc: ${HTML_INC_FILES}
	cp ${HTML_INC_FILES} ../../src-html/libxnee/include/libxnee

devel-doc-src: ${HTML_SRC_FILES}
	cp ${HTML_SRC_FILES} ../../src-html/libxnee/src


testnames:
	echo "HTML_INC_FILES=${HTML_INC_FILES}"

devel-doc: devel-doc-src devel-doc-inc
