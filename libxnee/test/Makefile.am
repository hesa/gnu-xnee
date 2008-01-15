LIBTEST = libtest
TESTSETGET = testsetget

noinst_PROGRAMS = $(LIBTEST) $(TESTSETGET)

EXTRA_DIST = test_setget.h


libtest_SOURCES = libtest.c
testsetget_SOURCES = test_setget.c test_all.c 

LIBXNEE_DIR = ../

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif


AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG}

libtest_LDADD =   -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} 
libtest_LDFLAGS = ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS)
libtest_DEPENDENCIES = 

testsetget_LDADD =   -L${LIBXNEE_DIR}/src -lxnee  ${X_LIBS} ${X11_LIBS} ${LIBDL}  
testsetget_LDFLAGS = ${GCOV_FLAG} ${GPROF_FLAG} 
testsetget_DEPENDENCIES = 

test: 
	./$(LIBTEST)
	./$(TESTSETGET)


