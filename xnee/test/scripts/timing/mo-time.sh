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
MYNAME=mouse-rep.sh
 
 
# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the xnee test dir "
    exit
else 
    . etc/base_funs
fi 
 
init_test 
parse_me "$*"

#######################################################################

MOUSE_DEVICE=/dev/swmouse


#start with the mouse in such a position that in can move around enough
function get_in_position()
{
    fast_move_mouse  u 1000
    fast_move_mouse  l 1000
    sleep 0
    fast_move_mouse  u 1000
    fast_move_mouse  l 1000
    sleep 0
    fast_move_mouse  d 100
    fast_move_mouse  r 100
    sleep 3
}


function move_mouse()
{
    TMP=0
    DIR=$1
    PIX=$2
    while [ "$TMP" != "$PIX" ];
    do
      echo "$DIR" > $MOUSE_DEVICE
      TMP=`expr $TMP + 1 `
      sleep 0
    done
}

function slow_move_mouse()
{
    TMP=0
    PIX=$2
    DIR=$1
    while [ "$TMP" != "$PIX" ];
    do
      echo "$DIR" > $MOUSE_DEVICE
      TMP=`expr $TMP + 1 `
      echo "TMP=$TMP"
      sleep 1
    done
}

function fast_move_mouse()
{
    PIX=$2
    DIR=$1
    echo "$DIR$PIX" > $MOUSE_DEVICE
}



#
#rm m*.log

NR_OF_PIX=400
HALF_WAYS=200


#get mouse in position and zero the device counter
#get_in_position
#zero_device swmouse
#record_and_replay 40 5 6 7

#get mouse in position and zero the device counter
get_in_position
zero_device swmouse
verbose "Storing start time"
REAL_START_TIME=`date '+%s'`
$XNEE --record --mouse --loops 5000 -o rep-time.log &

move_mouse r 600
sleep 1
move_mouse d 600
sleep 2
move_mouse l 600
sleep 3
move_mouse u 600
sleep 0

move_mouse r 400
sleep 1
move_mouse d 400
sleep 2
move_mouse l 400
sleep 3
move_mouse u 400
REAL_STOP_TIME=`date '+%s'`
sleep 1
killall -TERM $XNEE
verbose "Storing stop time"
REAL_TIME=$(( $REAL_STOP_TIME - $REAL_START_TIME ))
verbose "The mouse motions took $REAL_TIME secs"
echo "The mouse motions took $REAL_TIME secs"

function timed_replay()
{
    SPEED=$1
    verbose "replaying with speed=$SPEED%"
    FAKE_START_TIME=`date '+%s'`
    $XNEE --replay -f rep-time.log --speed-adjust $SPEED
    FAKE_STOP_TIME=`date '+%s'`

    FAKE_TIME=$(( $FAKE_STOP_TIME - $FAKE_START_TIME ))
    EXP_TIME=$(( $REAL_TIME * $SPEED / 100 ))

    FAKE_SPEED=$(( $FAKE_TIME  *100 / $REAL_TIME ))
    FAKE_SPEED_PERCENT=$(( $FAKE_SPEED *100 / $SPEED ))

    TIME_PERCENT=$(( $FAKE_TIME *100 / $EXP_TIME ))
    
    echo "  Speed (%)   ${SPEED}          $FAKE_SPEED     $FAKE_SPEED_PERCENT"
    echo "  Time (secs) $EXP_TIME          $FAKE_TIME     $TIME_PERCENT"
}


echo "Variable      Wanted      Actual      Percent"
echo "=============================================="

timed_replay 10

timed_replay 25

timed_replay 50

timed_replay 75

timed_replay 80

timed_replay 90

timed_replay 100

timed_replay 125

timed_replay 150

timed_replay 200

timed_replay 400

timed_replay 500

timed_replay 800

timed_replay 1000



result_log $MYNAME 



