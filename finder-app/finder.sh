#!/bin/bash
# finder.sh script for assignment 1
# Author: Peter Fonseca

set -e
set -u

pushd .

if [ $# -eq 2 ]
then
   if [ -d "$1" ]
   then
      cd "$1"
      NUMFILES=`grep -Rm 1 "$2" * | wc -l`
      NUMMATCH=`grep -R "$2" * | wc -l`

      echo "The number of files are $NUMFILES and the number of matching lines are $NUMMATCH"
      popd
      exit 0
   else
      echo "Directory $1 doesnt exist"
      popd
      exit 1
   fi
else
   echo "Usage: finder.sh <folder> <search term>"
   popd
   exit 1
fi

