/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include <sqlite3.h>



///****************************************************************************************************
///
/// SqlRequest
///
/// implements a mother class for an sql table. Allows to process requests: insert, delete, select
class SqlRequest
{
public:
  SqlRequest(const nglString& rTable);  
  ~SqlRequest();

  bool GetTags(std::vector<nglString>& rTags);
  
  
  /// select all object(s) from table, store the results in the list (rows) of maps (columns)
  bool Select(std::vector<std::vector<nglString> >& rResults);
  
  

private:
  
  
  nglString mTable;
};












