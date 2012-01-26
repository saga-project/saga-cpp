#!/usr/bin/perl -w

BEGIN {
  use strict;

  sub help ();
}

$| = 1; # always flush stdout

sub help ()
{
  print <<EOT;

  This is a simple test helper application allowing to test the local SAGA
  job adaptor. 

  Possible command line options:
  Usage: test_helper [options]:
    -h       print out program usage (this message)
    -i       run in interactive mode
    -o arg   initial output stream: stdout or stderr
    -e err   exit with error code 'err'

  In interactive mode this application understands the following commands 
  from std::cin (stdin):

    <stdout>
        from now on all output will be printed to std::cout (stdout)
    <stderr>
        from now on all output will be printed to std::cerr (stderr)
    <exit> or <exit:code>
        exit immediatly with the given exitcode (defaults to '-e err' or zero)
    <delayedexit(delay)> or <delayedexit(delay):code>
        exit after 'delay' seconds with the given exitcode (defaults to 
        '-e err' or zero)

    All other input will be echoed to the current output.

  In non-interactive mode the application just prints its command line 
  arguments to the requested output (see option -o).

EOT
}

# main
{
  my $read_stdin   = 0;
  my $print_stdout = 0;
  my $print_stderr = 0;
  my $retval       = 0;
  my $print        = "";

  my $arg = shift;

  while ( defined ($arg) )
  {
    if    ( $arg eq '-h' ) { help (); exit ( 0);  }
    elsif ( $arg eq '-i' ) { $read_stdin = 1;     }
    elsif ( $arg eq '-o' ) { $print      = shift; }
    elsif ( $arg eq '-e' ) { $retval     = shift; }
    else                   { help (); exit (-1);  }

    $arg = shift;
  }

  $print_stdout = 1  if ( $print eq "stdout" );
  $print_stderr = 1  if ( $print eq "stderr" );

  if ( $read_stdin )
  {
    INPUT:
    while ( <STDIN> )
    {
      my $line = $_;

      print STDOUT "$line"  if ( $print_stdout );
      print STDERR "$line"  if ( $print_stderr );

      if ( $line =~ /^<stdout>$/io )
      {
        $print_stdout = 1;
        $print_stderr = 0;
      }
      elsif ( $line =~ /^<stderr>$/io )
      {
        $print_stdout = 0;
        $print_stderr = 1;
      }
      elsif ( $line =~ /^<exit(?::(\d+))?>$/io )
      {
        $retval = $2 || $retval;
        last INPUT;
      }
      elsif ( $line =~ /^<delayedexit\((\d+)\)(?::(\d+))?>$/io )
      {
        $retval = $2 || $retval;
        sleep ($1);
        last INPUT;
      }
    }
  }

  exit $retval;
}

