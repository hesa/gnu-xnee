#!/bin/bash

SRC_DIR=~/gnu/
CONF_ARG="--disable-doc --disable-gui --disable-gnome-applet --disable-xinput2"
OUTPUT_DIR=~/buildlogs/xnee/
DATE=$(date '+%Y-%m-%d')
START_DIR=$(pwd)
NODE=$(uname -n)

while [ "$*" != "" ]
do
    case $1 in
	"--output-dir")
	    OUTPUT_DIR=$2
	    shift
	    ;;
	
	"--src-dir")
	    SRC_DIR=$2
	    shift
	    ;;
	
	"--node")
	    NODE=$2
	    shift
	    ;;
	
	*)
	    echo " discarding $*"
	    ;;
    esac
    shift
done



RES_DIR=$OUTPUT_DIR/$DATE/$NODE/
BUILD_LOG=$RES_DIR/xnee-build.txt
SYS_LOG=$RES_DIR/sys-info-long.txt
BRIEF_LOG=$RES_DIR/sys-info.txt
BUILD_INFO=$RES_DIR/build-time.txt


debug_settings()
{
    echo "SRC: $SRC_DIR"
    echo "RES_DIR: $RES_DIR"
    echo "BUILD_LOG: $BUILD_LOG"
    echo "SYS_LOG: $SYS_LOG"
    echo "BRIEF_LOG: $BRIEF_LOG"
    exit
}

init()
{
    makedir $RES_DIR
    rm -f $SYS_LOG
    rm -f $BRIEF_LOG
    rm $BUILD_LOG
}

makedir()
{
    if [ ! -d $1 ]
    then
	mkdir -p $1
    fi
}

log()
{
    makedir $RES_DIR
    echo "[Xnee remote-build.sh   $(date)]: $*" >> $BUILD_LOG
    echo "$*" 
}

check_ret()
{
    INRET=$1
    INCMD=$2

    if [ "$RET" != "0" ]
	then
	log "ERROR: $INCMD Failed ($INRET)"
	log " ... bailing out"
	exit $INRET
    fi
}

log_exec()
{
    log "$*"
    $*  >>  $BUILD_LOG 2>&1
}


log_exec_check()
{
    log "$*"
    $*  >>  $BUILD_LOG 2>&1
    RET=$?
    check_ret $RET "$*"
}

syslog()
{
    echo "$*" >> $SYS_LOG
}

syslog_command()
{
    syslog " "
    syslog " "
    syslog "COMMAND: \"$*\" "
    syslog "-----------------------------------------------"
    $*  >>  $SYS_LOG 2>&1
}

brief_log()
{
    echo "$*" >> $BRIEF_LOG
}

brief_log_command()
{
    $* >> $BRIEF_LOG
}

setup_cvs()
{
    log_exec_check cd $SRC_DIR
    
    if [ ! -d xnee ]
    then
	log_exec_check cvs -z3 -d:pserver:anonymous@cvs.savannah.gnu.org:/sources/xnee co xnee
	log_exec_check cd xnee
    else
	log_exec_check cd xnee
	log_exec_check cvs update -d
    fi
}

get_sys_info()
{
    brief_log_command  uname -a
    syslog_command date
    syslog_command uname -a
    syslog_command linuxinfo
    syslog_command cat /proc/cpuinfo
    syslog_command ldd $SRC_DIR/xnee/cnee/src/cnee
    syslog_command gcc --version
    syslog_command automake --version
    syslog_command autoconf --version
    syslog_command pkg-config --print-provides x11
    syslog_command pkg-config --print-provides xcb
    syslog_command pkg-config --print-provides xtst
    syslog_command pkg-config --print-provides xext
}


START_SEC=$(date '+%s')

init

get_sys_info

setup_cvs

#log_exec_check make -f Makefile.cvs

#log_exec_check ./configure $CONF_ARG

#log_exec_check make

STOP_SEC=$(date '+%s')

brief_log "Building GNU Xnee took: $(( $STOP_SEC - $START_SEC )) seconds." 
