#!/bin/sh

find -name "*.[ch]" |while read line; do
    grep -s -q $1 $line 
    if [ $? == 0 ]; then
		perl -0777 -pi -e 's/#include "sink_device_config.h"\n//g' $line
		echo remove $1 in $line
    fi
done
find -name "*.c" |while read line; do
    grep -s -q $1 $line 
    if [ $? != 0 ]; then
		declare -i if_num=0
		declare -i include_num=0
		if_num+=$(grep -n -m1 "#ifdef" $line |cut -f1 -d:)
		include_num+=$(grep -n -m1 "#include" $line |cut -f1 -d:)
		echo "if num $if_num, include num $include_num"
		if [ $if_num -gt 0 -a $if_num -lt $include_num ]; then
			echo ifdef add $1 in $line
			perl -0777 -pi -e 's/#ifdef(.*)/#include "sink_device_config.h"\n#ifdef\1/' $line
		else
			echo include add $1 in $line
			perl -0777 -pi -e 's/#include(.*)/#include "sink_device_config.h"\n#include\1/' $line
		fi
    fi

done
