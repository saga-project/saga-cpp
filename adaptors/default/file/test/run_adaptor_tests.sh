#!/bin/sh
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Header
while getopts h arg
do
   case $arg in
      h) echo "This program performs some rudimentary adaptor tests."
         exit 0;;
   esac
done


######################
#### EXAMPLE TEST ####
######################

# Define a filename
FILE=/tmp/saga.adaptor.default.test.$$
#
# Give the file some contents
STRING=saga_was_here
`echo "$STRING" > $FILE.src`

# SAGA CALL NUMBER 1
echo 'Running a remote echo test and put result in tmp file'
`$SAGA_LOCATION/bin/saga-file copy file://localhost/$FILE.src file://localhost/$FILE.tgt`
#
# SAGA CALL NUMBER 2
echo 'Try to cat the file using ssh'
RESULT=`$SAGA_LOCATION/bin/saga-file cat file://localhost/$FILE.tgt`
#
# SAGA CALL NUMBER 3
echo 'Clean up temporaries'
rm -f $FILE.src
`$SAGA_LOCATION/bin/saga-file remove file://localhost/$FILE.tgt`
#
# CHECK RESULT
if test "$RESULT" != "$STRING"
then
   # Something went wrong!
   echo "Saga could not succesfully cat the file!"
   exit 1
else
   # Tests succeeded! 
   echo "Success"
   exit 0;
fi

#####################################################
#### ADD MORE TESTS SIMILAR TO THE ONE ABOVE !!! ####
#####################################################

# Try to cover all commands the command line tool has to offer

# Try different combinations of URLs

# Make sure to return 0 in case of success and 1 in case of an error

# Please try to come up with *decriptive* error messages!

# If your tests uses remote infrastructure, e.g. Globus, please define 
# the hostname, directory names, etc. as VARIABLES so it can be migrated 
# without changing every single saga call!

# Thanks ;-)
