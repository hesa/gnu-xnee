docdir = $(datadir)/xnee

xnee_TEXINFOS=xnee_error.texi xnee_prot.texi version.texi  xnee_error.texi xnee_exampl.texi xnee_faq.texi xnee_install.texi xnee_intro.texi xnee_req.texi xnee_synch.texi xnee_usage.texi xnee_general.texi xnee_start.texi  xnee_intern.texi xnee_copying fdl.texi xnee_func.texi xnee_progs.texi 

EXTRA_DIST = xnee_copying fdl.texi *.dia xnee.dox

#MAKEINFO = @MAKEINFO@ --no-split
#TEXI2HTML = texi2html
#DVI2PS = dvi2ps
#PS2PDF = ps2pdf
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

GEN_IMAGES = $(IMG_EPS) $(IMG_PDF) $(IMG_PNG) $(IMG_JPG)
GEN_IMAGES_TO_INSTALL = $(IMG_PNG) $(IMG_JPG)
MANUALS = xnee.html xnee.pdf xnee.ps xnee.txt

doc_DATA = $(MANUALS) $(GEN_IMAGES_TO_INSTALL)


## ----------------------------- ##
## Other documentation formats.  ##
## ----------------------------- ##


# HTML
xnee.html: xnee.texi $(GEN_IMAGES)
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) $(srcdir)/xnee.texi


# PDF for RedHat
#xnee.pdf: xnee.texi 
#	-$(TEXI2DVI) --pdf --batch $(srcdir)/xnee.texi

# PDF for Debian
xnee.pdf: xnee.ps $(GEN_IMAGES)
	$(PS2PDF)  $(srcdir)/xnee.ps


# DVI
xnee.dvi: $(GEN_IMAGES)
	$(TEXI2DVI) --batch $(srcdir)/xnee.texi


# PS for RedHat
#xnee.ps: xnee.pdf
#	pdf2ps $(srcdir)/xnee.pdf

# PS for Debian
xnee.ps: xnee.dvi $(GEN_IMAGES)
	$(DVI2PS) -c xnee.ps $(srcdir)/xnee.dvi




%.eps:%.dia
	echo "creating EPS"
	$(DIA) --nosplash -e $@ $<

%.pdf: %.eps
	echo "creating PDF"
	$(EPSTOPDF) --outfile $@ $<

%.png: %.eps
	echo "creating PNG"
	$(CONVERT) -density 144x144 $< $@ 
	( mv $@ `echo $@ | sed 's,\.png,_big\.png,g'` )
	$(CONVERT) -density 32x32 $< $@ 
	( mv $@ `echo $@ | sed 's,\.png,_small\.png,g'` )
	$(CONVERT) -density 60x60 $< $@ 
%.jpg: %.eps
	echo "creating JPG"
	$(CONVERT) -density 144x144 $< $@ 
	( mv $@ `echo $@ | sed 's,\.jpg,_big\.jpg,g'` )
	$(CONVERT) -density 32x32 $< $@ 
	( mv $@ `echo $@ | sed 's,\.jpg,_small\.jpg,g'` )
	$(CONVERT) -density 60x60 $< $@ 


${IMG_EPS}: ${IMG_DIA}
${IMG_PDF}: ${IMG_EPS} 
${IMG_PNG}: ${IMG_EPS}
${IMG_JPG}: ${IMG_EPS}


api-doc: xnee.dox 
	\rm -fr alldocs/
	-mkdir alldocs
	-mkdir alldocs/devel
	\rm -fr latex
	$(DOXYGEN) xnee.dox 
	cd latex && make ps && make pdf



xnee.txt:
	$(MAKEINFO) -o xnee.txt --no-headers --no-split --no-validate xnee.texi

manual: $(MANUALS)
all: $(GEN_IMAGES) 
default: all


clean:  
	\rm -f *.pdf *.ps *.jpg *.png *.info


