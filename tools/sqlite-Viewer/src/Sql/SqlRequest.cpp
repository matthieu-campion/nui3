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




bool SqlRequest::Insert(const std::list<nglString>& rValues)
{
//  if (rValues.size() != tags.size())
//  {
//    NGL_OUT(_T("SqlRequest::Insert error: nb values (%d) doesn't match nb tags (%d).\n"), rValues.size(), tags.size());
//    NGL_ASSERT(0);
//    return false;
//  }
//  
//  nglString req;
//  std::list<nglString>::const_iterator it;
//
//  req.Append(_T("INSERT INTO ")).Append(mTable).Append(_T(" (")).Append(tags[0]);
//  for (uint32 i = 1; i < tags.size(); i++)
//    req.Append(_T(", ")).Append(tags[i]);
//  
//  it = rValues.begin();
//  req.Append(_T(") VALUES (")).Append(*it);
//  ++it;
//  
//  while(it != rValues.end())
//  {
//    req.Append(_T(", ")).Append(*it);
//    ++it;
//  }
//
//  req.Append(_T(")"));
//  
//  return Sql::Get()->Execute(req);
  return false;
}




bool SqlRequest::Delete(const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues)
{
  if (rCriteria.size() != rCriteriaValues.size())
  {
    NGL_OUT(_T("SqlRequest::Delete error: nb criteria (%d) doesn't match nb values (%d).\n"), rCriteria.size(), rCriteriaValues.size());
    NGL_ASSERT(0);
    return false;
  }
  
  nglString req;

  req.Append(_T("DELETE FROM ")).Append(mTable).Append(Where(rCriteria, rCriteriaValues));

  return Sql::Get()->Execute(req);  
}


bool SqlRequest::Delete(const nglString& rCriteria, const nglString& rCriteriaValue)
{
  std::list<nglString> criteria;
  criteria.push_back(rCriteria);
  
  std::list<nglString> values;
  values.push_back(rCriteriaValue);
  
  return SqlRequest::Delete(criteria, values);
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


bool SqlRequest::Select(std::vector<std::vector<nglString> >& rResults, const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues)
{
  if (rCriteria.size() != rCriteriaValues.size())
  {
    NGL_OUT(_T("SqlRequest::Select error: nb criteria (%d) doesn't match nb values (%d).\n"), rCriteria.size(), rCriteriaValues.size());
    NGL_ASSERT(0);
    return false;
  }
  
  nglString req;
  
  req.Append(_T("SELECT * FROM ")).Append(mTable).Append(Where(rCriteria, rCriteriaValues));
  
  bool res = Sql::Get()->Execute(req, rResults);
  if (!res)
    return false;
  
  return true;
}



bool SqlRequest::Select(std::vector<nglString>& rResults, const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues)
{
  bool res = SqlRequest::Select(rResults, rCriteria, rCriteriaValues);
  if (!res)
    return false;
  
  return true;
}

bool SqlRequest::Select(std::vector<nglString>& rResults, const nglString& rCriteria, const nglString& rCriteriaValue)
{
  std::list<nglString> criteria;
  criteria.push_back(rCriteria);
  std::list<nglString> values;
  values.push_back(rCriteriaValue);
  
  bool res = SqlRequest::Select(rResults, criteria, values);
  if (!res)
    return false;
  
  return true;
  
}


nglString SqlRequest::Where(const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues)
{
  NGL_ASSERT((rCriteria.size() != 0) && (rCriteriaValues.size() != 0));
  
  nglString req;
  std::list<nglString>::const_iterator itc = rCriteria.begin();
  std::list<nglString>::const_iterator itv = rCriteriaValues.begin();
  
  req.Append(_T(" WHERE ")).Append(*itc).Append(_T("=")).Append(*itv);
  ++itc;
  ++itv;
  while (itc != rCriteria.end())
  {
    req.Append(_T(" AND ")).Append(*itc).Append(_T("=")).Append(*itv);
    ++itc;
    ++itv;    
  }  
  
  return req;
}

bool SqlRequest::Drop(bool force)
{
  if (!force)
    return false;
  
  nglString req;
  std::list<nglString>::const_iterator it;
  
  req.Append(_T("DROP TABLE ")).Append(mTable);

  return Sql::Get()->Execute(req);
  
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








  


