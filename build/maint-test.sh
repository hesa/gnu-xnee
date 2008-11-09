#!/bin/bash


echo $0
BASE_DIR=$(dirname $0)/../
cd $BASE_DIR

MAKE='make -f GNUmakefile '

test_setup_makefiles()
{
    ./configure                     \
        --enable-maintainer-mode    \
        --disable-doc --disable-gui \
        --disable-gnome-applet     
    
}


test_setup_coverage()
{
    $MAKE init-coverage
    $MAKE build-for-coverage
}


test_run_tests()
{
    pushd cnee/test
    ./test_all.sh --clean
    ./test_all.sh --option
#    ./test_all.sh --function
    popd
}




test_create_report()
{
    $MAKE report-coverage
    $MAKE gen-coverage
}

while [ "$1" != "" ]
do
    case "$1" in

	"-c" | "--configure" )
	  test_setup_makefiles
	  ;;

	"-p" | "--prepare-coverage" )
	  test_setup_coverage
	  ;;

	"-pa" | "--prepare-all" )
	  test_setup_makefiles
	  test_setup_coverage
	  ;;
	
	"-rt" | "--run-tests" )
	  test_run_tests
	  ;;

	"-cr" | "--create-report" )
	  test_create_report
	  ;;

	"-a" | "--all" )
	  test_setup_makefiles
	  test_setup_coverage
	  test_setup_makefiles
	  test_setup_coverage
	  test_run_tests
	  test_create_report
	  ;;

	*)
	   echo "Could not parse"
	   ;;
    esac
    shift
done