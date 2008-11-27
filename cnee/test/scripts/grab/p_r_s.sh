#/bin/sh
#
#
# Filename:   grab/p_r_s.sh
# Created:  Thu Jan  5 21:42 CEST 2006
# 
# Description:
#
#  Test the Xnee functionality
#       grabbed key 
#
#
 
 
 
 
# name of myself
MYNAME=grab/p_r_s.sh
 
 
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

KEYBOARD_DEVICE=/dev/swkeybd



function test_keyboard()
{
    STR=$1
    FILE=$2

    
    LEN1=${#STRING1}
    LEN2=${#STRING2}
    LEN3=${#STRING3}
	
    verbose "starting $XNEE (grab (stop) on $STOP_KEY)"
    run_cnee --record --device-event-range 2-3 --data-to-record 10000 \
          --seconds-to-record 100000 --events-to-record -1 \
          -o $FILE -sk $STOP_KEY -pk $PAUSE_KEY  -rk $RESUME_KEY \
	  &
    XNEE_PID=$!
    
    verbose "XNEE PID = $XNEE_PID"
    sleep 2
    
    press_key_from_string ${STRING1}
    verbose "*** Pressing \"$PAUSE_KEY\" *** "
    press_key "[${PAUSE_KEY}]"
    sleep 1

    press_key_from_string ${STRING2}
    verbose "*** Pressing \"$RESUME_KEY\" *** "
    press_key "[${RESUME_KEY}]"
    sleep 1
    
    press_key_from_string ${STRING3}
    verbose "*** Pressing \"$STOP_KEY\" *** "
    press_key "[${STOP_KEY}]"    
    sleep 1

    sleep 2
    verbose "shoot Xnee down"
    $KILL_1 $XNEE_PID

}

replay_file()
{
    FILE=$1

    verbose "replaying file $RECORDED_FILE"
    launch_cat_wrapper    /tmp/xnee_test.txt  &

    sleep 4

    verbose "starting $XNEE (grab (stop) on $STOP_KEY)"
    run_cnee --replay --file $FILE 
    XNEE_PID=$!
    
    sleep 2 
    verbose "  faking control-d"
    fake_controld
    sleep 2 
    verbose "  faking control-d"
    fake_controld
    sleep 2 
    verbose "  faking control-c"
    fake_controlc
    sleep 1

    sleep 2
    verbose "shoot Xnee down"
    $KILL_1 $XNEE_PID
}


check_log_file()
{
    
    TMP_FILE=/tmp/xnee_tmp.tmp

    rm $TMP_FILE
    echo -n "${STRING1}${STRING3}" > $TMP_FILE

    sdiff -s $TMP_FILE /tmp/xnee_test.txt 

    export DIFF_RET=$?
}

# main ..... I guess

verify_device swkeybd
STOP_KEY="F1"
PAUSE_KEY="F2"
RESUME_KEY="F3"

FILE=kg.log
rm $FILE

export STRING1="first"
export STRING2="second"
export STRING3="third"
USE_GRAB=1
test_keyboard "$STRING"   $FILE
replay_file $FILE
check_log_file
verify_same $DIFF_RET 0

rm $FILE

result_log $MYNAME 
exit
