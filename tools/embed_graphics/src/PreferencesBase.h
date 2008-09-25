/*
 *  PreferencesBase.h
 *  MXEditor
 *
 *  Created by Loic Berthelot on 2008/08/27
 *  Copyright 2007 MXP4. All rights reserved.
 *
 */

#pragma once

#include "nui.h"
#include "nuiXML.h"

#define MAIN_KEY _T("MainKey")

enum PreferencesTarget
{
  eAppPref=0,
  eSessionPref
};

class PreferencesBase : public nuiNonCopyable
{
public: 

  PreferencesBase(const nglString& rPrefName);
  ~PreferencesBase();
  
  void PostInit();
  void Save();
  
  
  bool IsDefined(const nglString& rKey, PreferencesTarget target=eAppPref);
  bool IsDefined(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref);
  
  bool CreateKey(const nglString& rKey, PreferencesTarget target=eAppPref);
  bool RemoveKey(const nglString& rKey, PreferencesTarget target=eAppPref);
  bool RemoveName(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref);
  
  bool GetNames(const nglString& rKey, std::list<nglString>& rStore, PreferencesTarget target=eAppPref);///< gives the list of entries in a key
  
  // safe getters with bool return value to check if key exists
  bool GetInt(const nglString& rKey, const nglString& rName, int64& store, PreferencesTarget target=eAppPref) const;
  bool GetUInt(const nglString& rKey, const nglString& rName, uint64& store, PreferencesTarget target=eAppPref) const;
  bool GetFloat(const nglString& rKey, const nglString& rName, float& store, PreferencesTarget target=eAppPref) const;
  bool GetDouble(const nglString& rKey, const nglString& rName, double& store, PreferencesTarget target=eAppPref) const;  
  bool GetBool(const nglString& rKey, const nglString& rName, bool& store, PreferencesTarget target=eAppPref) const;
  bool GetString(const nglString& rKey, const nglString& rName, nglString& store, PreferencesTarget target=eAppPref) const;
  bool GetPath(const nglString& rKey, const nglString& rName, nglPath& store, PreferencesTarget target=eAppPref) const;
  
  // unsafe getters only fail an assert if key does not exist
  int64 GetInt(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref) const;
  uint64 GetUInt(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref) const;
  float GetFloat(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref) const;
  double GetDouble(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref) const;  
  bool GetBool(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref) const;
  nglString GetString(const nglString& rKey, const nglString& rName, PreferencesTarget target=eAppPref) const;

  void SetInt(const nglString& rKey, const nglString& rName, int64 value, PreferencesTarget target=eAppPref);
  void SetUInt(const nglString& rKey, const nglString& rName, uint64 value, PreferencesTarget target=eAppPref);
  void SetFloat(const nglString& rKey, const nglString& rName, float value, PreferencesTarget target=eAppPref);
  void SetDouble(const nglString& rKey, const nglString& rName, double value, PreferencesTarget target=eAppPref);
  void SetBool(const nglString& rKey, const nglString& rName, bool value, PreferencesTarget target=eAppPref);
  void SetString(const nglString& rKey, const nglString& rName, const nglString& value, PreferencesTarget target=eAppPref);
  void SetPath(const nglString& rKey, const nglString& rName, const nglPath& value, PreferencesTarget target=eAppPref);

protected:

  std::vector<nuiXML*> mXml;
  nglString mPrefName;
  
private:

  nuiXMLNode* OpenKey(const nglString& rKey, PreferencesTarget target);

};
