/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"
#include "nuiBindingManager.h"
#include "nuiBindings.h"


#define PROTYPES_H
#include "jstypes.h"
typedef double float64;
typedef JSWord jsword;
typedef jsword jsval;
typedef jsword jsid;
#include "jsapi.h"

/* The class of the global object. */
static JSClass global_class =
{
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

/* The error reporter callback. */
void reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u:%s\n",
            report->filename ? report->filename : "<no filename>",
            (unsigned int) report->lineno,
            message);
}

/* A simple JS wrapper for the rand() function, from the C standard library.
   This example shows how to return a number from a JSNative.
   This is nearly the simplest possible JSNative function. */
JSBool myjs_rand(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_NewNumberValue(cx, rand(), rval);
}

/* A wrapper for the srand() function, from the C standard library.
   This example shows how to handle optional arguments. */
JSBool myjs_srand(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    uint32 seed;

    if (!JS_ConvertArguments(cx, argc, argv, "/u", &seed))
        return JS_FALSE;

    /* If called with no arguments, use the current time as the seed. */
    if (argc == 0)
        seed = time(NULL);

    srand(seed);

    *rval = JSVAL_VOID;  /* return undefined */
    return JS_TRUE;
}

/* A wrapper for the printf() function, from the C standard library.
   This example shows how to report errors. */
JSBool myjs_out(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    const char *cmd;

    if (!JS_ConvertArguments(cx, argc, argv, "s", &cmd))
        return JS_FALSE;

    nglString s(cmd);
    NGL_OUT(_T("%ls"), s.GetChars());

    *rval = JSVAL_VOID;  /* return undefined */
    return JS_TRUE;
}

static void* GetVariantObjectFromJS(JSContext* cx, JSObject* pJSObject)
{
  JSClass* pJSClass = JS_GET_CLASS(cx, pJSObject);
  
  if (!pJSClass)
    return nuiVariant();
  
  nglString name(pJSClass->name);
  nuiClass* pClass = nuiBindingManager::GetManager().GetClass(name);
  if (!pClass)
    return nuiVariant((void*)NULL);
  
  void* pPrivate = JS_GetInstancePrivate(cx, pJSObject, pJSClass, NULL);
  return pClass->GetVariantFromVoidPtr(pPrivate);
}

std::map<JSFunction*, nuiFunction*> gFunctions;

template <bool method>
JSBool nuiGenericJSFunction(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  JSFunction* pJSFunc = JS_ValueToFunction(cx, JS_ARGV_CALLEE(argv));
  if (!pJSFunc)
    return JS_FALSE;

  std::map<JSFunction*, nuiFunction*>::const_iterator it = gFunctions.find(pJSFunc);
  nuiFunction* pFunc = it->second;

  if (!pFunc)
    return JS_FALSE;

  nuiCallContext ctx;

  if (method) // Only add this if we are calling a method
    ctx.AddArgument(GetVariantObjectFromJS(cx, JS_THIS_OBJECT(cx, argv)));
  
  for (uint32 i = 0; i < argc; i++)
  {
    JSType type = JS_TypeOfValue(cx, argv[i]);
    switch (type)
    {
      
      case JSTYPE_VOID:
        {
          return JS_FALSE;
        }
        break;
      
      case JSTYPE_OBJECT:
        {
          // Find the nui equivalent of the object
          ctx.AddArgument(GetVariantObjectFromJS(cx, JS_THIS_OBJECT(cx, argv)));
        }
        break;
      
      case JSTYPE_STRING:
        {
          JSString* pStr = JS_ValueToString(cx, argv[i]);
          nglString str(JS_GetStringBytes(pStr));
          ctx.AddArgument(str);
        }
        break;
      
      case JSTYPE_NUMBER:
        {
          jsdouble val;
          JS_ValueToNumber(cx, argv[i], &val);
          ctx.AddArgument(val);
        }
        break;
      
      case JSTYPE_BOOLEAN:
        {
          JSBool b = false;
          JS_ValueToBoolean(cx, argv[i], &b);
          ctx.AddArgument(b);
        }
        break;

      case JSTYPE_FUNCTION:
      case JSTYPE_NULL:
      case JSTYPE_XML:
      case JSTYPE_LIMIT:
        {
          jsval val;
          if (!JS_ConvertValue(cx, argv[i], JSTYPE_STRING, &val))
          {
            NGL_LOG(_T("JavaScript"), NGL_LOG_ERROR, _T("Unable to convert a complex type to a string"));
            return JS_FALSE;
          }
          
          JSString* pStr = JS_ValueToString(cx, val);
          nglString str(JS_GetStringBytes(pStr));
          ctx.AddArgument(str);
        }
        break;
    }

  }

  // Call the method or function:
  pFunc->Run(ctx);
  
  const nuiVariant& r(ctx.GetResult());
  nuiAttributeType t = r.GetType();
  if (t == nuiAttributeTypeTrait<uint8>::mTypeId || t == nuiAttributeTypeTrait<uint16>::mTypeId || t == nuiAttributeTypeTrait<uint32>::mTypeId || t == nuiAttributeTypeTrait<uint64>::mTypeId ||
      t == nuiAttributeTypeTrait<int8>::mTypeId  || t == nuiAttributeTypeTrait<int16>::mTypeId  || t == nuiAttributeTypeTrait<int32>::mTypeId  || t == nuiAttributeTypeTrait<int64>::mTypeId  ||
      t == nuiAttributeTypeTrait<float>::mTypeId || t == nuiAttributeTypeTrait<double>::mTypeId)
  {
    // Number
    JS_NewNumberValue(cx, r, rval);
  }
  else if (t == nuiAttributeTypeTrait<nglString>::mTypeId)
  {
    nglString str = r;
    std::string s(str.GetStdString()); 
    *rval = STRING_TO_JSVAL(JS_NewStringCopyN(cx, s.c_str(), s.size()));
  }
  else if (r.IsPointer())
  {
    
  }
  else //if (t == nuiAttributeTypeTrait<void>::mTypeId) or unknown type
  {
    *rval = JSVAL_VOID;  /* return undefined */
  }
  return JS_TRUE;
}

nglString TestFunction(const nglString& rStr, uint32 i, double d)
{
  NGL_OUT(_T("rStr: '%ls'\ni: %d\nd: %f\n\n"), rStr.GetChars(), i, d);
  
  return _T("MOOOOOOOOOOOORTEL!\n");
}

int JSTest()
{
  /* JS variables. */
  JSRuntime *rt;
  JSContext *cx;
  JSObject  *global;

  /* Create a JS runtime. */
  rt = JS_NewRuntime(8L * 1024L * 1024L);
  if (rt == NULL)
      return 1;

  /* Create a context. */
  cx = JS_NewContext(rt, 8192);
  if (cx == NULL)
      return 1;
  JS_SetOptions(cx, JSOPTION_VAROBJFIX);
  JS_SetVersion(cx, JSVERSION_LATEST);
  JS_SetErrorReporter(cx, reportError);

  /* Create the global object. */
  global = JS_NewObject(cx, &global_class, NULL, NULL);
  if (global == NULL)
      return 1;

  /* Populate the global object with the standard globals,
     like Object and Array. */
  if (!JS_InitStandardClasses(cx, global))
      return 1;


  static JSFunctionSpec myjs_global_functions[] = {
    JS_FS("rand",   myjs_rand,   0, 0, 0),
    JS_FS("srand",  myjs_srand,  0, 0, 0),
    JS_FS("out", myjs_out, 1, 0, 0),
    JS_FS_END
  };

  if (!JS_DefineFunctions(cx, global, myjs_global_functions))
    return JS_FALSE;

  nuiFunction* pF = nuiAddFunction("TestFunction", TestFunction);
  JSFunction* pJSF = JS_DefineFunction(cx, global, "TestFunction", nuiGenericJSFunction<false>, 3, 0);
  gFunctions[pJSF] = pF;


  /* Your application code here. This may include JSAPI calls
     to create your own custom JS objects and run scripts. */

  const char* script = "out(TestFunction(\"bleh!\", 42, 42.42));";
  jsval rval;
  JSBool res = JS_EvaluateScript(cx, global, script, strlen(script), "<inline>", 0, &rval);

  /* Cleanup. */
  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  JS_ShutDown();
  return 0;
}

/*
 * MainWindow
 */

void TestVariant();
void TestBinding();


MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  TestVariant();
  nuiInitBindings();
  TestBinding();

  JSTest();

  LoadCSS(_T("rsrc:/css/main.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  // image in the first box's cell
  nuiImage* pImg = new nuiImage();
  pImg->SetObjectName(_T("MyImage"));
  pImg->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pImg);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // button in the second cell : we use the default decoration for this button, but you could use the css to assign your own decoration
  nuiButton* pButton = new nuiButton();
  pButton->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pButton);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // click event on button
  mEventSink.Connect(pButton->Activated, &MainWindow::OnButtonClick);
  
  // label with border in the button (put the label string in the button's constructor if you don't need borders)
  nuiLabel* pButtonLabel = new nuiLabel(_T("click!"));
  pButtonLabel->SetPosition(nuiCenter);
  pButtonLabel->SetBorder(8,8);
  pButton->AddChild(pButtonLabel);

  // label with decoration in the third cell
  mMyLabel = new nuiLabel(_T("my label"));
  mMyLabel->SetObjectName(_T("MyLabel"));
  mMyLabel->SetPosition(nuiCenter);
  pLayoutBox->AddCell(mMyLabel);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
}



bool MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
  
  return true; // means the event is caught and not broadcasted
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}
