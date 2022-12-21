#!/bin/bash
# writer.sh script for assignment 1
# Author: Peter Fonseca

set -e
set -u

if [ $# -eq 2 ]
then
   mkdir -p "$(dirname "$1")"
   echo "$2" > $1
   if [ $? -ne 0 ]
   then
      echo "File couldn't be created"
      exit 1
   fi
else
   echo "Usage: writer.sh <filename> <text-to-add>"
   exit 1
fi

