/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"
#include "nuiTreeView.h"

#include "nuiScrollView.h"

class Node: public nuiTreeNode
{
public:
  Node(const nglString& rName)
  : nuiTreeNode(new nuiLabel(rName)), mName(rName), mSize(0)
  {
  }
  
  uint64 GetSize() const
  {
    return mSize;
  }
  
  void SetSize(uint64 s)
  {
    mSize = s;
    nuiLabel* pLabel = (nuiLabel*)GetElement();
    nglString txt;
    txt.Add(mSize).Add(_T(" - ")).Add(mName);
    pLabel->SetText(txt);
  }
  
private:
  uint64 mSize;
  nglString mName;
};

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDebugMode(true);
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

}

MainWindow::~MainWindow()
{
}

bool cmp(const std::pair<nglString, Node*>& rLeft, const std::pair<nglString, Node*>& rRight)
{
  
  return rLeft.second->GetSize() < rRight.second->GetSize();
}

void MainWindow::OnCreation()
{
  nuiScrollView* pScroll = new nuiScrollView();
  AddChild(pScroll);
    
  ///////////////////////////////
  
  
  nglPath p(_T("/Users/meeloo/Desktop/nm.txt"));
  nglIStream* pStream = p.OpenRead();
  
  nglString line;
  
  uint64 lastaddr = 0;
  nglString lastsymbol;
  
  std::map<nglString, Node*> methods;
  std::map<nglString, Node*> classes;
  
  while (pStream->ReadLine(line))
  {
    // Read address
    int32 c = 0;
    while (line[c] && line[c] != ' ')
      c++;
    nglString a(line.Extract(0, c));
    uint64 address = a.GetCUInt(16);

    c++;
    // Read type char
    nglChar type = line[c];
    
    // Read Symbol if we are on a method / function decl
    if (type == 't')
    {
      c++;

      if (!lastsymbol.IsEmpty())
      {
        uint64 lastsize = address - lastaddr;
        
        std::map<nglString, Node*>::iterator it = methods.find(lastsymbol);
        
        bool skip = false;
        Node* pMethod = NULL;
        if (it != methods.end())
        {
          pMethod = it->second;
          pMethod->SetSize(pMethod->GetSize() + lastsize);
          //it->second += lastsize;
          skip = true;
        }
        else
        {
          //NGL_OUT(_T("new method \t %ls\n"), lastsymbol.GetChars());
          pMethod = new Node(lastsymbol);
          pMethod->SetSize(lastsize);
          methods[lastsymbol] = pMethod;
        }
        
        NGL_ASSERT(pMethod != NULL);
        
        if (!skip) // The method already exist so no need to add it to its class
        {
          int32 pos = lastsymbol.Find(':');
          if (pos > 0 && lastsymbol[pos+1] == ':')
          {
            nglString classname = lastsymbol.GetLeft(pos);
            //NGL_OUT(_T("new class %ls\n"), classname.GetChars());
            
            std::map<nglString, Node*>::iterator it = classes.find(classname);
            Node* pNode = NULL;
            if (it != classes.end())
            {
              pNode = it->second;
              pNode->SetSize(it->second->GetSize() + lastsize);
            }
            else
            {
              pNode = new Node(classname);
              pNode->SetSize(lastsize);
              classes[classname] = pNode;
            }
            
            pNode->AddChild(pMethod);
          }
        }
      }
      
      lastaddr = address;
      lastsymbol = line.GetRight(line.GetLength() - c);
    }
  }
  
  delete pStream;
  
  std::list<std::pair<nglString, Node*> > sorted;
  {
    std::map<nglString, Node*>::const_iterator it = classes.begin();
    std::map<nglString, Node*>::const_iterator end = classes.end();
    
    while (it != end)
    {
      //NGL_OUT(_T("add unsorted %ls\n"), it->first.GetChars());
      sorted.push_back(std::pair<nglString, Node*>(it->first, it->second) );
      ++it;
    }
  }
  
  sorted.sort(cmp);
  
  nuiTreeNode* pTree = new nuiTreeNode(new nuiLabel(_T("Classes")));
  
  {
    std::list<std::pair<nglString, Node*> >::const_iterator it = sorted.begin();
    std::list<std::pair<nglString, Node*> >::const_iterator end = sorted.end();
    
    while (it != end)
    {
      //NGL_OUT(_T("%lld\t\t%ls\n"), it->second->GetSize(), it->first.GetChars());
      pTree->AddChild(it->second);
      ++it;
    }
  }
  
  nuiTreeView* pTreeView = new nuiTreeView(pTree);
  pScroll->AddChild(pTreeView);
}




void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}
