#!/bin/bash

while getopts "f:d:wx" opt; do
  case $opt in
    f)
      FILE=$OPTARG
      ;;
    d) 
      DIR=$OPTARG
      ;;
    x)
      EXEC_TEST=1
      ;;
    w)
      WRITE_TEST=1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

if [ -z "$FILE" ] && [ -z "$DIR" ]; 
then
  echo "You need to pass -f <filename> or -d <dirname>"
  exit
fi

if [ -n "$FILE" ] 
then
  if [ -f "$FILE" ] 
  then
    echo "File $FILE exists"
  else
    echo "File $FILE does not exist"
    exit
  fi
fi

if [ -n "$DIR" ]
then 
  if [ -d "$DIR" ] 
  then
    echo "Directory $DIR exists"
  else
    echo "Directory $DIR does not exist"
    exit
  fi
fi

if [ -n "$FILE" ] 
then 
  FILE_OR_DIR=$FILE
else
  FILE_OR_DIR=$DIR
fi

if [ -n "$EXEC_TEST" ]
then
  if [ `stat -c %A $FILE_OR_DIR | sed 's/...\(.\).\+/\1/'` == "x" ] 
  then
    echo "Owner has execute permission"
  else
    echo "Owner does not have execute permission"
  fi
fi

if [ -n "$WRITE_TEST" ] 
then
  if [ -w "$FILE_OR_DIR" ]
  then
    echo "Owner has write permission"
  else
    echo "Owner does not have write permission"
  fi
fi 
