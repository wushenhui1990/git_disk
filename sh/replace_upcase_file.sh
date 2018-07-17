#!/bin/sh
if [ $# -lt 2 ];then
echo "args: $#"
echo "args: path file_pattern"
exit 0
fi


find $1 -name $2 |grep -E "[^.]*[A-Z]+" |while read line; do
FILE_NAME=${line##*/}
#echo $FILE_NAME
FILE_PATH=${line%/*}
#echo $FILE_PATH
FILE_NAME=${FILE_NAME,,}
NEW_PATH=$FILE_PATH/$FILE_NAME
#echo $NEW_PATH

echo "$line -> $NEW_PATH ? [y/n]"

read varname </dev/tty
echo $varname
#echo $varname
if [ $varname = "y" ]; then
mv -v $line $NEW_PATH
fi

done;
