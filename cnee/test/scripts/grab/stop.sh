#/bin/sh
#
#
# Filename:   grab/stop.sh
# Created:  Thu Jan  5 21:42 CEST 2006
# 
# Description:
#
#  Test the Xnee functionality
#       grabbed key 
#
#
 
 
 
 
# name of myself
MYNAME=grab/stop.sh
 
 
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
    
    verbose "starting $XNEE (grab (stop) on $STOP_KEY)"
    $XNEE --record --device-event-range 2-3 --data-to-record 10000 --seconds-to-record 100000 --events-to-record $TLEN -o $FILE -sk $STOP_KEY &
    XNEE_PID=$!
    
    verbose "XNEE PID = $XNEE_PID"
    sleep 2
    
    TMP=0
    while [ "$TMP" != "$LEN" ];
      do
      press_key "${STR:$TMP:1}"
      TMP=$(( $TMP + 1 ))
      if [ "$USE_GRAB" = "$TMP" ]
	  then
	  verbose "*** Pressing \"$STOP_KEY\" *** "
	  sleep 1
	  press_key "[$STOP_KEY]"
	  sleep 1
      fi
    done
    
    sleep 2
    verbose "shoot Xnee down"
    $KILL_1 $XNEE_PID
#    press_key a 
    sleep 3
}

check_log_file()
{
    _STRING="$1"
    _FILE=$2
    _LEN_STRING=$3

    CNT=0
    TMP=0
    while [ "$TMP" != "$LEN" ];
    do
      echo ${STRING:$TMP:1} | grep -e '[A-Z]'  2>/dev/null 1>/dev/null
      if [ "$?" == "0" ]
	  then 
	  if [ $TMP -le $USE_GRAB ]
	      then
#	      verbose "Caps...."
	      CNT=$(( $CNT + 1))
	  fi

      fi
      TMP=$(( $TMP + 1 ))
      if [ $TMP -le $USE_GRAB ]
	  then
	  CNT=$(( $CNT + 1))
      fi
    done

    if [ "$USE_GRAB" = "0" ]
	then
	LEN_STRING=$TMP
        #add one for the enter
	LEN_STRING=$(( $LEN_STRING ))
    else
	LEN_STRING=$CNT
    fi

    if [ -f $_FILE ]
	then
	LEN_GRAB_PRESS=$(cat $_FILE | grep -e "^0,2" | wc -l)
	LEN_GRAB_RELEASE=$(cat $_FILE | grep -e "^0,3" | wc -l)
    else
	LEN_GRAB_PRESS=-1
	LEN_GRAB_RELEASE=-1
    fi
    export LEN_STRING
    export LEN_GRAB_PRESS
    export LEN_GRAB_RELEASE
}

# main ..... I guess

verify_device swkeybd
STOP_KEY="F1"
FILE=kg.log
rm $FILE

STRING="This is a simple test string for Xnee"
USE_GRAB=0
test_keyboard "$STRING"   $FILE
check_log_file "$STRING" $FILE
verbose "Verifying the lengths of $LEN_STRING $LEN_GRAB_PRESS"
verify_same $LEN_STRING $LEN_GRAB_PRESS
verbose "Verifying the lengths of $LEN_STRING $LEN_GRAB_RELEASE"
verify_same $LEN_STRING $LEN_GRAB_RELEASE

STRING="This is a simple test string for Xnee"
USE_GRAB=1
test_keyboard "$STRING"   $FILE 
check_log_file "$STRING" $FILE
verbose "Verifying the lengths of $LEN_STRING $LEN_GRAB_PRESS"
verify_same $LEN_STRING $LEN_GRAB_PRESS
verbose "Verifying the lengths of $LEN_STRING $LEN_GRAB_RELEASE"
verify_same $LEN_STRING $LEN_GRAB_RELEASE

STRING="This is a simple test string for Xnee"
USE_GRAB=3
test_keyboard "$STRING"   $FILE 
check_log_file "$STRING" $FILE
verbose "Verifying the lengths of $LEN_STRING $LEN_GRAB_PRESS"
verify_same $LEN_STRING $LEN_GRAB_PRESS
verbose "Verifying the lengths of $LEN_STRING $LEN_GRAB_RELEASE"
verify_same $LEN_STRING $LEN_GRAB_RELEASE



rm $FILE

result_log $MYNAME 
fake_controlc
fake_controlc
exit
