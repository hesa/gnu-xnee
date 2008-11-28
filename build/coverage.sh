#!/bin/sh

DATE=$(date '+%Y%m%d')
DATE=$(date '+%Y%m%d')

if [ "$1" = "" ]
then
    BASE_DEST_DIR=/tmp/xnee-temp
else
    BASE_DEST_DIR=$1
fi

DEST_DIR=$BASE_DEST_DIR/coverage/$DATE/x86
mkdir -p $DEST_DIR

#
# current version is nothing but a hack to try out some conecpts
#

./configure \
  --disable-gui \
  --disable-gnome-applet \
  --disable-doc            && \
make clean                 && \
make -f GNUMakefile coverage    && \
mkdir $DEST_DIR ; \
cp doc/coverage/* $DEST_DIR/


