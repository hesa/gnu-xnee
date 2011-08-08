#!/bin/bash

DATE=$(date '+%y%m%d-%H%M%S')

exit_on_error()
{
    RET=$1
    shift
    if [ "$RET" != "0" ]
	then
	echo "Error ($RET): $*"
	exit $RET
    fi
}

clean_up()
{
    rm -fr $XNEE_TMP_BUILD_DIR/
    rm -fr $XNEE_TMP_INSTALL_DIR/
}


checkout_sources()
{
    cvs -dhesa@cvs.savannah.gnu.org/sources/xnee checkout -d $CVS_TAG xnee
    exit_on_error $? "cvs -dhesa@cvs.savannah.gnu.org/sources/xnee checkout -d $CVS_TAG xnee"
}

build_sources()
{
    make -f Makefile.cvs 
    exit_on_error $? "make -f Makefile.cvs "

    ./configure --enable-doc --prefix  $XNEE_TMP_INSTALL_DIR/ 
    exit_on_error $? "./configure --enable-doc --prefix  $XNEE_TMP_INSTALL_DIR/"

    make 
    exit_on_error $? "make "

    make check
    exit_on_error $? "make check"

    make install
    exit_on_error $? "make install"

    make dist
    exit_on_error $? "make dist"

}

verify_sources()
{
    
    printf "%60s" "Check version number ($VERSION) in configure.in:  "
    WC=$(grep $VERSION configure.in | wc -l )
    if [ "$WC" != "1" ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi
}

verify_build()
{
    printf "%60s" "Check version ($VERSION) number in cnee:  "
    WC=$(./cnee/src/cnee --version 2>&1 | grep $VERSION | wc -l )
    if [ "$WC" != "1" ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi

    printf "%60s" "Check version ($VERSION) number in gnee:  "
    WC=$(./gnee/src/gnee --version 2>&1 | grep $VERSION | wc -l )
    if [ "$WC" != "1" ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi
}

verify_install()
{
    printf "%60s" "Check if cnee is installed: "
    if [ ! -f  $XNEE_TMP_INSTALL_DIR/bin/cnee ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi

    printf "%60s" "Check if gnee is installed: "
    if [ ! -f  $XNEE_TMP_INSTALL_DIR/bin/gnee ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi

    printf "%60s" "Check if html man is installed: "
    if [ ! -f  $XNEE_TMP_INSTALL_DIR/share/xnee/xnee.html ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi
}

verify_dist()
{
    printf "%60s" "Check dist file name ($DIST_FILE): "
    if [ ! -f  $DIST_FILE ]
	then
	echo "fail"
	exit 1
    else
	echo "OK"
    fi
}

prepare()
{
    clean_up
    mkdir $XNEE_TMP_BUILD_DIR/
}


usage()
{
    echo "$0 XNEE_3_10 3.10 [old date]"
}


if [ "$1" = "" ]
then
    echo "No tag specified"
    usage
    exit 1
else
    CVS_TAG=$1
fi



if [ "$2" = "" ]
then
    echo "No version specified"
    usage
    exit 1
else
    VERSION=$2
fi

if [ "$3" != "" ]
then
    echo "Using DATE $3"
    DATE=$3
fi

XNEE_TMP_BUILD_DIR=/tmp/xnee-tmp-$DATE
XNEE_TMP_INSTALL_DIR=/tmp/xnee-tmp-install
DIST_FILE=xnee-$VERSION.tar.gz


prepare

pushd    $XNEE_TMP_BUILD_DIR/
checkout_sources
cd    $CVS_TAG
exit_on_error $? "cd $CVS_TAG"

verify_sources
build_sources
verify_build
verify_install
verify_dist

popd

echo "Seems ok, "
echo "To test the dist:"
echo build/test-dist.sh $XNEE_TMP_BUILD_DIR/$CVS_TAG/$DIST_FILE
