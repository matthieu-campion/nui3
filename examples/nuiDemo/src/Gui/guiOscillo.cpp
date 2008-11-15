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
  
  // we ask for a 30fps rendering of this object, that's enough for our needs
  StartAutoDraw(30);
}  

guiOscillo::~guiOscillo()
{

}


// virtual, overloaded from nuiWidget, to be able to draw the oscillo manually
bool guiOscillo::Draw(nuiDrawContext* pContext)
{
 if (mrData.empty())
    return false;
  
  const nuiRect& rect = GetRect();
  
  uint32 s1 = mrData[0].size();
  uint32 s2 = rect.GetWidth();
  uint32 count = MIN(s1, s2);
  nuiSize hi = rect.GetHeight();
  nuiSize mid = hi / 2;

  nuiColor color[2];
  color[0] = nuiColor(84,117,203);
  color[1] = nuiColor(200,0,0);
  
  for (uint32 c = 0; c < mrData.size(); c++)
  {
    
    nuiRenderArray array(GL_LINE_STRIP);
    array.EnableArray(nuiRenderArray::eColor, true);
    
    array.SetColor(color[c]);
    for (uint32 s = 0; s < count; s++)
    {
      float value = mrData[c][s];
      array.SetVertex(s, mid + hi * value);
      array.PushVertex();
    }
    
    pContext->DrawArray(array);
  }
  
  return true;
}
