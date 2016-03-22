#!/bin/bash

#USAGE ./test.sh prog directory
# direcotry/*.in
# directory/*.out
# -v .err

if [ $# -lt 2 ]
then
	echo "USAGE: $0 (-v) prog directory";
	exit;
fi

if [ "$1" = "-v" ];
then
	verbose=true
	shift
fi

INPUT=$2"/*.in"

for input in $INPUT
do
	echo -n "$input: ";
	if [ "$verbose" = "true" ];
	then
		$1 -v < $input >log.out 2>log_err.out
	else
		$1 < $input >log.out 2>log_err.out
	fi

	if diff log.out ${input%in}out  >/dev/null 2>&1;
	then 
		echo -n "STDOUT:OK ";
	else
		echo -n "STDOUT:Zle! ";
	fi

	if [ "$verbose" = "true" ];
	then
		if diff log_err.out ${input%in}err  >/dev/null 2>&1;
		then 
			echo -n "STDERR:OK";
		else
			echo -n "STDERR:Zle!";
		fi
	fi
	echo

done

rm log.out log_err.out > /dev/null