#!/bin/bash

>ex1.txt

echo "Enter your name: "

read yourName

if [ -z "$yourName" ]
then
	echo "Empty String."	
	echo "Empty String.">>ex1.txt
else 
	echo "Your name is ${yourName}"
	echo "Your name is ${yourName} .">>ex1.txt
fi

sysInfor="$(uname -a)"
echo "Your system information is ;  $sysInfor  "
echo "Your system information is :  $sysInfor ">>ex1.txt
