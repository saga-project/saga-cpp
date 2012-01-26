#!/bin/sh
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Header
while getopts h arg
do
   case $arg in
      h) echo "This program performs some rudimentary ADVERT adaptor tests."
         exit 0;;
   esac
done

if test "x$REMOTEHOST" = "x"; then
  REMOTEHOST=localhost
  
echo '[saga.adaptors.default_advert]
  name      = default_advert
#  path      = $[saga.location]/lib

[saga.adaptors.default_advert.preferences]
  # adaptor configuration

  # specify what backend to use
  dbtype    = sqlite3


[saga.adaptors.default_advert.preferences.sqlite3]
  # specify a file name where sqlite3 can store its data.
  # relative and absolute paths are both supported.  Relative paths are 
  # relative to the applications cwd.

  dbconnect = ${HOME}/saga_advert.db' > .saga.ini
fi



# NOTE:

# Remove old content that may have been created by previous tests,
# which may not have succeeded
rm -f saga_advert.db



# Define pathnames and filenames

# Two paths
PATH1=advert://localhost/adaptortests/
PATH2=advert://localhost/adaptortests/saga.adaptor.advert.directory

# One file
FILE=saga.adaptor.advert.test

# Two physical file names
PFN1=me
PFN2=metoo

# SAGA CALL NUMBER -1
echo $SAGA_LOCATION/bin/saga-advert remove_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-advert remove_directory $PATH1`


# SAGA CALL NUMBER 0
echo $SAGA_LOCATION/bin/saga-advert add_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-advert add_directory $PATH1`


# SAGA CALL NUMBER 1
echo $SAGA_LOCATION/bin/saga-advert list_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-advert list_directory $PATH1`
if test "$RESULT" != ""
then
   echo "Error: Saga advert list_directory should be empty"
   exit 1
fi

# SAGA CALL NUMBER 2
echo $SAGA_LOCATION/bin/saga-advert add_entry $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert add_entry $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga advert add_lfn could not add an entry"
   exit 1
fi

# SAGA CALL NUMBER 3
echo $SAGA_LOCATION/bin/saga-advert list_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-advert list_directory $PATH1`
if test "$RESULT" != "  $FILE"
then
   echo "Error: Saga advert list_directory does not show a recently added entry"
   exit 1
fi


# SAGA CALL NUMBER 7
echo $SAGA_LOCATION/bin/saga-advert add_directory $PATH2
RESULT=`$SAGA_LOCATION/bin/saga-advert add_directory $PATH2`
if test "$RESULT" != ""
then
   echo "Error: Saga advert add_directory could not add a  directory"
   exit 1
fi

# SAGA CALL NUMBER 8
echo $SAGA_LOCATION/bin/saga-advert add_entry $PATH2/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert add_entry $PATH2/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga advert add_lfn could not add an entry"
   exit 1
fi

# SAGA CALL NUMBER 14
echo $SAGA_LOCATION/bin/saga-advert remove_entry $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert remove_entry $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga advert remove_lfn could not remove an entry"
   exit 1
fi

# SAGA CALL NUMBER 15
echo $SAGA_LOCATION/bin/saga-advert remove_entry $PATH2/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert remove_entry $PATH2/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga advert remove_lfn could not remove an entry"
   exit 1
fi

# SAGA CALL NUMBER 16
echo $SAGA_LOCATION/bin/saga-advert remove_directory $PATH2
RESULT=`$SAGA_LOCATION/bin/saga-advert remove_directory $PATH2`
if test "$RESULT" != ""
then
   echo "Error: Saga advert remove_lfn could not remove a  directory"
   exit 1
fi

# SAGA CALL NUMBER 17
echo $SAGA_LOCATION/bin/saga-advert list_directory $PATH1
RESULT=`$SAGA_LOCATION/bin/saga-advert list_directory $PATH1`
if test "$RESULT" != ""
then
   echo "Error: Saga advert list_directory is not empty after removing logical file"
   exit 1
fi

# ## Add directory, remove directory??

### list_attributes, add_attribute, remove_attribute
# SAGA CALL NUMBER 2
echo $SAGA_LOCATION/bin/saga-advert add_entry $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert add_entry $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga advert add_lfn could not add an entry"
   exit 1
fi

# SAGA CALL NUMBER 18
echo $SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE  | grep -v ctime | grep -v utime`
#echo "[$RESULT]"
if test "$RESULT" != ""
then
   echo "Error: Saga advert list_attributes is not empty in the atribute-less logical file"
   exit 1
fi

# SAGA CALL NUMBER 19
echo $SAGA_LOCATION/bin/saga-advert set_attribute $PATH1/$FILE Interactive True
RESULT=`$SAGA_LOCATION/bin/saga-advert set_attribute $PATH1/$FILE Interactive True`
RESULT2=`$SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE | grep -v ctime | grep -v utime`
#echo "[$RESULT]"
#echo "[$RESULT2]"
if test "$RESULT" != ""
then
   echo "Error: Saga advert set_attribute could not add an attribute to logical file"
   exit 1
fi

# SAGA CALL NUMBER 20
echo $SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE`
#echo "[$RESULT]"
if test "$RESULT" = ""
then
   echo "Error: Saga advert list_attributes is empty after adding attribute to logical file"
   exit 1
fi

# SAGA CALL NUMBER 21
echo $SAGA_LOCATION/bin/saga-advert remove_attribute $PATH1/$FILE Interactive 
RESULT=`$SAGA_LOCATION/bin/saga-advert remove_attribute $PATH1/$FILE Interactive | grep -v ctime | grep -v utime`
if test "$RESULT" != ""
then
   echo "Error: Saga advert remove attribute could not remove logical file attribute"
   exit 1
fi

# SAGA CALL NUMBER 22
echo $SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE`
#echo "[$RESULT]"
RESULT2=`$SAGA_LOCATION/bin/saga-advert list_attributes $PATH1/$FILE | grep -v ctime | grep -v utime`
#echo "[$RESULT2]"
if test "$RESULT2" != ""
then
   echo "Error: Saga advert list_attributes is not empty after removing attribute to logical file"
   exit 1
fi

# SAGA CALL NUMBER 14
echo $SAGA_LOCATION/bin/saga-advert remove_entry $PATH1/$FILE
RESULT=`$SAGA_LOCATION/bin/saga-advert remove_entry $PATH1/$FILE`
if test "$RESULT" != ""
then
   echo "Error: Saga advert remove_lfn could not remove an entry"
   exit 1
fi


# All tests succeeded! 
echo "Success"
exit 0;
