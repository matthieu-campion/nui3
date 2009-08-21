/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "DropDestination_Container.h"
#include "nuiBackgroundPane.h"
#include "nglDataObjects.h"

DropContainer::DropContainer()
{
  SetObjectName(_T("DropContainer"));

  mDnDing = false;
  mDnDValid = false;
  mLastX = 0;
  mLastY = 0;


  nuiBackgroundPane* pPane = new nuiBackgroundPane();
  AddChild(pPane);
  
  mpText = new nuiLabel();
  mpText->SetPosition(nuiTopLeft);
  pPane->AddChild(mpText);
  
  //
  mpText->SetText(_T("From the file tree, you can:\n- drag a file to this container\n- double-click on a file\n- double-click on a folder\n\nThe drag'n drop also works with the OS,\ntry to drag something between this application\nand the system's file explorer for instance."));

}


DropContainer::~DropContainer()
{
  
}


void DropContainer::Log(const nglString& rMsg)
{
  mpText->SetText(mpText->GetText() + _T("\n") + rMsg);
  mpText->Invalidate();
  NGL_OUT(rMsg);
}



// virtual 
nglDropEffect DropContainer::OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y)
{
  mLastX = X;
  mLastY = Y;
  Invalidate();
  
  // drop denied
  if (!pDragObject->IsTypeSupported(_T("ngl/Files")) && !pDragObject->IsTypeSupported(_T("ngl/PromiseFiles")))
  {
    if (pDragObject != mpLastDND)
    {
      mpLastDND = pDragObject;
      Log(_T("\nCan't accept this drop operation. Types:\n"));
      const std::map<nglString, nglDataObject*>& objects(pDragObject->GetSupportedTypesMap());
      std::map<nglString, nglDataObject*>::const_iterator it = objects.begin();
      std::map<nglString, nglDataObject*>::const_iterator end = objects.end();
      uint32 i = 0;
      for ( ; it != end; ++it)
      {
        nglString str;
        str.CFormat(_T("\t%d - '%ls'\n"), i, it->first.GetChars());
        Log(str.GetChars());
        i++;
      }
    }
    Log(_T("\n"));
    return eDropEffectNone;
  }
  
  if (mDnDing)
  {
    if (!mDnDValid)
      return mEffect;
  }
  else
  {
    Log(_T("\nDropContainer::OnCanDrop"));

    mDnDing = true;
    mDnDValid = false;
    nglDataFilesObject* pFiles;
    if (pDragObject->IsTypeSupported(_T("ngl/Files")))
    {
      pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/Files")));
    }
    else if (pDragObject->IsTypeSupported(_T("ngl/PromiseFiles")))
    {
      pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/PromiseFiles")));
    }
    const std::list<nglString>& rFiles = pFiles->GetFiles();
    std::list<nglString>::const_iterator it = rFiles.begin();
    std::list<nglString>::const_iterator end = rFiles.end();
    while (!mDnDValid && it != end)
    {
      const nglString obj = *it;
      
      mDnDValid = true;
      
      nglString msg;
      msg.Format(_T("DropContainer is ok to let '%ls' get in\n"), obj.GetChars());
      Log(msg);
      
      // if the drop must be denied, simply return false here.
      
      ++it;
    }
  }
  
  // the drop is allowed
  mEffect = eDropEffectCopy;
  if (IsKeyDown(NK_ALT))
    mEffect = eDropEffectMove;

  return mEffect;
}




// virtual 
void DropContainer::OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  mLastX = X;
  mLastY = Y;
  Invalidate();
  mDnDing = false;
  if (!mDnDValid)
    return;
  
  std::list<nglString> files;
  
  if (pDragObject->IsTypeSupported(_T("ngl/Files")))
  {
    nglDataFilesObject* pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/Files")));
    files = pFiles->GetFiles();
  }
  else if (pDragObject->IsTypeSupported(_T("ngl/PromiseFiles")))
  {
    nglDataFilesObject *pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/PromiseFiles")));
    files = pFiles->GetFiles();
  }

  nglString tmp;
  nglString msg = _T("\nDropContainer::OnDropped : the following files have been dropped:\n");
  std::list<nglString>::iterator it;
  for (it = files.begin(); it != files.end(); ++it)
  {
    const nglString& rPath = *it;
    tmp.Format(_T("%ls\n"), rPath.GetChars());
    msg.Append(tmp);

    // you can do what u want with the files now
  }

  Log(msg);

  mDnDing = false;
}


bool DropContainer::Draw(nuiDrawContext* pContext)
{
  bool res = nuiSimpleContainer::Draw(pContext);

  pContext->SetBlendFunc(nuiBlendTransp);
  if (mDnDing)
  {
    if (mDnDValid)
    {
      pContext->SetStrokeColor(nuiColor(0,1,0));
    }
    else
    {
      pContext->SetStrokeColor(nuiColor(1,0,0));
    }
  }
  else
  {
    pContext->SetStrokeColor(nuiColor(0,0,1));
  }

  pContext->DrawLine(mLastX - 60, mLastY, mLastX + 60, mLastY);
  pContext->DrawLine(mLastX, mLastY - 60, mLastX, mLastY + 60);
  return res;
}
