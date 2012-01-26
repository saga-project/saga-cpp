#!/bin/bash

function timer() {
  msg_=$1 ; shift
  func_=$*
  before_=`date '+%s'`
  
  $*
  
  after_=`date '+%s'`
  elapsed_=$(($after_ - $before_))

  printf " %3d s : %s\n" "$elapsed_" "$msg_"
}

function test_url() {
  url_="$1"
  id_="`id -nu`-`id -u`"
  pbase_="$url_/perf/"
  base_="$pbase_/$id_"
  bin_="$SAGA_LOCATION/bin/saga-advert"

  test -x $bin_ || ( echo "cannot run $bin_" ; exit )

  timer "add dir   0" $bin_ add_directory $pbase_
  timer "add dir   1" $bin_ add_directory $base_

  for (( d=0; d < 10 ; d++ )); 
  do
    timer "add dir   2 $d"   $bin_ add_directory $base_/dir_$d
    timer "add entry 2 $d 1" $bin_ add_entry     $base_/dir_$d/entry_1
    timer "add entry 2 $d 2" $bin_ add_entry     $base_/dir_$d/entry_2
  done

  for (( d=0; d < 10 ; d++ )); 
  do
    for (( k=0; k < 10 ; k++ )); 
    do
      timer "set attr  1 $d $k" $bin_ set_attribute  $base_/dir_$d/entry_1 key_$k val_$k
      timer "set attr  2 $d $k" $bin_ set_attribute  $base_/dir_$d/entry_2 key_$k val_$k
    done
  done

  timer "dump   dir" time $bin_ dump_directory   $base_
  timer "delete dir" time $bin_ remove_directory $base_
}

# test_url 'advert://advert.cct.lsu.edu:8080/'
# test_url ''
  test_url 'sqlfastadvert://advert.cct.lsu.edu:8080/'

