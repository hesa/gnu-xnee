ICONS = xnee.xpm xnee.png

icon_DATA = $(ICONS)
icondir = $(datadir)/pixmaps

EXTRA_DIST = $(ICONS) 

%.png:%.svg
	$(INKSCAPE) --export-png=$@ $<

%.xpm:%.png
	$(CONVERT) $< $@
