/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
#pragma once

#include "nui.h"
#include <sqlite3.h>


/// formaters for sql requests building
extern nglString& _SQL(nglString& rStr);
extern nglString  _SQL(float value);
extern nglString  _SQL(uint32 value);
extern nglString  _SQL(sqlite_int64 value);
extern nglString  _SQL(bool value);
extern nglString  _SQL(const nglPath& rPath);


/// implements the global sql database handler
class Sql
{
public:

  /// singleton
  static Sql* Get();

  ~Sql();
  
  /// open/close the DB access
  void SetDB(const nglPath& rPath);
  const nglPath& GetDB();
  bool Open();
  bool Close();
  bool IsOpened();
  
  bool GetTables(std::vector<nglString>& tables);

  
  /// execute a request, withouth the need of output results
  bool Execute(const nglString& rRequest);
  
  /// execute a sql request, and output the results in the given matrix  
  bool Execute(const nglString& rRequest, std::vector<std::vector<nglString> >& rResults);
  
protected:
  
  Sql();
  
  
private:
  
	static Sql*	mpInstance;

  nglPath mPath;
  sqlite3* mpDB;
  bool mOpened;
  char* mpErrMsg;  
};

