/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiSWF.h
\brief A widget to display ShockWave Flash movie
*/

#ifndef __nuiSWF_h__
#define __nuiSWF_h__

#include "nuiWidget.h"
#include "nuiContainer.h"
#include "nuiDrawContext.h"
#include "nuiUserArea.h"

namespace gameswf
{
  struct movie_definition;
  struct render_handler;
  struct root;
  struct player;
};


class NUI_API nuiSWF : public nuiWidget
{
public:
  nuiSWF();
  virtual bool Load(const nuiXMLNode* pNode);
  virtual ~nuiSWF();

  bool Load (const nglPath& rPath);
  /*!< Load a movie file
  \param rPath path to a SWF file (.swf extension)
  \return true if the file was succesfully loaded
  */

  virtual bool KeyDown (const nglKeyEvent& rEvent);
  virtual bool KeyUp   (const nglKeyEvent& rEvent);

  virtual bool MouseClicked   (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved (nuiSize X, nuiSize Y);

  virtual nuiRect CalcIdealSize();

  virtual bool Draw(nuiDrawContext* pContext);

  void Play();
  void Loop(bool set);
  void Stop();
  void SetFrame(int Frame);
  void SetFrame(const nglString& rName);
  int GetFrame() const;
  int GetFrameCount() const;
  void SetFrameRate(float fps);
  bool IsPlaying() const;

  void SetInterceptMouse(bool set); // If true all mouse events received will be abducted by the flash movie. Otherwise the movie will receive them and then they will be passed to the parent widget. (default = false).

protected:
  gameswf::movie_definition*  mpMovie;
  gameswf::render_handler*    mpRenderHandler;
  gameswf::root*   mpMovieInterface;
  gameswf::player*   mpPlayer;

  int mButtons;
  float mFPS;
  double mLastTime;
  int mWidth;
  int mHeight;
  int mFrames;
  int mTags;
  bool mInterceptMouse;
  bool mLoop;

  static uint32 mRefs;
};

#endif
