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
  
  
  /// for sql reseting
  bool Drop(bool force = false);
  
  /// insert a new entry into the sql base. values must fit to the table tags. see specific Insert methods in overloaded classes, below
  bool Insert(const std::list<nglString>& rValues);

  
  /// delete object(s), depending on the selection criteria
  bool Delete(const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues);
  
  /// ... for 1 selection criteria
  bool Delete(const nglString& rCriteria, const nglString& rCriteriaValue);

  
  
  /// select all object(s) from table, store the results in the list (rows) of maps (columns)
  bool Select(std::vector<std::vector<nglString> >& rResults);
  
  /// select object(s) from table, depending on the selection criteria, and store the results in the list (rows) of maps (columns)
  bool Select(std::vector<std::vector<nglString> >& rResults, const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues);
  
  /// ... for the first row only (<=> first result to match the selection criteria)
  bool Select(std::vector<nglString>& rResults, const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues);
  
  /// ... for the first row only, for a single selection criteria
  bool Select(std::vector<nglString>& rResults, const nglString& rCriteria, const nglString& rCriteriaValue);
  

  
  

private:
  
  /// internal routine to format the criteria part of the request string
  nglString Where(const std::list<nglString>& rCriteria, const std::list<nglString>& rCriteriaValues);
  
  
  nglString mTable;
};












