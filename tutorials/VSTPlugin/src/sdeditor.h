//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:28 $
//
// Category     : VST 2.x SDK Samples
// Filename     : sdeditor.h
// Created by   : Steinberg Media Technologies
// Description  : Simple Surround Delay plugin with Editor using VSTGUI
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __sdeditor__
#define __sdeditor__

#include "nui.h"
#include "AEffEditor.h"

//-----------------------------------------------------------------------------
class SDEditor : public nuiObject, public AEffEditor
{
public:
	SDEditor (AudioEffect* effect);
	virtual ~SDEditor ();

public:
	virtual bool open (void* ptr);
	virtual void close ();

	virtual void setParameter (VstInt32 index, float value);
// 	virtual void valueChanged (CDrawContext* context, CControl* control);

private:
  virtual bool getRect (ERect** rect);
  nuiMainWindow* mpWin;
  ERect mMainRect;

  void SetDelay(float set);
  void SetFeedback(float set);
  void SetOut(float set);
  
  nuiSlotsSink mSlotSink;

  bool mLock;
  void LockEdit();
  void UnlockEdit();
};

#endif
