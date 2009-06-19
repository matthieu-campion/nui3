/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLView.h"
#include "nuiHTTP.h"
#include "nuiFontManager.h"

class nuiHTMLContext
{
public:
  nuiHTMLContext()
  {
    mX = 0;
    mY = 0;
    mW = 0;
    mH = 0;

    mLeftMargin = 0;

    mIdealWidth = 800;
    mMaxWidth = 0;
    mMaxHeight = 0;
    
    mVSpace = 0;
    mHSpace = 0;
    
    mSetLayout = false;
    //mLine;
    mpNode = NULL;
  }
  
  nuiHTMLContext(const nuiHTMLContext& rContext)
  {
    mX = rContext.mX;
    mY = rContext.mY;
    mW = rContext.mW;
    mH = rContext.mH;
    
    mLeftMargin = rContext.mLeftMargin;

    mIdealWidth = rContext.mIdealWidth;
    mMaxWidth = rContext.mMaxWidth;
    mMaxHeight = rContext.mMaxHeight;
    
    mVSpace = rContext.mVSpace;
    mHSpace = rContext.mHSpace;
    
    mSetLayout = rContext.mSetLayout;
    mLine = rContext.mLine;
    mpNode = rContext.mpNode;
  }
  
  float mX;
  float mY;
  float mW;
  float mH;

  float mLeftMargin;

  float mIdealWidth;
  float mMaxWidth;
  float mMaxHeight;
  
  float mVSpace;
  float mHSpace;
  
  nuiWidgetList mLine;  
  nuiFontRequest mFont;
  bool mSetLayout;
  nuiHTMLNode* mpNode;
};

nuiHTMLView::nuiHTMLView(float IdealWidth)
{
  mpHTML = NULL;
  mIdealWidth = IdealWidth;
  mVSpace = 2.0f;
  mHSpace = 0.0f;
}

nuiHTMLView::~nuiHTMLView()
{
  delete mpHTML;
}

nuiRect nuiHTMLView::CalcIdealSize()
{
  float IdealWidth = mIdealWidth;
  if (mRect.GetWidth() > 0)
    IdealWidth = mRect.GetWidth();
  Clear();
  nuiHTMLContext context;
  context.mSetLayout = true;
  WalkTree(mpHTML, context);
//  return nuiRect(context.mMaxWidth, context.mH);
  return nuiRect(800, 600);
}

void nuiHTMLView::AddElement(nuiHTMLContext& rContext, nuiWidgetPtr pWidget)
{
  AddChild(pWidget);
  
  nuiRect r(pWidget->GetIdealRect());
  if ((rContext.mW != rContext.mLeftMargin) && (rContext.mW + r.GetWidth() > rContext.mIdealWidth))
    LayoutLine(rContext);
  
  rContext.mH = MAX(rContext.mH, r.GetHeight());
  rContext.mW += r.GetWidth();
  if (!rContext.mLine.empty())
    rContext.mW += rContext.mHSpace;
  rContext.mLine.push_back(pWidget);
}

void nuiHTMLView::LayoutLine(nuiHTMLContext& rContext)
{
  // Process the line
  if (rContext.mSetLayout)
  {
    int32 size = rContext.mLine.size();
    rContext.mX = rContext.mLeftMargin;
    for (int32 j = 0; j < size; j++)
    {
      nuiWidgetPtr pWidget = rContext.mLine[j];
      nuiRect r(pWidget->GetIdealRect());
      r.SetHeight(rContext.mH);
      r.MoveTo(rContext.mX, rContext.mY);
      pWidget->SetLayout(r);
      NGL_OUT(_T("%ls\n"), r.GetValue().GetChars());
      rContext.mX += r.GetWidth();
      rContext.mY += rContext.mHSpace;
    }
  }
  rContext.mMaxWidth = MAX(rContext.mMaxWidth, rContext.mW);
  rContext.mLine.clear();
  rContext.mY += rContext.mH + rContext.mVSpace;
  rContext.mH = 0;
  rContext.mW = 0;
}

bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  Clear();
  nuiHTMLContext context;
  context.mSetLayout = true;
  WalkTree(mpHTML, context);
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  nuiSimpleContainer::Draw(pContext);
  return true;
}

void nuiHTMLView::SetIdealWidth(float IdealWidth)
{
  mIdealWidth = IdealWidth;
  InvalidateLayout();
}

float nuiHTMLView::GetIdealWidth() const
{
  return mIdealWidth;
}

float nuiHTMLView::GetVSpace() const
{
  return mVSpace;
}

float nuiHTMLView::GetHSpace() const
{
  return mHSpace;
}

void nuiHTMLView::SetVSpace(float VSpace)
{
  mVSpace = VSpace;
  InvalidateLayout();
}

void nuiHTMLView::SetHSpace(float HSpace)
{
  mHSpace = HSpace;
  InvalidateLayout();
}

bool nuiHTMLView::SetText(const nglString& rHTMLText)
{
  Clear();
  delete mpHTML;
  mpHTML = new nuiHTML();
  
  std::string str(rHTMLText.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = mpHTML->Load(mem);
  
  InvalidateLayout();
  return res;
}

bool nuiHTMLView::SetURL(const nglString& rURL)
{
  nuiHTTPRequest request(rURL);
  nuiHTTPResponse* pResponse = request.SendRequest();
  if (!pResponse)
    return false;
  
  nuiHTML* pHTML = new nuiHTML();
  nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
  
  bool res = pHTML->Load(mem);
  
  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
    InvalidateLayout();
  }
  return res;
}



bool nuiHTMLView::InterpretTree(nuiHTMLContext& rContext)
{
  nuiHTMLNode* pNode = rContext.mpNode;
  
  switch (pNode->GetType())
  {
    case nuiHTML::eNode_Root:
    case nuiHTML::eNode_Text:
    case nuiHTML::eNode_Start:
    case nuiHTML::eNode_End:
    case nuiHTML::eNode_StartEnd:
      NGL_OUT(_T("Interpret %ls - %ls\n"), pNode->GetName().GetChars(), pNode->GetText().GetChars());
      if (!pNode->GetText().IsEmpty())
        AddElement(rContext, new nuiLabel(pNode->GetText()));
      return true;
    case nuiHTML::eNode_ProcIns:
    case nuiHTML::eNode_Comment:
    case nuiHTML::eNode_DocType:
    case nuiHTML::eNode_CDATA:
    case nuiHTML::eNode_Section:
    case nuiHTML::eNode_Asp:
    case nuiHTML::eNode_Jste:
    case nuiHTML::eNode_Php:
    case nuiHTML::eNode_XmlDecl:
    default:
      return false;
      break;
  }
  return true;
}

void nuiHTMLView::WalkTree(nuiHTMLNode* pNode, const nuiHTMLContext& rContext)
{
  nuiHTMLContext context(rContext);
  context.mpNode = pNode;
  if (!InterpretTree(context))
    return;
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    WalkTree(pChild, context);
  }
  LayoutLine(context);
}

