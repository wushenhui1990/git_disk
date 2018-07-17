#!/bin/bash
SYNC_HOME=$HOME/git_disk/home
find $SYNC_HOME -maxdepth 1 -mindepth 1| while read line; do
	NAME=${line%/}
	NAME=${NAME##*/}
	rsync -r -v  "$HOME/$NAME" $SYNC_HOME
done
