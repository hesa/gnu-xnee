#!/bin/sh

make -f Makefile.cvs && ./configure --disable-xinput --disable-doc && make && make check

echo "Xnee built well on: $(uname -a)"
