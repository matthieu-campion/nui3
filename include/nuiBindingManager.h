/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once
#include "nui.h"
#include "nuiTypeTraits.h"
#include "nuiVariant.h"

template <class Type> class nuiVariantTrait
{
public:
  static const nglChar* GetName()
  {
    return NULL;
  }
  
 };


///////////////////////////////////// Traits implems:


template <class Class> class nuiVariantTrait<Class&>
{
  static nglString mName;
public:  
  static const nglChar* GetName()
  {
    if (mName.IsNull())
    {
      mName.Add(typeid(Class).name()).Add(_T("&"));
    }
    return mName.GetChars();
  }
};

template <class Class> class nuiVariantTrait<const Class&>
{
  static nglString mName;
public:  
  static const nglChar* GetName()
  {
    if (mName.IsNull())
    {
      mName.Add(_T("const ")).Add(typeid(Class).name()).Add(_T("&"));
    }
    return mName.GetChars();
  }
};

template <class Class> class nuiVariantTrait<Class*>
{
  static nglString mName;
public:  
  static const nglChar* GetName()
  {
    if (mName.IsNull())
    {
      mName.Add(typeid(Class).name()).Add(_T("*"));
    }
    return mName.GetChars();
  }
};

template <class Class> nglString nuiVariantTrait<Class&>::mName;
template <class Class> nglString nuiVariantTrait<const Class&>::mName;

template <class Class> nglString nuiVariantTrait<Class*>::mName;



template <> class nuiVariantTrait<int>
{
public:  
  static const nglChar* GetName()
  {
    return _T("int");
  }
};

template <> class nuiVariantTrait<void>
{
public:  
  static const nglChar* GetName()
  {
    return _T("void");
  }
};

template <> class nuiVariantTrait<float>
{
public:  
  static const nglChar* GetName()
  {
    return _T("float");
  }
};


template <> class nuiVariantTrait<nglString>
{
public:  
  static const nglChar* GetName()
  {
    return _T("nglString");
  }
};

class nuiCallContext
{
public:
  nuiCallContext()
  {
  }
  ~nuiCallContext()
  {
  }

  void SetArguments(const std::vector<nuiVariant>& rArgs)
  {
    mArgs = rArgs;
  }
  
  void AddArguments(const std::vector<nuiVariant>& rArgs)
  {
    mArgs.reserve(mArgs.size() + rArgs.size());
    for (uint32 i = 0; i < rArgs.size(); i++)
      AddArgument(rArgs[i]);
  }
  
  void AddArgument(const nuiVariant& rArg)
  {
    mArgs.push_back(rArg);
  }
  
  nuiVariant& operator[](uint32 index)
  {
    return mArgs[index];
  }
  
  const nuiVariant& GetResult() const
  {
    return mResult;
  }
  
  nuiVariant& GetResult()
  {
    return mResult;
  }
  
private:
  std::vector<nuiVariant> mArgs;
  nuiVariant mResult;
};


class nuiTypeContainer
{
public:
  virtual void DumpArgs(std::vector<nglString>& rTypes) const = 0;
};

///! The nuiRunable class defines only a pure virtual method to Run a function from a set of params and store the result in a return object.
class nuiRunable : public nuiTypeContainer
{
public:
  virtual bool Run(nuiCallContext& rContext) = 0;
};


///! define all the nuiFunctionStorageXX class (one per number of arguments).
template <typename RetType> class nuiFunctionStorage0 : public nuiRunable
{
private:
  typedef RetType (*FunctionType)();
  FunctionType mStorage;
public:
  nuiFunctionStorage0(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    if (is_same_type<RetType, void>::value)
    {
      rContext.GetResult().Clear();
      (*mStorage)();
    }
    else
    {
      rContext.GetResult() = (*mStorage)();
    }
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
  }
};

template<> class nuiFunctionStorage0<void> : public nuiRunable
{
private:
  typedef void (*FunctionType)();
  FunctionType mStorage;
public:
    nuiFunctionStorage0(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    (*mStorage)();
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
  }
};


template <typename RetType, class Param1> class nuiFunctionStorage1 : public nuiRunable
{
private:
  typedef RetType (*FunctionType)(Param1 param1);
  FunctionType mStorage;
public:
  nuiFunctionStorage1(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult() = (*mStorage)(rContext[0]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
  }
};

template<class Param1> class nuiFunctionStorage1<void, Param1> : public nuiRunable
{
private:
  typedef void (*FunctionType)(Param1 param1);
  FunctionType mStorage;
public:
  nuiFunctionStorage1(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    (*mStorage)(rContext[0]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
  }
};

template <typename RetType, class Param1, class Param2> class nuiFunctionStorage2 : public nuiRunable
{
private:
  typedef RetType (*FunctionType)(Param1 param1, Param2 param2);
  FunctionType mStorage;
public:
  nuiFunctionStorage2(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult() = (*mStorage)(rContext[0], rContext[1]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
  }
};

template<class Param1, class Param2> class nuiFunctionStorage2<void, Param1, Param2> : public nuiRunable
{
private:
  typedef void (*FunctionType)(Param1 param1, Param2 param2);
  FunctionType mStorage;
public:
    nuiFunctionStorage2(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    (*mStorage)(rContext[0], rContext[1]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
  }
};


template <typename RetType, class Param1, class Param2, class Param3> class nuiFunctionStorage3 : public nuiRunable
{
private:
  typedef RetType (*FunctionType)(Param1 param1, Param2 param2, Param3 param3);
  FunctionType mStorage;
public:
  nuiFunctionStorage3(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult() = (*mStorage)(rContext[0], rContext[1], rContext[2]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
  }
};

template<class Param1, class Param2, class Param3> class nuiFunctionStorage3<void, Param1, Param2, Param3> : public nuiRunable
{
private:
  typedef void (*FunctionType)(Param1 param1, Param2 param2, Param3 param3);
  FunctionType mStorage;
public:
    nuiFunctionStorage3(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    (*mStorage)(rContext[0], rContext[1], rContext[2]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
  }
};

template <typename RetType, class Param1, class Param2, class Param3, class Param4> class nuiFunctionStorage4 : public nuiRunable
{
private:
  typedef RetType (*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4);
  FunctionType mStorage;
public:
  nuiFunctionStorage4(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult() = (*mStorage)(rContext[0], rContext[1], rContext[2], rContext[3]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
  }
};

template<class Param1, class Param2, class Param3, class Param4> class nuiFunctionStorage4<void, Param1, Param2, Param3, Param4> : public nuiRunable
{
private:
  typedef void (*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4);
  FunctionType mStorage;
public:
  nuiFunctionStorage4(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    (*mStorage)(rContext[0], rContext[1], rContext[2], rContext[3]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
  }
};


template <typename RetType, class Param1, class Param2, class Param3, class Param4, class Param5> class nuiFunctionStorage5 : public nuiRunable
{
private:
  typedef RetType (*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param4 param5);
  FunctionType mStorage;
public:
  nuiFunctionStorage5(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult() = (*mStorage)(rContext[0], rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
    rTypes.push_back(nuiVariantTrait<Param5>::GetName());
  }
};

template<class Param1, class Param2, class Param3, class Param4, class Param5> class nuiFunctionStorage5<void, Param1, Param2, Param3, Param4, Param5> : public nuiRunable
{
private:
  typedef void (*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5);
  FunctionType mStorage;
public:
    nuiFunctionStorage5(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    (*mStorage)(rContext[0], rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
    rTypes.push_back(nuiVariantTrait<Param5>::GetName());
  }
};


///! define all the nuiFunctionStorageXX class (one per number of arguments).
template <class Class, typename RetType> class nuiMethodStorage0 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)();
  FunctionType mStorage;
public:
  nuiMethodStorage0(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)();
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
  }
};

template <class Class> class nuiMethodStorage0<Class, void> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)();
  FunctionType mStorage;
public:
    nuiMethodStorage0(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)();
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
  }
};


template <class Class, typename RetType, class Param1> class nuiMethodStorage1 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1);
  FunctionType mStorage;
public:
  nuiMethodStorage1(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
  }
};

template <class Class, class Param1>
class nuiMethodStorage1<Class, void, Param1> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1);
  FunctionType mStorage;
public:
  nuiMethodStorage1(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    Class* pClass = rContext[0];
    (pClass->*mStorage)(rContext[1]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
  }
};

template <class Class, typename RetType, class Param1, class Param2>
class nuiMethodStorage2 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2);
  FunctionType mStorage;
public:
  nuiMethodStorage2(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
  }
};

template <class Class, class Param1, class Param2>
class nuiMethodStorage2<Class, void, Param1, Param2> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2);
  FunctionType mStorage;
public:
  nuiMethodStorage2(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    Class* pClass = rContext[0];
    (pClass->*mStorage)(rContext[1], rContext[2]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
  }
};


template <class Class, typename RetType, class Param1, class Param2, class Param3>
class nuiMethodStorage3 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3);
  FunctionType mStorage;
public:
  nuiMethodStorage3(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2], rContext[3]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
  }
};

template <class Class, class Param1, class Param2, class Param3>
class nuiMethodStorage3<Class, void, Param1, Param2, Param3> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3);
  FunctionType mStorage;
public:
    nuiMethodStorage3(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    (pClass->*mStorage)(rContext[1], rContext[2], rContext[3]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
  }
};


template <class Class, typename RetType, class Param1, class Param2, class Param3, class Param4>
class nuiMethodStorage4 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4);
  FunctionType mStorage;
public:
  nuiMethodStorage4(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
  }
};

template <class Class, class Param1, class Param2, class Param3, class Param4>
class nuiMethodStorage4<Class, void, Param1, Param2, Param3, Param4> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4);
  FunctionType mStorage;
public:
    nuiMethodStorage4(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
  }
};


template <class Class, typename RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class nuiMethodStorage5 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param4 param5);
  FunctionType mStorage;
public:
    nuiMethodStorage5(FunctionType t)
  {
      mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
    rTypes.push_back(nuiVariantTrait<Param5>::GetName());
  }
};


template <class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
class nuiMethodStorage5<Class, void, Param1, Param2, Param3, Param4, Param5> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param4 param5);
  FunctionType mStorage;
public:
  nuiMethodStorage5(FunctionType t)
  {
    mStorage = t;
  }
  
  bool Run(nuiCallContext& rContext)
  {
    rContext.GetResult().Clear();
    Class* pClass = rContext[0];
    (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4], rContext[5]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
    rTypes.push_back(nuiVariantTrait<Param5>::GetName());
  }
};

// Const versions of the method storages:
template <class Class, typename RetType> class nuiConstMethodStorage0 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)() const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage0(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)();
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
  }
};

template <class Class> class nuiConstMethodStorage0<Class, void> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)() const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage0(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)();
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
  }
};


template <class Class, typename RetType, class Param1> class nuiConstMethodStorage1 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage1(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
  }
};

template <class Class, class Param1>
class nuiConstMethodStorage1<Class, void, Param1> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage1(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)(rContext[1]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
  }
};

template <class Class, typename RetType, class Param1, class Param2>
class nuiConstMethodStorage2 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage2(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
  }
};

template <class Class, class Param1, class Param2>
class nuiConstMethodStorage2<Class, void, Param1, Param2> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage2(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)(rContext[1], rContext[2]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
  }
};


template <class Class, typename RetType, class Param1, class Param2, class Param3>
class nuiConstMethodStorage3 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage3(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2], rContext[3]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
  }
};

template <class Class, class Param1, class Param2, class Param3>
class nuiConstMethodStorage3<Class, void, Param1, Param2, Param3> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage3(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)(rContext[1], rContext[2], rContext[3]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
  }
};


template <class Class, typename RetType, class Param1, class Param2, class Param3, class Param4>
class nuiConstMethodStorage4 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage4(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
  }
};

template <class Class, class Param1, class Param2, class Param3, class Param4>
class nuiConstMethodStorage4<Class, void, Param1, Param2, Param3, Param4> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage4(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
  }
};


template <class Class, typename RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class nuiConstMethodStorage5 : public nuiRunable
{
private:
  typedef RetType (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param4 param5) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage5(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult() = (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4], rContext[5]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<RetType>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
    rTypes.push_back(nuiVariantTrait<Param5>::GetName());
  }
};


template <class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
class nuiConstMethodStorage5<Class, void, Param1, Param2, Param3, Param4, Param5> : public nuiRunable
{
private:
  typedef void (Class::*FunctionType)(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param4 param5) const;
  FunctionType mStorage;
public:
  nuiConstMethodStorage5(FunctionType t)
  {
    mStorage = t;
  }

  bool Run(nuiCallContext& rContext)
  {
    const Class* pClass = rContext[0];
    rContext.GetResult().Clear();
    (pClass->*mStorage)(rContext[1], rContext[2], rContext[3], rContext[4], rContext[5]);
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiVariantTrait<void>::GetName());
    rTypes.push_back(nuiVariantTrait<Param1>::GetName());
    rTypes.push_back(nuiVariantTrait<Param2>::GetName());
    rTypes.push_back(nuiVariantTrait<Param3>::GetName());
    rTypes.push_back(nuiVariantTrait<Param4>::GetName());
    rTypes.push_back(nuiVariantTrait<Param5>::GetName());
  }
};




class nuiFunction : public nuiRunable
{
public:
  template <typename RetType> nuiFunction(RetType (*pFunction)())
  {
    mArgC = 0;
    mpStorage = new nuiFunctionStorage0<RetType>(pFunction);
  }
  
  template <typename RetType, class Param1> nuiFunction(RetType (*pFunction)(Param1))
  {
    mArgC = 1;
    mpStorage = new nuiFunctionStorage1<RetType, Param1>(pFunction);
  }
  
  template <typename RetType, class Param1, class Param2> nuiFunction(RetType (*pFunction)(Param1, Param2))
  {
    mArgC = 2;
    mpStorage = new nuiFunctionStorage2<RetType, Param1, Param2>(pFunction);
  }
  
  template <typename RetType, class Param1, class Param2, class Param3> nuiFunction(RetType (*pFunction)(Param1, Param2, Param3))
  {
    mArgC = 3;
    mpStorage = new nuiFunctionStorage3<RetType, Param1, Param2, Param3>(pFunction);
  }
  
  template <typename RetType, class Param1, class Param2, class Param3, class Param4> nuiFunction(RetType (*pFunction)(Param1, Param2, Param3, Param4))
  {
    mArgC = 4;
    mpStorage = new nuiFunctionStorage4<RetType, Param1, Param2, Param3, Param4>(pFunction);
  }
  
  template <typename RetType, class Param1, class Param2, class Param3, class Param4, class Param5> nuiFunction(RetType (*pFunction)(Param1, Param2, Param3, Param4, Param5))
  {
    mArgC = 5;
    mpStorage = new nuiFunctionStorage5<RetType, Param1, Param2, Param3, Param4, Param5>(pFunction);
  }
  
  /////////
  template <typename RetType, class Class> nuiFunction(RetType (Class::*pFunction)())
  {
    mArgC = 0;
    mpStorage = new nuiMethodStorage0<Class, RetType>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1> nuiFunction(RetType (Class::*pFunction)(Param1))
  {
    mArgC = 1;
    mpStorage = new nuiMethodStorage1<Class, RetType, Param1>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2> nuiFunction(RetType (Class::*pFunction)(Param1, Param2))
  {
    mArgC = 2;
    mpStorage = new nuiMethodStorage2<Class, RetType, Param1, Param2>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2, class Param3> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3))
  {
    mArgC = 3;
    mpStorage = new nuiMethodStorage3<Class, RetType, Param1, Param2, Param3>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2, class Param3, class Param4> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4))
  {
    mArgC = 4;
    mpStorage = new nuiMethodStorage4<Class, RetType, Param1, Param2, Param3, Param4>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2, class Param3, class Param4, class Param5> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4, Param5))
  {
    mArgC = 5;
    mpStorage = new nuiMethodStorage5<Class, RetType, Param1, Param2, Param3, Param4, Param5>(pFunction);
  }
  
  /////////
  template <typename RetType, class Class> nuiFunction(RetType (Class::*pFunction)() const)
  {
    mArgC = 0;
    mpStorage = new nuiConstMethodStorage0<const Class, RetType>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1> nuiFunction(RetType (Class::*pFunction)(Param1) const)
  {
    mArgC = 1;
    mpStorage = new nuiConstMethodStorage1<const Class, RetType, Param1>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2> nuiFunction(RetType (Class::*pFunction)(Param1, Param2) const)
  {
    mArgC = 2;
    mpStorage = new nuiConstMethodStorage2<const Class, RetType, Param1, Param2>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2, class Param3> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3) const)
  {
    mArgC = 3;
    mpStorage = new nuiConstMethodStorage3<const Class, RetType, Param1, Param2, Param3>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2, class Param3, class Param4> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4) const)
  {
    mArgC = 4;
    mpStorage = new nuiConstMethodStorage4<const Class, RetType, Param1, Param2, Param3, Param4>(pFunction);
  }
  
  template <typename RetType, class Class, class Param1, class Param2, class Param3, class Param4, class Param5> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4, Param5) const)
  {
    mArgC = 5;
    mpStorage = new nuiConstMethodStorage5<const Class, RetType, Param1, Param2, Param3, Param4, Param5>(pFunction);
  }

  //////////////
  bool Run(nuiCallContext& rContext)
  {
    if (!mpStorage)
      return false;
    
    return mpStorage->Run(rContext);
  }
  
  uint32 GetArgCount() const
  {
    return mArgC;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
      return mpStorage->DumpArgs(rTypes);
  }

private:
  nuiRunable* mpStorage;
  uint32 mArgC;
};

template <class Class> Class* nuiConstructorStub0()
{
  return new Class;
}

template <class Class, class Param1>
Class* nuiConstructorStub1(Param1 param1)
{
  return new Class(param1);
}

template <class Class, class Param1, class Param2>
Class* nuiConstructorStub2(Param1 param1, Param2 param2)
{
  return new Class(param1, param2);
}

template <class Class, class Param1, class Param2, class Param3>
Class* nuiConstructorStub3(Param1 param1, Param2 param2, Param3 param3)
{
  return new Class(param1, param2, param3);
}

template <class Class, class Param1, class Param2, class Param3, class Param4>
Class* nuiConstructorStub4(Param1 param1, Param2 param2, Param3 param3, Param4 param4)
{
  return new Class(param1, param2, param3, param4);
}

template <class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
Class* nuiConstructorStub5(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5)
{
  return new Class(param1, param2, param3, param4, param5);
}

class nuiClass
{
public:
  nuiClass(const nglString& rName);
  virtual ~nuiClass();
  
  void AddParentClass(nuiClass* pParentClass);
  void AddMethod(const nglString& rMethodName, nuiFunction* pFunction);
  void AddConstructor(nuiFunction* pFunction);

  void GetMethods(const nglString& rName, std::vector<nuiFunction*>& rFunction) const;
  
  nuiClass* GetParentClass() const;
  const std::vector<nuiClass*>& GetParentClasses() const;
  const std::multimap<nglString, nuiFunction*>& GetMethods() const;
  const std::set<nuiFunction*>& GetConstructors() const;

  const nglString& GetName() const;

  void Dump(nglString& rString) const;
  
  virtual nuiVariant GetVariantFromVoidPtr(void* pPtr) const = 0;
  virtual nuiAttributeType GetClassType() const = 0;

  const char* GetCName() const;
protected:
  nglString mName;
  std::string mCName;
  std::vector<nuiClass*> mParentClasses;
  std::multimap<nglString, nuiFunction*> mMethods; 
  std::set<nuiFunction*> mConstructors;
};

template <typename Class>
class nuiClassImpl : public nuiClass
{
public:
  nuiClassImpl(const nglString& rName) : nuiClass(rName)
  {
  }
  
  virtual nuiVariant GetVariantFromVoidPtr(void* pPtr) const
  {
    return nuiVariant((Class*)pPtr);
  }
  
  virtual nuiAttributeType GetClassType() const
  {
    return nuiAttributeTypeTrait<Class*>::mTypeId;
  }
  
};

class nuiBindingManager
{
public:
  nuiBindingManager();
  virtual ~nuiBindingManager();

  nuiFunction* AddFunction(const nglString& rFunctionName, nuiFunction* pFunction);
  nuiClass* AddClass(nuiClass* pClass);

  nuiClass* GetClass(const nglString& rClassName) const;
  nuiClass* GetClass(nuiAttributeType type) const;
  
  static nuiBindingManager& GetManager()
  {
    return mManager;
  }

  void Dump(nglString& rString) const;

  typedef std::multimap<nglString, nuiFunction*> FunctionMap;
  typedef std::map<nglString, nuiClass*> ClassMap;
  typedef std::map<nuiAttributeType, nuiClass*> TypeMap;
  
  const FunctionMap& GetFunctions() const;
  const ClassMap& GetClasses() const;
  const TypeMap& GetTypes() const;
protected:
  FunctionMap mFunctions;
  ClassMap mClasses;
  TypeMap mTypes;

  static nuiBindingManager mManager;
};

////////////// Helper Macros:

#define nuiBindClass(CLASSNAME) \
{ \
  nuiClass* pClass = new nuiClassImpl<CLASSNAME>(_T(#CLASSNAME)); \
  nuiBindingManager::GetManager().AddClass(pClass);

#define nuiEndClass }

#define nuiInheritFrom(PARENT_CLASS) pClass->AddParentClass(nuiBindingManager::GetManager().GetClass(_T(#PARENT_CLASS)));
#define nuiAddCtor(CTOR_DECL) pClass->AddConstructor(new nuiFunction(CTOR_DECL));
#define nuiAddMethod(METHOD_NAME, METHOD)  pClass->AddMethod(_T(#METHOD_NAME), new nuiFunction(&METHOD));
#define nuiAddFunction(FUNCTION_NAME, FUNCTION) nuiBindingManager::GetManager().AddFunction(_T(#FUNCTION_NAME), new nuiFunction(FUNCTION));


