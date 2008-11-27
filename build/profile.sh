#!/bin/sh

#
# current version is nothing but a hack to try out some conecpts
#

FMT=pdf
PATH=$PATH:/opt/gprof2dot

mkdir doc/profile

./configure \
  --disable-gui \
  --disable-gnome-applet \
  --disable-doc                && \
make clean all check           && \
cd cnee/src                    && \
gprof cnee > gmon.out.txt      && \
gprof2dot.pl    \
   --transformer transformer.pl \
   --output cnee.dot --cluster-by-scope --merge-overloaded-functions \
   gmon.out.txt                && \
dot -T$FMT -o cnee.$FMT cnee.dot &&\
mv cnee.$FMT ../../doc/profile && \
cd ../.. && \
mogrify -rotate 90 doc/profile/cnee.pdf &&\
echo "OK :)"

