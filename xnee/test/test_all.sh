#!/bin/bash

. etc/base_funs
\rm -f $LOG $ERR_LOG

init_test

START=`date '+%s'`
for i in `ls scripts/options/*.sh | grep -v test_all\.sh `; 
do 
    $i $*
done


scripts/record/keyboard.sh    $*
scripts/record/mouse.sh       $*
scripts/replay/keyboard.sh    $*
scripts/replay/mouse.sh       $*
scripts/resolution/mo-res.sh  $*
scripts/timing/mo-time.sh     $*

STOP=`date '+%s'`

NAME=test_all.sh
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
    END { \
    printf "Total nr of tests:   %s\n", TOT_T ;\
    printf "  Success:           %s\n", SUC_T ;\
    printf "  Error:             %s\n", ERR_T ;\
    printf "  Warning:           %s\n", WARN_T ;\
    printf "  Return (correct):  %s\n", RET_T ;\
    printf "  Return (incorrect):%s\n", RET_E ;\
    printf "Untested scripts:    %s\n", UNT ;\
    }' | tee test_all.log

    
echo "TIME 0:`expr $STOP - $START`:$XNEE_VERSION:$X_VEND:$X_VERS:$ME:$OS" >> $NEW_LOG




