#/bin/sh
#
#
# This file is generated from new_script.sh
# and updated by hand by hesa
#
# Filename:   opt_scripts/print-request-name.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
# Description:
#
#  Tests the Xnee option
#       --print-request-name
#  By reading up X11 events (name and number) from
#  the header file X.h. These values are compared
#  with the output from Xnee 
#
#
 
 
 
 
 
# name of myself
MYNAME=print-request-name.sh
 
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
# compare Xnee number and names with the system
for i in `cat $X11_NAMES`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_EV=`$XNEE $LONG_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 "$XNEE $LONG_ARG $NAME_"

  XNEE_EV_NAME=`$XNEE $LONG_ARG $EV_`
  STATUS=$?
  check_retval $STATUS 0 "$XNEE $LONG_ARG $EV_"

  compare_data $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME "$MYNAME"

done


# compare Xnee number and names with the system
# short args
for i in `cat $X11_NAMES`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_EV=`$XNEE $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 

  XNEE_EV_NAME=`$XNEE $SHORT_ARG $EV_`
  STATUS=$?
  check_retval $STATUS 0 

  compare_data $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME
done



# loop through all requests and run them in a Xnee renaming pipe 
# Make sure that 2 == 2   
# after this has been done:  2 --xnee--> 'requestname' --xnee--> 2
# loop from  to 

TMP=1
while [ "$TMP" != "$LAST_REQ" ];
do
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  REC=`$XNEE $LONG_ARG  $TMP | xargs $XNEE $SHORT_ARG  `
  STATUS=$?
  check_retval $STATUS 0 
  if [ "$REC" != "$TMP" ];
      then
      log " unexpected result: using  \"$XNEE $LONG_ARG  $TMP | $XNEE $SHORT_ARG $TMP\""
      log " got: $REC ..... wanted: $TMP"
      ERR_TEST=`expr $ERR_TEST + 1`
  else
    SUCC_TEST=`expr $SUCC_TEST + 1`
  fi



  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  REC=`$XNEE $SHORT_ARG  $TMP | xargs $XNEE $LONG_ARG  `
  STATUS=$?
  check_retval $STATUS 0 
  if [ "$REC" != "$TMP" ];
      then
      log " unexpected result: using  \"$XNEE $SHORT_ARG  $TMP | $XNEE $LONG_ARG $TMP\""
      log " got: $REC ..... wanted: $TMP"
      ERR_TEST=`expr $ERR_TEST + 1`
  else
    SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  

  
  TMP=`expr $TMP + 1`

done




#
# negative tests
#
for i in 0=bartok -12=bela  200=shostakovich
do
  EV_=`echo $i | sed 's,\([-0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[-0-9]*=\([a-zA-Z0-9]*\),\1,g'`


  XNEE_EV=`$XNEE $LONG_ARG $NAME_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_EV" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`

  XNEE_EV_NAME=`$XNEE $LONG_ARG $EV_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_EV_NAME" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  
# ###
# Xnee returns no value so we can't compare
# ### 
#  compare_data_false $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME 


  XNEE_EV=`$XNEE $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_EV" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  
  XNEE_EV_NAME=`$XNEE $SHORT_ARG $EV_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_EV_NAME" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`

done


# more neg tests.....
#for i in    1=allan 67=pettersson 127=dimitri 
for i in    1=allan 
do
  EV_=`echo $i | sed 's,\([-0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[-0-9]*=\([a-zA-Z0-9]*\),\1,g'`


  XNEE_EV=`$XNEE $LONG_ARG $NAME_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_EV" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`


  XNEE_EV_NAME=`$XNEE $LONG_ARG $EV_`
  STATUS=$?
  check_retval $STATUS 0
  if [ "$XNEE_EV_NAME" == "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  
# ###
# Xnee returns no value so we can't compare
# ### 
#  compare_data_false $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME 

  XNEE_EV=`$XNEE $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_EV" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  


  XNEE_EV_NAME=`$XNEE $SHORT_ARG $EV_`
  STATUS=$?
  check_retval $STATUS 0
  if [ "$XNEE_EV_NAME" == "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`

# ###
# Xnee returns no value so we can't compare
# ### 
#  compare_data_false $EV_ $NAME_  $XNEE_EV $XNEE_EV_NAME 
done


\rm -f $X11_NAMES
result_log $MYNAME 
exit
