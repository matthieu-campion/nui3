#!/usr/bin/python
#
#  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
#  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
#
#  licence: see nui3/LICENCE.TXT
#

import sys

if (len(sys.argv) < 2):
  print "Usage: %s <binary font file> [<dest source file>]" % sys.argv[0]
  sys.exit()

sourcefile = file(sys.argv[1])

if (len(sys.argv) < 3):
  i = sys.argv[1].rfind('.')
  if i == -1:
    destfilename = sys.argv[1]
  else:
    destfilename = sys.argv[1][:i]
else:
  destfilename = sys.argv[2]

datafile = file(sys.argv[1])
data = datafile.read()
datafile.close()
datastr = ''.join(map(lambda x: '\\x%02x' % ord(x), data))
datastr =  '"\n"'.join([datastr[n:n+64] for n in range(0,len(datastr),64)])

size = len(data)

hfile = file(destfilename + '.h', 'w')
cppfile = file(destfilename + '.cpp', 'w')

i = sys.argv[1].rfind('/')
if i == -1:
  defname = destfilename.replace(' ', '_')
else:
  defname = destfilename[i+1:].replace(' ', '_')

hfile.write("""/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#ifndef __%s_h__
#define __%s_h__

extern const char* gpFontBase;
const long         gFontSize = %d;

#endif // __%s_h__""" % (defname, defname, size, defname))

cppfile.write("""/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

NGL_API const char* gpDefaultFontBase = \"%s\";""" % datastr)

hfile.close()
cppfile.close()