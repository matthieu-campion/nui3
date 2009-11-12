/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiObject.h"
#include "nuiApplication.h"
#include "nuiMainWindow.h"
#include "nuiXML.h"

#include "nuiBuilder.h"

//#define NUI_OBJECT_DEBUG_TOOLTIPS


using namespace std;

nuiObject::nuiObject()
{	
  nglString name;
  name.CFormat(_T("%p"), this);
  Init(name);
}

nuiObject::nuiObject(const nglString& rObjectName)
{
  Init(rObjectName);
}


void nuiObject::Init(const nglString& rObjectName)
{	
  mClassNameIndex = -1;
  if (SetObjectClass(_T("nuiObject")))
    InitAttributes();
  
  SetObjectName(rObjectName);
  
  mSerializeMode = eSaveNode;
  
  mpToken = NULL;
  
}




void nuiObject::InitAttributes()
{
  nuiAttribute<const nglString&>* AttributeClass = new nuiAttribute<const nglString&>
  (nglString(_T("Class")), nuiUnitName,
   nuiAttribute<const nglString&>::GetterDelegate(this, &nuiObject::GetObjectClass));


  nuiAttribute<const nglString&>* AttributeName = new nuiAttribute<const nglString&>
  (nglString(_T("Name")), nuiUnitName,
   nuiAttribute<const nglString&>::GetterDelegate(this, &nuiObject::GetObjectName), 
   nuiAttribute<const nglString&>::SetterDelegate(this, &nuiObject::SetObjectName));

	AddAttribute(_T("Class"), AttributeClass);
	AddAttribute(_T("Name"), AttributeName);
}



bool nuiObject::Load(const nuiXMLNode* pNode)
{
	bool res=true;

  nglString name;
  name.CFormat(_T("%p"),this);

	nuiAttrib<const nglString&> att(GetAttribute(_T("Name")));
	att.Set(name);
	SetProperty(nglString(_T("Name")), name);
	
	// set the class of the node in the property xmlClass so that we can easily guess the final type even if we haven't finished the construction inheritance.
  SetProperty(nglString(_T("xmlClass")),pNode->GetName());

  mSerializeMode = eSaveNode;
	
	// search for object attributes in xml attributes
	std::map<nglString, nuiAttribBase> attributes;
  GetAttributes(attributes);
	std::map<nglString, nuiAttribBase>::const_iterator itp;
	for (itp = attributes.begin(); itp != attributes.end(); ++itp)
	{
		nuiAttribBase base = itp->second;
		if (pNode->HasAttribute(base.GetName()))
		{
			if (base.IsReadOnly())
        continue;
        
			res &= base.Load(pNode);
		}
	}
	
  return res;
}








nuiXMLNode* nuiObject::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{   
  nuiXMLNode* pNode = NULL;

  if (mSerializeMode == eDontSaveNode)
    return NULL;

  if (mSerializeMode != eSkipNode)
  {
    if (pParentNode)
    {
      pNode = new nuiXMLNode(GetObjectClass(),pParentNode);
    }
    else
    {
      pNode = new nuiXML(GetObjectClass());
    }

    if (!pNode)
      return NULL;

    pNode->SetAttribute(_T("Name"), GetObjectName());
    pNode->SetAttribute(_T("Class"), GetObjectClass());

    nuiPropertyMap::const_iterator it;
    nuiPropertyMap::const_iterator end = mProperties.end();
    for (it = mProperties.begin(); it!=end; ++it)
    {
      pNode->SetAttribute((*it).first,(*it).second);
    }
  }
  else
    pNode = pParentNode;
		
	
  // save object attributes
	std::map<nglString, nuiAttribBase>::const_iterator it;
	std::map<nglString, nuiAttribBase> attributes;
  GetAttributes(attributes);
	for (it = attributes.begin(); it != attributes.end(); ++it)
	{
		nuiAttribBase base = it->second;

		base.Serialize(pNode);
	}
	
	
  return pNode;

}







void nuiObject::SetSerializeMode (nuiSerializeMode eMode)
{
  mSerializeMode = eMode;
  DebugRefreshInfo();
}

nuiSerializeMode nuiObject::GetSerializeMode () const
{
  return mSerializeMode;
}

nuiObject::~nuiObject()
{
//  OUT("Deleting object '%ls' (class='%ls')\n", GetObjectName().GetChars(), GetObjectClass().GetChars());
  delete mpToken;

  int32 c = mClassNameIndex;
  while (c >= 0)
  {
    // clean attributes
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mClassAttributes[c].begin();
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mClassAttributes[c].end();
    
    while (it != end)
    {
      nuiAttributeBase* pAttribute = it->second;
      pAttribute->KillAttributeHolder(this);
      ++it;
    }
    
    c = mInheritanceMap[c];
  }
  
}


const nglString& nuiObject::GetObjectClass() const
{
	return GetClassNameFromIndex(mClassNameIndex);
}



const nglString& nuiObject::GetObjectName() const
{
	return mObjectName;
}


void nuiObject::SetObjectName(const nglString& rName)
{
  mObjectName = rName;
  
  DebugRefreshInfo();
	
#ifdef NUI_OBJECT_DEBUG_TOOLTIPS
  {
    // Enable this to debug your tooltips and classes
    nglString tt;
    tt.Add(GetObjectClass()).Add(_T(" - ")).Add(GetObjectName());
    SetProperty(_T("ToolTip"), tt);
  }
#endif
  
}

std::vector<int32> nuiObject::mInheritanceMap;

bool nuiObject::SetObjectClass(const nglString& rClass)
{
  int32 c = GetClassNameIndex(rClass);
  bool first = mInheritanceMap[c] < -1;
  mInheritanceMap[c] = GetObjectClassNameIndex();

//	const nglString propname = _T("Class");
//  mProperties[propname] = rClass;
    
  mClassNameIndex = c;

  DebugRefreshInfo();
  
#ifdef NUI_OBJECT_DEBUG_TOOLTIPS
  {
    // Enable this to debug your tooltips and classes
    nglString tt;
    tt.Add(GetObjectClass()).Add(_T(" - ")).Add(GetObjectName());
    SetProperty(_T("ToolTip"), tt);
  }
#endif
  return first;
}

void nuiObject::GetObjectInheritance(std::vector<nglString>& rClasses) const
{
  int32 c = mClassNameIndex;
  
  do
  {
    rClasses.push_back(GetClassNameFromIndex(c));
    c = mInheritanceMap[c];
  }
  while (c >= 0);
}


bool nuiObject::IsOfClass(const nglString& rClass) const
{
  int32 c = GetClassNameIndex(rClass);
  return IsOfClass(c);
}

bool nuiObject::IsOfClass(int32 ClassIndex) const
{
  NGL_ASSERT(ClassIndex < mInheritanceMap.size());
  
  int32 c = mClassNameIndex;

  while (c && c > ClassIndex)
  {
    c = mInheritanceMap[c];
  }
  
  return c == ClassIndex;
}

void nuiObject::SetProperty (const nglString& rName, const nglString& rValue)
{
//  if (rName == _T("ToolTip") && GetObjectClass() == _T("HelpLabel"))
//  {
//    NGL_OUT(_T("nuiObject::SetProperty for 0x%x %ls / %ls = %ls\n"), this, GetObjectClass().GetChars(), GetObjectName().GetChars(), rValue.GetChars());
//  }
  
  mProperties[rName] = rValue;
  OnPropertyChanged(rName, rValue);
  DebugRefreshInfo();
}

const nglString& nuiObject::GetProperty (const nglString& rName) const
{
  nuiPropertyMap::const_iterator it = mProperties.find(rName);
  if (it == mProperties.end())
    return nglString::Null;

  return it->second;
}

const nglString& nuiObject::GetProperty(const char* pName) const
{
  nglString tmp(pName);
  return GetProperty(tmp);
}

void nuiObject::SetProperty(const char* pName, const nglString& rValue)
{
  nglString tmp(pName);
  SetProperty(tmp, rValue);
}

void nuiObject::SetProperty(const char* pName, const char* pValue)
{
  nglString tmp(pValue);
  SetProperty(pName, tmp);
}

bool nuiObject::HasProperty(const char* pName) const
{
  nglString tmp(pName);
  return HasProperty(tmp);
}

bool nuiObject::ClearProperty(const char* pName)
{
  nglString tmp(pName);
  return ClearProperty(tmp);
}

bool nuiObject::GetProperties (list<nglString>& rPropertyNames) const
{
  nuiPropertyMap::const_iterator it;
  nuiPropertyMap::const_iterator end = mProperties.end();
  for (it = mProperties.begin(); it!=end; ++it)
  {
    rPropertyNames.push_back((*it).first);
  }
  return true;
}

bool nuiObject::HasProperty (const nglString& rName) const
{
  return mProperties.find(rName) != mProperties.end();
}

bool nuiObject::ClearProperty(const nglString& rName)
{
  nuiPropertyMap::iterator it = mProperties.find(rName);
  if (it != mProperties.end())
  {
    mProperties.erase(rName);
    DebugRefreshInfo();
    return true;
  }

  return false;
}

bool nuiObject::ClearProperties(bool ClearNameAndClassToo)
{
  if (ClearNameAndClassToo)
  {
    mProperties.clear();
  }
  else
  {
    nuiPropertyMap::iterator it = mProperties.begin();
    nuiPropertyMap::iterator end = mProperties.end();
    
    nglString strname("Name");
    nglString strclass("Class");
    
    while ( it != end )
    {
      if ((*it).first != strname && (*it).first != strclass)
        mProperties.erase(it++);
      else
        ++it;
    }
  }
  
  DebugRefreshInfo();
  return true;
}




void nuiObject::SetToken(nuiTokenBase* pToken)
{
  if (mpToken != pToken)
    delete mpToken;
  mpToken = pToken;
}

nuiTokenBase* nuiObject::GetToken() const
{
  return mpToken;  
}




//** nuiAttributes system *******************************************

uint32 nuiObject::mUniqueAttributeOrder = 0;


void nuiObject::GetAttributes(std::map<nglString, nuiAttribBase>& rAttributeMap) const
{
  rAttributeMap.clear();

  // Add instance attributes:
  {
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mInstanceAttributes.begin();
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mInstanceAttributes.end();
    
    while (it != end)
    {
      rAttributeMap.insert(make_pair(it->first, nuiAttribBase(const_cast<nuiObject*>(this), it->second)));
      ++it;
    }
  }

  // Add classes attributes:
  int32 c = mClassNameIndex;
  while (c >= 0)
  {
    //printf("\t\tattr for class %ls\n", GetClassNameFromIndex(c).GetChars());
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mClassAttributes[c].begin();
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mClassAttributes[c].end();
    
    while (it != end)
    {
      rAttributeMap.insert(make_pair(it->first, nuiAttribBase(const_cast<nuiObject*>(this), it->second)));
      ++it;
    }
    
    c = mInheritanceMap[c];
  }
}

static bool NUIATTRIBUTES_COMPARE(const nuiAttribBase& rLeft, const nuiAttribBase& rRight)
{
  return (rLeft.GetOrder() < rRight.GetOrder());
}

void nuiObject::GetSortedAttributes(std::list<nuiAttribBase>& rListToFill) const
{
  rListToFill.clear();

  // Add classes attributes
  int32 c = mClassNameIndex;
  while (c >= 0)
  {
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mClassAttributes[c].begin();
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mClassAttributes[c].end();
    while (it != end)
    {
      nuiAttributeBase* pBase = it->second;
      rListToFill.push_back(nuiAttribBase(const_cast<nuiObject*>(this), pBase));
      
      ++it;
    }
    
    c = mInheritanceMap[c];
  }

  // Add instance attributes
  {
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mInstanceAttributes.begin();
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mInstanceAttributes.end();
    while (it != end)
    {
      nuiAttributeBase* pBase = it->second;
      rListToFill.push_back(nuiAttribBase(const_cast<nuiObject*>(this), pBase));
      
      ++it;
    }
  }
  
  rListToFill.sort(NUIATTRIBUTES_COMPARE);
}




nuiAttribBase nuiObject::GetAttribute(const nglString& rName) const
{
  // Search Instance Attributes:
  {
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mInstanceAttributes.find(rName);
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mInstanceAttributes.end();
    
    if (it != end)
      return nuiAttribBase(const_cast<nuiObject*>(this), it->second);
  }
  
  
  // Search classes attributes:
  int32 c = mClassNameIndex;
  while (c >= 0)
  {
    std::map<nglString,nuiAttributeBase*>::const_iterator it = mClassAttributes[c].find(rName);
    std::map<nglString,nuiAttributeBase*>::const_iterator end = mClassAttributes[c].end();
    
    if (it != end)
      return nuiAttribBase(const_cast<nuiObject*>(this), it->second);
    
    c = mInheritanceMap[c];
  }
  
  return nuiAttribBase();
}


void nuiObject::AddAttribute(const nglString& rName, nuiAttributeBase* pAttribute)
{
  mUniqueAttributeOrder++;
  pAttribute->SetOrder(mUniqueAttributeOrder);

  NGL_ASSERT(mClassNameIndex < mClassAttributes.size());
  mClassAttributes[mClassNameIndex][rName] = pAttribute;
}

void nuiObject::AddAttribute(nuiAttributeBase* pAttribute)
{
  mUniqueAttributeOrder++;
  pAttribute->SetOrder(mUniqueAttributeOrder);

  mClassAttributes[mClassNameIndex][pAttribute->GetName()] = pAttribute;
}

void nuiObject::AddInstanceAttribute(const nglString& rName, nuiAttributeBase* pAttribute)
{
  mUniqueAttributeOrder++;
  pAttribute->SetOrder(mUniqueAttributeOrder);
  pAttribute->SetAsInstanceAttribute(true);
  
  mInstanceAttributes[rName] = pAttribute;
}

void nuiObject::AddInstanceAttribute(nuiAttributeBase* pAttribute)
{
  mUniqueAttributeOrder++;
  pAttribute->SetOrder(mUniqueAttributeOrder);
  pAttribute->SetAsInstanceAttribute(true);
  
  mInstanceAttributes[pAttribute->GetName()] = pAttribute;
}


void nuiObject::CopyProperties(const nuiObject& rObject)
{
  mProperties = rObject.mProperties;
}

std::vector<nglString> nuiObject::mObjectClassNames;
std::map<nglString, int32> nuiObject::mObjectClassNamesMap;
std::vector<std::map<nglString,nuiAttributeBase*> > nuiObject::mClassAttributes;

int32 nuiObject::GetObjectClassNameIndex() const
{
  return mClassNameIndex;
}

int32 nuiObject::GetClassNameIndex(const nglString& rName)
{
  std::map<nglString, int32>::iterator it = mObjectClassNamesMap.find(rName);
  if (it == mObjectClassNamesMap.end())
  {
    int32 index = mObjectClassNames.size();
    mObjectClassNamesMap[rName] = index;
    mObjectClassNames.push_back(rName);
    mClassAttributes.resize(index + 1);
    mInheritanceMap.push_back(-2); // -1 = not parent, -2 = not initialized
    NGL_DEBUG( printf("New class: %ls [%d]\n", rName.GetChars(), index); )
    
    return index;
  }
  return mObjectClassNamesMap[rName];
}

const nglString& nuiObject::GetClassNameFromIndex(int32 index)
{
  return mObjectClassNames[index];
}

//////////////////////////// Global Properties
nuiPropertyMap nuiObject::mGlobalProperties;


void nuiObject::SetGlobalProperty(const nglString& rName, const nglString& rValue)
{
  mGlobalProperties[rName] = rValue;
}

void nuiObject::SetGlobalProperty(const char* pName, const nglString& rValue)
{
  SetGlobalProperty(nglString(pName), rValue);
}

void nuiObject::SetGlobalProperty(const char* pName, const char* pValue)
{
  SetGlobalProperty(nglString(pName), nglString(pValue));
}

const nglString& nuiObject::GetGlobalProperty(const nglString& rName)
{
  const nuiPropertyMap::const_iterator it = mGlobalProperties.find(rName);
  if (it == mGlobalProperties.end())
    return nglString::Null;
  
  return it->second;
}

const nglString& nuiObject::GetGlobalProperty(const char* pName)
{
  return GetGlobalProperty(nglString(pName));
}

bool nuiObject::IsGlobalProperty(const nglString& rName)
{
  const nuiPropertyMap::const_iterator it = mGlobalProperties.find(rName);
  return (it != mGlobalProperties.end());
}

bool nuiObject::IsGlobalProperty(const char* pName)
{
  return IsGlobalProperty(nglString(pName));
}

bool nuiObject::ClearGlobalProperties()
{
  mGlobalProperties.clear();
  return true;
}

bool nuiObject::GetGlobalProperties(std::list<nglString>& rPropertyNames)
{
  nuiPropertyMap::const_iterator it = mGlobalProperties.begin();
  nuiPropertyMap::const_iterator end = mGlobalProperties.end();
  
  while (it != end)
  {
    rPropertyNames.push_back(it->first);
    ++it;
  }
  
  return true;
}

bool nuiObject::ClearGlobalProperty(const nglString& rName)
{
  nuiPropertyMap::iterator it = mGlobalProperties.find(rName);
  if (it != mGlobalProperties.end())
    return false;
  mGlobalProperties.erase(it);
  return true;
}

bool nuiObject::ClearGlobalProperty(const char* pName)
{
  return ClearGlobalProperty(nglString(pName));
}

void nuiObject::OnPropertyChanged(const nglString& rProperty, const nglString& rValue)
{
  //...
}

