#!/bin/sh



#
#    Build script for Xnee documents
#
#
#
#

#
#
#  SETTINGS
#
#
export XNEE_DOC_PACKAGE_NAME="xnee_doc"

export DEBUG=/tmp/xnee_build_docs.log
export XNEE_RELEASE=1
export BUILD_RPM=false
export PUT_WWW=false
export EXEC=true
# DIRS
export DOCS_DIR=./alldocs/
export SESSION_DIR=$DOCS_DIR/sessions/
export USER_MAN_DIR=$DOCS_DIR/man
export USER_MANUAL_DIR=$DOCS_DIR/manual/
export DEV_DESCR_DIR=$DOCS_DIR/developer/description/
export DEV_API_DIR=$DOCS_DIR/developer/api/
# DOCUMENTS (from texi files)
export XNEE_USER_MANUAL="xnee.html xnee.info xnee.txt xnee.pdf "
export XNEE_USER_MAN="xnee.1  xnee_man.txt"
export XNEE_DEV_MANUAL="xnee_develop.html xnee_develop.info xnee_develop.txt xnee_develop.pdf"

# DOCUMENTS (from sources)
export XNEE_DEVEL_API="latex/refman.pdf latex/refman.ps rtf/refman.rtf"


function log_to_file()
{
    touch $DOCS_DIR/README
    echo "$*" >> $DOCS_DIR/README
    
}

function get_version()
{
    export XNEE_MANUAL_VERSION=`grep VERSION doc/version.texi | awk '{ print $3 }'`
    export XNEE_DEVELOPER_VERSION=`grep VERSION doc/dev_version.texi | awk '{ print $3 }'`
    export XNEE_VERSION=`grep AM_INIT_AUTOMAKE configure.in | awk 'BEGIN {FS=","} { print $2 }' | sed 's/)//g'`
}


function doexec()
{
    if [ $EXEC == "true" ];
    then
	$*
    else
	echo "build_docs.sh [no-exec]: \"$*\""
    fi
}

function verbose()
{
    if [ "$DEBUG" == "true" ] ; 
    then
	echo "xnee doc [curr dir= `pwd`] "
	echo "xnee doc:`basename $0`: $*"
	echo "xnee doc:"
    elif [ "$DEBUG" == "false" ] ;
    then
	TMP=0
    else 
	touch $DEBUG
	echo "xnee doc:" >> $DEBUG
	echo "xnee doc:`basename $0`: $*" >> $DEBUG
	echo "xnee doc:" >> $DEBUG
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

function suffix_to_dir()
{
    FILE=$1
    export SUFFIX=`echo $FILE | sed 's/[\.\/a-z\_0-9]*\.//g'`
    if [ $SUFFIX == "1" ]; 
	then 
	SUFFIX=../man/man$SUFFIX
    fi
}

function soft_check_status()
{
    STATUS=$1
    TEXT=$2
    if [ $STATUS != 0 ]; then
	echo "ERROR: $2 failed ... however build does continue"
    fi
}



function setup_doc_dirs()
{
    \rm -fr $DOCS_DIR
    for main_dir in $USER_MANUAL_DIR $USER_MAN_DIR  $DEV_DESCR_DIR $DEV_API_DIR  ;
      do
      verbose " making dir:  $main_dir"
      mkdir -p $main_dir
      for sub_dir in html rtf pdf ps txt info ;
	do
	verbose " making dir:   $main_dir/$sub_dir"
	mkdir -p $main_dir/$sub_dir
      done
    done
    for sub_dir in 1  ;
      do
	verbose " making dir: $USER_MAN_DIR/man$sub_dir"
      mkdir -p $USER_MAN_DIR/man$sub_dir
    done

	verbose " making dir: $SESSION_DIR"
    mkdir -p $SESSION_DIR
}

function gen_readme()
{
    log_to_file "Xnee document package" 
    log_to_file " "
    log_to_file " "
    log_to_file "This file gives a short explanation of the content of Xnee document package"
    log_to_file " "
    log_to_file " "
    log_to_file "The package and this file was generated:"
    log_to_file "    date: `date`"
    log_to_file " "
    log_to_file "Version information:"
    log_to_file "    Xnee Manual           $XNEE_MANUAL_VERSION"
    log_to_file "    Xnee Developer Manual $XNEE_DEVELOPER_VERSION"
    log_to_file "    Xnee Developer API    $XNEE_VERSION"
    log_to_file " "
    log_to_file "Main directory entries "
    log_to_file "    man                   man page(s) for xnee"
    log_to_file "    manual                user manual for xnee"
    log_to_file "    developer/description developer manual for xnee"
    log_to_file "    developer/api         API for xnee"
    log_to_file " "

}


function hard_collect_user()
{
    verbose "hard_collect $*"
    MAIN_DIR=$1

    shift
    for i in $* 
      do
      suffix_to_dir $i
      DEST_DIR=$MAIN_DIR/$SUFFIX/
      verbose "  document: doc/$1 --> $DEST_DIR "
      if [ ! -f $1 ] ;
	  then
	  echo "File: doc/$1  DOES NOT EXIST .... leaving"
	  exit
      fi
      if [ ! -f $1 ] ;
	  then
	  echo "Dir: $DEST_DIR  DOES NOT EXIST .... leaving"
	  exit
      fi
      cp $1 $DEST_DIR
      doexec check_status "$?" "Failed to copy in hard_collect_user:  $1 ----> $DEST_DIR  current dir `pwd`"
      shift
    done
}

function hard_collect()
{
    verbose "hard_collect $*"
    MAIN_DIR=$1
    
    shift
    for i in $* 
      do
      suffix_to_dir $i
      DEST_DIR=$MAIN_DIR/$SUFFIX/
      verbose "  document: $1 --> $DEST_DIR  current dir `pwd`"
      cp $1 $DEST_DIR
      doexec check_status "$?" "Failed to copy in hard_collect_user:  $1 ----> $DEST_DIR  current dir `pwd`"
      shift
    done
}

#
#  Main 
#
while [ "$1" != "" ];
do
    verbose "arg: $1"
    
    if [ "$1" == "--verbose" ];
    then
	DEBUG=true
    elif [ "$1" == "--no-exec" ] ;
    then
	EXEC=false
    fi

    shift
done


get_version
# GOto doc dur
cd doc

# Remove debug log file
\rm -f $DEBUG
touch  $DEBUG


# Setup directory structure
setup_doc_dirs


verbose "Calling make to build various Xnee user documents"
for i in html txt info xnee.1 pdf ;
do 
  verbose "   - $i"
  doexec make $i
  doexec check_status "$?" "Failed to make $i version of user documents"
done

verbose "Calling make to build various Xnee developer documents"
doexec doxygen xnee.dox 
pushd latex
for i in   pdf ps ;
do 
  verbose "   - $i in directory ` pwd `"
  doexec make $i
  doexec check_status "$?" "Failed to make $i version of user documents"
done
popd

verbose "Collecting Xnee user documents"
hard_collect_user  $USER_MANUAL_DIR  $XNEE_USER_MANUAL
hard_collect_user  $USER_MAN_DIR     $XNEE_USER_MAN
hard_collect_user  $DEV_DESCR_DIR    $XNEE_DEV_MANUAL  

verbose "Collecting Xnee API documents in current dir: ` pwd `"
hard_collect   $DEV_API_DIR     $XNEE_DEVEL_API
cp -r html/ $DEV_API_DIR
doexec check_status "$?" "Failed to copy html dir to  $DEV_API_DIR"
cp -r man/ $DEV_API_DIR
doexec check_status "$?" "Failed to copy man dir to  $DEV_API_DIR"

verbose "Collecting session examples"
doexec cp -r ../sessions/*.xnr  $SESSION_DIR
doexec check_status "$?" "Failed to copy  $DOCS_DIR/../sessions/  dir to  $SESSION_DIR"

verbose "Generating README file"
gen_readme

verbose "Making tar file"
cd $DOCS_DIR
tar cvf ../${XNEE_DOC_PACKAGE_NAME}_${XNEE_MANUAL_VERSION}.tar *
