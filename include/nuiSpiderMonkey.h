/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiBindingManager.h"
#include "nuiBindings.h"
#include "nuiScriptEngine.h"

typedef struct JSClass           JSClass;
typedef struct JSExtendedClass   JSExtendedClass;
typedef struct JSConstDoubleSpec JSConstDoubleSpec;
typedef struct JSContext         JSContext;
typedef struct JSErrorReport     JSErrorReport;
typedef struct JSFunction        JSFunction;
typedef struct JSFunctionSpec    JSFunctionSpec;
typedef struct JSTracer          JSTracer;
typedef struct JSIdArray         JSIdArray;
typedef struct JSPropertyDescriptor JSPropertyDescriptor;
typedef struct JSPropertySpec    JSPropertySpec;
typedef struct JSObject          JSObject;
typedef struct JSObjectMap       JSObjectMap;
typedef struct JSObjectOps       JSObjectOps;
typedef struct JSRuntime         JSRuntime;
typedef struct JSScript          JSScript;
typedef struct JSStackFrame      JSStackFrame;
typedef struct JSString          JSString;
typedef struct JSXDRState        JSXDRState;
typedef struct JSExceptionState  JSExceptionState;
typedef struct JSLocaleCallbacks JSLocaleCallbacks;
typedef struct JSSecurityCallbacks JSSecurityCallbacks;
typedef struct JSONParser        JSONParser;

typedef intptr_t jsval;
typedef int JSBool;
typedef uint uintN;


class nuiSpiderMonkey : public nuiScriptEngine
{
public:
  nuiSpiderMonkey(uint32 MaxBytes = 8L * 1024L * 1024L);
  virtual ~nuiSpiderMonkey();

  void SetGlobal(const nglString& rName, const nuiVariant& rVariant);
  nuiVariant GetGlobal(const nglString& rName) const;
  nuiVariant ExecuteExpression(const nglString& rExpression);
  bool CompileString(const nglString& rSourceName, const nglString& rProgram);
  
protected:

  uint32 mMaxBytes;
  bool Init();
  
  static JSRuntime *mRunTime;
  JSContext *mContext;
  JSObject  *mGlobal;

  std::map<JSFunction*, nuiFunction*> mFunctions;
  std::map<nuiAttributeType, JSClass*> mClassAttribs;
  std::map<JSClass*, nuiClass*> mNUIClasses;
  std::map<nuiClass*, JSClass*> mJSClasses;
  std::map<nuiClass*, JSObject*> mJSClassObjects;
  
  int32 mRunTimeRef;

  nuiVariant GetVariantObjectFromJS(JSObject* pJSObject) const;
  JSObject* GetJSObjectFromVariant(const nuiVariant& rObject) const;
  void GetJSValFromVariant(jsval* val, const nuiVariant& var) const;
  void GetVariantFromJSVal(nuiVariant& var, jsval val) const;
  JSBool ConstructJSClass(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
  JSObject* DefineJSClass(nuiClass* pClass);
  
  friend void smReportError(JSContext* cx, const char *message, JSErrorReport *report);
  void ReportError(const char *message, JSErrorReport *report);

  friend JSBool nuiGenericJSFunction(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
  friend JSBool nuiGenericJSMethod(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
  JSBool GenericJSFunction(JSObject *obj, uintN argc, jsval *argv, jsval *rval, bool method);
  
  friend void nuiFinalizeJSObject(JSContext *cx, JSObject *pJSObject);
  void FinalizeJSObject(JSObject *pJSObject);

  friend JSBool nuiPropertyAdd(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
  JSBool PropertyAdd(JSObject *obj, jsval id, jsval *vp);
  friend JSBool nuiPropertyDel(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
  JSBool PropertyDel(JSObject *obj, jsval id, jsval *vp);
  friend JSBool nuiPropertyGet(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
  JSBool PropertyGet(JSObject *obj, jsval id, jsval *vp);
  friend JSBool nuiPropertySet(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
  JSBool PropertySet(JSObject *obj, jsval id, jsval *vp);
  friend JSBool nuiConstructJSClass(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

  struct EventLink
  {
  public:
    nuiEventSource* mpSource;
    nglString mName;
    nuiWidget* mpWidget;
    jsval mJSVal;
    JSObject* mpObject;
  };

  std::multimap<std::pair<nuiWidget*, nglString>, EventLink*> mEvents; // Event name to event link
  void OnEvent(const nuiEvent& rEvent);

  void Connect(JSObject* pObject, jsval obj, nuiWidget* pWidget, const nglString& rEventName);
  void Disconnect(nuiWidget* pWidget, const nglString& rEventName);
  
  nuiEventSink<nuiSpiderMonkey> mEventSink;
};


