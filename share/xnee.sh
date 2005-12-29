MY_FIFO=/tmp/xnee_${USER}.$$
CNEE=cnee


xnee_init_fifo()
{
    rm -f $MY_FIFO
    touch $MY_FIFO
    $CNEE --replay --file $MY_FIFO   &
    CNEE_PID=$!
}

xnee_init_file()
{
    rm -f $MY_FIFO
    mkfifo $MY_FIFO
    (tail -f $MY_FIFO | $CNEE --replay --file stdin) &
    CNEE_PID=$!
}

fifo_write()
{
    echo "$*" >> $MY_FIFO
}

close_down()
{
    rm -f $MY_FIFO
    kill $CNEE_PID
}

xnee_move_mouse()
{
    MSEC=2
    if [ "$2" = "" ]
	then
	return
    fi
    X_POS=$1
    Y_POS=$2

    fifo_write "fake-motion x=$X_POS y=$Y_POS msec=$MSEC" 
}

xnee_fake_key()
{
    MSEC=2
    if [ "$1" = "" ]
	then
	return
    fi
    KEY=$1

    fifo_write "fake-key key=$KEY msec=$MSEC"
}

xnee_fake_key_press()
{
    MSEC=2
    if [ "$1" = "" ]
	then
	return
    fi
    KEY=$1

    fifo_write "fake-key-press key=$KEY msec=$MSEC"
}

xnee_fake_key_release()
{
    MSEC=2
    if [ "$1" = "" ]
	then
	return
    fi
    KEY=$1

    fifo_write "fake-key-release key=$KEY msec=$MSEC"
}
