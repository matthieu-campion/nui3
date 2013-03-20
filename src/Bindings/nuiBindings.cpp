/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

NUI_DECLARE_ATTRIBUTE_TYPE(void);
NUI_DECLARE_ATTRIBUTE_TYPE(void*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiObject*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiWidget*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiContainer*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiSimpleContainer*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiTopLevel*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiMainWindow*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiLabel*);

template <typename Type>
static Type* nuiCreateObject()
{
  return new Type();
}

template <typename Type, typename P0>
static Type* nuiCreateObject1(P0 p0)
{
  return new Type(p0);
}

template <typename Type, typename P0, typename P1>
static Type* nuiCreateObject2(P0 p0, P1 p1)
{
  return new Type(p0, p1);
}

template <typename Type, typename P0, typename P1, typename P2>
static Type* nuiCreateObject3(P0 p0, P1 p1, P2 p2)
{
  return new Type(p0, p1, p2);
}

template <typename Type, typename P0, typename P1, typename P2, typename P3>
static Type* nuiCreateObject4(P0 p0, P1 p1, P2 p2, P3 p3)
{
  return new Type(p0, p1, p2, p3);
}

template <typename Type, typename P0, typename P1, typename P2, typename P3, typename P4>
static Type* nuiCreateObject5(P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
{
  return new Type(p0, p1, p2, p3, p4);
}


bool nuiInitBindings()
{
  nuiBindClass(nuiObject);
    nuiAddCtor(nuiCreateObject<nuiObject>);
    nuiAddMethod(GetObjectName,   nuiObject::GetObjectName);
    nuiAddMethod(SetObjectName,   nuiObject::SetObjectName);
    nuiAddMethod(GetObjectClass,  nuiObject::GetObjectClass);
//    nuiAddMethod(IsOfClass,       nuiObject::IsOfClass);
//    nuiAddMethod(IsOfClassId,     nuiObject::IsOfClass );
    nuiAddMethod(GetObjectClassNameIndex, nuiObject::GetObjectClassNameIndex);
  nuiEndClass;

#ifndef _MINUI3_
  nuiBindClass(nuiWidget);
  nuiInheritFrom(nuiObject);
  nuiAddCtor(nuiCreateObject<nuiWidget>);
  nuiAddMethod(Trash, nuiWidget::Trash);
  nuiAddMethod(CalcIdealSize, nuiWidget::CalcIdealSize);
  nuiAddMethod(SetRect, nuiWidget::SetRect);
  nuiAddMethod(SetAlpha, nuiWidget::SetAlpha);
  nuiAddMethod(SetEnabled, nuiWidget::SetEnabled);
  nuiAddMethod(SetSelected, nuiWidget::SetSelected);
  nuiAddMethod(GetRect, nuiWidget::GetRect);
  nuiAddMethod(GetAlpha, nuiWidget::GetAlpha);
  nuiAddMethod(IsEnabled, nuiWidget::IsEnabled);
  nuiAddMethod(IsSelected, nuiWidget::IsSelected);
  nuiAddMethod(Grab, nuiWidget::Grab);
  nuiAddMethod(Ungrab, nuiWidget::Ungrab);
  nuiEndClass;
  
  nuiBindClass(nuiContainer);
  nuiInheritFrom(nuiWidget);
  //nuiAddMethod(GetChild, nuiContainer::GetChild);
  pClass->AddMethod(_T("GetChild"), new nuiFunction(&nuiContainer::GetChild));
  nuiAddMethod(GetChildrenCount, nuiContainer::GetChildrenCount);
  nuiAddMethod(GetNextFocussableChild, nuiContainer::GetNextFocussableChild);
  nuiAddMethod(GetPreviousFocussableChild, nuiContainer::GetPreviousFocussableChild);
  nuiAddMethod(GetNextSibling, nuiContainer::GetNextSibling);
  nuiAddMethod(GetPreviousSibling, nuiContainer::GetPreviousSibling);
  nuiEndClass;

  nuiBindClass(nuiSimpleContainer);
  nuiInheritFrom(nuiContainer);
  nuiAddCtor(nuiCreateObject<nuiSimpleContainer>);
  nuiAddMethod(AddChild, nuiSimpleContainer::AddChild);
  nuiEndClass;
  
  nuiBindClass(nuiTopLevel);
  nuiInheritFrom(nuiSimpleContainer);
  nuiEndClass;
  
  nuiBindClass(nuiMainWindow);
  nuiInheritFrom(nuiTopLevel);
  pClass->AddConstructor(new nuiFunction(nuiCreateObject3<nuiMainWindow,int32,int32,bool>));
  nuiEndClass;
  
  nuiBindClass(nuiLabel);
  nuiInheritFrom(nuiWidget);
  //nuiAddCtor(nuiCreateObject1<nuiLabel, const nglString&>);
  pClass->AddConstructor(new nuiFunction(nuiCreateObject1<nuiLabel, const nglString&>));
  nuiAddMethod(SetText, nuiLabel::SetText);
  nuiEndClass;
  
  
  // Add Enums:
  nuiBindingManager& rBM(nuiBindingManager::GetManager());
  
  // nuiPosition:
  nuiAddEnum(nuiCenter);
  nuiAddEnum(nuiLeft);
  nuiAddEnum(nuiRight);
  nuiAddEnum(nuiTop);
  nuiAddEnum(nuiBottom);
  nuiAddEnum(nuiTopLeft);
  nuiAddEnum(nuiTopRight);
  nuiAddEnum(nuiBottomLeft);
  nuiAddEnum(nuiBottomRight);
  nuiAddEnum(nuiFill);
  nuiAddEnum(nuiFillHorizontal);
  nuiAddEnum(nuiFillVertical);
  nuiAddEnum(nuiFillTop);
  nuiAddEnum(nuiFillBottom);
  nuiAddEnum(nuiFillLeft);
  nuiAddEnum(nuiFillRight);

  // nuiOrientation:
  nuiAddEnum(nuiHorizontal);
  nuiAddEnum(nuiVertical);
#endif // ifndef _MINUI3_

  nglString str;
  nuiBindingManager::GetManager().Dump(str);
  NGL_OUT(_T("Binding manager dump:\n%s\n"), str.GetChars());
  
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

