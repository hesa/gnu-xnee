###info_TEXINFOS=xnee.texi
xnee_TEXINFOS=xnee_error.texi version.texi  xnee_error.texi xnee_exampl.texi xnee_faq.texi xnee_install.texi xnee_intro.texi xnee_req.texi xnee_synch.texi xnee_usage.texi   xnee_copying fdl.texi

EXTRA_DIST = xnee_copying fdl.texi xnee*.html xnee.txt xnee.1 xnee.info 



man1_MANS=xnee.1

#MAKEINFO = @MAKEINFO@ --no-split
TEXI2HTML = texi2html
DVI2PS = dvi2ps
PS2PDF = ps2pdf

###   TEXI2HTML_FLAGS = -split_chapter

info_TEXINFOS = xnee.texi 

CLEANFILES = xnee.acs  xnee.cvs  xnee.mss  xnee.prs \
	     xnee.ats  xnee.evs  xnee.fns  xnee.ovs \
             xnee.tmp  *.1       *.html    \
             stamp-vti *.pdf     *.txt     *.info 
##
##  ALL
##
all: xnee.1
doc: html man info txt pdf ps


## ----------------------------- ##
## Other documentation formats.  ##
## ----------------------------- ##

html: xnee.html 

man:  xnee.1

pdf:  xnee.pdf 

info: xnee.info 

txt:  xnee.txt xnee_man.txt 

ps:   xnee.ps 



# HTML
xnee.html: xnee.texi 
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) $(srcdir)/xnee.texi


# MAN
xnee.1: ../src/xnee
	../src/xnee --manpage > xnee.1

# MAN_TXT
xnee_man.txt: xnee.1
	nroff -man < xnee.1 | perl -pe 's/\cH.//g' | uniq > xnee_man.txt

# PDF for RedHat
xnee.pdf: xnee.texi 
	-$(TEXI2DVI) --pdf --batch $(srcdir)/xnee.texi

# PDF for Debian
#xnee.pdf: xnee.ps 
#	$(PS2PDF)  $(srcdir)/xnee.ps


# DVI
xnee.dvi:
	$(TEXI2DVI) --batch $(srcdir)/xnee.texi


# PS for RedHat
xnee.ps: xnee.pdf
	pdf2ps $(srcdir)/xnee.pdf

# PS for Debian
#xnee.ps: xnee.dvi
#	$(DVI2PS) -c xnee.ps $(srcdir)/xnee.dvi
#



api-doc: xnee.dox 
	\rm -fr alldocs/
	-mkdir alldocs
	-mkdir alldocs/devel
	\rm -fr latex
	doxygen xnee.dox 
	cd latex && make ps && make pdf



xnee.txt:
	makeinfo -o xnee.txt --no-headers --no-split --no-validate xnee.texi


.PHONY: html pdf text info man





