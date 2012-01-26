#!/bin/sh
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Header
while getopts h arg
do
   case $arg in
      h) echo "This program performs some rudimentary REPLICA adaptor tests."
         exit 0;;
   esac
done

if test "x$REMOTEHOST" = "x"; then
  REMOTEHOST=localhost
  
echo '[saga.adaptors.default_replica]
  name      = default_replica
#  path      = $[saga.location]/lib

[saga.adaptors.default_replica.preferences]
  # adaptor configuration

  # specify what backend to use
  dbtype    = sqlite3


[saga.adaptors.default_replica.preferences.sqlite3]
  # specify a file name where sqlite3 can store its data.
  # relative and absolute paths are both supported.  Relative paths are 
  # relative to the applications cwd.

  dbconnect = ./saga_replica.db' > .saga.ini
  
fi

# NOTE:

# Remove old content that may have been created by previous tests,
# which may not have succeeded
rm -f saga_replica.db



# Define pathnames and filenames

# Two paths
PATH1=replica://localhost
PATH2=replica://localhost/saga.adaptor.replica.directory

# One file
FILE=saga.adaptor.replica.test

# Two physical file names
PFN1=me
PFN2=metoo



# SAGA CALL NUMBER 1
echo $SAGA_LOCATION/bin/saga-replica list_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-replica list_directory $PATH1`
if test "$RESULT" != ""
then
   echo $SAGA_LOCATION/bin/saga-replica remove_directory $PATH1
   `$SAGA_LOCATION/bin/saga-replica remove_directory $PATH1`
   #exit 1
fi

# SAGA CALL NUMBER 2
echo $SAGA_LOCATION/bin/saga-replica add_lfn $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica add_lfn $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_lfn could not add a logical file"
   exit 1
fi

# SAGA CALL NUMBER 3
echo $SAGA_LOCATION/bin/saga-replica list_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-replica list_directory $PATH1`
if test "$RESULT" != "  $FILE"
then
   echo "Error: Saga replica list_directory does not show a recently added logical file"
   exit 1
fi

# SAGA CALL NUMBER 4
echo $SAGA_LOCATION/bin/saga-replica list_pfns $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_pfns $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica list_pfns is not empty initially"
   exit 1
fi

# SAGA CALL NUMBER 5
echo $SAGA_LOCATION/bin/saga-replica add_pfn $PATH1/$FILE $PFN1
RESULT=`$SAGA_LOCATION/bin/saga-replica add_pfn $PATH1/$FILE $PFN1`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_pfn could not add a physical file name"
   exit 1
fi

# SAGA CALL NUMBER 6
echo $SAGA_LOCATION/bin/saga-replica list_pfns $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_pfns $PATH1/$FILE`
if test "$RESULT" != "  $PFN1"
then
   echo $RESULT
   echo $PFN1
   echo "Error: Saga replica list_pfns does not show a recently added physical file name"
   exit 1
fi

# SAGA CALL NUMBER 7
echo $SAGA_LOCATION/bin/saga-replica add_lfn $PATH2
RESULT=`$SAGA_LOCATION/bin/saga-replica add_lfn $PATH2`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_lfn could not add a logical directory"
   exit 1
fi

# SAGA CALL NUMBER 8
echo $SAGA_LOCATION/bin/saga-replica add_lfn $PATH2/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica add_lfn $PATH2/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_lfn could not add a logical file"
   exit 1
fi

# SAGA CALL NUMBER 9
echo $SAGA_LOCATION/bin/saga-replica add_pfn $PATH2/$FILE $PFN1
RESULT=`$SAGA_LOCATION/bin/saga-replica add_pfn $PATH2/$FILE $PFN1`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_pfn could not add a physical file name"
   exit 1
fi

# SAGA CALL NUMBER 10
echo $SAGA_LOCATION/bin/saga-replica add_pfn $PATH2/$FILE $PFN2
RESULT=`$SAGA_LOCATION/bin/saga-replica add_pfn $PATH2/$FILE $PFN2`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_pfn could not add a physical file name"
   exit 1
fi

# SAGA CALL NUMBER 11
echo $SAGA_LOCATION/bin/saga-replica list_pfns $PATH2/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_pfns $PATH2/$FILE`
if test "$RESULT" != "  $PFN1
  $PFN2"
then
   echo "Error: Saga replica list_pfns does not show the two recently added physical file names"
   exit 1
fi

# SAGA CALL NUMBER 12
echo $SAGA_LOCATION/bin/saga-replica remove_pfn $PATH1/$FILE $PFN1
RESULT=`$SAGA_LOCATION/bin/saga-replica remove_pfn $PATH1/$FILE $PFN1`
if test "$RESULT" != ""
then
   echo "Error: Saga replica remove_pfn could not remove a physical file name"
   exit 1
fi

# SAGA CALL NUMBER 13
echo $SAGA_LOCATION/bin/saga-replica list_pfns $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_pfns $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica list_pfns does still show a recently removed physical file name"
   exit 1
fi

# SAGA CALL NUMBER 14
echo $SAGA_LOCATION/bin/saga-replica remove_lfn $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica remove_lfn $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica remove_lfn could not remove a logical file"
   exit 1
fi

# SAGA CALL NUMBER 15
echo $SAGA_LOCATION/bin/saga-replica remove_lfn $PATH2/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica remove_lfn $PATH2/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica remove_lfn could not remove a logical file"
   exit 1
fi

# SAGA CALL NUMBER 16
echo $SAGA_LOCATION/bin/saga-replica remove_lfn $PATH2
RESULT=`$SAGA_LOCATION/bin/saga-replica remove_lfn $PATH2`
if test "$RESULT" != ""
then
   echo "Error: Saga replica remove_lfn could not remove a logical directory"
   exit 1
fi

# SAGA CALL NUMBER 17
echo $SAGA_LOCATION/bin/saga-replica list_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-replica list_directory $PATH1`
if test "$RESULT" != ""
then
   echo "Error: Saga replica list_directory is not empty after removing logical file"
   exit 1
fi

# ## Add directory, remove directory??

### list_attributes, add_attribute, remove_attribute
# SAGA CALL NUMBER 2
echo $SAGA_LOCATION/bin/saga-replica add_lfn $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica add_lfn $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica add_lfn could not add a logical file"
   exit 1
fi

# SAGA CALL NUMBER 18
echo $SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE  | grep -v ctime | grep -v utime`
#echo "[$RESULT]"
if test "$RESULT" != ""
then
   echo "Error: Saga replica list_attributes is not empty in the atribute-less logical file"
   exit 1
fi

# SAGA CALL NUMBER 19
echo $SAGA_LOCATION/bin/saga-replica set_attribute $PATH1/$FILE Interactive True
RESULT=`$SAGA_LOCATION/bin/saga-replica set_attribute $PATH1/$FILE Interactive True`
RESULT2=`$SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE | grep -v ctime | grep -v utime`
#echo "[$RESULT]"
#echo "[$RESULT2]"
if test "$RESULT" != ""
then
   echo "Error: Saga replica set_attribute could not add an attribute to logical file"
   exit 1
fi

# SAGA CALL NUMBER 20
echo $SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE`
#echo "[$RESULT]"
if test "$RESULT" = ""
then
   echo "Error: Saga replica list_attributes is empty after adding attribute to logical file"
   exit 1
fi

# SAGA CALL NUMBER 21
echo $SAGA_LOCATION/bin/saga-replica remove_attribute $PATH1/$FILE Interactive 
RESULT=`$SAGA_LOCATION/bin/saga-replica remove_attribute $PATH1/$FILE Interactive | grep -v ctime | grep -v utime`
if test "$RESULT" != ""
then
   echo "Error: Saga replica remove attribute could not remove logical file attribute"
   exit 1
fi

# SAGA CALL NUMBER 22
echo $SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE`
#echo "[$RESULT]"
RESULT2=`$SAGA_LOCATION/bin/saga-replica list_attributes $PATH1/$FILE | grep -v ctime | grep -v utime`
#echo "[$RESULT2]"
if test "$RESULT2" != ""
then
   echo "Error: Saga replica list_attributes is not empty after removing attribute to logical file"
   exit 1
fi

# SAGA CALL NUMBER 14
echo $SAGA_LOCATION/bin/saga-replica remove_lfn $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-replica remove_lfn $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga replica remove_lfn could not remove a logical file"
   exit 1
fi


# All tests succeeded! 
rm -f saga_replica.db
rm -f .saga.ini
echo "Success"
exit 0;
