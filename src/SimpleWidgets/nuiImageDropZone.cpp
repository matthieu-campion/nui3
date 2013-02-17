/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiImageDropZone::nuiImageDropZone(const nglPath& rImagePath)
{
  SetObjectClass(_T("nuiImageDropZone"));

	mPath = rImagePath;
	mpImage = new nuiImage(rImagePath);
	AddChild(mpImage);
  mDrawDndFrame=false;
}


nuiImageDropZone::nuiImageDropZone(const nglPath& rPath, nuiTexture* pTex)
{
  SetObjectClass(_T("nuiImageDropZone"));
  
	mPath = rPath;
	mpImage = new nuiImage(pTex);
	AddChild(mpImage);
  mDrawDndFrame=false;
}


nuiImageDropZone::~nuiImageDropZone()
{

}



const nglPath& nuiImageDropZone::GetPath() const
{
	return mPath;
}

nuiImage* nuiImageDropZone::GetImage()
{
	return mpImage;
}





nglDropEffect nuiImageDropZone::OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y)
{
  if (pDragObject->IsTypeSupported(_T("ngl/Files")))
  {
    nglDataFilesObject* pFiles = (nglDataFilesObject*)pDragObject->GetType(_T("ngl/Files"));
    const std::list<nglString>&rFiles(pFiles->GetFiles());
    
//    printf("dragged files: %d\n", rFiles.size());
//    if (rFiles.size() != 1)
//      return false;
  
    pDragObject->SetDesiredDropEffect(eDropEffectLink);
    mDrawDndFrame = true;
    Invalidate();
    return eDropEffectLink;
  }
  
  return eDropEffectNone;
}



void nuiImageDropZone::OnDropLeave()
{
  mDrawDndFrame = false;
  Invalidate();
}



void nuiImageDropZone::OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mDrawDndFrame = false;

  nglDataFilesObject* pFiles = (nglDataFilesObject*)pDragObject->GetType(_T("ngl/Files"));
  const std::list<nglString>&rFiles(pFiles->GetFiles());

  printf("dropped files: %d\n", rFiles.size());
  
  nglPath imgpath = rFiles.front();

  printf("dropped file: %s\n", imgpath.GetChars());

  nuiImage* pImage = new nuiImage(imgpath);
  if (pImage == NULL)
  {
    printf("error while loading the image\n");
    delete pImage;
    return;
  }
		
	ImageDropped(imgpath, pImage);
}




bool nuiImageDropZone::SetImage(const nglPath& rPath, nuiImage* pImage)
{
	if (!rPath.Exists())
		return false;
	
	mPath	 = rPath;
	mpImage->Trash();
	
	mpImage = pImage;
	AddChild(mpImage);	
	
	return true;
}


bool nuiImageDropZone::SetImage(const nglPath& rPath)
{
	if (!rPath.Exists())
		return false;
	
	mPath	 = rPath;
	mpImage->Trash();
	
  mpImage = new nuiImage(mPath);
	AddChild(mpImage);
	
	return true;
}



bool nuiImageDropZone::Draw(nuiDrawContext* pContext)
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




