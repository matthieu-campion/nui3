/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiXML.h"


#define PREFERENCES_PROJECTCREATOR _T("ProjectCreator")

class Preferences
{
public: 

  Preferences();
  ~Preferences();
  
  void Save();
  
  void Dump(nglString& rStore);
  
  bool IsDefined(const nglString& rKey);
  bool IsDefined(const nglString& rKey, const nglString& rName);
  
  bool RemoveKey(const nglString& rKey);
  
  bool GetNames(const nglString& rKey, std::list<nglString>& rStore);///< gives the list of entries in a key
  
  bool GetInt(const nglString& rKey, const nglString& rName, int64& store) const;
  bool GetUInt(const nglString& rKey, const nglString& rName, uint64& store) const;
  bool GetFloat(const nglString& rKey, const nglString& rName, float& store) const;
  bool GetDouble(const nglString& rKey, const nglString& rName, double& store) const;  
  bool GetBool(const nglString& rKey, const nglString& rName, bool& store) const;
  bool GetString(const nglString& rKey, const nglString& rName, nglString& store) const;

  void SetInt(const nglString& rKey, const nglString& rName, int64 value);
  void SetUInt(const nglString& rKey, const nglString& rName, uint64 value);
  void SetFloat(const nglString& rKey, const nglString& rName, float value);
  void SetDouble(const nglString& rKey, const nglString& rName, double value);
  void SetBool(const nglString& rKey, const nglString& rName, bool value);
  void SetString(const nglString& rKey, const nglString& rName, const nglString& value);

private:

  nuiXMLNode* OpenKey(const nglString& rKey);

  nuiXML mXml;
};