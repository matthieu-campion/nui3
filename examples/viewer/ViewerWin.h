/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __ViewerWin_h__
#define __ViewerWin_h__

#include "nui.h"
#include "nuiMainWindow.h"
#include "nuiFileTable.h"


class ViewerWin : public nuiMainWindow
{
public:
  ViewerWin(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, const nglPath& rPath);

  bool Load(const nglPath& rPath);
  bool OnFileSelected(const nuiEvent& pEvent);

private:
  nuiEventSink<ViewerWin> mViewerSink;

  nglPath        mPath;
  nuiFileTable*  mpFileTable;
  nuiSimpleContainer*  mpViewParent;
  nuiWidget*     mpView;

  bool Show (nuiWidget* pWidget);
  bool LoadMesh  (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType);
  bool LoadSVG   (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType);
  bool LoadImage (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType);
  bool LoadText  (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType);
  bool LoadSWF   (const nglPath& rPath, const nglString& rExtension, const nglString& rMimeType);
};

#endif // __ViewerWin_h__
