/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Preferences.h"


Preferences::Preferences() : mXml(_T("YapukaPreferences"))
{
  nglPath prefPath(ePathUserPreferences);
  
  prefPath += nglPath("/NUI");
  if(!prefPath.Exists())
    prefPath.Create();
  
  prefPath += nglPath("/yapuka.pref");

  if(!prefPath.Exists())
    Save();
    
  nglIFile xmlIFile(prefPath);
  if (!mXml.Load(xmlIFile))
  {
    printf("yapuka Preferences : ERROR in loading the xml preferences file '%s'!\n", prefPath.GetChars());
    NGL_ASSERT(0);
  }
  xmlIFile.Close();
}


Preferences::~Preferences()
{
  Save();
}

void Preferences::Dump(nglString& rStore)
{
  rStore = mXml.Dump();
}



void Preferences::Save()
{
  nglPath prefPath(ePathUserPreferences);
  prefPath += nglString("/NUI/yapuka.pref");
  nglOFile xmlOFile(prefPath, eOFileCreate);
  mXml.Save(xmlOFile);
  xmlOFile.Close();  
}


bool Preferences::IsDefined(const nglString& rKey)
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  return (pNode != NULL);
}


bool Preferences::IsDefined(const nglString& rKey, const nglString& rName)
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;
  
  return pNode->HasAttribute(rName);
}



bool Preferences::RemoveKey(const nglString& rKey)
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;

  mXml.DelChild(pNode);
  return true;
}


bool Preferences::GetNames(const nglString& rKey, std::list<nglString>& rStore)
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;

  
  uint count = pNode->GetAttributeCount();
  for (uint i=0; i < count; i++)
  {
    const nglString& name = pNode->GetAttributeName(i);
    rStore.push_back(name);
  }
  return true;
}




//******************************************************************************************
//
// GETTERS
//
//******************************************************************************************


bool Preferences::GetInt(const nglString& rKey, const nglString& rName, int64& store) const
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCInt64();
  return true;
}


bool Preferences::GetUInt(const nglString& rKey, const nglString& rName, uint64& store) const
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCUInt64();
  return true;
}


bool Preferences::GetFloat(const nglString& rKey, const nglString& rName, float& store) const
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCFloat();
  return true;
}


bool Preferences::GetDouble(const nglString& rKey, const nglString& rName, double& store) const
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName).GetCDouble();
  return true;
}


bool Preferences::GetBool(const nglString& rKey, const nglString& rName, bool& store) const
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  nglString value = pNode->GetAttribute(rName);
  if (!value.Compare(_T("false")))
    store = false;
  else
    store = true;
  
  return true;
}



bool Preferences::GetString(const nglString& rGroup, const nglString& rName, nglString& store) const
{
  nuiXMLNode* pNode = mXml.GetChild(rGroup);
  if (!pNode)
    return false;
  
  if (!pNode->HasAttribute(rName))
    return false;
  
  store = pNode->GetAttribute(rName);
  return true;
}




//******************************************************************************************
//
// SETTERS
//
//******************************************************************************************


void Preferences::SetInt(const nglString& rKey, const nglString& rName, int64 value)
{
  nuiXMLNode* pNode = OpenKey(rKey);
  pNode->SetAttribute(rName, value);
}



void Preferences::SetUInt(const nglString& rKey, const nglString& rName, uint64 value)
{
  nuiXMLNode* pNode = OpenKey(rKey);
  pNode->SetAttribute(rName, value);
}


void Preferences::SetFloat(const nglString& rKey, const nglString& rName, float value)
{
  nuiXMLNode* pNode = OpenKey(rKey);
  pNode->SetAttribute(rName, value);
}


void Preferences::SetDouble(const nglString& rKey, const nglString& rName, double value)
{
  nuiXMLNode* pNode = OpenKey(rKey);
  pNode->SetAttribute(rName, value);
}



void Preferences::SetBool(const nglString& rKey, const nglString& rName, bool value)
{
  nuiXMLNode* pNode = OpenKey(rKey);
  pNode->SetAttribute(rName, value);
}



void Preferences::SetString(const nglString& rKey, const nglString& rName, const nglString& value)
{
  nuiXMLNode* pNode = OpenKey(rKey);
  pNode->SetAttribute(rName, value);
}




//******************************************************************************************
//
// PRIVATE
//
//******************************************************************************************

nuiXMLNode* Preferences::OpenKey(const nglString& rKey)
{
  nuiXMLNode* pNode = mXml.GetChild(rKey);
  if (pNode)
    return pNode;
  
  pNode = new nuiXMLNode(rKey);
  mXml.AddChild(pNode);
  return pNode;
}





