#!/bin/bash


ARG=$1


NAME=`echo $1 | awk ' BEGIN {FS=","} { printf "%s\n", $1}' | sed 's,--,,g' | sed 's,<.*>,,g' ` 
SNAME=`echo $1 | awk ' BEGIN {FS=","} { printf "%s\n", $2}' | sed 's,<.*>,,g' ` 


SCRIPT_NAME=opt_scripts/${NAME}.sh


function write_to_file()
{
    echo "$*" >> $SCRIPT_NAME
}

function sh_header()
{
    write_to_file "#/bin/sh"
    write_to_file "#"
    write_to_file "#"
    write_to_file "# This file is generated from new_scrip.sh"
    write_to_file "#"
    write_to_file "#"
    write_to_file "# Filename:   $SCRIPT_NAME"
    write_to_file "# Generated:  `date`"
    write_to_file "# "
    write_to_file "#"
    write_to_file " "
    write_to_file " "
    write_to_file " "

    write_to_file " "
    write_to_file " # Result counters"
    write_to_file " TOTAL_TEST=0"
    write_to_file " SUCC_TEST=0"
    write_to_file " ERR_TEST=0"
    write_to_file " WARN_TEST=0"
    write_to_file " RETURN_TEST=0"
    write_to_file " RETURN_ERR=0"

    write_to_file " "
    write_to_file " # name of myself"
    write_to_file " MYNAME=$SCRIPT_NAME"
    write_to_file " "
    
    write_to_file "# Arguments to test (short and long)"
    write_to_file "export LONG_ARG=--$NAME"
    write_to_file "export SHORT_ARG=$SNAME"
}

function get_funs()
{
    write_to_file " "
    write_to_file "# source useful funs"
    write_to_file "if [ ! -f etc/base_funs ]; "
    write_to_file "then "
    write_to_file "   echo \"Can't find the file: base_funs\"  "
    write_to_file "   echo \"Start me up in the xnee test dir\" "
    write_to_file "   exit"
    write_to_file "else "
    write_to_file "   . etc/base_funs"
    write_to_file "fi "
    write_to_file " "
    write_to_file " "
    write_to_file " "
    write_to_file "#  "
    write_to_file "# "
    write_to_file "#  "
    write_to_file "# "


}



if [ -f $SCRIPT_NAME ];
then 
    echo "File $SCRIPT_NAME exists ... leaving"
    exit
fi



echo "Creating a new script from name \"$NAME\""

# make the script
touch     $SCRIPT_NAME
sh_header $SCRIPT_NAME
get_funs  $SCRIPT_NAME
chmod a+x $SCRIPT_NAME
