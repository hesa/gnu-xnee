#!/bin/sh

# TODO
#
# * Add make check
#

make -f Makefile.cvs && ./configure --disable-xinput2 --disable-doc --disable-gnome-applet --disable-gui && make 
RET=$?

if [ "$RET" = "0" ]
then
    echo "Xnee built well at $(date) on $(uname -a)"
else
    echo "Xnee built failed at $(date) on $(uname -a)"
fi