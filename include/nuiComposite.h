/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiComposite_h__
#define __nuiComposite_h__

#include "nuiSimpleContainer.h"

class NUI_API nuiComposite: public nuiSimpleContainer
{
public:
  /** @name Life */
  //@{
  nuiComposite()
    : nuiSimpleContainer()
  {
  }
  
  bool Load(const nuiXMLNode* pNode) ///< Create an nuiObject from an XML description.
  {
    return nuiSimpleContainer::Load(pNode);
  }
  
  virtual ~nuiComposite()
  {
  }
  //@}


};

#endif // __nuiComposite_h__
