#!/bin/sh

find_result=""
TEMP_TXT=temp_obj.txt
TEMP_TXT2=temp_obj2.txt
PREPROCESSED_C=cur_c.txt

find_fun_in_symbole_file() #$1 fun $2 C FILE
{
find_result=""
grep -q -s "$1" "$TEMP_TXT"
if [ $? == 0 ];then #find ok
#echo "find fun $1 in $TEMP_TXT"
	find_result="$1"
	break;
fi

TEMP=$(/home/wsh/git_disk/tools_src/del_fun_declare_c "$1" "$2")
if [ $? == 0 ] ;then
#echo "del fun declare $1 in $2"
	echo "$TEMP" > "$2"
fi

if [ -z $find_result ]; then
	FUN_COUNT=$(grep -c "$1" "$2")
	if [ "$FUN_COUNT" != '1' ];then #find ok call in own c file
#echo "find count:$FUN_COUNT $1 in $2"
		find_result="$1"
	fi
fi
}

find_funs_in_symbole_file() #$1:funs $2 objpath
{
DEL_FUN_COUNT=$((0))
SRC_PATH=${2%.o}.c
while read abc; do
#find_fun_in_symbole_file "$abc" "$SRC_PATH"
find_fun_in_symbole_file "$abc" "$PREPROCESSED_C"
if [ -z "$find_result" ];then
echo "file $SRC_PATH fun:$abc is not been use"
	TEMP=$(/home/wsh/git_disk/tools_src/del_fun_c "$abc" "$SRC_PATH")
	if [ $? == 0 ] ;then
		echo "del fun $abc in $SRC_PATH"
		echo "$TEMP" > "$SRC_PATH"
		DEL_FUN_COUNT=$((++DEL_FUN_COUNT))
	fi
#else 
#	echo file:"$1" fun:"$abc" is use in "$find_result"
fi
done <<< `echo "$1"`
return $DEL_FUN_COUNT
}

del_fun_declare() #$1 fun $2 cfile
{
	FUN_COUNT=$(grep -c "$1" "$2")
	if [ "$FUN_COUNT" != '1' ];then #find ok call in own c file
#echo "find count:$FUN_COUNT $1 in $2"
		find_result="$1"
	fi
}

prepare_obj_symbols() #$1 objs $2 op obj path
{
SRC_PATH=${2%.o}.c

grep -q -s "$2" "$TEMP_TXT"
if [ $? != 0 ] ;then
	echo "nm all obj"
	echo "" > "$TEMP_TXT"
	while read abc; do
	nm -o "$abc" >> "$TEMP_TXT"
	done <<< "$1"

	cp "$TEMP_TXT" "$TEMP_TXT2"
	grep -v "$2" "$TEMP_TXT2" > "$TEMP_TXT"
else
	cp "$TEMP_TXT" "$TEMP_TXT2"
	grep -v "$2" "$TEMP_TXT2" > "$TEMP_TXT"
fi

#nm -a "$2" > "$CUR_OBJ_TXT"
make src=$SRC_PATH dst=$PREPROCESSED_C preprocess

}

get_unused_fun() #$1 obj path $2 fun name
{
if [ -z "$2" ];then
	RAW=$(nm $1 | grep " T ")
	FUN=$(sed -E "s/[^ ]* T (.*)/\1/g" <<< $RAW)
else
	FUN="$2"
fi
#echo "$RAW"

OP_OBJ_PATH=$1

echo "OP_OBJ_PATH: $OP_OBJ_PATH"

OTHER_OBJS=$(find -name "*.o")
echo "$OTHER_OBJS" > origin.txt
OTHER_OBJS=$(grep -v "targets" <<< "$OTHER_OBJS")

echo "$OTHER_OBJS" > striped.txt

echo "prepare obj symbols"
prepare_obj_symbols "$OTHER_OBJS" "$1"
echo "prepare obj symbols ok"

find_funs_in_symbole_file "$FUN" "$1"
COUNT=$?
while [ $COUNT != '0' ]
do
find_funs_in_symbole_file "$FUN" "$1"
COUNT=$?
done

echo "revert obj symbol"
nm -o "$OP_OBJ_PATH" >> "$TEMP_TXT"

}

if [ "${1: -1}" == "o" ]; then
get_unused_fun "$1"
else
#get_unused_fun "$1" "$2"
OBJS=$(find "$1" -name "*.o")
OBJS=$(grep -v "targets" <<< "$OBJS")

while read line;do
get_unused_fun "$line"
done <<< "$OBJS"
fi

rm "$TEMP_TXT"
rm "$TEMP_TXT2"
rm "$PREPROCESSED_C"





