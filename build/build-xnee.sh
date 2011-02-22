#!/bin/sh

make -f Makefile.cvs && ./configure --disable-xinput --disable-doc && make && make check
RET=$?

if [ "$RET" = "0" ]
then
    echo "Xnee built well at $(date) on $(uname -a)"
else
    echo "Xnee built failed at $(date) on $(uname -a)"
fi