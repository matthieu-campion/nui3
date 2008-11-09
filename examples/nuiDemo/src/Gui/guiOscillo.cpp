/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

//
// have a look to guiOscillo.h
//

#include "nui.h"
#include "Gui/guiOscillo.h"

guiOscillo::guiOscillo(const std::vector<std::vector<float> >& rData)
: nuiWidget(), mrData(rData)
{
  // Setting the object class is not mandatory here. 
  // We use it in the css stylesheet to apply a decoration to the guiOscillo object. 
  // We could set the object name instead (SetObjectName), it's nearly the same for that purpose 
  // (except that, in that case, in the css stylesheet, the "guiOscillo" reference should be put between quotes.
  // Have a look to Gui.css, you've got some examples.)
  SetObjectClass(_T("guiOscillo"));
  StartAutoDraw(30);
}  

guiOscillo::~guiOscillo()
{

}


// virtual, overloaded from nuiWidget, to be able to draw the oscillo manually
bool guiOscillo::Draw(nuiDrawContext* pContext)
{
  uint32 s1 = mrData[0].size();
  uint32 s2 = GetRect().GetWidth();
  uint32 count = MIN(s1, s2);
  nuiSize hi = GetRect().GetHeight();
  nuiSize mid = hi / 2;
  for (uint32 j = 0; j < mrData.size(); j++)
  {
    nglString colors[] =
    {
      _T("red"),
      _T("green")
    };
    uint32 colorcount = 2;
    
    nuiRenderArray array(GL_LINE_STRIP);
    nuiColor Color(colors[j % colorcount]);
    array.SetColor(Color);
    for (uint32 i = 0; i < count; i++)
    {
      float value = mrData[j][i];
      array.SetVertex(i, mid + hi * value);
      array.PushVertex();
    }
    
    pContext->DrawArray(array);
  }
  return true;
}
