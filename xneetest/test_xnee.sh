#!/bin/bash

XNEE_CLI=../cnee/src/cnee
XNEE_TEST_PROG=./TestGui/src/testgui
TEST_XNEE_ERROR=1

err()
{
    echo "$*" 1>&2
}

finish_up()
{
    err "Cleaning up......"
    killall -9 testgui
    killall -9 cnee
}

make_ready()
{
    RES_FILE=$1
    
    finish_up
    if [ ! -f ${XNEE_CLI} ];
	then
	err "$XNEE_CLI not present... making."
	pushd ../
	if [ ! -f Makefile ];
	    then
	    make -f Makefile.cvs
	    ./configure
	fi
	make 
	popd
	if [ ! -f ${XNEE_CLI} ];
	    then
	    err "$XNEE_CLI not present after making it... bailing out."
	    exit $TEST_XNEE_ERROR
	fi
    fi

    err "Launching test program $XNEE_TEST_PROG"
    rm $RES_FILE
    $XNEE_TEST_PROG > $RES_FILE &
    sleep 5
}


make_ready resultlogs/test1.log
$XNEE_CLI --replay --file sessions/test1.xns
sleep 2
diff -s resultlogs/test1.log logs/test1.log > diff.log
DIFF_LINES=$(cat diff.log | grep -v identical | wc -l )
echo "------------------------"
echo "Lines diff: $DIFF_LINES"
cat diff.log
echo "------------------------"
finish_up

