bin_PROGRAMS = cnee
cnee_SOURCES = main.c parse.c parse.h 

LIBXNEE_DIR="../../libxnee"


cnee_CFLAGS = -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${GCOV_FLAG} ${GPROF_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS}

cnee_LDADD = -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} $(LIBSEMA)


test-cli:
	(cd test ; ./test_all.sh)

man: cnee.1

cnee.1: cnee
	./cnee --manpage > $@
