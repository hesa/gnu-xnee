#!/bin/sh

XNEE_DIST=$1
XNEE_DIR=$(echo $XNEE_DIST | sed 's,\.tar\.gz,,g')

DEFAULT_CONF_GLAGS="--prefix=/tmp/xnee_build_test"

if [ "$XNEE_DIST" = "" ]
then
    echo "give the dist file as argument"
    exit
fi

BUILD_DIR=.

LOG_FILE=$(pwd)/build.log
rm $LOG_FILE

exit_if_error()
{
    RET=$1
    MESS=$2


    if [ "$RET" != "0" ]
	then
	echo "** FAILURE **  ($RET)"
        printf " Failed:   $MESS\n"
	exit
    else
	printf " Success:   $MESS \n"  >> $LOG_FILE
    fi
}

config_and_make()
{
    CONFIG_FLAGS=$1
    
    tar zxvf $XNEE_DIST
    exit_if_error $? " untar the file $XNEE_DIST"

    cd $XNEE_DIR
    exit_if_error $? " cd to $XNEE_DIR"

    ./configure $CONFIG_FLAGS $DEFAULT_CONF_GLAGS
    exit_if_error $? " configure with the following flags\n   $CONFIG_FLAGS $DEFAULT_CONF_GLAGS"
    
    make 
    exit_if_error $? " make"

#    make test
#    exit_if_error $? " make test"

    make install 
    exit_if_error $? " make install"

    cd -
}


mkdir $BUILD_DIR
cd    $BUILD_DIR


config_and_make
config_and_make --disable-doc
config_and_make --enable-gnome-panel
config_and_make --disable-gui --disable-cli --disable-gnome-panel
config_and_make --disable-gui --disable-cli --disable-gnome-panel --disable-doc

