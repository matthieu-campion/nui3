/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#ifndef __nuiLabelRenamer_h__
#define __nuiLabelRenamer_h__


//#include "nui.h"
#include "nuiLabel.h"
#include "nuiModalContainer.h"
#include "nuiShadeContainer.h"

class nuiLabelRenamer : public nuiModalContainer
{
public:
  nuiLabelRenamer(nuiLabel* pLabel, bool AutoChangeLabel = true)
  : nuiModalContainer(pLabel->GetTopLevel()),
    mSink(this)
  {
    mpLabel = pLabel;
    mText = mpLabel->GetText();
    mAutoChangeLabel = AutoChangeLabel;
    mRejectName = false;

    nuiSize shadeSize = 10.f;
    nuiRect rect = pLabel->GetRect().Size();
    pLabel->LocalToGlobal(rect);
    rect.SetSize(rect.GetWidth() + shadeSize*2.f, rect.GetHeight() + shadeSize * 2.0f);
    rect.Move(-shadeSize, 0.f);
    
    nuiFixed* pFixed = new nuiFixed();
    AddChild(pFixed);

    
    nuiShadeContainer* pShadeContainer = new nuiShadeContainer(shadeSize);
    pFixed->AddChild(pShadeContainer);
    
    pShadeContainer->SetUserRect(rect);
    nuiScrollView* pScrollView = new nuiScrollView(true, false);
    pShadeContainer->AddChild(pScrollView);
    pScrollView->SetBarSize(0.f);
    mpEditLine = new nuiEditLine(mText);
    pScrollView->AddChild(mpEditLine);
    
    mpEditLine->SetFont(pLabel->GetFont());
    mpEditLine->Focus();
    mpEditLine->SetColor(eNormalTextBg,   nuiColor(255,255,255));
    mpEditLine->SetColor(eNormalTextFg,   nuiColor(0,0,0));
    mpEditLine->SetColor(eSelectedTextFg, nuiColor("#3EFEFE"));
    mpEditLine->SetFollowModifications(true);
    mpEditLine->Do(nuiEditText::eSelectAll, NULL);
    mpEditLine->SetDecoration(NULL);
    
    mSink.Connect(mpEditLine->Aborted,  &nuiLabelRenamer::OnEditLineCanceled, mpEditLine);
    mSink.Connect(mpEditLine->Activated, &nuiLabelRenamer::OnEditLineSelected, mpEditLine);
  }

  nuiSimpleEventSource<0> Renamed;
  nuiSimpleEventSource<0> Canceled;

  const nglString& GetText() const
  {
    return mText;
  }
  
  nuiLabel* GetLabel ()
  {
    return mpLabel;
  }
  
  void RejectName()
  {
    mRejectName = true;
  }
  
  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
  {
    if (!mpEditLine->IsInsideFromRoot(X,Y))
    {
      nuiEvent e;
      OnEditLineSelected(e);
      return e.IsCanceled();
    }
    else
      return false;
  }
  
  void OnSetFocus(nuiWidgetPtr pTarget)
  {
    if (pTarget != mpEditLine)
    {
      Canceled();
      Trash();
    }
  }
  
private:
  nuiEventSink<nuiLabelRenamer> mSink;

  void OnEditLineCanceled(const nuiEvent& rEvent)
  {
    Canceled();
    Trash();
    rEvent.Cancel();
  }

  void OnEditLineSelected(const nuiEvent& rEvent)
  {
    mText = mpEditLine->GetText();
    mRejectName = false;
    
    if (mText == mpLabel->GetText())
    {
      Canceled();
      Trash();
      rEvent.Cancel();
      return;
    }

    Renamed();

    if (!mRejectName)
    {
      if (mAutoChangeLabel)
        mpLabel->SetText(mText);
        
      Trash();
      rEvent.Cancel();
      return;
    }
  }

  nuiEditLine* mpEditLine;
  nglString mText;
  bool mAutoChangeLabel;
  nuiLabel* mpLabel;
  bool mRejectName;
};


#endif //ifndef __nuiLabelRenamer_h__
