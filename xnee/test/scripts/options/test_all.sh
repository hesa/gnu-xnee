#!/bin/bash

. etc/base_funs
\rm -f $LOG $ERR_LOG

init_test

START=`date '+%s'`
for i in `ls scripts/options/*.sh | grep -v test_all\.sh `; 
do 
  $i $*
done
STOP=`date '+%s'`

NAME=test_all.sh

echo " " 
/bin/echo -n "TOTAL:"
cat $LOG | awk 'BEGIN { FS="[:]" ; TOT_T=0 ; SUC_T=0 ; ERR_T=0 ; WARN_T=0 ; RET_T=0 ; RET_E=0 ; } \
    { \
    TOT_T=TOT_T   + $2 ;\
    SUC_T=SUC_T  + $3 ;\
    ERR_T=ERR_T   + $4 ;\
    WARN_T=WARN_T + $5 ;\
    RET_T=RET_T   + $6 ;\
    RET_E=RET_E   + $7 ;\
    } \
    END { \
    printf "%s:", TOT_T ;\
    printf "%s:", SUC_T ;\
    printf "%s:", ERR_T ;\
    printf "%s:", WARN_T ;\
    printf "%s:", RET_T ;\
    printf "%s:", RET_E ;\
    }'
    
echo "TIME 0:`expr $STOP - $START`:$XNEE_VERSION:$X_VEND:$X_VERS:$ME:$OS" >> $LOG



