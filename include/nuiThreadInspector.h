/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiScrollView.h"
#include "nuiVBox.h"
#include "nglThreadChecker.h"
#include "nuiFormatedLabel.h"

class nuiThreadInspector : public nuiSimpleContainer
{
public:
  nuiThreadInspector();
  virtual ~nuiThreadInspector();


private:

  void InitAttributes();
  void Build();

  void BuildThreadTree();

  nuiTreeNode* CreateNodeFromThread(nglThread::ID ID);
  nuiTreeNode* CreateNodeFromLock(const nglThreadState& rState);
  void Update(const nuiEvent& rEvent);
  void Clean(nuiTreeNode* pParent, const std::map<nglThread::ID, std::list<nglThreadState> >& states);
  
  // attributes
  bool IsRunning();
  void SetRunning(bool run);

  bool IsUpdating();
  void SetUpdating(bool update);
  
  const nuiRange& GetFrequency();
  void SetFrequency(const nuiRange& rFreq);
  
  bool GetShowLongLock();
  void SetShowLongLock(bool show);

  const nuiRange& GetLongLockTreshold();
  void SetLongLockTreshold(const nuiRange& rTreshold);
  
  
  
  nuiTreeNode* GetNodeFromThread(nglThread::ID ID, nuiTreeNode* pParent);
  nuiTreeNode* GetNodeFromLock(nglLock* pLock, nuiTreeNode* pParent);
  nuiLabel* GetLabelFromNode(nuiTreeNode* pNode);
  void SetNodeFromLock(nuiTreeNode* pLockNode, const nglThreadState& rState);

  nuiScrollView* mpScrollView;
  nuiVBox* mpMainBox;
  nuiVBox* mpTreeBox;
  
  nuiTreeNode* mpTree;
  
  nuiFormatedLabel* mpUpdateCount;
  bool mRunning;
  bool mUpdating;
  nuiRange mFrequency;
  double mCurrentTime;
  bool mShowLongLock;
  nuiRange mLongLockTreshold;
  
  
  nuiTimer* mpTimer;
  nuiEventSink<nuiThreadInspector> mEventSink;
};
