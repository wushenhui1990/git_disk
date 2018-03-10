#!/bin/sh

find_result=""
C_FILES=$(find -name "*.c")

find_fun_in_cfiles() #$1 fun $2 cfiles
{
find_result=""
while read line; do
	grep -q -s "$1" "$line"
	if [ $? == 0 ];then #find ok
		echo "find fun $1 in $line"
		find_result="$line"
		break;
	fi
done <<< `echo "$2"`
}

find_funs_in_cfiles() #$1:fun $2 cfiles
{
while read abc; do
find_fun_in_cfiles "$abc" "$2" 
if [ -z "$find_result" ];then
	echo "fun:$abc is not been use"
	TEMP=$(/home/wsh/git_disk/tools_src/del_fun_c "$abc" "$SRC_PATH")
	if [ $? == 0 ] ;then
		echo "del fun $abc in $SRC_PATH"
		echo "$TEMP" > "$SRC_PATH"
	fi
#else 
#	echo file:"$1" fun:"$abc" is use in "$find_result"
fi
done <<< `echo "$1"`
}

get_unused_fun() #$1 obj path $2 fun name
{
echo "find $1"
if [ -z "$2" ];then
	RAW=$(nm $1 | grep " T ")
	FUN=$(sed -E "s/[^ ]* T (.*)/\1/g" <<< $RAW)
else
	FUN="$2"
fi
#echo "$RAW"

SRC_FILE=$(basename $1)
SRC_FILE=${SRC_FILE%.o}.c

SRC_PATH=${1%.o}.c

echo "SRC_FILE: $SRC_FILE"
C_FILES=$(grep -v "$SRC_FILE" <<< "$C_FILES")

find_funs_in_cfiles "$FUN" "$C_FILES"
}

if [ "$1" == "all" ]; then
OBJS=$(find -name "*.o")
OBJS=$(grep -v "targets" <<< "$OBJS")

while read line;do
get_unused_fun "$line"
done <<< "$OBJS"

else
#get_unused_fun "$1" "$2"
get_unused_fun "$1"
fi






