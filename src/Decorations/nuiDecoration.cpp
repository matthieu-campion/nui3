/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDecoration.h"

nuiDecoration::nuiDecoration(const nglString& rName)
{
  if (SetObjectClass(_T("nuiDecoration")))
    InitAttributes();

  SetObjectName(rName);
    
  mUseWidgetAlpha = true;
  
  mLayer = eLayerBack;

  AddDecoration(this);
}



void nuiDecoration::InitAttributes()
{
  nuiAttribute<bool>* attUseWidgetAlpha = new nuiAttribute<bool>
  (nglString(_T("UseWidgetAlpha")), nuiUnitNone,
   nuiAttribute<bool>::GetterDelegate(this, &nuiDecoration::IsWidgetAlphaUsed),
   nuiAttribute<bool>::SetterDelegate(this, &nuiDecoration::UseWidgetAlpha));
	AddAttribute(_T("UseWidgetAlpha"), attUseWidgetAlpha);
  
  AddAttribute(new nuiAttribute<nuiDecorationLayer>
  (nglString(_T("Layer")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiDecoration::GetLayer), 
   nuiMakeDelegate(this, &nuiDecoration::SetLayer)));
 
}



nuiDecoration::~nuiDecoration()
{
  DelDecoration(this);
}


void nuiDecoration::SetLayer(nuiDecorationLayer layer)
{
  mLayer = layer;
  Changed();
}


nuiDecorationLayer nuiDecoration::GetLayer()
{
  return mLayer;
}




void nuiDecoration::SetName(const nglString& rName)
{
  nglString name(GetName());
  DelDecoration(this);
  SetObjectName(rName);
  AddDecoration(this);
  Changed();
}

const nglString& nuiDecoration::GetName() const
{
  return GetProperty(_T("Name"));
}

void nuiDecoration::UseWidgetAlpha(bool use)
{
  mUseWidgetAlpha = use;
  Changed();
}

bool nuiDecoration::IsWidgetAlphaUsed()
{
  return mUseWidgetAlpha;
}

nuiDecoration* nuiDecoration::Get(const nglString& rName, bool silent)
{
  DecorationMap::iterator it = mDecorations.find(rName);
  DecorationMap::iterator end = mDecorations.end();
  
  if (it == end)
  {
//    if (!silent)
//      NGL_OUT(_T("nuiDecoration warning : could not retrieve '%ls'\n"), rName.GetChars());
    return NULL;
  }
  
  nuiDecoration* pDecoration= it->second;
  pDecoration->Acquire();
  return pDecoration;
}

void nuiDecoration::AddDecoration(nuiDecoration* pDecoration)
{
  const nglString& name(pDecoration->GetObjectName());
  DecorationMap::iterator it = mDecorations.find(name);
  if (it != mDecorations.end())
    it->second->Release();
  mDecorations[name] = pDecoration;
  pDecoration->Acquire();
  DecorationsChanged();
}

void nuiDecoration::DelDecoration(nuiDecoration* pDecoration)
{
  nglString name(pDecoration->GetObjectName());
  
  DecorationMap::iterator it = mDecorations.find(name);
  if (it != mDecorations.end() && pDecoration == it->second)
    mDecorations.erase(it);
  DecorationsChanged();
}

nuiDecoration::DecorationMap nuiDecoration::mDecorations;
nuiSimpleEventSource<0> nuiDecoration::DecorationsChanged;

const nuiDecoration::DecorationMap& nuiDecoration::Enum()
{
  return mDecorations;
}


//virtual 
void nuiDecoration::DrawBack(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  if (mLayer != eLayerBack)
    return;
    
  Draw(pContext, pWidget, rRect);
}

// virtual
void nuiDecoration::DrawFront(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  if (mLayer != eLayerFront)
    return;
    
  Draw(pContext, pWidget, rRect);
}

// virtual 
void nuiDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  //do nothing
}


void nuiDecoration::GlobalToClientRect(nuiRect& rRect) const
{
  nuiRect clientRect = GetIdealClientRect();
//  nuiSize bordertop = GetBorder(nuiTop);
//  nuiSize borderleft = GetBorder(nuiLeft);
  nuiSize borderright = GetBorder(nuiRight);
  nuiSize borderbottom = GetBorder(nuiBottom);
  

  float X1 = (float)clientRect.Left();
  float X2 = (float)clientRect.Right();
  float X3 = (float)clientRect.Right()+borderright;

  float Y1 = (float)clientRect.Top();
  float Y2 = (float)clientRect.Bottom();
  float Y3 = (float)clientRect.Bottom()+borderbottom;

  const float x0 = (float)rRect.Left();
  const float x1 = x0 + X1;
  const float x3 = (float)rRect.Right();
  const float x2 = x3 - (X3 - X2);

  const float y0 = (float)rRect.Top();
  const float y1 = y0 + Y1;
  const float y3 = (float)rRect.Bottom();
  const float y2 = y3 - (Y3 - Y2);

  rRect.Set(x1, y1, x2, y2, false);
}

void nuiDecoration::ClientToGlobalRect(nuiRect& rRect) const
{
  nuiSize bordertop = GetBorder(nuiTop);
  nuiSize borderleft = GetBorder(nuiLeft);
  nuiSize borderright = GetBorder(nuiRight);
  nuiSize borderbottom = GetBorder(nuiBottom);

  rRect.Set(rRect.Left() - borderleft,
            rRect.Top() - bordertop,
            rRect.Right() + borderright,
            rRect.Bottom() + borderbottom,
            false);
            
            
}




nuiRect nuiDecoration::GetMinimumClientRect() const
{
  return GetIdealClientRect().Size();
}

nuiRect nuiDecoration::GetMaximumClientRect() const
{
  return GetIdealClientRect().Size();
}

