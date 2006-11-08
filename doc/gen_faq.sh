#!/bin/sh

XNEE_MANUAL=$1
FAQ_TXT=$2

if [ "$FAQ_TXT" = "" ]
then
    echo "Missing arg ...."
    exit 1
fi

rm -f $FAQ_TXT

add_to_faq()
{
    echo "$*" >> $FAQ_TXT
}

add_to_faq "*** GNU Xnee FAQ *** "
add_to_faq ""
add_to_faq "   (this file was generated from the complete GNU Xnee Manual)"
add_to_faq ""


cat $XNEE_MANUAL  | gawk 'BEGIN {faq_found=0; app_found=0; dots_found=0} \
                      /[0-9]* FAQ/ {faq_found++; dots_found=0} \
                      /Appendix/ {app_found++;} \
                      /***/ {dots_found++;} \
                      { if ( (faq_found==2) && (app_found==1) && (dots_found>2) ) { print $0 } }'  >> $FAQ_TXT
