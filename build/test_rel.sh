#!/bin/sh

BUILD_LOG=`pwd`/build.log
rm $BUILD_LOG


BASE_DIR=$(pwd)

LIBXNEE_SRC_DIR=${BASE_DIR}/libxnee
GNEE_SRC_DIR=${BASE_DIR}/gnee
CNEE_SRC_DIR=${BASE_DIR}/cnee



test_if_right_build_dir()
{
    TEST_DIR=$1
    if [ ! -d ${TEST_DIR} ];
	then 
	echo "Could not find ${TEST_DIR}"
	exit 1
    fi
}

test_if_right_build_dirs()
{
    test_if_right_build_dir ${LIBXNEE_SRC_DIR}
    test_if_right_build_dir ${GNEE_SRC_DIR}
    test_if_right_build_dir ${CNEE_SRC_DIR}
}


log()
{
    echo "$*" >> $BUILD_LOG
    echo "$*" 
}

log "Removing old dirs"
rm -fr $GNEE_DIR $CNEE_DIR $LIBXNEE_DIR

exec_and_check_status()
{
    log "  Executing: $*"
    $* 1>> $BUILD_LOG 2>> $BUILD_LOG
    if [ "$?" != "0" ];
	then
	log "Failure:"
	log "  $*"
	exit
    fi
}


version_check()
{
    if [ "$2" !=  "$3" ];
	then 
	log "Failure: $1 versions differ"
	log "    from tar.gz:    $1"
	log "    binary:         $2"
    fi
}

build_all()
{
    BUILD_BASE_DIR=$(pwd)
    XNEE_VERSION=$(ls -d Xnee*.tar.gz | sed 's,Xnee-\(.*\)\.tar\.gz,\1,g')
    XNEE_FILE=Xnee-$XNEE_VERSION.tar.gz
    XNEE_DIR=Xnee-$XNEE_VERSION
    
#    DOC_VERSION=$(ls -d Xnee-doc-src*.tar.gz | sed 's,Xnee-doc-src-\(.*\)\.tar\.gz,\1,g')
#    DOC_FILE=Xnee-doc-src-$DOC_VERSION.tar.gz
#    DOC_DIR=Xnee-doc-src-2.0-$DOC_VERSION

    log "Building xnee ($XNEE_VERSION)"
    exec_and_check_status tar zxvf $XNEE_FILE 
#    exec_and_check_status cd Xnee-$XNEE_VERSION 
    exec_and_check_status ./configure
    exec_and_check_status make
#    cd ..
    
#    log "Building doc from $DOC_FILE in `pwd`"
#    exec_and_check_status tar zxvf $DOC_FILE 
#    exec_and_check_status cd Xnee-doc-src-$DOC_VERSION 
#    exec_and_check_status ./configure
#    exec_and_check_status make
#    cd ..
    
    EXEC_CNEE_VERSION=$($CNEE_DIR/src/cnee --version 2>&1| grep using | grep cnee | awk ' { print $2 ; } ' )
    version_check "cnee" "$CNEE_VERSION" "$EXEC_CNEE_VERSION"
}


dist_all()
{
    log "Building xnee dist"
    exec_and_check_status make -f Makefile.cvs 
    exec_and_check_status ./configure    
    exec_and_check_status make clean all dist
}

clean_dist_files()
{
    rm -f doc/*.tar.gz 
}

deliver_dist()
{
    exec_and_check_status rm -fr /tmp/xnee_rel
    exec_and_check_status mkdir /tmp/xnee_rel
    exec_and_check_status cp Xnee-*.tar.gz /tmp/xnee_rel/
}

log "Starting build at `date`" 

test_if_right_build_dirs
clean_dist_files
dist_all
deliver_dist
pushd /tmp/xnee_rel
build_all
popd
log "build fisnihed at `date`" 


