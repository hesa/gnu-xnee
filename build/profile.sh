#!/bin/sh

#
# current version is nothing but a hack to try out some conecpts
#

FMT=pdf
PATH=$PATH:/opt/gprof2dot
DATE=$(date '+%Y%m%d')


if [ "$1" = "" ]
then
    BASE_DEST_DIR=/tmp/xnee-temp
else
    BASE_DEST_DIR=$1
fi

DEST_DIR=$BASE_DEST_DIR/profile/$DATE/x86
mkdir -p $DEST_DIR

mkdir doc/profile

./configure \
  --disable-gui \
  --disable-gnome-applet \
  --disable-doc          \
  --enable-gprof               && \
make clean all check           && \
cd cnee/src                        && \
mv ../test/gmon.out* ./ ; \
rm -f gmon.out.txt  gmon.sum       && \
gprof -s cnee gmon.ou*             && \
gprof cnee gmon.sum > gmon.out.txt && \
gprof2dot.pl    \
   --transformer transformer.pl \
   --output cnee.dot --cluster-by-scope --merge-overloaded-functions \
   gmon.out.txt                && \
dot -T$FMT -o cnee.$FMT cnee.dot &&\
mv cnee.$FMT ../../doc/profile && \
cd ../.. && \
cp doc/profile/cnee.$FMT $DEST_DIR/cnee-call-graph.$FMT && \
cp cnee/src/gmon.out.txt $DEST_DIR/ && \
echo "OK :)"



#mogrify -rotate 90 doc/profile/cnee.$FMT &&\
