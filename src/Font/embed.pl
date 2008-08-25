#!/usr/bin/perl

# Copyright (c) 2002-2003 NGL Team
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

my $out_base = "out";
my $sym_base = "";

my $legal_blurb = <<EOF;
/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
EOF


sub get_stamp
{
#  my $user = $ENV{'USER'};
#  my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday) = gmtime;
#  my $date = sprintf "%s/%02s/%02s at %02s:%02s GMT", 1900+$year, $mon, $mday, $hour, $min;

  return "/* This file was generated with embed.pl, please do not edit. */\n";
}

sub help
{
  printf <<EOF;
Usage: $0 [options] [input file]

  Turn any file into a C compilable data representation.
  If `input file' is not specified, stdin is used and -s is mandatory.
  Otherwise the symbol base name is mangled from the input file name.

  -o             base name of output files
  -s             base name of symbol
  -h, --help     help
EOF
}


# Parse arguments
#
while ((@ARGV > 0) && ($ARGV[0] =~ '^-'))
{
  my $opt = $ARGV[0];

  if ($opt eq '-h' || $opt eq '--help')
  {
    help();
    exit 0;
  }
  elsif ($opt eq '-o')
  {
    shift;
    $out_base = $ARGV[0];
  }
  elsif ($opt eq '-s')
  {
    shift;
    $sym_base = $ARGV[0];
  }
  else
  {
    printf STDERR "Error: unknown option '$opt'\n";
    exit 1;
  }
  shift;
}

if (@ARGV > 1)
{
  printf STDERR "Error: too much arguments\n";
  exit 0;
}


# Prepare input
#
if (@ARGV eq 1)
{
  my $fname = $ARGV[0];

  open STDIN, "<$fname" || die "Error: could not open $fname";
  if ($sym_base eq "")
  {
    $sym_base = $fname;
    $sym_base =~ s/[\.\/]/_/g;
  }
}
else
{
  if ($sym_base eq "")
  {
    printf STDERR "Error: specify symbol base name\n";
    exit 1;
  }
}

my $stamp = get_stamp();


# Output .cpp
#
open DOT_CPP, ">$out_base.cpp";

printf DOT_CPP <<EOF;
$legal_blurb
$stamp
#include "ngl.h"

EOF
printf DOT_CPP "NGL_API const char* gp${sym_base}Base =\n  \"";

my $data;
my $size = 0;

while (read STDIN, $data, 1)
{
  if ($size > 0 && $size % 16 == 0)
  {
    printf DOT_CPP "\"\n  \"";
  }
  printf DOT_CPP "\\x%.2x", ord $data;
  $size++;
}

printf DOT_CPP "\";\n";
close DOT_CPP;


# Output .h
#
open DOT_H, ">$out_base.h";

printf DOT_H <<EOF;
$legal_blurb
$stamp
#ifndef __${out_base}_h__
#define __${out_base}_h__

#include "ngl.h"


extern const char* gp${sym_base}Base;
const long         g${sym_base}Size = $size;

#endif // __${out_base}_h__
EOF

close DOT_H;

