#!/bin/sh

test_program()
{
    if [ ! -x $1 ]
    then
	echo "Program $1 does not exists, or is not executable"
	return 1
    fi

    xdpyinfo >/dev/null 2>&1 
    if [ "$?" != "0" ]
    then
	echo "Problem with X server (prob authorisation, or no X up and running)"
	echo "   when executing program $1"
	echo "   .... bailing out"
	return 1
    fi
    
    $1
}

if [ ! -f ./.libs/libtestcb.so ]
then
    cp ./.libs/libtestcb_la-callback_so.o ./.libs/libtestcb.so
fi
export LD_LIBRARY_PATH=./.libs

test_program $1
exit 0

