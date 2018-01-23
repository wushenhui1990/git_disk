pax> fspec=/path/to/some/file.txt

pax> echo ${fspec##*/}   # greedy remove */ at start
file.txt
There's also a single-# variant which is a non-greedy match and equivalents for suffixes:

pax> echo ${fspec#*/}    # non-greedy remove */ at start
path/to/some/file.txt

pax> echo ${fspec%%/*}   # greedy remove /* at end

pax> echo ${fspec%/*}    # non-greedy remove /* at end
/path/to/some
The ##*/ and %/* are roughly equivalent to what you get from basename and dirname respectively, but within bash so you don't have to invoke an external program:

pax> basename $fspec
file.txt

pax> dirname $fspec
/path/to/some
