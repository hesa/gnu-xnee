#/bin/bash
#
#
# Filename:   record/keyboard-rec.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
# Description:
#
#  Test the Xnee functionality
#       record keyboard motions
#
#
 
 
 
 
 
# name of myself
MYNAME=keyboard-rec.sh
 
 
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
    echo "FAKE   $KEY"  $KEYBOARD_DEVICE
    printf "$KEY\n" >> $KEYBOARD_DEVICE
    sleep 0
}


fake_string()
{
    STR=$1

    echo "For thos about to fake...."
    sleep 5
    echo "we salute you"

    TMP=0
    while [ "$TMP" != "$LEN" ];
    do
      press_key "${STR:$TMP:1}"
      TMP=$(( $TMP + 1 ))
      sleep 1
    done

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
      echo ${STR:$TMP:1} | grep -e '[A-Z]'  2>/dev/null 1>/dev/null
      if [ "$?" == "0" ]
	  then 
	  CAPS=$(( $CAPS + 1))
      fi
      TMP=$(( $TMP + 1 ))
    done

    verbose "CAPS=$CAPS"
    verbose "LEN=$LEN"
    LEN=$(( $LEN + $CAPS)) 
    TLEN=$(( $LEN * 2 ))
    verbose "TLEN=$TLEN"
    

    fake_string $STR &

    verbose "starting xnee"
    echo "XNEE Start"
   $XNEE --record --device-event-range 2-3 --events-to-record $TLEN -o $FILE 
#    $XNEE --record --keyboard  -sk q 
    XNEE_PID=$!

    verbose "XNEE PID = $XNEE_PID"
    sleep 2


    
    sleep 2
    verbose "shoot Xnee down"
#    press_key "q"
    sleep 2
    
    $KILL_1 $XNEE_PID
#    press_key a 
    sleep 3

    check_nr_of_loops $LEN $LEN $FILE
}


# main ..... I guess

verify_device swkeybd

rm k*.log

# make sure to exclude the char/letter q, which we use to stop Xnee
export STRING="This is a simple test string for Xnee"
export STRING="ThisIsXnee"
test_keyboard "$STRING"   k1.log
verbose  "starting...."


#rm k*.log

result_log $MYNAME 
exit
