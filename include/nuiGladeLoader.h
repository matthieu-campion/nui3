/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiGladeLoader_h__
#define __nuiGladeLoader_h__

#include "nuiXML.h"

nuiXML* ImportGladeXML(nglChar* xmlfile); ///< This very useful method permits to load very basic glade description into nui. Just load the xml description and give it to this function to retreive the equivalent nui xml tree.

#endif // __nuiGladeLoader_h__
