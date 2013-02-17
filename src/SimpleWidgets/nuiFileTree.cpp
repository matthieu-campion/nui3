/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/



#include "nui.h"


#define FIRST_ROW_HEIGHT  50.f


//**************************************************************************************************************
//
// nuiFileTree
//
//**************************************************************************************************************

nuiFileTree::nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const nglString& rFilter, bool showHiddenFiles)
: nuiFileSelectorBase(), mEventSink(this)
{
  std::list<nglString> filters;
  if (!rFilter.IsNull())
    filters.push_back(rFilter);
  
  Init(rPath, rRootPath, filters, showHiddenFiles);
}

nuiFileTree::nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const std::list<nglString>& rFilters, bool showHiddenFiles)
: nuiFileSelectorBase(), mEventSink(this)
{
  Init(rPath, rRootPath, rFilters, showHiddenFiles);  
}

void nuiFileTree::Init(const nglPath& rPath, const nglPath& rRootPath, const std::list<nglString>& rFilters, bool showHiddenFiles)
{
  if (SetObjectClass(_T("nuiFileTree")))
    InitAttributes();
  SetObjectName(_T("nuiFileTree"));
  
  mpFileBox = NULL;
  mpTreeView = NULL;
  
  mShowHiddenFiles = showHiddenFiles;
  mFilters = rFilters;
  mpCurrentTree = NULL;
  
  SetRootPath(rRootPath);
  SetPath(rPath);  
}



nuiFileTree::~nuiFileTree()
{
}


void nuiFileTree::InitAttributes()
{
  AddAttribute(new nuiAttribute<nuiColor>
               (nglString(_T("HandleColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiFileTree::GetHandleColor), 
                nuiMakeDelegate(this, &nuiFileTree::SetHandleColor)));    
}

nuiScrollBar* nuiFileTree::GetScrollBar(nuiOrientation Orientation)
{
  if (!mpScrollView)
    return NULL;
  return mpScrollView->GetScrollBar(Orientation);
}

nuiColor nuiFileTree::GetHandleColor()
{
  if (!mpTreeView)
    return nuiColor(0,0,0);
  
  return mpTreeView->GetHandleColor();
}


void nuiFileTree::SetHandleColor(nuiColor rColor)
{
  if (!mpTreeView)
    return;

  mpTreeView->SetHandleColor(rColor);
}

const nglPath& nuiFileTree::GetActivatedPath()
{
  return mActivatedPath;
}

bool nuiFileTree::GetSelectedPath(nglPath& rPath)
{
  nuiTreeNode* pNode = mpTreeView->GetSelectedNode();
  if (!pNode)
    return false;
  
  rPath = pNode->GetProperty(_T("Path"));
  return true;
}


void nuiFileTree::OnNodeActivated(const nuiEvent& rEvent)
{
  nuiFileSelectorNode* pNode = (nuiFileSelectorNode*)rEvent.mpUser;
  nglPath path(pNode->GetProperty(_T("Path")));
  
  mActivatedPath = path;
  
  // send signal
  bool res = Activated();
  
  if (!res && !path.IsLeaf() && !path.IsBundle())
  {
    SetRootPath(path);
    SetPath(path);    
  }
  
  rEvent.Cancel();
}



void nuiFileTree::OnGotoParentFolder(const nuiEvent& rEvent)
{
  nglPath path = mRootPath;
 
  nglPath parent = mRootPath.GetParent();
  
   // hack to see "/Volumes" as the ROOTPATH_ALLVOLUMES
  if ((mRootPath.GetPathName() == _T("/")) || ((mRootPath.GetPathName().GetLength() > 1) && (mRootPath.GetPathName().Extract(mRootPath.GetPathName().GetLength()-2, 2) == _T(":/"))))
  {
    parent = ROOTPATH_ALLVOLUMES;
    path = nglPath(_T("/"));
  }
  
  SetRootPath(parent);
  
  SetPath(path);
  
  rEvent.Cancel();
}



//******************************************************************

void nuiFileTree::FormatFileSize(nuiSize size, nglString& str)
{
  if (size > 1000000000)
    str.Format(_T("%.2fGo"), size / (1024*1024*1024));
  else if (size > 1000000)
    str.Format(_T("%.2fMo"), size / (1024*1024));
  else if (size > 1000)
    str.Format(_T("%.2fKo"), size / 1024);
  else 
    str.Format(_T("%db"), size);  
}


nglString nuiFileTree::GetFileInfo(const nglPath& rPath)
{
  nglString str;
  nglPathInfo info;
  rPath.GetInfo(info);


  // file modification date
  nglTimeInfo timeInfo;
  nglString timestr;
  nglTime lastMod = info.LastMod;
  lastMod.GetLocalTime(timeInfo);
  timestr.Format(_T("%d/%d/%d, %d:%d"), timeInfo.Year+1900, timeInfo.Month, timeInfo.Day, timeInfo.Hours, timeInfo.Minutes);

  str.Append(timestr);
  
  if (rPath.IsLeaf())
  {
    // file size
    str.Append(_T(" - "));
    FormatFileSize(info.Size, str);
  }

  return str;
}






bool nuiFileTree::SetRootPath(const nglPath& rPath)
{
  mRootPath = rPath;
  
  mEventSink.DisconnectAll();

  if (mpFileBox)
    mpFileBox->Trash();
  
  mpFileBox = new nuiVBox(2);
  mpFileBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(mpFileBox);
  
  mpFileBox->SetCellMinPixels(0, FIRST_ROW_HEIGHT);
  
  mpScrollView = new nuiScrollView(true,true);
  
  mpFileBox->SetCell(1, mpScrollView);
  mpFileBox->SetCellExpand(1, nuiExpandShrinkAndGrow);

  
  if (rPath == nglPath(ROOTPATH_ALLVOLUMES))
  {
    mpFileBox->SetCell(0, NULL);
    
    nuiTreeNodePtr pRoot = new nuiTreeNode(ROOTPATH_ALLVOLUMES);
    pRoot->Open(true);

    mpTreeView = new nuiTreeView(pRoot, false);
    mpTreeView->SetDeSelectable(false);
    mpTreeView->SetMultiSelectable(true);

    mpScrollView->AddChild(mpTreeView);

    std::list<nglPathVolume> volumes;
    nglPath::GetVolumes(volumes, nglPathVolume::All);
    
    std::list<nglPathVolume>::iterator it = volumes.begin();
    std::list<nglPathVolume>::iterator end = volumes.end();

    while (it != end)
    {      
      
      
      const nglPathVolume vol(*it);

      nuiTreeNodePtr pNode = GetNewNode(vol.mPath);
      pRoot->AddChild(pNode);
      
      ++it;
    }
    
    mpTreeView->SetDragStartDelegate(nuiMakeDelegate(this, &nuiFileTree::OnDragStartDelegate));
    mpTreeView->SetDragRequestDataDelegate(nuiMakeDelegate(this, &nuiFileTree::OnDragRequestDataDelegate));
    mpTreeView->SetDragStopDelegate(nuiMakeDelegate(this, &nuiFileTree::OnDragStopDelegate));
    
    return true;
  }
  
  
  // "go to parent folder" link
  nuiButton* pBtn = new nuiButton();
  pBtn->SetPosition(nuiLeft);
  pBtn->SetObjectClass(_T("nuiFileTree::ParentFolderButton"));
  pBtn->SetObjectName(_T("nuiFileTree::ParentFolderButton"));
  
  pBtn->InvalidateLayout();

  nuiImage* pIcon = new nuiImage();
  pIcon->SetObjectName(_T("nuiFileTree::ParentFolderIcon"));
  pIcon->SetObjectClass(_T("nuiFileTree::ParentFolderIcon"));
  pBtn->AddChild(pIcon);
  
  
  // connect link to event
  mEventSink.Connect(pBtn->Activated, &nuiFileTree::OnGotoParentFolder);
  
  
  mpFileBox->SetCell(0, pBtn);

  nuiTreeNodePtr pNode = GetNewNode(rPath);  
  
  
  
  if (pNode)
  {
    nuiTreeNodePtr pRoot = new nuiTreeNode(_T("HiddenRoot"));
    pRoot->AddChild(pNode);
    pRoot->Open(true);
    pNode->Open(true);

    mpTreeView = new nuiTreeView(pRoot, false);
    mpTreeView->SetDeSelectable(false);
    mpScrollView->AddChild(mpTreeView);
  }

  mpTreeView->SetDragStartDelegate(nuiMakeDelegate(this, &nuiFileTree::OnDragStartDelegate));
  mpTreeView->SetDragRequestDataDelegate(nuiMakeDelegate(this, &nuiFileTree::OnDragRequestDataDelegate));
  mpTreeView->SetDragStopDelegate(nuiMakeDelegate(this, &nuiFileTree::OnDragStopDelegate));
  
  
  return true;
}



void nuiFileTree::AddTree(const nglPath& rPath, bool Opened)
{
  nuiTreeNodePtr pNode = GetNewNode(rPath);
  pNode->Open(Opened);
  
  mpTreeRoot->AddChild(pNode);
  
  mTrees[rPath] = pNode;  
}



// virtual 
nuiTreeNode* nuiFileTree::GetNewNode(const nglPath& rPath)
{


  nuiHBox* pBox = NULL;
  nuiImage* pIcon = NULL;
  nuiLabel* pLabel = NULL;
  
  nglPath pathName = rPath;
  
  if (pathName.IsLeaf() || pathName.IsBundle())
  {
    // no file is filtered
    if (mFilters.size() == 0)
      return NULL;
    
    else if (IsFilterSet(_T("*")))
    {
      if (pathName.IsBundle())
        pathName = pathName.GetRemovedExtension();
      
      pBox = new nuiHBox(2);
      pIcon = new nuiImage();
      pIcon->SetObjectName(_T("nuiFileTree::FileIcon"));
      pIcon->SetObjectClass(_T("nuiFileTree::FileIcon"));
      
      pLabel = new nuiLabel(pathName.GetNodeName().IsEmpty()?_T("/"):pathName.GetNodeName());        
      pLabel->SetObjectName(_T("nuiFileTree::FileLabel"));      
      pLabel->SetObjectClass(_T("nuiFileTree::FileLabel"));      
      
    }
    
    else 
    {
      if (!IsFileFiltered(rPath.GetNodeName()))
        return NULL;
      else
      {
        if (pathName.IsBundle())
          pathName = pathName.GetRemovedExtension();
        
        
        pBox = new nuiHBox(2);
        pIcon = new nuiImage();
        nglString objectName;
        objectName.Format(_T("nuiFileTree::FileIcon::%s"), rPath.GetExtension().ToLower().GetChars());
        pIcon->SetObjectName(objectName);
        pIcon->SetObjectClass(objectName);
        pLabel = new nuiLabel(pathName.GetNodeName().IsEmpty()?_T("/"):pathName.GetNodeName());        
        pLabel->SetObjectName(_T("nuiFileTree::FileLabel"));
        pLabel->SetObjectClass(_T("nuiFileTree::FileLabel"));
      }
    }
    
    
    
  }
  else
  {
    if (!mShowHiddenFiles && (rPath.GetNodeName().GetChar(0) == _T('.')))
      return NULL;
    
    nglString iconObjectName, labelObjectName;
    
    if (isRoot(rPath))
    {
      iconObjectName = _T("nuiFileTree::VolumeIcon");
      labelObjectName = _T("nuiFileTree::FolderLabel");
    }
    else
    {
      iconObjectName = _T("nuiFileTree::FolderIcon");
      labelObjectName = _T("nuiFileTree::FolderLabel");
    }
    
    
    pBox = new nuiHBox(2);
    pIcon = new nuiImage();
    pIcon->SetObjectName(iconObjectName);
    pIcon->SetObjectClass(iconObjectName);
    pLabel = new nuiLabel(rPath.GetNodeName().IsEmpty() ? rPath.GetPathName() : rPath.GetNodeName());        
    pLabel->SetObjectName(labelObjectName);
    pLabel->SetObjectClass(labelObjectName);
  }
  
  pBox->SetCell(0, pIcon, nuiCenter);
  pBox->SetCell(1, pLabel);
  
  
  if (!pBox)
    return NULL;
  
  nuiFileSelectorNode* pNewNode = new nuiFileSelectorNode(this, rPath, pBox);
  mEventSink.Connect(pNewNode->Activated, &nuiFileTree::OnNodeActivated, (void*)pNewNode);

  return pNewNode;
}


bool nuiFileTree::isRoot(const nglPath& rPath)
{
  const nglString& pathName = rPath.GetPathName();
  
  return (
    rPath.GetParent().GetPathName().IsEmpty() || 
    !rPath.GetPathName().Compare(_T("/")) || 
    !rPath.GetParent().GetPathName().Compare(_T("/Volumes")) ||
    ((pathName.GetLength() == 3) && (pathName[1] == _T(':')) && (pathName[2] == _T('/')))
    );
}

bool nuiFileTree::SetPath(const nglPath& rPath)
{
  mPath = rPath;
  
  nglPath path(rPath);
  path.MakeRelativeTo(GetRootPath());
  std::vector<nglString> tokens;
  path.GetPathName().Tokenize(tokens, _T('/'));
  nuiTreeNodePtr pNode = mpTreeView->GetTree();
  
  if (!tokens.empty())
  {
    // Find start node:
    nuiTreeNodePtr pRes = NULL;
    for (uint32 i = 0; i < pNode->GetChildrenCount() && !pRes; i++)
    {
      nuiTreeNodePtr pBNode = (nuiTreeNodePtr)pNode->GetChild(i);
      bool old = pBNode->IsOpened();
      nglPath p(pBNode->GetProperty(_T("Path")));
      
      
      if (p.GetNodeName() == tokens.at(0))
        pRes = pBNode;
      else
      {
        if (!old)
          pBNode->Open(true);
        for (uint32 j = 0; j < pBNode->GetChildrenCount() && !pRes; j++)
        {
          nuiTreeNodePtr pBNode2 = (nuiTreeNodePtr)pBNode->GetChild(j);
          nglPath p(pBNode2->GetProperty(_T("Path")));
          
          if (p.GetNodeName() == tokens.at(0))
            pRes = pBNode2;
        }
        if (!pRes && !old)
          pBNode->Open(false);
      }
    }
    
    if (!pRes)
      return false;
    
    pNode = pRes;
  }
  else
  {
    pNode = (nuiTreeNodePtr)pNode->GetChild(0);
  }
  
  
  pNode->Select(false);
  nuiColumnTreeView* pColTreeView = dynamic_cast<nuiColumnTreeView*>(mpTreeView);
  
  for (uint i = 0; i < tokens.size(); i++)
  {
    pNode->OpenAllChildren(false);
    pNode->SelectAllChildren(false);
    
    std::vector<nuiTreePtr> children;
    pNode->GetChildren(children);
    for (uint j = 0; j< children.size(); j++)
    {
      nuiFileSelectorNode* pBNode = dynamic_cast<nuiFileSelectorNode*>(children.at(j));
      if (pBNode)
      {
        nglPath p(pBNode->GetProperty(_T("Path")));
        if (p.GetNodeName() == tokens.at(i))
        {
          pNode = pBNode;
          break;
        }
      }
      
    }
    
    if (pNode == mpTreeView->GetSelectedNode()) // Have we found the next node?
      return false; // No next node found: bail out...
    
    nuiTreeNodePtr pParent = pNode;
    while (pParent)
    {
      if (!pParent->IsEmpty())
        pParent->Open(true);
      if (pColTreeView)
        pParent->Select(true);
      pParent = static_cast<nuiTreeNodePtr>(pParent->GetParent());
    }
    
    if (pColTreeView)
      mpTreeView->Select(pNode, true);
    
    if (pNode->IsEmpty()) // Bail out if the last node is a leaf
    {
      PathChanged();
      mpTreeView->SelectionChanged();
      mpTreeView->SetLayout(mpTreeView->GetIdealRect());

      return false;
    }
    
  }
  
  PathChanged();
  mpTreeView->SelectionChanged();
  mpTreeView->SetLayout(mpTreeView->GetIdealRect());
  
  // Update infos and change the edit line:
  nglPathInfo info;
  rPath.GetInfo(info);
    
  mpTreeView->SetHotRect(mpTreeView->GetHotRect());
  
  return true;
}




nglPath nuiFileTree::GetPath() const
{
  return mPath;
  
//  nuiTreeNode* pTree = mpTreeView->GetSelectedNode();
//  if (!pTree)
//    return nglPath(nglString::Null);
//
//	nglPath pathName = pTree->GetProperty(_T("Path"));
//	nglString node = pathName.GetNodeName();
//	
//	nglString addin;
//	
//	if ((addin != nglString::Null) && (node.Compare(addin)))
//	{
//		if (pathName.IsLeaf())
//			return pathName.GetParent() + addin;
//			
//		return pathName + addin;
//	}
//	
//	return pathName;
}




nglPath nuiFileTree::GetRootPath() const
{
  return mRootPath;
}








// virtual
nglDragAndDrop* nuiFileTree::OnDragStartDelegate(nuiTreeNode* pNode)
{
  if (!pNode)
    return NULL;
  
  nglString iconName;
  nglPath path(pNode->GetProperty(_T("Path")));


  if (path.IsLeaf() || path.IsBundle())
  {
    if (IsFilterSet(_T("*")))
      iconName = _T("nuiFileTree::DraggedFileIcon");
    else 
      // for later update
      iconName.Format(_T("nuiFileTree::DraggedFileIcon"));

  }
  else
    iconName = _T("nuiFileTree::DraggedVolumeIcon");

  // create drag and drop object
  nglImage* pImage = nuiDefaultDecoration::GetImage(iconName);
  nglDragAndDrop* pDragObject = new nglDragAndDrop(eDropEffectCopy, pImage, 0, 0);
  pDragObject->AddSupportedDropEffect(eDropEffectMove);
 
  // add files data
  nglDataFilesObject* pDataFileObject = new nglDataFilesObject(_T("ngl/Files"));
  pDataFileObject->AddFile(path.GetPathName());
  pDragObject->AddType(pDataFileObject);

  return pDragObject;  
}






// virtual
void nuiFileTree::OnDragRequestDataDelegate(nuiTreeNode* pNode, nglDragAndDrop* pDragObject, const nglString& rMimeType)
{

}

// virtual
void nuiFileTree::OnDragStopDelegate(nuiTreeNode* pNode, bool canceled)
{
  nglString res = _T("true");
  if (!canceled)
    res = _T("false");
}
