#!/bin/bash
find $1 |while read line;do
    grep -q -s " " <<< $line 
    if [ $? == 0 ]; then
    REP=`sed "s/ /_/g" <<< $line`
	echo "$line -> $REP"
	mv "$line" $REP
    fi
done
