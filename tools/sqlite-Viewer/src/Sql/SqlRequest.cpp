/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "Sql/SqlRequest.h"
#include <sqlite3.h>
#include "Sql/Sql.h"





SqlRequest::~SqlRequest()
{
}

SqlRequest::SqlRequest(const nglString& rTable)
{
  mTable = rTable;
}





bool SqlRequest::Select(std::vector<std::vector<nglString> >& rResults)
{
  nglString req;
  
  req.Append(_T("SELECT * FROM ")).Append(mTable);
  
  bool res = Sql::Get()->Execute(req, rResults);
  if (!res)
    return false;
  
  return true;
}


bool SqlRequest::GetTags(std::vector<nglString>& rTags)
{
  std::vector<std::vector<nglString> >results;
  
  nglString req = _T("PRAGMA table_info(");
  req.Append(mTable).Append(_T(")"));
  
  bool res = Sql::Get()->Execute(req, results);
  
  for (uint32 i = 0; i < results.size(); i++)
  {
    const std::vector<nglString>& row = results[i];
    if (row.size() == 0)
      return false;
    
    rTags.push_back(row[1]);
  }
  
  return res;
}








  


