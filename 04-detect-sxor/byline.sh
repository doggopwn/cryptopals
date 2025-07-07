#!/bin/zsh

if [[ $# -lt 2 ]]; then
	print -P "Usage: byline [command] [input_file]\nThe \$line variable is defined to use in your commands."
	exit 1
fi
cmdpart=$1
input_file=$2

lineidx=1

cat $input_file | while read line; do
	eval "$cmdpart"
	((lineidx++))
done
