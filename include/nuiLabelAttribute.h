/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

//#include "nui.h"
#include "nuiAttribute.h"
#include "nuiFont.h"
#include "nuiLabel.h"

/// This widget implements a text label, linked to an nuiAttribute<const nglString&>. Therefore, the label updates automatically each time the attribute is changing
class NUI_API nuiLabelAttribute : public nuiLabel
{
public:
  nuiLabelAttribute(const nuiAttrib<const nglString&>& rAttribute, nuiTheme::FontStyle FontStyle=nuiTheme::Default);
  nuiLabelAttribute(const nuiAttrib<const nglString&>& rAttribute, nuiFont* pFont, bool AlreadyAcquired = false);
  virtual ~nuiLabelAttribute();
	
protected:
	virtual void OnAttributeChanged(const nglString& rText);

private:
	nuiSlotsSink mSink;
};

