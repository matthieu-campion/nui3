/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiObject_h__
#define __nuiObject_h__

#include "nui.h"
#include "nuiEvent.h"
#include "nuiToken.h"
#include "nuiRefCount.h"

#ifdef _DEBUG_
#define _NUI_DEBUG_OBJECTS_
#endif

class nuiEventSource;

class nuiObject;
class nuiAttributeBase;
class nuiAttribBase;

typedef std::map<nglString, nglString,nglString::LessFunctor> nuiPropertyMap;

enum nuiSerializeMode
{
  eDontSaveNode, ///< Do not serialize this node and any of its children if any.
  eSaveNode, ///< Serialize this node and all its children (unless a child is explicitely set to be skipped or not saved.)
  eSkipNode  ///< Skip this node's serilization and only serialize its children if any.
};

/// This is the root nui class. It implements all the basic widget management system.
class NUI_API nuiObject : virtual public nuiRefCount
{
public:
  /** @name Life */
  //@{
  nuiObject(); ///< Create an nuiObject
  nuiObject(const nglString& rObjectName); ///< constructor with an object name by default
  virtual ~nuiObject(); 
  //@}

  /** @name Serialization */
  //@{
  void SetSerializeMode (nuiSerializeMode eMode); ///< Set the serialization mode for this object (see nuiSerializeMode enum documentation).
  nuiSerializeMode GetSerializeMode () const; ///< Get the serialization mode for this object (see nuiSerializeMode enum documentation).
  //@}

  /** @name Object Mode basis (class and name) */
  //@{
  const nglString& GetObjectName() const; ///< 
  virtual void SetObjectName(const nglString& rName); ///< 
  const nglString& GetObjectClass() const; ///< 
  void GetObjectInheritance(std::vector<nglString>& rClasses) const;
  bool IsOfClass(const nglString& rClass) const;
  bool IsOfClass(int32 ClassIndex) const;
  int32 GetObjectClassNameIndex() const;
  static int32 GetClassNameIndex(const nglString& rName);
  static const nglString& GetClassNameFromIndex(int32 index);
  static int32 GetClassCount();
  //@}
  
  /** @name Properties system */
  //@{
  virtual void SetProperty(const nglString& rName, const nglString& rValue); ///< Add or change a property of the object.
  void SetProperty(const char* pName, const nglString& rValue); ///< Add or change a property of the object.
  void SetProperty(const char* pName, const char* pValue); ///< Add or change a property of the object.
  nglString GetProperty(const nglString& rName) const; ///< Return the property value corresponding to the given property name. If the object doesn't have the property the returned string is empty.
  nglString GetProperty(const char* pName) const;
  bool HasProperty(const nglString& rName) const; ///< Return true if the object contains the property.
  bool HasProperty(const char* pName) const; ///< Return true if the object contains the property.

  nuiSimpleEventSource<0> DebugRefreshInfo; ///< Send an event when the object has changed. This event is only used in debug mode.

  void SetToken(nuiTokenBase* pToken);
  nuiTokenBase* GetToken() const;
	
	/** @name Attributes system */
	//@{
	void GetAttributes(std::map<nglString, nuiAttribBase>& rAttributeMap) const;
	static void GetAttributesOfClass(uint32 ClassIndex, std::map<nglString, nuiAttributeBase*>& rAttributeMap);
	void GetSortedAttributes(std::list<nuiAttribBase>& rListToFill) const;
  nuiAttribBase GetAttribute(const nglString& rName) const;
  void AddInstanceAttribute(const nglString& rName, nuiAttributeBase* pProperty); ///< Add an attribute to this object (beware, only this instance of this class will have this attribute. If you wnat the attribute to be global to all instances of the class use AddAttribute instead).
  void AddInstanceAttribute(nuiAttributeBase* pAttribute); ///< Add an attribute to this object (beware, only this instance of this class will have this attribute. If you wnat the attribute to be global to all instances of the class use AddAttribute instead).
  //@}
  
  ///! Global Properties:
  static void SetGlobalProperty(const nglString& rName, const nglString& rValue); ///< Add or change a Global property.
  static void SetGlobalProperty(const char* pName, const nglString& rValue); ///< Add or change a Global property.
  static void SetGlobalProperty(const char* pName, const char* pValue); ///< Add or change a Global property.
  static const nglString& GetGlobalProperty(const nglString& rName); ///< Return the property value corresponding to the given Global property name. If the the Global property doesn't exists the returned string is empty.
  static const nglString& GetGlobalProperty(const char* pName);
  static bool IsGlobalProperty(const nglString& rName); ///< Return true if the Global property exists.
  static bool IsGlobalProperty(const char* pName); ///< Return true if the Global property exists.
  static bool ClearGlobalProperties(); ///< Remove all the global properties.
  static bool GetGlobalProperties(std::list<nglString>& rPropertyNames); ///< Populate @param rPropertyNames with the name of the global properties.
  static bool ClearGlobalProperty(const nglString& rName); ///< Remove the given global property.
  static bool ClearGlobalProperty(const char* pName); ///< Remove the given global property.

  inline void CheckValid() const
  {
#ifdef _NUI_DEBUG_OBJECTS_
    if (!mpTrace)
      printf("Object bugged: CLASS:%s NAME:%s\n", mObjectClassNames[mClassNameIndex].GetChars(), mObjectName.GetChars());
    NGL_ASSERT(mpTrace);
#else
    if (mpTrace)
#endif
      CheckValidInternal();
  }
  
protected:
  void CheckValidInternal() const;
  static std::vector<int32> mInheritanceMap;
  virtual bool SetObjectClass(const nglString& rClass); ///< does a SetProperty("Class"...). Returns true if this is the first time an object of this class is registered.
  void InitProperties(); ///< Take care of the property bindings.

	/** @name Attributes system */
	//@{
  void AddAttribute(const nglString& rName, nuiAttributeBase* pProperty); ///< Add an attribute to this class (beware, all instances of this class will have this attribute. If you wnat the attribute to be private to this instance of the class use AddInstanceAttribute instead).
  void AddAttribute(nuiAttributeBase* pAttribute); ///< Add an attribute to this class (beware, all instances of this class will have this attribute. If you wnat the attribute to be private to this instance of the class use AddInstanceAttribute instead).
  //@}
  
  static nuiPropertyMap mGlobalProperties;

  nuiSerializeMode mSerializeMode;
  
private : 
  void Init(const nglString& rObjectName);
  void InitAttributes();
  
  nuiTokenBase* mpToken;
  //std::map<nglString,nuiAttributeBase*> mAttributes;
  static uint32 mUniqueAttributeOrder; // to handle properties's order
  
  static std::vector<nglString> mObjectClassNames;
  static std::vector<std::map<nglString, nuiAttributeBase*> > mClassAttributes;
  std::map<nglString, nuiAttributeBase*> mInstanceAttributes;
  static std::map<nglString, int32> mObjectClassNamesMap;
  
  uint32 mClassNameIndex;
  nglString mObjectName;
  
protected:
  class Trace
  {
  public:
    nglString mClass;
    nglString mName;
    bool mAlive;
  };
  static std::map<nuiObject*, Trace> mObjects;
  static nglCriticalSection gObjectTraceCS;
  Trace* mpTrace;
};

//typedef nuiSmartPtr<nuiObject> nuiObjectPtr;
typedef nuiObject* nuiObjectPtr;
typedef std::list<nuiObjectPtr> nuiObjectList;

#define NUI_PROP(x,y,z) {_T(x),_T(y),_T(z),true,true} ///< Define a property as exportable and writable by the user.
#define NUI_PROPEX(x,y,z,exportable,writable) {_T(x),_T(y),_T(z),exportable,writable} ///< Define a property and let the programmer choose the acces rights to it.

#endif // __nuiObject_h__
