/*
 *  nuiLabelRenamer.h
 *  libnui
 *
 *  Created by Sebastien Metrot on 1/3/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
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
    rect.SetSize(rect.GetWidth() + shadeSize*2.f, rect.GetHeight() + shadeSize);
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
    
    mSink.Connect(mpEditLine->Aborted,  &nuiLabelRenamer::OnEditLineCanceled, mpEditLine);
    mSink.Connect(mpEditLine->Selected, &nuiLabelRenamer::OnEditLineSelected, mpEditLine);
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
    if (!mpEditLine->IsInside(X,Y))
      return OnEditLineSelected(NULL);
    else
      return false;
  }
  
private:
  nuiEventSink<nuiLabelRenamer> mSink;

  bool OnEditLineCanceled(const nuiEvent& rEvent)
  {
    Canceled();
    Trash();
    return true;
  }

  bool OnEditLineSelected(const nuiEvent& rEvent)
  {
    mText = mpEditLine->GetText();
    mRejectName = false;
    
    if (mText == mpLabel->GetText())
    {
      Canceled();
      Trash();
      return true;
    }

    Renamed();

    if (!mRejectName)
    {
      if (mAutoChangeLabel)
        mpLabel->SetText(mText);
        
      Trash();
      return true;
    }
    
    return false;
  }

  nuiEditLine* mpEditLine;
  nglString mText;
  bool mAutoChangeLabel;
  nuiLabel* mpLabel;
  bool mRejectName;
};


#endif //ifndef __nuiLabelRenamer_h__
