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

nuiAttributeBase::nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit unit, const nuiRange& rRange, bool readonly, bool writeonly)
: mName(rName),
  mType(type),
  mUnit(unit),
  mReadOnly(readonly),
  mWriteOnly(writeonly),
  mRange(rRange),
  mIgnoreAttributeChange(false),
  mOrder(0),
  mDimension(0)
{
}

nuiAttributeBase::nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit unit, const nuiRange& rRange, bool readonly, bool writeonly, uint32 dimension, const ArrayRangeDelegate& rRangeGetter)
: mName(rName),
  mType(type),
  mUnit(unit),
  mReadOnly(readonly),
  mWriteOnly(writeonly),
  mRange(rRange),
  mIgnoreAttributeChange(false),
  mOrder(0),
  mDimension(dimension),
  mRangeGetter(rRangeGetter)
{
}



nuiAttributeBase::~nuiAttributeBase()
{
}



bool nuiAttributeBase::IsReadOnly() const
{  
  return mReadOnly;
}

bool nuiAttributeBase::IsWriteOnly() const
{  
  return mWriteOnly;
}

bool nuiAttributeBase::CanGet() const
{  
  return !mWriteOnly;
}

bool nuiAttributeBase::CanSet() const
{  
  return !mReadOnly;
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

uint32 nuiAttributeBase::GetDimension() const
{
  return mDimension;
}

uint32 nuiAttributeBase::GetIndexRange(void* pTarget, uint32 Dimension) const
{
  NGL_ASSERT(GetDimension() > Dimension && mRangeGetter);
  ArrayRangeDelegate getter(mRangeGetter);
  getter.SetThis(pTarget);
  return getter(Dimension);
}

void nuiAttributeBase::IgnoreAttributeChange(bool ignore)
{
  mIgnoreAttributeChange = ignore;
}

bool nuiAttributeBase::IsAttributeChangeIgnored() const
{
  return mIgnoreAttributeChange;
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
bool nuiAttribute<bool>::ToString(bool value, nglString& rString) const
{
  if (value)
    rString = _T("true");
  else
    rString = _T("false");

  return true;
}

template <>
bool nuiAttribute<bool>::FromString(bool& rValue, const nglString& rString) const
{
  if (rString.Compare(_T("true"), false) == 0)
    rValue = true;
  else
    rValue = false;
  
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
bool nuiAttribute<int8>::ToString(int8 Value, nglString& rString) const
{
  rString.SetCInt(Value);
  return true;
}

template <>
bool nuiAttribute<int8>::FromString(int8& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;

  rValue = rString.GetCInt();
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
bool nuiAttribute<int16>::ToString(int16 Value, nglString& rString) const
{
  rString.SetCInt(Value);
  return true;
}

template <>
bool nuiAttribute<int16>::FromString(int16& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCInt();
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
bool nuiAttribute<int32>::ToString(int32 Value, nglString& rString) const
{
  rString.SetCInt(Value);
  return true;
}

template <>
bool nuiAttribute<int32>::FromString(int32& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCInt();
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
bool nuiAttribute<int64>::ToString(int64 Value, nglString& rString) const
{
  rString.SetCInt(Value);
  return true;
}

template <>
bool nuiAttribute<int64>::FromString(int64& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCInt64();
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
bool nuiAttribute<uint8>::ToString(uint8 Value, nglString& rString) const
{
  rString.SetCUInt(Value);
  return true;
}

template <>
bool nuiAttribute<uint8>::FromString(uint8& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCUInt();
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
bool nuiAttribute<uint16>::ToString(uint16 Value, nglString& rString) const
{
  rString.SetCUInt(Value);
  return true;
}

template <>
bool nuiAttribute<uint16>::FromString(uint16& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCUInt();
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
bool nuiAttribute<uint32>::ToString(uint32 Value, nglString& rString) const
{
  rString.SetCUInt(Value);
  return true;
}

template <>
bool nuiAttribute<uint32>::FromString(uint32& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCUInt();
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
bool nuiAttribute<uint64>::ToString(uint64 Value, nglString& rString) const
{
  rString.SetCUInt(Value);
  return true;
}

template <>
bool nuiAttribute<uint64>::FromString(uint64& rValue, const nglString& rString) const
{
  if (!rString.IsInt())
    return false;
  
  rValue = rString.GetCUInt64();
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
bool nuiAttribute<float>::ToString(float Value, nglString& rString) const
{
  rString.SetCFloat(Value);
  return true;
}

template <>
bool nuiAttribute<float>::FromString(float& rValue, const nglString& rString) const
{
  if (!rString.IsFloat())
    return false;
  
  rValue = rString.GetCFloat();
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
bool nuiAttribute<double>::ToString(double Value, nglString& rString) const
{
  rString.SetCDouble(Value);
  return true;
}

template <>
bool nuiAttribute<double>::FromString(double& rValue, const nglString& rString) const
{
  if (!rString.IsFloat())
    return false;
  
  rValue = rString.GetCDouble();
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
bool nuiAttribute<nuiPosition>::ToString(nuiPosition Value, nglString& rString) const
{
  rString = nuiGetPosition(Value);
  return true;
}

template <>
bool nuiAttribute<nuiPosition>::FromString(nuiPosition& rValue, const nglString& rString) const
{
  rValue = nuiGetPosition(rString);
  return true;
}








//********************************
//
// nglString
//

template class nuiAttribute<nglString>;

template <>
bool nuiAttribute<nglString>::ToString(nglString Value, nglString& rString) const
{
  rString = Value;
  return true;
}

template <>
bool nuiAttribute<nglString>::FromString(nglString& rValue, const nglString& rString) const
{
  rValue = rString;
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
bool nuiAttribute<const nglString&>::ToString(const nglString& rValue, nglString& rString) const
{
  rString = rValue;
  return true;
}

template <>
bool nuiAttribute<const nglString&>::FromString(nglString& rValue, const nglString& rString) const
{
  rValue = rString;
  return true;
}





//********************************
//
// nuiColor
//

template class nuiAttribute<nuiColor>;

template <>
bool nuiAttribute<nuiColor>::ToString(nuiColor Value, nglString& rString) const
{
  rString = Value.GetValue();
  return true;
}

template <>
bool nuiAttribute<nuiColor>::FromString(nuiColor& rValue, const nglString& rString) const
{
  rValue.SetValue(rString);
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
bool nuiAttribute<const nuiColor&>::ToString(const nuiColor& rValue, nglString& rString) const
{
  rString = rValue.GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiColor&>::FromString(nuiColor& rValue, const nglString& rString) const
{
  rValue.SetValue(rString);
  return true;
}









//********************************
//
// nuiPoint
//

template class nuiAttribute<nuiPoint>;

template <>
bool nuiAttribute<nuiPoint>::ToString(nuiPoint Value, nglString& rString) const
{
  return Value.GetValue(rString);
}

template <>
bool nuiAttribute<nuiPoint>::FromString(nuiPoint& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
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
bool nuiAttribute<nuiRect>::ToString(nuiRect Value, nglString& rString) const
{
  rString = Value.GetValue();
  return true;
}

template <>
bool nuiAttribute<nuiRect>::FromString(nuiRect& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
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
bool nuiAttribute<const nuiRect&>::ToString(const nuiRect& rValue, nglString& rString) const
{
  rString = rValue.GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiRect&>::FromString(nuiRect& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
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
bool nuiAttribute<nuiBorder>::ToString(nuiBorder Value, nglString& rString) const
{
  rString = Value.GetValue();
  return true;
}

template <>
bool nuiAttribute<nuiBorder>::FromString(nuiBorder& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
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
bool nuiAttribute<const nuiBorder&>::ToString(const nuiBorder& rValue, nglString& rString) const
{
  rString = rValue.GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiBorder&>::FromString(nuiBorder& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
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
bool nuiAttribute<nglPath>::ToString(nglPath Value, nglString& rString) const
{
  rString = Value.GetPathName();
  return true;
}

template <>
bool nuiAttribute<nglPath>::FromString(nglPath& rValue, const nglString& rString) const
{
  rValue = rString;
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
bool nuiAttribute<const nglPath&>::ToString(const nglPath& rValue, nglString& rString) const
{
  rString = rValue.GetPathName();
  return true;
}

template <>
bool nuiAttribute<const nglPath&>::FromString(nglPath& rValue, const nglString& rString) const
{
  rValue = rString;
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
bool nuiAttribute<nuiDecorationMode>::ToString(nuiDecorationMode Value, nglString& rString) const
{
  switch (Value)
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
  return true;
}

template <>
bool nuiAttribute<nuiDecorationMode>::FromString(nuiDecorationMode& rValue, const nglString& rString) const
{
  if (!rString.Compare(_T("Overdraw"), false))
  {
    rValue = eDecorationOverdraw;
    return true;
  }
  else if (!rString.Compare(_T("Border"), false))
  {
    rValue = eDecorationBorder;
    return true;
  }
  else if (!rString.Compare(_T("ClientOnly"), false))
  {
    rValue = eDecorationClientOnly;
    return true;
  }

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
bool nuiAttribute<nuiShapeMode>::ToString(nuiShapeMode Value, nglString& rString) const
{
  switch (Value)
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
bool nuiAttribute<nuiShapeMode>::FromString(nuiShapeMode& rValue, const nglString& rString) const
{
  if (!rString.Compare(_T("Stroke"), false))
  {
    rValue = eStrokeShape;
    return true;
  }
  else if (!rString.Compare(_T("Fill"), false))
  {
    rValue = eFillShape;
    return true;
  }
  else if (!rString.Compare(_T("StrokeAndFill"), false))
  {
    rValue = eStrokeAndFillShape;
    return true;
  }

  return false;
}




//********************************
//
// const nuiRange&
//

template class nuiAttribute<const nuiRange&>;


template <>
bool nuiAttribute<const nuiRange&>::ToString(const nuiRange& rValue, nglString& rString) const
{
  rString = rValue.GetValue();
  return true;
}

template <>
bool nuiAttribute<const nuiRange&>::FromString(nuiRange& rValue, const nglString& rString) const
{
  return rValue.FromString(rString);
}


template <>
nuiAttributeEditor* nuiAttribute<const nuiRange&>::GetDefaultEditor(void* pTarget)
{
//  switch (mUnit)
//  {
//    case nuiUnitRangeKnob:
//      return new nuiRangeKnobAttributeEditor(nuiAttrib<const nuiRange&>(pTarget, this));
//    default:      
//      return new nuiRangeAttributeEditor(nuiAttrib<const nuiRange&>(pTarget, this));
//  }
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
bool nuiAttribute<nuiDecorationLayer>::ToString(nuiDecorationLayer Value, nglString& rString) const
{
  if (Value == eLayerBack)
    rString = _T("Back");
  else
    rString = _T("Front");
  
  return true;
}

template <>
bool nuiAttribute<nuiDecorationLayer>::FromString(nuiDecorationLayer& rValue, const nglString& rString) const
{
  if (!rString.Compare(_T("Back"), false))
    rValue = eLayerBack;
  else if (!rString.Compare(_T("Front"), false))
    rValue = eLayerFront;
  else
    return false;
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
bool nuiAttribute<nuiMouseCursor>::ToString(nuiMouseCursor Value, nglString& rString) const
{
  switch(Value)
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
bool nuiAttribute<nuiMouseCursor>::FromString(nuiMouseCursor& rValue, const nglString& rString) const
{
  nuiMouseCursor cursor = eCursorNone;
  
  if (!rString.Compare(_T("DoNotSet"), false))
    rValue = eCursorDoNotSet;
  else if (!rString.Compare(_T("None"), false))
    rValue = eCursorNone;
  else if (!rString.Compare(_T("Arrow"), false))
    rValue = eCursorArrow;
  else if (!rString.Compare(_T("Cross"), false))
    rValue = eCursorCross;
  else if (!rString.Compare(_T("IBeam"), false))
    rValue = eCursorIBeam;
  else if (!rString.Compare(_T("Hand"), false))
    rValue = eCursorHand;
  else if (!rString.Compare(_T("ClosedHand"), false))
    rValue = eCursorClosedHand;
  else if (!rString.Compare(_T("Help"), false))
    rValue = eCursorHelp;
  else if (!rString.Compare(_T("Wait"), false))
    rValue = eCursorWait;
  else if (!rString.Compare(_T("Caret"), false))
    rValue = eCursorCaret;
  else if (!rString.Compare(_T("DnD"), false))
    rValue = eCursorDnD;
  else if (!rString.Compare(_T("Forbid"), false))
    rValue = eCursorForbid;
  else if (!rString.Compare(_T("Move"), false))
    rValue = eCursorMove;
  else if (!rString.Compare(_T("Resize"), false))
    rValue = eCursorResize;
  else if (!rString.Compare(_T("ResizeNS"), false))
    rValue = eCursorResizeNS;
  else if (!rString.Compare(_T("ResizeWE"), false))
    rValue = eCursorResizeWE;
  else if (!rString.Compare(_T("ResizeN"), false))
    rValue = eCursorResizeN;
  else if (!rString.Compare(_T("ResizeS"), false))
    rValue = eCursorResizeS;
  else if (!rString.Compare(_T("ResizeW"), false))
    rValue = eCursorResizeW;
  else if (!rString.Compare(_T("ResizeE"), false))
    rValue = eCursorResizeE;
  else if (!rString.Compare(_T("ResizeNW"), false))
    rValue = eCursorResizeNW;
  else if (!rString.Compare(_T("ResizeNE"), false))
    rValue = eCursorResizeNE;
  else if (!rString.Compare(_T("ResizeSW"), false))
    rValue = eCursorResizeSW;
  else if (!rString.Compare(_T("ResizeSE"), false))
    rValue = eCursorResizeSE;
  else
    return false;
  
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

//********************************
//
// nglVectorf
//

template class nuiAttribute<nglVectorf>;

template <>
bool nuiAttribute<nglVectorf>::ToString(nglVectorf Value, nglString& rString) const
{
  return Value.GetValue(rString);
}

template <>
bool nuiAttribute<nglVectorf>::FromString(nglVectorf& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
}

template <>
void nuiAttribute<nglVectorf>::FormatDefault(void* pTarget, nglString & string)
{
  Get(pTarget).GetValue(string);
}




//********************************
//
// const nglVectorf&
//

template class nuiAttribute<const nglVectorf&>;

template <>
void nuiAttribute<const nglVectorf&>::FormatDefault(void* pTarget, nglString & string)
{
  Get(pTarget).GetValue(string);
}

template <>
bool nuiAttribute<const nglVectorf&>::ToString(const nglVectorf& rValue, nglString& rString) const
{
  return rValue.GetValue(rString);
}

template <>
bool nuiAttribute<const nglVectorf&>::FromString(nglVectorf& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
}



//********************************
//
// nglMatrixf
//

template class nuiAttribute<nglMatrixf>;

template <>
bool nuiAttribute<nglMatrixf>::ToString(nglMatrixf Value, nglString& rString) const
{
  return Value.GetValue(rString);
}

template <>
bool nuiAttribute<nglMatrixf>::FromString(nglMatrixf& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
}

template <>
void nuiAttribute<nglMatrixf>::FormatDefault(void* pTarget, nglString & string)
{
  Get(pTarget).GetValue(string);
}




//********************************
//
// const nglMatrixf&
//

template class nuiAttribute<const nglMatrixf&>;

template <>
void nuiAttribute<const nglMatrixf&>::FormatDefault(void* pTarget, nglString & string)
{
  Get(pTarget).GetValue(string);
}

template <>
bool nuiAttribute<const nglMatrixf&>::ToString(const nglMatrixf& rValue, nglString& rString) const
{
  return rValue.GetValue(rString);
}

template <>
bool nuiAttribute<const nglMatrixf&>::FromString(nglMatrixf& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
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

bool nuiAttribBase::CanSet() const
{
  return mpAttributeBase->CanSet();
}

bool nuiAttribBase::CanGet() const
{
  return mpAttributeBase->CanGet();
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

bool nuiAttribBase::ToString(uint32 index, nglString& rString) const
{
  return mpAttributeBase->ToString(mpTarget, index, rString);
}

bool nuiAttribBase::FromString(uint32 index, const nglString& rString) const
{
  return mpAttributeBase->FromString(mpTarget, index, rString);
}

bool nuiAttribBase::ToString(uint32 index0, uint32 index1, nglString& rString) const
{
  return mpAttributeBase->ToString(mpTarget, index0, index1, rString);
}

bool nuiAttribBase::FromString(uint32 index0, uint32 index1, const nglString& rString) const
{
  return mpAttributeBase->FromString(mpTarget, index0, index1, rString);
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

uint32 nuiAttribBase::GetIndexRange(uint32 Dimension) const
{
  return mpAttributeBase->GetIndexRange(mpTarget, Dimension);
}

uint32 nuiAttribBase::GetDimension() const
{
  return mpAttributeBase->GetDimension();
}

