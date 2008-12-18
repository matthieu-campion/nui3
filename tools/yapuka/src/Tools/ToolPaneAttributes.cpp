/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "ToolPaneAttributes.h"
#include "nuiAttributeEditor.h"
#include "nuiVBox.h"
#include "nuiHBox.h"

ToolPaneAttributes::ToolPaneAttributes(const nglString& rTitle, const std::list<nuiAttribBase>& rAttributes, nuiButton* pBtn)
	:ToolPane(rTitle)
{
  Init(rAttributes, pBtn);  
}


ToolPaneAttributes::ToolPaneAttributes(nuiLabelAttribute* rLabel, const std::list<nuiAttribBase>& rAttributes, nuiButton* pBtn)
	:ToolPane(rLabel)
{
  Init(rAttributes, pBtn);
}


void ToolPaneAttributes::Init(const std::list<nuiAttribBase>& rAttributes, nuiButton* pBtn)
{
  nuiVBox* pBox = new nuiVBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
	SetContents(pBox);
  
  if (pBtn)
    pBox->AddCell(pBtn);
  
  nuiScrollView* pScroll = new nuiScrollView(false, true);
  pBox->AddCell(pScroll);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
	mpVBox = new nuiVBox(0);
	mpVBox->SetExpand(nuiExpandShrinkAndGrow);
  pScroll->AddChild(mpVBox);
  
  AddAttributes(rAttributes);
}

ToolPaneAttributes::~ToolPaneAttributes()
{

}



void ToolPaneAttributes::AddAttributes(const std::list<nuiAttribBase>& rAttributes)
{
	InitAttributes();


	uint nbCells=0;
	
	std::list<nuiAttribBase>::const_iterator it;
	for (it = rAttributes.begin(); it != rAttributes.end(); ++it, nbCells++)
	{
		nuiAttribBase attbase = *it;
    
    //LBDEBUG
    NGL_OUT(_T("ToolPaneAttributes::AddAttributes : attribute '%ls'\n"), attbase.GetName().GetChars());
		
    nuiAttributeEditor* ed = attbase.GetEditor();
    if (ed)
    {
      nuiHBox* pEditorBox = new nuiHBox(0);
      mpVBox->AddCell(pEditorBox);
     
      nglString label = attbase.GetName();
      label.Append(_T(":"));
      pEditorBox->AddCell(new nuiLabel(label));
      pEditorBox->AddCell(ed);
      
      mpVBox->SetCellExpand(mpVBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
      mpVBox->SetCellSpacing(mpVBox->GetNbCells()-1, 5);
    }
	}
}

