#!/bin/bash
if [ $# -le 2 ];then
echo "args: $#"
echo "str1, str2, [path]"
echo "str1 > str2, in path/or cur path if not set"
exit 0
fi

FPATH=$3
echo $1
echo $2
echo $3

if [ -f $FPATH ]; then
	NEWPATH=$(echo $FPATH |sed -E "s/$1/$2/g")
	mv -v $FPATH $NEWPATH
else
	find $FPATH -type f|while read line;do
		NEWPATH=$(echo $line |sed -E "s/$1/$2/g")
		mv -v $line $NEWPATH
	done
fi
