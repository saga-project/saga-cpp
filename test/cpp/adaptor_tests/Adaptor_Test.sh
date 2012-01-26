#!/bin/sh

# Copyright (c) 2009 Chris Miceli (cmicel1@cct.lsu.edu)
# Distributed under the Boost Software License, Version 1.0. (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Header
DESCRIBE_ONLY=""

while getopts d arg
do
   case $arg in
      d) DESCRIBE_ONLY="true";;
   esac
done

#Begin program

if test -z ${SAGA_LOCATION}
then
   echo "Please set SAGA_LOCATION!"
   return 0 
fi

for dir in *
do
   echo $dir
   if test -d $dir
   then
      echo $dir
      cd $dir
      for adaptor_test in *.test
      do
         if test "$DESCRIBE_ONLY" = "true"
         then
            sh $adaptor_test -d
         else
            # Copy universal saga.ini file to prevent
            # other adaptors from loading
            cp ../.saga.ini .
            echo -n "$adaptor_test..."
            sh $adaptor_test
            # > /dev/null 2>&1
            if test $? -ne 0
            then
               echo "Failure: $adaptor_test"
            fi
            rm .saga.ini
         fi
      done
   fi
done
