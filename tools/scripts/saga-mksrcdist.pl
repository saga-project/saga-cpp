#!/usr/bin/perl -w

BEGIN {
  use strict;
  use Data::Dumper;

  sub parse_pkg_file (;$);
}

my $PACKAGE = "PACKAGE";
my $VERSION = "VERSION";
my @FILES   = ();
my $DEFAULT = <<EOT;
  DIRS:
    */
EOT

my $PWD = `pwd`;
chomp ($PWD);

my $SYSTEM = `uname -s`;
chomp ($SYSTEM);

my $MAX = "-maxdepth 1";
if ( $SYSTEM eq "Darwin" )
{
  $MAX = "-depth 1";
}


sub parse_pkg_file (;$)
{
  my $pwd   = shift || undef;
  my @lines = ();

  if ( ! -e "$PACKAGE" )
  {
    if ( ! defined ($pwd) )
    {
      die "Could not find package description in $PACKAGE\n";
    }

    print " -> $pwd\n";
    @dirs  = `find . $MAX -type d -name '[^.]*'`;
    @files = `find . $MAX -type f -name '[^.]*'`;

    chomp (@dirs);
    chomp (@files);

    push (@lines, "");
    push (@lines, "DIRS:");
    foreach my $dir ( @dirs )
    {
      $dir =~ s|^./||iog;
      $dir =~ s|/+$||iog;
      push (@lines, "  $dir/");
    }

    push (@lines, "");
    push (@lines, "FILES:");
    foreach my $file ( @files )
    {
      $file =~ s|^./||iog;
      push (@lines, "  $file");
    }

    push (@lines, "");
  }
  else
  {
    if ( ! defined ($pwd) )
    {
      $pwd = ".";
    }

    print " -> $pwd/$PACKAGE\n";
    open (IN, "<$PACKAGE") || die "Can't open $PACKAGE: $!\n";
    @lines = <IN>;
    chomp (@lines);
    close (IN);
  }

  my %pkg   = ();
  my $type  = "";
  my $lnum  = 0;

LINE:
  foreach my $line ( @lines )
  {
    $lnum++;

    if ( $line =~ /^#/o )
    {
      next LINE;
    }

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
      if ( $line =~ /^\s+(\S+?(?:\s+\S.*?)?)\s*$/o )
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

  # store files
  foreach my $file ( @{$pkg{'FILES'}} )
  {
    push (@FILES, "$pwd/$file");
  }

  # recurse subdirs
  foreach my $dir ( @{$pkg{'DIRS'}} )
  {
    $dir =~ s/\/$//iog;

    my $old = `pwd`;
    chomp ($old);

    chdir ($dir) || die "Cannot descend into subdir $dir: $!\n";
    $pkg{'SUBDIR_PKG'}{$dir} = parse_pkg_file ("$pwd/$dir");
    chdir ($old);
  }

  return \%pkg;
}

# main
{
  # simple test for clean source tree
  if ( -e "config.status" )
  {
    die "run 'make distclean' before building the source distribution!\n";
  }

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

  # print Dumper (@FILES);

  # copy all files into a temporary target dir
  my $tgt = "$pkg->{PACKAGE}-$pkg->{VERSION}";

  `rm -rf $tgt`;
  `rm -f $tgt.tgz`;

  mkdir ($tgt) || die "Cannot create $tgt: $!\n";

FILE:
  foreach my $file ( @FILES )
  {
    my $src  = $file;
    my $dir  = $file;
    my $name = $file;

    if ( $src =~ /\.o$/o ||
         $src =~ /^lib.*\.(so|dylib|a)$/o )
    {
      next FILE;
    }

    $dir  =~ s/^(.+?)\/([^\/]+)$/$1/iog;
    $file =~ s/^(.+?)\/([^\/]+)$/$2/iog;

    `mkdir -p $tgt/$dir/`;
    `cp $src  $tgt/$dir/`;
    $| = 1;
    print ".";
  }
  print "\n";

  system ("tar zcvf $tgt.tgz $tgt");
  system ("rm -rf $tgt");

  print "\ncreated $tgt.tgz\n\n";
}

