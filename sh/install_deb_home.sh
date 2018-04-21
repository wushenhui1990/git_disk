#!/bin/bash
#extract packge
dpkg -x $1 $HOME/deb
#find symbol link
dpkg -c $1 |grep -e "->" > temp1
grep -E " \/.*$" temp1 > temp2
while read line           
do
LINK_NAME=$(echo $line |grep -o -E "\./\S* " |tr -d " ")
LINK_TARGET=$(echo $line |grep -o -E "\/\S*$" |tr -d " " |sed -e 's/^\//\/home\/wsh\/\deb\//g')
ln -f -s $LINK_TARGET $LINK_NAME
echo "$LINK_TARGET -> $LINK_NAME"
done <./temp2
rm temp1 temp2
