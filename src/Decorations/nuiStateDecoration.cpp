/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"




nuiStateDecoration::nuiStateDecoration(const nglString& rName)
  : nuiDecoration(rName), mClientRect(0,0,0,0), mUseSourceClientRect(false)
{
  if (SetObjectClass(_T("nuiStateDecoration")))
    InitAttributes();
}


nuiStateDecoration::nuiStateDecoration(const nglString& rName, const nglString& rUp, const nglString& rDown, const nglString& rHoverOn, const nglString& rDisabled, const nglString& rDisabledSelected)
  : nuiDecoration(rName), mClientRect(0,0,0,0), mUseSourceClientRect(false)
{
  if (SetObjectClass(_T("nuiStateDecoration")))
    InitAttributes();

  if (!rUp.IsNull())
    SetState(nuiStateEnabled  | nuiStateReleased, GetDecoration(rUp));
  if (!rDown.IsNull())
    SetState(nuiStateEnabled  | nuiStatePressed,  GetDecoration(rDown));
  if (!rHoverOn.IsNull())
    SetState(nuiStateEnabled  | nuiStateHoverOn,  GetDecoration(rHoverOn));
  if (!rDisabled.IsNull())
    SetState(nuiStateDisabled | nuiStateReleased, GetDecoration(rDisabled));
  if (!rDisabledSelected.IsNull())
  {
    SetState(nuiStateDisabled | nuiStateSelected, GetDecoration(rDisabledSelected));
    SetState(nuiStateDisabled | nuiStatePressed, GetDecoration(rDisabledSelected));
  }
}



nuiStateDecoration::nuiStateDecoration(const nglString& rName, const nglString& rUp, const nglString& rDown, const nglString& rUpHoverOn,  const nglString& rDownHoverOn, const nglString& rDisabled, const nglString& rDisabledSelected)
  : nuiDecoration(rName), mClientRect(0,0,0,0), mUseSourceClientRect(false)
{
  if (SetObjectClass(_T("nuiStateDecoration")))
    InitAttributes();

  SetState(nuiStateEnabled  | nuiStateReleased | nuiStateHoverOff, GetDecoration(rUp));
  SetState(nuiStateEnabled  | nuiStateReleased | nuiStateHoverOn,  GetDecoration(rUpHoverOn));  
  SetState(nuiStateEnabled  | nuiStatePressed | nuiStateHoverOff,  GetDecoration(rDown));
  SetState(nuiStateEnabled  | nuiStatePressed | nuiStateHoverOn,  GetDecoration(rDownHoverOn));
  SetState(nuiStateDisabled | nuiStateReleased, GetDecoration(rDisabled));
  SetState(nuiStateDisabled | nuiStateSelected, GetDecoration(rDisabledSelected));
  SetState(nuiStateDisabled | nuiStatePressed, GetDecoration(rDisabledSelected));
} 


nuiStateDecoration::~nuiStateDecoration()
{
  std::map<uint32, nuiDecoration*>::iterator it = mStates.begin();
  std::map<uint32, nuiDecoration*>::iterator end = mStates.end();
  
  while (it != end)
  {
    nuiDecoration* pDecoration = it->second;
    NGL_ASSERT(pDecoration);
    
    pDecoration->Release();
    
    ++it;
  }
}


void nuiStateDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiStateDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiStateDecoration::SetSourceClientRect)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnUp")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoUp), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoUp)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnDown")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoDown), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoDown)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnUpHover")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoUpHover), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoUpHover)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnDownHover")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoDownHover), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoDownHover)));
    
  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnSelected")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoSelected), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoSelected)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnDisabled")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoDisabled), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoDisabled)));

  AddAttribute(new nuiAttribute<const nglString&>
     (nglString(_T("OnDisabledSelected")), nuiUnitNone,
      nuiMakeDelegate(this, &nuiStateDecoration::GetDecoDisabledSelected), 
      nuiMakeDelegate(this, &nuiStateDecoration::SetDecoDisabledSelected)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnUpDisabled")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoUpDisabled), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoUpDisabled)));

  AddAttribute(new nuiAttribute<const nglString&>
   (nglString(_T("OnDownDisabled")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiStateDecoration::GetDecoDownDisabled), 
    nuiMakeDelegate(this, &nuiStateDecoration::SetDecoDownDisabled)));


}



nuiDecoration* nuiStateDecoration::GetDecoration(const nglString& rName) const
{
  nuiDecoration* pDecoration = nuiDecoration::Get(rName, true);
  
  if (pDecoration)
    return pDecoration;
  nuiTexture* pTexture = nuiTexture::GetTexture(nglPath(rName));
  if (!pTexture || !pTexture->IsValid())
  {
    NGL_OUT(_T("nuiStateDecoration::GetDecoration warning : could not load graphic resource '%s'\n"), rName.GetChars());
    return NULL;
  }
  
  nuiFrame* pFrame = new nuiFrame(rName, pTexture, nuiRect(0, 0, pTexture->GetWidth(), pTexture->GetHeight()));
  return pFrame;
}


nuiDecoration* nuiStateDecoration::GetDecorationForWidgetState(const nuiWidget* pWidget) const
{
  const nuiButton* pButton = dynamic_cast<const nuiButton*>(pWidget);
  
  nuiStateDescription state = 0;
  nuiDecoration* pChoice4 = GetState(state);


  nuiDecoration* pChoice3 = NULL;
  nuiDecoration* pChoice2 = NULL;

  if (pButton)
  {
    state += pButton->IsPressed() ? nuiStatePressed : nuiStateReleased;
    pChoice3 = GetState(state);
  }
  else
  {
    state += pWidget->IsSelected() ? nuiStateSelected : nuiStateUnselected;
    pChoice2 = GetState(state);  
  }

  state += pWidget->IsEnabled(true) ? nuiStateEnabled : nuiStateDisabled;
  nuiDecoration* pChoice1 = GetState(state);

  state += pWidget->GetHover() ? nuiStateHoverOn : nuiStateHoverOff;
  nuiDecoration* pChoice0 = GetState(state);  
  
  nuiDecoration* pChoice = NULL;
  
  if (pChoice0)
    pChoice = pChoice0;
  else if (pChoice1)
    pChoice = pChoice1;
  else if (pChoice2)
    pChoice = pChoice2;
  else if (pChoice3)
    pChoice = pChoice3;
  else if (pChoice4)
    pChoice = pChoice4;

  NGL_ASSERT(pChoice != this);
  return pChoice;
}




//virtual
void nuiStateDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  NGL_ASSERT(pWidget);
  nuiDecoration* pChoice = GetDecorationForWidgetState(pWidget);
  
  if (pChoice)
  {
    nuiRect r = rRect;  
    r.SetPosition(nuiCenter, pWidget->GetRect().Size());
    pChoice->Draw(pContext, pWidget, r);
  }
}



nuiDecoration* nuiStateDecoration::GetState(nuiStateDescription state) const
{
  std::map<uint32, nuiDecoration*>::const_iterator it = mStates.find(state);
  std::map<uint32, nuiDecoration*>::const_iterator end = mStates.end();
 
  if (it == end)
    return NULL;
  
  return it->second;
}




nuiRect nuiStateDecoration::GetIdealSourceClientRect() const
{
  return mClientRect.Size();
}



nuiRect nuiStateDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  if (mUseSourceClientRect)
    return GetIdealSourceClientRect();
    
  std::map<uint32, nuiDecoration*>::const_iterator it = mStates.begin();
  std::map<uint32, nuiDecoration*>::const_iterator end = mStates.end();
  
  nuiRect rect;

  if (!pWidget)
  {
    while (it != end)
    {
      nuiDecoration* pDecoration = it->second;
      NGL_ASSERT(pDecoration);
      
      rect.Union(rect, pDecoration->GetIdealClientRect(pWidget));
      
      ++it;
    }
  }
  else
  {
    nuiDecoration* pChoice = GetDecorationForWidgetState(pWidget);
    if (pChoice)
    {
      return pChoice->GetIdealClientRect(pWidget);
    }
  }

  return rect;
}




nuiSize nuiStateDecoration::GetSourceBorder(nuiPosition position) const
{  
  switch (position)
  {
  case nuiLeft:
  case nuiRight:
    return mClientRect.Left();
    break;
  case nuiTop:
  case nuiBottom:
    return mClientRect.Top();
    break;
  case nuiFillHorizontal:
    return mClientRect.Left()*2;
    break;
  case nuiFillVertical:
    return mClientRect.Top()*2;
    break;
  case nuiNoPosition: break;
  case nuiTopLeft: break;
  case nuiTopRight: break;
  case nuiBottomLeft: break;
  case nuiBottomRight: break;
  case nuiCenter: break;
  case nuiTile: break;
  case nuiFill: break;
  case nuiFillLeft: break;
  case nuiFillRight: break;
  case nuiFillTop: break;
  case nuiFillBottom: break;
  }
  //we should'nt arrive here
  return NULL;
}

nuiSize nuiStateDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
{
  if (!mBorderEnabled)
    return 0;
  
  if (mUseSourceClientRect)
    return GetSourceBorder(position);

  if (pWidget)
  {
    nuiDecoration* pChoice = GetDecorationForWidgetState(pWidget);
    if (pChoice)
    {
      return pChoice->GetBorder(position, pWidget);
    }
    
    return 0;
  }
  
  nuiSize border = 0;
  std::map<uint32, nuiDecoration*>::const_iterator it = mStates.begin();
  std::map<uint32, nuiDecoration*>::const_iterator end = mStates.end();
  
  while (it != end)
  {
    nuiDecoration* pDecoration = it->second;
    NGL_ASSERT(pDecoration);
    
    border = MAX(border, pDecoration->GetBorder(position, pWidget));
    
    ++it;
  }
  
  return border;
}

void nuiStateDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mBorderEnabled)
  {
    rLeft = rRight = rTop = rBottom = rHorizontal = rVertical = 0;
    return;
  }
  
  if (mUseSourceClientRect)
  {
    rLeft = GetSourceBorder(nuiLeft);
    rRight = GetSourceBorder(nuiRight);
    rTop = GetSourceBorder(nuiTop);
    rBottom = GetSourceBorder(nuiBottom);
    rHorizontal = GetSourceBorder(nuiFillHorizontal);
    rVertical = GetSourceBorder(nuiFillVertical);
    return;
  }
  
  if (pWidget)
  {
    nuiDecoration* pChoice = GetDecorationForWidgetState(pWidget);
    if (pChoice)
    {
      rLeft = pChoice->GetBorder(nuiLeft, pWidget);
      rRight = pChoice->GetBorder(nuiRight, pWidget);
      rTop = pChoice->GetBorder(nuiTop, pWidget);
      rBottom = pChoice->GetBorder(nuiBottom, pWidget);
      rHorizontal = pChoice->GetBorder(nuiFillHorizontal, pWidget);
      rVertical = pChoice->GetBorder(nuiFillVertical, pWidget);
      return;
    }
    
    rLeft = 0;
    rRight = 0;
    rTop = 0;
    rBottom = 0;
    rHorizontal = 0;
    rVertical = 0;
    return;
  }
  
  std::map<uint32, nuiDecoration*>::const_iterator it = mStates.begin();
  std::map<uint32, nuiDecoration*>::const_iterator end = mStates.end();
  
  rLeft = 0;
  rRight = 0;
  rTop = 0;
  rBottom = 0;
  rHorizontal = 0;
  rVertical = 0;

  while (it != end)
  {
    nuiDecoration* pDecoration = it->second;
    NGL_ASSERT(pDecoration);
    
    rLeft = MAX(rLeft, pDecoration->GetBorder(nuiLeft, pWidget));
    rRight = MAX(rRight, pDecoration->GetBorder(nuiRight, pWidget));
    rTop = MAX(rTop, pDecoration->GetBorder(nuiTop, pWidget));
    rBottom = MAX(rBottom, pDecoration->GetBorder(nuiBottom, pWidget));
    rHorizontal = MAX(rHorizontal, pDecoration->GetBorder(nuiFillHorizontal, pWidget));
    rVertical = MAX(rVertical, pDecoration->GetBorder(nuiFillVertical, pWidget));
    
    ++it;
  }
}


void nuiStateDecoration::SetState(nuiStateDescription State, nuiDecoration* pDecoration)
{
  if (pDecoration)
    pDecoration->Acquire();

  std::map<uint32, nuiDecoration*>::iterator it = mStates.find(State);
  std::map<uint32, nuiDecoration*>::iterator end = mStates.end();
  if (it != end)
  {
    nuiDecoration* pOld = it->second;
    pOld->Release();
  }

  if (pDecoration)
    mStates[State] = pDecoration;
  else
    mStates.erase(State);
  
  Changed();
}

void nuiStateDecoration::SetState(nuiStateDescription State, const nglString& rDecorationName)
{  
  if (rDecorationName.IsNull())
    return;
  nuiDecoration* pDecoration = nuiDecoration::Get(rDecorationName, true);
  if (!pDecoration)
    return;
  SetState(State, pDecoration);
}





//***********************************************************************
//
// attributes setters/getters
//

const nuiRect& nuiStateDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiStateDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
  mUseSourceClientRect = true;
  Changed();
}


const nglString& nuiStateDecoration::GetDecoUp()
{
  return mDecoUp;
}

void nuiStateDecoration::SetDecoUp(const nglString& rPath)
{
  mDecoUp = rPath;
  nuiDecoration* pDeco = GetDecoration(mDecoUp);
  if (pDeco)
    SetState(nuiStateEnabled | nuiStateReleased, pDeco);  
}


const nglString& nuiStateDecoration::GetDecoDown()
{
  return mDecoDown;
}


void nuiStateDecoration::SetDecoDown(const nglString& rPath)
{
  mDecoDown = rPath;
  nuiDecoration* pDeco = GetDecoration(mDecoDown);
  if (pDeco)
    SetState(nuiStateEnabled | nuiStatePressed, pDeco);
}


const nglString& nuiStateDecoration::GetDecoUpHover()
{
  return mDecoUpHover;
}


void nuiStateDecoration::SetDecoUpHover(const nglString& rPath)
{
  mDecoUpHover = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStateEnabled | nuiStateReleased | nuiStateHoverOn, pDeco);
  
  RedrawOnHover(true);
}


const nglString& nuiStateDecoration::GetDecoDownHover()
{
  return mDecoDownHover;
}


void nuiStateDecoration::SetDecoDownHover(const nglString& rPath)
{
  mDecoDownHover = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStateEnabled | nuiStatePressed | nuiStateHoverOn, pDeco);
  RedrawOnHover(true);
}




const nglString& nuiStateDecoration::GetDecoDisabled()
{
  return mDecoUpDisabled;
}


void nuiStateDecoration::SetDecoDisabled(const nglString& rPath)
{
  mDecoUpDisabled = rPath;
  mDecoDownDisabled = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStateDisabled, pDeco);
}



const nglString& nuiStateDecoration::GetDecoDisabledSelected()
{
  return mDecoDisabledSelected;
}


void nuiStateDecoration::SetDecoDisabledSelected(const nglString& rPath)
{
  mDecoDisabledSelected = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStateDisabled | nuiStateSelected, pDeco);
}




const nglString& nuiStateDecoration::GetDecoUpDisabled()
{
  return mDecoUpDisabled;
}


void nuiStateDecoration::SetDecoUpDisabled(const nglString& rPath)
{
  mDecoUpDisabled = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStateReleased | nuiStateDisabled, pDeco);
}




const nglString& nuiStateDecoration::GetDecoDownDisabled()
{
  return mDecoDownDisabled;
}


void nuiStateDecoration::SetDecoDownDisabled(const nglString& rPath)
{
  mDecoDownDisabled = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStatePressed | nuiStateDisabled, pDeco);
}




const nglString& nuiStateDecoration::GetDecoSelected()
{
  return mDecoSelected;
}


void nuiStateDecoration::SetDecoSelected(const nglString& rPath)
{
  mDecoSelected = rPath;
  nuiDecoration* pDeco = GetDecoration(rPath);
  if (pDeco)
    SetState(nuiStateSelected, pDeco);
}




