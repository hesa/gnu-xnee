###info_TEXINFOS=xnee.texi
xnee_TEXINFOS= dev_version.texi xnee_devel_build.texi  xnee_devel_over.texi  xnee_devel_down.texi  xnee_develop.texi  xnee_copying fdl.texi

EXTRA_DIST = xnee.dox *.texi 

#MAKEINFO = @MAKEINFO@ --no-split
TEXI2HTML = texi2html
DVI2PS = dvi2ps
PS2PDF = ps2pdf

###   TEXI2HTML_FLAGS = -split_chapter

info_TEXINFOS = xnee_develop.texi

CLEANFILES = *.acs  *.cvs  *.mss  *.prs \
	     *.ats  *.evs  *.fns  *.ovs \
             *.tmp         *.html    \
             stamp-vti *.pdf     *.txt     *.info 
##
##  ALL
##
all: 

devel-doc: html info man txt pdf ps 
doc: devel-doc 

## ----------------------------- ##
## Other documentation formats.  ##
## ----------------------------- ##

html: xnee_develop.html

man:  

pdf:  xnee_develop.pdf  

info: xnee_develop.info 

txt:  xnee_develop.txt

ps:   xnee_develop.ps



# HTML
xnee_develop.html: xnee_develop.texi 
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) $(srcdir)/xnee_develop.texi


# PDF for RedHat
xnee_develop.pdf: xnee_develop.texi 
	-$(TEXI2DVI) --pdf --batch $(srcdir)/xnee_develop.texi

# PDF for Debian
#xnee_develop.pdf: xnee_develop.texi 
#	$(PS2PDF)  $(srcdir)/xnee_develop.ps


# DVI
xnee_develop.dvi:
	$(TEXI2DVI) --batch $(srcdir)/xnee_develop.texi

# PS for RedHat
xnee_develop.ps: xnee_develop.pdf
	pdf2ps $(srcdir)/xnee_develop.pdf

# PS for Debian
#xnee.ps: xnee.dvi
#	$(DVI2PS) -c xnee.ps $(srcdir)/xnee.dvi
#
#xnee_develop.ps: xnee_develop.dvi
#	$(DVI2PS) -c xnee_develop.ps  $(srcdir)/xnee_develop.dvi



xnee_develop.txt:
	makeinfo -o xnee_develop.txt --no-headers --no-split --no-validate xnee_develop.texi


api-doc: xnee.dox 
	\rm -fr alldocs/
	-mkdir alldocs
	-mkdir alldocs/devel
	\rm -fr latex
	@ echo generating API docs makefiles
	doxygen xnee.dox 
	@ echo generating API docs
	cd latex && make html ; make ps ; make pdf



.PHONY: html pdf text info man





