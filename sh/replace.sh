#!/bin/bash
find $1 |while read line;do
    grep -q $2 $line 
    if [ $? == 0 ]; then
    echo replace $line
    sed -i "s/$2/$3/g" $line
    fi
done
