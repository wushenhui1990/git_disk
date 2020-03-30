#!/bin/bash
if [ $# -le 2 ];then
echo "args: $#"
echo "str1, str2, [path]"
echo "str1 > str2, in path/or cur path if not set"
exit 0
fi

if [ $# le 2 ];then
FPATH=$3
else
FPATH=.
fi

if [ -f $FPATH]; then
	grep -s -q "$1" $FPATH 
	if [ $? == 0 ]; then
	echo replace $FPATH
	sed -i "s/$1/$2/g" $FPATH
	fi
else
	find $FPATH |grep -v ".git" | while read line;do
		grep -s -q "$1" $line 
		if [ $? == 0 ]; then
		echo replace $line
		sed -i "s/$1/$2/g" $line
		fi
	done
fi
