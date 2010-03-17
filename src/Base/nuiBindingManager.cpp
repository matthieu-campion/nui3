/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiBindingManager.h"
#include "nuiTypeTraits.h"

NUI_DECLARE_ATTRIBUTE_TYPE(nuiWidget*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiObject*);

class nuiVariant
{
public:
  // POD CTOR
  template <typename Type>
  nuiVariant(Type data)
  {
    mIsPointer = is_pointer<Type>::value;
    mType = nuiAttributeTypeTrait<Type>::mTypeId;
    if (is_same_type<Type, int8>::value || is_same_type<Type, int16>::value || is_same_type<Type, int32>::value || is_same_type<Type, int64>::value)
    {
      mData.mInt = data;
    }
    else if (is_same_type<Type, uint8>::value || is_same_type<Type, uint16>::value || is_same_type<Type, uint32>::value || is_same_type<Type, uint64>::value)
    {
      mData.mUInt = data;
    }
    else if (is_same_type<Type, float>::value || is_same_type<Type, double>::value)
    {
      mData.mFloat = data;
    }
  }

  // Pointer CTOR
  template <typename Type>
  nuiVariant(Type* pData)
  {
    mIsPointer = true;
    mIsObject = is_base_of<nuiObject*, Type*>::value;
    mType = nuiAttributeTypeTrait<Type*>::mTypeId;
    mData.mpPointer = (void*)pData;
  }
  
  // Classes CTORs
  nuiVariant(const nglString& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nglString>::mTypeId;
    mString = rData;
  }
  
  nuiVariant(const nglPath& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nglPath>::mTypeId;
    mPath = rData;
  }
  
  nuiVariant(const nuiColor& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiColor>::mTypeId;
    mColor = rData;
  }
  
  nuiVariant(const nuiRect& rData)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<nuiRect>::mTypeId;
    mRect = rData;
  }
  
  // Copy CTOR
  nuiVariant(const nuiVariant& rObject)
  {
    mData = rObject.mData;
    
    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;
    mPath = rObject.mPath;
    
    mType = rObject.mType;
    
    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
  }
  
  nuiVariant& operator=(const nuiVariant& rObject)
  {
    mData = rObject.mData;
    
    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;
    mPath = rObject.mPath;
    
    mType = rObject.mType;
    
    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    return *this;
  }

  nuiAttributeType GetType() const
  {
    return mType;
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
      return mPath.GetPathName();
    
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

  // POD Cast:
  template<typename Type>
  operator Type() const
  {
    if (nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<float>::mTypeId
     || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<double>::mTypeId)
    {
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType == nuiAttributeTypeTrait<double>::mTypeId)
        return (Type)mData.mFloat;
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
        return (Type)mString.GetCDouble();
    }
    else if (nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int8>::mTypeId 
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int16>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int32>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<int64>::mTypeId)
    {
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)
        return (Type)mData.mInt;
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
        return (Type)mString.GetCInt64();
    }
    else if (nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint8>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint16>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint32>::mTypeId
             || nuiAttributeTypeTrait<Type>::mTypeId == nuiAttributeTypeTrait<uint64>::mTypeId)
    {
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType == nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType == nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)
        return (Type)mData.mUInt;
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)
        return (Type)mString.GetCUInt64();
    }

    NGL_ASSERT(0);
  }
  
  template<typename Type>
  operator Type*() const
  {
    if (nuiAttributeTypeTrait<Type*>::mTypeId == mType)
      return reinterpret_cast<Type*>(mData.mpPointer);
    
    if (!mIsPointer)
      return NULL;
    
    if (mIsObject && is_base_of<nuiObject, Type>::value)
      return dynamic_cast<Type*>(reinterpret_cast<nuiObject*>(mData.mpPointer));
    
    return NULL;
  }
    
private:
  nuiAttributeType mType;
  union 
  {
    int64 mInt;
    uint64 mUInt;
    double mFloat;

    void* mpPointer;
    nuiObject* mpObject;    
  } mData;

  nglString mString;
  nuiRect mRect;
  nglPath mPath;
  nuiColor mColor;
  
  bool mIsPointer : 1;
  bool mIsObject : 1;
};

#if 0
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class variant_t
{
public :
  variant_t()
  : data(NULL)
  {
  }
  
  variant_t(const variant_t & rhs)
  {
    if (rhs.data != NULL)
      rhs.data->AddRef();
    data = rhs.data;
  }

  ~variant_t()
  {
    if (data != NULL)
    data->Release();
  }

  // NOTE: This code takes care of self-asignment.
  // DO NOT CHANGE THE ORDER of the statements.
  variant_t& operator = (const variant_t& rhs)
  {
    if (rhs.data != NULL)
      rhs.data->AddRef();
    if (data != NULL)
      data->Release();
    data = rhs.data;
    return * this;
  }

  // This member template constructor allows you to
  // instance a variant_t object with a value of any type.
  template<typename T> variant_t(T v)
  : data(new Impl<T>(v))
  {
    data->AddRef();
  }
  
  // This generic conversion operator let you retrieve
  // the value held. To avoid template specialization conflicts,
  // it returns an instance of type T, which will be a COPY
  // of the value contained.
  template<typename T> operator T() const
  {
    return CastFromBase<T>(data)->data;
  }
  
  // This forms returns a REFERENCE and not a COPY, which
  // will be significant in some cases.
  template<typename T> const T & get() const
  {
    return CastFromBase<T>(data)->data;
  }
  
  template<typename T> bool is_type() const
  {
    return typeid(*data)==typeid(Impl<T>);
  }
  
  template<typename T> bool is_type(T v) const
  {
    return typeid(*data)==typeid(v);
  }
  
private :
  struct ImplBase
  {
    ImplBase() : refs(0)
    {
    }
    
    virtual ~ImplBase()
    {
    }
    
    void AddRef()
    {
      refs ++;
    }
    
    void Release()
    {
      refs --;
      if (refs == 0)
        delete this;
    }
    
#undef IMPL_CAST(X)
#define IMPL_CAST(X) \
virtual operator X() = 0;
    IMPL_CAST(int8);
    IMPL_CAST(int16);
    IMPL_CAST(int32);
    IMPL_CAST(int64);

    IMPL_CAST(uint8);
    IMPL_CAST(uint16);
    IMPL_CAST(uint32);
    IMPL_CAST(uint64);
    
    IMPL_CAST(float);
    IMPL_CAST(double);

    IMPL_CAST(nglString);

    IMPL_CAST(nuiRect);
    IMPL_CAST(nuiColor);
    
    IMPL_CAST(nuiWidget*);
    IMPL_CAST(nuiObject*);
    IMPL_CAST(nuiContainer*);
    IMPL_CAST(nuiTopLevel*);
    IMPL_CAST(nuiSimpleContainer*);
    IMPL_CAST(nuiMainWindow*);
    IMPL_CAST(nuiFont*);
    IMPL_CAST(nuiTexture*);
    size_t refs;
  };

  template<typename T>
  struct Impl : ImplBase
  {
    Impl(T v) : data(v)
    {
    }
    
    ~Impl()
    {
    }

#undef IMPL_CAST(X)
#define IMPL_CAST(X) \
    operator X() \
    {\
      return convert_cast<X, T>(data);\
    }
    //      if (is_pointer<T>::value && is_same_type<T, X*>::value)\
    //        return data;\
    //      else if (is_base_of<T, X*>::value || is_base_of<X*, T>::value)\
    //        return dynamic_cast<X*>(data);\
    //      else\
    //        return NULL;\
    
    IMPL_CAST(int8);
    IMPL_CAST(int16);
    IMPL_CAST(int32);
    IMPL_CAST(int64);
    
    IMPL_CAST(uint8);
    IMPL_CAST(uint16);
    IMPL_CAST(uint32);
    IMPL_CAST(uint64);
    
    IMPL_CAST(float);
    IMPL_CAST(double);
    
    IMPL_CAST(nglString);
    
    IMPL_CAST(nuiRect);
    IMPL_CAST(nuiColor);
    
    IMPL_CAST(nuiWidget*);
    IMPL_CAST(nuiObject*);
    IMPL_CAST(nuiContainer*);
    IMPL_CAST(nuiTopLevel*);
    IMPL_CAST(nuiSimpleContainer*);
    IMPL_CAST(nuiMainWindow*);
    IMPL_CAST(nuiFont*);
    IMPL_CAST(nuiTexture*);
    
    T data;
  };

  // The following method is static because it doesn't
  // operate on variant_t instances.
  template<typename T>
  static Impl<T>* CastFromBase(ImplBase* v)
  {
    // This upcast will fail if T is other than the T used
    // with the constructor of variant_t.
    Impl<T>* p = dynamic_cast<Impl<T>*>(v);
//    if (p == NULL)
//      throw std::exception(typeid(T).name()+std::string(" is not a valid type"));
    return p;
  }
  
  ImplBase* data;
};
#endif

void TestVariant()
{
#if 1
  {  
    nuiVariant s1(12);
    nuiVariant s2(12.34);
    nuiVariant s3(-10);
    nuiVariant s4(nglString(_T("ProutString")));
    nuiVariant s5(new nuiObject());
    nuiVariant s6(new nuiWidget());
    
    int r = s1;
    nglString r0 = s3;
    nglString r1 = s4;
    nuiObject* pObj1 = s5;
    nuiObject* pObj2 = s6;
    nuiWidget* pW1 = s5;
    nuiWidget* pW2 = s6;
  }
#endif
  
  
#if 0
  {  
    variant_t s1(12);
    variant_t s2(12.34);
    variant_t s3(-10);
    variant_t s4(nglString(_T("ProutString")));
    variant_t s5(new nuiObject());
    variant_t s6(new nuiWidget());
    
    int r = s1;
    nglString r0 = s3;
    nglString r1 = s4;
    nuiObject* pObj1 = s5;
    nuiObject* pObj2 = s6;
    nuiWidget* pW1 = s5;
    nuiWidget* pW2 = s6;
  }
#endif
}

//class  nuiClass:
nuiClass::nuiClass(const nglString& rName)
{
  mName = rName;
}

nuiClass::~nuiClass()
{
  // Delete functions:
  {
    std::multimap<nglString, nuiFunction*>::iterator it = mMethods.begin();
    std::multimap<nglString, nuiFunction*>::iterator end = mMethods.end();

    while (it != end)
    {
      delete it->second;
      ++it;
    }
  }
  
  // Delete constructors:
  {
    std::set<nuiFunction*>::iterator it = mConstructors.begin();
    std::set<nuiFunction*>::iterator end = mConstructors.end();

    while (it != end)
    {
      delete *it;
      ++it;
    }
  }
}

void nuiClass::AddParentClass(nuiClass* pParentClass)
{
  mParentClasses.push_back(pParentClass);
}

void nuiClass::AddMethod(const nglString& rMethodName, nuiFunction* pFunction)
{
  mMethods.insert(std::pair<nglString, nuiFunction*>(rMethodName, pFunction));
}

void nuiClass::AddConstructor(nuiFunction* pFunction)
{
  mConstructors.insert(pFunction);
}

const std::vector<nuiClass*>& nuiClass::GetParentClasse() const
{
  return mParentClasses;
}

const std::multimap<nglString, nuiFunction*>& nuiClass::GetMethods() const
{
  return mMethods;
}

const std::set<nuiFunction*>& nuiClass::GetConstructors() const
{
  return mConstructors;
}

const nglString& nuiClass::GetName() const
{
  return mName;
}

static void BuildTypeListString(uint StartIndex, const std::vector<nglString>& rArgs, nglString& rString)
{
  for (uint i = StartIndex; i < rArgs.size(); i++)
  {
    if (i != StartIndex)
      rString.Add(_T(", "));

    rString.Add(rArgs[i]);
  }
}

void nuiClass::Dump(nglString& rString) const
{
  rString.Wipe();

  // Class header:
  rString.Add(_T("class ")).Add(mName).AddNewLine();

  // Dump inheritances:
  {
    std::vector<nuiClass*>::const_iterator it = mParentClasses.begin();
    std::vector<nuiClass*>::const_iterator end = mParentClasses.end();

    while (it != end)
    {
      nuiClass* pClass = *it;

      rString.Add(_T("\t: public ")).Add(pClass->GetName()).AddNewLine();

      ++it;
    }
  }

  rString.Add(_T("{")).AddNewLine().Add(_T("public: // Construtors:")).AddNewLine();

  // Dump Ctors:
  {
    std::set<nuiFunction*>::const_iterator it = mConstructors.begin();
    std::set<nuiFunction*>::const_iterator end = mConstructors.end();

    while (it != end)
    {
      std::vector<nglString> argstypes;
      nuiFunction* pTypeContainer = *it;
      pTypeContainer->DumpArgs(argstypes);
      nglString args;
      BuildTypeListString(1, argstypes, args);
      rString.Add(_T("\t")).Add(mName).Add(_T("(")).Add(args).Add(_T(");")).AddNewLine();
      ++it;
    }
  }

  rString.AddNewLine().Add(_T("public: // Methods:")).AddNewLine();
  // Dump methods:
  {
    std::multimap<nglString, nuiFunction*>::const_iterator it = mMethods.begin();
    std::multimap<nglString, nuiFunction*>::const_iterator end = mMethods.end();

    while (it != end)
    {
      std::vector<nglString> argstypes;
      nglString name(it->first);
      nuiTypeContainer* pTypeContainer = it->second;
      pTypeContainer->DumpArgs(argstypes);
      nglString args;
      BuildTypeListString(1, argstypes, args);
      rString.Add(_T("\t")).Add(argstypes[0]).Add(" ").Add(name).Add(_T("(")).Add(args).Add(_T(");")).AddNewLine();
      ++it;
    }
  }

  rString.Add(_T("};\n"));
}

//class nuiBindingManager
nuiBindingManager::nuiBindingManager()
{
}

nuiBindingManager::~nuiBindingManager()
{
  // Delete functions:
  {
    FunctionMap::iterator it = mFunctions.begin();
    FunctionMap::iterator end = mFunctions.end();

    while (it != end)
    {
      delete it->second;
      ++it;
    }
  }

  // Delete functions:
  {
    ClassMap::iterator it = mClasses.begin();
    ClassMap::iterator end = mClasses.end();

    while (it != end)
    {
      delete it->second;
      ++it;
    }
  }
}

void nuiBindingManager::AddFunction(const nglString& rFunctionName, nuiFunction* pFunction)
{
  mFunctions.insert(std::pair<nglString, nuiFunction*>(rFunctionName, pFunction));
}

void nuiBindingManager::AddClass(nuiClass* pClass)
{
  mClasses.insert(std::pair<nglString, nuiClass*>(pClass->GetName(), pClass));
}

void nuiBindingManager::Dump(nglString& rString) const
{
  //std::map<nglString, nuiFunction*> mFunctions;

  // Dump functions:
  {
    FunctionMap::const_iterator it = mFunctions.begin();
    FunctionMap::const_iterator end = mFunctions.end();
    
    while (it != end)
    {
      std::vector<nglString> argstypes;
      nglString name(it->first);
      nuiTypeContainer* pTypeContainer = it->second;
      pTypeContainer->DumpArgs(argstypes);
      nglString args;
      BuildTypeListString(1, argstypes, args);
      rString.Add(argstypes[0]).Add(" ").Add(name).Add(_T("(")).Add(args).Add(_T(");")).AddNewLine();
      ++it;
    }
  }
  
  
  // Classes:
  ClassMap::const_iterator it = mClasses.begin();
  ClassMap::const_iterator end = mClasses.end();

  while (it != end)
  {
    nglString name = it->first;
    nuiClass* pClass = it->second;
    nglString classdump;
    pClass->Dump(classdump);

    rString.AddNewLine().AddNewLine().Add(classdump);

    ++it;
  }

}

nuiClass* nuiBindingManager::GetClass(const nglString& rClassName) const
{
  ClassMap::const_iterator it = mClasses.find(rClassName);
  if (it == mClasses.end())
    return NULL;
  return it->second;
}


nuiBindingManager nuiBindingManager::mManager;

