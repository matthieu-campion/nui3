/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiAttributeType.h"
#include "nuiTypeTraits.h"
#include "nuiBorder.h"
#include "nuiObject.h"
#include "nuiRange.h"
#include "nuiShape.h"
#include "nuiDecorationDefines.h"
#include "nuiMouseCursor.h"
#include "nuiRenderState.h"
#include "nuiEvent.h"

class nuiPoint;
class nuiRange;
class nuiWidget;
class nuiContainer;
class nuiSimpleContainer;
class nuiTopLevel;
class nuiMainWindow;
class nuiLabel;

#ifdef __clang__
template <> uint64 nuiAttributeTypeTrait<bool>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<int8>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<int16>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<int32>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<int64>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<uint8>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<uint16>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<uint32>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<uint64>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<float>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<double>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<nglString>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiPoint>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiRect>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiBorder>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiColor>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiRange>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiVector>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nglMatrixf>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiBlendFunc>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiExpandMode>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<void>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<void*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiObject*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiWidget*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiContainer*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiSimpleContainer*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiTopLevel*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiMainWindow*>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<nuiLabel*>::mTypeId;
#endif



class nuiVariant
{
public:
  nuiVariant()
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<void>::mTypeId;
  }
  
  // POD CTOR
  template <typename Type>
  nuiVariant(Type data)
  {
    mIsPointer = is_pointer<Type>::value;
    mIsObject = is_base_of<nuiObject, Type>::value;
    mType = nuiAttributeTypeTrait<Type>::mTypeId;
    mIsPOD = false;
    mIsArray = false;
    NGL_ASSERT(0);
  }

  template <typename Type>
  nuiVariant(const std::vector<Type>& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = 0;
    mIsPOD = false;
    mIsArray = true;
    mData.mpArray = new std::vector<nuiVariant>();
    mData.mpArray->reserve(data.size());
    for (int32 i = 0; i < data.size(); i++)
      mData.mpArray->push_back(data[i]);
  }
  
#define CTOR(TYPE)\
  nuiVariant(TYPE data)\
  {\
    mIsPointer = is_pointer<TYPE>::value;\
    mIsObject = is_base_of<nuiObject, TYPE>::value;\
    mType = nuiAttributeTypeTrait<TYPE>::mTypeId;\
    mIsPOD = true;\
    mIsArray = false;\
    if (is_same_type<TYPE, int8>::value || is_same_type<TYPE, int16>::value || is_same_type<TYPE, int32>::value || is_same_type<TYPE, int64>::value)\
    {\
      mData.mInt = (int64)data;\
    }\
    else if (is_same_type<TYPE, uint8>::value || is_same_type<TYPE, uint16>::value || is_same_type<TYPE, uint32>::value || is_same_type<TYPE, uint64>::value)\
    {\
      mData.mUInt = (uint64)data;\
    }\
    else if (is_same_type<TYPE, float>::value || is_same_type<TYPE, double>::value)\
    {\
      mData.mFloat = (double)data;\
    }\
  }

  CTOR(int8);
  CTOR(int32);
  CTOR(int64);
  
  CTOR(uint8);
  CTOR(uint32);
  CTOR(uint64);
  
  CTOR(float);
  CTOR(double);
  
#define EASYCTOR(TYPE) \
  nuiVariant(TYPE data) \
  {\
    mIsPointer = false;\
    mIsObject = false;\
    mType = nuiAttributeTypeTrait<TYPE>::mTypeId;\
    mIsPOD = true;\
    mIsArray = false;\
    mData.mInt = (uint64)data;\
  }

  EASYCTOR(nuiPosition);
  EASYCTOR(nuiOrientation);
  EASYCTOR(nuiBlendFunc);
  EASYCTOR(nuiDecorationMode);
  EASYCTOR(nuiMouseCursor);
  EASYCTOR(nuiDirection);
  EASYCTOR(nuiShapeMode);
  EASYCTOR(nuiDecorationLayer);
  EASYCTOR(nuiExpandMode);
#undef EASYCTOR
  
#undef CTOR
  
  
  nuiVariant(bool set)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<bool>::mTypeId;
    mIsPOD = true;
    mIsArray = false;
    
    mData.mBool = set;
  }
  
  // Classes CTORs
  nuiVariant(const nglString& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<nglString>::mTypeId;
    mString = rData;
  }
  
  nuiVariant(const nglPath& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<nglPath>::mTypeId;
    mString = rData.GetPathName();
  }
  
  nuiVariant(const nuiColor& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<nuiColor>::mTypeId;
    mColor = rData;
  }
  
  nuiVariant(const nuiRect& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<nuiRect>::mTypeId;
    mRect = rData;
  }
  
  nuiVariant(const nuiMatrix& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<nuiRect>::mTypeId;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiBorder& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiBorder>::mTypeId;
    mIsPOD = false;
    mIsArray = false;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiPoint& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiBorder>::mTypeId;
    mIsPOD = false;
    mIsArray = false;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiRange& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiRange>::mTypeId;
    mIsPOD = false;
    mIsArray = false;
    NGL_ASSERT(0);
  }
  
  nuiVariant(const nuiVector& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiVector>::mTypeId;
    mIsPOD = false;
    mIsArray = false;
    NGL_ASSERT(0);
  }
  
  // Copy CTOR
  nuiVariant(const nuiVariant& rObject)
  {
    mData = rObject.mData;
    
    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;
    
    mType = rObject.mType;
    
    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    mIsPOD = rObject.mIsPOD;
    mIsArray = rObject.mIsArray;

    if (mIsObject)
      mData.mpObject->Acquire();

    if (mIsArray)
      mData.mpArray = new std::vector<nuiVariant>(*rObject.mData.mpArray);
  }

  // Pointer CTOR
  template <typename Type>
  nuiVariant(Type* pData)
  {
    mIsPointer = true;
    mIsObject = is_base_of<nuiObject, Type>::value;
    mType = nuiAttributeTypeTrait<Type*>::mTypeId;
    mData.mpPointer = (void*)pData;
    mIsPOD = false;
    mIsArray = false;
    
    if (mIsObject)
      mData.mpObject->Acquire();
  }
  
  
  // DTOR:
  ~nuiVariant()
  {
    if (mIsObject)
      mData.mpObject->Release();
    if (mIsArray)
      delete mData.mpArray;
  }
  
  nuiVariant& operator=(const nuiVariant& rObject)
  {
    if (mIsObject)
      mData.mpObject->Release();
    if (mIsArray)
      delete mData.mpArray;
    
    mData = rObject.mData;
    
    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;
    
    mType = rObject.mType;
    
    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    mIsPOD = rObject.mIsPOD;
    mIsArray = rObject.mIsArray;
    
    if (mIsObject)
      mData.mpObject->Acquire();

    if (mIsArray)
      mData.mpArray = new std::vector<nuiVariant>(*rObject.mData.mpArray);
    return *this;
  }
  
  nuiAttributeType GetType() const
  {
    return mType;
  }
  
  void Clear()
  {
    if (mIsObject)
      mData.mpObject->Release();
    if (mIsArray)
      delete mData.mpArray;

    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<void>::mTypeId;
  }
  
  bool IsVoid() const
  {
    return mType == nuiAttributeTypeTrait<void>::mTypeId;
  }
  
  bool IsPointer() const
  {
    return mIsPointer;
  }
  
  bool IsObject() const
  {
    return mIsObject;
  }
  
  bool IsPOD() const
  {
    return mIsPOD;
  }
  
  bool IsArray() const
  {
    return mIsArray;
  }
  
  
  //////////////////////////////////////
  // Casting:
  template<typename Type>
  operator Type() const
  {
    return Type();
  }
  
  template<typename PointerType>
  operator PointerType*() const
  {
    if (nuiAttributeTypeTrait<PointerType*>::mTypeId == mType)
      return reinterpret_cast<PointerType*>(mData.mpPointer);
    
    if (!mIsPointer)
      return NULL;
    
    //if (mIsObject && is_base_of<nuiObject, PointerType>::value)
      return dynamic_cast<PointerType*>(reinterpret_cast<nuiObject*>(mData.mpPointer));
    
    return NULL;
  }
  
  template<typename Type>
  operator std::vector<Type>() const
  {
    std::vector<Type> v;
    if (mIsArray)
    {
      NGL_ASSERT(mData.mpArray);
      std::vector<nuiVariant>& rArray(*mData.mpArray);
      v.reserve(rArray.size());
      for (int32 i = 0; i < rArray.size(); i++)
        v.push_back(rArray[i]);
    }
    else
    {
      v.push_back((Type)*this);
    }

    return v;
  }
  
  
  // nglString Cast:
  operator nglString() const
  {
    if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
      return mString;
    
    if (mIsPointer)
      return nglString::Null;
    
    if (mType == nuiAttributeTypeTrait<nuiRect>::mTypeId)
      return mRect.GetValue();
    
    if (mType == nuiAttributeTypeTrait<nuiColor>::mTypeId)
      return mColor.GetValue();
    
    if (mType == nuiAttributeTypeTrait<nglPath>::mTypeId)
      return mString;
    
    nglString str;
    if (mType == nuiAttributeTypeTrait<float>::mTypeId
        || mType == nuiAttributeTypeTrait<double>::mTypeId)
      str.Add(mData.mFloat);
    else if (mType == nuiAttributeTypeTrait<int8>::mTypeId 
             || mType == nuiAttributeTypeTrait<int16>::mTypeId
             || mType == nuiAttributeTypeTrait<int32>::mTypeId
             || mType == nuiAttributeTypeTrait<int64>::mTypeId)
      str.Add(mData.mInt);
    else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId
             || mType == nuiAttributeTypeTrait<uint16>::mTypeId
             || mType == nuiAttributeTypeTrait<uint32>::mTypeId
             || mType == nuiAttributeTypeTrait<uint64>::mTypeId)
      str.Add(mData.mUInt);
    
    return str;
  }
  
  // nglPath Cast:
  operator nglPath() const
  {
    if (mType == nuiAttributeTypeTrait<nglPath>::mTypeId || mType == nuiAttributeTypeTrait<nglString>::mTypeId)
      return mString;

    return nglPath();
  }
  
  // POD Cast:
#define CAST(TYPE)\
operator TYPE() const\
  {\
    if (nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<float>::mTypeId\
        || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<double>::mTypeId)\
    {\
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)\
        return static_cast<TYPE>(mData.mFloat);\
      else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId) \
        return static_cast<TYPE>(mData.mUInt); \
      else if (mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId) \
        return static_cast<TYPE>(mData.mInt); \
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)\
        return static_cast<TYPE>(mString.GetCDouble());\
    }\
    else if (nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<int8>::mTypeId \
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<int16>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<int32>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<int64>::mTypeId)\
    {\
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId\
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)\
        return static_cast<TYPE>(mData.mInt);\
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)\
        return static_cast<TYPE>(ToBelow(mData.mFloat));\
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)\
          return static_cast<TYPE>(mString.GetCInt64());\
    }\
    else if (nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<uint8>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<uint16>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<uint32>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<uint64>::mTypeId)\
    {\
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId\
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)\
        return static_cast<TYPE>(mData.mUInt);\
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)\
        return static_cast<TYPE>(ToBelow(mData.mFloat));\
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)\
        return static_cast<TYPE>(mString.GetCUInt64());\
    }\
    NGL_ASSERT(0);\
    return static_cast<TYPE>(0);\
  }
  
  CAST(int8);
  CAST(int16);
  CAST(int32);
  CAST(int64);
  
  CAST(uint8);
  CAST(uint16);
  CAST(uint32);
  CAST(uint64);
  
  CAST(float);
  CAST(double);
  
#undef CAST
  
#define EASYCAST(X) operator X() const { return (X)(uint32)*this; }
  EASYCAST(nuiPosition);
  EASYCAST(nuiOrientation);
  EASYCAST(nuiBlendFunc);
  EASYCAST(nuiDecorationMode);
  EASYCAST(nuiMouseCursor);
  EASYCAST(nuiDirection);
  EASYCAST(nuiShapeMode);
  EASYCAST(nuiDecorationLayer);
  EASYCAST(nuiExpandMode);
#undef EASYCAST
  
  operator bool() const
  {
    if (mType == nuiAttributeTypeTrait<bool>::mTypeId)
      return mData.mBool;

    if (mType == nuiAttributeTypeTrait<float>::mTypeId
        || mType == nuiAttributeTypeTrait<double>::mTypeId)
    {
      return (bool)(mData.mFloat != 0);
    }
    else if (mType == nuiAttributeTypeTrait<int8>::mTypeId 
             || mType == nuiAttributeTypeTrait<int16>::mTypeId
             || mType == nuiAttributeTypeTrait<int32>::mTypeId
             || mType == nuiAttributeTypeTrait<int64>::mTypeId)
    {
      return (bool)(mData.mInt != 0);
    }
    else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId
             || mType == nuiAttributeTypeTrait<uint16>::mTypeId
             || mType == nuiAttributeTypeTrait<uint32>::mTypeId
             || mType == nuiAttributeTypeTrait<uint64>::mTypeId)
    {
      return (bool)(mData.mUInt != 0);
    }
    
    return false;
  }

  operator nuiRect() const
  {
    if (mType == nuiAttributeTypeTrait<nuiRect>::mTypeId)
      return mRect;
    if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
    {
      nuiRect r;
      r.SetValue(mString);
      return r;
    }
    return nuiRect();
  }
  
  operator nuiColor() const
  {
    if (mType == nuiAttributeTypeTrait<nuiColor>::mTypeId)
      return mColor;
    if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
      return nuiColor(mString);
    return nuiColor();
  }
  
  operator nuiMatrix() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiMatrix>::mTypeId)
      return nuiMatrix();
    return nuiMatrix();
  }
  
  operator nuiBorder() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiBorder>::mTypeId)
      return nuiBorder();
    return nuiBorder();
  }
  
  operator nuiPoint() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiPoint>::mTypeId)
      return nuiPoint();
    return nuiPoint();
  }
  
  operator nuiRange() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiRange>::mTypeId)
      return nuiRange();
    return nuiRange();
  }

  operator nuiVector() const
  {
    NGL_ASSERT(0);
    if (mType == nuiAttributeTypeTrait<nuiVector>::mTypeId)
      return nuiVector();
    return nuiVector();
  }
  
               
private:
  nuiAttributeType mType;
  union 
  {
    int64 mInt;
    uint64 mUInt;
    double mFloat;
    bool mBool;
    
    void* mpPointer;
    std::vector<nuiVariant>* mpArray;
    nuiObject* mpObject;    
  } mData;
  
  nglString mString;
  nuiRect mRect;
  nuiColor mColor;
  
  bool mIsPointer : 1;
  bool mIsObject : 1;
  bool mIsPOD : 1;
  bool mIsArray : 1;
};

