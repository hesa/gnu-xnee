#! /bin/bash


#
#  This script eases up development .... only just a little bit
#
#  ... it is not needed for anything else then developing Xnee
#
#
#
\rm -f /tmp/xnee_build.log
export DEBUG=/tmp/xnee_build.log
export XNEE_RELEASE=1
export BUILD_RPM=false
export PUT_WWW=false

function verbose()
{

    if [ "$DEBUG" == "true" ] ; 
    then
	echo "xnee:"
	echo "xnee:`basename $0`: $*"
	echo "xnee:"
    elif [ "$DEBUG" == "false" ] ;
    then
	TMP=0
    else 
	touch $DEBUG
	echo "xnee:" >> $DEBUG
	echo "xnee:`basename $0`: $*" >> $DEBUG
	echo "xnee:" >> $DEBUG
    fi
}


function check_status()
{
    STATUS=$1
    TEXT=$2
    if [ $STATUS != 0 ]; then
	echo "ERROR: $2 failed"
	exit
    fi
}

function check_version()
{
    EXPECTED=`echo $1 | sed 's/[ \t]*//g'`
    ERR_MESS=$2
    verbose "Checking version on $2"
    RECEIVED=`./xnee/src/xnee --version 2>&1 | grep  '^xnee [0-9\. a-zA-Z]*$'| awk '{print $2}' `
    
    if [ "$EXPECTED" != "$RECEIVED" ] ;
    then
	echo "Different versions"
	echo "  expected:  \"$EXPECTED\""
	echo "  got:       \"$RECEIVED\""
        exit 
    else
	echo "Versions OK"
	echo "  expected:  \"$EXPECTED\""
	echo "  got:       \"$RECEIVED\""
    fi

}



while [ "$1" != "" ];
do
    verbose "arg: $1"
    
    if [ "$1" == "--verbose" ];
    then
	DEBUG=true
    elif [ "$1" == "--fileverbose" ] ;
    then
	shift
	DEBUG=$1
    elif [ "$1" == "--rpm" ] ;
    then
	BUILD_RPM=true
    elif [ "$1" == "--www" ] ;
    then
	PUT_WWW=true
    fi

    shift
done



verbose "  getting version from conf"
export XNEE_VERSION=`grep AM_INIT_AUTOMAKE configure.in | awk 'BEGIN {FS=","} { print $2 }' | sed 's/)//g'`

verbose "  getting version from conf"
export XNEE_PACKAGE=`grep AM_INIT_AUTOMAKE configure.in | awk 'BEGIN {FS="("} { print $2 }' | sed 's/)//g' | sed 's/,[\.0-9a-zA-Z]*//g'`

verbose "  XNEE_VERSION=$XNEE_VERSION"
verbose "  XNEE_PACKAGE=$XNEE_PACKAGE"

#
# UPDATING other Makefiles
#
verbose "Updating makefiles"
perl -i -p -s -e 's/PACKAGE\=\\\"[a-zA-Z_0-9]*\\\"/PACKAGE\=\\\"$ENV{"XNEE_PACKAGE"}\\\"/g' Makefile.xnee

perl -i -p -s -e 's/VERSION\=\\\"[a-zA-Z_0-9\.]*\\\"/VERSION\=\\\"$ENV{"XNEE_VERSION"}\\\"/g' Makefile.xnee

#
#  ATUTO
#
verbose "Doing some autstuff"
make -f Makefile.cvs 
check_status "$?" "autoconf"


#
#  CONFIGURE
#
verbose "Configuring"
verbose "   configure"
./configure 
check_status "$?" "configure"


#
#  MAKE
#
verbose "Making ...."
verbose "    make clean all text html man info"
make clean all text html man info txt 
check_status "$?" "make"
check_version $XNEE_VERSION "xnee from configure && make"
cp xnee/src/xnee ./xnee.makefile_configure

#
#  SOURCE DIST
#
verbose "Generating dist"
verbose "    make dist"
make dist
check_status "$?" "make dist"


#
#  GNU/Linux binary
#
cp xnee/src/xnee ./xnee.configure





#
#  RedHat RPM
#
if [ "$BUILD_RPM" == "true" ] ;
then
    verbose "RedHat RPM"
    verbose "   checking that we are allowed to do root stuff"
    sudo ls -l /proc 
    if [ "$STATUS" == "1" ];
    then 
	echo "WARNING: user not allowd to do sudo"
	echo "         skipping RPM stuff"
    else
	echo "ENTER PASSWORD"
	read PASSWD
	verbose "  removing old SPEC file"
	RPM_BUILD_DIR=/usr/src/redhat
	XNEE_SPEC=${RPM_BUILD_DIR}/SPECS/xnee.spec
	echo $PASSWD | sudo -S rm -f ${XNEE_SPEC}
	
	cat redhat/SPECS/xnee.spec.tmpl | perl -i -p -s -e 's/XNEE_VERSION/$ENV{"XNEE_VERSION"}/g' | perl -i -p -s -e 's/XNEE_RELEASE/$ENV{"XNEE_RELEASE"}/g' > /tmp/xnee.spec
	echo $PASSWD | sudo -S mv /tmp/xnee.spec ${XNEE_SPEC}
	check_status "$?" "  making spec file"
	verbose "   found $XNEE_VERSION"
	export XNEE_TAR_GZ=xnee-${XNEE_VERSION}.tar.gz
	verbose "   about to copy ${XNEE_TAR_GZ} ---> ${RPM_BUILD_DIR}/SOURCES"
	echo "   PASSWD=$PASSWD"
	verbose "    ls  ${RPM_BUILD_DIR}/SOURCES   -> `ls -l ${RPM_BUILD_DIR}/SOURCES`"
	echo $PASSWD | sudo -S rm -f ${RPM_BUILD_DIR}/SOURCES/${XNEE_TAR_GZ} 
	verbose "      removed old tar.gz file"
	verbose "$?" "  copying gzipped tar file "
	echo $PASSWD | sudo -S cp ${XNEE_TAR_GZ} ${RPM_BUILD_DIR}/SOURCES
	check_status "$?" "  copied gzipped tar file "
	verbose "$?" "  copied gzipped tar file "
	
	verbose "  pushing to  ${RPM_BUILD_DIR}/SPECS"
	pushd ${RPM_BUILD_DIR}/SPECS
	verbose "   building .... rpm -ba ${XNEE_SPEC}"    
	verbose "sudo -S  rpm -ba `basename ${XNEE_SPEC}`"
	echo $PASSWD | sudo -S  rpm -ba `basename ${XNEE_SPEC}`
	check_status "$?" "  building RPM "
	
	XNEE_RPM=`find ${RPM_BUILD_DIR}/RPMS -type f -name "xnee*.rpm"`
	check_status "$?" "  finding redhat binary rpm"
	XNEE_SRPM=`find ${RPM_BUILD_DIR}/SRPMS -type f -name "xnee*.rpm"`
	check_status "$?" "  finding redhat source rpm"
	    
	    
	popd
	
	echo $PASSWD | sudo -S cp $XNEE_RPM $XNEE_SRPM ./
	echo $PASSWD | sudo -S chown ${USER} ./*.rpm
	check_status "$?" "  copying source and binary redhat rpms"
    fi
fi




#
# Test OpenBSD, Solaris and template Makefile
#
#make -f Makefile.openbsd clean all
#check_version $XNEE_VERSION "xnee from Makefile.openbsd"
#mv xnee/src/xnee ./xnee.makefile_openbsd

#make -f Makefile.solaris clean all
#check_version $XNEE_VERSION "xnee from Makefile.solaris"
#mv xnee/src/xnee ./xnee.makefile_solaris

make -f Makefile.xnee clean all
check_version $XNEE_VERSION "xnee from Makefile.xnee"
mv xnee/src/xnee ./xnee.makefile_xnee


verbose "Putting back configure binary"
cp ./xnee.makefile_configure xnee/src/xnee


