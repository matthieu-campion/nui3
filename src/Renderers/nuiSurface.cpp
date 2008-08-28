#include "nui.h"
#include "nuiSurface.h"
#include "nuiDrawContext.h"
// opengl es painter
#include "nuiGLESPainter.h"
//

nuiSurface::nuiSurface(const nuiSize& rWidth, const nuiSize& rHeight)
  : nuiObject()
{
  SetObjectClass(_T("nuiSurface"));
  mWidth = rWidth;
  mHeight= rHeight;
}
