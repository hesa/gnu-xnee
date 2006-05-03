#/bin/sh
#
#
# Filename:   record/keyboard.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
# Description:
#
#  Test the Xnee functionality
#       record keyboard 
#
#
 
 
 
 
 
# name of myself
MYNAME=record/keyboard.sh
 
 
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



function check_nr_of_loops()
{
    PRESS_EXPECTED=$1
    RELEASE_EXPECTED=$2
    LOGFILE=$3
    
    PRESS_NR=`grep -e '^0,2' $LOGFILE | wc -l | sed 's,[ \t]*,,g'`
    RELEASE_NR=`grep -e '^0,2' $LOGFILE | wc -l | sed 's,[ \t]*,,g'`

    verify_same $PRESS_EXPECTED   $PRESS_NR   $LOGFILE 
    verify_same $RELEASE_EXPECTED $RELEASE_NR $LOGFILE 
}



function press_key()
{
    KEY=$1
    verbose "faking key $KEY"
    echo "$KEY" > $KEYBOARD_DEVICE
    sleep 0
}



function test_keyboard()
{
    STR=$1
    FILE=$2

    
    LEN=${#STR}
	
	
	
    CAPS=0
    TMP=0
    while [ "$TMP" != "$LEN" ];
    do
      CAPS_ADD=$(echo ${STR:$TMP:1} | sed 's,[a-z],,g' | grep -c -e "[A-Z]")
      CAPS=$(( $CAPS + $CAPS_ADD ))
      TMP=$(( $TMP + 1 ))
    done

    verbose "CAPS=$CAPS"
    verbose "LEN=$LEN"

    LEN=$(( $LEN + $CAPS)) 
    TLEN=$(( $LEN * 2 ))
    verbose "TLEN=$TLEN"
    

    verbose "starting xnee"
    $XNEE --record --device-event-range 2-3 --data-to-record 10000 --seconds-to-record 10000 --events-to-record $TLEN -o $FILE &
    XNEE_PID=$!

    verbose "XNEE PID = $XNEE_PID"
    sleep 2

    TMP=0
    while [ "$TMP" != "$LEN" ];
    do
      press_key "${STR:$TMP:1}"
      TMP=$(( $TMP + 1 ))
    done

    
    sleep 2
    verbose "shoot Xnee down"
    $KILL_1 $XNEE_PID
#    press_key a 
    sleep 3

    check_nr_of_loops $LEN $LEN $FILE
}


# main ..... I guess

verify_device swkeybd

rm k*.log

STRING="This is a simple test string for Xnee"
test_keyboard "$STRING"   k1.log
verbose  "starting...."


#rm k*.log

result_log $MYNAME 
exit
