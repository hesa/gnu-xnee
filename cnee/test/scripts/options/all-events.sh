#/bin/sh
#
#
# This file is generated from new_scrip.sh
#
#
# Filename:   opt_scripts/all-events.sh
# Generated:  Sun Mar 16 20:17:14 CET 2003
# 
#
 
 
# Arguments to test (short and long)
export LONG_ARG=--all-events
export SHORT_ARG=-ae
 
# name of myself
MYNAME=all-events.sh

# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the current dir (or one dir up)  "
    exit
else 
   . etc/base_funs
fi 
 
init_test $MYNAME

parse_me "$*"

#
#
#

XNEE_STDOUT=/tmp/xnee_arg.tmp

function check_arg()
{
    FILE=$1
    EXPECTED=$2
    MSG=$3
    
    verbose "testing: $1 - $2 - $3"
    TOTAL_TEST=`expr $TOTAL_TEST + 1`
    RESULT=`grep "# all-events" $FILE | awk '{ printf "%s\n", $3}' | sed 's,[ ]*,,g'`
    if [ "$RESULT" != "$EXPECTED" ] ;
	then 
	log " unexpected result: $MSG"
	ERR_TEST=`expr $ERR_TEST + 1`
    else
	verbose "success...."
	SUCC_TEST=`expr $SUCC_TEST + 1`
    fi

}


# positive test
$XNEE $LONG_ARG --record  --loops 0 > $XNEE_STDOUT
STATUS=$?
check_retval $STATUS 0 
check_arg $XNEE_STDOUT 1 "$LONG_ARG"

# positive test
$XNEE $SHORT_ARG --record --loops 0 > $XNEE_STDOUT
check_retval $STATUS 0 
check_arg  $XNEE_STDOUT 1 "$SHORT_ARG"

# negative test
$XNEE --record --device-event-range 6 --loops 0 > $XNEE_STDOUT
check_retval $STATUS 0 
check_arg  $XNEE_STDOUT 0 "$SHORT_ARG"


\rm -f  $XNEE_STDOUT

result_log $MYNAME 

