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
    LIBXNEE_VERSION=$(ls -d libxnee*.tar.gz | sed 's,libxnee-\(.*\)\.tar\.gz,\1,g')
    LIBXNEE_FILE=libxnee-$LIBXNEE_VERSION.tar.gz
    LIBXNEE_DIR=libxnee-$LIBXNEE_VERSION
    
    CNEE_VERSION=$(ls -d cnee*.tar.gz | sed 's,cnee-\(.*\)\.tar\.gz,\1,g')
    CNEE_FILE=cnee-$CNEE_VERSION.tar.gz
    CNEE_DIR=cnee-$CNEE_VERSION
    
    GNEE_VERSION=$(ls -d gnee*.tar.gz | sed 's,gnee-\(.*\)\.tar\.gz,\1,g')
    GNEE_FILE=gnee-$GNEE_VERSION.tar.gz
    GNEE_DIR=gnee-$GNEE_VERSION

    DOC_VERSION=$(ls -d xnee-doc-src*.tar.gz | sed 's,xnee-doc-src-\(.*\)\.tar\.gz,\1,g')
    DOC_FILE=xnee-doc-src-$DOC_VERSION.tar.gz
    DOC_DIR=xnee-doc-src-2.0-$DOC_VERSION

    log "Building libxnee ($LIBXNEE_VERSION)"
    exec_and_check_status tar zxvf $LIBXNEE_FILE 
    exec_and_check_status cd libxnee-$LIBXNEE_VERSION 
    exec_and_check_status ./configure
    exec_and_check_status make
    cd ..
    
    log "Building cnee (cnee $CNEE_VERSION based on libxnee $LIBXNEE_VERSION)"
    exec_and_check_status tar zxvf $CNEE_FILE 
    exec_and_check_status cd cnee-$CNEE_VERSION 
    exec_and_check_status ./configure --with-libxnee-dir=${BUILD_BASE_DIR}/${LIBXNEE_DIR}
    exec_and_check_status make
    cd ..
    
    log "Building gnee (gnee $GNEE_VERSION based on libxnee $LIBXNEE_VERSION)"
    exec_and_check_status tar zxvf $GNEE_FILE 
    exec_and_check_status cd gnee-$GNEE_VERSION 
    exec_and_check_status ./configure --with-libxnee-dir=${BUILD_BASE_DIR}/${LIBXNEE_DIR}
    exec_and_check_status make
    cd ..
    
    log "Building doc from $DOC_FILE in `pwd`"
    exec_and_check_status tar zxvf $DOC_FILE 
    exec_and_check_status cd xnee-doc-src-$DOC_VERSION 
    exec_and_check_status ./configure
    exec_and_check_status make
    cd ..
    
    EXEC_CNEE_VERSION=$($CNEE_DIR/src/cnee --version 2>&1| grep using | grep cnee | awk ' { print $2 ; } ' )
    version_check "cnee" "$CNEE_VERSION" "$EXEC_CNEE_VERSION"
}


dist_all()
{
    log "Building libxnee dist"
    exec_and_check_status cd libxnee
    exec_and_check_status make -f Makefile.cvs 
    exec_and_check_status ./configure    
    exec_and_check_status make clean all dist
    cd ..

    log "Building cnee dist"
    exec_and_check_status cd cnee
    exec_and_check_status make -f Makefile.cvs 
    exec_and_check_status ./configure    
    exec_and_check_status make clean all dist
    cd ..

    log "Building gnee dist"
    exec_and_check_status cd gnee
    exec_and_check_status ./autogen.sh --with-libxnee-dir=${LIBXNEE_SRC_DIR}
    exec_and_check_status make clean all dist
    cd ..

    log "Building doc dist"
    exec_and_check_status cd doc
    exec_and_check_status make -f Makefile.cvs 
    exec_and_check_status ./configure    
    exec_and_check_status make clean all dist doc-deliv
    cd ..

}

clean_dist_files()
{
    rm -f libxnee/libxnee-*.tar.gz 
    rm -f cnee/cnee-*.tar.gz 
    rm -f gnee/gnee-*.tar.gz 
    rm -f doc/xnee-doc-*.tar.gz 
}

deliver_dist()
{
    exec_and_check_status rm -fr /tmp/xnee_rel
    exec_and_check_status mkdir /tmp/xnee_rel
    exec_and_check_status cp libxnee/libxnee-*.tar.gz /tmp/xnee_rel/
    exec_and_check_status cp cnee/cnee-*.tar.gz /tmp/xnee_rel/
    exec_and_check_status cp gnee/gnee-*.tar.gz /tmp/xnee_rel/
    exec_and_check_status cp doc/xnee-doc-*.tar.gz /tmp/xnee_rel/
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


