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

# This script generates :
#
#  - ngl_glext.h, which holds the extension functions pointers prototypes
#  - ngl_glext_table.cpp, which holds function names and categorisation
#
# Input is <GL/glext.h> from the OpenGL extension registry :
#   http://oss.sgi.com/projects/ogl-sample/registry/

my $dot_h   = "ngl_glext.h";
my $dot_cpp = "ngl_glext_table.cpp";

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

my $dot_h_header = <<EOF;
/* This file is included and \\b must only be included by nglContext.
 * These prototypes become part of nglContext's own methods.
 */
EOF


sub get_stamp {
#  my $user = $ENV{'USER'};
#  my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday) = gmtime;
#  my $date = sprintf "%s/%02s/%02s at %02s:%02s GMT", 1900+$year, $mon, $mday, $hour, $min;

  return "/* This file was generated with glext_gen.pl, please do not edit.  */\n";
}

sub get_registry_url {
  my $ext = shift;
  my $path;
  my $specbase = "http://www.opengl.org/developers/documentation";

  if    ($ext eq 'GL_VERSION_1_2') { return "$specbase/Version1.2/OpenGL_spec_1.2.1.pdf"; }
  elsif ($ext eq 'GL_VERSION_1_3') { return "$specbase/version1_3/glspec13.pdf"; }
  elsif ($ext eq 'GL_VERSION_1_4') { return "$specbase/version1_4/glspec14.pdf"; }

  (undef, $vendor, @extname) = split /_/, $ext;
  my $extname = join '_', @extname;

  return "http://oss.sgi.com/projects/ogl-sample/registry/$vendor/$extname.txt";
}

sub help {
  print <<EOF;
Usage: $0 [options] [input file]

  Generate NGL's context skeleton for GL extensions from `glext.h'.
  If `input file' is not specified, stdin is used.

  -h, --help     help
EOF
}


# Parse arguments
#
while ((@ARGV > 0) && ($ARGV[0] =~ '^-')) {
  my $opt = $ARGV[0];

  if ($opt eq '-h' || $opt eq '--help') {
    help();
    exit 0;
  } else {
    print STDERR "Error: unknown option '$opt'\n";
    exit 1;
  }
  shift @ARGV;
}

if (@ARGV > 1) {
  print STDERR "Error: too much arguments\n";
  exit 0;
}


# Prepare input
#
open(STDIN, "<$ARGV[0]") if (@ARGV eq 1);


# Prepare outputs
#
open DOT_H, ">$dot_h";
open DOT_CPP, ">$dot_cpp";

my $stamp = get_stamp();
print DOT_H "$legal_blurb\n$dot_h_header\n$stamp\n";
print DOT_CPP "$legal_blurb\n$stamp\n";


# Parse and generate
#
# The DOT_H file is written on the fly (as protos are parsed),
# the DOT_CPP is built later from collected data.

my $ext_cnt = 0;
my $func_cnt = 0;
my $func_start;
my $ext_name;
my $ext_parsing = 0;
my $ext_name_table = '';
my $ext_func_table = '';
my %ext_list;

while (<>) {
  if (/^#define /) {
    $ext_name = (split)[1];
  }
  elsif (/^#ifdef GL_GLEXT_PROTOTYPES/) {
    # This blurb marks the start of a proto list, the extension name was
    # caught a few lines above when the previous #define <ext_name> occured

    # Check if we already parsed this extension
    next if defined $ext_list{$ext_name};

    $ext_list{$ext_name} = 1;
    $ext_parsing = 1;
    $func_start = $func_cnt;

    my $url = get_registry_url($ext_name);
    print DOT_H "/** \@name $ext_name\n";
    print DOT_H " *  See $url\n";
    print DOT_H " */\n//\@{\n";

    $ext_func_table .= "\n/* $ext_name\n */\n";

    $ext_cnt++;
  }
  elsif (/^#endif/) {
    # If we were parsing a proto list, this is the end of the list
    #
    if ($ext_parsing eq 1) {
      print DOT_H "//@}\n\n";
      $ext_name_table .= sprintf "  { %-33s %-3d },\n", "\"$ext_name\",", $func_start;
      $ext_parsing = 0;
    }
  }
  elsif ($ext_parsing eq 1) {
    # We are currently parsing a proto list, $_ is one of them
    #
    my (undef, $ret_type, undef, $name, @args) = split;
    my $args_str = join ' ', @args;

    printf DOT_H "%-9s (APIENTRY *$name) $args_str\n", $ret_type;
    $ext_func_table .= sprintf "/* %-3d */ { \"$name\", (GLExtFuncPtr)&nglContext::$name },\n", $func_cnt;

    $func_cnt++;
  }
}

# Output dot_cpp tables
#
print DOT_CPP <<EOF;
#include "ngl.h"
#include "ngl_glext_table.h"


GLExtNameInfo gpGLExtensionNameTable[] =
{
$ext_name_table
  // End of list
  { NULL, $func_cnt }
};


GLExtFuncInfo gpGLExtensionFuncTable[] =
{$ext_func_table
  // End of list
  { NULL, (GLExtFuncPtr)NULL }
};
EOF


# Finalize both files
#
my $stats = "$ext_cnt extensions, $func_cnt total functions";
my $instats = "\n\n/* $stats */\n";

print DOT_H $instats;
print DOT_CPP $instats;

close DOT_H;
close DOT_CPP;


# Display stats on stdoud
#
print "$stats\n";
