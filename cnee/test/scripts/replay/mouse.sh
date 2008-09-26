#/bin/sh
#
#
# Filename:   replay/mouse-rec.sh
# Generated:  Sat Aug  9 01:29:48 CEST 2003
# 
# Description:
#
#  Test the Xnee functionality
#       replay mouse motions
#
#
 
 
 
 
 
# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the xnee test dir "
    exit
else 
    . etc/base_funs
fi 
 
# name of myself
MYNAME=replay/mouse.sh 
 
init_test $MYNAME
parse_me "$*"

#######################################################################


function record_and_replay()
{
    RIGHT_PIX=$1
    DOWN_PIX=$2
    LEFT_PIX=$3
    UP_PIX=$4
    
    LOG_FILE=rep-mouse.log
    SLEEP_INTERVAL=10
    SUM=$(( $1 + $2 + $3 + $4 ))
    verbose "SUM of $1 $2 $3 $4 = $SUM"
    

    #
    # RECORDING
    #
    verbose "Start recording ..."
    rm rep-mouse.log
    get_pointer
    STARTX=$XPOS
    STARTY=$YPOS
    verbose "   ### start point:    $XPOS $YPOS `date`"
    $XNEE --record --mouse --data-to-record 10000 --seconds-to-record 10000 --events-to-record $SUM -o rep-mouse.log &

    verbose " Sleeping 2 secs"
    sleep 2
    verbose "Start faking with swinput:"


    verbose "  r $RIGHT_PIX"
    save_pointer
    verbose "   ### new point:    $XPOS $YPOS `date`"
    TIME1=`date '+%s'`
    move_mouse r $RIGHT_PIX
    TIME2=`date '+%s'`
    R_DELAY=$(( $TIME2 - $TIME1 ))
    get_pointer
    verbose "       new point:    $XPOS $YPOS `date` DELAY=$R_DELAY"
    verify_not_same $SAVE_XPOS $XPOS
    verify_same     $SAVE_YPOS $YPOS
    verify_same     $(( $XPOS - $SAVE_XPOS )) $RIGHT_PIX
    
    verbose " Sleeping $SLEEP_INTERVAL secs"
    sleep $SLEEP_INTERVAL

    save_pointer
    verbose "  d $DOWN_PIX"
    verbose "   ### new point:    $XPOS $YPOS `date`"
    TIME1=`date '+%s'`
    move_mouse d $DOWN_PIX
    TIME2=`date '+%s'`
    D_DELAY=$(( $TIME2 - $TIME1 ))
    get_pointer
    verbose "       new point:    $XPOS $YPOS `date` DELAY=$D_DELAY"
    verify_same      $SAVE_XPOS $XPOS
    verify_not_same  $SAVE_YPOS $YPOS
    verify_same     $(( $YPOS - $SAVE_YPOS )) $DOWN_PIX

    verbose " Sleeping $SLEEP_INTERVAL secs"
    sleep $SLEEP_INTERVAL

    save_pointer
    verbose "  l $LEFT_PIX"
    verbose "   ### new point:    $XPOS $YPOS `date`"
    TIME1=`date '+%s'`
    move_mouse l $LEFT_PIX
    TIME2=`date '+%s'`
    L_DELAY=$(( $TIME2 - $TIME1 ))
    get_pointer
    verbose "       new point:    $XPOS $YPOS `date` DELAY=$L_DELAY"
    verify_not_same $SAVE_XPOS $XPOS
    verify_same     $SAVE_YPOS $YPOS
    verify_same     $(( $SAVE_XPOS - $XPOS )) $LEFT_PIX

    verbose " Sleeping $SLEEP_INTERVAL secs"
    sleep $SLEEP_INTERVAL

    verbose "  u $UP_PIX"
    save_pointer
    verbose "   ### new point:    $XPOS $YPOS `date`"
    TIME1=`date '+%s'`
    move_mouse u $UP_PIX
    TIME2=`date '+%s'`
    U_DELAY=$(( $TIME2 - $TIME1 ))
    get_pointer
    verbose "       new point:    $XPOS $YPOS `date`  DELAY=$U_DELAY"
    verify_same     $SAVE_XPOS $XPOS
    verify_not_same $SAVE_YPOS $YPOS
    verify_same     $(( $SAVE_YPOS - $YPOS )) $UP_PIX

    verbose " Sleeping5 secs"
    sleep 5
    $KILLALL_1 $XNEE
    verbose "Making sure we have a valid log file...."
    export NR_OF_LOGS=`grep -e "0,6" rep-mouse.log | wc -l`
    verbose "Nr of logs:"
    verbose "  in file: $NR_OF_LOGS"
    verbose "  wanted:  $SUM"
    verify_same  $NR_OF_LOGS $SUM
    


    #
    # REPLAYING
    # ... log file should be ok by now... or else errors are reported
    
    get_in_position
    verbose "Replay starting ....... "
    verbose "###############################"
    verbose "###############################"
    verbose "###############################"
    verbose "###############################"
    verbose "###############################"

    save_pointer
    verbose "  -- checking start   $XPOS $YPOS  `date`"
    verify_same $STARTX $SAVE_XPOS
    verify_same $STARTY $SAVE_YPOS
    $XNEE --replay -f rep-mouse.log &
    TMP=0

#    echo "$TMP `$getx`  `$gety`"
#    while [ "$TMP" != "100" ];
#    do
#      sleep 5
#      echo "$TMP  `$getx`  `$gety`"
#      TMP=$(( $TMP + 5 ))
#    done
#exit
    # sleep half the sleeping interval
    MYSLEEP=$(( $SLEEP_INTERVAL / 2 ))
    verbose "sleeping $MYSLEEP"

    verbose " Sleeping $R_DELAY + $MYSLEEP secs"
    sleep $R_DELAY
    sleep $MYSLEEP
    get_pointer
    verbose "  -- checking r   $XPOS $YPOS  `date`"
    verify_not_same $SAVE_XPOS $XPOS
    verify_same     $SAVE_YPOS $YPOS
    verify_same     $(( $XPOS - $SAVE_XPOS )) $RIGHT_PIX

    save_pointer
    verbose " Sleeping $SLEEP_INTERVAL + more secs"
    sleep $SLEEP_INTERVAL
    sleep $D_DELAY
    get_pointer
    verbose "  -- checking d   $XPOS $YPOS  `date`"
    verify_same      $SAVE_XPOS $XPOS
    verify_not_same  $SAVE_YPOS $YPOS
    verify_same     $(( $YPOS - $SAVE_YPOS )) $DOWN_PIX

    save_pointer
    verbose " Sleeping $SLEEP_INTERVAL + more secs"
    sleep $SLEEP_INTERVAL
    sleep $L_DELAY
    get_pointer
    verbose "  -- checking l   $XPOS $YPOS  `date`"
    verify_not_same $SAVE_XPOS $XPOS
    verify_same     $SAVE_YPOS $YPOS
    verify_same     $(( $SAVE_XPOS - $XPOS )) $LEFT_PIX

    save_pointer
    verbose " Sleeping $SLEEP_INTERVAL + more secs"
    sleep $SLEEP_INTERVAL
    sleep $U_DELAY
    get_pointer
    verbose "  -- checking u   $XPOS $YPOS  `date`"
    verify_same     $SAVE_XPOS $XPOS
    verify_not_same $SAVE_YPOS $YPOS
    verify_same     $(( $SAVE_YPOS - $YPOS )) $UP_PIX
}

# main ..... I guess

verify_device $MOUSE_DEVICE


#
#rm m*.log

NR_OF_PIX=400
HALF_WAYS=200

#get mouse in position and zero the device counter
get_in_position
zero_device $MOUSE_DEVICE



record_and_replay 40 5 6 7
sleep 5

#get mouse in position and zero the device counter
get_in_position
zero_device $MOUSE_DEVICE
record_and_replay 200 300 250 170
sleep 5

#get mouse in position and zero the device counter
get_in_position
zero_device $MOUSE_DEVICE
record_and_replay 500 200 200 100
sleep 5

#get mouse in position and zero the device counter
#get_in_position
#zero_device $MOUSE_DEVICE
#record_and_replay 800 600 700 500
#sleep 5


result_log $MYNAME 

