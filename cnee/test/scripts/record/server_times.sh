#/bin/sh
#
#
# Filename:   record/server_time.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
# Description:
#
#  Test the Xnee functionality
#       recorded timestamp
#
#
 
 
 
 
 
# name of myself
MYNAME=record/server_times.sh
 
 
# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the xnee test dir "
    exit
else 
    . etc/base_funs
fi 
 
init_test $MYNAME
parse_me "$*"

#######################################################################

XEV_LOG=./xev_time.log
XNEE_LOG=./xn_time.log

#XEV_CMD="$XEV "
create_log()
{
    rm -f $XEV_LOG
    rm  $XNEE_LOG
    sleep 0 
    touch  $XEV_LOG
    touch  $XNEE_LOG
}

kill_all()
{
    $KILL_1 TERM $XEV_PID
    $KILL_1 TERM $XNEE_PID
    $KILL_1 TERM $XNEST_PID
}

start_all()
{
    gdmXnest &
    XNEST_PID=$?

    verbose "sleeoing 5 secs to let Xnest start up"
    sleep 2

    $XNEE  --display :20 --record --device-event-range MotionNotify --out-file $XNEE_LOG --data-to-record -1 --events-to-record -1 --seconds-to-record -1 &
    XNEE_PID=$!

    ROOT_WINDOW_ID=$(xdpyinfo -display :20 | grep "root window id" | awk '{ print $4 }' | sed 's/[ \t]*//g')
    xev -id $ROOT_WINDOW_ID -display :20 > $XEV_LOG &
    XEV_PID=$!
}

fix_files()
{
    #
    # XEV
    #

    # flatten the file
    mv $XEV_LOG ${XEV_LOG}.tmp
    cat $XEV_LOG.tmp | perl -p -e 's/,[ \n\t\r]*$/,/g' | \
    perl -p -e 's/^[ \t\r\n]*$//g'  >  $XEV_LOG 

    # remove the non-MotionNotify lines
    mv $XEV_LOG ${XEV_LOG}.tmp
    grep MotionNotify ${XEV_LOG}.tmp > ${XEV_LOG}


    #
    # XNEE
    #
    mv $XNEE_LOG ${XNEE_LOG}.tmp
    grep "0,6" ${XNEE_LOG}.tmp > ${XNEE_LOG}

    # delete the temp files
    rm -f ${XNEE_LOG}.tmp ${XEV_LOG}.tmp
}

check_motion_times()
{
    XNEE_TMP=xnee_tmp
    XEV_TMP=xev_tmp
    
    XNEE_BIGGER=0
    XEV_BIGGER=0
    XNEE_BIGGER_AFTER_EQ=0
    XEV_BIGGER_AFTER_EQ=0

    EQ_CNT=0

    cat $XEV_LOG | awk ' { print $14 } ' | sed 's/,//g' > $XEV_TMP
    cat $XNEE_LOG  |  awk ' BEGIN { FS="," ; } { print $8 }' > $XNEE_TMP


    # open the files
    exec 4< $XEV_TMP
    exec 5< $XNEE_TMP
    
    # read 'em up Scotty
    read xev_time <&4
    read xnee_time <&5

    while (true)
    do
	if [ "${#xev_time}" = "0" ]
	then
	    verbose "xev fisnihed....."
	    break
	fi

	if [ "${#xnee_time}" = "0" ]
	then
	    verbose "xev fisnihed....."
	    break
	fi

	if [ $xev_time -lt $xnee_time ]
	then
	    verbose "    xnee is large"
	    XNEE_BIGGER=$(( $XNEE_BIGGER + 1 ))
	    if [ "$EV_CNT" = "0" ]
	    then 
		XNEE_BIGGER_AFTER_EQ=$(( $XNEE_BIGGER_AFTER_EQ + 1 ))
	    fi
	    
	    read xev_time <&4
	elif [ $xev_time -gt $xnee_time ]
	then
	    verbose "    xev is large"
	    XEV_BIGGER=$(( $XEV_BIGGER + 1 ))
	    if [ "$EV_CNT" = "0" ]
	    then 
		XEV_BIGGER_AFTER_EQ=$(( $XEV_BIGGER_AFTER_EQ + 1 ))
	    fi
	    read xnee_time <&5
	else
	    verbose "    xnee is eq xev   $EQ_CNT"
	    EQ_CNT=$(( $EQ_CNT + 1 ))
	    read xev_time <&4
	    read xnee_time <&5
	fi
    #
    done

    verbose   "XNEE_BIGGER_AFTER_EQ $XNEE_BIGGER_AFTER_EQ "
    verbose   "XEV_BIGGER_AFTER_EQ  $XEV_BIGGER_AFTER_EQ  "
    verbose   "EQ_CNT               $EQ_CNT             "

    verify_same     $XNEE_BIGGER_AFTER_EQ 0
    verify_same     $XEV_BIGGER_AFTER_EQ  0
    verify_not_same $EQ_CNT               0

    # close 'em fds
    exec 4<&-
    exec 5<&-
}

move_my_mouse()
{
    move_mouse      d  $1
    move_mouse      l  $1
    move_mouse      r  $1
    move_mouse      u  $1
}


verify_device swmouse

get_in_position

verbose "creating log files"
create_log

verbose "start the recording apps (cnee and xev)"
start_all


verbose "sleeping 2 secs to start up everything before recording"
sleep 2
move_my_mouse 10
sleep 1
move_my_mouse 50
sleep 1
move_my_mouse 100
sleep 1
move_my_mouse 200
sleep 1

verbose "moving the mouse using swinput"


sleep 2


verbose "adjusting the recorded files"
fix_files

verbose "verify the contents"
check_motion_times

kill_all


result_log $MYNAME 
exit





