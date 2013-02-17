/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiLabelAttribute::nuiLabelAttribute(const nuiAttrib<const nglString&>& rAttribute, nuiTheme::FontStyle FontStyle)
	: nuiLabel(rAttribute.Get(), FontStyle)
{
	SetObjectClass(_T("nuiLabelAttribute"));

	mSink.Connect(rAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiLabelAttribute::OnAttributeChanged));
}


nuiLabelAttribute::nuiLabelAttribute(const nuiAttrib<const nglString&>& rAttribute, nuiFont* pFont, bool AlreadyAcquired)
	: nuiLabel(rAttribute.Get(), pFont, AlreadyAcquired)
{
	mSink.Connect(rAttribute.GetChangedSignal(), nuiMakeDelegate(this, &nuiLabelAttribute::OnAttributeChanged));
}


nuiLabelAttribute::~nuiLabelAttribute()
{

}


void nuiLabelAttribute::OnAttributeChanged(const nglString& rText)
{
	SetText(rText);
}

