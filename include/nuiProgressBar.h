/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiProgressBar_h__
#define __nuiProgressBar_h__

//#include "nui.h"
#include "nuiWidget.h"
#include "nuiContainer.h"

class NUI_API nuiProgressBar : public nuiWidget
{
public:
  nuiProgressBar(float Progress);
  nuiProgressBar();
  virtual ~nuiProgressBar();

  virtual nuiRect CalcIdealSize();
  virtual bool Draw(nuiDrawContext* pContext);

  void SetProgress(float Progress); // use percentage value between 0 and 100
  float GetProgress() const;

  void SetGlowStrength(float Strength);
  float GetGlowStrength() const;
  
  bool IsEndless() const;
  void SetEndless(bool endless);

protected:
  
  void InitAttributes();
  
  void SetFGColor(const nuiColor& Color);
  const nuiColor& GetFGColor() const;
  void SetBGColor(const nuiColor& Color);
  const nuiColor& GetBGColor() const;  
  
  void SetGlow(float value);
  float GetGlow() const;
  
  float mProgress;
  float mAlphaIncr;
  float mAlphaAnim;
  nglTime mLastRender;
  float mGlowStrength;
  
  bool mEndless; //false if this is a real progress bar, true if it is only an animated aiting placeholder
  float mEndlessAnim;
  float mEndlessIncr;
  
  nuiColor mFGColor, mBGColor;
};


#endif // #ifndef __nuiProgressBar_h__

