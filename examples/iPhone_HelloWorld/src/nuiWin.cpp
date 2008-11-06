#include "nui.h"

#include "nuiWin.h"
#include "nuiButton.h"

nuiWin::nuiWin( const nglContextInfo& rContext,
                        const nglWindowInfo& rInfo,
                        const nglContext* pShared)
: nuiMainWindow(rContext, rInfo, pShared, nglPath((_T("rsrc:"))))
{
  SetObjectClass(_T("nuiWin"));
  SetObjectName(_T("nuiWin"));

#ifdef _UIKIT_
  OnCreation();
  Invalidate();
  OnPaint();
#endif//_UIKIT_
}

nuiWin::~nuiWin()
{
}

void nuiWin::OnCreation()
{
//SetFrameRateLimit(20.f);
  AddChild(new nuiButton(_T("Hello World!")));
}

void nuiWin::OnClose()
{
}
