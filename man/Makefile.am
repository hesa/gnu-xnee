XNEE_MAN_PAGE=xnee.1
man_MANS = $(XNEE_MAN_PAGE)

EXTRA_DIST=$(man_MANS) $(man_MANS).in
CLEAN_FILES=$(man_MANS)

$(XNEE_MAN_PAGE): $(XNEE_MAN_PAGE).in
	cat $(XNEE_MAN_PAGE).in | sed "s/__VERSION_TAG__/$(VERSION)/g" > $(XNEE_MAN_PAGE)


all:
