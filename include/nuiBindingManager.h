/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once
#include "nui.h"

class nuiArgument
{
public:
  typedef uint32 Flags;
  static const Flags none = 0;
  static const Flags Void = 1;
  
  template<class Class> nuiArgument(const Class& value)
  {
  }

  ~nuiArgument()
  {

  }
  
  void SetVoid(bool set)
  {
    mFlags |= Void;
  };
  bool IsVoid()
  {
    return mFlags & Void;
  };
  
private:
  Flags mFlags;
};

template<class T, class U>
class is_same_type
{
public:
  enum
  {
    value = false
  };
};

template<class T>
class is_same_type<T, T>
{
public:
  enum
  {
    value = true
  };
};

template <class Type> class nuiArgumentTrait
{
public:
  static const nglChar* GetName()
  {
    return NULL;
  }
  
  static Type Convert(const nuiArgument& rArg)
  {
    //return rArg;
    return (Type)0;
  }
};


///////////////////////////////////// Traits implems:


template <class Class> class nuiArgumentTrait<Class&>
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

  static Class& Convert(const nuiArgument& rArg)
  {
    return *(Class*)NULL;
  }
};

template <class Class> class nuiArgumentTrait<const Class&>
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

  static const Class& Convert(const nuiArgument& rArg)
  {
    return *(const Class*)NULL;
  }
};

template <class Class> class nuiArgumentTrait<Class*>
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

  static Class* Convert(const nuiArgument& rArg)
  {
    return NULL;
  }
};

template <class Class> nglString nuiArgumentTrait<Class&>::mName;
template <class Class> nglString nuiArgumentTrait<const Class&>::mName;

template <class Class> nglString nuiArgumentTrait<Class*>::mName;



template <> class nuiArgumentTrait<int>
{
public:  
  static const nglChar* GetName()
  {
    return _T("int");
  }
  
  static int Convert(const nuiArgument& rArg)
  {
    return 0;
  }
};

template <> class nuiArgumentTrait<void>
{
public:  
  static const nglChar* GetName()
  {
    return _T("void");
  }
  
  static void Convert(const nuiArgument& rArg)
  {
    return;
  }
};

template <> class nuiArgumentTrait<float>
{
public:  
  static const nglChar* GetName()
  {
    return _T("float");
  }
  
  static float Convert(const nuiArgument& rArg)
  {
    return 0.0f;
  }
};


template <> class nuiArgumentTrait<nglString>
{
public:  
  static const nglChar* GetName()
  {
    return _T("nglString");
  }
  
  static nglString Convert(const nuiArgument& rArg)
  {
    return nglString::Null;
  }
};

class nuiCallContext
{
public:
  nuiCallContext();
  ~nuiCallContext();

  
  
  nuiArgument& operator[](uint32 index) const
  {
    return *mArgs[index];
  }
  
  nuiArgument& GetResult() const
  {
    return *mpResult;
  }
  
private:
  std::vector<nuiArgument*> mArgs;
  nuiArgument* mpResult;
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
template <class RetType> class nuiFunctionStorage0 : public nuiRunable
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
      rContext.GetResult().SetVoid(true);
      (*mStorage)();
    }
    else
    {
      rContext.GetResult() = *mStorage();
    }
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (*mStorage)();
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
  }
};


template <class RetType, class Param1> class nuiFunctionStorage1 : public nuiRunable
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
    rContext.GetResult() = (*mStorage)(nuiArgumentTrait<Param1>::Convert(rContext[0]));
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (*mStorage)(nuiArgumentTrait<Param1>::Convert(rContext[0]));
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
  }
};

template <class RetType, class Param1, class Param2> class nuiFunctionStorage2 : public nuiRunable
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
    rContext.GetResult() = 
      (*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[0]),
      nuiArgumentTrait<Param2>::Convert(rContext[1])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (*mStorage)(
                nuiArgumentTrait<Param1>::Convert(rContext[0]),
                nuiArgumentTrait<Param2>::Convert(rContext[1])
                );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
  }
};


template <class RetType, class Param1, class Param2, class Param3> class nuiFunctionStorage3 : public nuiRunable
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
    rContext.GetResult() = 
      (*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[0]),
      nuiArgumentTrait<Param2>::Convert(rContext[1]),
      nuiArgumentTrait<Param3>::Convert(rContext[2])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (*mStorage)(
                nuiArgumentTrait<Param1>::Convert(rContext[0]),
                nuiArgumentTrait<Param2>::Convert(rContext[1]),
                nuiArgumentTrait<Param3>::Convert(rContext[2])
                );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
  }
};

template <class RetType, class Param1, class Param2, class Param3, class Param4> class nuiFunctionStorage4 : public nuiRunable
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
    rContext.GetResult() = 
      (*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[0]),
      nuiArgumentTrait<Param2>::Convert(rContext[1]),
      nuiArgumentTrait<Param3>::Convert(rContext[2]),
      nuiArgumentTrait<Param4>::Convert(rContext[3])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (*mStorage)(
                nuiArgumentTrait<Param1>::Convert(rContext[0]),
                nuiArgumentTrait<Param2>::Convert(rContext[1]),
                nuiArgumentTrait<Param3>::Convert(rContext[2]),
                nuiArgumentTrait<Param4>::Convert(rContext[3])
                );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
  }
};


template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5> class nuiFunctionStorage5 : public nuiRunable
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
    rContext.GetResult() = 
      (*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[0]),
      nuiArgumentTrait<Param2>::Convert(rContext[1]),
      nuiArgumentTrait<Param3>::Convert(rContext[2]),
      nuiArgumentTrait<Param4>::Convert(rContext[3]),
      nuiArgumentTrait<Param5>::Convert(rContext[4])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param5>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (*mStorage)(
                nuiArgumentTrait<Param1>::Convert(rContext[0]),
                nuiArgumentTrait<Param2>::Convert(rContext[1]),
                nuiArgumentTrait<Param3>::Convert(rContext[2]),
                nuiArgumentTrait<Param4>::Convert(rContext[3]),
                nuiArgumentTrait<Param4>::Convert(rContext[4])
                );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param5>::GetName());
  }
};


///! define all the nuiFunctionStorageXX class (one per number of arguments).
template <class Class, class RetType> class nuiMethodStorage0 : public nuiRunable
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
    rContext.GetResult() = (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)();
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)();
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
  }
};


template <class Class, class RetType, class Param1> class nuiMethodStorage1 : public nuiRunable
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
    rContext.GetResult() = (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)(nuiArgumentTrait<Param1>::Convert(rContext[1]));
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)(nuiArgumentTrait<Param1>::Convert(rContext[1]));
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
  }
};

template <class Class, class RetType, class Param1, class Param2>
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
    if (is_same_type<RetType, void>::value)
    {
      rContext.GetResult().SetVoid(true);
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
        (
          nuiArgumentTrait<Param1>::Convert(rContext[1]),
          nuiArgumentTrait<Param2>::Convert(rContext[2])
        );
    }
    else
    {
      rContext.GetResult() = 
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
        (
          nuiArgumentTrait<Param1>::Convert(rContext[1]),
          nuiArgumentTrait<Param2>::Convert(rContext[2])
        );
    }
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
       nuiArgumentTrait<Param1>::Convert(rContext[1]),
       nuiArgumentTrait<Param2>::Convert(rContext[2])
       );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
  }
};


template <class Class, class RetType, class Param1, class Param2, class Param3>
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
    rContext.GetResult() = 
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
    (
     nuiArgumentTrait<Param1>::Convert(rContext[1]),
     nuiArgumentTrait<Param2>::Convert(rContext[2]),
     nuiArgumentTrait<Param3>::Convert(rContext[3])
     );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
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
    rContext.GetResult() = 
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
    (
     nuiArgumentTrait<Param1>::Convert(rContext[1]),
     nuiArgumentTrait<Param2>::Convert(rContext[2]),
     nuiArgumentTrait<Param3>::Convert(rContext[3])
     );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
  }
};


template <class Class, class RetType, class Param1, class Param2, class Param3, class Param4>
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
    rContext.GetResult() = 
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
    (
     nuiArgumentTrait<Param1>::Convert(rContext[1]),
     nuiArgumentTrait<Param2>::Convert(rContext[2]),
     nuiArgumentTrait<Param3>::Convert(rContext[3]),
     nuiArgumentTrait<Param4>::Convert(rContext[4])
     );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
       nuiArgumentTrait<Param1>::Convert(rContext[1]),
       nuiArgumentTrait<Param2>::Convert(rContext[2]),
       nuiArgumentTrait<Param3>::Convert(rContext[3]),
       nuiArgumentTrait<Param4>::Convert(rContext[4])
       );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
  }
};


template <class Class, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
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
    rContext.GetResult() = 
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
    (
     nuiArgumentTrait<Param1>::Convert(rContext[1]),
     nuiArgumentTrait<Param2>::Convert(rContext[2]),
     nuiArgumentTrait<Param3>::Convert(rContext[3]),
     nuiArgumentTrait<Param4>::Convert(rContext[4]),
     nuiArgumentTrait<Param5>::Convert(rContext[5])
     );
    return true;
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param5>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
    (
     nuiArgumentTrait<Param1>::Convert(rContext[1]),
     nuiArgumentTrait<Param2>::Convert(rContext[2]),
     nuiArgumentTrait<Param3>::Convert(rContext[3]),
     nuiArgumentTrait<Param4>::Convert(rContext[4]),
     nuiArgumentTrait<Param5>::Convert(rContext[5])
     );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param5>::GetName());
  }
};

// Const versions of the method storages:
template <class Class, class RetType> class nuiConstMethodStorage0 : public nuiRunable
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
    rContext.GetResult() = (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)();
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)();
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
  }
};


template <class Class, class RetType, class Param1> class nuiConstMethodStorage1 : public nuiRunable
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
    rContext.GetResult() = (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)(nuiArgumentTrait<Param1>::Convert(rContext[1]));
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)(nuiArgumentTrait<Param1>::Convert(rContext[1]));
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
  }
};

template <class Class, class RetType, class Param1, class Param2>
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
    if (is_same_type<RetType, void>::value)
    {
      rContext.GetResult().SetVoid(true);
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
        (
        nuiArgumentTrait<Param1>::Convert(rContext[1]),
        nuiArgumentTrait<Param2>::Convert(rContext[2])
        );
    }
    else
    {
      rContext.GetResult() = 
        (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
        (
        nuiArgumentTrait<Param1>::Convert(rContext[1]),
        nuiArgumentTrait<Param2>::Convert(rContext[2])
        );
    }
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
  }
};


template <class Class, class RetType, class Param1, class Param2, class Param3>
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
    rContext.GetResult() = 
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2]),
      nuiArgumentTrait<Param3>::Convert(rContext[3])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
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
    rContext.GetResult() = 
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2]),
      nuiArgumentTrait<Param3>::Convert(rContext[3])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
  }
};


template <class Class, class RetType, class Param1, class Param2, class Param3, class Param4>
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
    rContext.GetResult() = 
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2]),
      nuiArgumentTrait<Param3>::Convert(rContext[3]),
      nuiArgumentTrait<Param4>::Convert(rContext[4])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2]),
      nuiArgumentTrait<Param3>::Convert(rContext[3]),
      nuiArgumentTrait<Param4>::Convert(rContext[4])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
  }
};


template <class Class, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
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
    rContext.GetResult() = 
      (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2]),
      nuiArgumentTrait<Param3>::Convert(rContext[3]),
      nuiArgumentTrait<Param4>::Convert(rContext[4]),
      nuiArgumentTrait<Param5>::Convert(rContext[5])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<RetType>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param5>::GetName());
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
    rContext.GetResult().SetVoid(true);
    (nuiArgumentTrait<Class&>::Convert(rContext[0]).*mStorage)
      (
      nuiArgumentTrait<Param1>::Convert(rContext[1]),
      nuiArgumentTrait<Param2>::Convert(rContext[2]),
      nuiArgumentTrait<Param3>::Convert(rContext[3]),
      nuiArgumentTrait<Param4>::Convert(rContext[4]),
      nuiArgumentTrait<Param5>::Convert(rContext[5])
      );
    return true;
  }

  void DumpArgs(std::vector<nglString>& rTypes) const
  {
    rTypes.clear();
    rTypes.push_back(nuiArgumentTrait<void>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param1>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param2>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param3>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param4>::GetName());
    rTypes.push_back(nuiArgumentTrait<Param5>::GetName());
  }
};




class nuiFunction : public nuiRunable
{
public:
  template <class RetType> nuiFunction(RetType (*pFunction)())
  {
    mpStorage = new nuiFunctionStorage0<RetType>(pFunction);
  }
  
  template <class RetType, class Param1> nuiFunction(RetType (*pFunction)(Param1))
  {
    mpStorage = new nuiFunctionStorage1<RetType, Param1>(pFunction);
  }
  
  template <class RetType, class Param1, class Param2> nuiFunction(RetType (*pFunction)(Param1, Param2))
  {
    mpStorage = new nuiFunctionStorage2<RetType, Param1, Param2>(pFunction);
  }
  
  template <class RetType, class Param1, class Param2, class Param3> nuiFunction(RetType (*pFunction)(Param1, Param2, Param3))
  {
    mpStorage = new nuiFunctionStorage3<RetType, Param1, Param2, Param3>(pFunction);
  }
  
  template <class RetType, class Param1, class Param2, class Param3, class Param4> nuiFunction(RetType (*pFunction)(Param1, Param2, Param3, Param4))
  {
    mpStorage = new nuiFunctionStorage4<RetType, Param1, Param2, Param3, Param4>(pFunction);
  }
  
  template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5> nuiFunction(RetType (*pFunction)(Param1, Param2, Param3, Param4, Param5))
  {
    mpStorage = new nuiFunctionStorage5<RetType, Param1, Param2, Param3, Param4, Param5>(pFunction);
  }
  
  /////////
  template <class RetType, class Class> nuiFunction(RetType (Class::*pFunction)())
  {
    mpStorage = new nuiMethodStorage0<Class, RetType>(pFunction);
  }
  
  template <class RetType, class Class, class Param1> nuiFunction(RetType (Class::*pFunction)(Param1))
  {
    mpStorage = new nuiMethodStorage1<Class, RetType, Param1>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2> nuiFunction(RetType (Class::*pFunction)(Param1, Param2))
  {
    mpStorage = new nuiMethodStorage2<Class, RetType, Param1, Param2>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2, class Param3> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3))
  {
    mpStorage = new nuiMethodStorage3<Class, RetType, Param1, Param2, Param3>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2, class Param3, class Param4> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4))
  {
    mpStorage = new nuiMethodStorage4<Class, RetType, Param1, Param2, Param3, Param4>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2, class Param3, class Param4, class Param5> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4, Param5))
  {
    mpStorage = new nuiMethodStorage5<Class, RetType, Param1, Param2, Param3, Param4, Param5>(pFunction);
  }
  
  /////////
  template <class RetType, class Class> nuiFunction(RetType (Class::*pFunction)() const)
  {
    mpStorage = new nuiConstMethodStorage0<const Class, RetType>(pFunction);
  }
  
  template <class RetType, class Class, class Param1> nuiFunction(RetType (Class::*pFunction)(Param1) const)
  {
    mpStorage = new nuiConstMethodStorage1<const Class, RetType, Param1>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2> nuiFunction(RetType (Class::*pFunction)(Param1, Param2) const)
  {
    mpStorage = new nuiConstMethodStorage2<const Class, RetType, Param1, Param2>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2, class Param3> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3) const)
  {
    mpStorage = new nuiConstMethodStorage3<const Class, RetType, Param1, Param2, Param3>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2, class Param3, class Param4> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4) const)
  {
    mpStorage = new nuiConstMethodStorage4<const Class, RetType, Param1, Param2, Param3, Param4>(pFunction);
  }
  
  template <class RetType, class Class, class Param1, class Param2, class Param3, class Param4, class Param5> nuiFunction(RetType (Class::*pFunction)(Param1, Param2, Param3, Param4, Param5) const)
  {
    mpStorage = new nuiConstMethodStorage5<const Class, RetType, Param1, Param2, Param3, Param4, Param5>(pFunction);
  }

  //////////////
  bool Run(nuiCallContext& rContext)
  {
    if (!mpStorage)
      return false;
    
    return mpStorage->Run(rContext);
  }
  
  void DumpArgs(std::vector<nglString>& rTypes) const
  {
      return mpStorage->DumpArgs(rTypes);
  }

private:
  nuiRunable* mpStorage;
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

  const std::vector<nuiClass*>& GetParentClasse() const;
  const std::multimap<nglString, nuiFunction*>& GetMethods() const;
  const std::set<nuiFunction*>& GetConstructors() const;

  const nglString& GetName() const;

  void Dump(nglString& rString) const;
protected:
  nglString mName;
  std::vector<nuiClass*> mParentClasses;
  std::multimap<nglString, nuiFunction*> mMethods; 
  std::set<nuiFunction*> mConstructors;
};

class nuiBindingManager
{
public:
  nuiBindingManager();
  virtual ~nuiBindingManager();

  void AddFunction(const nglString& rFunctionName, nuiFunction* pFunction);
  void AddClass(nuiClass* pClass);

  nuiClass* GetClass(const nglString& rClassName) const;
  
  static nuiBindingManager& GetManager()
  {
    return mManager;
  }

  void Dump(nglString& rString) const;
protected:
  typedef std::multimap<nglString, nuiFunction*> FunctionMap;
  typedef std::map<nglString, nuiClass*> ClassMap;
  FunctionMap mFunctions;
  ClassMap mClasses;

  static nuiBindingManager mManager;
};

////////////// Helper Macros:

#define nuiBindClass(CLASSNAME) \
{ \
  nuiClass* pClass = new nuiClass(_T(#CLASSNAME)); \
  nuiBindingManager::GetManager().AddClass(pClass);

#define nuiEndClass }

#define nuiInheritFrom(PARENT_CLASS) pClass->AddParentClass(nuiBindingManager::GetManager().GetClass(_T(#PARENT_CLASS)));
#define nuiAddCtor(CTOR_DECL) pClass->AddConstructor(new nuiFunction(CTOR_DECL));
#define nuiAddMethod(METHOD_NAME, METHOD)  pClass->AddMethod(_T(#METHOD_NAME), new nuiFunction(&METHOD));
#define nuiAddFunction(FUNCTION_NAME, FUNCTION) nuiBindingManager::GetManager().AddFunction(_T(#FUNCTION_NAME), new nuiFunction(FUNCTION));


