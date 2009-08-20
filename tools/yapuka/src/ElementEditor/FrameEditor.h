/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"
#include "nuiFrame.h"
#include "nuiFrameView.h"
#include "ElementEditor/ElementDesc.h"
#include "nuiMouseEvent.h"
#include "Tools/ElementInspector.h"




class FrameEditor : public nuiSimpleContainer
{
public:
  FrameEditor(ElementDesc* pDesc, ElementInspector* pInspector);
	void InitAttributes();

  virtual ~FrameEditor();

  virtual nglDropEffect OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y);
  virtual void OnDropLeave();
  virtual void OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  
  virtual bool Draw(nuiDrawContext* pContext);
  
  void CommitChanges();
	
	nuiFrame* GetFrame();

	

	
private:

	// attributes ***********************************
	nuiAttrib<float> mAttributeScale;
	nuiAttrib<const nuiRect&> mAttributeRect;
	nuiAttrib<const nuiColor&> mAttributeColor;

	nuiAttribute<float>* mpAttributeScale;
  
	// attributes ***********************************
	float GetScale();
	void SetScale(float scale);


  bool OnCommitChanges(const nuiEvent& rEvent);
  
  ElementDesc* mpDesc;
	ElementInspector* mpInspector;
  nuiFrame* mpFrame;
  nuiFrameView* mpFrameView;
  nuiScrollView* mpFrameViewHolder;
  nuiSimpleContainer* mpPictureHolder;
	nuiImage* mpImage;
  bool mDrawDndFrame;
  
  void UpdateFrameViews(bool CopyImageRect);
  void OnFrameRectChanged(const nuiRect& rRect);
  void OnFrameViewRectChanged(const nuiRect& rRect);
	void OnColorChanged(const nuiColor& color);
  bool OnFrameMouseMoved(const nuiEvent& rEvent);
  nuiSlotsSink mSlotSink;
  nuiEventSink<FrameEditor> mEventSink;
  
	
	
	// attributes ***********************************
  float mScale;
};

