#!/bin/sh

BUILD_LOG=`pwd`/build.log
rm $BUILD_LOG

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
    BASE_DIR=$(pwd)
    LIBXNEE_VERSION=$(ls -d libxnee*.tar.gz | sed 's,libxnee-\(.*\)\.tar\.gz,\1,g')
    LIBXNEE_FILE=libxnee-$LIBXNEE_VERSION.tar.gz
    LIBXNEE_DIR=libxnee-$LIBXNEE_VERSION
    
    CNEE_VERSION=$(ls -d cnee*.tar.gz | sed 's,cnee-\(.*\)\.tar\.gz,\1,g')
    CNEE_FILE=cnee-$CNEE_VERSION.tar.gz
    CNEE_DIR=cnee-$CNEE_VERSION
    
    GNEE_VERSION=$(ls -d gnee*.tar.gz | sed 's,gnee-\(.*\)\.tar\.gz,\1,g')
    GNEE_FILE=gnee-$GNEE_VERSION.tar.gz
    GNEE_DIR=gnee-$GNEE_VERSION

    log "Building libxnee ($LIBXNEE_VERSION)"
    exec_and_check_status tar zxvf $LIBXNEE_FILE 
    exec_and_check_status cd libxnee-$LIBXNEE_VERSION 
    exec_and_check_status ./configure
    exec_and_check_status make
    cd ..
    
    log "Building cnee ($CNEE_VERSION based on libxnee $LIBXNEE_VERSION)"
    exec_and_check_status tar zxvf $CNEE_FILE 
    exec_and_check_status cd cnee-$CNEE_VERSION 
    exec_and_check_status ./configure --with-libxnee-dir=${BASE_DIR}/${LIBXNEE_DIR}
    exec_and_check_status make
    cd ..
    
    log "Building gnee ($GNEE_VERSION based on libxnee $LIBXNEE_VERSION)"
    exec_and_check_status tar zxvf $GNEE_FILE 
    exec_and_check_status cd gnee-$GNEE_VERSION 
    exec_and_check_status ./configure --with-libxnee-dir=${BASE_DIR}/${LIBXNEE_DIR}
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

    log "Building cnee dist"
    exec_and_check_status cd gnee
    exec_and_check_status make -f Makefile.cvs 
    exec_and_check_status ./configure    
    exec_and_check_status make clean all dist
    cd ..

}

deliver_dist()
{
    exec_and_check_status rm -fr /tmp/xnee_rel
    exec_and_check_status mkdir /tmp/xnee_rel
    exec_and_check_status cp libxnee/libxnee-*.tar.gz /tmp/xnee_rel/
    exec_and_check_status cp cnee/cnee-*.tar.gz /tmp/xnee_rel/
    exec_and_check_status cp gnee/gnee-*.tar.gz /tmp/xnee_rel/
}

log "Starting build at `date`" 
dist_all
deliver_dist
cd /tmp/xnee_rel
build_all
log "build fisnihed at `date`" 


