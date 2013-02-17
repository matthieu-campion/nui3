/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiDecoration::nuiDecoration(const nglString& rName)
{
  if (SetObjectClass(_T("nuiDecoration")))
    InitAttributes();

  SetObjectName(rName);
    
  mUseWidgetAlpha = true;
  mBorderEnabled = true;
  mRedrawOnHover = false;

  
  mLayer = eLayerBack;

  AddDecoration(this);
}



void nuiDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("EnableBorder")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiDecoration::IsBorderEnabled),
                nuiMakeDelegate(this, &nuiDecoration::EnableBorder)));
  
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("UseWidgetAlpha")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiDecoration::IsWidgetAlphaUsed),
                nuiMakeDelegate(this, &nuiDecoration::UseWidgetAlpha)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("RedrawOnHover")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiDecoration::GetRedrawOnHover),
                nuiMakeDelegate(this, &nuiDecoration::RedrawOnHover)));
  
  
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


nuiDecorationLayer nuiDecoration::GetLayer() const
{
  return mLayer;
}

void nuiDecoration::EnableBorder(bool set)
{
  mBorderEnabled = set;
}

bool nuiDecoration::IsBorderEnabled() const
{
  return mBorderEnabled;
}




void nuiDecoration::SetObjectName(const nglString& rName)
{
  DelDecoration(this);
  nuiObject::SetObjectName(rName);
  AddDecoration(this);
  Changed();
}

void nuiDecoration::UseWidgetAlpha(bool use)
{
  mUseWidgetAlpha = use;
  Changed();
}

bool nuiDecoration::IsWidgetAlphaUsed() const
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
//      NGL_OUT(_T("nuiDecoration warning : could not retrieve '%s'\n"), rName.GetChars());
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
  pDecoration->Acquire();
  if (it != mDecorations.end())
    it->second->Release();
  mDecorations[name] = pDecoration;
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


void nuiDecoration::GlobalToClientRect(nuiRect& rRect, const nuiWidget* pWidget) const
{
  nuiRect clientRect = GetIdealClientRect(pWidget);
//  nuiSize bordertop = GetBorder(nuiTop, pWidget);
//  nuiSize borderleft = GetBorder(nuiLeft, pWidget);
  nuiSize borderright = GetBorder(nuiRight, pWidget);
  nuiSize borderbottom = GetBorder(nuiBottom, pWidget);
  

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

void nuiDecoration::ClientToGlobalRect(nuiRect& rRect, const nuiWidget* pWidget) const
{
  nuiSize bordertop = GetBorder(nuiTop, pWidget);
  nuiSize borderleft = GetBorder(nuiLeft, pWidget);
  nuiSize borderright = GetBorder(nuiRight, pWidget);
  nuiSize borderbottom = GetBorder(nuiBottom, pWidget);

  rRect.Set(rRect.Left() - borderleft,
            rRect.Top() - bordertop,
            rRect.Right() + borderright,
            rRect.Bottom() + borderbottom,
            false);
            
            
}




nuiRect nuiDecoration::GetMinimumClientRect(const nuiWidget* pWidget) const
{
  return GetIdealClientRect(pWidget).Size();
}

nuiRect nuiDecoration::GetMaximumClientRect(const nuiWidget* pWidget) const
{
  return GetIdealClientRect(pWidget).Size();
}

bool ShouldSkipAttrib(const nglString& rName)
{
  return rName == _T("Class") || rName == _T("Name");
}

nglString nuiDecoration::GetCSSDeclaration() const
{
  nglString decl;
  decl.Add(_T("@")).Add(GetObjectClass()).Add(_T(" ")).Add(GetObjectName()).AddNewLine().Add(_T("{")).AddNewLine();
  
  // build attributes list
  std::map<nglString, nuiAttribBase> attributes;
  GetAttributes(attributes);
  uint32 i = 0;
  std::map<nglString, nuiAttribBase>::const_iterator it_a = attributes.begin();
  std::map<nglString, nuiAttribBase>::const_iterator end_a = attributes.end();
  
  while (it_a != end_a)
  {
    nglString pname(it_a->first);
    if (!ShouldSkipAttrib(pname))
    {
      
      //printf("\tattr: %s\n", pname.GetChars());
      nuiAttribBase Base = it_a->second;
      
      nglString value;
      switch (Base.GetDimension())
      {
        case 0:
          Base.ToString(value);
          break;
        case 1:
        {
          nglString str;
          uint32 count = Base.GetIndexRange(0);
          for (uint32 i = 0; i < count; i++)
          {
            Base.ToString(i, str);
            value.Add(i).Add(_T(":")).Add(str).Add("\t");
          }
          value.Trim(_T('\t'));
        }
          break;
        case 2:
        {
          nglString str;
          uint32 counti = Base.GetIndexRange(0);
          uint32 countj = Base.GetIndexRange(1);
          for (uint32 i = 0; i < MIN(10, counti); i++)
          {
            for (uint32 j = 0; j < MIN(10, countj); j++)
            {
              Base.ToString(i, j, str);
              value.Add(i).Add(",").Add(j).Add(_T(":")).Add(str).Add("\t");
            }
          }
          value.Trim(_T('\t'));
        }
          
      }

      if (value.Find(' ') >= 0)
        value = nglString(_T("\"")).Add(value).Add(_T("\""));
      if (value.IsEmpty())
        value = _T("\"\"");
      decl.Add(_T("  ")).Add(Base.GetName()).Add(_T(" : ")).Add(value).Add(_T(";")).AddNewLine();
    }
    
    ++it_a;
    i++;
  }
  
  decl.Add(_T("}")).AddNewLine().AddNewLine();
  return decl;
}

nuiAttributeEditor* nuiDecoration::GetAttributeEditor(void* pTarget, nuiAttribute<const nglString&>* pAttribute)
{
  std::vector<std::pair<nglString, nglString> > values;
  DecorationMap::const_iterator it = mDecorations.begin();
  DecorationMap::const_iterator end = mDecorations.end();
  
  values.push_back(std::make_pair(_T("-- None --"), nglString::Null));
  while (it != end)
  {
    values.push_back(std::make_pair(it->first, it->first));
    ++it;
  }

  return new nuiComboAttributeEditor<const nglString&>(nuiAttrib<const nglString&>(pTarget, pAttribute), values);
}

void nuiDecoration::RedrawOnHover(bool set)
{
  mRedrawOnHover = set;
}

bool nuiDecoration::GetRedrawOnHover() const
{
  return mRedrawOnHover;
}

void nuiDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mBorderEnabled)
  {
    rLeft = rRight = rTop = rBottom = rHorizontal = rVertical = 0;
    return;
  }
  
  rLeft = GetBorder(nuiLeft, pWidget);
  rRight = GetBorder(nuiRight, pWidget);
  rTop = GetBorder(nuiTop, pWidget);
  rBottom = GetBorder(nuiBottom, pWidget);
  rHorizontal = GetBorder(nuiFillHorizontal, pWidget);
  rVertical = GetBorder(nuiFillVertical, pWidget);
}

//////////////////// nuiDecoration Creators:

nuiDecoration* nuiDecoration::CreateDecoration(const nglString& rDecorationTypeName, const nglString& rDecorationName)
{
  std::map<nglString, DecorationCreatorFunction>::const_iterator it = mDecorationCreators.find(rDecorationTypeName);
  if (it != mDecorationCreators.end())
    return it->second(rDecorationName);
  return NULL;
}

void nuiDecoration::AddDecorationType(const nglString& rDecorationTypeName, const DecorationCreatorFunction& rCreatorDelegate)
{
  mDecorationCreators[rDecorationTypeName] = rCreatorDelegate;
}


#include "nuiFrame.h"
#include "nuiBorderDecoration.h"
#include "nuiColorDecoration.h"
#include "nuiImageDecoration.h"
#include "nuiGradientDecoration.h"
#include "nuiCheckerboardDecoration.h"
#include "nuiNavigationViewDecoration.h"
#include "nuiStateDecoration.h"
#include "nuiTreeHandleDecoration.h"
#include "nuiMetaDecoration.h"

void nuiDecoration::InitDecorationEngine()
{
  nuiDecoration::AddDecorationType(_T("nuiFrame"),                      &nuiCreateDecoration<nuiFrame>);
  nuiDecoration::AddDecorationType(_T("nuiBorderDecoration"),           &nuiCreateDecoration<nuiBorderDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiColorDecoration"),            &nuiCreateDecoration<nuiColorDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiImageDecoration"),            &nuiCreateDecoration<nuiImageDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiGradientDecoration"),         &nuiCreateDecoration<nuiGradientDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiCheckerboardDecoration"),     &nuiCreateDecoration<nuiCheckerboardDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiNavigationViewDecoration"),   &nuiCreateDecoration<nuiNavigationViewDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiStateDecoration"),            &nuiCreateDecoration<nuiStateDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiTreeHandleDecoration"),       &nuiCreateDecoration<nuiTreeHandleDecoration>);
  nuiDecoration::AddDecorationType(_T("nuiMetaDecoration"),             &nuiCreateDecoration<nuiMetaDecoration>);
}

std::map<nglString, nuiDecoration::DecorationCreatorFunction> nuiDecoration::mDecorationCreators;

void nuiDecoration::ExitDecorationEngine()
{
  DecorationsChanged.DisconnectAll();
  while (!mDecorations.empty())
  {
    DecorationMap::iterator it = mDecorations.begin();
    nuiDecoration* pDeco = it->second;
    //NGL_OUT(_T("Releasing decoration %p '%s'\n"), pDeco, pDeco->GetObjectName().GetChars());
    pDeco->Release();
  }
  mDecorationCreators.clear();
}

