#!/bin/bash
git log $1 |while read line;do

grep -q -s -E ^commit <<< $line

if [ $? == 0 ];then
	COMMIT=`sed -E "s/commit (.+)/\1/" <<< $line`
#echo $COMMIT
	SIZE=`git ls-tree -l $COMMIT $1 | sed -E "s/(.+) (.+) (.+) (.+)/\4/"`
else
	grep -q -s -E ^Date <<< $line 
	if [ $? == 0 ]; then
	echo $line $SIZE
	fi
fi

done
