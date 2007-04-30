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
export DO_CVS=false

export SUDO="sudo"

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

    
    RECEIVED=`./cnee/src/cnee --version 2>&1 | grep  'Xnee [0-9\. a-zA-Z]*$'| awk '{print $2}' `
    
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
    elif [ "$1" == "--cvs" ] ;
    then
	DO_CVS=true
    elif [ "$1" == "--help" ] ;
    then
	echo "options available: --verbose, --fileverbose, --rpm, --www, --help"
	exit 0
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



function rpm_build()
{
    PACKAGE_NAME=$1
    SUB_PACKAGE_NAME=$2
    SPEC_FILE_TMPL=$3
    
#1  Xnee
#2  Xnee, cnee, gnee
#3  build/SPECS/xnee.spec.tmpl




    verbose "  Removing old SPEC file"
    
    RPM_BUILD_DIR=/usr/src/rpm
    rm -fr ${RPM_BUILD_DIR}/
    mkdir -p ${RPM_BUILD_DIR}/
    mkdir ${RPM_BUILD_DIR}/SOURCES
    mkdir ${RPM_BUILD_DIR}/SRPMS
    mkdir ${RPM_BUILD_DIR}/RPMS
    mkdir ${RPM_BUILD_DIR}/SPECS
    mkdir ${RPM_BUILD_DIR}/BUILD
    mkdir ${RPM_BUILD_DIR}/tmp

    XNEE_SPEC=${RPM_BUILD_DIR}/SPECS/${SUB_PACKAGE_NAME}.spec
    ${SUDO} rm -f ${XNEE_SPEC}
    
    ls -l $SPEC_FILE_TMPL
    cat $SPEC_FILE_TMPL | perl -i -p -s -e 's/XNEE_VERSION/$ENV{"XNEE_VERSION"}/g' | perl -i -p -s -e 's/XNEE_RELEASE/$ENV{"XNEE_RELEASE"}/g' > /tmp/xnee.spec
    ls -l /tmp/xnee.spec
    ${SUDO} mv /tmp/xnee.spec ${XNEE_SPEC}
    check_status "$?" "  making spec file"
    verbose "   found $XNEE_VERSION"

    export XNEE_TAR_GZ=Xnee-${XNEE_VERSION}.tar.gz
    export XNEE_TAR_GZ=${PACKAGE_NAME}-${XNEE_VERSION}.tar.gz
    verbose "   about to copy ${XNEE_TAR_GZ} ---> ${RPM_BUILD_DIR}/SOURCES"
    echo "   PASSWD=$PASSWD"
    verbose "    ls  ${RPM_BUILD_DIR}/SOURCES   -> `ls -l ${RPM_BUILD_DIR}/SOURCES`"
    ${SUDO} rm -f ${RPM_BUILD_DIR}/SOURCES/${XNEE_TAR_GZ} 
    verbose "      removed old tar.gz file"
    verbose "$?" "  copying gzipped tar file "
    ${SUDO} cp ${XNEE_TAR_GZ} ${RPM_BUILD_DIR}/SOURCES/${XNEE_TAR_GZ}
    check_status "$?" "  copied gzipped tar file "
    verbose "$?" "  copied gzipped tar file "
    
    verbose "  pushing to  ${RPM_BUILD_DIR}/SPECS"
    pushd ${RPM_BUILD_DIR}/SPECS
    verbose "   building .... rpm -ba ${XNEE_SPEC}"    
    verbose "${SUDO}  rpm -ba `basename ${XNEE_SPEC}`"
    ${SUDO}  rpm -ba   `basename ${XNEE_SPEC}`
    check_status "$?" "  building RPM "
    
    XNEE_RPM=`find ${RPM_BUILD_DIR}/RPMS -type f -name "*nee*.rpm"`
    check_status "$?" "  finding  binary rpm"
    XNEE_SRPM=`find ${RPM_BUILD_DIR}/SRPMS -type f -name "*nee*.rpm"`
    check_status "$?" "  finding  source rpm"
    
    
    popd
    
    ${SUDO} cp $XNEE_RPM $XNEE_SRPM ./
    ${SUDO} chown ${USER} ./*.rpm
    check_status "$?" "  copying source and binary rpms"
}



build_rpm_prepare()
{
    CONFIGURE_FLAGS=$1
    MAKE_FLAGS=$2
    COMMENT=$3

    verbose "Doing some autstuff in order to $3"
    verbose "   make -f Makefile.cvs "
    make -f Makefile.cvs 
    check_status "$?" "autoconf"
    verbose "Configuring"
    verbose "   ./configure $CONFIGURE_FLAGS"
    verbose "    "
    ./configure $CONFIGURE_FLAGS
    check_status "$?" "configure"
    verbose "Making ...."
    rm *.gz
    verbose "   make $MAKE_FLAGS"
    make $MAKE_FLAGS
    check_status "$?" "make"
}

build_cvs()
{
    rm -fr /tmp/xnee_tar
    mkdir  /tmp/xnee_tar
    cd     /tmp/xnee_tar
    export CVS_RSH=ssh
    cvs -dhesa@cvs.savannah.gnu.org:/cvsroot/xnee co xnee
    DATE=$(date '+%Y%m%d')
    tar cvf Xnee-cvs-${DATE}.tar *
    gzip Xnee-cvs-${DATE}.tar 
    cd -
    mv /tmp/xnee_tar/Xnee-cvs-${DATE}.tar.gz ./
}
build_cvs

if [ "$BUILD_RPM" != "true" ] ;
    then
    
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
    ./configure --enable-gui 
    check_status "$?" "configure"
    

#
#  MAKE
#
    verbose "Making ...."
    verbose "    make clean all text html man info"
    make clean all manual
    check_status "$?" "make"

    echo "======================================="
    pwd

    check_version $XNEE_VERSION "xnee from configure && make"
    cp cnee/src/cnee ./cnee.makefile_configure
    
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
    cp cnee/src/cnee ./cnee.configure
    
    
    make -f Makefile.xnee clean all
    check_version $XNEE_VERSION "xnee from Makefile.xnee"
    mv cnee/src/cnee ./cnee.makefile_xnee
    
    
    verbose "Putting back configure binary"
    cp ./cnee.makefile_configure cnee/src/cnee
    
#
#   RPM
#
else
    verbose " RPM"
    verbose "   checking that we are allowed to do root stuff"
#    echo "Plese, enter password for coming sudo session"
#    read PASSWD
    
    if [ "$STATUS" == "1" ];
	then 
	echo "WARNING: user not allowd to do sudo"
	echo "         skipping RPM stuff"
	exit 1
    fi
	# Build:  Xnee rpm
	#
    build_rpm_prepare "--enable-gui --enable-doc" "clean all manual man dist" "Build Xnee rpm"
    rpm_build Xnee Xnee build/SPECS/xnee.spec.tmpl
    
	# Build:  cnee rpm
	#
    build_rpm_prepare "--disable-gui --disable-doc" "clean all man dist" "Build cnee rpm"
    rpm_build Xnee cnee build/SPECS/cnee.spec.tmpl

	# Build:  doc rpm
	#
    build_rpm_prepare "--enable-doconly" "clean all dist" "Build Xnee docs"
    rpm_build Xnee xnee-doc build/SPECS/doc.spec.tmpl

	# Build:  xnee-devel rpm
	#
    build_rpm_prepare "--disable-cli --disable-gui" "clean all dist" "Build Xnee devel"
    rpm_build Xnee devel build/SPECS/xnee-devel.spec.tmpl
    
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

if [ "$DO_CVS" = "true" ]
then
    build_cvs
fi

