/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSpiderMonkey_Android.h"


nuiSpiderMonkey::nuiSpiderMonkey(uint32 MaxBytes)
: nuiScriptEngine(_T("spidermonkey"), _T("javascript"))
{

}

nuiSpiderMonkey::~nuiSpiderMonkey()
{

}
