/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

uint32 nuiFakeGetRange1(uint32 dimension)
{
  if (dimension == 0)
    return 1;
  return 0;
}


uint64 nuiGetNewAttributeUniqueId()
{
  static uint64 IdCounter = 0;
  return IdCounter++;
}

#ifndef _MINUI3_
nuiAttributeEditor* nuiCreateGenericAttributeEditor(void* pTarget, nuiAttributeBase* pAttribute)
{
  return new nuiGenericAttributeEditor(nuiAttribBase(pTarget, pAttribute));
}

template <class Type>
nuiAttributeEditor* nuiCreateGenericAttributeEditor(void* pTarget, nuiAttributeBase* pAttribute)
{
  const nuiRange& rRange = pAttribute->GetRange();
  if (rRange.IsValid()) // Edit with a knob if at all possible
    return new nuiClampedValueAttributeEditor<Type>(nuiAttrib<Type>(nuiAttribBase(pTarget, pAttribute)), rRange);
  return new nuiGenericAttributeEditor(nuiAttribBase(pTarget, pAttribute));
}
#endif

nuiAttributeBase::nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit unit, const nuiRange& rRange, bool readonly, bool writeonly, Kind kind, void* pOffset)
: mName(rName),
  mType(type),
  mUnit(unit),
  mReadOnly(readonly),
  mWriteOnly(writeonly),
  mIgnoreAttributeChange(false),
  mInstanceAttribute(false),
  mRange(rRange),
  mOrder(0),
  mDimension(0),
  mKind(kind),
  mOffset(pOffset),
  mRangeGetter(nuiFakeGetRange1)
{
}

nuiAttributeBase::nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit unit, const nuiRange& rRange, bool readonly, bool writeonly, uint32 dimension, const ArrayRangeDelegate& rRangeGetter, Kind kind, void* pOffset)
: mName(rName),
  mType(type),
  mUnit(unit),
  mReadOnly(readonly),
  mWriteOnly(writeonly),
  mIgnoreAttributeChange(false),
  mInstanceAttribute(false),
  mRange(rRange),
  mOrder(0),
  mDimension(dimension),
  mRangeGetter(rRangeGetter),
  mKind(kind),
  mOffset(pOffset)
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
  if (!IsInstanceAttribute() && (getter != (ArrayRangeDelegate)nuiFakeGetRange1))
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

nuiSimpleEventSource<0>& nuiAttributeBase::GetChangedEvent(void* pTarget) const
{
  AttributeEventMap::iterator it = mAttributeChangedEvents.find(pTarget);
  if (it == mAttributeChangedEvents.end())
  {
    nuiSimpleEventSource<0>* pS = new nuiSimpleEventSource<0>;
    mAttributeChangedEvents[pTarget] = pS;
    return *pS;
  }
  return *(it->second);
}

void nuiAttributeBase::SetAsInstanceAttribute(bool set)
{
  mInstanceAttribute = set;
}

bool nuiAttributeBase::IsInstanceAttribute() const
{
  return mInstanceAttribute;
}

void nuiAttributeBase::KillAttributeHolder(void* pHolder)
{
  // Events:
  {
    AttributeEventMap::iterator it  = mAttributeChangedEvents.find(pHolder);
    AttributeEventMap::iterator end = mAttributeChangedEvents.end();
    if (it != end)
    {
      delete it->second;
      mAttributeChangedEvents.erase(it);
    }
  }
}

nuiAttributeBase::Kind nuiAttributeBase::GetKind() const
{
  return mKind;
}

void* nuiAttributeBase::GetOffset() const
{
  return mOffset;
}

bool nuiAttributeBase::IsValid(void* pTarget) const
{
  if (!pTarget) // We just want a general test about the availability of the Getter
    return true;

  switch (GetDimension())
  {
    case 0:
      return true;
      break;
    case 1:
      return (GetIndexRange(pTarget, 0) > 0);
      break;
    case 2:
      return (GetIndexRange(pTarget, 0) > 0) && (GetIndexRange(pTarget, 1) > 0);
      break;
  }
  return false; // Any other dimension is an error!
}


//////////////////
// Declaration of some property types specializations:
//template<uint32> nuiAttributeTypeTrait<uint32> nuiAttributeTypeTrait<uint32>::singleton;

NUI_DECLARE_ATTRIBUTE_TYPE(bool);

NUI_DECLARE_ATTRIBUTE_TYPE(int8);
NUI_DECLARE_ATTRIBUTE_TYPE(int16);
NUI_DECLARE_ATTRIBUTE_TYPE(int32);
NUI_DECLARE_ATTRIBUTE_TYPE(int64);

NUI_DECLARE_ATTRIBUTE_TYPE(uint8);
NUI_DECLARE_ATTRIBUTE_TYPE(uint16);
NUI_DECLARE_ATTRIBUTE_TYPE(uint32);
NUI_DECLARE_ATTRIBUTE_TYPE(uint64);

NUI_DECLARE_ATTRIBUTE_TYPE(float);
NUI_DECLARE_ATTRIBUTE_TYPE(double);

NUI_DECLARE_ATTRIBUTE_TYPE(nglString);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nglString&);

#ifndef _MINUI3_
NUI_DECLARE_ATTRIBUTE_TYPE(nuiPoint);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiRect);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nuiRect&);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiBorder);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nuiBorder&);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiColor);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nuiColor&);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiRange);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nuiRange&);
NUI_DECLARE_ATTRIBUTE_TYPE(nglVectorf);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nglVectorf&);
NUI_DECLARE_ATTRIBUTE_TYPE(nglVector3f);
NUI_DECLARE_ATTRIBUTE_TYPE(nglMatrixf);
//NUI_DECLARE_ATTRIBUTE_TYPE(const nglMatrixf&);
NUI_DECLARE_ATTRIBUTE_TYPE(nglQuaternionf);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiBlendFunc);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiExpandMode);
#endif

template <typename T>
uint64 nuiAttributeTypeTrait<T>::mTypeId = nuiGetNewAttributeUniqueId();


//********************************************************************************************************
//
// TEMPLATE TRAITS SPECIALIZATION : GetDefaultEditor
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

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<bool>::GetDefaultEditor(void* pTarget)
{
	return new nuiBooleanAttributeEditor(nuiAttrib<bool>(pTarget, this));
}
#endif

template <>
void nuiAttribute<bool>::FormatDefault(bool value, nglString& rString) const
{
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

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<int8>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<int8>(pTarget, this);
}
#endif

template <>
void nuiAttribute<int8>::FormatDefault(int8 value, nglString& string) const
{
  string.SetCInt(value);

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
  if (rString.IsInt())
  {
    rValue = rString.GetCInt();
    return true;
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    rValue = ToZero(dValue);
    return true;
  }

  return false;
}



//********************************
//
// int16
//

template class nuiAttribute<int16>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<int16>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<int16>(pTarget, this);
}
#endif

template <>
void nuiAttribute<int16>::FormatDefault(int16 value, nglString & string) const
{
  string.SetCInt(value);

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
  if (rString.IsInt())
  {
    rValue = rString.GetCInt();
    return true;
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    rValue = ToZero(dValue);
    return true;
  }

  return false;
}




//********************************
//
// int32
//

template class nuiAttribute<int32>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<int32>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<int32>(pTarget, this);
}
#endif

template <>
void nuiAttribute<int32>::FormatDefault(int32 value, nglString & string) const
{
  string.SetCInt(value);

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
  if (rString.IsInt())
  {
    rValue = rString.GetCInt();
    return true;
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    rValue = ToZero(dValue);
    return true;
  }

  return false;
}



//********************************
//
// int64
//

template class nuiAttribute<int64>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<int64>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<int64>(pTarget, this);
}
#endif

template <>
void nuiAttribute<int64>::FormatDefault(int64 value, nglString & string) const
{
  string.SetCInt(value);

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
  if (rString.IsInt())
  {
    rValue = rString.GetCInt64();
    return true;
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    rValue = ToZero(dValue);
    return true;
  }

  return false;
}




//********************************
//
// uint8
//

template class nuiAttribute<uint8>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<uint8>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<uint8>(pTarget, this);
}
#endif

template <>
void nuiAttribute<uint8>::FormatDefault(uint8 value, nglString & string) const
{
  string.SetCInt(value);

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
  if (rString.IsInt())
  {
    int32 iValue = rString.GetCInt();
    if (iValue >= 0)
    {
      rValue = iValue;
      return true;
    }
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    if (dValue >= 0)
    {
      rValue = ToZero(dValue);
      return true;
    }
  }

  return false;
}



//********************************
//
// uint16
//

template class nuiAttribute<uint16>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<uint16>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<uint16>(pTarget, this);
}
#endif

template <>
void nuiAttribute<uint16>::FormatDefault(uint16 value, nglString & string) const
{
  string.SetCInt(value);

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
  if (rString.IsInt())
  {
    int32 iValue = rString.GetCInt();
    if (iValue >= 0)
    {
      rValue = iValue;
      return true;
    }
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    if (dValue >= 0)
    {
      rValue = ToZero(dValue);
      return true;
    }
  }

  return false;
}




//********************************
//
// uint32
//

template class nuiAttribute<uint32>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<uint32>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<uint32>(pTarget, this);
}
#endif

template <>
void nuiAttribute<uint32>::FormatDefault(uint32 value, nglString & string) const
{
  string.SetCInt(value);
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
  if (rString.IsInt())
  {
    int32 iValue = rString.GetCInt();
    if (iValue >= 0)
    {
      rValue = iValue;
      return true;
    }
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    if (dValue >= 0)
    {
      rValue = ToZero(dValue);
      return true;
    }
  }

  return false;
}




//********************************
//
// uint64
//

template class nuiAttribute<uint64>;


#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<uint64>::GetDefaultEditor(void* pTarget)
{
	//#FIXME TODO
	// if this code is executed, it means a case processing is missing
	return nuiCreateGenericAttributeEditor<uint64>(pTarget, this);
}
#endif

template <>
void nuiAttribute<uint64>::FormatDefault(uint64 value, nglString & string) const
{
  string.SetCInt(value);
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
  if (rString.IsInt())
  {
    int64 iValue = rString.GetCInt64();
    if (iValue >= 0)
    {
      rValue = iValue;
      return true;
    }
  }
  else if (rString.IsFloat())
  {
    double dValue = rString.GetCDouble();
    if (dValue >= 0)
    {
      rValue = ToZero(dValue);
      return true;
    }
  }

  return false;
}



//********************************
//
// float
//

template class nuiAttribute<float>;

#ifndef _MINUI3_
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
	return nuiCreateGenericAttributeEditor<float>(pTarget, this);
}
#endif


template <>
void nuiAttribute<float>::FormatDefault(float value, nglString & string) const
{
  string.SetCFloat(value);
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

#ifndef _MINUI3_
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
  return nuiCreateGenericAttributeEditor<double>(pTarget, this);
}
#endif

template <>
void nuiAttribute<double>::FormatDefault(double value, nglString & string) const
{
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


#ifndef _MINUI3_

//********************************
//
// nuiPosition
//

template class nuiAttribute<nuiPosition>;

template <>
nuiAttributeEditor* nuiAttribute<nuiPosition>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiPosition> > values;
  values.push_back(std::make_pair(_T("Left"), nuiLeft));
  values.push_back(std::make_pair(_T("Right"), nuiRight));
  values.push_back(std::make_pair(_T("Top"), nuiTop));
  values.push_back(std::make_pair(_T("Bottom"), nuiBottom));
  values.push_back(std::make_pair(_T("Center"), nuiCenter));
  values.push_back(std::make_pair(_T("Fill"), nuiFill));
  values.push_back(std::make_pair(_T("Fill Horizontal"), nuiFillHorizontal));
  values.push_back(std::make_pair(_T("Fill Vertical"), nuiFillVertical));
  values.push_back(std::make_pair(_T("Fill Left"), nuiFillLeft));
  values.push_back(std::make_pair(_T("Fill Right"), nuiFillRight));
  values.push_back(std::make_pair(_T("Fill Top"), nuiFillTop));
  values.push_back(std::make_pair(_T("Fill Bottom"), nuiFillBottom));
  values.push_back(std::make_pair(_T("Top Left"), nuiTopLeft));
  values.push_back(std::make_pair(_T("Top Right"), nuiTopRight));
  values.push_back(std::make_pair(_T("Bottom Left"), nuiBottomLeft));
  values.push_back(std::make_pair(_T("Bottom Right"), nuiBottomRight));
  values.push_back(std::make_pair(_T("Tile"), nuiTile));
  values.push_back(std::make_pair(_T("No Position"), nuiNoPosition));
  return new nuiComboAttributeEditor<nuiPosition>(nuiAttrib<nuiPosition>(pTarget, this), values);
}

template <>
void nuiAttribute<nuiPosition>::FormatDefault(nuiPosition value, nglString & string) const
{
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
// nuiOrientation
//

template class nuiAttribute<nuiOrientation>;

template <>
nuiAttributeEditor* nuiAttribute<nuiOrientation>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiOrientation> > values;
  values.push_back(std::make_pair(_T("Horizontal"), nuiHorizontal));
  values.push_back(std::make_pair(_T("Vertical"), nuiVertical));
  return new nuiComboAttributeEditor<nuiOrientation>(nuiAttrib<nuiOrientation>(pTarget, this), values);
}

template <>
void nuiAttribute<nuiOrientation>::FormatDefault(nuiOrientation value, nglString & string) const
{
  string = nuiGetOrientation(value);
}


template <>
bool nuiAttribute<nuiOrientation>::ToString(nuiOrientation Value, nglString& rString) const
{
  rString = nuiGetOrientation(Value);
  return true;
}

template <>
bool nuiAttribute<nuiOrientation>::FromString(nuiOrientation& rValue, const nglString& rString) const
{
  rValue = nuiGetOrientation(rString);
  return true;
}



//********************************
//
// nuiDirection
//

template class nuiAttribute<nuiDirection>;

template <>
nuiAttributeEditor* nuiAttribute<nuiDirection>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiDirection> > values;
  values.push_back(std::make_pair(_T("Forward"), nuiForward));
  values.push_back(std::make_pair(_T("Backward"), nuiBackward));
  return new nuiComboAttributeEditor<nuiDirection>(nuiAttrib<nuiDirection>(pTarget, this), values);
}

template <>
void nuiAttribute<nuiDirection>::FormatDefault(nuiDirection value, nglString & string) const
{
  string = nuiGetDirection(value);
}


template <>
bool nuiAttribute<nuiDirection>::ToString(nuiDirection Value, nglString& rString) const
{
  rString = nuiGetDirection(Value);
  return true;
}

template <>
bool nuiAttribute<nuiDirection>::FromString(nuiDirection& rValue, const nglString& rString) const
{
  rValue = nuiGetDirection(rString);
  return true;
}

#endif /////////////








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


#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<nglString>::GetDefaultEditor(void* pTarget)
{
  return new nuiStringAttributeEditor(nuiAttrib<nglString>(pTarget, this));
}
#endif

template <>
void nuiAttribute<nglString>::FormatDefault(nglString value, nglString & string) const
{
  string = value;
}




//********************************
//
// const nglString&
//

template class nuiAttribute<const nglString&>;

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<const nglString&>::GetDefaultEditor(void* pTarget)
{
  return new nuiStringAttributeEditor(nuiAttrib<const nglString&>(pTarget, this));
}
#endif

template <>
void nuiAttribute<const nglString&>::FormatDefault(const nglString& rValue, nglString & string) const
{
  string = rValue;
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



#ifndef _MINUI3_


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


#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<nuiColor>::GetDefaultEditor(void* pTarget)
{
  return new nuiColorAttributeEditor(nuiAttrib<nuiColor>(pTarget, this));
}
#endif

template <>
void nuiAttribute<nuiColor>::FormatDefault(nuiColor value, nglString & string) const
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

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<const nuiColor&>::GetDefaultEditor(void* pTarget)
{
  return new nuiColorAttributeEditor(nuiAttrib<const nuiColor&>(pTarget, this));
}
#endif

template <>
void nuiAttribute<const nuiColor&>::FormatDefault(const nuiColor& rValue, nglString & string) const
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
void nuiAttribute<nuiPoint>::FormatDefault(nuiPoint value, nglString & string) const
{
  string.CFormat(_T("X: %.0fpx\nY: %.0fpx"), value[0], value[1]);
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
void nuiAttribute<nuiRect>::FormatDefault(nuiRect value, nglString & string) const
{
  string.CFormat(_T("(%.0f,%.0f) : (%.0f x %0.f)"), value.Left(), value.Top(), value.GetWidth(), value.GetHeight());
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
void nuiAttribute<const nuiRect&>::FormatDefault(const nuiRect& rValue, nglString & string) const
{
  string.CFormat(_T("(%.0f,%.0f) : (%.0f x %0.f)"), rValue.Left(), rValue.Top(), rValue.GetWidth(), rValue.GetHeight());
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
void nuiAttribute<nuiBorder>::FormatDefault(nuiBorder value, nglString & string) const
{
  string.CFormat(_T("(%.0f,%.0f,%.0f,%0.f)"), value.Left(), value.Top(), value.Right(), value.Bottom());
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
void nuiAttribute<const nuiBorder&>::FormatDefault(const nuiBorder& rValue, nglString & string) const
{
  string.CFormat(_T("(%.0f,%.0f,%.0f,%0.f)"), rValue.Left(), rValue.Top(), rValue.Right(), rValue.Bottom());
}

#endif






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
void nuiAttribute<nglPath>::FormatDefault(nglPath value, nglString & string) const
{
  string = value.GetPathName();
}




//********************************
//
// const nglPath&
//

template class nuiAttribute<const nglPath&>;

template <>
void nuiAttribute<const nglPath&>::FormatDefault(const nglPath& rValue, nglString & string) const
{
  string = rValue.GetPathName();
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



#ifndef _MINUI3_


//********************************
//
// DecorationMode
//

template class nuiAttribute<nuiDecorationMode>;

template <>
void nuiAttribute<nuiDecorationMode>::FormatDefault(nuiDecorationMode value, nglString & string) const
{
  switch (value)
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


template <>
nuiAttributeEditor* nuiAttribute<nuiDecorationMode>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiDecorationMode> > values;
  values.push_back(std::make_pair(_T("Overdraw"), eDecorationOverdraw));
  values.push_back(std::make_pair(_T("Border"), eDecorationBorder));
  values.push_back(std::make_pair(_T("Client Only"), eDecorationClientOnly));
  return new nuiComboAttributeEditor<nuiDecorationMode>(nuiAttrib<nuiDecorationMode>(pTarget, this), values);
}





//********************************
//
// ShapeMode
//

template class nuiAttribute<nuiShapeMode>;

template <>
void nuiAttribute<nuiShapeMode>::FormatDefault(nuiShapeMode value, nglString & string) const
{
  switch (value)
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


template <>
nuiAttributeEditor* nuiAttribute<nuiShapeMode>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiShapeMode> > values;
  values.push_back(std::make_pair(_T("Stroke"), eStrokeShape));
  values.push_back(std::make_pair(_T("Fill"), eFillShape));
  values.push_back(std::make_pair(_T("Stroke and Fill"), eStrokeAndFillShape));
  return new nuiComboAttributeEditor<nuiShapeMode>(nuiAttrib<nuiShapeMode>(pTarget, this), values);
}




//********************************
//
// const nuiRange&
//

template class nuiAttribute<const nuiRange&>;


template <>
bool nuiAttribute<const nuiRange&>::ToString(const nuiRange& rValue, nglString& rString) const
{
  rString.SetCDouble(rValue.GetValue());
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
  if (GetUnit() == nuiUnitRangeKnob)
    return new nuiRangeKnobAttributeEditor(nuiAttrib<const nuiRange&>(pTarget, this));
  else
    return new nuiRangeAttributeEditor(nuiAttrib<const nuiRange&>(pTarget, this));
}


template <>
void nuiAttribute<const nuiRange&>::FormatDefault(const nuiRange& rValue, nglString & string) const
{
  nuiRange range = rValue;
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
  std::vector<std::pair<nglString, nuiDecorationLayer> > values;
  values.push_back(std::make_pair(_T("Back"), eLayerBack));
  values.push_back(std::make_pair(_T("Front"), eLayerFront));
  return new nuiComboAttributeEditor<nuiDecorationLayer>(nuiAttrib<nuiDecorationLayer>(pTarget, this), values);
}

template <>
void nuiAttribute<nuiDecorationLayer>::FormatDefault(nuiDecorationLayer value, nglString & string) const
{
  if (value == eLayerBack)
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
    case eCursorPointingHand:
      rString = _T("PointingHand");
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
  rValue = eCursorArrow;

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
  else if (!rString.Compare(_T("PointingHand"), false))
    rValue = eCursorPointingHand;
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
  std::vector<std::pair<nglString, nuiMouseCursor> > values;
  values.push_back(std::make_pair(_T("Do not set"), eCursorDoNotSet));
  values.push_back(std::make_pair(_T("None"), eCursorNone));
  values.push_back(std::make_pair(_T("Arrow"), eCursorArrow));
  values.push_back(std::make_pair(_T("Cross"), eCursorCross));
  values.push_back(std::make_pair(_T("IBeam"), eCursorIBeam));
  values.push_back(std::make_pair(_T("Hand"), eCursorHand));
  values.push_back(std::make_pair(_T("ClosedHand"), eCursorClosedHand));
  values.push_back(std::make_pair(_T("PointingHand"), eCursorPointingHand));
  values.push_back(std::make_pair(_T("Help"), eCursorHelp));
  values.push_back(std::make_pair(_T("Wait"), eCursorWait));
  values.push_back(std::make_pair(_T("Caret"), eCursorCaret));
  values.push_back(std::make_pair(_T("DnD"), eCursorDnD));
  values.push_back(std::make_pair(_T("Forbid"), eCursorForbid));
  values.push_back(std::make_pair(_T("Move"), eCursorMove));
  values.push_back(std::make_pair(_T("Resize"), eCursorResize));
  values.push_back(std::make_pair(_T("ResizeNS"), eCursorResizeNS));
  values.push_back(std::make_pair(_T("ResizeWE"), eCursorResizeWE));
  values.push_back(std::make_pair(_T("ResizeN"), eCursorResizeN));
  values.push_back(std::make_pair(_T("ResizeS"), eCursorResizeS));
  values.push_back(std::make_pair(_T("ResizeW"), eCursorResizeW));
  values.push_back(std::make_pair(_T("ResizeE"), eCursorResizeE));
  values.push_back(std::make_pair(_T("ResizeNW"), eCursorResizeNW));
  values.push_back(std::make_pair(_T("ResizeNE"), eCursorResizeNE));
  values.push_back(std::make_pair(_T("ResizeSW"), eCursorResizeSW));
  values.push_back(std::make_pair(_T("ResizeSE"), eCursorResizeSE));
  return new nuiComboAttributeEditor<nuiMouseCursor>(nuiAttrib<nuiMouseCursor>(pTarget, this), values);
}



template <>
void nuiAttribute<nuiMouseCursor>::FormatDefault(nuiMouseCursor value, nglString& string) const
{
  ToString(value, string);
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
void nuiAttribute<nglVectorf>::FormatDefault(nglVectorf value, nglString & string) const
{
  value.GetValue(string);
}




//********************************
//
// const nglVectorf&
//

template class nuiAttribute<const nglVectorf&>;

template <>
void nuiAttribute<const nglVectorf&>::FormatDefault(const nglVectorf& rValue, nglString & string) const
{
  rValue.GetValue(string);
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
// nglVector3f
//

template class nuiAttribute<nglVector3f>;

template <>
bool nuiAttribute<nglVector3f>::ToString(nglVector3f Value, nglString& rString) const
{
  return Value.GetValue(rString);
}

template <>
bool nuiAttribute<nglVector3f>::FromString(nglVector3f& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
}

template <>
void nuiAttribute<nglVector3f>::FormatDefault(nglVector3f value, nglString & string) const
{
  value.GetValue(string);
}




//********************************
//
// const nglVector3f&
//

template class nuiAttribute<const nglVector3f&>;

template <>
void nuiAttribute<const nglVector3f&>::FormatDefault(const nglVector3f& rValue, nglString & string) const
{
  rValue.GetValue(string);
}

template <>
bool nuiAttribute<const nglVector3f&>::ToString(const nglVector3f& rValue, nglString& rString) const
{
  return rValue.GetValue(rString);
}

template <>
bool nuiAttribute<const nglVector3f&>::FromString(nglVector3f& rValue, const nglString& rString) const
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
void nuiAttribute<nglMatrixf>::FormatDefault(nglMatrixf value, nglString & string) const
{
  value.GetValue(string);
}




//********************************
//
// const nglMatrixf&
//

template class nuiAttribute<const nglMatrixf&>;

template <>
void nuiAttribute<const nglMatrixf&>::FormatDefault(const nglMatrixf& rValue, nglString & string) const
{
  rValue.GetValue(string);
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

//********************************
//
// nglQuaternionf
//

template class nuiAttribute<nglQuaternionf>;

template <>
bool nuiAttribute<nglQuaternionf>::ToString(nglQuaternionf Value, nglString& rString) const
{
  return Value.GetValue(rString);
}

template <>
bool nuiAttribute<nglQuaternionf>::FromString(nglQuaternionf& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
}

template <>
void nuiAttribute<nglQuaternionf>::FormatDefault(nglQuaternionf value, nglString & string) const
{
  value.GetValue(string);
}




//********************************
//
// const nglQuaternionf&
//

template class nuiAttribute<const nglQuaternionf&>;

template <>
void nuiAttribute<const nglQuaternionf&>::FormatDefault(const nglQuaternionf& rValue, nglString & string) const
{
  rValue.GetValue(string);
}

template <>
bool nuiAttribute<const nglQuaternionf&>::ToString(const nglQuaternionf& rValue, nglString& rString) const
{
  return rValue.GetValue(rString);
}

template <>
bool nuiAttribute<const nglQuaternionf&>::FromString(nglQuaternionf& rValue, const nglString& rString) const
{
  return rValue.SetValue(rString);
}



#endif


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


// To/From String
bool nuiAttribBase::ToString(nglString& rString) const
{
  return mpAttributeBase->ToString(mpTarget, 0, 0, rString);
}

bool nuiAttribBase::FromString(const nglString& rString) const
{
  return mpAttributeBase->FromString(mpTarget, 0, 0, rString);
}

bool nuiAttribBase::ToString(uint32 index, nglString& rString) const
{
  return mpAttributeBase->ToString(mpTarget, index, 0, rString);
}

bool nuiAttribBase::FromString(uint32 index, const nglString& rString) const
{
  return mpAttributeBase->FromString(mpTarget, index, 0, rString);
}

bool nuiAttribBase::ToString(uint32 index0, uint32 index1, nglString& rString) const
{
  return mpAttributeBase->ToString(mpTarget, index0, index1, rString);
}

bool nuiAttribBase::FromString(uint32 index0, uint32 index1, const nglString& rString) const
{
  return mpAttributeBase->FromString(mpTarget, index0, index1, rString);
}

// To/From Variant
bool nuiAttribBase::ToVariant(nuiVariant& rVariant) const
{
  return mpAttributeBase->ToVariant(mpTarget, 0, 0, rVariant);
}

bool nuiAttribBase::FromVariant(const nuiVariant& rVariant) const
{
  return mpAttributeBase->FromVariant(mpTarget, 0, 0, rVariant);
}

bool nuiAttribBase::ToVariant(uint32 index, nuiVariant& rVariant) const
{
  return mpAttributeBase->ToVariant(mpTarget, index, 0, rVariant);
}

bool nuiAttribBase::FromVariant(uint32 index, const nuiVariant& rVariant) const
{
  return mpAttributeBase->FromVariant(mpTarget, index, 0, rVariant);
}

bool nuiAttribBase::ToVariant(uint32 index0, uint32 index1, nuiVariant& rVariant) const
{
  return mpAttributeBase->ToVariant(mpTarget, index0, index1, rVariant);
}

bool nuiAttribBase::FromVariant(uint32 index0, uint32 index1, const nuiVariant& rVariant) const
{
  return mpAttributeBase->FromVariant(mpTarget, index0, index1, rVariant);
}



// Format
void nuiAttribBase::Format(nglString& rString) const
{
  mpAttributeBase->Format(mpTarget, 0, 0, rString);
}

void nuiAttribBase::Format(uint32 index, nglString& rString) const
{
  mpAttributeBase->Format(mpTarget, index, 0, rString);
}

void nuiAttribBase::Format(uint32 index0, uint32 index1, nglString& rString) const
{
  mpAttributeBase->Format(mpTarget, index0, index1, rString);
}



void nuiAttribBase::IgnoreAttributeChange(bool ignore)
{
  mpAttributeBase->IgnoreAttributeChange(ignore);
}


bool nuiAttribBase::IsAttributeChangeIgnored() const
{
  return mpAttributeBase->IsAttributeChangeIgnored();
}


#ifndef _MINUI3_
nuiAttributeEditor* nuiAttribBase::GetEditor() const
{
  return mpAttributeBase->GetEditor(mpTarget);
}
#endif

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

nuiSimpleEventSource<0>& nuiAttribBase::GetChangedEvent()
{
  return mpAttributeBase->GetChangedEvent(mpTarget);
}







#ifndef _MINUI3_


//********************************
//
// BlendFunc
//

template class nuiAttribute<nuiBlendFunc>;

template <>
void nuiAttribute<nuiBlendFunc>::FormatDefault(nuiBlendFunc value, nglString & string) const
{
  ToString(value, string);
}

template <>
bool nuiAttribute<nuiBlendFunc>::ToString(nuiBlendFunc Value, nglString& rString) const
{
  switch (Value)
  {
    case nuiBlendSource:
      rString = _T("Source"); break;
    case nuiBlendTransp:
      rString = _T("Transp"); break;
    case nuiBlendClear:
      rString = _T("Clear"); break;
    case nuiBlendDest:
      rString = _T("Dest"); break;
    case nuiBlendOver:
      rString = _T("Over"); break;
    case nuiBlendOverRev:
      rString = _T("OverRev"); break;
    case nuiBlendIn:
      rString = _T("In"); break;
    case nuiBlendInRev:
      rString = _T("InRev"); break;
    case nuiBlendOut:
      rString = _T("Out"); break;
    case nuiBlendOutRev:
      rString = _T("OutRev"); break;
    case nuiBlendTop:
      rString = _T("Top"); break;
    case nuiBlendTopRev:
      rString = _T("TopRev"); break;
    case nuiBlendXOR:
      rString = _T("XOR"); break;
    case nuiBlendAdd:
      rString = _T("Add"); break;
    case nuiBlendSaturate:
      rString = _T("Saturate"); break;

    case nuiBlendTranspClear:
      rString = _T("Clear"); break;
    case nuiBlendTranspAdd:
      rString = _T("TranspAdd"); break;
    case nuiBlendTranspOver:
      rString = _T("TranspOver"); break;
    case nuiBlendTranspInRev:
      rString = _T("TranspInRev"); break;

    default:
      rString = _T("UnknownBlendFunc");
      return false;
  }

  return true;

}

template <>
bool nuiAttribute<nuiBlendFunc>::FromString(nuiBlendFunc& rValue, const nglString& rString) const
{
  if (!rString.Compare(_T("Source"), false))
  {
    rValue = nuiBlendSource;
    return true;
  }
  else if (!rString.Compare(_T("Transp"), false))
  {
    rValue = nuiBlendTransp;
    return true;
  }
  else if (!rString.Compare(_T("Clear"), false))
  {
    rValue = nuiBlendClear;
    return true;
  }
  else if (!rString.Compare(_T("Dest"), false))
  {
    rValue = nuiBlendDest;
    return true;
  }
  else if (!rString.Compare(_T("Over"), false))
  {
    rValue = nuiBlendOver;
    return true;
  }
  else if (!rString.Compare(_T("OverRev"), false))
  {
    rValue = nuiBlendOverRev;
    return true;
  }
  else if (!rString.Compare(_T("In"), false))
  {
    rValue = nuiBlendIn;
    return true;
  }
  else if (!rString.Compare(_T("InRev"), false))
  {
    rValue = nuiBlendInRev;
    return true;
  }
  else if (!rString.Compare(_T("Out"), false))
  {
    rValue = nuiBlendOut;
    return true;
  }
  else if (!rString.Compare(_T("OutRev"), false))
  {
    rValue = nuiBlendOutRev;
    return true;
  }
  else if (!rString.Compare(_T("Top"), false))
  {
    rValue = nuiBlendTop;
    return true;
  }
  else if (!rString.Compare(_T("TopRev"), false))
  {
    rValue = nuiBlendTopRev;
    return true;
  }
  else if (!rString.Compare(_T("XOR"), false))
  {
    rValue = nuiBlendXOR;
    return true;
  }
  else if (!rString.Compare(_T("Add"), false))
  {
    rValue = nuiBlendAdd;
    return true;
  }
  else if (!rString.Compare(_T("Saturate"), false))
  {
    rValue = nuiBlendSaturate;
    return true;
  }
  else if (!rString.Compare(_T("Clear"), false))
  {
    rValue = nuiBlendClear;
    return true;
  }
  else if (!rString.Compare(_T("TranspAdd"), false))
  {
    rValue = nuiBlendTranspAdd;
    return true;
  }
  else if (!rString.Compare(_T("TranspOver"), false))
  {
    rValue = nuiBlendTranspOver;
    return true;
  }
  else if (!rString.Compare(_T("TranspInRev"), false))
  {
    rValue = nuiBlendTranspInRev;
    return true;
  }

  rValue = nuiBlendTransp;
  return false;
}

template <>
nuiAttributeEditor* nuiAttribute<nuiBlendFunc>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiBlendFunc> > values;
  values.push_back(std::make_pair(_T("Source"), nuiBlendSource));
  values.push_back(std::make_pair(_T("Transp"), nuiBlendTransp));
  values.push_back(std::make_pair(_T("Clear"), nuiBlendClear));
  values.push_back(std::make_pair(_T("Dest"), nuiBlendDest));
  values.push_back(std::make_pair(_T("Over"), nuiBlendOver));
  values.push_back(std::make_pair(_T("OverRev"), nuiBlendOverRev));
  values.push_back(std::make_pair(_T("In"), nuiBlendIn));
  values.push_back(std::make_pair(_T("InRev"), nuiBlendInRev));
  values.push_back(std::make_pair(_T("Out"), nuiBlendOut));
  values.push_back(std::make_pair(_T("OutRev"), nuiBlendOutRev));
  values.push_back(std::make_pair(_T("Top"), nuiBlendTop));
  values.push_back(std::make_pair(_T("TopRev"), nuiBlendTopRev));
  values.push_back(std::make_pair(_T("XOR"), nuiBlendXOR));
  values.push_back(std::make_pair(_T("Add"), nuiBlendAdd));
  values.push_back(std::make_pair(_T("Saturate"), nuiBlendSaturate));
  values.push_back(std::make_pair(_T("Clear"), nuiBlendClear));
  values.push_back(std::make_pair(_T("TranspAdd"), nuiBlendTranspAdd));
  values.push_back(std::make_pair(_T("TranspOver"), nuiBlendTranspOver));
  values.push_back(std::make_pair(_T("TranspInRev"), nuiBlendTranspInRev));
  return new nuiComboAttributeEditor<nuiBlendFunc>(nuiAttrib<nuiBlendFunc>(pTarget, this), values);
}


//********************************
//
// ExpandMode
//

template class nuiAttribute<nuiExpandMode>;

template <>
void nuiAttribute<nuiExpandMode>::FormatDefault(nuiExpandMode value, nglString & string) const
{
  ToString(value, string);
}

template <>
bool nuiAttribute<nuiExpandMode>::ToString(nuiExpandMode Value, nglString& rString) const
{
  switch (Value)
  {
    case nuiExpandFixed:
      rString = _T("Fixed"); break;
    case nuiExpandGrow:
      rString = _T("Grow"); break;
    case nuiExpandShrink:
      rString = _T("Shrink"); break;
    case nuiExpandShrinkAndGrow:
      rString = _T("ShrinkAndGrow"); break;

    default:
      rString = _T("UnknownExpandMode");
      return false;
  }

  return true;

}

template <>
bool nuiAttribute<nuiExpandMode>::FromString(nuiExpandMode& rValue, const nglString& rString) const
{
  if (!rString.Compare(_T("Fixed"), false))
  {
    rValue = nuiExpandFixed;
    return true;
  }
  else if (!rString.Compare(_T("Grow"), false))
  {
    rValue = nuiExpandGrow;
    return true;
  }
  else if (!rString.Compare(_T("Shrink"), false))
  {
    rValue = nuiExpandShrink;
    return true;
  }
  else if (!rString.Compare(_T("ShrinkAndGrow"), false))
  {
    rValue = nuiExpandShrinkAndGrow;
    return true;
  }

  rValue = nuiExpandFixed;
  return false;
}

#ifndef _MINUI3_
template <>
nuiAttributeEditor* nuiAttribute<nuiExpandMode>::GetDefaultEditor(void* pTarget)
{
  std::vector<std::pair<nglString, nuiExpandMode> > values;
  values.push_back(std::make_pair(_T("Fixed"), nuiExpandFixed));
  values.push_back(std::make_pair(_T("Grow"), nuiExpandGrow));
  values.push_back(std::make_pair(_T("Shrink"), nuiExpandShrink));
  values.push_back(std::make_pair(_T("Shrink and Grow"), nuiExpandShrinkAndGrow));
  if (GetDimension() == 0)
    return new nuiComboAttributeEditor<nuiExpandMode>(nuiAttrib<nuiExpandMode>(pTarget, this), values);
  else
 		return nuiCreateGenericAttributeEditor(pTarget, this);
}
#endif
#endif

#if 0

// The primary template
template<class T>
class Stack
{
  T* data;
  std::size_t count;
  std::size_t capacity;
  enum { INIT = 5 };
public:
  Stack()
  {
    count = 0;
    capacity = INIT;
    data = new T[INIT];
  }

  void push(const T& t)
  {
    if(count == capacity)
    {
      // Grow array store
      std::size_t newCapacity = 2 * capacity;
      T* newData = new T[newCapacity];
      for(size_t i = 0; i < count; ++i)
        newData[i] = data[i];
      delete [] data;
      data = newData;
      capacity = newCapacity;
    }
    assert(count < capacity);
    data[count++] = t;
  }

  void pop()
  {
    assert(count > 0);
    --count;
  }

  T top() const
  {
    assert(count > 0);
    return data[count-1];
  }

  std::size_t size() const
  {
    return count;
  }
};

// Full specialization for void*
template<>
class Stack<void *>
{
  void** data;
  std::size_t count;
  std::size_t capacity;
  enum { INIT = 5 };
public:
  Stack()
  {
    count = 0;
    capacity = INIT;
    data = new void*[INIT];
  }

  void push(void* const & t)
  {
    if(count == capacity)
    {
      std::size_t newCapacity = 2*capacity;
      void** newData = new void*[newCapacity];
      std::memcpy(newData, data, count*sizeof(void*));
      delete [] data;
      data = newData;
      capacity = newCapacity;
    }
    assert(count < capacity);
    data[count++] = t;
  }

  void pop()
  {
    assert(count > 0);
    --count;
  }

  void* top() const
  {
    assert(count > 0);
    return data[count-1];
  }

  std::size_t size() const
  {
    return count;
  }
};

// Partial specialization for other pointer types
template<class T>
class Stack<T*> : private Stack<void *>
{
  typedef Stack<void *> Base;
public:
  void push(T* const & t)
  {
    Base::push(t);
  }

  void pop()
  {
    Base::pop();
  }

  T* top() const
  {
    return static_cast<T*>(Base::top());
  }

  std::size_t size()
  {
    return Base::size();
  }
};

#endif


///////////////////
// Attribute Wrapper class:
template <typename T>
class nuiWrapper
{
public:
  explicit nuiWrapper(T value)
  {
    mValue = value;
  }

  explicit nuiWrapper()
  {
  }

  nuiWrapper(const nuiWrapper<T>& value)
  {
    mValue = value;
  }

  operator T() const
  {
    return mValue;
  }

  T operator =(T value)
  {
    mValue = value;
    return mValue;
  }
private:
  T mValue;
};

void wrapper_test()
{
  nuiWrapper<float> f(0);
  nuiWrapper<float> ff(10);

  f = 10;
  f = 15.0;
  f = 25.0f;

  f = f * 5;
  f = f * 5.0;
  f = f * 5.0f;
}
