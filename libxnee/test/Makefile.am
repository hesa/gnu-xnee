LIBTEST = libtest
TESTSETGET = testsetget
TESTFEEDBACK = testfeedback
TESTPRINT = testprint
TESTDISP = testdisplay

noinst_PROGRAMS = $(LIBTEST) $(TESTSETGET) $(TESTFEEDBACK) $(TESTPRINT) $(TESTDISP)

EXTRA_DIST = test_setget.h


libtest_SOURCES = libtest.c
testsetget_SOURCES = test_setget.c test_all.c 
testfeedback_SOURCES = test_feedback.c
testprint_SOURCES = print.c
testdisplay_SOURCES = disp.c

LIBXNEE_DIR = ../

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif


AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG}
AM_LDFLAGS =   -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS)
AM_LDADD   =   -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS)

test: 
	./$(LIBTEST)
	./$(TESTSETGET)
	./$(TESTFEEDBACK)
	./$(TESTPRINT)

