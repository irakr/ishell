#!/bin/bash

######### Insert license #########
#
# Written by: Irak Rigia
# Date: 14th July 2016
#################################
#
# This shell script inserts the license text from the file LICENSE.comment to the beginning of each source/header
# files passed as arguments to the script.
# 

#Storing arguments
args=("$@")

#No. of args
n=${#args[@]}

#Directories
#assembler=src/assembler/
#loader=src/loader/

#Process all arguments one by one
for (( i=0;i<$n;i++ ));
do
	echo "Processing ${args[$i]}"
	cat ${args[$i]} >${args[$i]}.temp
	#echo "/*" >${args[$i]}
	cat LICENSE.comment >${args[$i]}
	#echo "*/" >>${args[$i]}
	cat ${args[$i]}.temp >>${args[$i]}
	rm ${args[$i]}.temp
done 
