XNEE_DATA_FILES = simple_bash.sh 
EXTRA_DIST = $(XNEE_DATA_FILES)
pkgdata_DATA = $(XNEE_DATA_FILES)

simple_bash.sh: simple_bash.sh.in1
	echo "#!" "$(BASH)"                     > simple_bash.sh
	echo " "                               >> simple_bash.sh 
	echo " "                               >> simple_bash.sh 
	echo "if [ -f ${bindir}/bin/cnee ]"    >> simple_bash.sh 
	echo "then"                            >> simple_bash.sh 
	echo "    CNEE=${bindir}/bin/cnee"     >> simple_bash.sh 
	echo "fi"                              >> simple_bash.sh 
	echo " "                               >> simple_bash.sh 
	echo " "                               >> simple_bash.sh 
	echo "if [ -f ${datadir}/$(PACKAGE)/Xnee.sh ]"   >> simple_bash.sh 
	echo "then"                                      >> simple_bash.sh 
	echo "    . ${datadir}/$(PACKAGE)/Xnee.sh"       >> simple_bash.sh 
	echo "    XNEE_SH=${datadir}/$(PACKAGE)/Xnee.sh" >> simple_bash.sh 
	echo "fi"                              >> simple_bash.sh 
	cat simple_bash.sh.in1                 >> simple_bash.sh 
	chmod ugo+x simple_bash.sh 

