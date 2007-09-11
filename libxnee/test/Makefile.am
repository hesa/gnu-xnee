LIBTEST = libtest
TESTSETGET = testsetget

noinst_PROGRAMS = $(LIBTEST) $(TESTSETGET)

EXTRA_DIST = test_setget.h


libtest_SOURCES = libtest.c
testsetget_SOURCES = test_setget.c test_all.c 

LIBXNEE_DIR = ../

AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG}

libtest_LDADD =   -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} 
libtest_LDFLAGS = ${GCOV_FLAG} ${GPROF_FLAG} -static
libtest_DEPENDENCIES = ${LIBXNEE_DIR}/src/libxnee.a

testsetget_LDADD =   -L${LIBXNEE_DIR}/src -lxnee  ${X_LIBS} ${X11_LIBS} ${LIBDL}  
testsetget_LDFLAGS = ${GCOV_FLAG} ${GPROF_FLAG} 
testsetget_DEPENDENCIES = ${LIBXNEE_DIR}/src/libxnee.a

test:
	./$(LIBTEST)
	./$(TESTSETGET)


