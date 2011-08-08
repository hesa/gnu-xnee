#!/bin/sh

XNEE_DIST=$1
XNEE_DIR=$(basename $XNEE_DIST | sed 's,\.tar\.gz,,g')

DEFAULT_CONF_GLAGS="--prefix=/tmp/xnee_build_test"

if [ "$XNEE_DIST" = "" ]
then
    echo "give the dist file as argument"
    exit
fi

BUILD_DIR=.

LOG_FILE=$(pwd)/build.log
rm $LOG_FILE

SUCC=0

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
	SUCC=$(( $SUCC + 1 ))
    fi
}


config_and_make()
{
    SUB_START_TIME=$(date '+%s')
    NR_OF_BUILDS=$(( $NR_OF_BUILDS + 1 ))
    CONFIG_FLAGS=$*
    
    rm -fr ./$XNEE_DIR/
    exit_if_error $? " removing $XNEE_DIR"

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
    SUB_STOP_TIME=$(date '+%s')
    echo "   make (nr $NR_OF_BUILDS, $SUCC tests)  took $(($SUB_STOP_TIME - $SUB_START_TIME)) seconds"  >> $LOG_FILE 
    echo " " >> $LOG_FILE 
}


mkdir $BUILD_DIR
cd    $BUILD_DIR


GUI=--enable-gui
NO_GUI=--disable-gui
CLI=--enable-cli
NO_CLI=--disable-cli
APPLET=--enable-gnome-applet
NO_APPLET=--disable-gnome-applet
DOC=--enable-doc
NO_DOC=--disable-doc
VERB=--enable-verbose
NO_VERB=--disable-verbose

START_TIME=$(date '+%s')
NR_OF_BUILDS=0

config_and_make
config_and_make --enable-doc-only

config_and_make  $VERB      $GUI       $CLI       $APPLET       $DOC
config_and_make  $VERB      $GUI       $CLI       $APPLET       $NO_DOC
config_and_make  $VERB      $GUI       $CLI       $NO_APPLET    $DOC
config_and_make  $VERB      $GUI       $CLI       $NO_APPLET    $NO_DOC
config_and_make  $VERB      $GUI       $NO_CLI    $APPLET       $DOC
config_and_make  $VERB      $GUI       $NO_CLI    $APPLET       $NO_DOC
config_and_make  $VERB      $GUI       $NO_CLI    $NO_APPLET    $DOC
config_and_make  $VERB      $GUI       $NO_CLI    $NO_APPLET    $NO_DOC
config_and_make  $VERB      $NO_GUI    $CLI       $APPLET       $DOC
config_and_make  $VERB      $NO_GUI    $CLI       $APPLET       $NO_DOC
config_and_make  $VERB      $NO_GUI    $CLI       $NO_APPLET    $DOC
config_and_make  $VERB      $NO_GUI    $CLI       $NO_APPLET    $NO_DOC
config_and_make  $VERB      $NO_GUI    $NO_CLI    $APPLET       $DOC
config_and_make  $VERB      $NO_GUI    $NO_CLI    $APPLET       $NO_DOC
config_and_make  $VERB      $NO_GUI    $NO_CLI    $NO_APPLET    $DOC
config_and_make  $VERB      $NO_GUI    $NO_CLI    $NO_APPLET    $NO_DOC
config_and_make  $NO_VERB   $GUI       $CLI       $APPLET       $DOC
config_and_make  $NO_VERB   $GUI       $CLI       $APPLET       $NO_DOC
config_and_make  $NO_VERB   $GUI       $CLI       $NO_APPLET    $DOC
config_and_make  $NO_VERB   $GUI       $CLI       $NO_APPLET    $NO_DOC
config_and_make  $NO_VERB   $GUI       $NO_CLI    $APPLET       $DOC
config_and_make  $NO_VERB   $GUI       $NO_CLI    $APPLET       $NO_DOC
config_and_make  $NO_VERB   $GUI       $NO_CLI    $NO_APPLET    $DOC
config_and_make  $NO_VERB   $GUI       $NO_CLI    $NO_APPLET    $NO_DOC
config_and_make  $NO_VERB   $NO_GUI    $CLI       $APPLET       $DOC
config_and_make  $NO_VERB   $NO_GUI    $CLI       $APPLET       $NO_DOC
config_and_make  $NO_VERB   $NO_GUI    $CLI       $NO_APPLET    $DOC
config_and_make  $NO_VERB   $NO_GUI    $CLI       $NO_APPLET    $NO_DOC
config_and_make  $NO_VERB   $NO_GUI    $NO_CLI    $APPLET       $DOC
config_and_make  $NO_VERB   $NO_GUI    $NO_CLI    $APPLET       $NO_DOC
config_and_make  $NO_VERB   $NO_GUI    $NO_CLI    $NO_APPLET    $DOC
config_and_make  $NO_VERB   $NO_GUI    $NO_CLI    $NO_APPLET    $NO_DOC

STOP_TIME=$(date '+%s')
both()
{
    echo "$*"
    echo "$*" >> $LOG_FILE
}
both "Tested for $(($STOP_TIME - $START_TIME)) seconds"  
both "Tested $NR_OF_BUILDS different config/builds"
both "       sub tests: $SUCC"
both ".... bye"
