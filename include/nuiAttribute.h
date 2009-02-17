/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#pragma once


#include "nui.h"
#include "nuiSignalsSlots.h"
#include "nuiXML.h"
#include "nuiFastDelegate.h"
#include "nuiRange.h"
#include "nuiShape.h"
#include "nuiDecorationDefines.h"
#include "nuiMouseCursor.h"
#include "nuiRenderState.h"

class nuiAttributeEditor;
class nuiPoint;
class nuiRect;
class nuiBorder;


typedef uint64 nuiAttributeType;




// don't forget to update your application source code if
// another unit is added here.
// for instance, for Yapuka application,
// it's in ElementInspector.cpp::ElementInspector::AddToolpane

enum nuiAttributeUnit
{
	nuiUnitNone = 0,
  nuiUnitSeconds,
	nuiUnitPixels,
  nuiUnitPercent,
  nuiUnitBytes,
  nuiUnitOnOff,
	nuiUnitYesNo,
  nuiUnitBoolean,
	nuiUnitName,
	nuiUnitHumanName,
	nuiUnitPosition,
	nuiUnitSize,
	nuiUnitExpand,
	nuiUnitOrientation,
  nuiUnitColor,
  nuiUnitVector,
  nuiUnitMatrix,
  nuiUnitRangeKnob,
  nuiUnitCustom,
};


class nuiAttributeBase
{
public:
  typedef nuiFastDelegate1<uint32 /*dimension */, uint32> ArrayRangeDelegate;
  
  virtual ~nuiAttributeBase();

  bool IsReadOnly() const;
  bool IsWriteOnly() const;
  bool CanGet() const;
  bool CanSet() const;
  
  nuiAttributeType GetType() const;
  nuiAttributeUnit GetUnit() const;
  const nglString& GetName() const;
  const nuiRange& GetRange() const;
  nuiRange& GetRange();
  
  int32 GetOrder() const;
  void SetOrder(int32 order);

	
	
	virtual bool Load(void* pTarget, const nuiXMLNode* pNode) = 0;
	virtual nuiXMLNode* Serialize(void* pTarget, nuiXMLNode* pParentNode) const = 0;

  virtual bool ToString(void* pTarget, nglString& rString) const = 0;
  virtual bool FromString(void* pTarget, const nglString& rString) const = 0;
  virtual bool ToString(void* pTarget, int32 index, nglString& rString) const = 0;
  virtual bool FromString(void* pTarget, int32 index, const nglString& rString) const = 0;
  virtual bool ToString(void* pTarget, int32 index0, int32 index1, nglString& rString) const = 0;
  virtual bool FromString(void* pTarget, int32 index0, int32 index1, const nglString& rString) const = 0;
  
  void IgnoreAttributeChange(bool ignore);
  bool IsAttributeChangeIgnored() const;

  uint32 GetDimension() const;
  uint32 GetIndexRange(void* pTarget, uint32 Dimension) const;

	virtual nuiAttributeEditor* GetEditor(void* pTarget) = 0;
  
  virtual void KillAttributeHolder(void* pHolder) = 0;
protected:
  nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit units, const nuiRange& rRange, bool readonly, bool writeonly);  ///< property 
  nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit units, const nuiRange& rRange, bool readonly, bool writeonly, uint32 dimension, const ArrayRangeDelegate& rArrayRangeGetter);  ///< property 
  
private:
  nglString mName;
  nuiAttributeType mType;
  nuiAttributeUnit mUnit;
  bool mReadOnly;
  bool mWriteOnly;
  nuiRange mRange;
  bool mIgnoreAttributeChange;
  int32 mOrder;
  uint32 mDimension;
  ArrayRangeDelegate mRangeGetter;
};

inline uint64 GetNewAttributeUniqueId()
{
  static uint64 IdCounter = 0;
  return IdCounter++;
}



template <class Type>
class nuiAttributeTypeTrait
{
public:
  static nuiAttributeType GetTypeId()
  {
    return nuiAttributeTypeTrait<Type>::mTypeId;
  }
    
private:
  static uint64 mTypeId;
};



template <class Contents>  
class nuiAttribute : public nuiAttributeBase
{
public:
  // Direct access (dimension = 0)
  typedef nuiFastDelegate0<Contents> GetterDelegate;
  typedef nuiFastDelegate1<Contents> SetterDelegate;
  // Array (dimension = 1)
  typedef nuiFastDelegate1<uint32, Contents> GetterDelegate1;
  typedef nuiFastDelegate2<uint32, Contents> SetterDelegate1;
  // Array (dimension = 2)
  typedef nuiFastDelegate2<uint32, uint32, Contents> GetterDelegate2;
  typedef nuiFastDelegate3<uint32, uint32, Contents> SetterDelegate2;

  typedef nuiFastDelegate2<void*, nuiAttribute<Contents>*, nuiAttributeEditor* > NewEditorDelegate;
  typedef nuiFastDelegate3<void*, nglString&, Contents> FormaterDelegate;

  typedef nuiSignal1<Contents> Signal;
  typedef nuiSignal2<uint32, Contents> Signal0;
  typedef nuiSignal3<uint32, uint32, Contents> Signal1;
  typedef std::map<void*, nuiSignal*> AttributeSignalMap;
private:
  mutable AttributeSignalMap mAttributeChangedSignals;
public:
  

  // Direct Access: (dim = 0)
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate& rGetter, const SetterDelegate& rSetter, const nuiRange& rRange = nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false, false),
      mGetter(rGetter.GetMemento()),
      mSetter(rSetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate& rGetter, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, true, false),
    mGetter(rGetter.GetMemento())
  {
  }

  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const SetterDelegate& rSetter, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false, true),
    mSetter(rSetter.GetMemento())
  {
  }
  
	// Array (dim = 1)
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate1& rGetter, const SetterDelegate1& rSetter, const ArrayRangeDelegate& rRangeGetter, const nuiRange& rRange = nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false, false, 1, rRangeGetter),
    mGetter(rGetter.GetMemento()),
    mSetter(rSetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate1& rGetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, true, false, 1, rRangeGetter),
    mGetter(rGetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const SetterDelegate1& rSetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false, true, 1, rRangeGetter),
    mSetter(rSetter.GetMemento())
  {
  }
  
	// Array (dim = 2)
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate2& rGetter, const SetterDelegate2& rSetter, const ArrayRangeDelegate& rRangeGetter, const nuiRange& rRange = nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false, false, 2, rRangeGetter),
    mGetter(rGetter.GetMemento()),
    mSetter(rSetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate2& rGetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, true, false, 2, rRangeGetter),
    mGetter(rGetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const SetterDelegate2& rSetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false, true, 2, rRangeGetter),
    mSetter(rSetter.GetMemento())
  {
  }
  
	// inherited from AttributeBase, to be specialized for each type of Attribute
	virtual bool Load(void* pTarget, const nuiXMLNode* pNode)
	{
    if (pNode->HasAttribute(GetName()))
      return FromString(pTarget, pNode->GetAttribute(GetName()));
    return false;
	}

	// inherited from AttributeBase, to be specialized for each type of Attribute
	virtual nuiXMLNode* Serialize(void* pTarget, nuiXMLNode* pParentNode) const
	{
    nglString str;
    if (ToString(pTarget, str))
      pParentNode->SetAttribute(GetName(), str);
    return pParentNode;
	}

  bool ToString(Contents Value, nglString& rString) const
  {
    return rString = Value;
  }
  
  bool FromString(Contents& Value, const nglString& rString) const
  {
    return Value = rString;
  }
  
  bool ToString(void* pTarget, nglString& rString) const
  {
    return ToString(Get(pTarget), rString);
  }
  
  bool FromString(void* pTarget, const nglString& rString) const
  {
    Contents val;
    bool res = FromString(val, rString);
    Set(pTarget, val);
    return res;
  }
  
  bool ToString(void* pTarget, int32 index, nglString& rString) const
  {
    return ToString(Get(pTarget, index), rString);
  }
  
  bool FromString(void* pTarget, int32 index, const nglString& rString) const
  {
    Contents val;
    bool res = FromString(val, rString);
    Set(pTarget, index, val);
    return res;
  }
  
  bool ToString(void* pTarget, int32 index0, int32 index1, nglString& rString) const
  {
    return ToString(Get(pTarget, index0, index1), rString);
  }
  
  virtual bool FromString(void* pTarget, int32 index0, int32 index1, const nglString& rString) const
  {
    Contents val;
    bool res = FromString(val, rString);
    Set(pTarget, index0, index1, val);
    return res;
  }
    
  // Getters for each dimension:
  Contents Get(void* pTarget) const
  {
    NGL_ASSERT(!mGetter.empty());
    NGL_ASSERT(GetDimension() == 0);
    GetterDelegate Getter;
    Getter.SetMemento(mGetter);
    Getter.SetThis(pTarget);
    return Getter();
  }
  
  Contents Get(void* pTarget, uint32 index) const
  {
    NGL_ASSERT(!mGetter.empty());
    NGL_ASSERT(GetDimension() == 1);
    GetterDelegate1 Getter;
    Getter.SetMemento(mGetter);
    Getter.SetThis(pTarget);
    return Getter(index);
  }
  
  Contents Get(void* pTarget, uint32 index0, uint32 index1) const
  {
    NGL_ASSERT(!mGetter.empty());
    NGL_ASSERT(GetDimension() == 2);
    GetterDelegate2 Getter;
    Getter.SetMemento(mGetter);
    Getter.SetThis(pTarget);
    return Getter(index0, index1);
  }
  
  // Setters for each dimension:
  void Set(void* pTarget, Contents rValue) const
  {
    NGL_ASSERT(!mSetter.empty());
    NGL_ASSERT(GetDimension() == 0);
    SetterDelegate Setter;
    Setter.SetMemento(mSetter);
    Setter.SetThis(pTarget);
    Setter(rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, rValue); 
    }
  }
  
  void Set(void* pTarget, uint32 index, Contents rValue) const
  {
    NGL_ASSERT(!mSetter.empty());
    NGL_ASSERT(GetDimension() == 1);
    SetterDelegate1 Setter;
    Setter.SetMemento(mSetter);
    Setter.SetThis(pTarget);
    Setter(index, rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, index, rValue); 
    }
  }
  
  void Set(void* pTarget, uint32 index0, uint32 index1, Contents rValue) const
  {
    NGL_ASSERT(!mSetter.empty());
    NGL_ASSERT(GetDimension() == 2);
    SetterDelegate2 Setter;
    Setter.SetMemento(mSetter);
    Setter.SetThis(pTarget);
    Setter(index0, index1, rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, index0, index1, rValue); 
    }
  }

  // Get/Set Editors:
	void SetEditor(const NewEditorDelegate& rNewEditor)
	{
		mNewEditor = rNewEditor;
	}
  
	virtual nuiAttributeEditor* GetEditor(void* pTarget)
	{
		if (mNewEditor)
    {
      // the user has specified its own editor.
      return mNewEditor(pTarget, this);
    }

    return GetDefaultEditor(pTarget);
	}

  // return a NULL editor in general. The editors by default are defined in nuiAttribute.cpp, as a specialization of the template trait nuiAttribute<T>::GetEditor
  // the user can also defines its own editor, in calling SetEditor	
	nuiAttributeEditor* GetDefaultEditor(void* pTarget)
	{
		// if this code is executed, it means a template trait specialization is missing in nuiAttribute.cpp.
		return NULL;
	}
	
	void SetFormater(const FormaterDelegate& rFormater)
	{
		mFormater = rFormater;
	}

	// Format methods for each dimensions:
	void Format(void* pTarget, nglString& string)
	{
		if (mFormater)
		{
			// the user has specified its own formater.
			return mFormater(pTarget, string, Get(pTarget));
		}
		
		return FormatDefault(pTarget, string);
	}
	
	void Format(void* pTarget, uint32 index, nglString& string)
	{
		if (mFormater)
		{
			// the user has specified its own formater.
			return mFormater(pTarget, string, Get(pTarget, index));
		}
		
		return FormatDefault(pTarget, string);
	}
	
	void Format(void* pTarget, uint32 index0, uint32 index1, nglString& string)
	{
		if (mFormater)
		{
			// the user has specified its own formater.
			return mFormater(pTarget, string, Get(pTarget, index0, index1));
		}
		
		return FormatDefault(pTarget, string);
	}
	
	void FormatDefault(void* pTarget, nglString& string)
	{
		// if don't want to use this default formater for your attribute, simply create a template trait specialization in attribute.cpp
		return;
	}
	
  Signal& GetChangedSignal(void* pTarget) const
  {
    AttributeSignalMap::iterator it = mAttributeChangedSignals.find(pTarget);
    if (it == mAttributeChangedSignals.end())
    {
      Signal* pS = new Signal;
      mAttributeChangedSignals[pTarget] = pS;
      return *pS;
    }
    return reinterpret_cast<Signal&>(*(it->second));
  }
  
  Signal0& GetChangedSignal0(void* pTarget) const
  {
    AttributeSignalMap::iterator it = mAttributeChangedSignals.find(pTarget);
    if (it == mAttributeChangedSignals.end())
    {
      Signal0* pS = new Signal0;
      mAttributeChangedSignals[pTarget] = pS;
      return *pS;
    }
    return reinterpret_cast<Signal0&>(*(it->second));
  }
  
  Signal1& GetChangedSignal1(void* pTarget) const
  {
    AttributeSignalMap::iterator it = mAttributeChangedSignals.find(pTarget);
    if (it == mAttributeChangedSignals.end())
    {
      Signal1* pS = new Signal1;
      mAttributeChangedSignals[pTarget] = pS;
      return *pS;
    }
    return reinterpret_cast<Signal1&>(*(it->second));
  }
  
  void KillAttributeHolder(void* pHolder)
  {
    AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pHolder);
    AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      delete it->second;
      mAttributeChangedSignals.erase(it);
    }
  }

private:
  nuiDelegateMemento mGetter;
  nuiDelegateMemento mSetter;
	NewEditorDelegate mNewEditor;
	FormaterDelegate mFormater;
  ArrayRangeDelegate mRangeGetter;
  
  void SendAttributeChanged(void* pTarget, Contents value) const
  {
    const AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pTarget);
    const AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      nuiSignal1<Contents>* pSignal = (nuiSignal1<Contents>*)it->second;
      (*pSignal)(value);
    }
  }	

  void SendAttributeChanged(void* pTarget, uint32 index, Contents value) const
  {
    const AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pTarget);
    const AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      nuiSignal2<uint32, Contents>* pSignal = (nuiSignal2<uint32, Contents>*)it->second;
      (*pSignal)(index, value);
    }
  }	

  void SendAttributeChanged(void* pTarget, uint32 index0, uint32 index1, Contents value) const
  {
    const AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pTarget);
    const AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      nuiSignal3<uint32, uint32, Contents>* pSignal = (nuiSignal3<uint32, uint32, Contents>*)it->second;
      (*pSignal)(index0, index1, value);
    }
  }	
};


//////////////////////////////////////////////
template <class Contents>  
class nuiAttribute<const Contents&> : public nuiAttributeBase
{
public:
  // Direct access (dimension = 0)
  typedef nuiFastDelegate0<const Contents&> GetterDelegate;
  typedef nuiFastDelegate1<const Contents&> SetterDelegate;
  // Array (dimension = 1)
  typedef nuiFastDelegate1<uint32, const Contents&> GetterDelegate1;
  typedef nuiFastDelegate2<uint32, const Contents&> SetterDelegate1;
  // Array (dimension = 2)
  typedef nuiFastDelegate2<uint32, uint32, const Contents&> GetterDelegate2;
  typedef nuiFastDelegate3<uint32, uint32, const Contents&> SetterDelegate2;
  
  typedef nuiFastDelegate2<void*, nuiAttribute<const Contents&>*, nuiAttributeEditor* > NewEditorDelegate;
  typedef nuiFastDelegate3<void*, nglString&, const Contents&> FormaterDelegate;
  
  typedef nuiSignal1<const Contents&> Signal;
  typedef nuiSignal2<uint32, const Contents&> Signal0;
  typedef nuiSignal3<uint32, uint32, const Contents&> Signal1;
  typedef std::map<void*, nuiSignal*> AttributeSignalMap;
private:
  mutable AttributeSignalMap mAttributeChangedSignals;
public:
  
  
  // Direct Access: (dim = 0)
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate& rGetter, const SetterDelegate& rSetter, const nuiRange& rRange = nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, false, false),
    mGetter(rGetter.GetMemento()),
    mSetter(rSetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate& rGetter, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, true, false),
    mGetter(rGetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const SetterDelegate& rSetter, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, false, true),
    mSetter(rSetter.GetMemento())
  {
  }
  
  // Array (dim = 1)
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate1& rGetter, const SetterDelegate1& rSetter, const ArrayRangeDelegate& rRangeGetter, const nuiRange& rRange = nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, false, false, 1, rRangeGetter),
    mGetter(rGetter.GetMemento()),
    mSetter(rSetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate1& rGetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, true, false, 1, rRangeGetter),
    mGetter(rGetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const SetterDelegate1& rSetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, false, true, 1, rRangeGetter),
    mSetter(rSetter.GetMemento())
  {
  }
  
  // Array (dim = 2)
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate2& rGetter, const SetterDelegate2& rSetter, const ArrayRangeDelegate& rRangeGetter, const nuiRange& rRange = nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, false, false, 2, rRangeGetter),
    mGetter(rGetter.GetMemento()),
    mSetter(rSetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate2& rGetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, true, false, 2, rRangeGetter),
    mGetter(rGetter.GetMemento())
  {
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const SetterDelegate2& rSetter, const ArrayRangeDelegate& rRangeGetter = NULL, const nuiRange& rRange = nuiRange()) ///< Read only property
  : nuiAttributeBase(rName, nuiAttributeTypeTrait<const Contents&>::GetTypeId(), units, rRange, false, true, 2, rRangeGetter),
  mSetter(rSetter.GetMemento())
  {
  }
  
  // inherited from AttributeBase, to be specialized for each type of Attribute
  virtual bool Load(void* pTarget, const nuiXMLNode* pNode)
  {
    if (pNode->HasAttribute(GetName()))
      return FromString(pTarget, pNode->GetAttribute(GetName()));
    return false;
  }
  
  // inherited from AttributeBase, to be specialized for each type of Attribute
  virtual nuiXMLNode* Serialize(void* pTarget, nuiXMLNode* pParentNode) const
  {
    nglString str;
    if (ToString(pTarget, str))
      pParentNode->SetAttribute(GetName(), str);
    return pParentNode;
  }
  
  bool ToString(const Contents& rValue, nglString& rString) const
  {
    return rString = rValue;
  }
  
  bool FromString(Contents& rValue, const nglString& rString) const
  {
    return rValue = rString;
  }
  
  bool ToString(void* pTarget, nglString& rString) const
  {
    return ToString(Get(pTarget), rString);
  }
  
  bool FromString(void* pTarget, const nglString& rString) const
  {
    Contents val;
    bool res = FromString(val, rString);
    Set(pTarget, val);
    return res;
  }
  
  bool ToString(void* pTarget, int32 index, nglString& rString) const
  {
    return ToString(Get(pTarget, index), rString);
  }
  
  bool FromString(void* pTarget, int32 index, const nglString& rString) const
  {
    Contents val;
    bool res = FromString(val, rString);
    Set(pTarget, index, val);
    return res;
  }
  
  bool ToString(void* pTarget, int32 index0, int32 index1, nglString& rString) const
  {
    return ToString(Get(pTarget, index0, index1), rString);
  }
  
  virtual bool FromString(void* pTarget, int32 index0, int32 index1, const nglString& rString) const
  {
    Contents val;
    bool res = FromString(val, rString);
    Set(pTarget, index0, index1, val);
    return res;
  }
  
  // Getters for each dimension:
  const Contents& Get(void* pTarget) const
  {
    NGL_ASSERT(!mGetter.empty());
    NGL_ASSERT(GetDimension() == 0);
    GetterDelegate Getter;
    Getter.SetMemento(mGetter);
    Getter.SetThis(pTarget);
    return Getter();
  }
  
  const Contents& Get(void* pTarget, uint32 index) const
  {
    NGL_ASSERT(!mGetter.empty());
    NGL_ASSERT(GetDimension() == 1);
    GetterDelegate1 Getter;
    Getter.SetMemento(mGetter);
    Getter.SetThis(pTarget);
    return Getter(index);
  }
  
  const Contents& Get(void* pTarget, uint32 index0, uint32 index1) const
  {
    NGL_ASSERT(!mGetter.empty());
    NGL_ASSERT(GetDimension() == 2);
    GetterDelegate2 Getter;
    Getter.SetMemento(mGetter);
    Getter.SetThis(pTarget);
    return Getter(index0, index1);
  }
  
  // Setters for each dimension:
  void Set(void* pTarget, const Contents& rValue) const
  {
    NGL_ASSERT(!mSetter.empty());
    NGL_ASSERT(GetDimension() == 0);
    SetterDelegate Setter;
    Setter.SetMemento(mSetter);
    Setter.SetThis(pTarget);
    Setter(rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, rValue); 
    }
  }
  
  void Set(void* pTarget, uint32 index, const Contents& rValue) const
  {
    NGL_ASSERT(!mSetter.empty());
    NGL_ASSERT(GetDimension() == 1);
    SetterDelegate1 Setter;
    Setter.SetMemento(mSetter);
    Setter.SetThis(pTarget);
    Setter(index, rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, index, rValue); 
    }
  }
  
  void Set(void* pTarget, uint32 index0, uint32 index1, const Contents& rValue) const
  {
    NGL_ASSERT(!mSetter.empty());
    NGL_ASSERT(GetDimension() == 2);
    SetterDelegate2 Setter;
    Setter.SetMemento(mSetter);
    Setter.SetThis(pTarget);
    Setter(index0, index1, rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, index0, index1, rValue); 
    }
  }
  
  // Get/Set Editors:
  void SetEditor(const NewEditorDelegate& rNewEditor)
  {
    mNewEditor = rNewEditor;
  }
  
  virtual nuiAttributeEditor* GetEditor(void* pTarget)
  {
    if (mNewEditor)
    {
      // the user has specified its own editor.
      return mNewEditor(pTarget, this);
    }
    
    return GetDefaultEditor(pTarget);
  }
  
  // return a NULL editor in general. The editors by default are defined in nuiAttribute.cpp, as a specialization of the template trait nuiAttribute<T>::GetEditor
  // the user can also defines its own editor, in calling SetEditor	
  nuiAttributeEditor* GetDefaultEditor(void* pTarget)
  {
    // if this code is executed, it means a template trait specialization is missing in nuiAttribute.cpp.
    return NULL;
  }
  
  void SetFormater(const FormaterDelegate& rFormater)
  {
    mFormater = rFormater;
  }
  
  // Format methods for each dimensions:
  void Format(void* pTarget, nglString& string)
  {
    if (mFormater)
    {
      // the user has specified its own formater.
      return mFormater(pTarget, string, Get(pTarget));
    }
    
    return FormatDefault(pTarget, string);
  }
  
  void Format(void* pTarget, uint32 index, nglString& string)
  {
    if (mFormater)
    {
      // the user has specified its own formater.
      return mFormater(pTarget, string, Get(pTarget, index));
    }
    
    return FormatDefault(pTarget, string);
  }
  
  void Format(void* pTarget, uint32 index0, uint32 index1, nglString& string)
  {
    if (mFormater)
    {
      // the user has specified its own formater.
      return mFormater(pTarget, string, Get(pTarget, index0, index1));
    }
    
    return FormatDefault(pTarget, string);
  }
  
  void FormatDefault(void* pTarget, nglString& string)
  {
    // if don't want to use this default formater for your attribute, simply create a template trait specialization in attribute.cpp
    return;
  }
  
  Signal& GetChangedSignal(void* pTarget) const
  {
    AttributeSignalMap::iterator it = mAttributeChangedSignals.find(pTarget);
    if (it == mAttributeChangedSignals.end())
    {
      Signal* pS = new Signal;
      mAttributeChangedSignals[pTarget] = pS;
      return *pS;
    }
    return reinterpret_cast<Signal&>(*(it->second));
  }
  
  Signal0& GetChangedSignal0(void* pTarget) const
  {
    AttributeSignalMap::iterator it = mAttributeChangedSignals.find(pTarget);
    if (it == mAttributeChangedSignals.end())
    {
      Signal0* pS = new Signal0;
      mAttributeChangedSignals[pTarget] = pS;
      return *pS;
    }
    return reinterpret_cast<Signal0&>(*(it->second));
  }
  
  Signal1& GetChangedSignal1(void* pTarget) const
  {
    AttributeSignalMap::iterator it = mAttributeChangedSignals.find(pTarget);
    if (it == mAttributeChangedSignals.end())
    {
      Signal1* pS = new Signal1;
      mAttributeChangedSignals[pTarget] = pS;
      return *pS;
    }
    return reinterpret_cast<Signal1&>(*(it->second));
  }
  
  void KillAttributeHolder(void* pHolder)
  {
    AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pHolder);
    AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      delete it->second;
      mAttributeChangedSignals.erase(it);
    }
  }

  bool CanSet() const
  {
    return !mSetter.empty();
  }
  
  bool CanGet() const
  {
    return !mGetter.empty();
  }
  
  
private:
  nuiDelegateMemento mGetter;
  nuiDelegateMemento mSetter;
  NewEditorDelegate mNewEditor;
  FormaterDelegate mFormater;
  ArrayRangeDelegate mRangeGetter;
  
  void SendAttributeChanged(void* pTarget, const Contents& value) const
  {
    const AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pTarget);
    const AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      nuiSignal1<const Contents&>* pSignal = (nuiSignal1<const Contents&>*)it->second;
      (*pSignal)(value);
    }
  }	
  
  void SendAttributeChanged(void* pTarget, uint32 index, const Contents& value) const
  {
    const AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pTarget);
    const AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      nuiSignal2<uint32, const Contents&>* pSignal = (nuiSignal2<uint32, const Contents&>*)it->second;
      (*pSignal)(index, value);
    }
  }	
  
  void SendAttributeChanged(void* pTarget, uint32 index0, uint32 index1, const Contents& value) const
  {
    const AttributeSignalMap::iterator it  = mAttributeChangedSignals.find(pTarget);
    const AttributeSignalMap::iterator end = mAttributeChangedSignals.end();
    if (it != end)
    {
      nuiSignal3<uint32, uint32, const Contents&>* pSignal = (nuiSignal3<uint32, uint32, const Contents&>*)it->second;
      (*pSignal)(index0, index1, value);
    }
  }	
};
//////////////////////////////////////////////

template <class Type> 
nuiAttribute<Type>* nuiAttributeCast(const nuiAttributeBase* pBase)
{
  if (pBase->GetType() == nuiAttributeTypeTrait<Type>::GetTypeId())
    return (nuiAttribute<Type>*)pBase;
  
  return NULL;
}



class nuiAttribBase
{
public:
  nuiAttribBase();
  nuiAttribBase(void* pTarget, nuiAttributeBase* pAttribute);
  nuiAttribBase(const nuiAttribBase& rOriginal);
  ~nuiAttribBase();

  nuiAttribBase& operator=(const nuiAttribBase& rOriginal);

  nuiAttributeBase* GetAttribute();
  const nuiAttributeBase* GetAttribute() const;
  void* GetTarget();
  const void* GetTarget() const;

  // From nuiAttribute and nuiAttributeBase
  bool IsReadOnly() const;
  bool CanSet() const;
  bool CanGet() const;
  
  nuiAttributeType GetType() const;
  nuiAttributeUnit GetUnit() const;
  const nglString& GetName() const;
  const nuiRange& GetRange() const;
  nuiRange& GetRange();
  
  int32 GetOrder() const;
  void SetOrder(int32 order);
  
	bool Load(const nuiXMLNode* pNode);
	nuiXMLNode* Serialize(nuiXMLNode* pParentNode) const;
  
  bool ToString(nglString& rString) const;
  bool FromString(const nglString& rString) const;
  bool ToString(uint32 index, nglString& rString) const;
  bool FromString(uint32 index, const nglString& rString) const;
  bool ToString(uint32 index0, uint32 index1, nglString& rString) const;
  bool FromString(uint32 index0, uint32 index1, const nglString& rString) const;
  
  void IgnoreAttributeChange(bool ignore);
  
  bool IsAttributeChangeIgnored() const;
  
	nuiAttributeEditor* GetEditor();

  uint32 GetDimension() const;
  uint32 GetIndexRange(uint32 Dimension) const;
  
  bool IsValid() const;
  operator bool() const;
  
protected:
  nuiAttributeBase* mpAttributeBase;
  void* mpTarget;
};

template <class Contents>
class nuiAttrib : public nuiAttribBase
{
public:
  nuiAttrib()
  : mpAttribute(NULL)
  {
  }
  
  nuiAttrib(const nuiAttribBase& rAttribBase)
  : nuiAttribBase(rAttribBase),
    mpAttribute(nuiAttributeCast<Contents>(mpAttributeBase))
  {
    NGL_ASSERT(mpAttribute);
  }
  
  nuiAttrib(void* pTarget, nuiAttribute<Contents>* pAttribute)
  : nuiAttribBase(pTarget, pAttribute),
    mpAttribute(pAttribute)
  {
    NGL_ASSERT(mpAttribute);
  }
  
  nuiAttrib(const nuiAttrib<Contents>& rOriginal)
  : nuiAttribBase(rOriginal), 
    mpAttribute(rOriginal.mpAttribute)
  {
    NGL_ASSERT(mpAttribute);
  }
  
  ~nuiAttrib()
  {
    ~nuiAttribBase();
  }

  nuiAttrib<Contents>& operator=(const nuiAttrib<Contents>& rOriginal)
  {
    ((nuiAttribBase*)this)->operator=(rOriginal);
    mpAttribute = rOriginal.mpAttribute;
    return *this;
  }
  
  
  Contents Get() const
  {
    return mpAttribute->Get(mpTarget);
  }
  
  void Set(Contents rValue) const
  {
    mpAttribute->Set(mpTarget, rValue);
  }
	
  Contents Get(uint32 index) const
  {
    return mpAttribute->Get(mpTarget, index);
  }
  
  void Set(uint32 index, Contents rValue) const
  {
    mpAttribute->Set(mpTarget, index, rValue);
  }
	
  Contents Get(uint32 index0, uint32 index1) const
  {
    return mpAttribute->Get(mpTarget, index0, index1);
  }
  
  void Set(uint32 index0, uint32 index1, Contents rValue) const
  {
    mpAttribute->Set(mpTarget, index0, index1, rValue);
  }

  //	void SetEditor(const nuiAttribute<Contents>::NewEditorDelegate& rNewEditor)
//	{
//		mpAttribute->SetEditor(rNewEditor);
//	}
  
	nuiAttributeEditor* GetEditor()
	{
    return mpAttribute->GetEditor(mpTarget);
	}
  
  
  // return a NULL editor in general. The editors by default are defined in nuiAttribute.cpp, as a specialization of the template trait nuiAttribute<T>::GetEditor
  // the user can also defines its own editor, in calling SetEditor	
	nuiAttributeEditor* GetDefaultEditor()
	{
		// if this code is executed, it means a template trait specialization is missing in nuiAttribute.cpp.
		return mpAttribute->GetDefaultEditor(mpTarget);
	}
	
//	void SetFormater(const nuiAttribute<Contents>::FormaterDelegate& rFormater)
//	{
//		mpAttribute->SetFormater(rFormater);
//	}
	
	void Format(nglString& rString)
	{
    mpAttribute->Format(mpTarget, rString);
	}
	
	void FormatDefault(nglString& rString)
	{
		mpAttribute->FormatDefault(mpTarget, rString);
	}

	void Format(uint32 index, nglString& rString)
	{
    mpAttribute->Format(mpTarget, index, rString);
	}
	
	void FormatDefault(uint32 index, nglString& rString)
	{
		mpAttribute->FormatDefault(mpTarget, index, rString);
	}
	
	void Format(uint32 index0, uint32 index1, nglString& rString)
	{
    mpAttribute->Format(mpTarget, index0, index1, rString);
	}
	
	void FormatDefault(uint32 index0, uint32 index1, nglString& rString)
	{
		mpAttribute->FormatDefault(mpTarget, index0, index1, rString);
	}
	
	
  nuiSignal1<Contents>& GetChangedSignal() const
  {
    NGL_ASSERT(mpAttribute->GetDimension() == 0);
    return mpAttribute->GetChangedSignal(mpTarget);
  }
  
  nuiSignal2<uint32, Contents>& GetChangedSignal0() const
  {
    NGL_ASSERT(mpAttribute->GetDimension() == 1);
    return mpAttribute->GetChangedSignal0(mpTarget);
  }
  
  nuiSignal3<uint32, uint32, Contents>& GetChangedSignal1() const
  {
    NGL_ASSERT(mpAttribute->GetDimension() == 2);
    return mpAttribute->GetChangedSignal1(mpTarget);
  }
  
private:
  nuiAttribute<Contents>* mpAttribute;
};



#define NUI_DECLARE_ATTRIBUTE_TYPE(X)  template<> uint64 nuiAttributeTypeTrait<X>::mTypeId = GetNewAttributeUniqueId();

///////////////////////////// Specializations for simple types:

template <>
bool nuiAttribute<bool>::ToString(bool Value, nglString& rString) const;

template <>
bool nuiAttribute<bool>::FromString(bool& Value, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<bool>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<bool>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// int8
//

template <>
nuiAttributeEditor* nuiAttribute<int8>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int8>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int8>::ToString(int8 Value, nglString& rString) const;

template <>
bool nuiAttribute<int8>::FromString(int8& Value, const nglString& rString) const;



//********************************
//
// int16
//

template <>
nuiAttributeEditor* nuiAttribute<int16>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int16>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int16>::ToString(int16 rValue, nglString& rString) const;

template <>
bool nuiAttribute<int16>::FromString(int16& rValue, const nglString& rString) const;


//********************************
//
// int32
//

template <>
nuiAttributeEditor* nuiAttribute<int32>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int32>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int32>::ToString(int32 rValue, nglString& rString) const;

template <>
bool nuiAttribute<int32>::FromString(int32& rValue, const nglString& rString) const;

//********************************
//
// int64
//

template <>
nuiAttributeEditor* nuiAttribute<int64>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int64>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int64>::ToString(int64 rValue, nglString& rString) const;

template <>
bool nuiAttribute<int64>::FromString(int64& rValue, const nglString& rString) const;


//********************************
//
// uint8
//

template <>
nuiAttributeEditor* nuiAttribute<uint8>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint8>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint8>::ToString(uint8 rValue, nglString& rString) const;

template <>
bool nuiAttribute<uint8>::FromString(uint8& rValue, const nglString& rString) const;


//********************************
//
// uint16
//

template <>
nuiAttributeEditor* nuiAttribute<uint16>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint16>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint16>::ToString(uint16 rValue, nglString& rString) const;

template <>
bool nuiAttribute<uint16>::FromString(uint16& rValue, const nglString& rString) const;



//********************************
//
// uint32
//

template <>
nuiAttributeEditor* nuiAttribute<uint32>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint32>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint32>::ToString(uint32 rValue, nglString& rString) const;

template <>
bool nuiAttribute<uint32>::FromString(uint32& rValue, const nglString& rString) const;




//********************************
//
// uint64
//

template <>
nuiAttributeEditor* nuiAttribute<uint64>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint64>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint64>::ToString(uint64 Value, nglString& rString) const;

template <>
bool nuiAttribute<uint64>::FromString(uint64& rValue, const nglString& rString) const;




//********************************
//
// float
//

template <>
nuiAttributeEditor* nuiAttribute<float>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<float>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<float>::ToString(float Value, nglString& rString) const;

template <>
bool nuiAttribute<float>::FromString(float& rValue, const nglString& rString) const;



//********************************
//
// double
//

template <>
nuiAttributeEditor* nuiAttribute<double>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<double>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<double>::ToString(double Value, nglString& rString) const;

template <>
bool nuiAttribute<double>::FromString(double& rValue, const nglString& rString) const;



//********************************
//
// nuiPosition
//

template <>
nuiAttributeEditor* nuiAttribute<nuiPosition>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<nuiPosition>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<nuiPosition>::ToString(nuiPosition Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiPosition>::FromString(nuiPosition& rValue, const nglString& rString) const;






//********************************
//
// nglString
//

template <>
bool nuiAttribute<nglString>::ToString(nglString Value, nglString& rString) const;

template <>
bool nuiAttribute<nglString>::FromString(nglString& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nglString>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nglString>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nglString&
//

template <>
bool nuiAttribute<const nglString&>::ToString(const nglString& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nglString&>::FromString(nglString&, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nglString&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nglString&>::FormatDefault(void* pTarget, nglString& string);





//********************************
//
// nuiColor
//

template <>
bool nuiAttribute<nuiColor>::ToString(nuiColor Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiColor>::FromString(nuiColor& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiColor>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiColor>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nuiColor&
//

template <>
bool nuiAttribute<const nuiColor&>::ToString(const nuiColor& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nuiColor&>::FromString(nuiColor& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiColor&>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<const nuiColor&>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// nuiPoint
//

template <>
bool nuiAttribute<nuiPoint>::ToString(nuiPoint Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiPoint>::FromString(nuiPoint& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiPoint>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiPoint>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// nuiRect
//

template <>
bool nuiAttribute<nuiRect>::ToString(nuiRect Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiRect>::FromString(nuiRect& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiRect>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiRect>::FormatDefault(void* pTarget, nglString& string);


//********************************
//
// const nuiRect&
//

template <>
bool nuiAttribute<const nuiRect&>::ToString(const nuiRect& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nuiRect&>::FromString(nuiRect& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiRect&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nuiRect&>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// nuiBorder
//

template <>
bool nuiAttribute<nuiBorder>::ToString(nuiBorder Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiBorder>::FromString(nuiBorder& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiBorder>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiBorder>::FormatDefault(void* pTarget, nglString& string);


//********************************
//
// const nuiBorder&
//

template <>
bool nuiAttribute<const nuiBorder&>::ToString(const nuiBorder& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nuiBorder&>::FromString(nuiBorder& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiBorder&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nuiBorder&>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// nglPath
//

template <>
bool nuiAttribute<nglPath>::ToString(nglPath Value, nglString& rString) const;

template <>
bool nuiAttribute<nglPath>::FromString(nglPath& rValue, const nglString& rString) const;

template <>
void nuiAttribute<nglPath>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nglString&
//

template <>
bool nuiAttribute<const nglPath&>::ToString(const nglPath& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nglPath&>::FromString(nglPath& rValue, const nglString& rString) const;

template <>
void nuiAttribute<const nglPath&>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// DecorationMode
//

template <>
bool nuiAttribute<nuiDecorationMode>::ToString(nuiDecorationMode Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiDecorationMode>::FromString(nuiDecorationMode& rValue, const nglString& rString) const;

template <>
void nuiAttribute<nuiDecorationMode>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// ShapeMode
//

template <>
bool nuiAttribute<nuiShapeMode>::ToString(nuiShapeMode Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiShapeMode>::FromString(nuiShapeMode& rValue, const nglString& rString) const;

template <>
void nuiAttribute<nuiShapeMode>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// const nuiRange&
//

template <>
bool nuiAttribute<const nuiRange&>::ToString(const nuiRange& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nuiRange&>::FromString(nuiRange& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiRange&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nuiRange&>::FormatDefault(void* pTarget, nglString& string);





//********************************
//
// nuiDecorationLayer
//

template <>
bool nuiAttribute<nuiDecorationLayer>::ToString(nuiDecorationLayer Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiDecorationLayer>::FromString(nuiDecorationLayer& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiDecorationLayer>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<nuiDecorationLayer>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// nuiMouseCursor
//

template <>
bool nuiAttribute<nuiMouseCursor>::ToString(nuiMouseCursor Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiMouseCursor>::FromString(nuiMouseCursor& rValue, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiMouseCursor>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<nuiMouseCursor>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nglVectorf&
//

template <>
bool nuiAttribute<const nglVectorf&>::ToString(const nglVectorf& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nglVectorf&>::FromString(nglVectorf& rValue, const nglString& rString) const;

template <>
void nuiAttribute<const nglVectorf&>::FormatDefault(void* pTarget, nglString& string);

//********************************
//
// nglVectorf
//

template <>
bool nuiAttribute<nglVectorf>::ToString(nglVectorf Value, nglString& rString) const;

template <>
bool nuiAttribute<nglVectorf>::FromString(nglVectorf& rValue, const nglString& rString) const;

template <>
void nuiAttribute<nglVectorf>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nglMatrixf&
//

template <>
bool nuiAttribute<const nglMatrixf&>::ToString(const nglMatrixf& rValue, nglString& rString) const;

template <>
bool nuiAttribute<const nglMatrixf&>::FromString(nglMatrixf& rValue, const nglString& rString) const;


template <>
void nuiAttribute<const nglMatrixf&>::FormatDefault(void* pTarget, nglString& string);

//********************************
//
// nglMatrixf
//

template <>
bool nuiAttribute<nglMatrixf>::ToString(nglMatrixf Value, nglString& rString) const;

template <>
bool nuiAttribute<nglMatrixf>::FromString(nglMatrixf& rValue, const nglString& rString) const;


template <>
void nuiAttribute<nglMatrixf>::FormatDefault(void* pTarget, nglString& string);


//********************************
//
// BlendFunc
//

template <>
bool nuiAttribute<nuiBlendFunc>::ToString(nuiBlendFunc Value, nglString& rString) const;

template <>
bool nuiAttribute<nuiBlendFunc>::FromString(nuiBlendFunc& rValue, const nglString& rString) const;

template <>
void nuiAttribute<nuiBlendFunc>::FormatDefault(void* pTarget, nglString& string);


