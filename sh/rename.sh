#!/bin/bash

echo $1
#find $1 -maxdepth 1 -type f |while read line;do
find "$1" -type f |while read line;do
#echo $line
FILE=$(basename "$line")
FPATH=$(dirname "$line")
RENAME=$(echo $FILE |sed -e 's/\[[^]]*\]//g' -e 's/^\.//g')
#echo "$FILE -> $RENAME"

echo $FPATH
echo $RENAME
TPATH=$FPATH/$RENAME
echo "$line ->"
echo $TPATH
echo ------------------

mv $line $TPATH





done
