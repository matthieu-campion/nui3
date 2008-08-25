/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "ViewerWin.h"
#include "nuiSplitter.h"
#include "nuiScrollView.h"
#include "nuiMesh.h"
#include "nuiSVGView.h"
#include "nuiImage.h"
#include "nuiText.h"
#include "nuiSWF.h"


ViewerWin::ViewerWin (const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglPath& rPath) :
  nuiMainWindow(rContextInfo, rInfo, NULL, nglPath(_T("../data/"))), mViewerSink(this)
{
  mViewerSink.SetTarget(this);
  mpViewParent = NULL;
  mpView = NULL;
  mPath = rPath.GetAbsolutePath();

  bool is_file = mPath.IsLeaf();
  nuiSplitter* splitter = new nuiSplitter(nuiVertical);
  splitter->SetHandlePos(30);
  AddChild(splitter);
  
  // Left pane
  mpFileTable = new nuiFileTable(is_file ? mPath.GetParent() : mPath);
  nuiScrollView* pScroll = new nuiScrollView();
  splitter->AddChild(pScroll);
  pScroll->AddChild(mpFileTable);
  
  mpFileTable->SetShowType(false);
  mViewerSink.Connect(mpFileTable->Activated, &ViewerWin::OnFileSelected);
  
  // Right pane
  mpViewParent = new nuiSimpleContainer(); 
  splitter->AddChild(mpViewParent);
  
  if (is_file)
    Load(mPath);
  
  GetDrawContext()->SetClearColor(nuiColor("white"));
}

bool ViewerWin::OnFileSelected (const nuiEvent& rEvent)
{
  std::list<uint> selected;

  mpFileTable->GetSelected(selected);
  nglPath path(mpFileTable->GetRow(selected.front())->GetProperty("Path"));
  if (path.IsLeaf() && path.Exists())
    return Load(path.GetPathName());

  return false;
}

bool ViewerWin::Load (const nglPath& rPath)
{
  nglString ext  = rPath.GetExtension();
  nglString mime = rPath.GetMimeType();
  ext.ToLower();

  return
    LoadMesh (rPath, ext, mime) ||
    LoadSVG  (rPath, ext, mime) ||
    LoadSWF  (rPath, ext, mime) ||
    LoadImage(rPath, ext, mime) ||
    LoadText (rPath, ext, mime);
}

bool ViewerWin::Show (nuiWidget* pWidget)
{
  if (mpView)
  {
    mpView->Trash();
    mpView = NULL;
  }

  if (!pWidget)
    return false;

  mpView = pWidget;
  return true;
}

bool ViewerWin::LoadMesh (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType)
{
  if (rExtension != _T("3ds") && rExtension != _T("prj"))
    return false;

  nuiMesh* mesh = new nuiMesh();
  mpViewParent->AddChild(mesh);

  return mesh->Load(rPath) && Show(mesh);
}

bool ViewerWin::LoadSVG (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType)
{
  if (rExtension != _T("svg") && rMimeType != _T("image/svg+xml"))
    return false;

  nuiSVGView* svgview = new nuiSVGView();
  nuiScrollView* pScroll = new nuiScrollView();
  mpViewParent->AddChild(pScroll);
  pScroll->AddChild(svgview);

  nglIFile file(rPath);
  return svgview->Load(file) && Show(pScroll);
}

bool ViewerWin::LoadImage (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType)
{
  if (rMimeType.Compare(_T("image/"), 0, 6) != 0
      && rExtension.Compare(_T("png"), false)
      && rExtension.Compare(_T("jpg"), false)
      && rExtension.Compare(_T("jpeg"), false)
      && rExtension.Compare(_T("tif"), false)
      && rExtension.Compare(_T("tiff"), false)
      && rExtension.Compare(_T("gif"), false)
      && rExtension.Compare(_T("bmp"), false)
      && rExtension.Compare(_T("ppm"), false)
    )
    return false;

  nuiScrollView* pScroll = new nuiScrollView();
  mpViewParent->AddChild(pScroll);
  nuiImage* image = new nuiImage(rPath);
  pScroll->AddChild(image);
  image->SetPosition(nuiFill);
  image->SetFillRule(nuiCenter);
  return Show(pScroll);
}

bool ViewerWin::LoadText (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType)
{
  if (rExtension != _T("txt") && rMimeType.Compare(_T("text/"), 0, 5) != 0)
    return false;

  nglIFile file(rPath);
  nglString buffer;

  if (file.ReadText(buffer) == 0)
    return false;

  nuiScrollView* pScroll = new nuiScrollView();
  mpViewParent->AddChild(pScroll);
  nuiText* text = new nuiText(_T(""));
  pScroll->AddChild(text);
  text->SetFont(nuiTheme::Fixed);
  text->SetFollowModifications(false);
  text->SetText(buffer);
  text->SetFollowModifications(true);

  return Show(pScroll);
}

bool ViewerWin::LoadSWF (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType)
{
  if (rExtension != _T("swf") && rMimeType != _T("application/x-shockwave-flash"))
    return false;

  nuiScrollView* pScroll = new nuiScrollView();
  mpViewParent->AddChild(pScroll);
  nuiSWF* swfview = new nuiSWF();
  pScroll->AddChild(swfview);

  bool res = swfview->Load(rPath);
  if (!res)
    return false;

  swfview->Play();

  return res && Show(pScroll);
}

