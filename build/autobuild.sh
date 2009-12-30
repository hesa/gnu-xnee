#!/bin/sh

XNEE_OUTPUT_FILE=-xnee.txt
XNEE_OUTPUT_FILE_H=xnee.html

rm $XNEE_OUTPUT_FILE

# make sure we're in the correct dir
#   ... well, it's not bullet proof
if [ ! -f Makefile.cvs ]
then
    echo "Make sure you're in the root dir"
    exit
fi

make clean 
./configure --disable-gui --disable-gnome-applet > ${XNEE_OUTPUT_FILE}
make check >>  ${XNEE_OUTPUT_FILE}
htmlize ${XNEE_OUTPUT_FILE}.html

autobuild --verbose ${XNEE_OUTPUT_FILE}.html > ${XNEE_OUTPUT_FILE_H}
