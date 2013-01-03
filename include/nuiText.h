/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiText_h__
#define __nuiText_h__

#include "nuiEvent.h"
#include "nuiTheme.h"
#include "nuiEditText.h"

/// This is a non editable text display widget based on nuiEditText.
class NUI_API nuiText : public nuiEditText
{
public:
  nuiText(const nglString& rText = nglString::Empty);
  virtual ~nuiText();


protected:
};

#endif // __nuiText_h__
