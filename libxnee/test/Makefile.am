LIBTEST = libtest
TESTSETGET = testsetget
TESTFEEDBACK = testfeedback
TESTPRINT = testprint
TESTDISP = testdisplay
TESTCALLBACK = testcallback
lib_LTLIBRARIES = libtestcb.la

noinst_PROGRAMS = $(LIBTEST) $(TESTSETGET) $(TESTFEEDBACK) $(TESTPRINT) $(TESTDISP) $(TESTCALLBACK)

EXTRA_DIST = test_setget.h

AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG}
AM_LDFLAGS =  -L../src/.libs/  -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS) -shared
AM_LDADD   =  -L../src/.libs/ -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS)


libtest_SOURCES = libtest.c
testsetget_SOURCES = test_setget.c test_all.c 
testfeedback_SOURCES = test_feedback.c
testprint_SOURCES = print.c
testdisplay_SOURCES = disp.c
testcallback_SOURCES = callback.c
libtestcb_la_SOURCES = callback_so.c
libtestcb_la_CFLAGS = -shared $(AM_CFLAGS)

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif


test: 
	./$(LIBTEST)
	./$(TESTSETGET)
	./$(TESTFEEDBACK)
	./$(TESTPRINT)
	./$(TESTCALLBACK)

