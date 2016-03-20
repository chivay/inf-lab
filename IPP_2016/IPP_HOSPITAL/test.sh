#!/bin/bash

#USAGE ./test.sh prog directory
# direcotry/*.in
# directory/*.out
# -v .err

if [ $# -lt 2 ]
then
	echo "USAGE: $0 prog directory";
	exit;
fi

INPUT=$2"/*.in"

for input in $INPUT
do
	echo -n "$input: ";
	if $1 < $input | diff - ${input%in}out  >/dev/null 2>&1;
	then 
		echo OK;
	else
		echo Źle!;
	fi
done