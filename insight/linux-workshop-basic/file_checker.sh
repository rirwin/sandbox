#!/bin/bash

while getopts "f:d:wx" opt; do
  case $opt in
    f)
      FC_FILE=$OPTARG
      ;;
    d) 
      FC_FILE=$OPTARG
      ;;
    x)
      FC_EXEC_TEST=1
      ;;
    w)
      FC_WRITE_TEST=1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

if [ -z "$FC_FILE" ]; then
  echo "You need to pass -f <filename> or -d <dirname>"
  exit
fi


if [ -f "$FC_FILE" ] 
then
  echo "$FC_FILE exists"
else
  echo "$FC_FILE does not exist"
  exit
fi

if [ -z "$FC_EXEC_TEST" ]
  then
  if [ `stat -c %A $FILE | sed 's/...\(.\).\+/\1/'` == "x" ] 
  then
    echo "Owner has execute permission"
  else
    echo "Owner does not have execute permission"
  fi
fi



echo $FC_FILE
echo $FC_EXEC_TEST
echo $FC_WRITE_TEST

