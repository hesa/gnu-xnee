###info_TEXINFOS=xnee.texi
xnee_TEXINFOS=xnee_error.texi xnee_prot.texi version.texi  xnee_error.texi xnee_exampl.texi xnee_faq.texi xnee_install.texi xnee_intro.texi xnee_req.texi xnee_synch.texi xnee_usage.texi xnee_general.texi xnee_start.texi  xnee_intern.texi xnee_copying fdl.texi

EXTRA_DIST = xnee_copying fdl.texi pics/*.dia pics/*.sh 


#MAKEINFO = @MAKEINFO@ --no-split
TEXI2HTML = texi2html
DVI2PS = dvi2ps
PS2PDF = ps2pdf

###   TEXI2HTML_FLAGS = -split_chapter

info_TEXINFOS = xnee.texi 

CLEANFILES = xnee.acs  xnee.cvs  xnee.mss  xnee.prs \
	     xnee.ats  xnee.evs  xnee.fns  xnee.ovs \
             xnee.tmp  *.1       *.html    \
             stamp-vti *.pdf     *.txt     *.info \
             */*.png */*.eps */*.pdf *.png *.eps *.pdf 


IMAGES = xndetail.eps xngener.eps xnrec.eps xnrep.eps xnswinp.eps \
         xndetail.pdf xngener.pdf xnrec.pdf xnrep.pdf xnswinp.pdf \
         xndetail.png xngener.png xnrec.png xnrep.png xnswinp.png

##	
##  ALL
##
all: doc 

doc:  images html info txt pdf ps

images: ${IMAGES}

DOC_DELIV=xnee-doc-${VERSION}

${IMAGES}: pics/xndetail.dia pics/xngener.dia pics/xnrec.dia pics/xnrep.dia pics/xnswinp.dia
	echo "Creating pics"
	( cd pics ; ./dia_gen.sh ; cp *.png *.pdf *.eps ../ )

doc-deliv: doc
	-rm ${DOC_DELIV}.tar.gz
	tar cvf ${DOC_DELIV}.tar *.png xnee.pdf xnee.ps xnee.html xnee.txt
	gzip ${DOC_DELIV}.tar


## ----------------------------- ##
## Other documentation formats.  ##
## ----------------------------- ##

html: xnee.html 

man:  xnee.1

pdf:  xnee.pdf 

info: xnee.info 

txt:  xnee.txt  

ps:   xnee.ps 



# HTML
xnee.html: xnee.texi 
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) $(srcdir)/xnee.texi


# PDF for RedHat
#xnee.pdf: xnee.texi 
#	-$(TEXI2DVI) --pdf --batch $(srcdir)/xnee.texi

# PDF for Debian
xnee.pdf: xnee.ps 
	$(PS2PDF)  $(srcdir)/xnee.ps


# DVI
xnee.dvi:
	$(TEXI2DVI) --batch $(srcdir)/xnee.texi


# PS for RedHat
#xnee.ps: xnee.pdf
#	pdf2ps $(srcdir)/xnee.pdf

# PS for Debian
xnee.ps: xnee.dvi
	$(DVI2PS) -c xnee.ps $(srcdir)/xnee.dvi




api-doc: xnee.dox 
	\rm -fr alldocs/
	-mkdir alldocs
	-mkdir alldocs/devel
	\rm -fr latex
	doxygen xnee.dox 
	cd latex && make ps && make pdf



xnee.txt:
	makeinfo -o xnee.txt --no-headers --no-split --no-validate xnee.texi


