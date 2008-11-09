#!/bin/bash

. etc/base_funs
init_test

sighandler()
{
   echo "Signal received in test_all.sh"
   echo "  while executig $CMD"
}

trap sighandler 2
trap sighandler 15


retype_keyboard()
{

    CMD="scripts/retype/keyboard.sh    $*"
    echo "executing $CMD"
    scripts/retype/keyboard.sh    $*

}

option_scripts()
{
    for i in `ls scripts/options/*.sh | grep -v test_all\.sh `; 
      do 
      echo "exec $i"
      CMD="$i $*"
      $i $*
      sleep 1
    done
}

function_scripts()
{
    CMD="scripts/record/keyboard.sh    $* "
    echo "executing $CMD"
    scripts/record/keyboard.sh    $* 

    CMD="scripts/record/mouse.sh       $*"
    echo "executing $CMD"
    scripts/record/mouse.sh       $*

    CMD="scripts/replay/keyboard.sh    $*"
    echo "executing $CMD"
    scripts/replay/keyboard.sh    $*

    CMD="scripts/replay/mouse.sh       $*"
    echo "executing $CMD"
    scripts/replay/mouse.sh       $*

    CMD="scripts/resolution/mo-res.sh  $*"
    echo "executing $CMD"
    scripts/resolution/mo-res.sh  $*

    CMD="scripts/timing/mo-time.sh     $*"
    echo "executing $CMD"
    scripts/timing/mo-time.sh     $*

    CMD="scripts/timing/mo-time.sh     $*"
    echo "executing $CMD"
    scripts/timing/mo-time.sh     $*

#    CMD="scripts/retype/keyboard.sh    $*"
#    echo "executing $CMD"
#    scripts/retype/keyboard.sh    $*
}


scan_logs()
{
    NEW_LOG=test_all.log
    rm    $NEW_LOG
    touch $NEW_LOG
    echo "Script name: test_all.sh" >> $NEW_LOG
    echo "Results from test"  >> $NEW_LOG
    cat $LOG | awk 'BEGIN { FS="[:]" ; TOT_T=0 ; SUC_T=0 ; ERR_T=0 ; WARN_T=0 ; RET_T=0 ; RET_E=0 ; UNT=0 ; } \
    { \
    TOT_T=TOT_T   + $2 ;\
    SUC_T=SUC_T  + $3 ;\
    ERR_T=ERR_T   + $4 ;\
    WARN_T=WARN_T + $5 ;\
    RET_T=RET_T   + $6 ;\
    RET_E=RET_E   + $7 ;\
    UNT=UNT   + $8 ;\
    } \
    END { 
    printf "\n" ;\
    printf "Functionality test:\n" ;\
    printf "===================\n" ;\
    printf "  Tested scripts:      %s\n", TOT_T ;\
    printf "  Untested scripts:    %s\n", UNT ;\
    printf "  ------------------------\n" ;\
    printf "    Success:           %s\n", SUC_T ;\
    printf "    Error:             %s\n", ERR_T ;\
    printf "    Warning:           %s\n", WARN_T ;\
    printf "\n" ;\
    printf "Return values test:\n" ;\
    printf "===================\n" ;\
    printf "    Correct:           %s\n", RET_T ;\
    printf "    Incorrect:         %s\n", RET_E ;\
    printf "\n" ;\
    if (ERR_T > 0) {  printf "\n\n*** %d error(s) found ***\n"   , ERR_T} ;\
    if (WARN_T > 0) { printf "\n\n*** %d warning(s) found ***\n" , WARN_T } ;\
    }' | tee $NEW_LOG

    echo "TIME 0:"$(( $STOP - $START ))":$XNEE_VERSION:$X_VEND:$X_VERS:$ME:$OS" >> $NEW_LOG

    echo "... perhaps we should add analysis of gcov, valgrind and gprof files....."

}

NAME=test_all.sh

remove_gcov_gprof_file

if [ "$1" == "--no-run" ];
    then
    scan_logs
elif [ "$1" == "--help" ];
    then
    echo "OPTIONS:"
    echo "   --clean     removes all generated files"
    echo "   --function  test cnee functionality only"
    echo "   --option    test cnee options only"
    echo "   --no-run    Dont exec cnee. Scan logs"
elif [ "$1" == "--build" ];
    then
    cd src
    make all 
    cd ..

elif [ "$1" == "--clean" ];
    then
    remove_gcov_gprof_file
    rm -f *.log
    rm -f *.tmp
    rm -fr gmon*.*
    rm -fr gcov*.*
    rm -fr xnee_val.*
    rm -fr rep-*.log
    rm -fr src/xgetter.o src/xgetter
    
elif [ "$1" == "--function" ];
    then
    START=`date '+%s'`
    \rm -f $LOG $ERR_LOG
    shift
    function_scripts "$*"
    STOP=`date '+%s'`
    scan_logs
elif [ "$1" == "--option" ];
    then
    START=`date '+%s'`
    \rm -f $LOG $ERR_LOG
    option_scripts "$*"
    STOP=`date '+%s'`
    scan_logs
elif [ "$1" == "--kbd" ];
    then
    START=`date '+%s'`
    \rm -f $LOG $ERR_LOG
    retype_keyboard "$*"
    STOP=`date '+%s'`
    scan_logs
else
    START=`date '+%s'`
    \rm -f $LOG $ERR_LOG
    option_scripts "$*"
    function_scripts "$*"
    STOP=`date '+%s'`
    scan_logs
fi





