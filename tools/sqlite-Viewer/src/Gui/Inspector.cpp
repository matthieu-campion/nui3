/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Gui/Inspector.h"
#include "Application.h"
#include "MainWindow.h"
#include "Gui/Gui.h"

#include "Sql/SqlRequest.h"
#include "Sql/Sql.h"

#include "nuiMessageBox.h"

Inspector::Inspector()
: nuiSimpleContainer()
{
  SetObjectClass(_T("Inspector"));
}

Inspector::~Inspector()
{

}

void Inspector::Update(const nglString& rTable)
{
  Clear();
  
  Sql::Get()->Open();
  
  GetPreferences().SetString(KEY_USER, _T("CurrentTable"), rTable);    

  SqlRequest req(rTable);
  
  // get column names
  std::vector<nglString> tags;
  req.GetTags(tags);
  
  // get all sql results
  std::vector<std::vector<nglString> > results;
  req.Select(results);

  Sql::Get()->Close();
  
  if (tags.size() == 0)
  {
    NGL_OUT(_T("Inspector::Update error\n"));
    nuiMessageBox* pBox = new nuiMessageBox(GetMainWindow(), _T("error"), _T("table has no tags!"), eMB_OK, false);
    pBox->QueryUser();
    return;
  }
  
  

  
  
  
  nuiScrollView* pScroll = new nuiScrollView(true, true);
  AddChild(pScroll);
  
  uint32 nbRows = (results.size() == 0) ? 2 : results.size()+1;

  nuiGrid* pGrid = new nuiGrid(tags.size(), nbRows);
  pScroll->AddChild(pGrid);

  // column names
  for (uint32 i = 0; i < tags.size(); i++)
  {
    nuiLabel* pLabel = new nuiLabel(tags[i]);
    pLabel->SetObjectClass(_T("InspectorColumnLabel"));
    pGrid->SetCell(i, 0, pLabel);
  }
  
  
  // empty table
  if (results.size() == 0)
  {
    nuiLabel* pLabel = new nuiLabel(_T("empty table!"));
    pLabel->SetObjectClass(_T("InspectorMessage"));
    pGrid->SetCell(0,1, pLabel);
    return;
  }
  
  // display sql results
  for (uint32 i = 0; i < results.size(); i++)
  {
    const std::vector<nglString>& row = results[i];
    
    for (uint32 j = 0; j < row.size(); j++)
    {
      nuiLabel* pLabel = new nuiLabel(row[j]);
      pLabel->SetObjectClass(_T("InspectorResult"));
      pGrid->SetCell(j,i+1, pLabel);      
    }
  }

}
