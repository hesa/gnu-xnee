AUTOMAKE_OPTIONS = gnu


LIBXNEE_H_FILES=../include/libxnee/xnee.h     \
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
	../include/libxnee/xnee_strings.h \
	../include/libxnee/xnee_session.h \
	../include/libxnee/xnee_utils.h \
	../include/libxnee/xnee_alloc.h \
	../include/libxnee/xnee_fileop.h \
	../include/libxnee/xnee_display.h \
	../include/libxnee/xnee_settings.h \
	../include/libxnee/xnee_plugin.h \
	../include/libxnee/xnee_internal.h \
	../include/libxnee/xnee_window.h \
	../include/libxnee/x11_files.h \
	../include/libxnee/feedback.h  \
	../include/libxnee/xnee_xinput.h

LIB_XNEE_SOURCE_FILES = xnee.c  \
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
	xnee_strings.c \
	xnee_session.c \
	xnee_utils.c \
	xnee_alloc.c \
	xnee_fileop.c \
	xnee_display.c \
	xnee_settings.c \
	xnee_plugin.c \
	xnee_window.c \
	feedback.c \
	xnee_xinput.c \
	$(LIBXNEE_H_FILES)

libxnee_la_SOURCES = $(LIB_XNEE_SOURCE_FILES)
EXTRA_LTLIBRARIES = libxnee.la
lib_LTLIBRARIES = $(WANTED_LIBXNEE)
libxnee_la_LDFLAGS = -rpath '$(libdir)'
#dnl libxnee_la_SOURCES = $(LIB_XNEE_SOURCE_FILES) 
libxnee_a_CFLAGS = $(AM_CFLAGS)

#noinst_LIBRARIES = libxnee.a 

noinst_LTLIBRARIES = $(WANTED_LIBXNEE)


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
PEDANTIC_FLAGS =  -W -Wall -pedantic -pedantic-errors  -Werror -fno-strict-aliasing -Wno-unused-parameter 
else
PEDANTIC_FLAGS = 
endif

if XNEE_XINPUT_SUPPORT
XNEE_XINPUT_SUPPORT_FLAGS = -DXNEE_XINPUT_SUPPORT
else
XNEE_XINPUT_SUPPORT_FLAGS =
endif



if NOT_INSTALL_LIBS
install-exec-local:
	echo " ... not installing libs"
install:
else
install-exec-local:
	echo " ... installing libs"
endif

AM_CFLAGS = -I../include ${X_CFLAGS} ${PEDANTIC_FLAGS} -g ${VERBOSE_FLAG} ${GCOV_FLAG} ${GPROF_FLAG} ${BUF_VERBOSE_FLAG}  ${XOSD_FLAGS} ${DEBUG_FLAGS}  ${XNEE_XINPUT_SUPPORT_FLAGS} 

HTML_SRC_FILES = $(libxnee_a_SOURCES:%.c=%.c.html)
HTML_INC_FILES = $(LIBXNEE_H_FILES:%.h=%.h.html)

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

