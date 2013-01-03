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

if (os.path.exists(sys.argv[2])):
  os.remove(sys.argv[2])

if (len(sys.argv) == 4):
  if (os.path.exists(sys.argv[3])):
    os.remove(sys.argv[3])

rcFile = file(sys.argv[2], 'w')
androidResources = []

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
        rcFile.write('%s        NUI_RESOURCE    "%s"\n' % (prefix + filename, rootpath + '/' + prefix + filename))
        androidResources.append('%s\n' % (prefix + filename))

rcFile.close()

if (len(sys.argv) == 4):
  androidFile = file(sys.argv[3], 'w')
  for r in androidResources:
    androidFile.write(r)
  androidFile.close()
