LIBTEST = libtest
TESTSETGET = testsetget
TESTFEEDBACK = testfeedback
TESTPRINT = testprint
TESTDISP = testdisplay
TESTCALLBACK = testcallback
lib_LTLIBRARIES = libtestcb.la

noinst_PROGRAMS = $(LIBTEST) $(TESTSETGET) $(TESTFEEDBACK) $(TESTPRINT) $(TESTDISP) $(TESTCALLBACK)
TEST_WRAPPER=./test-wrapper.sh

EXTRA_DIST = test_setget.h $(TEST_WRAPPER)

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif

AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG}
AM_LDFLAGS =  -L../src/.libs/  -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS) 
AM_LDADD   =  -L../src/.libs/ -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS)


libtest_SOURCES = libtest.c
testsetget_SOURCES = test_setget.c test_all.c 
testfeedback_SOURCES = test_feedback.c
testprint_SOURCES = print.c
testdisplay_SOURCES = disp.c
testcallback_SOURCES = callback.c
libtestcb_la_SOURCES = callback_so.c
libtestcb_la_CFLAGS =  $(AM_CFLAGS)

test: 
	chmod a+x ./test-wrapper.sh 
	$(TEST_WRAPPER) ./$(LIBTEST)
	$(TEST_WRAPPER) ./$(TESTSETGET)
	$(TEST_WRAPPER) ./$(TESTFEEDBACK)
	$(TEST_WRAPPER) ./$(TESTPRINT)
	$(TEST_WRAPPER) ./$(TESTCALLBACK)

xtest: 
	./$(LIBTEST)
	./$(TESTSETGET)
	./$(TESTFEEDBACK)
	./$(TESTPRINT)
	./$(TESTCALLBACK)

