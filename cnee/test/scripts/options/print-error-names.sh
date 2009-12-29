#/bin/sh
#
#
# This file is generated from new_script.sh
#
#
# Filename:   opt_scripts/print-error-names.sh
# Generated:  Sun Mar 16 21:57:36 CET 2003
# 
#
#  Test the Xnee option
#       --print-error-names
#  By reading up X11 errors (name and number) from
#  the header file X.h. These values are compared
#  with the output from Xnee 
#

 
# name of myself
MYNAME=print-error-names.sh
 
# Arguments to test (short and long)
export LONG_ARG=--print-error-names
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
X11_NAMES=/tmp/xnee_errors.tmp
 



#
#
#  compare_error
#
#
#  Compare if the the name,number pair given
#  as argument is the same as given by Xnee --print-error-names
#  
# 
compare_error()
{
    EXP_ERR=$1
    EXP_ERR_NAME=$2
    MSG=$3


    TOTAL_TEST=`expr $TOTAL_TEST + 1`

    # Xnee's number,name pair is located in the file $XNEE_NAMES
    REC_ERR=`grep -w $EXP_ERR           $XNEE_NAMES | awk '{ print $1}'  | sed 's,[ ],,g'`
    REC_ERR_NAME=`grep -w $EXP_ERR_NAME $XNEE_NAMES | awk ' { print $2}' | sed 's,[ ],,g'`
    verbose "verifying error names and numbers"
    verbose " \"$EXP_ERR\"==\"$REC_ERR\""
    verbose " \"$EXP_ERR_NAME\"==\"$REC_ERR_NAME\""

    if [ "$REC_ERR" != "$EXP_ERR" ];
        then
        log " unexpected result: $MSG"
        ERR_TEST=`expr $ERR_TEST + 1`
        return 1
    fi
    if [ "$REC_ERR_NAME" != "$EXP_ERR_NAME" ];
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
#  compare_error_false
#
#
# 
compare_error_false()
{
    EXP_ERR=$1
    EXP_ERR_NAME=$2
    MSG=$3


    TOTAL_TEST=`expr $TOTAL_TEST + 1`

    # Xnee's number,name pair is located in the file $XNEE_NAMES
    REC_ERR=`grep -w $EXP_ERR           $XNEE_NAMES | awk '{ print $1}'  | sed 's,[ ],,g'`
    REC_ERR_NAME=`grep -w $EXP_ERR_NAME $XNEE_NAMES | awk ' { print $2}' | sed 's,[ ],,g'`
    verbose "verifying error names and numbers"
    verbose " \"$EXP_ERR\"==\"$REC_ERR\""
    verbose " \"$EXP_ERR_NAME\"==\"$REC_ERR_NAME\""

    if [ "$REC_ERR" = "$EXP_ERR" ];
	then
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
	return 1
    fi
    if [ "$REC_ERR_NAME" = "$EXP_ERR_NAME" ];
	then
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
	return 1
    fi
    verbose "success...."
    SUCC_TEST=`expr $SUCC_TEST + 1`
}



#
# main .... sort of
#

# get the error numbers and names from the devel header file
find_errors  /usr/include/X11/X.h  $X11_NAMES

LAST_ERROR=`cat $X11_NAMES | grep -v Extension | tail -1 | awk 'BEGIN {FS="="} ; { print $1}' | sed 's,[ ]*,,g' `

#
# positive test
#
#get the error numbers and names from Xnee
run_cnee $LONG_ARG | grep -e "^[0-9]" >  $XNEE_NAMES
STATUS=$?
# compare Xnee number and names with the system
for i in `cat $X11_NAMES`
do
  EV_=`echo $i | sed 's,\([0-9]*\)=.*,\1,g'`
  NAME_=`echo $i | sed 's,[0-9]*=\([a-zA-Z0-9]*\),\1,g'`
  if [ "$NAME_" = "$LAST_ERROR" ];
      then
      verbose " -- comparing (false): \"$EV_\" \"$NAME_\" "
      compare_error_false $EV_ $NAME_ "$EV_ $NAME_ "
  else
      verbose " -- comparing: \"$EV_\" \"$NAME_\" "
      compare_error $EV_ $NAME_ "$EV_ $NAME_ "
  fi
done
check_retval $STATUS 0 

result_log $MYNAME 


result_log $MYNAME 



# positive test
run_cnee $SHORT_ARG | grep -e "^[0-9]" > $XNEE_NAMES.2
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
