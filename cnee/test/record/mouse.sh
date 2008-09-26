#/bin/sh
#
#
# Filename:   record/mouse-rec.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
# Description:
#
#  Test the Xnee functionality
#       record mouse motions
#
#
 
 
 
 
 
# name of myself
MYNAME=mouse-rec.sh
 
 
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


function check_nr_of_loops()
{
    EXPECTED=$1
    LOGFILE=$2

    
    NR=`grep -e '^0,6' $LOGFILE | wc -l | sed 's,[ \t]*,,g'`

    verify_same $EXPECTED $NR $LOGFILE 
}


function check_intervals()
{
    PIX=$1
    DIRECTION=$2
    FILE=$3
    FIELD=$4
    EXP_DIFF=$5

    TMP=0
    LAST=""


    TOTAL_TEST=`expr $TOTAL_TEST + 1`

    for i in `grep -e "^0,6" $FILE | awk -F , -v fi=$FIELD '{ print $fi }' `
    do
      THIS=$i
      if [ "$LAST" != "" ] ;
	  then
	  DIFF=`expr $LAST - $THIS`
	  
	  if [ "$DIFF" != "$EXP_DIFF" ]
	      then
	      MSG=" Comparing intervals in file $FILE $FIELD i=$i: $DIFF"
	      verbose " Data differs: $MSG"
	      error_log " Data differs: $MSG"
	      ERR_TEST=`expr $ERR_TEST + 1`
	      return 1
	  fi

      fi
      LAST=$THIS
    done
    SUCC_TEST=`expr $SUCC_TEST + 1`
}


function move_mouse()
{
    TMP=0
    PIX=$2
    DIR=$1
    while [ "$TMP" != "$PIX" ];
    do
      echo "$DIR" > $MOUSE_DEVICE
      TMP=`expr $TMP + 1 `
    done
}


function test_mouse()
{
    DIRECTION=$1
    PIX=$2
    FILE=$3
    FIELD=$4
    EXP_DIFF=$5

    TMP=-1
    verbose "starting xnee"
    $XNEE --record --device-event-range 6 --events-to-record $PIX -o $FILE &

    sleep 2
    while [ "$TMP" != "$PIX" ];
      do
      echo "$DIRECTION" > $MOUSE_DEVICE
      TMP=`expr $TMP + 1 `
    done
    sleep 2

    check_nr_of_loops $PIX $FILE 
    check_intervals   $PIX $DIRECTION $FILE $FIELD $EXP_DIFF

}




# main ..... I guess

verify_device $MOUSE_DEVICE_NAME



rm m*.log

NR_OF_PIX=400
HALF_WAYS=200

#start with the mouse in such a position that in can move around enought
move_mouse      u  $NR_OF_PIX
move_mouse      u  $NR_OF_PIX
move_mouse      u  $NR_OF_PIX
move_mouse      u  $NR_OF_PIX
move_mouse      l  $NR_OF_PIX
move_mouse      l  $NR_OF_PIX
move_mouse      l  $NR_OF_PIX
move_mouse      l  $NR_OF_PIX
move_mouse      d  $NR_OF_PIX
move_mouse      d  $HALF_WAYS
move_mouse      r  $NR_OF_PIX
move_mouse      r  $HALF_WAYS


#zero the device counter
zero_device $MOUSE_DEVICE


verbose  "starting...."
move_mouse      d  $NR_OF_PIX
test_mouse      u  $NR_OF_PIX mu.log 4 1


move_mouse      r  $NR_OF_PIX
test_mouse      l  $NR_OF_PIX ml.log 3 1

move_mouse      l  $NR_OF_PIX
test_mouse      r  $NR_OF_PIX mr.log 3 -1

move_mouse      u  $NR_OF_PIX
test_mouse      d  $NR_OF_PIX md.log 4 -1


#rm m*.log

result_log $MYNAME 
exit
