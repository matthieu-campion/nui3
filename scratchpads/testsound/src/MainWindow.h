/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#ifndef __MainWindow_h__
#define __MainWindow_h__

#include "nuiMainWindow.h"
#include "nglRingBuffer.h"
#include "BiQuad.h"

class nuiAudioDevice;

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();

  void OnClose();

  void ProcessAudioInput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames);
  void ProcessAudioOutput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames);
protected:
  nuiEventSink<MainWindow> mSink;
  
  nuiAudioDevice* mpInAudioDevice;
  nuiAudioDevice* mpOutAudioDevice;
  std::vector<std::vector<float> > mInput;
  std::vector<std::vector<float> > mOutput;
  
  nglRingBuffer* mpRingBuffer;
  BiQuad mFilter;

};

#endif//__MainWindow_h__
