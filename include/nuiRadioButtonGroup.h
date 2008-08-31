/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRadioButtonGroup_h__
#define __nuiRadioButtonGroup_h__

#include <set>

#include "nuiEvent.h"

class nuiRadioButton;
class nuiXMLNode;

// nuiRadioButtonGroup:
class NUI_API nuiRadioButtonGroup
{
public:
  nuiRadioButtonGroup();
  nuiRadioButtonGroup(nuiXMLNode* pNode);
  virtual ~nuiRadioButtonGroup();

  void AddRadioButton(nuiRadioButton *pRadioButton);
  void RemoveRadioButton(nuiRadioButton *pRadioButton);

protected:
  virtual bool OnEvent(const nuiEvent& rEvent);
protected:
  void Init();
  nuiEventSink<nuiRadioButtonGroup> mSink;
  std::set<nuiRadioButton*> mRadioButtons;
};

#endif // nuiRadioButtonGroup
