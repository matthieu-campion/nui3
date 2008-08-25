/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWidgetInspector_h__
#define __nuiWidgetInspector_h__

#include "nuiTreeView.h"
#include "nuiComposite.h"

class nuiVBox;
class nuiGrid;
class nuiWidgetProxy;
class nuiFolderPane;

class NUI_API nuiWidgetInspectorNode : public nuiTreeNode
{
public:
  nuiWidgetInspectorNode(nuiWidget* pTarget);
  virtual ~nuiWidgetInspectorNode();

  virtual void Open(bool Opened);
  virtual bool IsEmpty() const;
  nuiWidget* GetTarget();
private:
  nuiEventSink<nuiWidgetInspectorNode> mInspectorNodeSink;
  bool UpdateInfos(const nuiEvent& rEvent);
  nuiWidget* mpTarget;
  nuiWidgetInspectorNode* mpTree;
};


class NUI_API nuiWidgetInfo : public nuiComposite
{
public:
  nuiWidgetInfo(nuiWidget* pTarget);
  virtual ~nuiWidgetInfo();

  void SetTarget(nuiWidget* pWidget);
  void RebuildInfo();
protected:
  bool OnTimerTick(const nuiEvent& rEvent);
  bool OnDebugUpdate(const nuiEvent& rEvent);
  bool OnTrashed(const nuiEvent& rEvent);
  nuiEventSink<nuiWidgetInfo> mWISink;
  nuiWidget* mpTarget;
  nuiTimer* mpTimer;
  bool mNeedUpdate;
  
  nuiSimpleContainer* mpDummy;
  nuiVBox* mpInfos;

  nuiLabel* mpName;
  nuiLabel* mpClass;
  nuiLabel* mpCType;
#ifndef __GNUC__
  nuiLabel* mpCRawType;
#endif
  nuiLabel* mpIdealRect;
  nuiLabel* mpUserRect;
  nuiLabel* mpHasUserSize;
  nuiLabel* mpHasUserPos;
  nuiLabel* mpRect;
  nuiLabel* mpNeedLayout;
  nuiLabel* mpNeedRender;
  nuiLabel* mpNeedSelfRedraw;
  nuiLabel* mpNeedIdealRectCalc;
  nuiLabel* mpGlobalRect;
  nuiGrid* mpMatrix;
  nuiGrid* mpPropertyGrid;
  nuiGrid* mpAttributeGrid;
  nuiWidgetProxy* mpProxy;
  nuiFolderPane* mpMetaPainterPane;
  void BuildInfo();
};

class NUI_API nuiWidgetInspector : public nuiComposite
{
public:
  nuiWidgetInspector(nuiWidget* pTarget);
  virtual ~nuiWidgetInspector();

protected:
  nuiEventSink<nuiWidgetInspector> mInspectorSink;
  nuiWidget* mpTarget;
  nuiTreeNode* mpTree;
  nuiWidgetInfo* mpWidgetInfo;
  bool OnSelectionChanged(const nuiEvent& rEvent);
};

#endif //ifndef __nuiWidgetInspector_h__
