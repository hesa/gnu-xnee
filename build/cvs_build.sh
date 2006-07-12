#!/bin/sh

#
# this file:
#   * downloads (anonymous) from CVS
#   * builds tar ball
#   * build dist
#
# well, that's about it
#
# No nedd to license it at the moment 
#


export XNEE_TMP_DIR=/tmp/xnee_cvs
export CVS_RSH=ssh
DEST_DIR=/var/www/xnee_cvs/
TARBALL_DIR=${DEST_DIR}/nightly-tar-ball
NIGHT_DIR=${DEST_DIR}/nightly-dists
LOG_DIR=${DEST_DIR}/logs
#DATE=$(date '+%Y%m%d')
DATE=20060801
LOG_FILE=/tmp/xnee_cvs/cvs_build-${DATE}.log



log()
{
    echo "cvs_build.sh [$DATE]:   $*" >> $LOG_FILE
    echo "cvs_build.sh [$DATE]:   $*"
}

do_exec()
{
    log "$*"
    $*
    
    RET=$?
    if [ "$RET" != "0" ] 
	then
	echo "failed to execute :$*"
	exit $RET
    fi
}

create_tmp_dir()
{
    if [ -d ${XNEE_TMP_DIR} ]
	then
	rm -fr ${XNEE_TMP_DIR}
    fi
    mkdir ${XNEE_TMP_DIR}
}


get_cvs()
{
    cvs -z3 -d:pserver:anonymous@cvs.savannah.gnu.org:/sources/xnee co xnee
}


update_version()
{
    
 cat configure.in  | sed "s/\(A[CM]_INIT[_a-zA-Z]*([a-zA-Z]*\),[0-9\.]*/\1,cvs-${DATE}/g"  >  configure.tmp
 mv -f configure.tmp  configure.in
}

copy_and_latest()
{
    do_exec "cp $1 $2/"
    do_exec "pushd  $2"
    do_exec "rm latest"
    do_exec "ln -s $1 latest"
    do_exec "popd"
}

build_from_cvs()
{
    do_exec "make -f Makefile.cvs"
    do_exec "./configure"
    do_exec "make"
    do_exec "make dist"
    GZ_FILE=$(ls *.gz)

    copy_and_latest ${GZ_FILE}   ${NIGHT_DIR}
}


build_cvs_tar()
{
    pushd ..
    do_exec "tar cvf xnee-cvs-tarball-${DATE}.tar xnee"
    do_exec "gzip    xnee-cvs-tarball-${DATE}.tar "

    copy_and_latest xnee-cvs-tarball-${DATE}.tar.gz  ${TARBALL_DIR}

    popd
}

check_dir_sub()
{
    if [ ! -d $1 ]
	then
	mkdir -p $1
    fi
}

check_dirs()
{
    check_dir_sub $TARBALL_DIR
    check_dir_sub $NIGHT_DIR
    check_dir_sub ${LOG_DIR}
}

#
#   main
#
check_dirs
create_tmp_dir
cd ${XNEE_TMP_DIR}
get_cvs
cd xnee

build_cvs_tar

update_version
build_from_cvs

copy_and_latest $LOG_FILE ${LOG_DIR}


