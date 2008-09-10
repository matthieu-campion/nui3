/*
 *  nuiBindings.cpp
 *  nui3
 *
 *  Created by Sebastien Metrot on 05/09/2008.
 *  Copyright 2008 MXP4. All rights reserved.
 *
 */

#include "nui.h"
#include "nuiBindings.h"
#include "nuiBindingManager.h"

#include "nuiWidget.h"

#define nuiBindClass(CLASSNAME) \
{ \
nuiClass* pClass = new nuiClass(_T(#CLASSNAME)); \
nuiBindingManager::GetManager().AddClass(pClass);

#define nuiEndClass }

#define nuiInheritFrom(PARENT_CLASS) pClass->AddParentClass(nuiBindingManager::GetManager().GetClass(_T(#PARENT_CLASS)));
#define nuiAddCtor(CTOR_DECL) pClass->AddConstructor(new nuiFunction(CTOR_DECL));
#define nuiAddMethod(METHOD_NAME, METHOD)  pClass->AddMethod(_T(#METHOD_NAME), new nuiFunction(&METHOD));
#define nuiAddFunction(FUNCTION_NAME, FUNCTION) nuiBindingManager::GetManager().AddFunction(_T(#FUNCTION_NAME), new nuiFunction(FUNCTION));

static nuiObject* nuiCreate_nuiObject()
{
  return new nuiObject();
}

bool nuiInitBindings()
{
  nuiBindClass(nuiObject);
    nuiAddCtor(nuiCreate_nuiObject);
    nuiAddMethod(GetObjectName,   nuiObject::GetObjectName);
    nuiAddMethod(SetObjectName,   nuiObject::SetObjectName);
    nuiAddMethod(GetObjectClass,  nuiObject::GetObjectClass);
//    nuiAddMethod(IsOfClass,       nuiObject::IsOfClass);
//    nuiAddMethod(IsOfClassId,     nuiObject::IsOfClass );
//    nuiAddMethod(SetProperty,     nuiObject::SetProperty);
//    nuiAddMethod(GetProperty,     nuiObject::GetProperty);
//    nuiAddMethod(HasProperty,     nuiObject::HasProperty);
    nuiAddMethod(ClearProperties, nuiObject::ClearProperties);
//    nuiAddMethod(ClearProperty,   nuiObject::ClearProperty);
    nuiAddMethod(GetObjectClassNameIndex, nuiObject::GetObjectClassNameIndex);

  nuiEndClass
  
  nglString str;
  nuiBindingManager::GetManager().Dump(str);
  NGL_OUT(_T("Binding manager dump:\n%ls\n"), str.GetChars());
  
  return true;
}


