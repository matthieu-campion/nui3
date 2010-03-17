/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiBindingManager.h"
#include "nuiTypeTraits.h"

NUI_DECLARE_ATTRIBUTE_TYPE(void);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiWidget*);
NUI_DECLARE_ATTRIBUTE_TYPE(nuiObject*);

void TestVariant()
{
  bool b1 = is_base_of<nuiObject, nuiWidget>::value;
  bool b2 = is_base_of<nuiWidget, nuiObject>::value;
  bool b3 = is_base_of<nuiObject*, nuiWidget*>::value;
  bool b4 = is_base_of<nuiWidget*, nuiObject*>::value;
  bool b5 = is_base_of<nuiObject, nuiObject>::value;
  bool b6 = is_base_of<nuiWidget, nuiWidget>::value;

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

  NGL_OUT(_T("sizeof nuiVariant: %d\n"), sizeof(nuiVariant));
}

void TestBinding()
{
  nuiClass* pClass = nuiBindingManager::GetManager().GetClass(_T("nuiWidget"));

  if (!pClass)
    return;
  
  nuiVariant object;
  {
    const std::set<nuiFunction*>& rCtors(pClass->GetConstructors());
    nuiFunction* pCtor = *rCtors.begin();
    nuiCallContext context;
    pCtor->Run(context);
    object = (context.GetResult());
  }

  {
    std::vector<nuiFunction*> Methods;
    pClass->GetMethods(_T("GetObjectClass"), Methods);
    NGL_ASSERT(!Methods.empty());
    nuiFunction* pMethod = Methods[0];
    nuiCallContext context;
    context.AddArgument(object);
    pMethod->Run(context);
    nuiVariant result = context.GetResult();
    nglString res = result;
    NGL_OUT(_T("GetObjectClass result: %ls\n"), res.GetChars());
  }
  
  {
    std::vector<nuiFunction*> Methods;
    pClass->GetMethods(_T("SetObjectName"), Methods);
    NGL_ASSERT(!Methods.empty());
    nuiFunction* pMethod = Methods[0];
    nuiCallContext context;
    context.AddArgument(object);
    context.AddArgument(nuiRect(50, 100, 320, 240));
    pMethod->Run(context);  
  }
  
  {
    std::vector<nuiFunction*> Methods;
    pClass->GetMethods(_T("GetObjectName"), Methods);
    NGL_ASSERT(!Methods.empty());
    nuiFunction* pMethod = Methods[0];
    nuiCallContext context;
    context.AddArgument(object);
    pMethod->Run(context);
    nuiVariant result = context.GetResult();
    nglString res = result;
    NGL_OUT(_T("GetObjectName result: %ls\n"), res.GetChars());
  }
  
  {
    std::vector<nuiFunction*> Methods;
    pClass->GetMethods(_T("Trash"), Methods);
    NGL_ASSERT(!Methods.empty());
    nuiFunction* pMethod = Methods[0];
    nuiCallContext context;
    context.AddArgument(object);
    pMethod->Run(context);  
  }
  
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

void nuiClass::GetMethods(const nglString& rName, std::vector<nuiFunction*>& rFunctions) const
{
  std::pair<std::multimap<nglString, nuiFunction*>::const_iterator, std::multimap<nglString, nuiFunction*>::const_iterator> ret = mMethods.equal_range(rName);
  if (ret.first != mMethods.end())
  {
    std::multimap<nglString, nuiFunction*>::const_iterator it(ret.first);
    std::multimap<nglString, nuiFunction*>::const_iterator end(ret.second);
    
    while (it != end)
    {
      rFunctions.push_back(it->second);
      ++it;
    }
  }
  
  uint32 count = mParentClasses.size();
  for (uint32 i = 0; i < count; i++)
  {
    nuiClass* pParent = mParentClasses[count - 1 - i];
    NGL_ASSERT(pParent);
    pParent->GetMethods(rName, rFunctions);
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

