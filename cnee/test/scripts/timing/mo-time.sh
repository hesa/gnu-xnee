#/bin/sh
#
#
# Filename:   replay/.sh
# Generated:  
# 
# Description:
#
#  Test the Xnee functionality
#       replay mouse motions with different resolution
#
#
 
 
 
 
 
# name of myself
MYNAME=timing/mo-time.sh
 
 
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

MOUSE_DEVICE=/dev/swmouse




#
#rm m*.log

NR_OF_PIX=400
HALF_WAYS=200
verify_device swmouse


#get mouse in position and zero the device counter
#get_in_position
#zero_device swmouse
#record_and_replay 40 5 6 7

#get mouse in position and zero the device counter
get_in_position
zero_device swmouse
verbose "Storing start time"
verbose "Recording a session ......"
REAL_START_TIME=`date '+%s'`
$XNEE --record --mouse --data-to-record 10000 --seconds-to-record 10000 --events-to-record 5000 -o rep-time.log &

move_mouse r 600
move_mouse d 600
move_mouse l 600
move_mouse u 600
sleep 3

move_mouse r 400
sleep 1
move_mouse d 400
sleep 2
move_mouse l 400
sleep 2
move_mouse u 400
REAL_STOP_TIME=`date '+%s'`
sleep 1
$KILLALL_1 $XNEE
verbose "Storing stop time"
verbose " .... finished recording a session"
REAL_TIME=$(( $REAL_STOP_TIME - $REAL_START_TIME ))
verbose "The mouse motions took $REAL_TIME secs"


echo "Temp fix... removing 100% speed line in log file"
mv rep-time.log rep-time.tmp
cat rep-time.tmp | sed 's/speed-percent/#/g' > rep-time.log 

function timed_replay()
{
    SPEED=$1
    verbose "replaying with speed=$SPEED%"

    FAKE_START_TIME=`date '+%s'`
    $XNEE --replay -f rep-time.log --speed-percent $SPEED
    echo $XNEE --replay -f rep-time.log --speed-percent $SPEED
    FAKE_STOP_TIME=`date '+%s'`
    
    FAKE_TIME=$(( $FAKE_STOP_TIME - $FAKE_START_TIME ))
    EXP_TIME=$(( $REAL_TIME * $SPEED / 100 ))

    FAKE_SPEED=$(( $FAKE_TIME  *100 / $REAL_TIME ))
    FAKE_SPEED_PERCENT=$(( $FAKE_SPEED *100 / $SPEED ))

    TIME_PERCENT=$(( $FAKE_TIME *100 / $EXP_TIME ))

    LIMIT__=$(( $EXP_TIME * 90 )) 
    LIMIT_=$(( $LIMIT__ / 100 )) 
    LIMIT=$(( $EXP_TIME - $LIMIT_ )) 
    verbose "  Speed (%)   ${SPEED}          $FAKE_SPEED     $FAKE_SPEED_PERCENT"
    verbose "  Time (secs) $EXP_TIME          $FAKE_TIME     $LIMIT"
    
    verify_pos_alm_same $EXP_TIME          $FAKE_TIME     $LIMIT
}



verbose "Variable      Wanted      Actual      Percent"
verbose "=============================================="

timed_replay 10

timed_replay 25

timed_replay 50

timed_replay 75
#timed_replay 100
#timed_replay 200
#timed_replay 500
#timed_replay 1000

result_log $MYNAME 




