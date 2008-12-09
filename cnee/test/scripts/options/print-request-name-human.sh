#/bin/sh
#
#
#  Tests the Xnee option
#       --print-request-name-human
#  By reading up X11 events (name and number) from
#  the header file X.h. These values are compared
#  with the output from Xnee 
#
#
 
 
 
# name of myself
MYNAME=print-request-name-human.sh
 
# Arguments to test (short and long)
export LONG_ARG=--print-request-name
export SHORT_ARG=-prn
 
# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the xnee test dir "
    exit
else 
    . etc/base_funs
fi 
 
init_test $MY_NAME 
parse_me "$*"

#######################################################################


X11_NAMES=/tmp/xnee_requests.tmp
 




#
# main .... sort of
#

# get the event numbers and names from the devel header file
find_requests  /usr/include/X11/Xproto.h $X11_NAMES

LAST_REQ=`grep X_GetModifierMapping /usr/include/X11/Xproto.h | awk '{ print $3}' | sed 's,[ ]*,,g' `




#
# positive tests of long args
#
for i in `cat $X11_NAMES`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_EV=`run_cnee -hp $LONG_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 "run_cnee $LONG_ARG $NAME_"

  XNEE_EV_NAME=`run_cnee -hp $LONG_ARG $EV_`
  STATUS=$?
  check_retval $STATUS 0 "run_cnee $LONG_ARG $EV_"

done


for i in `cat $X11_NAMES`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_EV=`run_cnee -hp $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 

  XNEE_EV_NAME=`run_cnee -hp $SHORT_ARG $EV_`
  STATUS=$?
  check_retval $STATUS 0 

done




\rm -f $X11_NAMES
result_log $MYNAME 
exit
