#!/bin/bash


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


FAKE_MOTION="fake-motion"
FAKE_KEY="fake-key"
XNEE_ARGS="--replay --file stdin"

TMP_X=0
TMP_Y=0
CTR=0

SLEEP_DELAY=0
#get_in_position





function move_rel_square_sub2()
{
    DIR=$1
    RELATIVE=$2
    MSLEEP=$3
    SQUARE_SIZE=$4
    PIXEL_TO_MOVE=$5

    CTR=0
    if  [ "$DIR" == "x" ];
	then
	TMP_Y=0
	while [ "$CTR" != "$SQUARE_SIZE" ]
	  do
	  CTR=$(( $CTR + 1 ))
	  TMP_X=$PIXEL_TO_MOVE 
	  printf "%s   x=%s%.3d y=%s%.3d msecs=%d \n" $FAKE_MOTION $RELATIVE $TMP_X $RELATIVE  $TMP_Y $MSLEEP
	done 
    else
	TMP_X=0
	while [ "$CTR" != "$SQUARE_SIZE" ]
	  do
	  CTR=$(( $CTR + 1 ))
	  TMP_Y=$PIXEL_TO_MOVE
	  printf "%s   x=%s%.3d y=%s%.3d msecs=%d \n" $FAKE_MOTION $RELATIVE  $TMP_X  $RELATIVE  $TMP_Y $MSLEEP
	done 
    fi
}

function move_square_sub2()
{
    DIR=$1
    SIGN=$2
    MSLEEP=$3
    SQUARE_SIZE=$4
    PIXEL_TO_MOVE=$5

    CTR=0
    if  [ "$DIR" == "x" ];
	then
	while [ "$CTR" != "$SQUARE_SIZE" ]
	  do
	  CTR=$(( $CTR + 1 ))
	  TMP_X=$(( $TMP_X $SIGN $PIXEL_TO_MOVE ))
	  printf "%s   x=%.3d y=%.3d msecs=%d \n" $FAKE_MOTION $TMP_X   $TMP_Y $MSLEEP
	done 
    else
	while [ "$CTR" != "$SQUARE_SIZE" ]
	  do
	  CTR=$(( $CTR + 1 ))
	  TMP_Y=$(( $TMP_Y $SIGN $PIXEL_TO_MOVE ))
	  printf "%s   x=%.3d y=%.3d msecs=%d \n" $FAKE_MOTION   $TMP_X   $TMP_Y $MSLEEP
	done 
    fi
}

function move_square_sub1()
{
    MSLEEP=$1
    SQUARE_SIZE=$2
    PIXEL_TO_MOVE=$3
    RELATIVE=$4

    move_square_sub2 "x" "+" "$MSLEEP" "$SQUARE_SIZE" "$PIXEL_TO_MOVE"  "$RELATIVE"
    sleep $SLEEP_DELAY
    move_square_sub2 "y" "+" $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE  "$RELATIVE"
    sleep $SLEEP_DELAY
    move_square_sub2 "x" "-" $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE "$RELATIVE"
    sleep $SLEEP_DELAY
    move_square_sub2 "y" "-" $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE "$RELATIVE"
    sleep $SLEEP_DELAY
}

function move_rel_square_sub1()
{
    MSLEEP=$1
    SQUARE_SIZE=$2
    PIXEL_TO_MOVE=$3

    move_rel_square_sub2 "x" "+" "$MSLEEP" "$SQUARE_SIZE" "$PIXEL_TO_MOVE" 
    sleep $SLEEP_DELAY
    move_rel_square_sub2 "y" "+" $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE 
    sleep $SLEEP_DELAY
    move_rel_square_sub2 "x" "-" $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE 
    sleep $SLEEP_DELAY
    move_rel_square_sub2 "y" "-" $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE 
    sleep $SLEEP_DELAY
}

function move_square()
{
    MSLEEP=$1
    SQUARE_SIZE=$2
    PIXEL_TO_MOVE=$3

    move_square_sub1  $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE "0" | $XNEE $XNEE_ARGS 
}

function move_rel_square()
{
    MSLEEP=$1
    SQUARE_SIZE=$2
    PIXEL_TO_MOVE=$3

    move_rel_square_sub1  $MSLEEP $SQUARE_SIZE $PIXEL_TO_MOVE  | $XNEE $XNEE_ARGS 
}


function fake_string_sub()
{
    STRING=$1
    MSEC=$2
    LAST="${#STRING}"
    CTR=0

    while [  "$CTR" != "$LAST" ]
      do
      CHAR=${STRING:$CTR:1}
      if [ "$CHAR" = " " ]
	  then
	  CHAR=XK_space
      fi

      echo "$FAKE_KEY key=$CHAR msecs=$MSEC"
      CTR=$(( $CTR + 1 ))
    done  
    echo "$FAKE_KEY key=XK_Return msecs=$MSEC "
}

function fake_string()
{
    fake_string_sub "$1"  $2 | $XNEE   $XNEE_ARGS 
}


function fake_button_sub()
{
    for i in 1 2 3
    do
      echo "fake-button button=$i msecs=1000"
    done
}

function fake_buttons()
{
    fake_button_sub |  $XNEE   $XNEE_ARGS 
}

function grab_window()
{
    echo "fake-key-press key=Alt_L msecs=100"
    echo "fake-button-press button=1 msecs=100"
}

function ungrab_window()
{
    echo "fake-button-release button=1 msecs=100"
    echo "fake-key-release key=Alt_L msecs=100"
}

function move_win_sub()
{
    grab_window

    sleep 1
    move_rel_square 1 100 1 
    sleep 1
    
    ungrab_window
}

function move_window()
{
    move_win_sub   |  $XNEE   $XNEE_ARGS 
}




move_square 1 100 1 
move_square 1 400 1
move_square 1 800 1



fake_string "This is a test and a hard place" 10
fake_string "ls -al" 200

get_in_position
move_window


fake_buttons 

