#!/usr/bin/env bash

MYNAME=./check_mem.sh


# source useful funs
if [ ! -f etc/base_funs ]; 
then 
    echo "Can't find the file: base_funs"  
    echo "Start me up in the xnee test dir "
    exit
else 
    . etc/base_funs
fi 
 
init_test $MYNAME
parse_me "$*"

verbose "Checking if the binary is gcoved and gprofed"

if [ "$(nm $XNEE | grep GCOV | wc -l)" != 0 ];
then
    GCOVED=true
    verbose "   $XNEE is gcoved" 
else
    verbose "   $XNEE is not gcoved" 
    GCOVED=false
fi

verbose " --- gcov ---"

gcov_check()
{
    NEW_DIR=$1
    PREFIX=$2


    pushd $NEW_DIR
    GCOV_FILE=./gcov_${PREFIX}.txt
    rm -f    ${GCOV_FILE} 2>/dev/null
    touch ${GCOV_FILE}
    for c_file in $( find . -name "*.c" ) ; 
      do
      O_FILE=$( basename $c_file | sed 's,\.c,\.o,g' )
      GCOV_O_FILE="${PREFIX}""${O_FILE}"
      verbose "gcoving $c_file  $GCOV_O_FILE"
      verbose "gcov -c -f -p -b $c_file --object-file  $GCOV_O_FILE"
      gcov -c -f -p -b "${c_file}" --object-file  "${GCOV_O_FILE}" >>  ${GCOV_FILE}
    done

    rm -f /tmp/${GCOV_FILE}  /tmp/*.gcov
    mv ${GCOV_FILE} /tmp/${GCOV_FILE}
    mv *.gcov /tmp/
    popd
    mv /tmp/${GCOV_FILE}  ./
    mv /tmp/*.gcov        ./

}

# gcov
verbose " *** gcov ***"
rm *.gcov
gcov_check $LIBXNEE "libxnee_a-"
gcov_check ../src "cnee-"



# gcov
rm gmon.sum
verbose " *** gprof ***"
gprof -s $XNEE gmon.out 





