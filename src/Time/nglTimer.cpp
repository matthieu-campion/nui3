/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"


/*
 * User callback
 */

void nglTimer::OnTick(nglTime Elapsed)
{
  NGL_DEBUG( NGL_LOG(_T("timer"), NGL_LOG_INFO, _T("tick [period: %.3fs, elapsed: %.3fs]"), (double)GetPeriod(), (double)Elapsed); )
}
