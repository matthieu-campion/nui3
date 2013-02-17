/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#ifdef WIN32
#define PROTYPES_H
#include "jstypes.h"
typedef double float64;
typedef JSWord jsword;
typedef jsword jsval;
typedef jsword jsid;
#endif
#include "jsapi.h"


JSRuntime* nuiSpiderMonkey::mRunTime = NULL;

/* A wrapper for the NGL_OUT() macro
 This example shows how to report errors. */
JSBool js_NGL_LOUT(JSContext *mContext, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  const char *cmd;
  
  if (!JS_ConvertArguments(mContext, argc, argv, "s", &cmd))
    return JS_FALSE;
  
  nglString s(cmd);
  NGL_OUT(_T("%s\n"), s.GetChars());
  
  *rval = JSVAL_VOID;  /* return undefined */
  return JS_TRUE;
}


nuiSpiderMonkey::nuiSpiderMonkey(uint32 MaxBytes)
: nuiScriptEngine(_T("spidermonkey"), _T("javascript")), mMaxBytes(MaxBytes), mContext(NULL), mGlobal(NULL), mEventSink(this)
{
  if (!mRunTime)
  {
    mRunTime = JS_NewRuntime(mMaxBytes);
    mRunTimeRef++;
  }
  
  Init();
}

nuiSpiderMonkey::~nuiSpiderMonkey()
{
  JS_DestroyContext(mContext);
  JS_DestroyRuntime(mRunTime);
  JS_ShutDown();
}

nuiVariant nuiSpiderMonkey::ExecuteExpression(const nglString& rExpression)
{
  std::string program(rExpression.GetStdString());
  jsval rval;
  JSBool res = JS_EvaluateScript(mContext, mGlobal, program.c_str(), program.size(), "<inline>", 0, &rval);
  nuiVariant var;
  GetVariantFromJSVal(var, rval);
  return var;
}

bool nuiSpiderMonkey::CompileString(const nglString& rSourceName, const nglString& rProgram)
{
  std::string program(rProgram.GetStdString());
  std::string filename(rSourceName.GetStdString());
  jsval rval;
  JSBool res = JS_EvaluateScript(mContext, mGlobal, program.c_str(), program.size(), filename.c_str(), filename.size(), &rval);
  nuiVariant var;
  GetVariantFromJSVal(var, rval);
  return var;
}


void nuiSpiderMonkey::SetGlobal(const nglString& rName, const nuiVariant& rVariant)
{
  jsval v;
  GetJSValFromVariant(&v, rVariant);
  std::string name(rName.GetStdString());
  JSBool res = JS_DefineProperty(mContext, mGlobal, name.c_str(), v, NULL, NULL, JSPROP_ENUMERATE);
}

nuiVariant nuiSpiderMonkey::GetGlobal(const nglString& rName) const
{
  jsval v = 0;
  std::string name(rName.GetStdString());
  JSBool res = JS_LookupProperty(mContext, mGlobal, name.c_str(), &v);
  nuiVariant var;
  if (res)
    GetVariantFromJSVal(var, v);
  return var;
}


/* The class of the global object. */
static JSClass global_class =
{
  "nui_global", JSCLASS_GLOBAL_FLAGS,
  JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
  JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
  JSCLASS_NO_OPTIONAL_MEMBERS
};

#define REDIRECT nuiSpiderMonkey* pSM = (nuiSpiderMonkey*)JS_GetContextPrivate(cx); NGL_ASSERT(pSM);

/* The error reporter callback. */
void smReportError(JSContext* cx, const char *message, JSErrorReport *report)
{
  REDIRECT;
  pSM->ReportError(message, report);
}

void nuiSpiderMonkey::ReportError(const char *message, JSErrorReport *report)
{
  NGL_LOG(_T("spidermonkey"), NGL_LOG_ERROR, _T("%s:%u:%s\n"), report->filename ? report->filename : "<no filename>", (unsigned int)report->lineno, message);
}

nuiVariant nuiSpiderMonkey::GetVariantObjectFromJS(JSObject* pJSObject) const
{
  JSClass* pJSClass = JS_GET_CLASS(mContext, pJSObject);
  
  if (!pJSClass)
    return nuiVariant();
  
  nglString name(pJSClass->name);
  nuiClass* pClass = nuiBindingManager::GetManager().GetClass(name);
  if (!pClass)
    return nuiVariant((void*)NULL);
  
  nuiVariant* pPrivate = (nuiVariant*)JS_GetInstancePrivate(mContext, pJSObject, pJSClass, NULL);
  return *pPrivate;
}

JSObject* nuiSpiderMonkey::GetJSObjectFromVariant(const nuiVariant& rObject) const
{
  if (!rObject.IsPointer())
    return NULL;
  
  std::map<nuiAttributeType, JSClass*>::const_iterator it = mClassAttribs.find(rObject.GetType());
  if (it == mClassAttribs.end())
    return NULL;
  JSClass* pJSClass = it->second;
  JSObject* pJSProto = NULL;
  JSObject* pJSParent = NULL;
  JSObject* pJSObject = JS_NewObject(mContext, pJSClass, pJSProto, pJSParent);
  
  nuiVariant* pVariant = new nuiVariant(rObject);
  JS_SetPrivate(mContext, pJSObject, pVariant);
  
  return pJSObject;
}

void nuiSpiderMonkey::GetJSValFromVariant(jsval* val, const nuiVariant& var) const
{
  nuiAttributeType t = var.GetType();
  if (t == nuiAttributeTypeTrait<uint8>::mTypeId || t == nuiAttributeTypeTrait<uint16>::mTypeId || t == nuiAttributeTypeTrait<uint32>::mTypeId || t == nuiAttributeTypeTrait<uint64>::mTypeId ||
      t == nuiAttributeTypeTrait<int8>::mTypeId  || t == nuiAttributeTypeTrait<int16>::mTypeId  || t == nuiAttributeTypeTrait<int32>::mTypeId  || t == nuiAttributeTypeTrait<int64>::mTypeId  ||
      t == nuiAttributeTypeTrait<float>::mTypeId || t == nuiAttributeTypeTrait<double>::mTypeId)
  {
    // Number
    JS_NewNumberValue(mContext, var, val);
  }
  else if (t == nuiAttributeTypeTrait<nglString>::mTypeId)
  {
    nglString str = var;
    std::string s(str.GetStdString()); 
    *val = STRING_TO_JSVAL(JS_NewStringCopyN(mContext, s.c_str(), s.size()));
  }
  else if (t == nuiAttributeTypeTrait<void>::mTypeId) // Void value
  {
    *val = JSVAL_VOID;  /* return undefined */
  }
  else
  {
    JSObject* obj = GetJSObjectFromVariant(var);
    *val = OBJECT_TO_JSVAL(obj);
  }
  
  //#TODO We need to add support for other simple types
}

void nuiSpiderMonkey::GetVariantFromJSVal(nuiVariant& var, jsval val) const
{
  JSType type = JS_TypeOfValue(mContext, val);
  switch (type)
  {
      
    case JSTYPE_VOID:
      {
        var = nuiVariant();
      }
      break;
      
    case JSTYPE_OBJECT:
      {
        // Find the nui equivalent of the object
        JSObject* o = NULL;
        JS_ValueToObject(mContext, val, &o);
        var = GetVariantObjectFromJS(o);
      }
      break;
      
    case JSTYPE_STRING:
      {
        JSString* pStr = JS_ValueToString(mContext, val);
        nglString str(JS_GetStringBytes(pStr));
        var = str;
      }
      break;
      
    case JSTYPE_NUMBER:
      {
        jsdouble v;
        JS_ValueToNumber(mContext, val, &v);
        var = v;
      }
      break;
      
    case JSTYPE_BOOLEAN:
      {
        JSBool b = false;
        JS_ValueToBoolean(mContext, val, &b);
        var = b;
      }
      break;
      
    case JSTYPE_FUNCTION:
    case JSTYPE_NULL:
    case JSTYPE_XML:
    case JSTYPE_LIMIT:
      {
        NGL_LOG(_T("JavaScript"), NGL_LOG_ERROR, _T("Unable to convert a complex type to a string"));
        var = nuiVariant();
        return;
        
//        jsval v;
//        if (!JS_ConvertValue(mContext, v, JSTYPE_STRING, &val))
//        {
//          NGL_LOG(_T("JavaScript"), NGL_LOG_ERROR, _T("Unable to convert a complex type to a string"));
//          var = nuiVariant();
//          return;
//        }
//        
//        JSString* pStr = JS_ValueToString(mContext, v);
//        nglString str(JS_GetStringBytes(pStr));
//        var = str;
      }
      break;
  }  
}

JSBool nuiGenericJSFunction(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  REDIRECT;
  return pSM->GenericJSFunction(obj, argc, argv, rval, false);
}

JSBool nuiGenericJSMethod(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  REDIRECT;
  return pSM->GenericJSFunction(obj, argc, argv, rval, true);
}

JSBool nuiSpiderMonkey::GenericJSFunction(JSObject *obj, uintN argc, jsval *argv, jsval *rval, bool method)
{
  JSFunction* pJSFunc = JS_ValueToFunction(mContext, JS_ARGV_CALLEE(argv));
  if (!pJSFunc)
    return JS_FALSE;
  
  std::map<JSFunction*, nuiFunction*>::const_iterator it = mFunctions.find(pJSFunc);
  nuiFunction* pFunc = it->second;
  
  if (!pFunc)
    return JS_FALSE;
  
  nuiCallContext ctx;
  
  if (method) // Only add this if we are calling a method
    ctx.AddArgument(GetVariantObjectFromJS(obj));
  
  for (uint32 i = 0; i < argc; i++)
  {
    nuiVariant var;
    GetVariantFromJSVal(var, argv[i]);
    ctx.AddArgument(var);
  }
  
  // Call the method or function:
  pFunc->Run(ctx);
  
  const nuiVariant& r(ctx.GetResult());
  GetJSValFromVariant(rval, r);
  return JS_TRUE;
}

void nuiFinalizeJSObject(JSContext *cx, JSObject *pJSObject)
{
  REDIRECT;
  pSM->FinalizeJSObject(pJSObject);
}

void nuiSpiderMonkey::FinalizeJSObject(JSObject *pJSObject)
{
  JSClass* pJSClass = JS_GET_CLASS(mContext, pJSObject);
  
  NGL_ASSERT(pJSClass);
  
  nuiVariant* pPrivate = (nuiVariant*)JS_GetInstancePrivate(mContext, pJSObject, pJSClass, NULL);
  delete pPrivate;
}

JSBool nuiPropertyAdd(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  REDIRECT;
  return pSM->PropertyAdd(obj, id, vp);
}

JSBool nuiSpiderMonkey::PropertyAdd(JSObject *obj, jsval id, jsval *vp)
{
  if (JSVAL_IS_NUMBER(id))
    return JS_FALSE;
  
  JSString* pStr = JS_ValueToString(mContext, id);
  nglString str(JS_GetStringBytes(pStr));
  
  NGL_OUT(_T("Add Property %s\n"), str.GetChars());
  JSType type = JS_TypeOfValue(mContext, *vp);
  
  if (JSVAL_IS_VOID(*vp))
    NGL_OUT(_T("\tis void\n"));
  
  return JS_TRUE;
}

JSBool nuiPropertyDel(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  REDIRECT;
  return pSM->PropertyDel(obj, id, vp);
}

JSBool nuiSpiderMonkey::PropertyDel(JSObject *obj, jsval id, jsval *vp)
{
  if (JSVAL_IS_NUMBER(id))
    return JS_FALSE;
  
  JSString* pStr = JS_ValueToString(mContext, id);
  nglString str(JS_GetStringBytes(pStr));
  
  NGL_OUT(_T("Del Property %s\n"), str.GetChars());
  JSType type = JS_TypeOfValue(mContext, *vp);
  
  if (JSVAL_IS_VOID(*vp))
    NGL_OUT(_T("\tis void\n"));
  
  return JS_TRUE;
}

JSBool nuiPropertyGet(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  REDIRECT;
  return pSM->PropertyGet(obj, id, vp);
}

JSBool nuiSpiderMonkey::PropertyGet(JSObject *obj, jsval id, jsval *vp)
{
  if (JSVAL_IS_NUMBER(id))
    return JS_FALSE;
  
  nuiVariant variant = GetVariantObjectFromJS(obj);
  if (!variant.IsObject())
    return JS_TRUE;
  
  // We are trying to access an nuiObject attribute
  JSString* pStr = JS_ValueToString(mContext, id);
  nglString str(JS_GetStringBytes(pStr));
  
  nuiObject* pObj = variant;
  NGL_ASSERT(pObj);
  nuiAttribBase attrib(pObj->GetAttribute(str));
  if (!attrib.IsValid())
    return JS_TRUE; // Return the default value
  
  // Return the actual value
  nuiVariant v;
  attrib.ToVariant(v);
  GetJSValFromVariant(vp, v);
  //NGL_OUT(_T("Get Property %s\n"), str.GetChars());
  
  return JS_TRUE;
}

JSBool nuiPropertySet(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  REDIRECT;
  return pSM->PropertySet(obj, id, vp);
}

JSBool nuiSpiderMonkey::PropertySet(JSObject *obj, jsval id, jsval *vp)
{
  if (JSVAL_IS_NUMBER(id))
    return JS_FALSE;
  
  nuiVariant variant = GetVariantObjectFromJS(obj);
  if (!variant.IsObject())
    return JS_TRUE;
  
  // We are trying to access an nuiObject attribute
  JSString* pStr = JS_ValueToString(mContext, id);
  nglString str(JS_GetStringBytes(pStr));
  
  nuiObject* pObj = variant;
  NGL_ASSERT(pObj);
  nuiAttribBase attrib(pObj->GetAttribute(str));
  if (attrib.IsValid())
  {
    // Set the actual value
    nuiVariant v;
    GetVariantFromJSVal(v, *vp);
    attrib.FromVariant(v);
  }
  else if (JS_ValueToFunction(mContext, *vp))
  {
    nuiWidget* pWidget = variant;
    if (pWidget)
    {
      nuiEventSource* pEventSource = pWidget->GetEvent(str);
      if (pEventSource)
      {
        Connect(obj, *vp, pWidget, str);
      }
    }
  }
  //NGL_OUT(_T("Set Property %s\n"), str.GetChars());
  
  return JS_TRUE;
}


JSBool nuiConstructJSClass(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  REDIRECT;
  return pSM->ConstructJSClass(obj, argc, argv, rval);
}

JSBool nuiSpiderMonkey::ConstructJSClass(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  JSFunction* pJSFunc = JS_ValueToFunction(mContext, JS_ARGV_CALLEE(argv));
  if (!pJSFunc)
    return JS_FALSE;
  
  std::map<JSFunction*, nuiFunction*>::const_iterator it = mFunctions.find(pJSFunc);
  nuiFunction* pFunc = NULL;
  if (it != mFunctions.end())
    pFunc = it->second;
  
  if (!pFunc)
  {
    // In this case we need to find the class object
    JSClass* pJSClass = JS_GetClass(obj);
    if (!pJSClass)
      return JS_FALSE;
    
    std::map<JSClass*, nuiClass*>::const_iterator it = mNUIClasses.find(pJSClass);
    if (it == mNUIClasses.end())
      return JS_FALSE;
    
    nuiClass* pClass = it->second;
    if (pClass->GetConstructors().empty())
      return JS_FALSE;
    pFunc = *(pClass->GetConstructors().begin());
  }
  
  nuiCallContext ctx;
  
  for (uint32 i = 0; i < argc; i++)
  {
    nuiVariant var;
    GetVariantFromJSVal(var, argv[i]);
    ctx.AddArgument(var);
  }
  
  // Call the method or function:
  pFunc->Run(ctx);
  
  nuiVariant* pVariant = new nuiVariant(ctx.GetResult());
  JS_SetPrivate(mContext, obj, pVariant);
  return JS_TRUE;
}

JSObject* nuiSpiderMonkey::DefineJSClass(nuiClass* pClass)
{
  {
    // Check if this class has already been recursively defined because of inheritance
    std::map<nuiClass*, JSObject*>::const_iterator it = mJSClassObjects.find(pClass);
    if (it != mJSClassObjects.end())
    {
      NGL_OUT(_T("Skipping class '%s' (already defined)\n\n"), pClass->GetName().GetChars());
      return it->second;
    }
  }
  
  NGL_OUT(_T("\nDefining class '%s (%d)'\n"), pClass->GetName().GetChars(), pClass->GetClassType());
  
  JSClass cls =
  {
    pClass->GetCName(),
    JSCLASS_HAS_PRIVATE, // flags
    
    /* Mandatory non-null function pointer members. */
    nuiPropertyAdd, //JSPropertyOp        addProperty;
    nuiPropertyDel, //JSPropertyOp        delProperty;
    nuiPropertyGet, //JSPropertyOp        getProperty;
    nuiPropertySet, //JSPropertyOp        setProperty;
    JS_EnumerateStub, //JSEnumerateOp       enumerate;
    JS_ResolveStub, //JSResolveOp         resolve;
    JS_ConvertStub, //JSConvertOp         convert;
    nuiFinalizeJSObject, //JSFinalizeOp        finalize;
    
    /* Optionally non-null members start here. */
    NULL, //JSGetObjectOps      getObjectOps;
    NULL, //JSCheckAccessOp     checkAccess;
    NULL, // JSNative            call;
    NULL, //JSNative            construct;
    NULL, //JSXDRObjectOp       xdrObject;
    NULL, //JSHasInstanceOp     hasInstance;
    NULL, //JSMarkOp            mark;
    NULL, //JSReserveSlotsOp    reserveSlots;
  };
  
  JSClass* pJSClass = new JSClass(cls);
  
  JSObject* pJSProto = NULL;
  nuiClass* pParent = pClass->GetParentClass();
  if (pParent)
  {
    NGL_OUT(_T("\t inherits from '%s'\n"), pParent->GetName().GetChars());
    std::map<nuiClass*, JSObject*>::const_iterator it = mJSClassObjects.find(pParent);
    if (it != mJSClassObjects.end())
    {
      pJSProto = it->second;
    }
    else
    {
      NGL_OUT(_T("\t (recursive definition) of '%s'\n"), pParent->GetName().GetChars());
      pJSProto = DefineJSClass(pParent);
    }
  }
  
  JSObject* pJSObj = JS_InitClass(mContext, mGlobal, pJSProto, pJSClass, nuiConstructJSClass, 0, NULL, NULL, NULL, NULL);
  
  mClassAttribs[pClass->GetClassType()] = pJSClass;
  mJSClasses[pClass] = pJSClass;
  mNUIClasses[pJSClass] = pClass;
  mJSClassObjects[pClass] = pJSObj;
  
  // Add functions to the class prototype:
  const std::multimap<nglString, nuiFunction*>& rFuncs(pClass->GetMethods());
  std::multimap<nglString, nuiFunction*>::const_iterator it = rFuncs.begin();
  std::multimap<nglString, nuiFunction*>::const_iterator end = rFuncs.end();
  
  uint32 s = rFuncs.size();
  std::vector<JSFunctionSpec> funcs;
  funcs.reserve(s);
  
  nglString last;
  while (it != end)
  {
    if (last != it->first)
    {
      nuiFunction* pF = it->second;
      nglString name(it->first);
      
      JSFunction* pJSF = JS_DefineFunction(mContext, pJSObj, name.Export(), nuiGenericJSMethod, pF->GetArgCount(), 0);
      mFunctions[pJSF] = pF;
      printf("define function %s (0x%p -> 0x%p)", name.GetChars(), pJSF, pF);
    }
    
    last = it->first;
    ++it;
  }
  
  return pJSObj;
}

static JSFunctionSpec nui_js_global_functions[] =
{
  JS_FS("NGL_OUT", js_NGL_LOUT, 1, 0, 0),
  JS_FS_END
};

bool nuiSpiderMonkey::Init()
{
  // Create a JS runtime.
  if (mRunTime == NULL)
    return false;
  
  /* Create a context. */
  mContext = JS_NewContext(mRunTime, 8192);
  if (mContext == NULL)
    return false;

  JS_SetOptions(mContext, JSOPTION_VAROBJFIX | JSOPTION_JIT);
  JS_SetVersion(mContext, JSVERSION_LATEST);
  JS_SetContextPrivate(mContext, this);
  JS_SetErrorReporter(mContext, smReportError);
  
  /* Create the global object. */
  mGlobal = JS_NewObject(mContext, &global_class, NULL, NULL);
  if (mGlobal == NULL)
    return 1;
  
  /* Populate the global object with the standard globals,
   like Object and Array. */
  if (!JS_InitStandardClasses(mContext, mGlobal))
    return 1;
  
  
  if (!JS_DefineFunctions(mContext, mGlobal, nui_js_global_functions))
    return JS_FALSE;

  // Define classes:
  {
    const nuiBindingManager::ClassMap& rClasses(nuiBindingManager::GetManager().GetClasses());
    nuiBindingManager::ClassMap::const_iterator it = rClasses.begin();
    nuiBindingManager::ClassMap::const_iterator end = rClasses.end();
    while (it != end)
    {
      nuiClass* pClass = it->second;
      DefineJSClass(pClass);
      ++it;
    }
  }
  
  // Define global functions:
  {
    const nuiBindingManager::FunctionMap& rFunctions(nuiBindingManager::GetManager().GetFunctions());
    nuiBindingManager::FunctionMap::const_iterator it = rFunctions.begin();
    nuiBindingManager::FunctionMap::const_iterator end = rFunctions.end();
    while (it != end)
    {
      nuiFunction* pF = it->second;
      nglString name(it->first);
      
      JSFunction* pJSF = JS_DefineFunction(mContext, mGlobal, name.Export(), nuiGenericJSFunction, pF->GetArgCount(), 0);
      mFunctions[pJSF] = pF;
      printf("define function %s (0x%p -> 0x%p)", name.GetChars(), pJSF, pF);
      ++it;
    }
  }
  
  // Define global enums:
  {
    const nuiBindingManager::EnumMap& rEnums(nuiBindingManager::GetManager().GetEnums());
    nuiBindingManager::EnumMap::const_iterator it = rEnums.begin();
    nuiBindingManager::EnumMap::const_iterator end = rEnums.end();

    while (it != end)
    {
      uint32 val = it->second;
      jsval v;
      GetJSValFromVariant(&v, nuiVariant(val));
      std::string name(it->first.GetStdString());
      JSBool res = JS_DefineProperty(mContext, mGlobal, name.c_str(), v, NULL, NULL, JSPROP_ENUMERATE);
      ++it;
    }
  }
  
    
  
  return true;
}

void nuiSpiderMonkey::OnEvent(const nuiEvent& rEvent)
{
  EventLink* pLink = (EventLink*)rEvent.mpUser;
  // Call javascript handler
  jsval resval = 0;
  std::vector<jsval> args;
  args.reserve(rEvent.GetArgCount());
  for (uint32 i = 0; i < rEvent.GetArgCount(); i++)
  {
    jsval v;
    GetJSValFromVariant(&v, rEvent[i]);
    args.push_back(v);
  }
  JSBool res = JS_CallFunctionValue(mContext, pLink->mpObject, pLink->mJSVal, args.size(), &args[0], &resval);
}

void nuiSpiderMonkey::Connect(JSObject* pObject, jsval obj, nuiWidget* pWidget, const nglString& rEventName)
{
  nuiEventSource* pSource = pWidget->GetEvent(rEventName);
  if (!pSource)
    return;

  EventLink* pLink = new EventLink();
  pLink->mpWidget = pWidget;
  pLink->mName = rEventName;
  pLink->mpSource = pSource;
  pLink->mJSVal = obj;
  pLink->mpObject = pObject;
  
  mEventSink.Connect(*pSource, &nuiSpiderMonkey::OnEvent, pLink);
  mEvents.insert(std::pair<std::pair<nuiWidget*, nglString>, EventLink*>(std::pair<nuiWidget*, nglString>(pWidget, rEventName), pLink));
  
}

void nuiSpiderMonkey::Disconnect(nuiWidget* pWidget, const nglString& rEventName)
{
  std::multimap<std::pair<nuiWidget*, nglString>, EventLink*>::iterator it = mEvents.find(std::pair<nuiWidget*, nglString>(pWidget, rEventName));
  if (it != mEvents.end())
  {
    delete it->second;
    mEvents.erase(it);
  }
}

