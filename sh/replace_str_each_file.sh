#!/bin/bash
if [ $# -le 1 ];then
echo "args: $#"
echo "str1, or str2, [path]"
echo "str1 > str2 in path or strip str1 in path"
exit 0
fi

REP_STR=
if [ -e $2 ];then
FPATH=$2
else
REP_STR=$2
FPATH=$3
fi

echo "replace in $FPATH  str: $1 -> $REP_STR"
if [ -f $FPATH ]; then
	grep -s -q "$1" $FPATH 
	if [ $? == 0 ]; then
	echo "replace $FPATH $1 -> $REP_STR"
	sed -i "s/$1/$REP_STR/g" $FPATH
	fi
else
	find $FPATH |grep -v ".git" | while read line;do
		grep -s -q "$1" $line 
		if [ $? == 0 ]; then
		echo "replace $line $1 -> $REP_STR"
		sed -i "s/$1/$REP_STR/g" $line
		fi
	done
fi
