#!/bin/sh
#
#
# Filename:   options/print-error-name.sh
# Generated:  
# 
# Description:
#
#  Test the Xnee option
#       --print-error-name
#  By reading up X11 error (name and number) from
#  the header file X.h. These values are compared
#  with the output from Xnee 
#
#
 
 
 
 
 
# name of myself
MYNAME=print-error-name.sh
 
# Arguments to test (short and long)
export LONG_ARG=--print-error-name
export SHORT_ARG=-pern
 
# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the xnee test dir "
    exit
else 
    . etc/base_funs
fi 
 
##init_test 
parse_me "$*"

#######################################################################


X11_NAMES=/tmp/xnee_errors.tmp
 




#
# main .... sort of
#

# get the error numbers and names from the devel header file
find_errors  /usr/include/X11/X.h  $X11_NAMES
LAST_ERROR=`cat $X11_NAMES | grep -v Extension | tail -1 | awk 'BEGIN {FS="="} ; { print $1}' | sed 's,[ ]*,,g' `



#
# positive tests
#
# compare Xnee number and names with the system
for i in `cat $X11_NAMES`
do
  ER_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_ER=`$XNEE $LONG_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 
  

  XNEE_ER_NAME=`$XNEE $LONG_ARG $ER_`
  STATUS=$?
  check_retval $STATUS 0 
  compare_data "$ER_" "$NAME_"  "$XNEE_ER" "$XNEE_ER_NAME"

done





# compare Xnee number and names with the system
for i in `cat $X11_NAMES`
do
  ERR_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_ERR=`$XNEE $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval $STATUS 0 

  XNEE_ERR_NAME=`$XNEE $SHORT_ARG $ERR_`
  STATUS=$?
  check_retval $STATUS 0 

  compare_data $ERR_ $NAME_  $XNEE_ERR $XNEE_ERR_NAME
done




# loop through all errors and run them in a Xnee renaming pipe 
# Make sure that 2 == 2   
# after this has been done:  2 --xnee--> BadFont --xnee--> 2
# loop from 1 to BadImplementation

TMP=1
while [ "$TMP" != "$LAST_ERROR" ];
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
for i in -12=bela  67=pettersson 127=dimitri 200=shostakovich
do
  ERR_=`echo $i | sed 's,\([-0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[-0-9]*=\([a-zA-Z0-9]*\),\1,g'`

  XNEE_ERR=`$XNEE $LONG_ARG $NAME_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_ERR" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`

  XNEE_ERR_NAME=`$XNEE $LONG_ARG $ERR_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_ERR_NAME" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  

  # ###
  # Xnee returns no value so we can't compare
  # ### 
  #  compare_data_false $ERR_ $NAME_  $XNEE_ERR $XNEE_ERR_NAME 


  XNEE_ERR=`$XNEE $SHORT_ARG $NAME_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_ERR" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`
  


  XNEE_ERR_NAME=`$XNEE $SHORT_ARG $ERR_`
  STATUS=$?
  check_retval_false $STATUS 0
  if [ "$XNEE_ERR_NAME" != "" ];
      then
      ERR_TEST=`expr $ERR_TEST + 1`
  else
      SUCC_TEST=`expr $SUCC_TEST + 1`
  fi
  TOTAL_TEST=`expr $TOTAL_TEST + 1`



# ###
# Xnee returns no value so we can't compare
# ### 
#  compare_data_false $ERR_ $NAME_  $XNEE_ERR $XNEE_ERR_NAME 
done




\rm -f $X11_NAMES
result_log $MYNAME 
exit

