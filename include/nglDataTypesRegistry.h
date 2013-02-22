/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglDataTypesRegistry.h
\brief Data Object Types Registry (MIME registry for drag and drop, see nglWindow)
*/

#ifndef __nglDataTypesRegistry_h__
#define __nglDataTypesRegistry_h__

//#include "nui.h"

#ifdef _WIN32_
typedef UINT        nglNativeObjectType;
#endif
#ifdef _UNIX_
typedef uint        nglNativeObjectType;
#endif
#ifdef _COCOA_
typedef uint        nglNativeObjectType;
#endif
#ifdef _UIKIT_
typedef uint        nglNativeObjectType;
#endif
#ifdef _ANDROID_
typedef uint        nglNativeObjectType;
#endif
#ifdef _CARBON_
typedef FlavorType  nglNativeObjectType;
#endif


class nglDataObject;

typedef nglDataObject* (*nglDataObjectCreatorFunc)(const nglString& rMime);

class NGL_API nglDataTypesRegistry
{
public:
  nglDataTypesRegistry();
  ~nglDataTypesRegistry();

  bool IsTypeRegistered(nglNativeObjectType Type);
  bool IsTypeRegistered(const nglString& rMimeType);
  
  nglNativeObjectType RegisterDataType(const nglString& rMimeType, nglNativeObjectType Type, nglDataObjectCreatorFunc pCreator);

  bool GetRegisteredMimeType(nglNativeObjectType Type, nglString& rMimeType);

  bool GetRegisteredFormatType(const nglString& rMimeType, nglNativeObjectType& rType);
  
  void EnumMimeTypes(std::list<nglString>& rEnumedTypes);

  nglDataObject* Create(const nglString& rMimeType);
protected:
  std::map<nglString, nglNativeObjectType> mRegisteredNativeTypes;
  std::map<nglString, nglDataObjectCreatorFunc, nglString::LessFunctor> mDataObjectCreators;
};

#endif // __nglDataTypesRegistry_h__
