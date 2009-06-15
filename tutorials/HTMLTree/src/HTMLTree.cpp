/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "HTMLTree.h"

#include "nuiGrid.h"
#include "nuiTreeView.h"
#include "nuiHTML.h"
#include "nuiSplitter.h"
#include "nuiScrollView.h"
#include "nuiHTTP.h"

class HTMLTreeNode : public nuiTreeNode
{
public:
  HTMLTreeNode(const nuiHTMLNode* pNode)
    : nuiTreeNode(_T("TEMP"))
  {
    nuiLabel* pLabel = (nuiLabel*)mpElement;
    nglString str;
    if (pNode->GetName().IsEmpty())
    {
      str = _T("\"") + pNode->GetText() + _T("\"");
    }
    else
    {
      nglString id(nglString::Empty);
      const nuiHTMLAttrib* pAttrib = pNode->GetAttribute(_T("id"));
      if (pAttrib)
      {
        nglString val = pAttrib->GetValue();
        id.Add(_T(" id=\"")).Add(val).Add(_T("\""));
      }
      str = _T("<") + pNode->GetName() + id + _T(">");
    }
    pLabel->SetText(str);
    mpNode = pNode;
  }
  virtual ~HTMLTreeNode()
  {
    //
  }
  
  void Open(bool Opened)
  {
    if (Opened && !mOpened && !GetChildrenCount())
    {
      uint32 count = mpNode->GetNbChildren();
      for (uint32 i = 0; i < count; i++)
      {
        const nuiHTMLNode* pNode = mpNode->GetChild(i);
        HTMLTreeNode* pChild = new HTMLTreeNode(pNode);
        AddChild(pChild);
      }
    }

    nuiTreeNode::Open(Opened);
  }

  virtual bool IsEmpty() const
  {
    return mpNode->GetNbChildren() == 0;
  }

  const nuiHTMLNode* GetHTMLNode() const
  {
    return mpNode;
  }
  
private:
  const nuiHTMLNode* mpNode;
};


HTMLTree::HTMLTree()
: mSink(this)
{
  SetObjectClass(_T("HTMLTree"));
  mpAttributes = new nuiGrid(2, 0);
  mpAttributes->SetColumnExpand(1, nuiExpandShrinkAndGrow);
  mpTreeView = new nuiTreeView();
  mpHTML = NULL;
  
  mpSplitter = new nuiSplitter(nuiVertical);
  mpScroll1 = new nuiScrollView();
  mpScroll2 = new nuiScrollView();
  
  AddChild(mpSplitter);
  
  mpSplitter->AddChild(mpScroll1);
  mpSplitter->AddChild(mpScroll2);
  
  mpScroll1->AddChild(mpTreeView);
  mpScroll2->AddChild(mpAttributes);
  
  mSink.Connect(mpTreeView->SelectionChanged, &HTMLTree::OnSelectionChanged);
}

HTMLTree::~HTMLTree()
{
  delete mpHTML;
}

bool HTMLTree::Parse(const nglString& rString)
{
  mHTML = rString;
  delete mpHTML;
  mpHTML = new nuiHTML();

  std::string str(rString.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = mpHTML->Load(mem);
  
  nuiTreeNode* pTree = new HTMLTreeNode(mpHTML);
  mpTreeView->SetTree(pTree);
  
  return res;
}

bool HTMLTree::ParseURL(const nglString& rString)
{
  nuiHTTPRequest request(rString);
  nuiHTTPResponse* pResponse = request.SendRequest();
  if (!pResponse)
    return false;

  nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
  mHTML.Import(&pResponse->GetBody()[0], pResponse->GetBody().size(), eUTF8);

  delete mpHTML;
  mpHTML = new nuiHTML();
  bool res = mpHTML->Load(mem);
  
  nuiTreeNode* pTree = new HTMLTreeNode(mpHTML);
  pTree->Open(true);
  
  mpTreeView->SetTree(pTree);
  
  return res;
}

bool HTMLTree::OnSelectionChanged(const nuiEvent& rEvent)
{
  HTMLTreeNode* pNode = (HTMLTreeNode*)mpTreeView->GetSelectedNode();
  if (!pNode)
    return false;
  
  mpAttributes->RemoveRows(0, mpAttributes->GetNbRows());
  
  const nuiHTMLNode* pHTML = pNode->GetHTMLNode();
  NGL_ASSERT(pHTML);
  uint32 count = pHTML->GetNbAttributes();
  if (!count)
    return false;
  mpAttributes->AddRows(0, count);
  for (uint32 i = 0; i < count; i++)
  {
    const nuiHTMLAttrib* pAttrib = pHTML->GetAttribute(i);
    const nglString& name(pAttrib->GetName());
    const nglString& value(pAttrib->GetValue());
    mpAttributes->SetCell(0, i, new nuiLabel(name));
    mpAttributes->SetCell(1, i, new nuiLabel(value));
  }
  return false;
}
