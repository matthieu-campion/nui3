/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nglDataTypesRegistry::nglDataTypesRegistry()
{

}

nglDataTypesRegistry::~nglDataTypesRegistry()
{

}

bool nglDataTypesRegistry::IsTypeRegistered(nglNativeObjectType Type)
{
  for (std::map<nglString, nglNativeObjectType>::iterator it = mRegisteredNativeTypes.begin(); it != mRegisteredNativeTypes.end(); it++)
    if (it->second == Type)
      return true;
  return false;
}
  
bool nglDataTypesRegistry::IsTypeRegistered(const nglString& rMimeType)
{
  std::map<nglString, nglNativeObjectType>::iterator it = mRegisteredNativeTypes.find(rMimeType);
  if (it != mRegisteredNativeTypes.end())
    return true;
  return false;
}
  
nglNativeObjectType nglDataTypesRegistry::RegisterDataType(const nglString& rMimeType, nglNativeObjectType Type, nglDataObjectCreatorFunc pCreator)
{
#ifdef _WIN32_
  if (!Type)
    Type = RegisterClipboardFormat((LPCTSTR)(rMimeType.GetChars()));
  if (!Type)
  {
    NGL_LOG(_T("nglNativeObjectTypesRegistry"), NGL_LOG_ERROR, _T("RegisterDragAndDropNativeType: Can't RegisterClipboardFormat(\")%s\")"), rMimeType.GetChars());
  }
#endif
  
  {
    std::map<nglString, nglNativeObjectType>::iterator it = mRegisteredNativeTypes.find(rMimeType);
    
    if (it != mRegisteredNativeTypes.end())
      Type = it->second;
    else
    // dynamically generate a new Type
    {
      if (Type == 'nui3')
      {
        uint32 size = mRegisteredNativeTypes.size(); // use the size (on 24bits, we should not have any overflow:) ) of the map as a unique index
        Type = ('n' << 24) | (size & 0xffffff);
      }

      // LBTODO ?
      mDataObjectCreators[rMimeType] = pCreator;
      mRegisteredNativeTypes[rMimeType] = Type;
    }
  }
  
  return Type;
}

bool nglDataTypesRegistry::GetRegisteredMimeType(nglNativeObjectType Type, nglString& rMimeType)
{
  for (std::map<nglString, nglNativeObjectType>::iterator it = mRegisteredNativeTypes.begin(); it != mRegisteredNativeTypes.end(); ++it)
  {
    if (it->second == Type)
    {
      rMimeType = it->first;
      return true;
    }
  }
  return false;
}

bool nglDataTypesRegistry::GetRegisteredFormatType(const nglString& rMimeType, nglNativeObjectType& rType)
{
  std::map<nglString, nglNativeObjectType>::iterator it = mRegisteredNativeTypes.find(rMimeType);
  if (it != mRegisteredNativeTypes.end())
  {
    rType = mRegisteredNativeTypes[rMimeType];
    return true;
  }
  return false;
}

void nglDataTypesRegistry::EnumMimeTypes(std::list<nglString>& rEnumedTypes)
{
  std::map<nglString, nglNativeObjectType>::iterator end = mRegisteredNativeTypes.end();
  std::map<nglString, nglNativeObjectType>::iterator it = mRegisteredNativeTypes.begin();
  while (it != end)
  {
    rEnumedTypes.push_back(it->first);
    it++;
  }
}

nglDataObject* nglDataTypesRegistry::Create(const nglString& rMimeType)
{
  std::map<nglString, nglDataObjectCreatorFunc, nglString::LessFunctor>::iterator 
    it = mDataObjectCreators.find(rMimeType);
  
  NGL_ASSERT(it != mDataObjectCreators.end());
  
  nglDataObjectCreatorFunc pCreator = it->second;

  nglDataObject* pObject = pCreator(rMimeType);
  
  NGL_ASSERT(pObject);
  
  return pObject;
}

