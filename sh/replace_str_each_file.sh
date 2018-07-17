#!/bin/bash
if [ $# -le 2 ];then
echo "args: $#"
echo "str1, str2, [path]"
echo "str1 > str2, in path/or cur path if not set"
exit 0
fi
echo "$1 > $2"

if [ "$#" > 2 ];then
FPATH=$3
else
FPATH=.
fi

find $FPATH | while read line;do
    grep -s -q $1 $line 
    if [ $? == 0 ]; then
    echo replace $line
    sed -i "s/$1/$2/g" $line
    fi
done
