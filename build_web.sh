#! /bin/bash


#
#  This script eases up development .... only just a little bit
#
#  ... it is not needed for anything else then developing Xnee
#
#
#
\rm -f /tmp/xnee_build.log
export DEBUG=/tmp/xnee_web.log
export XNEE_RELEASE=1
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
    RECEIVED=`src/cli/xnee --version 2>&1 | grep Version | awk '{print $3}' `
    
    if [ "$EXPECTED" != "$RECEIVED" ] ;
    then
	echo "Different versions"
	echo "  expected:  \"$EXPECTED\""
	echo "  got:       \"$RECEIVED\""
        exit 
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
    elif [ "$1" == "--www" ] ;
    then
	PUT_WWW=true
    fi

    shift
done



function test_dir()
{
    DIR=$1
    ERR_MESS=$2

    verbose "testing dir $DIR"
    if [ ! -d $DIR ];
    then
	echo "$ERR_MESS"
	exit
    fi
}

function test_file()
{
    verbose "---> test_file"
    FILE=$1
    ERR_MESS=$2
    verbose "testing file $FILE"
    if [ ! -f $FILE ];
    then
	echo "$ERR_MESS"
	exit
    fi

    verbose "<--- test_file"
}

function test_file_cp()
{
    verbose "---> test_file_cp"
    FILE=$1
    ERR_MESS=$2
    test_file $1 $2

    verbose "copying  file $FILE to web/doc/"
    cp $FILE web/doc/
    verbose "<--- test_file_cp"
}


\rm -fr web/doc/*.* web/doc/html


#
#
# Test if API is generated
test_dir doc/rtf "RTF directory missing. .... leaving"
test_dir doc/html "HTML directory missing. .... leaving"
test_dir doc/latex "LATEX directory missing. .... leaving"
test_file_cp doc/rtf/refman.rtf "RTF Reference manual missing... leaving"
test_file_cp doc/latex/refman.ps "PS Reference manual missing... leaving"
test_file_cp doc/latex/refman.pdf "PDF Reference manual missing... leaving"
test_file     doc/html/index.html "HTML Reference manual pages missing... leaving"

cp -r doc/html/  web/doc/

cd doc/
texi2html xnee_faq.texi
cp xnee_faq.html ../web/doc/
cd ..

cd web
verbose "Building todo page"
rm -f todo.html
cp todo.top todo.html
cat ../TODO >> todo.html
cat todo.bot >> todo.html
cd ..


cd web
verbose "Copying files in `pwd` to Sourceforge.net"
scp -r ./ sandklef@xnee.sourceforge.net:/home/groups/x/xn/xnee/htdocs/


