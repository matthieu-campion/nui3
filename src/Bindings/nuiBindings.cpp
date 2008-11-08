/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
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


// c:\work\nui3\include\nuiBindingManager.h(1087) : error C2664: 
// 'nuiMethodStorage0<Class,RetType>::nuiMethodStorage0(const nglString (__thiscall nuiObject::* )(void))' : cannot convert parameter 1 from 
// 'const nglString &(__thiscall nuiObject::* )(void) const' to 
// 'const nglString (__thiscall nuiObject::* )(void)'

// c:\work\nui3\include\nuiBindingManager.h(1087) : error C2664: 
// 'nuiMethodStorage0<Class,RetType>::nuiMethodStorage0(const nglString (__thiscall nuiObject::* )(void))' : cannot convert parameter 1 from
// 'const nglString &(__thiscall nuiObject::* )(void) const' to
// 'const nglString (__thiscall nuiObject::* )(void)'

