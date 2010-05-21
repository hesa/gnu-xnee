#!/bin/sh

MASTER="Xnee"


add_xi2_master()
{
    xinput  --list | awk ' BEGIN { mode="no" ; } /Virtual core pointer/ { mode="pointer" ; }    /Virtual core keyboard/ { mode="kbd" ; }   { if ( mode=="pointer" ) { print $0 ; } } ' | grep -v "Virtual core" | sed  -e 's,^⎜   ↳[ \t.]*,,g'  -e 's,id=[0-9]*[ \t]*\[[a-z0-9\(\) ]*][ \t]*,,g' | \
	while (true) ; 
    do 
	read LINE; 
	if [ "$LINE" = "" ] ; then break ; fi ; 
#	echo xinput --reattach "\"$LINE\"" "\"$MASTER pointer\""
	xinput --reattach "$LINE" "$MASTER pointer"
    done
}

rem_xi2_master()
{
    xinput  --list | awk ' BEGIN { mode="no" ; } /Xnee pointer/ { mode="pointer" ; }    /Virtual core keyboard/ { mode="kbd" ; }   { if ( mode=="pointer" ) { print $0 ; } } ' | grep -v "Virtual core" | sed  -e 's,^⎜   ↳[ \t.]*,,g'  -e 's,id=[0-9]*[ \t]*\[[a-z0-9\(\) ]*][ \t]*,,g' | \
	while (true) ; 
    do 
	read LINE; 
	if [ "$LINE" = "" ] ; then break ; fi ; 
#	echo xinput --reattach "\"$LINE\"" "\"$MASTER pointer\""
	xinput --reattach "$LINE" "$MASTER pointer"
    done
}

create_xi2_master()
{
   xinput --create-master "$MASTER" 
}

remove_xi2_master()
{
    xinput --remove-master "$MASTER pointer" 
    xinput --remove-master "$MASTER keyboard" 
}


if [ "$1" = "-d" ]
then
    MASTER="Virtual core"
    rem_xi2_master
    MASTER="Xnee"
    remove_xi2_master
else
    create_xi2_master
    add_xi2_master
fi



