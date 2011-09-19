/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
 
#include "FrameEditor.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "nuiSeparator.h"
#include "nuiRectView.h"
#include "Main/MainWindow.h"
#include "Main/Yapuka.h"
#include "Tools/ToolPaneAttributes.h"


FrameEditor::FrameEditor(ElementDesc* pDesc, ElementInspector* pInspector)
: mEventSink(this)
{
	InitAttributes();
	
  mpFrame = NULL;
  mpDesc = pDesc;
	mpInspector = pInspector;
  mpFrameView = NULL;

  
  mEventSink.Connect(mpDesc->CommitChanges, &FrameEditor::OnCommitChanges);

  nuiVBox* pBox = new nuiVBox();
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pBox);
  pBox->AddCell(new nuiSeparator(nuiHorizontal));
  
  nglString str;
  str.Add(_T("nuiFrame editor (")).Add(pDesc->GetName()).Add(_T(")"));
  nuiLabel* pLabel = new nuiLabel(str, nuiFont::GetFont(24));
  pLabel->SetTextColor(nuiColor(_T("lightgray")));
  pBox->AddCell(pLabel, nuiCenter);
  pBox->AddCell(new nuiSeparator(nuiHorizontal));
  
  nuiSplitter* pSplitter = new nuiSplitter(nuiHorizontal, eModePixel);
  pSplitter->SetMasterChild(false);

  pBox->AddCell(pSplitter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);

  pSplitter->SetHandlePos(200);

  // Frame view:
  mpPictureHolder = new nuiSimpleContainer();
  nuiScrollView* pScrollView = new nuiScrollView();
  pScrollView->AddChild(mpPictureHolder);
  pSplitter->AddChild(pScrollView);
  
  mpFrameViewHolder = new nuiScrollView();
  pSplitter->AddChild(mpFrameViewHolder);
  
  const nuiXMLNode* pNode = mpDesc->GetXML();
  if (pNode)
  {
    mpFrame = new nuiFrame(pDesc->GetName());
    //#FIXME!!! Serialization API is deprecated
    //if (mpFrame->Load(pNode))
    {
      UpdateFrameViews(false);
    } 
  }

  if (!mpFrame)
  {
    mpFrameViewHolder->AddChild(new nuiLabel(_T("Drop an image file here...")));
  }
  
  mDrawDndFrame = false;
}



void FrameEditor::InitAttributes()
{
	mScale = 4.0f;

  mpAttributeScale = new nuiAttribute<float>
  (nglString(_T("Zoom")), nuiUnitPercent,
   nuiAttribute<float>::GetterDelegate(this, &FrameEditor::GetScale), 
   nuiAttribute<float>::SetterDelegate(this, &FrameEditor::SetScale),
	 nuiRange(4.0f, 1.0f, 8.0f, 1.0f, 0.0f, 0.0f));
	mAttributeScale = nuiAttrib<float>(this, mpAttributeScale);
}



FrameEditor::~FrameEditor()
{
  CommitChanges();
}



nglDropEffect FrameEditor::OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y)
{
  if (pDragObject->IsTypeSupported(_T("ngl/Files")))
  {
    nglDataFilesObject* pFiles = (nglDataFilesObject*)pDragObject->GetType(_T("ngl/Files"));
    const std::list<nglString>& rFiles(pFiles->GetFiles());
  
    pDragObject->SetDesiredDropEffect(eDropEffectLink);
    mDrawDndFrame = true;
    Invalidate();
    return eDropEffectCopy;
  }
  
  return eDropEffectNone;
}

void FrameEditor::OnDropLeave()
{
  mDrawDndFrame = false;
  Invalidate();
}

void FrameEditor::OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mDrawDndFrame = false;

  nglDataFilesObject* pFiles = (nglDataFilesObject*)pDragObject->GetType(_T("ngl/Files"));
  const std::list<nglString>&rFiles(pFiles->GetFiles());
  
  nglPath imgpath = rFiles.front();

  printf("dropped file: %ls\n", imgpath.GetChars());

  mpFrameViewHolder->Clear();
  mpPictureHolder->Clear();
  mpFrame = new nuiFrame(mpDesc->GetName(), imgpath, nuiRect());

  UpdateFrameViews(true);

	
}

bool FrameEditor::Draw(nuiDrawContext* pContext)
{
  if (mDrawDndFrame)
  {
    nuiRect r(GetRect().Size());
    r.Grow(-2, -2);
    nuiShape shp;
    shp.AddRect(r);
    pContext->SetLineWidth(4);
    pContext->SetStrokeColor(nuiColor(230, 230, 255));
    pContext->DrawShape(&shp, eStrokeShape);
  }

  nuiSimpleContainer::Draw(pContext);
  return true;
}

void FrameEditor::UpdateFrameViews(bool CopyImageRect)
{
  nuiFixed* pFixed = new nuiFixed();
  mpFrameViewHolder->AddChild(pFixed);
  mpFrameView = new nuiFrameView(mpFrame);
  mpFrameView->AddChild(new nuiLabel(_T("This frame should\nlook nice at any size."), nuiFont::GetFont(20)));
  pFixed->AddChild(mpFrameView);
  
  nuiRectView* pFrameRectView = new nuiRectView(mpFrameView->GetIdealRect());
  mSlotSink.Connect(pFrameRectView->RectChanged, nuiMakeDelegate(this, &FrameEditor::OnFrameViewRectChanged));
	mEventSink.Connect(pFrameRectView->MovedMouse, &FrameEditor::OnFrameMouseMoved);
	pFrameRectView->EnableMouseEvent(true);

  pFrameRectView->SetColor(eShapeStroke, nuiColor(_T("blue")));
  mpFrameViewHolder->AddChild(pFrameRectView);
  
  nuiTexture* pTexture = mpFrame->GetTexture();
	mpImage = new nuiImage(pTexture);
  mpImage->SetPosition(nuiTopLeft);
  mpPictureHolder->AddChild(mpImage);

  if (CopyImageRect)
    mpFrame->SetSourceClientRect(nuiRect(0, 0, pTexture->GetImage()->GetWidth(), pTexture->GetImage()->GetHeight()));
  
  nuiRectView* pRectView = new nuiRectView(mpFrame->GetSourceClientRect());
  mSlotSink.Connect(pRectView->RectChanged, nuiMakeDelegate(this, &FrameEditor::OnFrameRectChanged));
	mEventSink.Connect(pRectView->MovedMouse, &FrameEditor::OnFrameMouseMoved);
	pRectView->EnableMouseEvent(true);
  pRectView->SetColor(eShapeStroke, nuiColor(_T("red")));
  mpPictureHolder->AddChild(pRectView);

  mAttributeScale.Set(mScale);
	
	
	// load attributes editors in inspector	
	mAttributeRect = nuiAttrib<const nuiRect&>(mpFrame->GetAttribute(_T("ClientRect")));
	mAttributeColor = nuiAttrib<const nuiColor&>(mpFrame->GetAttribute(_T("Color")));
	
	
	std::list<nuiAttribBase> attributes;

	attributes.push_back(GetMainWindow()->mAttributeMouseCoord);
	attributes.push_back(mAttributeScale);
	
	ToolPaneAttributes* pTPA = new ToolPaneAttributes(_T("Information"), attributes);
	mpInspector->AddToolpane(pTPA);

	attributes.clear();
	attributes.push_back(mAttributeRect);
	attributes.push_back(mAttributeColor);

	ToolPaneAttributes* pTPA2 = new ToolPaneAttributes(_T("Frame"), attributes);
	mpInspector->AddToolpane(pTPA2);
	
	
	// connect color change signal
	mSlotSink.Connect(mAttributeColor.GetChangedSignal(), nuiMakeDelegate(this, &FrameEditor::OnColorChanged));

}



void FrameEditor::OnColorChanged(const nuiColor& color)
{
	mpFrameView->InvalidateLayout();
}


void FrameEditor::OnFrameRectChanged(const nuiRect& rRect)
{
	mAttributeRect.Set(rRect);
  mpFrameView->InvalidateLayout();
}

void FrameEditor::OnFrameViewRectChanged(const nuiRect& rRect)
{
  mpFrameView->SetUserRect(rRect);
}


void FrameEditor::OnFrameMouseMoved(const nuiEvent& rEvent)
{
	nuiMouseMovedEvent* pEvent = (nuiMouseMovedEvent*)&rEvent;
	// set the information nuiAttribute. It will automatically update the gui
	nuiPoint point(pEvent->mX, pEvent->mY);

	GetMainWindow()->mAttributeMouseCoord.Set(point);
  rEvent.Cancel();
}


void FrameEditor::CommitChanges()
{
  if (!mpFrame)
    return;

  nuiXMLNode* pNode = new nuiXMLNode(_T(""));
  //#FIXME!!! Serialization API is deprecated
  //mpFrame->Serialize(pNode);
  pNode->SetAttribute(_T("Name"), mpDesc->GetName());
  mpDesc->SetXML(pNode);
}



void FrameEditor::OnCommitChanges(const nuiEvent& rEvent)
{
  CommitChanges();
}


nuiFrame* FrameEditor::GetFrame()
{	
	return mpFrame;
}



// attributes **************************************************************


void FrameEditor::SetScale(float scale)
{
  mScale = scale;
  nglMatrixf m;
  m.SetScaling(mScale, mScale, 1);
  mpPictureHolder->SetMatrix(m);
}

float FrameEditor::GetScale()
{
  return mScale;
}




