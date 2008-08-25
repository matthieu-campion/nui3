/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiAttribute.h"
#include "nuiPercentAttributeEditor.h"
#include "nuiPointAttributeEditor.h"
#include "nuiColorAttributeEditor.h"
#include "nuiRectAttributeEditor.h"
#include "nuiStringAttributeEditor.h"
#include "nuiBooleanAttributeEditor.h"
#include "nuiSizeAttributeEditor.h"
#include "nuiBorderAttributeEditor.h"
#include "nuiRangeAttributeEditor.h"

nuiAttributeBase::nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit unit, const nuiRange& rRange, bool readonly)
: mName(rName),
  mType(type),
  mUnit(unit),
  mReadOnly(readonly),
  mRange(rRange),
  mIgnoreAttributeChange(false),
  mOrder(0)
{

}



nuiAttributeBase::~nuiAttributeBase()
{
}



bool nuiAttributeBase::IsReadOnly() const
{  
  return mReadOnly;
}

nuiAttributeType nuiAttributeBase::GetType() const
{
  return mType;
}

nuiAttributeUnit nuiAttributeBase::GetUnit() const
{
  return mUnit;
}

const nglString& nuiAttributeBase::GetName() const
{
  return mName;
}

const nuiRange& nuiAttributeBase::GetRange() const
{
  return mRange;
}

nuiRange& nuiAttributeBase::GetRange()
{
  return mRange;
} 

void nuiAttributeBase::SetOrder(int32 order)
{
  mOrder = order;
}

int32 nuiAttributeBase::GetOrder() const
{
  return mOrder;
}


////////////////// 
// Declaration of some property types specializations:
//template<uint32> nuiAttributeTypeTrait<uint32> nuiAttributeTypeTrait<uint32>::singleton;
template <typename T>
uint64 nuiAttributeTypeTrait<T>::mTypeId = GetNewAttributeUniqueId();

DECLARE_NUIATTRIBUTE_TYPE(bool);

DECLARE_NUIATTRIBUTE_TYPE(int8);
DECLARE_NUIATTRIBUTE_TYPE(int16);
DECLARE_NUIATTRIBUTE_TYPE(int32);
DECLARE_NUIATTRIBUTE_TYPE(int64);

DECLARE_NUIATTRIBUTE_TYPE(uint8);
DECLARE_NUIATTRIBUTE_TYPE(uint16);
DECLARE_NUIATTRIBUTE_TYPE(uint32);
DECLARE_NUIATTRIBUTE_TYPE(uint64);

DECLARE_NUIATTRIBUTE_TYPE(float);
DECLARE_NUIATTRIBUTE_TYPE(double);

DECLARE_NUIATTRIBUTE_TYPE(nglString);
DECLARE_NUIATTRIBUTE_TYPE(const nglString&);
DECLARE_NUIATTRIBUTE_TYPE(nuiPoint);
DECLARE_NUIATTRIBUTE_TYPE(nuiRect);
DECLARE_NUIATTRIBUTE_TYPE(const nuiRect&);
DECLARE_NUIATTRIBUTE_TYPE(nuiBorder);
DECLARE_NUIATTRIBUTE_TYPE(const nuiBorder&);
DECLARE_NUIATTRIBUTE_TYPE(nuiColor);
DECLARE_NUIATTRIBUTE_TYPE(const nuiColor&);
DECLARE_NUIATTRIBUTE_TYPE(const nuiRange&);



//********************************************************************************************************
//
// TEMPLATE TRAITS SPECIALIZATION : Xml Load, Xml Serialize, GetDefaultEditor
//

//********************************
//
// bool
//

template class nuiAttribute<bool>;

template <>
bool nuiAttribute<bool>::ToString(void* pTarget, nglString& rString) const
{
  if (Get(pTarget))
    rString = _T("true");
  else
    rString = _T("false");

  return true;
}

template <>
bool nuiAttribute<bool>::FromString(void* pTarget, const nglString& rString) const
{
  if (rString.Compare(_T("true"), false) == 0)
    Set(pTarget, true);
  else
    Set(pTarget, false);
  
  return true;
}

template <>
nuiAttributeEditor* nuiAttribute<bool>::GetDefaultEditor(void* pTarget)
{
	return new nuiBooleanAttributeEditor(nuiAttrib<bool>(pTarget, this));
}

template <>
void nuiAttribute<bool>::FormatDefault(void* pTarget, nglString& rString)
{
	bool value = Get(pTarget);
	if (value)
		rString = _T("true");
	else
		rString = _T("false");
}


//********************************
//
// int8
//

template class nuiAttribute<int8>;

template <>
nuiAttributeEditor* nuiAttribute<int8>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<int8>::FormatDefault(void* pTarget, nglString& string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<int8>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<int8>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;

  Set(pTarget, rString.GetCInt());
  return true;
}



//********************************
//
// int16
//

template class nuiAttribute<int16>;

template <>
nuiAttributeEditor* nuiAttribute<int16>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<int16>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<int16>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<int16>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCInt());
  return true;
}




//********************************
//
// int32
//

template class nuiAttribute<int32>;

template <>
nuiAttributeEditor* nuiAttribute<int32>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<int32>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<int32>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<int32>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCInt());
  return true;
}



//********************************
//
// int64
//

template class nuiAttribute<int64>;

template <>
nuiAttributeEditor* nuiAttribute<int64>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<int64>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<int64>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<int64>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCInt64());
  return true;
}




//********************************
//
// uint8
//

template class nuiAttribute<uint8>;

template <>
nuiAttributeEditor* nuiAttribute<uint8>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<uint8>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<uint8>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCUInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<uint8>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCUInt());
  return true;
}



//********************************
//
// uint16
//

template class nuiAttribute<uint16>;

template <>
nuiAttributeEditor* nuiAttribute<uint16>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<uint16>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<uint16>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCUInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<uint16>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCUInt());
  return true;
}




//********************************
//
// uint32
//

template class nuiAttribute<uint32>;

template <>
nuiAttributeEditor* nuiAttribute<uint32>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<uint32>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<uint32>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCUInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<uint32>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCUInt());
  return true;
}




//********************************
//
// uint64
//

template class nuiAttribute<uint64>;


template <>
nuiAttributeEditor* nuiAttribute<uint64>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return NULL;
}

template <>
void nuiAttribute<uint64>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<uint64>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCUInt(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<uint64>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  Set(pTarget, rString.GetCUInt64());
  return true;
}



//********************************
//
// float
//

template class nuiAttribute<float>;

template <>
nuiAttributeEditor* nuiAttribute<float>::GetDefaultEditor(void* pTarget)
{
	switch (GetUnit())
	{
	case nuiUnitPercent:
		return new nuiPercentAttributeEditor(nuiAttrib<float>(pTarget, this));
	case nuiUnitNone : break;
    case nuiUnitSeconds : break;
	case nuiUnitPixels : break;
    case nuiUnitBytes : break;
    case nuiUnitOnOff : break;
	case nuiUnitYesNo : break;
	case nuiUnitBoolean : break;
	case nuiUnitName : break;
	case nuiUnitHumanName : break;
	case nuiUnitPosition : break;
	case nuiUnitSize : break;
	case nuiUnitExpand : break;
	case nuiUnitOrientation : break;
	case nuiUnitColor : break;
	case nuiUnitCustom : break;
	}
	
	// if this code is executed, it means a case processing is missing
	return NULL;
}


template <>
void nuiAttribute<float>::FormatDefault(void* pTarget, nglString & string)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	NGL_ASSERT(0);
	return;
}

template <>
bool nuiAttribute<float>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCFloat(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<float>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsFloat())
    return false;
  
  Set(pTarget, rString.GetCFloat());
  return true;
}


//********************************
//
// double
//

template class nuiAttribute<double>;

template <>
nuiAttributeEditor* nuiAttribute<double>::GetDefaultEditor(void* pTarget)
{
	switch (GetUnit())
  {
    case nuiUnitSize:
    return new nuiSizeAttributeEditor(nuiAttrib<double>(pTarget, this));
    case nuiUnitNone : break;
    case nuiUnitSeconds : break;
    case nuiUnitPixels : break;
    case nuiUnitBytes : break;
    case nuiUnitOnOff : break;
    case nuiUnitYesNo : break;
    case nuiUnitBoolean : break;
    case nuiUnitName : break;
    case nuiUnitHumanName : break;
    case nuiUnitPosition : break;
    case nuiUnitPercent : break;
    case nuiUnitExpand : break;
    case nuiUnitOrientation : break;
    case nuiUnitColor : break;
    case nuiUnitCustom : break;
  }

  // if this code is executed, it means a case processing is missing
  return NULL;
}

template <>
void nuiAttribute<double>::FormatDefault(void* pTarget, nglString & string)
{
  double value = Get(pTarget);
  string.SetCDouble(value);
}


template <>
bool nuiAttribute<double>::ToString(void* pTarget, nglString& rString) const
{
  rString.SetCDouble(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<double>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.IsFloat())
    return false;
  
  Set(pTarget, rString.GetCDouble());
  return true;
}



//********************************
//
// nuiPosition
//

template class nuiAttribute<nuiPosition>;

template <>
nuiAttributeEditor* nuiAttribute<nuiPosition>::GetDefaultEditor(void* pTarget)
{
  //#FIXME TODO
  // if this code is executed, it means a case processing is missing
  return NULL;
}

template <>
void nuiAttribute<nuiPosition>::FormatDefault(void* pTarget, nglString & string)
{
  nuiPosition value = Get(pTarget);
  string = nuiGetPosition(value);
}


template <>
bool nuiAttribute<nuiPosition>::ToString(void* pTarget, nglString& rString) const
{
  rString = nuiGetPosition(Get(pTarget));
  return true;
}

template <>
bool nuiAttribute<nuiPosition>::FromString(void* pTarget, const nglString& rString) const
{
  Set(pTarget, nuiGetPosition(rString));
  return true;
}








//********************************
//
// nglString
//

template class nuiAttribute<nglString>;

template <>
bool nuiAttribute<nglString>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget);
  return true;
}

template <>
bool nuiAttribute<nglString>::FromString(void* pTarget, const nglString& rString) const
{
  Set(pTarget, rString);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<nglString>::GetDefaultEditor(void* pTarget)
{
  return new nuiStringAttributeEditor(nuiAttrib<nglString>(pTarget, this));
}

template <>
void nuiAttribute<nglString>::FormatDefault(void* pTarget, nglString & string)
{
  string = Get(pTarget);
}




//********************************
//
// const nglString&
//

template class nuiAttribute<const nglString&>;

template <>
nuiAttributeEditor* nuiAttribute<const nglString&>::GetDefaultEditor(void* pTarget)
{	
  return new nuiStringAttributeEditor(nuiAttrib<const nglString&>(pTarget, this));
}

template <>
void nuiAttribute<const nglString&>::FormatDefault(void* pTarget, nglString & string)
{
  string = Get(pTarget);
}

template <>
bool nuiAttribute<const nglString&>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget);
  return true;
}

template <>
bool nuiAttribute<const nglString&>::FromString(void* pTarget, const nglString& rString) const
{
  Set(pTarget, rString);
  return true;
}





//********************************
//
// nuiColor
//

template class nuiAttribute<nuiColor>;

template <>
bool nuiAttribute<nuiColor>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<nuiColor>::FromString(void* pTarget, const nglString& rString) const
{
  nuiColor col(rString);
  Set(pTarget, col);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<nuiColor>::GetDefaultEditor(void* pTarget)
{
  return new nuiColorAttributeEditor(nuiAttrib<nuiColor>(pTarget, this));
}

template <>
void nuiAttribute<nuiColor>::FormatDefault(void* pTarget, nglString & string)
{
  //#FIXME TODO
  // if this code is executed, it means a case processing is missing
  NGL_ASSERT(0);
  return;
}





//********************************
//
// const nuiColor&
//

template class nuiAttribute<const nuiColor&>;

template <>
nuiAttributeEditor* nuiAttribute<const nuiColor&>::GetDefaultEditor(void* pTarget)
{
  return new nuiColorAttributeEditor(nuiAttrib<const nuiColor&>(pTarget, this));
}

template <>
void nuiAttribute<const nuiColor&>::FormatDefault(void* pTarget, nglString & string)
{
  //#FIXME TODO
  // if this code is executed, it means a case processing is missing
  NGL_ASSERT(0);
  return;
}


template <>
bool nuiAttribute<const nuiColor&>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiColor&>::FromString(void* pTarget, const nglString& rString) const
{
  nuiColor col(rString);
  Set(pTarget, col);
  return true;
}









//********************************
//
// nuiPoint
//

template class nuiAttribute<nuiPoint>;

template <>
bool nuiAttribute<nuiPoint>::ToString(void* pTarget, nglString& rString) const
{
  nuiPoint point = Get(pTarget);
  rString.CFormat(_T("%f,%f"), point[0], point[1]);
  return true;
}

template <>
bool nuiAttribute<nuiPoint>::FromString(void* pTarget, const nglString& rString) const
{
  std::vector<nglString> tokens;
  rString.Tokenize(tokens, nglChar(','));
  nuiSize x = tokens[0].GetCDouble();
  nuiSize y = tokens[1].GetCDouble();
  
  nuiPoint point(x,y);
  Set(pTarget, point);
  return true;
}

template <>
nuiAttributeEditor* nuiAttribute<nuiPoint>::GetDefaultEditor(void* pTarget)
{
  return new nuiPointAttributeEditor(nuiAttrib<nuiPoint>(pTarget, this));
}

template <>
void nuiAttribute<nuiPoint>::FormatDefault(void* pTarget, nglString & string)
{
  nuiPoint point = Get(pTarget);
  string.CFormat(_T("X: %.0fpx\nY: %.0fpx"), point[0], point[1]);
}




//********************************
//
// nuiRect
//

template class nuiAttribute<nuiRect>;

template <>
bool nuiAttribute<nuiRect>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<nuiRect>::FromString(void* pTarget, const nglString& rString) const
{
  nuiRect rect;
  rect.SetValue(rString);
  Set(pTarget, rect);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<nuiRect>::GetDefaultEditor(void* pTarget)
{
  return new nuiRectAttributeEditor(nuiAttrib<nuiRect>(pTarget, this));
}


template <>
void nuiAttribute<nuiRect>::FormatDefault(void* pTarget, nglString & string)
{
  nuiRect rect = Get(pTarget);
  string.CFormat(_T("(%.0f,%.0f) : (%.0f x %0.f)"), rect.Left(), rect.Top(), rect.GetWidth(), rect.GetHeight());
}









//********************************
//
// const nuiRect&
//

template class nuiAttribute<const nuiRect&>;


template <>
bool nuiAttribute<const nuiRect&>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiRect&>::FromString(void* pTarget, const nglString& rString) const
{
  nuiRect rect;
  rect.SetValue(rString);
  Set(pTarget, rect);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<const nuiRect&>::GetDefaultEditor(void* pTarget)
{
  return new nuiRectAttributeEditor(nuiAttrib<const nuiRect&>(pTarget, this));
}


template <>
void nuiAttribute<const nuiRect&>::FormatDefault(void* pTarget, nglString & string)
{
  nuiRect rect = Get(pTarget);
  string.CFormat(_T("(%.0f,%.0f) : (%.0f x %0.f)"), rect.Left(), rect.Top(), rect.GetWidth(), rect.GetHeight());
}










//********************************
//
// nuiBorder
//

template class nuiAttribute<nuiBorder>;

template <>
bool nuiAttribute<nuiBorder>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<nuiBorder>::FromString(void* pTarget, const nglString& rString) const
{
  nuiBorder border;
  border.SetValue(rString);
  Set(pTarget, border);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<nuiBorder>::GetDefaultEditor(void* pTarget)
{
  return new nuiBorderAttributeEditor(nuiAttrib<nuiBorder>(pTarget, this));
}


template <>
void nuiAttribute<nuiBorder>::FormatDefault(void* pTarget, nglString & string)
{
  nuiBorder border = Get(pTarget);
  string.CFormat(_T("(%.0f,%.0f,%.0f,%0.f)"), border.Left(), border.Top(), border.Right(), border.Bottom());
}









//********************************
//
// const nuiBorder&
//

template class nuiAttribute<const nuiBorder&>;


template <>
bool nuiAttribute<const nuiBorder&>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiBorder&>::FromString(void* pTarget, const nglString& rString) const
{
  nuiBorder border;
  border.SetValue(rString);
  Set(pTarget, border);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<const nuiBorder&>::GetDefaultEditor(void* pTarget)
{
  return new nuiBorderAttributeEditor(nuiAttrib<const nuiBorder&>(pTarget, this));
}


template <>
void nuiAttribute<const nuiBorder&>::FormatDefault(void* pTarget, nglString & string)
{
  nuiBorder border = Get(pTarget);
  string.CFormat(_T("(%.0f,%.0f,%.0f,%0.f)"), border.Left(), border.Top(), border.Right(), border.Bottom());
}







//********************************
//
// nglPath
//

template class nuiAttribute<nglPath>;

template <>
bool nuiAttribute<nglPath>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetPathName();
  return true;
}

template <>
bool nuiAttribute<nglPath>::FromString(void* pTarget, const nglString& rString) const
{
  Set(pTarget, rString);
  return true;
}

template <>
void nuiAttribute<nglPath>::FormatDefault(void* pTarget, nglString & string)
{
  string = Get(pTarget).GetPathName();
}




//********************************
//
// const nglPath&
//

template class nuiAttribute<const nglPath&>;

template <>
void nuiAttribute<const nglPath&>::FormatDefault(void* pTarget, nglString & string)
{
  string = Get(pTarget).GetPathName();
}

template <>
bool nuiAttribute<const nglPath&>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetPathName();
  return true;
}

template <>
bool nuiAttribute<const nglPath&>::FromString(void* pTarget, const nglString& rString) const
{
  Set(pTarget, rString);
  return true;
}





//********************************
//
// DecorationMode
//

template class nuiAttribute<nuiDecorationMode>;

template <>
void nuiAttribute<nuiDecorationMode>::FormatDefault(void* pTarget, nglString & string)
{
  nuiDecorationMode mode = Get(pTarget);
  switch (mode)
  {
    case eDecorationOverdraw:
      string = "Overdraw";
      break;
    case eDecorationBorder:
      string = "Border";
      break;
    case eDecorationClientOnly:
      string = "ClientOnly";
      break;
    default:
      string = "UnknownDecorationMode";
      break;
  }
  
}

template <>
bool nuiAttribute<nuiDecorationMode>::ToString(void* pTarget, nglString& rString) const
{
  nuiDecorationMode mode = Get(pTarget);
  switch (mode)
  {
    case eDecorationOverdraw:
      rString = "Overdraw";
      return true;
    case eDecorationBorder:
      rString = "Border";
      return true;
    case eDecorationClientOnly:
      rString = "ClientOnly";
      return true;
    default:
      rString = "UnknownDecorationMode";
      return false;
  }
  
}

template <>
bool nuiAttribute<nuiDecorationMode>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.Compare(_T("Overdraw")))
  {
    Set(pTarget, eDecorationOverdraw);
    return true;
  }
  else if (!rString.Compare(_T("Border")))
  {
    Set(pTarget, eDecorationBorder);
    return true;
  }
  if (!rString.Compare(_T("ClientOnly")))
  {
    Set(pTarget, eDecorationClientOnly);
    return true;
  }
  else
    return false;
}















//********************************
//
// ShapeMode
//

template class nuiAttribute<nuiShapeMode>;

template <>
void nuiAttribute<nuiShapeMode>::FormatDefault(void* pTarget, nglString & string)
{
  nuiShapeMode mode = Get(pTarget);
  switch (mode)
  {
    case eStrokeShape:
      string = "Stroke";
      break;
    case eFillShape:
      string = "Fill";
      break;
    case eStrokeAndFillShape:
      string = "StrokeAndFill";
      break;
    default:
      string = "UnknownShapeMode";
      break;
  }
  
}

template <>
bool nuiAttribute<nuiShapeMode>::ToString(void* pTarget, nglString& rString) const
{
  nuiShapeMode mode = Get(pTarget);
  switch (mode)
  {
    case eStrokeShape:
      rString = "Stroke";
      break;
    case eFillShape:
      rString = "Fill";
      break;
    case eStrokeAndFillShape:
      rString = "StrokeAndFill";
      break;
    default:
      rString = "UnknownShapeMode";
      return false;
  }
  
  return true;
  
}

template <>
bool nuiAttribute<nuiShapeMode>::FromString(void* pTarget, const nglString& rString) const
{
  if (!rString.Compare(_T("Stroke")))
  {
    Set(pTarget, eStrokeShape);
    return true;
  }
  else if (!rString.Compare(_T("Fill")))
  {
    Set(pTarget, eFillShape);
    return true;
  }
  if (!rString.Compare(_T("StrokeAndFill")))
  {
    Set(pTarget, eStrokeAndFillShape);
    return true;
  }
  else
    return false;
}




//********************************
//
// const nuiRange&
//

template class nuiAttribute<const nuiRange&>;


template <>
bool nuiAttribute<const nuiRange&>::ToString(void* pTarget, nglString& rString) const
{
  rString = Get(pTarget).GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiRange&>::FromString(void* pTarget, const nglString& rString) const
{
  nuiRange range;
  range.FromString(rString);
  Set(pTarget, range);
  return true;
}


template <>
nuiAttributeEditor* nuiAttribute<const nuiRange&>::GetDefaultEditor(void* pTarget)
{
  return new nuiRangeAttributeEditor(nuiAttrib<const nuiRange&>(pTarget, this));
}


template <>
void nuiAttribute<const nuiRange&>::FormatDefault(void* pTarget, nglString & string)
{
  nuiRange range = Get(pTarget);
  range.ToString(string);
}







//********************************
//
// nuiDecorationLayer
//

template class nuiAttribute<nuiDecorationLayer>;

template <>
bool nuiAttribute<nuiDecorationLayer>::ToString(void* pTarget, nglString& rString) const
{
  nuiDecorationLayer layer = Get(pTarget);
  if (layer == eLayerBack)
    rString = _T("Back");
  else
    rString = _T("Front");
  
  return true;
}

template <>
bool nuiAttribute<nuiDecorationLayer>::FromString(void* pTarget, const nglString& rString) const
{
  nuiDecorationLayer layer;
  if (!rString.Compare(_T("Back")))
    layer = eLayerBack;
  else
    layer = eLayerFront;
  
  Set(pTarget, layer);
  return true;
}

template <>
nuiAttributeEditor* nuiAttribute<nuiDecorationLayer>::GetDefaultEditor(void* pTarget)
{
  // #FIXME TODO
  return NULL;
}

template <>
void nuiAttribute<nuiDecorationLayer>::FormatDefault(void* pTarget, nglString & string)
{
  nuiDecorationLayer layer = Get(pTarget);
  if (layer == eLayerBack)
    string = _T("Back");
  else
    string = _T("Front");
}



//********************************
//
// nuiMouseCursor
//

template class nuiAttribute<nuiMouseCursor>;

template <>
bool nuiAttribute<nuiMouseCursor>::ToString(void* pTarget, nglString& rString) const
{
  nuiMouseCursor cursor = Get(pTarget);
  
  switch(cursor)
  {
    case eCursorDoNotSet:
      rString = _T("DoNotSet");
      break;
    case eCursorNone:
      rString = _T("None");
      break;
    case eCursorArrow:
      rString = _T("Arrow");
      break;
    case eCursorCross:
      rString = _T("Cross");
      break;
    case eCursorIBeam:
      rString = _T("IBeam");
      break;
    case eCursorHand:
      rString = _T("Hand");
      break;
    case eCursorClosedHand:
      rString = _T("ClosedHand");
      break;
    case eCursorHelp:
      rString = _T("Help");
      break;
    case eCursorWait:
      rString = _T("Wait");
      break;
    case eCursorCaret:
      rString = _T("Caret");
      break;
    case eCursorDnD:
      rString = _T("DnD");
      break;
    case eCursorForbid:
      rString = _T("Forbid");
      break;
    case eCursorMove:
      rString = _T("Move");
      break;
    case eCursorResize:
      rString = _T("Resize");
      break;
    case eCursorResizeNS:
      rString = _T("ResizeNS");
      break;
    case eCursorResizeWE:
      rString = _T("ResizeWE");
      break;
    case eCursorResizeN:
      rString = _T("ResizeN");
      break;
    case eCursorResizeS:
      rString = _T("ResizeS");
      break;
    case eCursorResizeW:
      rString = _T("ResizeW");
      break;
    case eCursorResizeE:
      rString = _T("DoNotSet");
      break;
    case eCursorResizeNW:
      rString = _T("ResizeNW");
      break;
    case eCursorResizeNE:
      rString = _T("ResizeNE");
      break;
    case eCursorResizeSW:
      rString = _T("ResizeSW");
      break;
    case eCursorResizeSE:
      rString = _T("ResizeSE");
      break;
  };
  
  return true;
}

template <>
bool nuiAttribute<nuiMouseCursor>::FromString(void* pTarget, const nglString& rString) const
{
  nuiMouseCursor cursor = eCursorNone;
  
  if (!rString.Compare(_T("DoNotSet")))
    cursor = eCursorDoNotSet;
  else if (!rString.Compare(_T("None")))
    cursor = eCursorNone;
  else if (!rString.Compare(_T("Arrow")))
    cursor = eCursorArrow;
  else if (!rString.Compare(_T("Cross")))
    cursor = eCursorCross;
  else if (!rString.Compare(_T("IBeam")))
    cursor = eCursorIBeam;
  else if (!rString.Compare(_T("Hand")))
    cursor = eCursorHand;
  else if (!rString.Compare(_T("ClosedHand")))
    cursor = eCursorClosedHand;
  else if (!rString.Compare(_T("Help")))
    cursor = eCursorHelp;
  else if (!rString.Compare(_T("Wait")))
    cursor = eCursorWait;
  else if (!rString.Compare(_T("Caret")))
    cursor = eCursorCaret;
  else if (!rString.Compare(_T("DnD")))
    cursor = eCursorDnD;
  else if (!rString.Compare(_T("Forbid")))
    cursor = eCursorForbid;
  else if (!rString.Compare(_T("Move")))
    cursor = eCursorMove;
  else if (!rString.Compare(_T("Resize")))
    cursor = eCursorResize;
  else if (!rString.Compare(_T("ResizeNS")))
    cursor = eCursorResizeNS;
  else if (!rString.Compare(_T("ResizeWE")))
    cursor = eCursorResizeWE;
  else if (!rString.Compare(_T("ResizeN")))
    cursor = eCursorResizeN;
  else if (!rString.Compare(_T("ResizeS")))
    cursor = eCursorResizeS;
  else if (!rString.Compare(_T("ResizeW")))
    cursor = eCursorResizeW;
  else if (!rString.Compare(_T("ResizeE")))
    cursor = eCursorResizeE;
  else if (!rString.Compare(_T("ResizeNW")))
    cursor = eCursorResizeNW;
  else if (!rString.Compare(_T("ResizeNE")))
    cursor = eCursorResizeNE;
  else if (!rString.Compare(_T("ResizeSW")))
    cursor = eCursorResizeSW;
  else if (!rString.Compare(_T("ResizeSE")))
    cursor = eCursorResizeSE;
  
  Set(pTarget, cursor);
  return true;
}

template <>
nuiAttributeEditor* nuiAttribute<nuiMouseCursor>::GetDefaultEditor(void* pTarget)
{
  // #FIXME TODO
  return NULL;
}

template <>
void nuiAttribute<nuiMouseCursor>::FormatDefault(void* pTarget, nglString& string)
{
  ToString(pTarget, string);
}


/////////////////////////////////
// nuiAttribBase
nuiAttribBase::nuiAttribBase()
: mpAttributeBase(NULL),
  mpTarget(NULL)
{
  
}

nuiAttribBase::nuiAttribBase(void* pTarget, nuiAttributeBase* pAttributeBase)
: mpAttributeBase(pAttributeBase),
  mpTarget(pTarget)
{
  NGL_ASSERT(mpTarget);
  NGL_ASSERT(mpAttributeBase);
}

nuiAttribBase::nuiAttribBase(const nuiAttribBase& rOriginal)
: mpAttributeBase(rOriginal.mpAttributeBase),
  mpTarget(rOriginal.mpTarget)
{
  NGL_ASSERT(mpTarget);
  NGL_ASSERT(mpAttributeBase);
}

nuiAttribBase::~nuiAttribBase()
{
}

nuiAttribBase& nuiAttribBase::operator=(const nuiAttribBase& rOriginal)
{
  mpAttributeBase = rOriginal.mpAttributeBase;
  mpTarget = rOriginal.mpTarget;
  return *this;
}

nuiAttributeBase* nuiAttribBase::GetAttribute()
{
  return mpAttributeBase;
}

const nuiAttributeBase* nuiAttribBase::GetAttribute() const
{
  return mpAttributeBase;
}

void* nuiAttribBase::GetTarget()
{
  return mpTarget;
}

const void* nuiAttribBase::GetTarget() const
{
  return mpTarget;
}


// From nuiAttribute and nuiAttributeBase
bool nuiAttribBase::IsReadOnly() const
{
  return mpAttributeBase->IsReadOnly();
}


nuiAttributeType nuiAttribBase::GetType() const
{
  return mpAttributeBase->GetType();
}

nuiAttributeUnit nuiAttribBase::GetUnit() const
{
  return mpAttributeBase->GetUnit();
}

const nglString& nuiAttribBase::GetName() const
{
  return mpAttributeBase->GetName();
}

const nuiRange& nuiAttribBase::GetRange() const
{
  return mpAttributeBase->GetRange();
}

nuiRange& nuiAttribBase::GetRange()
{
  return mpAttributeBase->GetRange();
}


int32 nuiAttribBase::GetOrder() const
{
  return mpAttributeBase->GetOrder();
}

void nuiAttribBase::SetOrder(int32 order)
{
  return mpAttributeBase->SetOrder(order);
}


bool nuiAttribBase::Load(const nuiXMLNode* pNode)
{
  return mpAttributeBase->Load(mpTarget, pNode);
}

nuiXMLNode* nuiAttribBase::Serialize(nuiXMLNode* pParentNode) const
{
  return mpAttributeBase->Serialize(mpTarget, pParentNode);
}


bool nuiAttribBase::ToString(nglString& rString) const
{
  return mpAttributeBase->ToString(mpTarget, rString);
}

bool nuiAttribBase::FromString(const nglString& rString) const
{
  return mpAttributeBase->FromString(mpTarget, rString);
}


void nuiAttribBase::IgnoreAttributeChange(bool ignore)
{
  mpAttributeBase->IgnoreAttributeChange(ignore);
}


bool nuiAttribBase::IsAttributeChangeIgnored() const
{
  return mpAttributeBase->IsAttributeChangeIgnored();
}


nuiAttributeEditor* nuiAttribBase::GetEditor()
{
  return mpAttributeBase->GetEditor(mpTarget);
}

bool nuiAttribBase::IsValid() const
{
  return mpTarget && mpAttributeBase;
}

nuiAttribBase::operator bool() const
{
  return IsValid();
}

