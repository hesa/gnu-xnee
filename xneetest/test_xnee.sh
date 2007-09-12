#!/bin/bash

. ./functions_etc

make_ready 

echo "Minimize all windows...."
echo "... starting in 5 secs"
count_down 2
xterm -T "Start testgui here" -geometry -0-0 &
sleep 2


err "Launching cnee replay "
$XNEE_CLI --replay -rwp --file sessions/test${TEST_VERSION_NR}.xns 
#--verbose 2>/tmp/xnee.err 

exit

exit
RES_FILE=resultlogs/test${TEST_VERSION_NR}.log 
#err "Launching test program $XNEE_TEST_PROG  ($RES_FILE)"

#rm $RES_FILE
#$XNEE_TEST_PROG > $RES_FILE &

echo "mv test.log $RES_FILE"
