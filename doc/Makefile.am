###info_TEXINFOS=xnee.texi
xnee_TEXINFOS=xnee_error.texi xnee_prot.texi version.texi  xnee_error.texi xnee_exampl.texi xnee_faq.texi xnee_install.texi xnee_intro.texi xnee_req.texi xnee_synch.texi xnee_usage.texi xnee_general.texi xnee_start.texi  xnee_intern.texi xnee_copying fdl.texi

EXTRA_DIST = xnee_copying fdl.texi *.dia 


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




IMG_DIA = xndetail.dia xngener.dia xnrec.dia xnrep.dia xnswinp.dia 
IMG_EPS = xndetail.eps xngener.eps xnrec.eps xnrep.eps xnswinp.eps 
IMG_PDF = xndetail.pdf xngener.pdf xnrec.pdf xnrep.pdf xnswinp.pdf 
IMG_PNG = xndetail.png xngener.png xnrec.png xnrep.png xnswinp.png
IMG_JPG = xndetail.jpg xngener.jpg xnrec.jpg xnrep.jpg xnswinp.jpg

GEN_IMAGES = ${IMG_EPS} ${IMG_PDF} ${IMG_PNG} ${IMG_JPG}
SRC_IMAGES = ${IMG_DIA}
##	
##  ALL
##

doc:  ${DOC_TARGETS}


images: ${SRC_IMAGES} ${GEN_IMAGES}

%.jpg: %.eps




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
xnee.html: xnee.texi images
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) $(srcdir)/xnee.texi


# PDF for RedHat
#xnee.pdf: xnee.texi 
#	-$(TEXI2DVI) --pdf --batch $(srcdir)/xnee.texi

# PDF for Debian
xnee.pdf: xnee.ps 
	$(PS2PDF)  $(srcdir)/xnee.ps


# DVI
xnee.dvi: images
	$(TEXI2DVI) --batch $(srcdir)/xnee.texi


# PS for RedHat
#xnee.ps: xnee.pdf
#	pdf2ps $(srcdir)/xnee.pdf

# PS for Debian
xnee.ps: xnee.dvi
	$(DVI2PS) -c xnee.ps $(srcdir)/xnee.dvi

xnee.1: ../cnee/src/cnee
	../cnee/src/cnee --manpage > $@


api-doc: xnee.dox 
	\rm -fr alldocs/
	-mkdir alldocs
	-mkdir alldocs/devel
	\rm -fr latex
	doxygen xnee.dox 
	cd latex && make ps && make pdf



xnee.txt:
	makeinfo -o xnee.txt --no-headers --no-split --no-validate xnee.texi


all: doc

#all: ${SRC_IMAGES} ${GEN_IMAGES}

${IMG_EPS}: %.eps: %.dia
	dia --nosplash -e $@ $<

${IMG_PDF}: %.pdf: %.eps
	epstopdf --outfile $@ $<

${IMG_PNG}: %.png: %.eps
	convert -density 144x144 $< $@ 
	( mv $@ `echo $@ | sed 's,\.png,_big\.png,g'` )
	convert -density 32x32 $< $@ 
	( mv $@ `echo $@ | sed 's,\.png,_small\.png,g'` )
	convert -density 60x60 $< $@ 

${IMG_JPG}: %.jpg: %.eps
	convert -density 144x144 $< $@ 
	( mv $@ `echo $@ | sed 's,\.jpg,_big\.jpg,g'` )
	convert -density 32x32 $< $@ 
	( mv $@ `echo $@ | sed 's,\.jpg,_small\.jpg,g'` )
	convert -density 60x60 $< $@ 

clean:
	\rm -f ${GEN_IMAGES} *_big.* *_small.*
