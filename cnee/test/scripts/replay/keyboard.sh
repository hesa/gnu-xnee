#/bin/sh
#
#
# Filename:   recplay/keyboard.sh
# Created:  Fri Aug  8 00:04:38 CEST 2003
# 
# Description:
#
#  Test the Xnee functionality
#       replay recorded keyboard motions
#
#
 
 
 
 
# name of myself
MYNAME=replay/keyboard.sh
 
 
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



function replay_file()
{
    RECORDED_FILE=$1
    
    verbose "replaying file $RECORDED_FILE"
    rm /tmp/xnee_test.txt
    verbose "  launching xterm/cat"
    xterm -e 'cat > /tmp/xnee_test.txt' &
    sleep 2
    verbose "  launching xnee"
    $XNEE --replay -f $RECORDED_FILE 
    sleep 2 
    verbose "  faking enter"
    fake_enter
    sleep 3
    verbose "  faking control-d"
    fake_controld
    sleep 1
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
    TLEN=$(( $LEN * 3 ))
    verbose "TLEN=$TLEN"
    

    verbose "starting xnee"
     $XNEE --record --device-event-range 2-3 --data-to-record 10000 --seconds-to-record 100000 --events-to-record $TLEN -o $FILE &
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
    kill -2 $XNEE_PID
#    press_key a 
    sleep 3

}


# main ..... I guess

verify_device swkeybd

FILE=k1.log
rm $FILE

STRING="This is a simple test string for Xnee"
test_keyboard "$STRING"   $FILE

verbose  "starting...."

replay_file  $FILE


LEN_STRING=${#STRING}
#add one for the enter
LEN_STRING=$(( $LEN_STRING + 1 ))

if [ -f /tmp/xnee_test.txt ]
then
    LEN_FAKE=`wc -c /tmp/xnee_test.txt | awk '{print $1}' | sed 's, ,,g'`
else
    LEN_FAKE=0
fi

verbose "Verifying the lengths of $LEN_STRING $LEN_FAKE"
verify_same $LEN_STRING $LEN_FAKE

rm $FILE

result_log $MYNAME 
fake_controlc
fake_controlc
exit
