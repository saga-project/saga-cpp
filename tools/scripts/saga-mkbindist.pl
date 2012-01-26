#!/usr/bin/perl -w

BEGIN {
  use strict;
  use Data::Dumper;

  sub parse_pkg_file (;$);
}

my $PACKAGE = "PACKAGE";
my $VERSION = "VERSION";

my $PWD = `pwd`;
chomp ($PWD);

my $SYSTEM = `uname -s`;
chomp ($SYSTEM);

my $CORES = 1;
if ( $SYSTEM eq 'Darwin' )
{
  $CORES = `/usr/sbin/system_profiler SPHardwareDataType | grep Cores | rev | cut -f 1 -d ' ' | rev`;
  chomp ($CORES);
}
elsif ( $SYSTEM eq 'Linux' )
{
  $CORES = `cat /proc/cpuinfo | grep processor | wc -l`;
  chomp ($CORES);
}

if ( $CORES !~ /^\d+$/io )
{
  $CORES = 1;
}

sub parse_pkg_file (;$)
{
  my $pwd   = shift || undef;
  my @lines = ();

  if ( ! -e "$PACKAGE" )
  {
    die "Could not find package description in $PACKAGE\n";
  }

  open (IN, "<$PACKAGE") || die "Can't open $PACKAGE: $!\n";
  @lines = <IN>;
  chomp (@lines);
  close (IN);

  my %pkg   = ();
  my $type  = "";
  my $lnum  = 0;

  foreach my $line ( @lines )
  {
    $lnum++;

    # if no type is set, find a new type
    if ( $type eq "" )
    {
      if ( $line =~ /^([_A-Z]+?):\s*$/o )
      {
        $type = $1;
      }
      elsif ( $line =~ /^([_A-Z]+?):\s*(\S.*?)\s*$/o )
      {
        $pkg{$1} = $2;
      }
      elsif ( $line =~ /^\s*$/o )
      {
        # ignore empty line
      }
      else
      {
        die "line $lnum: expected new type, but found '$line'\n";
      }
    }

    else
    {
      if ( $line =~ /^\s+(\S+?)\s*$/o )
      {
        push (@{$pkg{$type}}, $1);
      }
      elsif ( $line =~ /^\s*$/o )
      {
        $type = "";
      }
      else
      {
        die "line $lnum: expected $type entry, but found '$line'\n";
      }
    }
  }

  return \%pkg;
}

# main
{
  # parse package description
  my $pkg = parse_pkg_file ();

  if ( ! exists ($pkg->{'PACKAGE'}) )
  {
    die "No package name defined\n";
  }


  if ( ! exists ($pkg->{'VERSION'}) )
  {
    if ( ! -e "$VERSION" )
    {
      die "Could not find package version in $PACKAGE nor $VERSION\n";
    }

    my $version = `cat $VERSION`;
    chomp ($version);

    $pkg->{'VERSION'} = $version;
  }


  my $srctgt = "$pkg->{PACKAGE}-$pkg->{VERSION}";
  my $bintgt = "$pkg->{PACKAGE}-bin-$pkg->{VERSION}";

  `rm -rf $bintgt`;
  `rm -f $bintgt.tgz`;

  mkdir ($bintgt) || die "Cannot create $bintgt: $!\n";

  system ("tar zxvf $srctgt.tgz") == 0 
    or die "Cannot untar srcdist tarball\n";

  system ("cd $srctgt && ./configure --prefix=/usr") == 0 
    or die "Cannot configure in bindist";

  system ("cd $srctgt && make -j $CORES") == 0 
    or die "make failed for bindist\n";

  system ("cd $srctgt && make DESTDIR=$PWD/$bintgt install") == 0 
    or die "Cannot install bindist\n";

  system ("cd $bintgt && tar zcvf ../$bintgt.tgz *") == 0 
    or die "Cannot create tarball for bindist\n";

  system ("rm -rf $srctgt") == 0 
    or warn "Cannot remove bindist src\n";

  system ("rm -rf $bintgt") == 0 
    or warn "Cannot bindist target\n";

  print "\ncreated $bintgt.tgz\n\n";
}

