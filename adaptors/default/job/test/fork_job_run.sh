#!/bin/sh -x

# Copyright (c) 2009 Andre Merzky <andre@merzky.net>
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt 
# or copy at http://www.boost.org/LICENSE_1_0.txt)

LOCALHOST=localhost

STRING=saga_was_here
FILE=/tmp/saga.adaptor.fork.test.$$

# Run a remote echo test and put result in tmp file
$SAGA_LOCATION/bin/saga-job run fork://$LOCALHOST/ /bin/sh -c "echo $STRING > $FILE"

# Check what the remote temp file contains
RESULT=`$SAGA_LOCATION/bin/saga-job run fork://$LOCALHOST/ cat $FILE`

# Clean up temporaries
$SAGA_LOCATION/bin/saga-job run fork://$LOCALHOST/ rm -f $FILE

# Verify the cat exited what was expected
if test "$RESULT" = "$STRING"; then
   exit 0;
else
   exit 1
fi

