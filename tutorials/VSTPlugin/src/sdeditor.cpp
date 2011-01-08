//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:28 $
//
// Category     : VST 2.x SDK Samples
// Filename     : sdeditor.cpp
// Created by   : Steinberg Media Technologies
// Description  : Simple Surround Delay plugin with Editor using VSTGUI
//
// � 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "nui.h"
#include "nuiVBox.h"
#include "nuiClampedValueAttributeEditor.h"

#ifndef __sdeditor__
#include "sdeditor.h"
#endif

#ifndef __adelay__
#include "adelay.h"
#endif

#include <stdio.h>

#ifdef WIN32
extern void* hInstance;
#endif

//-----------------------------------------------------------------------------
// prototype string convert float -> percent

nuiAttributeEditor* CreateEditor(void* pObj, nuiAttribute<float>* pAttrib)
{
  return new nuiClampedValueAttributeEditor<float>(nuiAttrib<float>(pObj, pAttrib), nuiRange(pAttrib->Get(pObj), 0.0, 1.0, 0.05, 0.1));
}

void DelayFormater(nglString& rString, float value)
{
  rString.CFormat(_T("%2.2fs"), value);
}

void FeedbackFormater(nglString& rString, float value)
{
  rString.CFormat(_T("%3.1f%%"), 100.0f * value);
}

void OutFormater(nglString& rString, float value)
{
  rString.CFormat(_T("%3.1f%%"), 100.0f * value);
}

//-----------------------------------------------------------------------------
// SDEditor class implementation
//-----------------------------------------------------------------------------
SDEditor::SDEditor (AudioEffect *effect)
 : AEffEditor (effect), mpWin(NULL), mLock(false)
{
  nuiInit(NULL, NULL);
  SetObjectClass(_T("SDEditor"));

  nuiAttribute<float>* pDelayAttrib = new nuiValueAttribute<float>(_T("Delay"), effect->getParameter(kDelay));
  pDelayAttrib->SetEditor(CreateEditor);
  pDelayAttrib->SetFormater(DelayFormater);
  nuiAttrib<float> delay(this, pDelayAttrib);
  mSlotSink.Connect(delay.GetChangedSignal(), nuiMakeDelegate(this, &SDEditor::SetDelay));
  AddAttribute(pDelayAttrib);

  nuiAttribute<float>* pFeedbackAttrib = new nuiValueAttribute<float>(_T("Feedback"), effect->getParameter(kFeedBack));
  pFeedbackAttrib->SetEditor(CreateEditor);
  pFeedbackAttrib->SetFormater(FeedbackFormater);
  nuiAttrib<float> feedback(this, pFeedbackAttrib);
  mSlotSink.Connect(feedback.GetChangedSignal(), nuiMakeDelegate(this, &SDEditor::SetFeedback));
  AddAttribute(pFeedbackAttrib);

  nuiAttribute<float>* pOutAttrib = new nuiValueAttribute<float>(_T("Out"), effect->getParameter(kOut));
  pOutAttrib->SetEditor(CreateEditor);
  pOutAttrib->SetFormater(OutFormater);
  nuiAttrib<float> out(this, pOutAttrib);
  mSlotSink.Connect(out.GetChangedSignal(), nuiMakeDelegate(this, &SDEditor::SetOut));
  AddAttribute(pOutAttrib);

  mMainRect.top = 0;
  mMainRect.left = 0;
  mMainRect.bottom = 240;
  mMainRect.right = 320;
  effect->setEditor(this);
}

//-----------------------------------------------------------------------------
SDEditor::~SDEditor ()
{
  nuiUninit();
}

//-----------------------------------------------------------------------------
bool SDEditor::open (void *ptr)
{
	// !!! always call this !!!
	AEffEditor::open (ptr);

  nuiContextInfo ctxinfo;
  nglWindowInfo wininfo;
#ifdef WIN32
  wininfo.OSInfo.Parent = (HWND)ptr;
#else
  wininfo.OSInfo.Parent = (WindowRef)ptr;
#endif
  wininfo.Width = mMainRect.right;
  wininfo.Height = mMainRect.bottom;

  mpWin = new nuiMainWindow(ctxinfo, wininfo);
  mpWin->SetState(nglWindow::eShow);

  nuiVBox* pBox = new nuiVBox();
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  nuiImage* pBg = new nuiImage(_T("rsrc:/decorations/image.png"));
  pBg->SetAlpha(.3f);
  pBg->SetPosition(nuiCenter);
  nuiRect r(0, 0, mMainRect.right, mMainRect.bottom);
  r.Scale(0.1f, 0.1f);
  pBg->SetLayout(r);
  pBg->SetLayoutAnimationDuration(3.0f);
  pBg->SetLayoutAnimationEasing(nuiEasingElasticOut<500>);

  mpWin->AddChild(pBg);
  mpWin->AddChild(pBox);
  nuiLabel* pLabel = new nuiLabel(_T("Simple NUI VST Plugin"));
  pLabel->SetFont(nuiFont::GetFont(24));
  pLabel->SetBorder(0, 4);
  pBox->AddCell(pLabel, nuiCenter);
  nuiAttrib<float> delay(GetAttribute(_T("Delay")));
  nuiAttrib<float> feedback(GetAttribute(_T("Feedback")));
  nuiAttrib<float> out(GetAttribute(_T("Out")));
  pBox->AddCell(delay.GetEditor());
  pBox->SetCellExpand(1, nuiExpandShrinkAndGrow);
  pBox->AddCell(feedback.GetEditor());
  pBox->SetCellExpand(2, nuiExpandShrinkAndGrow);
  pBox->AddCell(out.GetEditor());
  pBox->SetCellExpand(3, nuiExpandShrinkAndGrow);

	return true;
}

//-----------------------------------------------------------------------------
void SDEditor::close ()
{
  delete mpWin;
  mpWin = NULL;
}

//-----------------------------------------------------------------------------
void SDEditor::setParameter (VstInt32 index, float value)
{
  if (mLock)
    return;

	// called from ADelayEdit
	switch (index)
	{
		case kDelay:
      {
        nuiAttrib<float> attrib(GetAttribute(_T("Delay")));
        attrib.Set(value);
      }
// 			if (delayFader)
// 				delayFader->setValue (effect->getParameter (index));
// 			if (delayDisplay)
// 				delayDisplay->setValue (effect->getParameter (index));
			break;

		case kFeedBack:
// 			if (feedbackFader)
// 				feedbackFader->setValue (effect->getParameter (index));
// 			if (feedbackDisplay)
// 				feedbackDisplay->setValue (effect->getParameter (index));
      {
        nuiAttrib<float> attrib(GetAttribute(_T("Feedback")));
        attrib.Set(value);
      }
			break;

		case kOut:
// 			if (volumeFader)
// 				volumeFader->setValue (effect->getParameter (index));
// 			if (volumeDisplay)
// 				volumeDisplay->setValue (effect->getParameter (index));
      {
        nuiAttrib<float> attrib(GetAttribute(_T("Out")));
        attrib.Set(value);
      }
			break;
	}
}

//-----------------------------------------------------------------------------
// void SDEditor::valueChanged (CDrawContext* context, CControl* control)
// {
// 	long tag = control->getTag ();
// 	switch (tag)
// 	{
// 		case kDelay:
// 		case kFeedBack:
// 		case kOut:
// 			effect->setParameterAutomated (tag, control->getValue ());
// // 			control->setDirty ();
// 		break;
// 	}
// }

void SDEditor::SetDelay(float set)
{
  LockEdit();
  effect->setParameterAutomated(kDelay, set);
  UnlockEdit();
}

void SDEditor::SetFeedback(float set)
{
  LockEdit();
  effect->setParameterAutomated(kFeedBack, set);
  UnlockEdit();
}

void SDEditor::SetOut(float set)
{
  LockEdit();
  effect->setParameterAutomated(kOut, set);
  UnlockEdit();
}

bool SDEditor::getRect(ERect** rect)
{
  *rect = &mMainRect;
  return false;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SDEditor::LockEdit()
{
  mLock = true;
}

void SDEditor::UnlockEdit()
{
  mLock = false;
}
