#!/bin/sh

#
# current version is nothing but a hack to try out some conecpts
#

FMT=pdf
PATH=$PATH:/opt/gprof2dot
DATE=$(date '+%Y-%m-%d')

if [ "$1" = "" ]
then
    BASE_DEST_DIR=/tmp/xnee-temp
else
    BASE_DEST_DIR=$1
fi


init_test()
{
    for i in $(find -name "gmon.out*" )
    do
	rm $i
    done
#
#  Setup virtual X environment
#
    Xvfb -ac -screen 0 640x480x16 :32  2> /dev/null  > /dev/null &
    XVFB_PID=$!
    
    
    DISPLAY=:32
    sleep 2
    

    DEST_DIR=$BASE_DEST_DIR/profile/$DATE/x86
    mkdir -p $DEST_DIR
    
}

gen_x11_data()
{
    TMP_CNT=0 ; 
    while (true) ; 
    do 

	echo "Fake X11 data round: $TMP_CNT"
	xterm -e sleep 1  2> /dev/null  > /dev/null; 
	sleep 0
	xdpyinfo 2> /dev/null > /dev/null
	sleep 0
	xwininfo -root 2> /dev/null > /dev/null
	sleep 0
	xset r on 2> /dev/null > /dev/null

	# check if we've lost the X server
	RET=$? ; 

	if [ $RET -ne 0 ] ; 
	then
	    echo  "NOK, date: $(date)   time=$TMP";  
	    break ; 
	fi ; 

	TMP_CNT=$(( $TMP_CNT + 1 )) ; 

	if [ $TMP_CNT -gt 50 ] ; 
	then
	    echo  " ok, we should be done generating X11 data";  
	    break ; 
	fi ; 
	
    done


}



prepare_sw_for_test()
{
    ./configure \
	--disable-gui \
	--disable-gnome-applet \
	--disable-doc          \
	--enable-gprof         && \
	make clean all           
    RET=$?
    return $RET
}

do_the_tests()
{
    make  check           && \
	cd cnee/src                        && \
	mv ../test/gmon.out* ./ ; 

	rm -f gmon.out.txt  gmon.sum       && \
	gprof -s cnee gmon.ou*             && \
	gprof cnee gmon.sum > gmon.out.txt && \
	gprof2dot.pl    \
	--transformer transformer.pl \
	--output cnee.dot --cluster-by-scope --merge-overloaded-functions \
	gmon.out.txt                && \
	dot -T$FMT -o cnee.$FMT cnee.dot && \
        mv cnee.$FMT ../../doc/profile && \
	cd ../.. 

	cp doc/profile/cnee.$FMT $DEST_DIR/cnee-call-graph.$FMT 
	cp cnee/src/gmon.out.txt $DEST_DIR/ 
	echo "OK :)"
}


close_down()
{
    sleep 1 
    kill    $XVFB_PID
    sleep 1 
    kill -9 $XVFB_PID
    sleep 1
}


init_test
prepare_sw_for_test
gen_x11_data&
do_the_tests
close_down

#mogrify -rotate 90 doc/profile/cnee.$FMT &&\
