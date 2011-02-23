#!/bin/sh

EXEC_FILE=/home/hesa/gnu/xnee/build/build-and-log.sh
LOG_FILE=/tmp/rem-build.log

rm $LOG_FILE

log()
{
    echo "[$(date) $(uname -n)] $*" >> $LOG_FILE
}

remote_build()
{
    NODE=$1
    HOST=$2
    HOST_PORT=$3

    scp  -P $HOST_PORT  $EXEC_FILE $HOST: 
    log "RETURNED:   $?   scp $EXEC_FILE  $HOST:"

    ssh $HOST -p $HOST_PORT uname -a
    log "RETURNED:   $?   ssh $HOST uname -a"

    ssh hesa@$HOST -p $HOST_PORT  ./build-and-log.sh --node $NODE --output-dir ~/buildlogs/xnee --src-dir ~/gnu/xnee
    log "RETURNED:   $?   ssh hesa@$HOST ./build-and-log.sh --node $NODE --output-dir ~/buildlogs/xnee --src-dir ~/gnu/xnee"

    rsync -e "ssh -p $HOST_PORT" -avz hesa@$HOST:buildlogs/xnee ~/buildlogs/
    log "RETURNED:   $?   rsync -avz hesa@$HOST:buildlogs/xnee ~/buildlogs/"
}

remote_start()
{
    echo "will start remote computer"
}

remote_start 
remote_build bruckner       192.168.1.9 22
remote_build lenny-bruckner 192.168.1.9 2225
