#!/bin/bash

# prints out every command during execution
#set -x

ls .
if [ $? -ne 0 ]
then
    exit 1
fi

#ls /root/ -lsa
if [ $? -ne 0 ]
then
    exit 1
fi

ls .
if [ $? -ne 0 ]
then
    exit 1
fi

# dangerous pretty print of PATH
echo "Modifying IFS way of printing \$PATH"
old=$IFS
IFS=:
printf "%s\n" $PATH
IFS=$old

echo "the awk way"

# prints $PATH sorted
#echo $PATH | awk '{split($0,a,":"); } END { for (x in a) print a[x]}' | sort

# also removes duplicates
echo $PATH | awk '{split($0,a,":"); } END { for (x in a) print a[x]}' | sort | awk '{ if (a[$1]++ == 0) print $0; }' "$@"


echo "First arg (printed with \$1): $1"
echo "Second arg (printed with \$2): $2"

echo "Parsing with getopt\nAll args:"
for i
do 
  echo $i
done
args=`getopt x:y:z: $*`
set -- $args
echo "Parsed arguments"
for i
do
  echo $i
done

echo "Array:$args"
if [[ " ${args[*]} " == *"-x"* ]]; then
  echo "Arguments contains -x"
else
  echo "Missing argument -x"
  exit
fi

if [[ " ${args[*]} " == *"-y"* ]]; then
  echo "Arguments contains -y"
else
  echo "Missing argument -y"
  exit
fi

if [[ " ${args[*]} " == *"-z"* ]]; then
  echo "Arguments contains -z"
else
  echo "Missing argument -z, but that's okay"
fi

for i in {1..10}
do
  echo "$i"
done

x=`ls /`
echo $x

# bash function that returns
function get_timestamp() {
    local __resultvar=$1
    local myresult=`date +%Y%m%d-%H%M%S`
    eval $__resultvar="'$myresult'"
}

get_timestamp result
echo "I got $result"

set -x
date -d "5 days ago"

for i in {1..4}
do 
    get_timestamp result
    echo $result > out/foo-${result}
    #sleep 1
done
