/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiFileSelector.h"
#include "nuiFileTree.h"


//**************************************************************************************************************
//
// nuiFileTree
//
//**************************************************************************************************************

nuiFileTree::nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const nglString& rFilter, bool showHiddenFiles)
: nuiFileSelectorBase(), mEventSink(this)
{
  std::list<nglString> filters;
  if (rFilter != nglString::Null)
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
  SetObjectClass(_T("nuiFileTree"));
  SetObjectName(_T("nuiFileTree"));
  
  mpTreeView = new nuiTreeView(NULL, false);
  //mpTreeView->EnableSubElements(1);
  AddChild(mpTreeView);
  
  mShowHiddenFiles = showHiddenFiles;
  mFilters = rFilters;
  mpCurrentTree = NULL;
  
  SetRootPath(rRootPath);
  SetPath(rPath);  
  
}


nuiFileTree::~nuiFileTree()
{
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
  if (rPath == nglPath(ROOTPATH_ALLVOLUMES))
  {
    nuiTreeNodePtr pRoot = new nuiTreeNode(ROOTPATH_ALLVOLUMES);
    pRoot->Open(true);

    std::list<nglPathVolume> volumes;
    nglPath::GetVolumes(volumes, nglPathVolume::All);
    
    std::list<nglPathVolume>::iterator it = volumes.begin();
    std::list<nglPathVolume>::iterator end = volumes.end();

    while (it != end)
    {      
      const nglPathVolume vol(*it);
      nuiTreeNodePtr pNode = GetNewNode(vol.mPath);
      //pNode->Open(true);
      pRoot->AddChild(pNode);
      
      ++it;
    }
    
    mpTreeView->SetTree(pRoot);
    
    mEventSink.Connect(pRoot->Opened, &nuiFileTree::OnRootStateChanged, (void*)1);
    mEventSink.Connect(pRoot->Closed, &nuiFileTree::OnRootStateChanged, (void*)0);
        
    return true;
  }
  
  nuiTreeNodePtr pNode = GetNewNode(rPath);  
  if (pNode)
  {
    nuiTreeNodePtr pRoot = new nuiTreeNode(_T("HiddenRoot"));
    pRoot->AddChild(pNode);
    pRoot->Open(true);
    pNode->Open(true);
    mpTreeView->SetTree(pRoot);
    
    mEventSink.Connect(pRoot->Opened, &nuiFileTree::OnRootStateChanged, (void*)1);
    mEventSink.Connect(pRoot->Closed, &nuiFileTree::OnRootStateChanged, (void*)0);
    
    SetPath(rPath);
  }
  return true;
}



void nuiFileTree::AddTree(const nglPath& rPath, bool Opened)
{
  nuiTreeNodePtr pNode = GetNewNode(rPath);
  pNode->Open(Opened);
  
  mpTreeRoot->AddChild(pNode);
  
  mTrees[rPath] = pNode;  
}


//pInfo = new nuiLabel(GetFileInfo(rPath));
//pInfo->SetObjectName(_T("nuiFileTree::FileInfo"));      
//pInfo->SetObjectClass(_T("nuiFileTree::FileInfo"));      


// virtual 
nuiTreeNode* nuiFileTree::GetNewNode(const nglPath& rPath)
{
  nuiHBox* pBox = NULL;
  nuiSimpleContainer* pIcon = NULL;
  nuiLabel* pLabel = NULL;
  
  nglPath pathName = rPath;
  
  if (pathName.IsLeaf() || !pathName.GetExtension().Compare(_T("app")))
  {
    // no file is filtered
    if (mFilters.size() == 0)
      return NULL;
    
    else if (IsFilterSet(_T("*")))
    {
      if (!pathName.GetExtension().Compare(_T("app")))
        pathName = pathName.GetRemovedExtension();
      
      pBox = new nuiHBox(2);
      pIcon = new nuiSimpleContainer();
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
        if (!pathName.GetExtension().Compare(_T("app")))
          pathName = pathName.GetRemovedExtension();
        
        
        pBox = new nuiHBox(2);
        pIcon = new nuiSimpleContainer();
        nglString objectName;
        objectName.Format(_T("nuiFileTree::FileIcon::%ls"), rPath.GetExtension().GetChars());
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
    
    if (rPath.GetParent().GetPathName().IsEmpty() || !rPath.GetPathName().Compare(_T("/")) || !rPath.GetParent().GetPathName().Compare(_T("/Volumes")))
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
    pIcon = new nuiSimpleContainer();
    pIcon->SetObjectName(iconObjectName);
    pIcon->SetObjectClass(iconObjectName);
    pLabel = new nuiLabel(rPath.GetNodeName().IsEmpty()?_T("/"):rPath.GetNodeName());        
    pLabel->SetObjectName(labelObjectName);
    pLabel->SetObjectClass(labelObjectName);
  }
  
  pBox->SetCell(0, pIcon, nuiCenter);
  pBox->SetCell(1, pLabel);
  pBox->SetCellPixels(1, 100);
  
  
  if (!pBox)
    return NULL;
  
  nuiFileSelectorNode* pNewNode = new nuiFileSelectorNode(this, rPath, pBox);
  
  return pNewNode;
}





bool nuiFileTree::SetPath(const nglPath& rPath)
{
  //NGL_OUT(_T("nuiFileTree::SetPath('%ls')\n"), rPath.GetChars());
  nglPath path(rPath);
  path.MakeRelativeTo(GetRootPath());
  std::vector<nglString> tokens;
  path.GetPathName().Tokenize(tokens, '/');
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
      //NGL_OUT(_T("%d compare '%ls' with '%ls'\n"), i, p.GetNodeName().GetChars(), tokens.at(0).GetChars());
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
          //NGL_OUT(_T("%d %d compare '%ls' with '%ls'\n"), i, j, p.GetNodeName().GetChars(), tokens.at(0).GetChars());
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
      mpTreeView->SetRect(mpTreeView->GetIdealRect());
      if (pColTreeView)
      {
        //        pColTreeView->CalcHotRect();
        //        pColTreeView->CalcVerticalHotRect();
      }
      return false;
    }
    
    //    if (pColTreeView)
    //      pColTreeView->CreateScrollBars();
  }
  
  //  if (!pColTreeView)
  //  {
  //    mpTreeView->Select(pNode, true);
  //  }
  
  PathChanged();
  mpTreeView->SelectionChanged();
  mpTreeView->SetRect(mpTreeView->GetIdealRect());
  if (pColTreeView)
  {
    //    pColTreeView->CalcHotRect();
    //    pColTreeView->CalcVerticalHotRect();
  }  
  
  
  // Update infos and change the edit line:
  nglPathInfo info;
  rPath.GetInfo(info);
    
  mpTreeView->SetHotRect(mpTreeView->GetHotRect());
  
  return true;
}




nglPath nuiFileTree::GetPath() const
{
  nuiTreeNode* pTree = mpTreeView->GetSelectedNode();
  if (!pTree)
    return nglPath(nglString::Null);

	nglPath pathName = pTree->GetProperty(_T("Path"));
	nglString node = pathName.GetNodeName();
	
	nglString addin;
	
	if ((addin != nglString::Null) && (node.Compare(addin)))
	{
		if (pathName.IsLeaf())
			return pathName.GetParent() + addin;
			
		return pathName + addin;
	}
	
	return pathName;
}




nglPath nuiFileTree::GetRootPath() const
{
  return mRootPath;
}






//bool nuiFileTree::OnRootOpened(const nuiEvent& rEvent)
//{
//  nuiTreeView* pTreeView = (nuiTreeView*)rEvent.mpUser;
//  
//  std::map<nglPath, nuiTreeView*>::iterator it;
//  for (it = mTreeViews.begin(); it != mTreeViews.end(); ++it)
//  {
//    nuiTreeView* pPtr = it->second;
//    
//    if (pPtr != pTreeView)
//      pTreeView->GetTree()->Open(false);
//  }
//  return true;
//}





bool nuiFileTree::OnSelected (const nuiEvent& rEvent)
{
  nuiTreeView* pTreeView = (nuiTreeView*)rEvent.mpUser;
  NGL_ASSERT(pTreeView);
  
//	nglPath curPath;
//  nuiTreeNode* pTree = pTreeView->GetSelectedNode();
//	if (pTree != NULL)
//		curPath = pTree->GetProperty(_T("Path"));
//  
//   
//	if (curPath.IsLeaf())
//  {
//		nglString addin = curPath.GetNodeName();
//  }
//	
	return false;
}
  


bool nuiFileTree::OnRootStateChanged(const nuiEvent& rEvent)
{
  uint32 opened = (uint32)rEvent.mpUser;
  
  // redirect event
//  if (opened)
//    Opened();
//  else
//    Closed();
  
  return true;
}


