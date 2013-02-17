/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"


nuiMetaDecoration::nuiMetaDecoration(const nglString& rName)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiMetaDecoration")))
    InitAttributes();

}

nuiMetaDecoration::~nuiMetaDecoration()
{
  Clear();
}




void nuiMetaDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("Decoration")), nuiUnitNone,
    nuiAttribute<const nglString&>::GetterDelegate(this, &nuiMetaDecoration::GetDecoration),
    nuiAttribute<const nglString&>::SetterDelegate(this, &nuiMetaDecoration::AddDecoration)));
  
}

  
//virtual
void nuiMetaDecoration::DrawBack(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  std::vector<nuiDecoration*>::iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::iterator end = mDecorations.end();
  
  nuiRect r = pWidget->GetRect().Size();
  
  while (it != end)
  {
    nuiDecoration* pDecoration = (*it);
    NGL_ASSERT(pDecoration);
    
    if (pDecoration->GetLayer() != eLayerBack)
    {
      ++it;
      continue;
    }
    
    nuiRect rect = r;
    pDecoration->ClientToGlobalRect(rect, pWidget);
    pDecoration->DrawBack(pContext, pWidget, rect);
    
    ++it;
  }
}


//virtual
void nuiMetaDecoration::DrawFront(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  std::vector<nuiDecoration*>::iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::iterator end = mDecorations.end();
  
  nuiRect r = pWidget->GetRect().Size();
  
  while (it != end)
  {
    nuiDecoration* pDecoration = (*it);
    NGL_ASSERT(pDecoration);
    
    if (pDecoration->GetLayer() != eLayerFront)
    {
      ++it;
      continue;
    }

    nuiRect rect = r;
    pDecoration->ClientToGlobalRect(rect, pWidget);
    pDecoration->DrawFront(pContext, pWidget, rect);
    
    ++it;
  }
}

  
nuiRect nuiMetaDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  std::vector<nuiDecoration*>::const_iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::const_iterator end = mDecorations.end();
  
  nuiRect rect;
  
  while (it != end)
  {
    nuiDecoration* pDecoration = (*it);
    NGL_ASSERT(pDecoration);
    
    rect.Union(rect, pDecoration->GetIdealClientRect(pWidget));
    
    ++it;
  }
  
  return rect;
}

nuiSize nuiMetaDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
{
  
  std::vector<nuiDecoration*>::const_iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::const_iterator end = mDecorations.end();
  
  nuiSize border = 0;
  
  
  while (it != end)
  {
    nuiDecoration* pDecoration = (*it);
    NGL_ASSERT(pDecoration);
    
    if (pDecoration->IsBorderEnabled())
      border = MAX(border, pDecoration->GetBorder(position, pWidget));
    
    ++it;
  }
  
  return border;
}

  
void nuiMetaDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  rLeft = 0;
  rRight = 0;
  rTop = 0;
  rBottom = 0;
  rHorizontal = 0;
  rVertical = 0;

  if (!mBorderEnabled)
  {
    return;
  }
  
  std::vector<nuiDecoration*>::const_iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::const_iterator end = mDecorations.end();
  
  
  while (it != end)
  {
    nuiDecoration* pDecoration = *it;
    NGL_ASSERT(pDecoration);
    
    if (pDecoration->IsBorderEnabled())
    {
      rLeft = MAX(rLeft, pDecoration->GetBorder(nuiLeft, pWidget));
      rRight = MAX(rRight, pDecoration->GetBorder(nuiRight, pWidget));
      rTop = MAX(rTop, pDecoration->GetBorder(nuiTop, pWidget));
      rBottom = MAX(rBottom, pDecoration->GetBorder(nuiBottom, pWidget));
      rHorizontal = MAX(rHorizontal, pDecoration->GetBorder(nuiFillHorizontal, pWidget));
      rVertical = MAX(rVertical, pDecoration->GetBorder(nuiFillVertical, pWidget));
    }
    
    ++it;
  }
}


void nuiMetaDecoration::AddDecoration(nuiDecoration* pDeco)
{
  mDecorations.push_back(pDeco);
  Changed();
}

void nuiMetaDecoration::AddDecoration(const nglString& rDecoName)
{
  nuiDecoration* pDeco = nuiDecoration::Get(rDecoName);
  if (!pDeco)
    return;
    
  AddDecoration(pDeco);
}

const nglString& nuiMetaDecoration::GetDecoration()
{
  if (mDecorations.size() == 0)
  {
    static const nglString theWindBlows (nglString::Null);  
    return theWindBlows;
  }
    
  return mDecorations[mDecorations.size()-1]->GetObjectName();
}


void nuiMetaDecoration::Clear()
{
//#FIXME : no, nuiMetaDecoration doesn't have to release anything.
// please confirm before deleting this code
//  std::vector<nuiDecoration*>::iterator it = mDecorations.begin();
//  std::vector<nuiDecoration*>::iterator end = mDecorations.end();
//  
//  
//  while (it != end)
//  {
//    nuiDecoration* pDecoration = (*it);
//    NGL_ASSERT(pDecoration);
//    
//    pDecoration->Release();
//    
//    ++it;
//  }

  mDecorations.clear();

  Changed();
}

