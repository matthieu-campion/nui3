/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiMetaDecoration.h"


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



  
bool nuiMetaDecoration::Load(const nuiXMLNode* pNode)
{
  // do nothing
  return false;
}

nuiXMLNode* nuiMetaDecoration::Serialize(nuiXMLNode* pNode)
{
  // do nothing
  return NULL;
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
    pDecoration->ClientToGlobalRect(rect);
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
    pDecoration->ClientToGlobalRect(rect);
    pDecoration->DrawFront(pContext, pWidget, rect);
    
    ++it;
  }
}

  
nuiRect nuiMetaDecoration::GetIdealClientRect() const
{
  std::vector<nuiDecoration*>::const_iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::const_iterator end = mDecorations.end();
  
  nuiRect rect;
  
  while (it != end)
  {
    nuiDecoration* pDecoration = (*it);
    NGL_ASSERT(pDecoration);
    
    rect.Union(rect, pDecoration->GetIdealClientRect());
    
    ++it;
  }
  
  return rect;
}

nuiSize nuiMetaDecoration::GetBorder(nuiPosition position) const
{
  std::vector<nuiDecoration*>::const_iterator it = mDecorations.begin();
  std::vector<nuiDecoration*>::const_iterator end = mDecorations.end();
  
  nuiSize border = 0;
  
  
  while (it != end)
  {
    nuiDecoration* pDecoration = (*it);
    NGL_ASSERT(pDecoration);
    
    border = MAX(border, pDecoration->GetBorder(position));
    
    ++it;
  }
  
  return border;
}

  
void nuiMetaDecoration::AddDecoration(nuiDecoration* pDeco)
{
  mDecorations.push_back(pDeco);
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
    
  return mDecorations[mDecorations.size()-1]->GetName();
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

}

