#!/bin/sh

check_src_dst()
{
    SRC=$1
    DST=$2

    if [ ! -f $SRC ]
	then
	echo "File $SRC missing .... leaving"
	exit
    fi
    if [ -f $DST ]
	then
	rm $DST
    fi
}

convert_to_eps()
{
    check_src_dst "$1.dia" "$1.eps"
    dia --nosplash -e $1.eps $1.dia
}

convert_to_pdf()
{
    check_src_dst "$1.eps" "$1.pdf"
    epstopdf  --outfile $1.pdf $1.eps
}

convert_to_png()
{
    check_src_dst "$1.eps" "$1.png"
    convert -density 144x144 $1.eps "$1"_big.png  
    convert -density 60x60 $1.eps $1.png  
    convert -density 32x32 $1.eps "$1"_small.png  
}

convertall()
{
    DIA_FILE=$1
    NOSUFF_FILE=$(echo $1 | sed 's,\.dia,,g')

    echo "converting $DIA_FILE  "
    convert_to_eps   $NOSUFF_FILE
    convert_to_pdf   $NOSUFF_FILE
    convert_to_png   $NOSUFF_FILE
}


if [ "$1" = "clean" ]
then
    rm -f *.eps *.pdf *.png
    exit
fi

echo "Finding all dia files"
for i in $(ls *.dia)
do
  convertall $i
done
