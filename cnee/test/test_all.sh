#!/bin/bash

. etc/base_funs
init_test

sighandler()
{
   echo "Signal received in test_all.sh"
}

trap sighandler 2
trap sighandler 15




option_scripts()
{
    for i in `ls scripts/options/*.sh | grep -v test_all\.sh `; 
      do 
      echo "exec $i"
      $i $*
      sleep 1
    done
}

function_scripts()
{
    scripts/record/keyboard.sh    $* 
    scripts/record/mouse.sh       $*
    scripts/replay/keyboard.sh    $*
    scripts/replay/mouse.sh       $*
    scripts/resolution/mo-res.sh  $*
    scripts/timing/mo-time.sh     $*
    scripts/timing/mo-time.sh     $*
    scripts/retype/keyboard.sh    $*
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
}

NAME=test_all.sh

remove_gcov_gprof_file

if [ "$1" == "--no-run" ];
then
    scan_logs
elif [ "$1" == "--clean" ];
then
    remove_gcov_gprof_file
    rm -f *.log
    rm -f *.tmp
    rm -fr gmon*.*
    rm -fr gcov*.*
    rm -fr xnee_val.*
    rm -fr rep-*.log

elif [ "$1" == "--function" ];
then
    START=`date '+%s'`
    \rm -f $LOG $ERR_LOG
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
else
    START=`date '+%s'`
    \rm -f $LOG $ERR_LOG
    option_scripts "$*"
    function_scripts "$*"
    STOP=`date '+%s'`
    scan_logs
fi





