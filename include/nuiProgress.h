/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiProgress_h__
#define __nuiProgress_h__

//#include "nui.h"
#include "nuiWidget.h"
#include "nuiContainer.h"

class NUI_API nuiProgress : public nuiWidget
{
public:
  nuiProgress(float Progress);
  nuiProgress();
  virtual bool Load(const nuiXMLNode* pNode);
  virtual ~nuiProgress();

  virtual nuiRect CalcIdealSize();
  virtual bool Draw(nuiDrawContext* pContext);

  void SetProgress(float Progress); // use percentage value between 0 and 100
  float GetProgress() const;

  void SetGlowStrength(float Strength);
  float GetGlowStrength() const;
  
  bool IsEndless() const;
  void SetEndless(bool endless);

protected:
  float mProgress;
  float mAlphaIncr;
  float mAlphaAnim;
  nglTime mLastRender;
  float mGlowStrength;
  
  bool mEndless; //false if this is a real progress bar, true if it is only an animated aiting placeholder
  float mEndlessAnim;
  float mEndlessIncr;
};


#endif // #ifndef __nuiProgress_h__

