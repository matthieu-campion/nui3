/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define COL_NAME 0
#define COL_STATE 1
#define COL_TIME 2
#define COL_WARNING 3

#define COL_SIZE_NAME 250
#define COL_SIZE_STATE 50
#define COL_SIZE_TIME 100
#define COL_SIZE_WARNING 100


nuiThreadInspector::nuiThreadInspector()
  : mEventSink(this)
{
  if (SetObjectClass(_T("nuiThreadInspector")))
    InitAttributes();
      
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  if (pDeco)
  {
    SetDecoration(pDeco, eDecorationBorder);
  }
  
  mpTimer = new nuiTimer(1.0f);
  mEventSink.Connect(mpTimer->Tick, &nuiThreadInspector::Update);
  
  mpScrollView = NULL;
  mpTree = NULL;

  mRunning = nglThreadChecker::IsCheckerEnabled();
  mUpdating = true;
  mShowLongLock = false;
  
  Build();

}

nuiThreadInspector::~nuiThreadInspector()
{
  mpTimer->Stop();
  delete mpTimer;
}


void nuiThreadInspector::InitAttributes()
{
  AddAttribute(new nuiAttribute<bool>
   (nglString(_T("Running")), nuiUnitOnOff,
    nuiAttribute<bool>::GetterDelegate(this, &nuiThreadInspector::IsRunning),
    nuiAttribute<bool>::SetterDelegate(this, &nuiThreadInspector::SetRunning)));
  
  AddAttribute(new nuiAttribute<bool>
   (nglString(_T("Updating")), nuiUnitOnOff,
    nuiAttribute<bool>::GetterDelegate(this, &nuiThreadInspector::IsUpdating),
    nuiAttribute<bool>::SetterDelegate(this, &nuiThreadInspector::SetUpdating)));
  
  AddAttribute(new nuiAttribute<const nuiRange&>
   (nglString(_T("Frequency")), nuiUnitSeconds,
    nuiAttribute<const nuiRange&>::GetterDelegate(this, &nuiThreadInspector::GetFrequency),
    nuiAttribute<const nuiRange&>::SetterDelegate(this, &nuiThreadInspector::SetFrequency)));
  
  AddAttribute(new nuiAttribute<bool>
   (nglString(_T("ShowLongLock")), nuiUnitOnOff,
    nuiAttribute<bool>::GetterDelegate(this, &nuiThreadInspector::GetShowLongLock),
    nuiAttribute<bool>::SetterDelegate(this, &nuiThreadInspector::SetShowLongLock)));
    
  AddAttribute(new nuiAttribute<const nuiRange&>
   (nglString(_T("LongLockTreshold")), nuiUnitSeconds,
    nuiAttribute<const nuiRange&>::GetterDelegate(this, &nuiThreadInspector::GetLongLockTreshold),
    nuiAttribute<const nuiRange&>::SetterDelegate(this, &nuiThreadInspector::SetLongLockTreshold)));
  
}




//
// build permanent part of interface, once for all
//
void nuiThreadInspector::Build()
{
  if (mpScrollView)
    mpScrollView->Trash();
    
  mpScrollView = new nuiScrollView();
  AddChild(mpScrollView);
  
  mpMainBox = new nuiVBox(3);
  mpMainBox->SetExpand(nuiExpandShrinkAndGrow);
  mpScrollView->AddChild(mpMainBox);


  nuiGrid* pGrid = new nuiGrid(4, 1);
  mpMainBox->SetCell(0, pGrid);
  pGrid->DisplayGridBorder(true, 1);
  pGrid->SetColor(eBorder, INTROSPECTOR_COLOR_GRID);
  
  uint row = 0;
  
  // column headers
  pGrid->SetColumnExpand(0, nuiExpandFixed);
  pGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);
  pGrid->SetColumnExpand(2, nuiExpandFixed);
  pGrid->SetColumnExpand(3, nuiExpandShrinkAndGrow);
  
  nuiLabel* pLabel = new nuiLabel(_T("ThreadChecker"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
  pLabel->SetColor(eNormalTextFg, INTROSPECTOR_COLOR_GRID_TITLE);
  pLabel->SetDecoration(nuiDecoration::Get(INTROSPECTOR_DECO_GRID_TITLE), eDecorationBorder);
  pGrid->SetCell(0,0, pLabel);

  pLabel = new nuiLabel(_T(""), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
  pLabel->SetDecoration(nuiDecoration::Get(INTROSPECTOR_DECO_GRID_TITLE), eDecorationBorder);
  pGrid->SetCell(1,0, pLabel);
  pLabel = new nuiLabel(_T(""), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
  pLabel->SetDecoration(nuiDecoration::Get(INTROSPECTOR_DECO_GRID_TITLE), eDecorationBorder);
  pGrid->SetCell(2,0, pLabel);
  pLabel = new nuiLabel(_T(""), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
  pLabel->SetDecoration(nuiDecoration::Get(INTROSPECTOR_DECO_GRID_TITLE), eDecorationBorder);
  pGrid->SetCell(3,0, pLabel);


  //**********************************************************
  //
  // user controls for threadchecker, 1st column
  //
  
  // Running
  row++;
  pGrid->AddRows(row, 1);
  pLabel = new nuiLabel(_T("Running"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(0,1, pLabel);
  nuiAttribBase Base = GetAttribute(_T("Running"));
  nuiAttributeEditor* pEditor = Base.GetEditor();
  pEditor->SetBorder(2, 2);
  pGrid->SetCell(1,1, pEditor, nuiCenter);


  // Updating
  row++;
  pGrid->AddRows(row, 1);
  pLabel = new nuiLabel(_T("Updating"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(0,2, pLabel);
  Base = GetAttribute(_T("Updating"));
  pEditor = Base.GetEditor();
  pEditor->SetBorder(2, 2);
  pGrid->SetCell(1,2, pEditor, nuiCenter);
  
  // read-only updates count
  row++;
  pGrid->AddRows(row, 1);
  pLabel = new nuiLabel(_T("Update Count"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(0,3, pLabel);
  mpUpdateCount = new nuiFormatedLabel(nuiFormatedLabel::DefaultFormater, 0.f, nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(1,3, mpUpdateCount);

  // update frequency
  row++;
  mFrequency = nuiRange(1.f, 0.f, 1.f, 0.1f, 0.f, 0.f);
  pGrid->AddRows(row, 1);
  pLabel = new nuiLabel(_T("Update Frequency"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(0,4, pLabel);
  nuiAttrib<const nuiRange&> Att(GetAttribute(_T("Frequency")));
  Att.Set(mFrequency); //init
  pEditor = Att.GetEditor();
  pEditor->SetBorder(2, 2);
  pGrid->SetCell(1,4, pEditor);


  //**********************************************************
  //
  // user controls for threadchecker, 2nd column
  //
  
  // show long lock
  pLabel = new nuiLabel(_T("Show Long Locks"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(2,1, pLabel);
  Base = GetAttribute(_T("ShowLongLock"));
  pEditor = Base.GetEditor();
  pEditor->SetBorder(2, 2);
  pGrid->SetCell(3,1, pEditor, nuiCenter);


  // long lock treshold
  mLongLockTreshold = nuiRange(10.f, 0.f, 60.f, 1.f, 0.f, 0.f);
  pLabel = new nuiLabel(_T("Long Lock Treshold"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pGrid->SetCell(2,2, pLabel);
  Att = nuiAttrib<const nuiRange&>(GetAttribute(_T("LongLockTreshold")));
  Att.Set(mLongLockTreshold); // init
  pEditor = Att.GetEditor();
  pEditor->SetBorder(2, 2);
  pGrid->SetCell(3,2, pEditor);



  
}




//
// build threadchecker's tree. each time the "running" button is pressed
//
void nuiThreadInspector::BuildThreadTree()
{
  mCurrentTime = nglTime();
  std::map<nglThread::ID, std::list<nglThreadState> > states;
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it;
  nglThreadChecker::GetStates(states);
  
  mpTree = new nuiTreeNode(_T("Threads"));
  
  nuiTreeView* pTree = new nuiTreeView(mpTree);
  mpMainBox->SetCell(1, pTree);
  mpTree->Open(true);
  
  
  // a node for each thread referenced in the threadchecker
  for (it = states.begin(); it != states.end(); ++it)
  {
    nglThread::ID ID = it->first;
    const std::list<nglThreadState>& list = it->second;

    nuiTreeNode* pNode = CreateNodeFromThread(ID);
    mpTree->AddChild(pNode);
    
    // no critical sections referenced for this thread
    if (list.size() == 0)
      continue;

    // a node for each lock referenced by this thread
    std::list<nglThreadState>::const_iterator its;
    for (its = list.begin(); its != list.end(); ++its)
    {
      const nglThreadState& state = *its;
      
      nuiTreeNode* pLockNode = CreateNodeFromLock(state);
      pNode->AddChild(pLockNode);
    }

  }
  

}



//*******************************************************************************



nuiTreeNode* nuiThreadInspector::CreateNodeFromThread(nglThread::ID ID)
{
  nglString threadName;
  nglThreadChecker::GetThreadName(ID, threadName);

  // a box the for the thread's node
  nuiHBox* pBox = new nuiHBox(0);
  nuiTreeNode* pNode = new nuiTreeNode(pBox, true);
  // give the node a token (the thread ID) for later update
  pNode->SetToken(new nuiToken<nglThread::ID>(ID));

  
  // thread's name
  nglString label;
  label.Format(_T("thread '%s' [0x%x]"), threadName.GetChars(), ID);
  pBox->AddCell(new nuiLabel(label,  nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD"))));

  return pNode;
}




nuiTreeNode* nuiThreadInspector::CreateNodeFromLock(const nglThreadState& rState)
{
  // a box the for the cs's node
  nuiHBox* pBox = new nuiHBox(0);
  nuiTreeNode* pNode = new nuiTreeNode(pBox, true);
  // give the node a token (the cs pointer) for later update
  pNode->SetToken(new nuiToken<nglLock*>(rState.GetLockPointer(), false));

  
  // lock's name
  nglString tmp;
  tmp.Format(_T("%s '%s' [0x%x]"), 
    rState.GetLockPointer()->GetLabel().GetChars(), 
    rState.GetLockPointer()->GetName().GetChars(), rState.GetLockPointer());
  nuiLabel* pLabel = new nuiLabel(tmp, nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
  pLabel->UseEllipsis(true);
  pBox->AddCell(pLabel);
  pBox->SetCellPixels(COL_NAME, COL_SIZE_NAME);

  // lock's state
  tmp.Format(_T("- %s -"), rState.GetStateToString().GetChars());
  pBox->AddCell(new nuiLabel(tmp, nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL"))));
  pBox->SetCellPixels(COL_STATE, COL_SIZE_STATE);
  
  // lock's state time
  tmp.Format(_T("%.3fs"), rState.GetTimeLength(mCurrentTime));
  pBox->AddCell(new nuiLabel(tmp, nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL"))));
  pBox->SetCellPixels(COL_TIME, COL_SIZE_TIME);
  
  // lock's warning
  pBox->AddCell(new nuiLabel(_T(""), nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL"))));
  pBox->SetCellPixels(COL_WARNING, COL_SIZE_WARNING);
  
  return pNode;
}



//**********************************************************************************************


//
// parse the current threadchecker's state and parse the threadchecker's tree. update the tree from the state.
//
void nuiThreadInspector::Update(const nuiEvent& rEvent)
{
  if (!mpTree)
  {
    mpTimer->Stop();
    mCurrentTime = nglTime();
    BuildThreadTree();
    mpTimer->Start();
    rEvent.Cancel();
    return;
  }
  
  mpUpdateCount->SetValue(mpUpdateCount->GetValue()+1);
  mCurrentTime = nglTime();
  
  // get the current threadchecker data
  std::map<nglThread::ID, std::list<nglThreadState> > states;
  std::map<nglThread::ID, std::list<nglThreadState> >::iterator it;
  nglThreadChecker::GetStates(states);

  
  // for each thread in data, look for the node in the tree
  // if the expected node is not found, add it to the tree
  for (it = states.begin(); it != states.end(); ++it)
  {
    nglThread::ID ID = it->first;
    const std::list<nglThreadState>& list = it->second;
    
    nuiTreeNode* pNode = GetNodeFromThread(ID, mpTree);
    
    // node not found. add it
    if (!pNode)
    {
      pNode = CreateNodeFromThread(ID);
      mpTree->AddChild(pNode);
      continue;
    }

    // node found. check if the associated thread contains critical section or not
    if (list.size() == 0)
      continue;


 
     // yes it does. check locks for update
    std::list<nglThreadState>::const_iterator its;
    for (its = list.begin(); its != list.end(); ++its)
    {
      const nglThreadState& state = *its;
      
      nuiTreeNode* pLockNode = GetNodeFromLock(state.GetLockPointer(), pNode);
      
      // lock not found. add it
      if (!pLockNode)
      {
        pLockNode = CreateNodeFromLock(state);
        pNode->AddChild(pLockNode);
        continue;
      }
      
      // lock found. update the lock's state
      SetNodeFromLock(pLockNode, state);      
    }
    
  }
  
  // clean tree. remove dead threads
  Clean(mpTree, states);
  
  rEvent.Cancel();
}



// update cs's node
void nuiThreadInspector::SetNodeFromLock(nuiTreeNode* pLockNode, const nglThreadState& rState)
{
  nuiHBox* pBox = (nuiHBox*)pLockNode->GetElement();
  
  nglString tmp;
  tmp.Format(_T("- %s -"), rState.GetStateToString().GetChars());
  nuiLabel* pLabel = (nuiLabel*)pBox->GetCell(COL_STATE);
  pLabel->SetText(tmp);
  
  tmp.Format(_T("%.3fs"), rState.GetTimeLength(mCurrentTime));
  pLabel = (nuiLabel*)pBox->GetCell(COL_TIME);
  pLabel->SetText(tmp);

  pLabel = (nuiLabel*)pBox->GetCell(COL_WARNING);
  pLabel->SetText(rState.GetWarningToString().GetChars());

  if (rState.GetWarning() != nglThreadState::eWarningNone)
  {
    nuiDecoration* pDeco = nuiDecoration::Get(_T("THREADINSPECTOR_DECO_WARNING"));
    if (!pDeco)
      pDeco = new nuiColorDecoration(_T("THREADINSPECTOR_DECO_WARNING"), nuiRect(2, 2, 0, 0), nuiColor(240,0,0));
    pLabel->SetDecoration(pDeco, eDecorationBorder);
  }
  else
    pLabel->SetDecoration(NULL);
  
}




void nuiThreadInspector::Clean(nuiTreeNode* pParent, const std::map<nglThread::ID, std::list<nglThreadState> >& states)
{
  std::map<nglThread::ID, std::list<nglThreadState> >::const_iterator itc;
  std::list<nglThreadState>::const_iterator itl;

  // clean threads
  if (pParent->GetChildrenCount() == 0)
    return;
  nuiTreeNode* pNode = (nuiTreeNode*)pParent->GetChild(0);
  while (pNode)
  {
    nglThread::ID threadID;
    bool res = nuiGetTokenValue<nglThread::ID>(pNode->GetToken(), threadID);
    NGL_ASSERT(res);

    nuiTreeNode* pNext = pNode->GetNextInParent();

    itc = states.find(threadID);
    
    // the thread doesn't exist anymore
    if (itc == states.end())
      pParent->DelChild(pNode);

    pNode = pNext;
  }


  // clean threads' lock
  pNode = (nuiTreeNode*)pParent->GetChild(0);
  while (pNode)
  {
    // parse the thread's lock nodes
    if (pNode->GetChildrenCount() == 0)
    {
      pNode = pNode->GetNextInParent();
      continue;
    }
  
    // retrieve thread's infos
    nglThread::ID threadID;
    bool res = nuiGetTokenValue<nglThread::ID>(pNode->GetToken(), threadID);
    NGL_ASSERT(res);
          
    itc = states.find(threadID);
    const std::list<nglThreadState>& list = itc->second;
      
    nuiTreeNode* pLockNode = (nuiTreeNode*)pNode->GetChild(0);
    while (pLockNode)
    {
      nglLock* pLock = NULL;
      bool lockdead = true;
      bool res = nuiGetTokenValue<nglLock*>(pLockNode->GetToken(), pLock);
      
      if (!res)
      {
        nuiTreeNode* pNext = pLockNode->GetNextInParent();
        pLockNode = pNext; 
        continue;
      }
      
      //
        
      for (itl = list.begin(); itl != list.end(); ++itl)
      {
        const nglThreadState& state = *itl;
        
        // lock's not dead yet
        if (pLock == state.GetLockPointer())
        {
          lockdead = false;
          break;
        }
      }

      nuiTreeNode* pNext = pLockNode->GetNextInParent();
      
      if (lockdead)
         pNode->DelChild(pLockNode);

      pLockNode = pNext;

    }
    
    nuiTreeNode* pNext = pNode->GetNextInParent();
    pNode = pNext;
    
  }

}





//
// parse the tree's root to retrieve a node from the given thread ID
//
nuiTreeNode* nuiThreadInspector::GetNodeFromThread(nglThread::ID ID, nuiTreeNode* pParent)
{
  int count = pParent->GetChildrenCount();
  for (int i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = (nuiTreeNode*)pParent->GetChild(i);
    nglThread::ID nodeID;
    bool res = nuiGetTokenValue<nglThread::ID>(pNode->GetToken(), nodeID);
    if (res && (nodeID == ID))
      return pNode;
  }
  
  return NULL;
}



//
// parse a thread's node to retrieve a node from the given lock's pointer
//
nuiTreeNode* nuiThreadInspector::GetNodeFromLock(nglLock* pLock, nuiTreeNode* pParent)
{
  int count = pParent->GetChildrenCount();
  for (int i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = (nuiTreeNode*)pParent->GetChild(i);
    nglLock* pPtr;
    bool res = nuiGetTokenValue<nglLock*>(pNode->GetToken(), pPtr);
    if (res && (pPtr == pLock))
      return pNode;
  }
  
  return NULL;
}


nuiLabel* nuiThreadInspector::GetLabelFromNode(nuiTreeNode* pNode)
{
  return (nuiLabel*)((nuiHBox*)pNode->GetElement())->GetCell(1);
}


//***********************************************************************************************
//
// attributes
//
bool nuiThreadInspector::IsRunning()
{
  return mRunning;
}


void nuiThreadInspector::SetRunning(bool run)
{
  if (mRunning == run)
    return;
    
  mRunning = run;
  
  nglThreadChecker::EnableChecker(mRunning);
  
  if (mRunning)
  {
    BuildThreadTree();
    if (mUpdating)
      mpTimer->Start();
  }
  else
  {
    mpTimer->Stop();
    mpUpdateCount->SetValue(0);  
  }
}


bool nuiThreadInspector::IsUpdating()
{
  return mUpdating;
}

void nuiThreadInspector::SetUpdating(bool update)
{
  if (mUpdating == update)
    return;
    
  mUpdating = update;
  
  if (mUpdating)
  {
    mpTimer->Start();
  }
  else
  {
    mpTimer->Stop();
    mpUpdateCount->SetValue(0);
  }
}


const nuiRange& nuiThreadInspector::GetFrequency()
{
  return mFrequency;
}

void nuiThreadInspector::SetFrequency(const nuiRange& rFreq)
{
  mFrequency = rFreq;
  bool run = mpTimer->IsRunning();
  mpTimer->Stop();
  mpTimer->SetPeriod(mFrequency.GetValue());
  if (run)
    mpTimer->Start(false);
}


  
bool nuiThreadInspector::GetShowLongLock()
{
  return mShowLongLock;
}

void nuiThreadInspector::SetShowLongLock(bool show)
{
  mShowLongLock = show;
  nglThreadChecker::EnableLongLockChecker(show);
}

const nuiRange& nuiThreadInspector::GetLongLockTreshold()
{
  return mLongLockTreshold;
}

void nuiThreadInspector::SetLongLockTreshold(const nuiRange& rTreshold)
{
  mLongLockTreshold = rTreshold;
  nglThreadChecker::SetLongLockThreshold(rTreshold.GetValue());
}
  


