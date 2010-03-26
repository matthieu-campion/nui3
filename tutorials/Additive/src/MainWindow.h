/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"


class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  bool Draw(nuiDrawContext* pContext);
  void OnClose();

  bool MouseClicked(const nglMouseInfo& rInfo);
  bool MouseUnclicked(const nglMouseInfo& rInfo);
  bool MouseMoved(const nglMouseInfo& rInfo);
  
protected:
  float mMouseX;
  float mMouseY;
  bool mClicked;
  
  // Blur:
  float mMean;
  float mDeviation;
  float mVariance;
  float mAmount;

  float GetMean() const;
  float GetAmount() const;
  float GetDeviation() const;
  float GetVariance() const;
  
  void SetMean(float v);
  void SetAmount(float v);
  void SetDeviation(float v);
  void SetVariance(float v);
  
  nuiSurface* mpSurface1;
  nuiTexture* mpTexture1;
  nuiSurface* mpSurface2;
  nuiTexture* mpTexture2;
  nuiSurface* mpSurface3;
  nuiTexture* mpTexture3;

  void UpdateSurfaces();
  
  bool LoadCSS(const nglPath& rPath);
  
  nuiEventSink<MainWindow> mEventSink;
};

