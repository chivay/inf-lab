#!/bin/bash

# Running without arguments
if [ $# -lt 2 ]
then
	echo "USAGE: $0 (-v) prog directory";
	exit;
fi

# Check verbosity
if [ "$1" = "-v" ];
then
	verbose=true
	shift
fi

INPUT=$2"/*.in"

for input in $INPUT
do
	echo "Checking $input...";

	# Run test
	if [ "$verbose" = "true" ]; then
		$1 -v < $input >log.out 2>log_err.out
	else
		$1 < $input >log.out 2>log_err.out
	fi

	# Check STDOUT
	if ! diff log.out ${input%in}out  >/dev/null 2>&1; then
		echo "STDOUT: ERROR!";
	fi

	# If verbose, check STDERR
	if [ "$verbose" = "true" ]; then
		if ! diff log_err.out ${input%in}err  >/dev/null 2>&1; then
			echo "STDERR: ERROR!";
		fi
	fi

done

# Remove temporary files
rm log.out log_err.out > /dev/null