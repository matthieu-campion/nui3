/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "Sql/Sql.h"
#include "Sql/SqlRequest.h"


#define LOG_SQL 1


Sql* Sql::mpInstance = NULL;

// static
Sql* Sql::Get()
{
	if (mpInstance == NULL)
	{
		mpInstance = new Sql();
		NGL_ASSERT(mpInstance);
	}
	return mpInstance;
}


Sql::Sql()
{
  mpDB = NULL;
  mpErrMsg = NULL;
  mOpened = false;
}



bool Sql::IsOpened()
{
  return mOpened;
}



void Sql::SetDB(const nglPath& rPath)
{
  if (mOpened)
    Close();
  mPath = rPath;
}

const nglPath& Sql::GetDB()
{
  return mPath;
}

bool Sql::Open()
{
  if (mOpened)
    Close();
  
  if (SQLITE_OK != sqlite3_open(mPath.GetPathName().GetStdString().c_str(), &mpDB))
  {
    NGL_OUT(_T("Sql : error : could not open the database '%ls'\n"), mPath.GetChars());
    mpDB = NULL;
    return false;
  }

  mOpened = true;
  NGL_OUT(_T("DB opened: '%ls'\n"), mPath.GetChars());
  return true;
}



bool Sql::Close()
{
  if (!mOpened)
    return false;
  
  mOpened = false;
  sqlite3_close(mpDB);
  return true;
}








Sql::~Sql()
{
}


//**********************************************************************************************************
//
// requests process
//
//**********************************************************************************************************

bool Sql::GetTables(std::vector<nglString>& tables)
{
  nglString req = _T("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name");
  
  std::vector<std::vector<nglString> > results;
  bool res = Execute(req, results);
  
  for (uint32 i = 0; i < results.size(); i++)
  {
    const std::vector<nglString>& row = results[i];
    if (row.size() == 0)
      return false;
    
    tables.push_back(row[0]);
  }
  
  return res;
}



bool Sql::Execute(const nglString& rRequest)
{
  // avoid memory leak
  if (mpErrMsg)
    sqlite3_free(mpErrMsg);

  int32 returnCode = sqlite3_exec(mpDB, rRequest.GetStdString().c_str(), NULL, NULL, &mpErrMsg);

  // print result message
  #ifdef LOG_SQL
  if (returnCode == SQLITE_OK)
    NGL_OUT(_T("DB Executed: %ls\n"), rRequest.GetChars());
  else
    NGL_OUT(_T("DB error : %s in request %ls\n"), mpErrMsg, rRequest.GetChars());
  #endif
  
  return (returnCode == SQLITE_OK);  
}


bool Sql::Execute(const nglString& rRequest, std::vector<std::vector<nglString> >& rResults)
{
  char **result;
  int32 nbRows = 0;
  int32 nbCols = 0;
  
  // avoid memory leak
  if (mpErrMsg)
    sqlite3_free(mpErrMsg);
    
  #ifdef LOG_SQL
  NGL_OUT(_T("DB Executes: %ls\n"), rRequest.GetChars());
  #endif
  
  
  int32 returnCode = sqlite3_get_table(mpDB, rRequest.GetStdString().c_str(), &result, &nbRows, &nbCols, &mpErrMsg);
  
  if (returnCode == SQLITE_OK) 
  {
    // init results vector
    rResults.clear();
    rResults.resize(nbRows);
    for (uint32 row = 0; row < nbRows; row++)
      rResults[row].resize(nbCols);
    
    // read request results
    for (uint32 row = 0; row < nbRows; row++)
    {
      for (uint32 col = 0; col < nbCols; col++)
      {
        rResults[row][col] = nglString(result[nbCols + row*nbCols +col]);
      }
    }
  }
  
  // print result message
  #ifdef LOG_SQL
  if (returnCode != SQLITE_OK)
  NGL_OUT(_T("DB error : %s in request %ls\n"), mpErrMsg, rRequest.GetChars());
  #endif
  
  sqlite3_free_table(result);
  return (returnCode == SQLITE_OK);  
}

//int32 DB::GetLastRowID()
//{
//  return sqlite3_last_insert_rowid(mpDB);
//}
//







//***************************************************************************************************************
//
// SQL formaters
//
//


nglString& _SQL(nglString& rStr)
{
  rStr.Prepend(_T("'"));
  rStr.Append(_T("'"));
  return rStr;  
}

nglString  _SQL(float value)
{
  nglString str;
  str.SetCFloat(value);
  return str;
}

nglString  _SQL(uint32 value)
{
  nglString str;
  str.SetCInt(value);
  return str;
}

nglString  _SQL(sqlite_int64 value)
{
  nglString str;
  str.SetCInt(value);
  return str;  
}


extern nglString  _SQL(bool value)
{
  int32 flag = (value)? 1 : 0;  
  nglString str;
  str.SetCInt(flag);
  return str;
}


nglString  _SQL(const nglPath& rPath)
{
  nglString str = rPath.GetPathName();
  str.Prepend(_T("'"));
  str.Append(_T("'"));
  return str;
}

