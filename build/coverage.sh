#!/bin/sh

DATE=$(date '+%Y-%m-%d')

if [ "$1" = "" ]
then
    BASE_DEST_DIR=/tmp/xnee-temp
else
    BASE_DEST_DIR=$1
fi
export CPU=x86
if [ "$(uname -a | grep -c ppc)" != "0" ]
then
    export CPU=ppc
fi


export DEST_DIR=$BASE_DEST_DIR/coverage/$DATE/$CPU
mkdir -p $DEST_DIR


#
# current version is nothing but a hack to try out some conecpts
#

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
    
    
    export DISPLAY=:32
    sleep 2
    

<<<<<<< coverage.sh
=======
    
>>>>>>> 1.4
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
	--disable-doc            && \
	make clean                 
    RET=$?
    return $RET
}

do_the_tests()
{

    make -f GNUmakefile coverage    
    cp -r doc/coverage/* $DEST_DIR/
    
    echo "Result copying from $(pwd) to $DEST_DIR"
    pwd

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
gen_x11_data&
do_the_tests
close_down
