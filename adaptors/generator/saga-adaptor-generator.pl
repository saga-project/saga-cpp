#!/usr/bin/perl -w

BEGIN 
{
  use strict;
  use Getopt::Long;
  use IO::Dir;
  use File::Copy;

  sub get_yesno (  );
  sub help      (;$$);

  $| = 1;  # flush stdout
}


#####################################################################
#
# defaults
#
my $GEN_ROOT = "$ENV{SAGA_LOCATION}/share/saga/adaptor_generator/skeleton/";
my $TARGET   = `pwd`;
my $TYPE     = "";
my $NAME     = "";
my $SUITE    = "";
my $HELP     = 0;

chomp ($TARGET);
$TARGET .= "/target";


#####################################################################
#
# get/check parameters
#
GetOptions ("dir=s"    => \$TARGET,
            "type=s"   => \$TYPE,
            "name=s"   => \$NAME,
            "suite=s"  => \$SUITE,
            "help"     => \$HELP)
   || exit;


if ( $HELP )
{
  help ();
}

if ( ! -d $GEN_ROOT && exists $ENV{SAGA_ROOT} )
{ 
  $GEN_ROOT = "$ENV{SAGA_ROOT}/adaptors/generator/skeleton/";
}

if ( ! -d $GEN_ROOT )
{
  help ("Cannot find generator support files in $GEN_ROOT.");
}

if ( $TYPE !~ /^(context|advert|file|job|replica|rpc|stream)$/io )
{
  help ("Unknown type $TYPE\n" .
        "Known types are: context, advert, file, job, replica, rpc, stream");
}


if ( $SUITE eq "" )
{
  help ("No suite specified.\n" .
        "Suite should be some kind of adaptor class,\n" .
        "e.g. 'globus', 'unicore', 'posix' etc.");
}

print "\n";


$TYPE      = uc ($TYPE );
$NAME      = uc ($NAME );
$SUITE     = uc ($SUITE);

my $type   = lc ($TYPE );
my $name   = lc ($NAME );
my $suite  = lc ($SUITE);

my $Type   = uc ((split ('', $type ))[0, ]) . lc (join ('', ((split ('', $type ))[ 1 .. (length($type )-1)])));
my $Name   = uc ((split ('', $name ))[0, ]) . lc (join ('', ((split ('', $name ))[ 1 .. (length($name )-1)])));
my $Suite  = uc ((split ('', $suite))[0, ]) . lc (join ('', ((split ('', $suite))[ 1 .. (length($suite)-1)])));

if ( $name =~ /[^0-9a-z_]/io )
{
  die "'name' can contain only alphanumerics and underscore ($name)\n\n";
}

if ( $type =~ /[^0-9a-z_]/io )
{
  die "'type' can contain only alphanumerics and underscore ($type)\n\n";
}

if ( $suite =~ /[^0-9a-z_]/io )
{
  die "'suite' can contain only alphanumerics and underscore ($suite)\n\n";
}



# name is attached to type now, if it was specified, -> f(ull)type.
my $ftype  = "$type";
my $Ftype  = "$Type";
my $FTYPE  = "$TYPE";

if ( $name ne "" )
{
  $ftype = "$name" . "_$type";
  $Ftype = "$Name" . "_$Type";
  $FTYPE = "$NAME" . "_$TYPE";
}



#####################################################################
#
# determine what to do, and start to create new adaptor tree
#

my $tgt = "$TARGET/$suite/$suite\_$ftype";

print <<EOT;

  suite:             $suite
  type:              $type
  ftype:             $ftype
  name:              $name
  directory:         $tgt

EOT

if ( ! -d $TARGET )
{
  system ("mkdir -p $TARGET") == 0 or
  die "Cannot create target dir $TARGET: $!\n";
}


if ( -d $tgt )
{
  warn "Target directory $tgt exists.  Replace?\n";

  unless ( get_yesno () )
  {
    die "abort\n";
  }
  print "\n";

  system ("rm -rf $tgt") == 0
    or die "Cannot clean old target directory: $!\n";
}

system ("mkdir -p $tgt") == 0 
  or die "Cannot create target directory: $!\n";

print "  copying files:     ...\n";
system ("cp $GEN_ROOT/$type/*                            $tgt");
system ("cp $GEN_ROOT/###suite###_###type###_adaptor.ini $tgt/saga_adaptor_${suite}_$ftype.ini");
system ("cp $GEN_ROOT/CHANGES.in                         $tgt/CHANGES");
system ("cp $GEN_ROOT/README.in                          $tgt/README");
system ("cp $GEN_ROOT/LICENSE.in                         $tgt/LICENSE");
system ("cp $GEN_ROOT/PACKAGE.in                         $tgt/PACKAGE");


#####################################################################
#
# for the new tree: rename all files with templated name
#
my $d = IO::Dir->new ($tgt);

unless ( defined $d ) 
{
  die "Creation of adaptor failed: copy failed\n";
}

my @files = $d->read ();

$d->close ();

# ignore './' and '../'
@files = grep (!/^\.+$/, @files);

my @cleanfiles = ();

print "  fixing file names: .";
foreach my $file ( @files )
{
  my $orig = $file;

  $file =~ s/###SUITE###/$SUITE/g;
  $file =~ s/###TYPE###/$FTYPE/g;
  $file =~ s/###NAME###/$NAME/g;

  $file =~ s/###suite###/$suite/g;
  $file =~ s/###type###/$ftype/g;
  $file =~ s/###name###/$name/g;

  $file =~ s/###Suite###/$Suite/g;
  $file =~ s/###Type###/$Ftype/g;
  $file =~ s/###Name###/$Name/g;

  if ( $orig ne $file )
  {
    print ".";
    move ("$tgt/$orig", "$tgt/$file");
  }

  push (@cleanfiles, $file);
}
print "\n";

my $cleanfiles = join ("\n  ", grep (/.*\.[hc]pp$/, @cleanfiles));

#####################################################################
#
# for the new tree: search and replace templatized file contents
#
print "  fixing files:      .";
foreach my $file ( @cleanfiles )
{
  # print "fixing $file\n";

  my $IN  = "$tgt/$file";
  my $OUT = "$tgt/$file.tmp";

  $in  = new IO::File ("< $IN")  || die "Cannot open $IN:  $!\n";
  $out = new IO::File ("> $OUT") || die "Cannot open $OUT: $!\n";;

  while ( <$in> )
  {
    my $line  = $_;
    my $debug = 0;

    $line =~ s/###SUITE###/$SUITE/g;
    $line =~ s/###TYPE###/$FTYPE/g;
    $line =~ s/###NAME###/$NAME/g;

    $line =~ s/###Suite###/$Suite/g;
    $line =~ s/###Type###/$Ftype/g;
    $line =~ s/###Name###/$Name/g;

    $line =~ s/###suite###/$suite/g;
    $line =~ s/###type###/$ftype/g;
    $line =~ s/###name###/$name/g;


    $line =~ s/###packagefiles###/$cleanfiles/g;

    $out->print ($line);
  }

  move ($OUT, $IN);
  print ".";
}
print "\n";

#####################################################################
#
# done
#
print <<EOT;

  You can now cd to $tgt, 
  and run 'make; make install'.  

  Note that you need to set SAGA_LOCATION before,
  and point it to your SAGA installation tree.

EOT


#####################################################################
#
# TODO: test compile of new adaptor
#


#####################################################################
#
# sub: get interactive user feedback
#
sub get_yesno ()
{
  my $answer = "n";

  print "[y/N] ";

  while ( 1 )
  {
    $answer = getc (STDIN);

    if ( $answer =~ /[yn\n]/i )
    {
      last;
    }
  }

  if ( $answer eq 'y' )
  {
    return 1;
  }

  return 0;
}

#####################################################################
#
# sub: print help message and exit
#
sub help (;$$)
{
  my $msg = shift || "";
  my $ret = shift || 0;

  if ( $msg ne "" )
  {
    $msg =~ s/\n/\n    /og;

    print <<EOT;
    
  Error:
  ------

    $msg
EOT

  }

  print <<EOT;


  Usage:
  ------
  
    $0 [options]

    This script creates stubs for new SAGA adaptors, along with 
    Makefiles and configuration files.  All methods in the resulting 
    adaptors throw a 'NotImplemented' exception, so the adaptors
    are useless.  They can serve, however, as a convenient starting 
    point for new adaptors.


  Options: 
  --------

    -d|--dir=<path>
                target directory for created adaptor.  The adaptor 
                will actualy reside in
                'path/suite/suite_type[_name]/'
                Defaults to \$PWD.

    -s|--suite=<suite>
                The adaptor suite groups adaptors for the same
                underlying technology.  For example, a Globus Web
                Service adaptor should be in the 'globus-ws'
                adaptor suite.  Note that 'suite + type [+ name]'
                form the adaptor name, so should be unique.
                No default.
                
    -t|--type=<type>
                type of adaptor to create.  Allowed values are the 
                names of the supported SAGA packages, which are, 
                at the moment: 'context', 'advert', 'file', 'job', 
                'replica', 'rpc' and 'stream'.
                No default.

    -n|--name=<name>
                name of adaptor to create.  This is an optional 
                short string to further qualify the adaptor, e.g. 
                to distinguish between multiple file adaptors for 
                the same adaptor suite.  
                Defaults to empty string.

    -h|--help   prints this help message


  Environment:
  ------------

    SAGA_LOCATION 
               must point to a valid SAGA installation.  This script
               needs that information to pick up the adaptor skeleton
               files.

  Bugs:
  -----

    At some point in the future, this script will also try to compile
    and use the created adaptor - that is not implemented, yet.

EOT

  exit ($ret);
}

