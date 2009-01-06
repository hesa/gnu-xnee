CNEE=cnee
bin_PROGRAMS = $(CNEE)
cnee_SOURCES = cnee_fake.c  cnee_printer.c  cnee_strings.c  cnee_demo.c  main.c  parse.c  ../include/cnee_fake.h  ../include/cnee_demo.h ../include/cnee_printer.h  ../include/cnee_strings.h ../include/parse.h  ../include/cnee.h 
man_MANS = cnee.1

BUILT_SOURCES = cnee.texi 
CLEAN_FILES   = cnee.info cnee.texi

doc_DATA = $(CNEE_INFO)
docdir = $(datadir)/info

texi_TEXINFOS = cnee.texi 
#info_TEXINFOS = cnee.texi


EXTRA_DIST=$(man_MANS) $(texi_TEXINFOS)


LIBXNEE_DIR = ../../libxnee

if VERBOSE
VERBOSE_FLAG = -DUSE_VERBOSE
else
VERBOSE_FLAG = -DNO_VERBOSE
endif

if GCOV
GCOV_FLAG = -fprofile-arcs -ftest-coverage  -g
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


AM_CFLAGS = -I../include -I${LIBXNEE_DIR}/include  ${PEDANTIC_FLAGS}  ${X_CFLAGS}  -g ${VERBOSE_FLAG} ${BUF_VERBOSE_FLAG}  ${USER_CFLAGS} ${GCOV_FLAG} ${GPROF_FLAG} 
cnee_LDADD =   -L${LIBXNEE_DIR}/src -lxnee ${X_LIBS} ${X11_LIBS} ${LIBDL} 

if BUILD_STATIC
STATIC_FLAGS = -static
else
STATIC_FLAGS = 
endif

cnee_LDFLAGS = ${GCOV_FLAG} ${GPROF_FLAG} $(STATIC_FLAGS)

cnee_DEPENDENCIES = 

verify-cli:
	./cnee --version && echo "cnee seems to be ok"

test-cli:
	(cd test ; ./test_all.sh)

#.info.texi:
#	makeinfo 

info: $(CNEE_INFO)

man: ./$(CNEE)$(EXEEXT)
	@echo "Generating man page file from: $(CNEE)$(EXEEXT)"
	./$(CNEE)$(EXEEXT) --manpage > cnee.1
	@echo "Generating texi file from: $(CNEE)$(EXEEXT)"
	./$(CNEE)$(EXEEXT) --texipage > cnee.texinfo
	@echo "Generating info file from cnee.texi"
	$(MAKEINFO) cnee.info


cnee.1: $(CNEE)$(EXEEXT)
	./$(CNEE)$(EXEEXT) --manpage > $@

cnee.texi: $(CNEE)$(EXEEXT)
	@echo "Generating texi file from: $(CNEE)$(EXEEXT)"
	./$(CNEE)$(EXEEXT) --texipage > $@

cnee.info: $(texi_TEXINFOS)
	@echo "Generating info file from cnee.texi"
	$(MAKEINFO) $< 

cnee.html:  $(texi_TEXINFOS)
	@echo "Generating html file from cnee.texi"
	$(TEXI2HTML) $< 

cnee.pdf:  $(texi_TEXINFOS)
	@echo "Generating pdf file from cnee.texi"
	$(TEXI2PDF) $< 

cnee.txt:  $(texi_TEXINFOS)
	@echo "Generating txt file from cnee.texi"
	man ./cnee.1 | col -b > $@