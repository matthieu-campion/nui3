#!/usr/bin/python

import os
import sys

invalidpaths = ['.svn', 'CVS', '.DS_Store']

if (len(sys.argv) < 3):
  print "Usage: %s <res path> <res file>" % sys.argv[0]
  sys.exit()

rootpath = sys.argv[1]
if rootpath[-1] == '/':
  rootpath = rootpath[:-1]

f = file(sys.argv[2], 'w')

for root, dirs, files in os.walk(rootpath):
  pathname = root[len(rootpath)+1:]
  for p in invalidpaths:
   if p in dirs:
    dirs.remove(p)
   if p in files:
    files.remove(p)
  for filename in files:
        if (len(pathname) == 0):
          prefix = ''
        else:
          prefix = pathname + '/'
        f.write('%s        NUI_RESOURCE    "%s"\n' % (prefix + filename, rootpath + '/' + prefix + filename))

f.close()
