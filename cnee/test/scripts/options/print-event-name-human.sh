#/bin/sh
#
#
# Description:
#
#  Test the Xnee option
#       --print-event-name-human
#  By reading up X11 events (name and number) from
#  the header file X.h. These values are compared
#  with the output from Xnee 
#
#
 
 
 
 
# name of myself
MYNAME=print-event-name-human.sh
 
# Arguments to test (short and long)
export LONG_ARG=--print-event-name
export SHORT_ARG=-pen
 
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


X11_NAMES=/tmp/xnee_events.tmp
 




#
# main .... sort of
#

# get the event numbers and names from the devel header file
find_events  /usr/include/X11/X.h $X11_NAMES
LAST_EVENT=`grep LASTEvent /usr/include/X11/X.h | awk '{ print $3}' | sed 's,[ ]*,,g' `







#
# positive tests
#
# compare Xnee number and names with the system
for i in `cat $X11_NAMES  | grep -v GenericEvent` 
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`


  XNEE_EV=`run_cnee -hp $LONG_ARG $NAME_`

  STATUS=$?
  check_retval $STATUS 0 


  XNEE_EV_NAME=`run_cnee -hp $LONG_ARG $EV_`
  STATUS=$?
  check_retval  $STATUS 0 

  compare_data $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME
done



# compare Xnee number and names with the system
for i in `cat $X11_NAMES | grep -v GenericEvent`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`
  XNEE_EV=`run_cnee -hp $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 

  XNEE_EV_NAME=`run_cnee -hp $SHORT_ARG $EV_`
  STATUS=$?
  check_retval  $STATUS 0 

  compare_data $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME
done




\rm -f $X11_NAMES

result_log $MYNAME 
exit
