/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "Gui/Navigator.h"
#include "Sql/Sql.h"
#include "Application.h"
#include "MainWindow.h"
#include "Gui/Gui.h"

#include "nuiHBox.h"

Navigator::Navigator()
: mEventSink(this)
{
  SetObjectClass(_T("Navigator"));
  
  nuiScrollView* pScroll = new nuiScrollView(false, true);
  AddChild(pScroll);
  
  mpBox = new nuiVBox(0);
  mpBox->SetExpand(nuiExpandShrinkAndGrow);
  pScroll->AddChild(mpBox);
}

Navigator::~Navigator()
{
  
}


void Navigator::Update()
{
  mpBox->Clear();
  Sql::Get()->Open();
  
  std::vector<nglString> tables;
  Sql::Get()->GetTables(tables);
  
  for (uint32 i = 0; i < tables.size(); i++)
  {
    nuiRadioButton* pButton = new nuiRadioButton();
    pButton->SetObjectClass(_T("NavigatorItem"));
    pButton->SetGroup(_T("Navigator"));
    mpBox->AddCell(pButton);
    
    nuiHBox* pBox = new nuiHBox(2);
    pBox->SetExpand(nuiExpandShrinkAndGrow);
    pButton->AddChild(pBox);
    
    nuiLabel* pLabel = new nuiLabel(tables[i]);
    pLabel->SetObjectClass(_T("NavigatorItemLabel"));
    pBox->SetCell(1, pLabel);
    
    pButton->SetToken(new nuiToken<nglString>(tables[i]));
    
    mEventSink.Connect(pButton->Activated, &Navigator::OnItemActivated, (void*)pButton);
    
  }
  
  Sql::Get()->Close();
  
}

bool Navigator::OnItemActivated(const nuiEvent& rEvent)
{
  nuiButton* pButton = (nuiButton*)rEvent.mpUser;
  nglString table;
  
  if (!nuiGetTokenValue<nglString>(pButton->GetToken(), table))
    return true;
  
  nuiNotification* pNotif = new nuiNotification(NOTIF_TABLE_SELECTED);
  pNotif->SetToken(new nuiToken<nglString>(table));
  GetMainWindow()->PostNotification(pNotif);
  
  return true;
}

