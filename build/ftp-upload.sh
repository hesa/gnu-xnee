#!/bin/bash


log()
{
    echo "# $*" 
}


check_retval()
{
    RET=$1

    if [ "$RET" != "0" ];
	then
	exit $RET
    fi
}


upload()
{
    FILE=$1
    PRODUCT=$FILE
#    PRODUCT=$(echo $FILE | sed 's,\.tar.gz,,g')

    rm -f ${FILE}.sig

    log "Makeing binary sig"
    gpg -b $FILE
    check_retval $?
    ls -l ${FILE}.sig

    echo "Upload to alpha?"
    read ANSWER

    ALPHA=false
    if [ "$ANSWER" = "Y" ];
	then
	echo "    to alpha"
	ALPHA=true
    fi


    rm -f    ${PRODUCT}.directive ${PRODUCT}.directive.asc    
    log "Making directives file:"
    DIR=xnee
    echo "directory: $DIR" > ${PRODUCT}.directive

    log "clearsiging directives file:"
    gpg --clearsign ${PRODUCT}.directive
    check_retval $?


    log "Start an ftp session:"
    echo "          ftp ftp-upload.gnu.org"
    echo "User: anonymous"


    log "Type the following commands:"
    if [ "$ALPHA" = "true" ];
	then
	echo "          cd /incoming/alpha "
    else
	echo "          cd /incoming/ftp "
    fi
    echo "          binary"
    echo "          put $FILE"
    echo "          put ${FILE}.sig"
    echo "          put ${PRODUCT}.directive.asc    "
}

for i in $( ls *.gz | grep -v directive | grep -v cvs) 
do

  echo "Do you wish to upload [Y/n]: $i"

  read ANSWER

  if [ "$ANSWER" = "Y" ];
      then
      upload $i
  fi

done
