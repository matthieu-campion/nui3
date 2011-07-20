/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTitledPane_h__
#define __nuiTitledPane_h__

//#include "nui.h"
#include "nuiContainer.h"
#include "nuiVBox.h"
#include "nuiLabelAttribute.h"

/// This widget implements a simple box with a cell for the title and a cell for the contents
class NUI_API nuiTitledPane : public nuiVBox
{
public:
  nuiTitledPane(nuiWidget* pTitleWidget = NULL);
  nuiTitledPane(const nglString& rText);
  nuiTitledPane(nuiLabelAttribute* pLabel);
  
  // overloaded to redirect the use. Please use SetContents instead
  virtual bool AddChild(nuiWidgetPtr pChild);
	
	virtual ~nuiTitledPane();

	nuiWidget* GetTitle();
	void SetTitle(const nglString& rTitle, nuiPosition position=nuiFill);
	void SetTitle(nuiWidget* pTitleWidget, nuiPosition position=nuiFill);

  nuiWidget* GetContents();
  virtual void SetContents(nuiWidget* pContentsWidget, nuiPosition position=nuiFill);
  
};

#endif // __nuiTitledPane_h__
