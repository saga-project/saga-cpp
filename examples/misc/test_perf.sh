#!/bin/sh

/bin/date

i=0
while test $i != 100 
do
  i=$[$i+1]
  /bin/echo -n "."

#               /bin/date >& /dev/null
  ssh localhost /bin/date >& /dev/null

done

echo

/bin/date

