#/bin/sh
#
#
# Filename:   retype/keyboard.sh
# Created:  Fri Aug  8 00:04:38 CEST 2003
# 
# Description:
#
#  Test the Xnee functionality
#       replay recorded keyboard motions
#
#
 
 
 
 
# name of myself
MYNAME=keyboard.sh
 
 
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

KEYBOARD_DEVICE=/dev/swkeybd



function retype_file()
{
    RECORDED_FILE=$1
    
    verbose "replaying file $RECORDED_FILE"
    launch_cat_wrapper    /tmp/xnee_test.txt

    sleep 4
    verbose "  launching cnee"
    $XNEE --retype-file scripts/retype/onefile.txt
    sleep 2 
    verbose "  faking control-d"
    fake_controld
    sleep 1
}


# main ..... I guess

verify_device swkeybd

retype_file  $FILE


DIFF_LINES=$(sdiff -s  /tmp/xnee_test.txt scripts/retype/onefile.txt | wc -l)


verbose "Verifying the lengths of $LEN_STRING $LEN_FAKE"
verify_same $DIFF_LINES 0

result_log $MYNAME 
exit
