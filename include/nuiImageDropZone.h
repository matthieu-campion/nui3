/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiImageDropZone_h__
#define __nuiImageDropZone_h__

#include "nui.h"

/// This class implements a drag'n drop zone for an image
class NUI_API nuiImageDropZone : public nuiSimpleContainer
{
public:
	nuiImageDropZone(const nglPath& rImagePath);
	nuiImageDropZone(const nglPath& rPath, nuiTexture* pTex);
  virtual ~nuiImageDropZone();
	
	const nglPath& GetPath() const;
	nuiImage* GetImage();


	virtual bool OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y);
  virtual void OnDropLeave();
  virtual void OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);

	virtual bool Draw(nuiDrawContext* pContext);

  nuiSignal2<const nglPath&, nuiImage*> ImageDropped; ///< warns the user that an image has been dropped in the zone. returns true or false to tells the zone to accept the image or not.

	bool SetImage(const nglPath& rPath, nuiImage* pImage);
	bool SetImage(const nglPath& rPath);

protected:

	bool mDrawDndFrame;
	nglPath mPath;
	nuiImage* mpImage;

};

#endif // __nuiImageDropZone_h__
