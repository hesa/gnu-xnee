#/bin/sh
#
#
# This file is generated from new_script.sh
#
#
# Filename:   opt_scripts/print-event-names.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
#
#  Test the Xnee option
#       --print-event-names
#  By reading up X11 events (name and number) from
#  the header file X.h. These values are compared
#  with the output from Xnee 
#

 
# name of myself
MYNAME=print-event-names.sh
 
# Arguments to test (short and long)
export LONG_ARG=--print-event-names
export SHORT_ARG=-pens
 
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


XNEE_NAMES=/tmp/xnee_arg.tmp
X11_NAMES=/tmp/xnee_events.tmp
 



#
# main .... sort of
#

# get the event numbers and names from the devel header file
find_events  /usr/include/X11/X.h $X11_NAMES



#
#
#  compare_event
#
#
#  Compare if the the name,number pair given
#  as argument is the same as given by Xnee --print-event-names
#  
# 
function compare_event()
{
    EXP_EV=$1
    EXP_EV_NAME=$2
    MSG=$3


    TOTAL_TEST=`expr $TOTAL_TEST + 1`

    # Xnee's number,name pair is located in the file $XNEE_NAMES
    REC_EV=`grep -w $EXP_EV           $XNEE_NAMES | awk '{ print $1}'  | sed 's,[ ],,g'`
    REC_EV_NAME=`grep -w $EXP_EV_NAME $XNEE_NAMES | awk ' { print $2}' | sed 's,[ ],,g'`
    verbose "verifying event names and numbers"
    verbose " \"$EXP_EV\"==\"$REC_EV\""
    verbose " \"$EXP_EV_NAME\"==\"$REC_EV_NAME\""

    if [ "$REC_EV" != "$EXP_EV" ];
	then
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
	return 1
    fi
    if [ "$REC_EV_NAME" != "$EXP_EV_NAME" ];
	then
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
	return 1
    fi
    verbose "success...."
    SUCC_TEST=`expr $SUCC_TEST + 1`
}


#
#
#  compare_event_false
#
#
# 
function compare_event_false()
{
    EXP_EV=$1
    EXP_EV_NAME=$2
    MSG=$3


    TOTAL_TEST=`expr $TOTAL_TEST + 1`

    # Xnee's number,name pair is located in the file $XNEE_NAMES
    REC_EV=`grep -w $EXP_EV           $XNEE_NAMES | awk '{ print $1}'  | sed 's,[ ],,g'`
    REC_EV_NAME=`grep -w $EXP_EV_NAME $XNEE_NAMES | awk ' { print $2}' | sed 's,[ ],,g'`
    verbose "verifying event names and numbers"
    verbose " \"$EXP_EV\"==\"$REC_EV\""
    verbose " \"$EXP_EV_NAME\"==\"$REC_EV_NAME\""

    if [ "$REC_EV" == "$EXP_EV" ];
	then
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
	return 1
    fi
    if [ "$REC_EV_NAME" == "$EXP_EV_NAME" ];
	then
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
	return 1
    fi
    verbose "success...."
    SUCC_TEST=`expr $SUCC_TEST + 1`
}




#
# positive test
#
#get the event numbers and names from Xnee
$XNEE $LONG_ARG | grep -e "^[0-9]" >  $XNEE_NAMES
STATUS=$?
# compare Xnee number and names with the system
for i in `cat $X11_NAMES`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`
# LASTEvent is now concidered valid 
#  if [ "$NAME_" == "LASTEvent" ];
#      then
#      verbose "   comparing (false): \"$EV_\" \"$NAME_\" "
#      compare_event_false $EV_ $NAME_ "$EV_ $NAME_ "
#  else
      verbose "   comparing: \"$EV_\" \"$NAME_\" "
      compare_event $EV_ $NAME_ "$EV_ $NAME_ "
#  fi
done
check_retval $STATUS 0 





# positive test
$XNEE $SHORT_ARG | grep -e "^[0-9]" > $XNEE_NAMES.2
STATUS=$?

diff $XNEE_NAMES $XNEE_NAMES.2>/dev/null 1>/dev/null

TOTAL_TEST=`expr $TOTAL_TEST + 1`
if [ "$?" != "0" ] ; 
    then
    echo " xnee $SHORT_ARG differs from $LONG_ARG"
    ERR_TEST=`expr $ERR_TEST + 1`
else
    SUCC_TEST=`expr $SUCC_TEST + 1`
fi
check_retval $STATUS 0 "$STATUS 0"


\rm -f $X11_NAMES
\rm -f $XNEE_NAMES $XNEE_NAMES.2


result_log $MYNAME 
