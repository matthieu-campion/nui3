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
  nuiUnitCustom
};


class nuiAttributeBase
{
public:
  virtual ~nuiAttributeBase();

  bool IsReadOnly() const;
  
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
  
  void IgnoreAttributeChange(bool ignore)
  {
    mIgnoreAttributeChange = ignore;
  }
  
  bool IsAttributeChangeIgnored() const
  {
    return mIgnoreAttributeChange;
  }

	virtual nuiAttributeEditor* GetEditor(void* pTarget) = 0;
  
  virtual void KillAttributeHolder(void* pHolder) = 0;
protected:
  nuiAttributeBase(const nglString& rName, nuiAttributeType type, nuiAttributeUnit units, const nuiRange& rRange, bool readonly);  ///< property 
  
private:
  nglString mName;
  nuiAttributeType mType;
  nuiAttributeUnit mUnit;
  bool mReadOnly;
  nuiRange mRange;
  bool mIgnoreAttributeChange;
  int32 mOrder;
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
  typedef typename nuiFastDelegate::FastDelegate0<Contents> GetterDelegate;
  typedef typename nuiFastDelegate::FastDelegate1<Contents> SetterDelegate;
  typedef typename nuiFastDelegate::FastDelegate2<void*, nuiAttribute<Contents>*, nuiAttributeEditor* > NewEditorDelegate;
  typedef nuiFastDelegate::FastDelegate3<void*, nglString&, Contents> FormaterDelegate;
  typedef nuiSignal1<Contents> Signal;
  typedef std::map<void*, nuiSignal*> AttributeSignalMap;
private:
  mutable AttributeSignalMap mAttributeChangedSignals;
public:
  
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate& rGetter, const SetterDelegate& rSetter, const nuiRange& rRange=nuiRange()) ///< Read/write property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, false),
      mGetter(rGetter),
      mSetter(rSetter)
  {
    NGL_ASSERT(mSetter != NULL);
  }
  
  nuiAttribute(const nglString& rName, nuiAttributeUnit units, const GetterDelegate& rGetter, const nuiRange& rRange=nuiRange()) ///< Read only property
    : nuiAttributeBase(rName, nuiAttributeTypeTrait<Contents>::GetTypeId(), units, rRange, true),
    mGetter(rGetter),
    mSetter(NULL)
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

  virtual bool ToString(void* pTarget, nglString& rString) const
  {
    return false;
  }
  
  virtual bool FromString(void* pTarget, const nglString& rString) const
  {
    return false;
  }

  Contents Get(void* pTarget) const
  {
    NGL_ASSERT(mGetter);
    GetterDelegate Getter(mGetter);
    Getter.SetThis(pTarget);
    return Getter();
  }
  
  void Set(void* pTarget, Contents rValue) const
  {
    SetterDelegate Setter(mSetter);
    Setter.SetThis(pTarget);
    Setter(rValue);
    
    if (!IsAttributeChangeIgnored())
    {
      SendAttributeChanged(pTarget, rValue); 
    }
  }
	
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
	
	void Format(void* pTarget, nglString& string)
	{
		if (mFormater)
		{
			// the user has specified its own formater.
			return mFormater(pTarget, string, Get(pTarget));
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
  GetterDelegate mGetter;
  SetterDelegate mSetter;
	NewEditorDelegate mNewEditor;
	FormaterDelegate mFormater;
  
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
};




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
  
  void IgnoreAttributeChange(bool ignore);
  
  bool IsAttributeChangeIgnored() const;
  
	nuiAttributeEditor* GetEditor();

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
	
  nuiSignal1<Contents>& GetChangedSignal() const
  {
    return mpAttribute->GetChangedSignal(mpTarget);
  }
  
private:
  nuiAttribute<Contents>* mpAttribute;
};



#define DECLARE_NUIATTRIBUTE_TYPE(X)  template<> uint64 nuiAttributeTypeTrait<X>::mTypeId = GetNewAttributeUniqueId();

///////////////////////////// Specializations for simple types:

template <>
bool nuiAttribute<bool>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<bool>::FromString(void* pTarget, const nglString& rString) const;

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
bool nuiAttribute<int8>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<int8>::FromString(void* pTarget, const nglString& rString) const;



//********************************
//
// int16
//

template <>
nuiAttributeEditor* nuiAttribute<int16>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int16>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int16>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<int16>::FromString(void* pTarget, const nglString& rString) const;


//********************************
//
// int32
//

template <>
nuiAttributeEditor* nuiAttribute<int32>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int32>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int32>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<int32>::FromString(void* pTarget, const nglString& rString) const;

//********************************
//
// int64
//

template <>
nuiAttributeEditor* nuiAttribute<int64>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<int64>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<int64>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<int64>::FromString(void* pTarget, const nglString& rString) const;


//********************************
//
// uint8
//

template <>
nuiAttributeEditor* nuiAttribute<uint8>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint8>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint8>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<uint8>::FromString(void* pTarget, const nglString& rString) const;


//********************************
//
// uint16
//

template <>
nuiAttributeEditor* nuiAttribute<uint16>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint16>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint16>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<uint16>::FromString(void* pTarget, const nglString& rString) const;



//********************************
//
// uint32
//

template <>
nuiAttributeEditor* nuiAttribute<uint32>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint32>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint32>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<uint32>::FromString(void* pTarget, const nglString& rString) const;




//********************************
//
// uint64
//

template <>
nuiAttributeEditor* nuiAttribute<uint64>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<uint64>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<uint64>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<uint64>::FromString(void* pTarget, const nglString& rString) const;




//********************************
//
// float
//

template <>
nuiAttributeEditor* nuiAttribute<float>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<float>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<float>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<float>::FromString(void* pTarget, const nglString& rString) const;



//********************************
//
// double
//

template <>
nuiAttributeEditor* nuiAttribute<double>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<double>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<double>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<double>::FromString(void* pTarget, const nglString& rString) const;



//********************************
//
// nuiPosition
//

template <>
nuiAttributeEditor* nuiAttribute<nuiPosition>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<nuiPosition>::FormatDefault(void* pTarget, nglString& string);

template <>
bool nuiAttribute<nuiPosition>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiPosition>::FromString(void* pTarget, const nglString& rString) const;






//********************************
//
// nglString
//

template <>
bool nuiAttribute<nglString>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nglString>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nglString>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nglString>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nglString&
//

template <>
bool nuiAttribute<const nglString&>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<const nglString&>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nglString&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nglString&>::FormatDefault(void* pTarget, nglString& string);





//********************************
//
// nuiColor
//

template <>
bool nuiAttribute<nuiColor>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiColor>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiColor>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiColor>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nuiColor&
//

template <>
bool nuiAttribute<const nuiColor&>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<const nuiColor&>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiColor&>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<const nuiColor&>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// nuiPoint
//

template <>
bool nuiAttribute<nuiPoint>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiPoint>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiPoint>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiPoint>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// nuiRect
//

template <>
bool nuiAttribute<nuiRect>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiRect>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiRect>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiRect>::FormatDefault(void* pTarget, nglString& string);


//********************************
//
// const nuiRect&
//

template <>
bool nuiAttribute<const nuiRect&>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<const nuiRect&>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiRect&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nuiRect&>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// nuiBorder
//

template <>
bool nuiAttribute<nuiBorder>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiBorder>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiBorder>::GetDefaultEditor(void* pTarget);
template <>
void nuiAttribute<nuiBorder>::FormatDefault(void* pTarget, nglString& string);


//********************************
//
// const nuiBorder&
//

template <>
bool nuiAttribute<const nuiBorder&>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<const nuiBorder&>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiBorder&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nuiBorder&>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// nglPath
//

template <>
bool nuiAttribute<nglPath>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nglPath>::FromString(void* pTarget, const nglString& rString) const;

template <>
void nuiAttribute<nglPath>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// const nglString&
//

template <>
bool nuiAttribute<const nglPath&>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<const nglPath&>::FromString(void* pTarget, const nglString& rString) const;

template <>
void nuiAttribute<const nglPath&>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// DecorationMode
//

template <>
bool nuiAttribute<nuiDecorationMode>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiDecorationMode>::FromString(void* pTarget, const nglString& rString) const;

template <>
void nuiAttribute<nuiDecorationMode>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// ShapeMode
//

template <>
bool nuiAttribute<nuiShapeMode>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiShapeMode>::FromString(void* pTarget, const nglString& rString) const;

template <>
void nuiAttribute<nuiShapeMode>::FormatDefault(void* pTarget, nglString& string);




//********************************
//
// const nuiRange&
//

template <>
bool nuiAttribute<const nuiRange&>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<const nuiRange&>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<const nuiRange&>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<const nuiRange&>::FormatDefault(void* pTarget, nglString& string);





//********************************
//
// nuiDecorationLayer
//

template <>
bool nuiAttribute<nuiDecorationLayer>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiDecorationLayer>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiDecorationLayer>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<nuiDecorationLayer>::FormatDefault(void* pTarget, nglString& string);



//********************************
//
// nuiMouseCursor
//

template <>
bool nuiAttribute<nuiMouseCursor>::ToString(void* pTarget, nglString& rString) const;

template <>
bool nuiAttribute<nuiMouseCursor>::FromString(void* pTarget, const nglString& rString) const;

template <>
nuiAttributeEditor* nuiAttribute<nuiMouseCursor>::GetDefaultEditor(void* pTarget);

template <>
void nuiAttribute<nuiMouseCursor>::FormatDefault(void* pTarget, nglString& string);





