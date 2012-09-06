/*
 *  PreferencesBase.h
 *  MXEditor
 *
 *  Created by Loic Berthelot on 2008/08/27
 *  Copyright 2007 MXP4. All rights reserved.
 *
 */

#include "Application.h"
#include "PreferencesBase.h"


PreferencesBase::PreferencesBase(const nglString& rPrefName) 
: mPrefName(rPrefName)
{
  if (mPrefName == nglString::Null)
    return;
  
  nuiXML* appXml = new nuiXML(mPrefName + nglString(_T("Preferences")));
  nuiXML* sessXml = new nuiXML(mPrefName + nglString(_T("SessionPreferences")));
  mXml.push_back(appXml);
  mXml.push_back(sessXml);
  nglPath prefPath(ePathUserPreferences);
  
  prefPath += nglPath("/NUI");
  
  if(!prefPath.Exists())
    prefPath.Create();
  
  nglString nodename = mPrefName + nglString(".pref");
  prefPath += nodename;
  
  if (!prefPath.Exists())  // if pref file does not exist, create it
  {
    Save();
    PostInit();
  }
  
  nglIFile xmlIFile(prefPath);
  if (!mXml[eAppPref]->Load(xmlIFile))
  {
    printf("Preferences : ERROR in loading the xml preferences file '%ls'!\n", prefPath.GetPathName().GetChars());
    NGL_ASSERT(0);
  }
  xmlIFile.Close();
}


PreferencesBase::~PreferencesBase()
{
  Save();
  delete mXml[eAppPref];
  delete mXml[eSessionPref];
}


void PreferencesBase::PostInit()
{
  
}

void PreferencesBase::Save()
{
  nglPath prefPath(ePathUserPreferences);
  prefPath += "/NUI";
  
  nglString nodename = mPrefName + nglString(".pref");
  prefPath += nodename;
  nglOFile xmlOFile(prefPath, eOFileCreate);
  bool result = mXml[eAppPref]->Save(xmlOFile);
  xmlOFile.Close();  
}

bool PreferencesBase::IsDefined(const nglString& rKey, PreferencesTarget target)
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  return (pNode != NULL);
}


bool PreferencesBase::IsDefined(const nglString& rKey, const nglString& rName, PreferencesTarget target)
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  return pNode->HasAttribute(rName);
}

bool PreferencesBase::CreateKey(const nglString& rKey, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  
  return pNode;
}

bool PreferencesBase::RemoveKey(const nglString& rKey, PreferencesTarget target)
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;

  mXml[target]->DelChild(pNode);
  return true;
}

bool PreferencesBase::RemoveName(const nglString& rKey, const nglString& rName, PreferencesTarget target)
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  return pNode->DelAttribute(rName);
  
}



bool PreferencesBase::GetNames(const nglString& rKey, std::list<nglString>& rStore, PreferencesTarget target)
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;

  
  uint count = mXml[target]->GetAttributeCount();
  for (uint i=0; i < count; i++)
  {
    const nglString& name = mXml[target]->GetAttributeName(i);
    rStore.push_back(name);
  }
  return true;
}




//******************************************************************************************
//
// GETTERS
//
//******************************************************************************************


bool PreferencesBase::GetInt(const nglString& rKey, const nglString& rName, int64& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCInt64();
  return true;
}


bool PreferencesBase::GetUInt(const nglString& rKey, const nglString& rName, uint64& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCUInt64();
  return true;
}


bool PreferencesBase::GetFloat(const nglString& rKey, const nglString& rName, float& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCFloat();
  return true;
}


bool PreferencesBase::GetDouble(const nglString& rKey, const nglString& rName, double& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCDouble();
  return true;
}


bool PreferencesBase::GetBool(const nglString& rKey, const nglString& rName, bool& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).Compare(_T("false"));
  
  return true;
}



bool PreferencesBase::GetString(const nglString& rGroup, const nglString& rName, nglString& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rGroup);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName);
  return true;
}


bool PreferencesBase::GetPath(const nglString& rGroup, const nglString& rName, nglPath& store, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rGroup);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = nglPath(pNode->GetAttribute(rName));
  return true;  
}


int64 PreferencesBase::GetInt(const nglString& rKey, const nglString& rName, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  
  NGL_ASSERT(pNode);
  NGL_ASSERT(pNode->HasAttribute(rName));
  
  return pNode->GetAttribute(rName).GetCInt64();
}


uint64 PreferencesBase::GetUInt(const nglString& rKey, const nglString& rName, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  
  NGL_ASSERT(pNode);
  NGL_ASSERT(pNode->HasAttribute(rName));
  
  return pNode->GetAttribute(rName).GetCUInt64();
}


float PreferencesBase::GetFloat(const nglString& rKey, const nglString& rName, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  
  NGL_ASSERT(pNode);
  NGL_ASSERT(pNode->HasAttribute(rName));
  
  return pNode->GetAttribute(rName).GetCFloat();
}


double PreferencesBase::GetDouble(const nglString& rKey, const nglString& rName, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  
  NGL_ASSERT(pNode);
  NGL_ASSERT(pNode->HasAttribute(rName));
  
  return pNode->GetAttribute(rName).GetCDouble();
}


bool PreferencesBase::GetBool(const nglString& rKey, const nglString& rName, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  
  NGL_ASSERT(pNode);
  NGL_ASSERT(pNode->HasAttribute(rName));
  
  return pNode->GetAttribute(rName).Compare(_T("false"));
}



nglString PreferencesBase::GetString(const nglString& rGroup, const nglString& rName, PreferencesTarget target) const
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rGroup);
  
  NGL_ASSERT(pNode);
  NGL_ASSERT(pNode->HasAttribute(rName));
  
  return pNode->GetAttribute(rName);
}



//******************************************************************************************
//
// SETTERS
//
//******************************************************************************************


void PreferencesBase::SetInt(const nglString& rKey, const nglString& rName, int64 value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value);
}



void PreferencesBase::SetUInt(const nglString& rKey, const nglString& rName, uint64 value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value);
}


void PreferencesBase::SetFloat(const nglString& rKey, const nglString& rName, float value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value);
}


void PreferencesBase::SetDouble(const nglString& rKey, const nglString& rName, double value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value);
}



void PreferencesBase::SetBool(const nglString& rKey, const nglString& rName, bool value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value);
}



void PreferencesBase::SetString(const nglString& rKey, const nglString& rName, const nglString& value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value);
}


void PreferencesBase::SetPath(const nglString& rKey, const nglString& rName, const nglPath& value, PreferencesTarget target)
{
  nuiXMLNode* pNode = OpenKey(rKey, target);
  pNode->SetAttribute(rName, value.GetPathName());
}



//******************************************************************************************
//
// PRIVATE
//
//******************************************************************************************

nuiXMLNode* PreferencesBase::OpenKey(const nglString& rKey, PreferencesTarget target)
{
  nuiXMLNode* pNode = mXml[target]->GetChild(rKey);
  if (pNode)
    return pNode;
  
  pNode = new nuiXMLNode(rKey);
  mXml[target]->AddChild(pNode);
  return pNode;
}





