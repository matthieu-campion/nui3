/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiScrollingLabel::nuiScrollingLabel(const nglString& rText, nuiTheme::FontStyle FontStyle, nuiScrollLoop ScrollLoop, bool smartScroll, bool centered)
: mScrollingLabelSink(this)
{
  SetObjectClass(_T("nuiScrollingLabel"));
  mTickCount = 0;
  mScrollLoop = ScrollLoop;
  mSmartScroll = smartScroll;
  mCentered = centered;
  mPPF = 1.0;
  mOldRect = nuiRect();
  mNeedSubStrings = true;
  mTextChanged = true;
  mCompleteLabelWidth = 0;
  
  mHPause = 2;
  mHWait = 2;
  mVPause = 2;
  mVWait = 0;
  
  mHFPS = DEFAULT_HORIZONTAL_FPS;
  mVFPS = DEFAULT_VERTICAL_FPS;
  
  if ((mScrollLoop == eScrollLoopHRightToLeft) || (mScrollLoop == eScrollLoopHLeftToRight) || (mScrollLoop == eScrollLoopHPingPong))
  {
    mOrientation = nuiHorizontal;
    mFPS = mHFPS;
  }
  else
  {
    mOrientation = nuiVertical;
    mFPS = mVFPS;
  }
  
  mCompleteString = rText;
  mpLabel = new nuiLabel(mCompleteString, FontStyle);
  AddChild(mpLabel);
  
  
  double period = 1.0 / mFPS;
  mpTimer = new nuiTimer(period); 
  mScrollingLabelSink.Connect(mpTimer->Tick, &nuiScrollingLabel::OnTimerChange);
  mpTimer->Start(true, true);
}

nuiScrollingLabel::nuiScrollingLabel(const nglString& rText, nuiFont* pFont, bool AlreadyAcquired, nuiScrollLoop ScrollLoop, bool smartScroll, bool centered)
: mScrollingLabelSink(this)
{
  SetObjectClass(_T("nuiScrollingLabel"));
  mTickCount = 0;
  mScrollLoop = ScrollLoop;
  mSmartScroll = smartScroll;
  mCentered = centered;
  mPPF = 1.0;
  mOldRect = nuiRect();
  mNeedSubStrings = true;
  mTextChanged = true;
  mCompleteLabelWidth = 0;
  
  mHPause = 2;
  mHWait = 2;
  mVPause = 2;
  mVWait = 0;
  
  mHFPS = DEFAULT_HORIZONTAL_FPS;
  mVFPS = DEFAULT_VERTICAL_FPS;
    
  if ((mScrollLoop == eScrollLoopHRightToLeft) || (mScrollLoop == eScrollLoopHLeftToRight) || (mScrollLoop == eScrollLoopHPingPong))
  {
    mOrientation = nuiHorizontal;
    mFPS = mHFPS;
  }
  else
  {
    mOrientation = nuiVertical;
    mFPS = mVFPS;
  }
  
  mCompleteString = rText;
  mpLabel = new nuiLabel(mCompleteString, pFont, AlreadyAcquired);
  AddChild(mpLabel);
  
  double period = 1.0 / mFPS;
  mpTimer = new nuiTimer(period); 
  mScrollingLabelSink.Connect(mpTimer->Tick, &nuiScrollingLabel::OnTimerChange);
  mpTimer->Start(true, true);
}


nuiScrollingLabel::nuiScrollingLabel(nuiLabel* pLabel, nuiScrollLoop ScrollLoop, bool smartScroll, bool centered)
: mScrollingLabelSink(this)
{
  SetObjectClass(_T("nuiScrollingLabel"));
  mTickCount = 0;
  mScrollLoop = ScrollLoop;
  mSmartScroll = smartScroll;
  mCentered = centered;
  mPPF = 1.0;
  mOldRect = nuiRect();
  mNeedSubStrings = true;
  mTextChanged = true;
  mCompleteLabelWidth = 0;
  
  mHPause = 2;
  mHWait = 2;
  mVPause = 2;
  mVWait = 0;
  
  mHFPS = DEFAULT_HORIZONTAL_FPS;
  mVFPS = DEFAULT_VERTICAL_FPS;
  
  if ((mScrollLoop == eScrollLoopHRightToLeft) || (mScrollLoop == eScrollLoopHLeftToRight) || (mScrollLoop == eScrollLoopHPingPong))
  {
    mOrientation = nuiHorizontal;
    mFPS = mHFPS;
  }
  else
  {
    mOrientation = nuiVertical;
    mFPS = mVFPS;
  }
  
  mCompleteString = pLabel->GetText();
  mpLabel = pLabel;
  AddChild(mpLabel);
  
  
  double period = 1.0 / mFPS;
  mpTimer = new nuiTimer(period); 
  mScrollingLabelSink.Connect(mpTimer->Tick, &nuiScrollingLabel::OnTimerChange);
  mpTimer->Start(true, true);
}



nuiScrollingLabel::~nuiScrollingLabel()
{
  delete mpTimer;
}



//////////////////////////////
//////////////////////////////
//
// SCROLLING MANAGEMENT
//
//////////////////////////////
//////////////////////////////


bool nuiScrollingLabel::SetRect(const nuiRect& rRect)
{
  // Check if rect changed or if it's only a tick event that requested to update layout
  bool rectChanged = !(mOldRect == rRect);
  // update mOldRect
  mOldRect = rRect;
  
  nuiWidget::SetRect(rRect);
  
  nuiSize width = rRect.GetWidth();
  nuiSize height = rRect.GetHeight();

    
  if (mOrientation == nuiHorizontal) // Scroll horizontally
  {
    
    nuiRect labelRect = mpLabel->GetIdealRect();
    nuiSize labelWidth = labelRect.GetWidth();
    nuiSize labelHeight = labelRect.GetHeight();
    
    // In nuihorizontal scrolling, the text never changes, so there's nothing more to do with completelabel (width) or (height), it will only be useful if switching to nuivertical at some point, to make sure those values have been updated
    if (mTextChanged)
    {
      mCompleteLabelWidth = labelWidth;
      mCompleteLabelHeight = labelHeight;
      mTextChanged = false;
    }
    
    nuiSize extraWidth = labelWidth - width;
    
    if ((extraWidth <= 0) && mSmartScroll) // no need to scroll
    {
      nuiRect r;
      if (mCentered)
      {
        r = nuiRect(nuiSize(0), (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
      }
      else
      {
        r = nuiRect(nuiSize(0), nuiSize(0), labelWidth, labelHeight);
      }
      r.RoundToNearest();
      mpLabel->SetLayout(r);
      
      // Stop Timer until text or layout changes
      mpTimer->Stop();
    }
    else if ((extraWidth < 2*mpLabel->GetHMargin())  && mSmartScroll) // the label would fit in if the margin were removed
    {
      nuiRect r;
      if (mCentered)
      {
        r = nuiRect(nuiSize(-extraWidth/2), (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
      }
      else
      {
        r = nuiRect(nuiSize(-extraWidth/2), nuiSize(0), labelWidth, labelHeight);
      }
      r.RoundToNearest();
      mpLabel->SetLayout(r);
      
      // Stop Timer until text or layout changes
      mpTimer->Stop();
    }
    else
    {
      // Check if timer is running and restart it if necessary
      if (!IsScrolling())
      {
        RestartScrolling();
      }

      if (mScrollLoop == eScrollLoopHRightToLeft)
      {
        nuiSize wait = mHWait*mFPS; // skip (wait) ticks before scrolling another line
        
        nuiSize progress = mTickCount % (int)((labelWidth + width + wait) / mPPF);
        progress *= mPPF;
        
        nuiRect r;
        if (progress <= (labelWidth + width))
        {
          if (mCentered)
          {
            r = nuiRect(width - progress, (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
          }
          else
          {
            r = nuiRect(width - progress, nuiSize(0), labelWidth, labelHeight);
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else
        {
          // just stay there for (wait) ticks
        }
      }
      else if (mScrollLoop == eScrollLoopHLeftToRight)
      {
        nuiSize wait = mHWait*mFPS; // skip (wait) ticks before scrolling another line
        
        nuiSize progress = mTickCount % (int)((labelWidth + width + wait) / mPPF);
        progress *= mPPF;
        nuiRect r;
        if (progress <= (labelWidth + width))
        {
          if (mCentered)
          {
            r = nuiRect(-labelWidth + progress, (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
          }
          else
          {
            r = nuiRect(-labelWidth + progress, nuiSize(0), labelWidth, labelHeight);
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else
        {
          // just stay there for (wait) ticks
        }
      }
      else if (mScrollLoop == eScrollLoopHPingPong)
      {
        nuiSize pause = mHPause*mFPS; // skip (pause) ticks between ping and pong
        nuiSize wait = mHWait*mFPS; // skip (wait) ticks before starting another ping pong
        
        if (extraWidth > 0) // label is bigger.
        {  
          if (extraWidth < 2*mpLabel->GetHMargin()) // the label would fit in if the margin were removed
          {
            nuiRect r;
            if (mCentered)
            {
              r = nuiRect(nuiSize(-extraWidth/2), (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
            }
            else
            {
              r = nuiRect(nuiSize(-extraWidth/2), nuiSize(0), labelWidth, labelHeight);
            }
            r.RoundToNearest();
            mpLabel->SetLayout(r);
            
            // Stop Timer until text or layout changes
            mpTimer->Stop();
          }
          else // it is definitely too big to fit in, scroll in order to show it all.
          {
            nuiSize progress = mTickCount % (int)((2*extraWidth + pause + wait) / mPPF);
            progress *= mPPF;
            
            if (progress <= extraWidth)
            {
              nuiRect r;
              if (mCentered)
              {
                r = nuiRect(-progress, (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
              }
              else
              {
                r = nuiRect(-progress, nuiSize(0), labelWidth, labelHeight);
              }
              
              r.RoundToNearest();
              mpLabel->SetLayout(r);
            }
            else if (progress <= extraWidth + wait)
            {
              // just stay there for (wait) ticks
            }
            else if (progress <= 2*extraWidth + wait)
            {
              nuiRect r;
              if (mCentered)
              {
                r = nuiRect(progress - wait - 2*extraWidth, (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
              }
              else
              {
                r = nuiRect(progress - wait - 2*extraWidth, nuiSize(0), labelWidth, labelHeight);
              }
              r.RoundToNearest();
              mpLabel->SetLayout(r);
            }
            else
            {
              // just stay there for (pause) ticks
            }
          }
        }
        else if (extraWidth >= -60) // It just fits, no need to scroll (extrawidth ==0) / Label is smaller (extrawidth <= 0), but not THAT much smaller, don't scroll.
        {
          nuiRect r;
          if (mCentered)
          {
            r = nuiRect(nuiSize(0), (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
          }
          else
          {
            r = nuiRect(nuiSize(0), nuiSize(0), labelWidth, labelHeight);
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
          
          // Stop Timer until text or layout changes
          mpTimer->Stop();
        }
        else // label is much smaller, scroll in order to spread over all the available space
        {
          extraWidth *= -1;
          nuiSize progress = mTickCount % (int)((2*extraWidth) / mPPF);
          progress *= mPPF;
          if (progress < extraWidth)
          {
            nuiRect r;
            if (mCentered)
            {
              r = nuiRect(progress, (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
            }
            else
            {
              r = nuiRect(progress, nuiSize(0), labelWidth, labelHeight);
            }
            r.RoundToNearest();
            mpLabel->SetLayout(r);
          }
          else
          {
            nuiRect r;
            if (mCentered)
            {
              r = nuiRect(2*extraWidth - progress, (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
            }
            else
            {
              r = nuiRect(2*extraWidth - progress, nuiSize(0), labelWidth, labelHeight);
            }
            r.RoundToNearest();
            mpLabel->SetLayout(r);
          }
        }
      }
    }
  }
  else if (mOrientation == nuiVertical)
  {  

    nuiRect labelRect = mpLabel->GetIdealRect();
    nuiSize labelWidth = labelRect.GetWidth();
    nuiSize labelHeight = labelRect.GetHeight();
    
    // if text changed, it's time to update the complete label values while its text is mCompleteString
    if (mTextChanged)
    {
      mCompleteLabelWidth = labelWidth;
      mCompleteLabelHeight = labelHeight;
      mTextChanged = false;
    }
       
    if ((mCompleteLabelWidth <= width) && mSmartScroll) // no need to scroll: display the complete string as is
    {   
      mpLabel->SetText(mCompleteString);
      nuiRect r;
      if (mCentered)
      {
        r = nuiRect((width - mCompleteLabelWidth) / 2, (height - mCompleteLabelHeight) / 2, mCompleteLabelWidth, mCompleteLabelHeight); // vertically & horizontally centered
      }
      else
      {
        r = nuiRect(nuiSize(0), nuiSize(0), mCompleteLabelWidth, mCompleteLabelHeight);
      }
      r.RoundToNearest();
      mpLabel->SetLayout(r);
      
      // Stop Timer until text changes
      mpTimer->Stop();
    }
    else // We'll have to scroll either because text is too long, or smart scroll is disabled. In either case, we'll have to use the substrings
    {          
      // if setrect was called because rect changed due to resizing or whatever else, we need to recreate new substrings
      // if (mTextChanged), mNeedSubStrings has already been set to true
      // otherwise, there's no need to create new substrings, the current ones apply
      if (rectChanged)
      {
        mNeedSubStrings = true;
      }
      
   
      // if necessary ( = not just a setrect called after a tick event) update the substrings
      if (mNeedSubStrings)
      {
        if (mCompleteLabelWidth <= width) // the whole label fits, but we want to scroll because (mSmartScroll == false). There's no need to calculate any substrings though
        {
          mSubStrings.clear();
          mNbSubStrings = 1;
          mSubStrings[0] = mCompleteString;
          mNeedSubStrings = false;
        } 
        else
        {
          nuiSize textWidth = width - 2* mpLabel->GetHMargin();
          CreateSubStrings(textWidth);   
        } 
        
        // (DEBUG PRINT)
//        NGL_OUT(_T("\nCreated %d substrings\n"), mNbSubStrings);
//        std::map<int, nglString>::iterator it = mSubStrings.begin();
//        std::map<int, nglString>::iterator end = mSubStrings.end();
//        while (it != end)
//        {
//          NGL_OUT(_T("%d %s\n"), it->first, it->second.GetChars());
//          ++it;
//        }
      }
      

      
      // Check if timer is running and restart it if necessary
      if (!IsScrolling())
      {
        RestartScrolling();
      }
      
      if (mScrollLoop == eScrollLoopVBottomToTop)
      {
        nuiSize pause = mVPause*mFPS; // skip (pause) ticks with the label vertically centered
        nuiSize wait = mVWait*mFPS; // skip (wait)ticks before scrolling the next line
        
        int subStringsProgress = mTickCount / (int)((labelHeight + height + pause + wait) / mPPF); // how many substrings have been displayed
        subStringsProgress = subStringsProgress % mNbSubStrings; // we're looping on the substrings, so which sub string are we displaying right now ?
        
        mpLabel->SetText(mSubStrings[subStringsProgress]);
        
        labelRect = mpLabel->GetIdealRect();
        labelWidth = labelRect.GetWidth();
        labelHeight = labelRect.GetHeight(); // is the label height depending on what characters are displayed ? If so, this is required, otherwise, we already konw this value
        
        nuiSize progress = mTickCount % (int)((labelHeight + height + pause + wait) / mPPF);
        progress *= mPPF;
        
        nuiRect r;
        if (progress <= ((height + labelHeight) / 2))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, height - progress, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), height - progress, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else if (progress <= ((height + labelHeight) / 2) + pause)
        {
          // just stay there for (pause) ticks
        }
        else if (progress <= (height + labelHeight + pause))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, height - progress + pause, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), height - progress + pause, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else
        {
          // just stay there for (wait) ticks
        }
      }
      else if (mScrollLoop == eScrollLoopVTopToBottom)
      {
        nuiSize pause = mVPause*mFPS; // skip (pause) ticks with the label vertically centered
        nuiSize wait = mVWait*mFPS; // skip (wait)ticks before scrolling another line
        
        int subStringsProgress = mTickCount / (int)((labelHeight + height + pause + wait) / mPPF); // how many substrings have been displayed
        subStringsProgress = subStringsProgress % mNbSubStrings; // we're looping on the substrings, so which sub string are we displaying right now ?
        
        mpLabel->SetText(mSubStrings[subStringsProgress]);
        
        labelRect = mpLabel->GetIdealRect();
        labelWidth = labelRect.GetWidth();
        labelHeight = labelRect.GetHeight(); // is the label height depending on what characters are displayed ? If so, this is required, otherwise, we already konw this value
        
        nuiSize progress = mTickCount % (int)((labelHeight + height + pause + wait) / mPPF);
        progress *= mPPF;
        
        nuiRect r;
        if (progress <= ((height + labelHeight) / 2))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, -labelHeight + progress, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), -labelHeight + progress / 2, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else if (progress <= ((height + labelHeight) / 2) + pause)
        {
          // just stay there for (pause) ticks
        }
        else if (progress <= (height + labelHeight + pause))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, progress - labelHeight - pause, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), progress - labelHeight - pause, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else
        {
          // just stay there for (wait) ticks
        }
      }
      else if (mScrollLoop == eScrollLoopVBounceUp)
      {
        nuiSize pause = mVPause*mFPS; // skip (pause) ticks with the label vertically centered
        nuiSize wait = mVWait*mFPS; // skip (wait)ticks before scrolling another line
        
        int subStringsProgress = mTickCount / (int)((labelHeight + height + pause + wait) / mPPF); // how many substrings have been displayed
        subStringsProgress = subStringsProgress % mNbSubStrings; // we're looping on the substrings, so which sub string are we displaying right now ?
        
        mpLabel->SetText(mSubStrings[subStringsProgress]);
        
        labelRect = mpLabel->GetIdealRect();
        labelWidth = labelRect.GetWidth();
        labelHeight = labelRect.GetHeight(); // is the label height depending on what characters are displayed ? If so, this is required, otherwise, we already konw this value
        
        nuiSize progress = mTickCount % (int)((labelHeight + height + pause + wait) / mPPF);
        progress *= mPPF;
        
        nuiRect r;
        if (progress <= ((height + labelHeight) / 2))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, -labelHeight + progress, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), -labelHeight + progress / 2, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else if (progress <= ((height + labelHeight) / 2) + pause)
        {
          // just stay there for (pause) ticks
        }
        else if (progress <= (height + labelHeight + pause))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, height - progress + pause, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), height - progress + pause, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else
        {
          // just stay there for (wait) ticks
        }
      }
      else if (mScrollLoop == eScrollLoopVBounceDown)
      {
        nuiSize pause = mVPause*mFPS; // skip (pause) ticks with the label vertically centered
        nuiSize wait = mVWait*mFPS; // skip (wait)ticks before scrolling another line
        
        int subStringsProgress = mTickCount / (int)((labelHeight + height + pause + wait) / mPPF); // how many substrings have been displayed
        subStringsProgress = subStringsProgress % mNbSubStrings; // we're looping on the substrings, so which sub string are we displaying right now ?
        
        mpLabel->SetText(mSubStrings[subStringsProgress]);
        
        labelRect = mpLabel->GetIdealRect();
        labelWidth = labelRect.GetWidth();
        labelHeight = labelRect.GetHeight(); // is the label height depending on what characters are displayed ? If so, this is required, otherwise, we already konw this value
        
        nuiSize progress = mTickCount % (int)((labelHeight + height + pause + wait) / mPPF);
        progress *= mPPF;
        
        nuiRect r;
        if (progress <= ((height + labelHeight) / 2))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, height - progress, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), height - progress, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else if (progress <= ((height + labelHeight) / 2) + pause)
        {
          // just stay there for (pause) ticks
        }
        else if (progress <= (height + labelHeight + pause))
        {
          if (mCentered)
          {
            r = nuiRect((width - labelWidth) / 2, progress - labelHeight - pause, labelWidth, labelHeight); // horizontally centered
          }
          else
          {
            r = nuiRect(nuiSize(0), progress - labelHeight - pause, labelWidth, labelHeight); // horizontally centered
          }
          r.RoundToNearest();
          mpLabel->SetLayout(r);
        }
        else
        {
          // just stay there for (wait) ticks
        }
      }
    }
  }
  return true;
}


void nuiScrollingLabel::RestartScrolling()
{
  //mpTimer->Stop();
  mpTimer->Start(true, true);
  mTickCount = 0;
}

void nuiScrollingLabel::StopScrolling()
{
  mpTimer->Stop();
  
  nuiSize height = GetRect().GetHeight();
  nuiSize width = GetRect().GetWidth();
  
  nuiRect labelRect = mpLabel->GetIdealRect();
  nuiSize labelWidth = labelRect.GetWidth();
  nuiSize labelHeight = labelRect.GetHeight();
  
  if (mOrientation == nuiHorizontal)
  {
    nuiRect r;
    if (mCentered)
    {
      r = nuiRect(nuiSize(0), (height - labelHeight) / 2, labelWidth, labelHeight); // vertically centered
    }
    else
    {
      r = nuiRect(nuiSize(0), nuiSize(0), labelWidth, labelHeight);
    }
    r.RoundToNearest();
    mpLabel->SetLayout(r);
  }
  else
  {
    nuiRect r = nuiRect((width - labelWidth) / 2, nuiSize(0), labelWidth, labelHeight); // horizontally centered
    r.RoundToNearest();
    mpLabel->SetLayout(r);
  }
}

void nuiScrollingLabel::UnpauseScrolling()
{
  mpTimer->Start(true, false);
}

void nuiScrollingLabel::PauseScrolling()
{
  mpTimer->Stop();
}

bool nuiScrollingLabel::IsScrolling() const
{
  return mpTimer->IsRunning();
}

void nuiScrollingLabel::OnTimerChange(const nuiEvent& rEvent)
{
  mTickCount++;
  UpdateLayout();

  rEvent.Cancel();
}



//////////////////////////////
//////////////////////////////
//
// LABEL MANAGEMENT
//
//////////////////////////////
//////////////////////////////


nuiLabel* nuiScrollingLabel::GetLabel()
{
  return mpLabel;
}

void nuiScrollingLabel::SetLabel(nuiLabel* pLabel)
{
  mCompleteString = pLabel->GetText();
  mpLabel = pLabel;
  mNeedSubStrings = true;
  mTextChanged = true;
  RestartScrolling();
}

void nuiScrollingLabel::SetText(const nglString& rText)
{
  mCompleteString = rText;
  mpLabel->SetText(rText);
  mNeedSubStrings = true;
  mTextChanged = true;
  RestartScrolling();
}


// TODO: add more separation characters than just whitespaces (e.g. '-' ',' '.')

void nuiScrollingLabel::CreateSubStrings(nuiSize width)
{  
  // clear the previous substrings map
  mSubStrings.clear();
  mNbSubStrings = 0;
  
    
  bool allSubStringsComplete = false; // everything done
  nglString firstString = mCompleteString; // contains the text we're splitting
  firstString.Trim(); // remove leading and trailing whitespaces
  nglString secondString = nglString::Empty; // contains the text we're splitting
  nuiTextLayout* pFirstStringLayout = new nuiTextLayout(mpLabel->GetFont(), nuiHorizontal);

  
  
  // (DEBUG PRINT)
  // NGL_OUT(_T("Splitting %s\n"), firstString.GetChars());
  
  
  while (!allSubStringsComplete) // there is at least one substring to create. Create it and repeat on second string, until the whole text has been handled
  {
    delete pFirstStringLayout;
    pFirstStringLayout = new nuiTextLayout(mpLabel->GetFont(), nuiHorizontal);
    int nbGlyphs = pFirstStringLayout->Layout(firstString);
    
    nuiSize stringWidth = pFirstStringLayout->GetRect().GetWidth();
    
    // (DEBUG PRINT)
    // NGL_OUT(_T("%f / %f "), stringWidth, width);
    
    if (nbGlyphs == 0) // no more glyphs to handle (firststring is either empty, or not a single character fits in), we're done
    {
      if (mNbSubStrings == 0) // No glyphs were found at all, return a single empty string in the substrings map.
      {
        mSubStrings[0] = nglString::Empty;
        mNbSubStrings = 1;
      }
      
      // (DEBUG PRINT)
      // NGL_OUT(_T("Complete\n"));
      allSubStringsComplete = true;
    }
    else if (pFirstStringLayout->GetRect().GetWidth() <= width) // if the first string fits in entirely, we're done with it, just fine tune the split, and then update the map and proceed with second string
    {
      
      
      // (DEBUG PRINT)
      // NGL_OUT(_T(":%d: '%s' \t\t // remaining: '%s'\n"), mNbSubStrings, firstString.GetChars(), secondString.GetChars());
      
      // First search for a better position to cut the string, and remove unnecessary whitespaces
      
      if (secondString.IsEmpty())
      {
        // nothing to do here
      }
      else if (secondString.GetChar(0) == _T(' ')) // split position is ok, just delete any useless whitespaces around it
      {
        firstString.TrimRight();
        secondString.TrimLeft();
      }
      else
      {
        // find the closest whitespace from the end of first string
        int pos = firstString.GetLength() - 1;
        
        while (firstString.GetChar(pos) != _T(' '))
        {
          pos --;
          if (pos < 0)
          {
            break;
          }
        }
        
        // update the strings with the new split position
        
        if (pos <0) // no whitespace found, just split it where we were supposed to
        {
        }
        else // there is a whitespace in firststring (and not the first character since we trimmed it before, either at the beginning, or in the previous step as a second string
        {
          secondString = firstString.GetRight(firstString.GetLength() - pos) + secondString;
          firstString = firstString.GetLeft(pos);
          
          firstString.TrimRight();
          secondString.TrimLeft();
        }
      }
      
      
      // (DEBUG PRINT)
      // NGL_OUT(_T("%f / %f "), stringWidth, width);
      // NGL_OUT(_T("--> '%s' \t\t // remaining: '%s'\n"), firstString.GetChars(), secondString.GetChars());
      
      // update the subStrings map
      mSubStrings[mNbSubStrings] = firstString;
      mNbSubStrings++;
      
      // repeat on second string
      firstString = secondString;
      secondString = nglString::Empty;
    }
    else // there are more glyphs to process
    {
      
      // (DEBUG PRINT)
      // NGL_OUT(_T("... '%s' \t\t // remaining: '%s'\n"), firstString.GetChars(), secondString.GetChars());
      
      // remove last glyph
      const nuiTextGlyph* pGlyphLayout = pFirstStringLayout->GetGlyph(nbGlyphs -1);
      int pos = (pGlyphLayout->mCluster);
      
      // update strings to repeat
      secondString = firstString.Extract(pos, firstString.GetLength() - pos) + secondString;
      firstString = firstString.Extract(0, pos);
    }
  }
  mNeedSubStrings = false;
}
  

//////////////////////////////
//////////////////////////////
//
// GETTERS / SETTERS
//
//////////////////////////////
//////////////////////////////


void nuiScrollingLabel::SetHFPS(float hfps)
{
  mHFPS = hfps;
  if (mOrientation == nuiHorizontal)
  {
    SetFPS(mHFPS);
  }
}

float nuiScrollingLabel::GetHFPS() const
{
  return mHFPS;
}

void nuiScrollingLabel::SetVFPS(float vfps)
{
  mVFPS = vfps;
  if (mOrientation == nuiVertical)
  {
    SetFPS(mVFPS);
  }
}

float nuiScrollingLabel::GetVFPS() const
{
  return mVFPS;
}

void nuiScrollingLabel::SlowDown(float factor)
{
  if ((factor >= 1) || (factor <= 0))
  {
    return;
  }
  
  SetFPS(mFPS*factor);
}

void nuiScrollingLabel::SpeedUp(float factor)
{
  if (factor <= 1)
  {
    return;
  }
  
  SetFPS(mFPS*factor);
}


// private - only used internally
void nuiScrollingLabel::SetFPS(float fps)
{
  mFPS = fps;

  mpTimer->Stop();
  
  // update timer period
  double period = 1.0 / mFPS;
  mpTimer->SetPeriod(period);
  
  // restart the timer
  RestartScrolling();
}

float nuiScrollingLabel::GetFPS() const
{
  return mFPS;
}

void nuiScrollingLabel::SetPPF(float ppf)
{
  mPPF = ppf;
  
  // restart the timer
  RestartScrolling();
}

float nuiScrollingLabel::GetPPF() const
{
  return mPPF;
}

void nuiScrollingLabel::SetCentered(bool centered)
{
  mCentered = centered;
}

bool nuiScrollingLabel::IsCentered() const
{
  return mCentered;
}

void nuiScrollingLabel::SetScrollLoop(nuiScrollLoop scrollLoop)
{
  mScrollLoop = scrollLoop;
  
  nuiOrientation oldOrientation = mOrientation;
  
  // update orientation & fps
  if ((mScrollLoop == eScrollLoopHRightToLeft) || (mScrollLoop == eScrollLoopHLeftToRight) || (mScrollLoop == eScrollLoopHPingPong))
  {
    mOrientation = nuiHorizontal;
    
    // if switching from vertical to horizontal, the substrings are not necessary anymore, the label is the complete string
    if (oldOrientation == nuiVertical)
    {
      mpLabel->SetText(mCompleteString);
      mNeedSubStrings = false;
    }
    
    // update FPS to the orientation's value, and restart the timer
    SetFPS(mHFPS);
  }
  else
  {
    mOrientation = nuiVertical;
    
    // if switching from horizontal to vertical, creating substrings might be necessary
    if (oldOrientation == nuiHorizontal)
    {
      mNeedSubStrings = true;
    }
    
    // update FPS to the orientation's value, and restart the timer
    SetFPS(mVFPS);
  }
}

nuiScrollLoop nuiScrollingLabel::GetScrollLoop()
{
  return mScrollLoop;
}

void nuiScrollingLabel::SetSmartScroll(bool smartScroll)
{
  mSmartScroll = smartScroll;
  
  // restart timer
  RestartScrolling();
}

bool nuiScrollingLabel::GetSmartScroll() const
{
  return mSmartScroll;
}

void nuiScrollingLabel::SetHPause(float hPause)
{
  mHPause = hPause;
}

void nuiScrollingLabel::SetHWait(float hWait)
{
  mHPause = hWait;
}

void nuiScrollingLabel::SetVPause(float vPause)
{
  mVPause = vPause;
}

void nuiScrollingLabel::SetVWait(float vWait)
{
  mVWait = vWait;
}

float nuiScrollingLabel::GetHPause() const
{
  return mHPause;
}

float nuiScrollingLabel::GetHWait() const
{
  return mHWait;
}

float nuiScrollingLabel::GetVPause() const
{
  return mVPause;
}

float nuiScrollingLabel::GetVWait() const
{
  return mVWait;
}
