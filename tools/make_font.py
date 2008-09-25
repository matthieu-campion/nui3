#!/usr/bin/python
#
#  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
#  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
#
#  licence: see nui3/LICENCE.TXT
#

import sys

if (len(sys.argv) < 3):
  print "Usage: %s <binary font file> <dest font name>" % sys.argv[0]
  sys.exit()

sourcefile = file(sys.argv[1])
destfilename = sys.argv[2]

datafile = file(sys.argv[1])
data = datafile.read()
datafile.close()
datastr = ''.join(map(lambda x: '\\x%02x' % ord(x), data))
datastr =  '"\n"'.join([datastr[n:n+64] for n in range(0,len(datastr),64)])

size = len(data)

hfile = file(destfilename + '.h', 'w')
cppfile = file(destfilename + '.cpp', 'w')

defname = destfilename.replace(' ', '_')
defname = defname.rpartition('/')[2]

hfile.write("""/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#pragma once

extern const char* gp%s;
const long         g%sSize = %d;

""" % (defname, defname, size))

cppfile.write("""/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

NGL_API const char* gp%s = \"%s\";""" % (defname, datastr))

hfile.close()
cppfile.close()