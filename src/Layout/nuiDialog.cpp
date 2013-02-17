/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#include "nui.h"

nuiDialog::nuiDialog(nuiContainer* pParent)
  : nuiModalContainer(pParent), mSink(this),  
  mpLayoutContainer(NULL), mpGlobalGrid(NULL), mpButtonsGrid(NULL),  
  mResult(eDialogAccepted), mpButtonOk(NULL), mpButtonCancel(NULL), mpButtonApply(NULL), mpPane(NULL), mpDefaultButton(NULL)
{
  if (SetObjectClass(_T("nuiDialog")))
    InitAttributes();
  ///< Calls InitDialog() just after this ctor(), needed to enable virtual calls
}




void nuiDialog::InitDialog(nuiSimpleContainer* pLayoutContainer, uint32 buttons)
{
  if (pLayoutContainer)
    SetLayoutContainer(pLayoutContainer);
  else
    SetLayoutContainer(CreateDefaultLayout());
  
  NGL_ASSERT(mpLayoutContainer);
  NGL_ASSERT(GetTopLevel());

  SetDefaultPos();
  SetDefaultButtons(buttons); ///< if buttons == 0, default Grids wont be created to enable a user construction by parenting any user items on mpLayoutContainer returned by GetLayoutContainer
  SetDefaultAnimations();

  // Register Hot Keys:
  RegisterHotKeyKey(_T("CancelDialog"), NK_ESC, 0, false, false, _T("Cancel dialog"));
  mSink.Connect( GetHotKeyEvent(_T("CancelDialog")), &nuiDialog::OnCancel);
  RegisterHotKeyKey(_T("AcceptDialog"), NK_ENTER, 0, false, false, _T("Accept dialog"));
  mSink.Connect( GetHotKeyEvent(_T("AcceptDialog")), &nuiDialog::OnAccept );
  RegisterHotKeyKey(_T("AcceptDialogAlt"), NK_PAD_ENTER, 0, false, false, _T("Accept dialog (alternate)"));
  mSink.Connect( GetHotKeyEvent(_T("AcceptDialogAlt")), &nuiDialog::OnAccept );  
}

void nuiDialog::InitDialog(const nglString& rMessage, nuiSimpleContainer* pLayoutContainer, uint32 buttons)
{
  nuiLabel* pLabel = new nuiLabel(rMessage);
  
  pLabel->SetHMargin(5.f);
  pLabel->SetVMargin(8.f);
  InitDialog(pLabel, pLayoutContainer, buttons);
}

void nuiDialog::InitDialog(nuiLabel* pLabel, nuiSimpleContainer* pLayoutContainer, uint32 buttons)
{
  pLabel->SetObjectClass(_T("nuiDialog::Title"));
  pLabel->SetObjectName(_T("nuiDialog::Title"));

  InitDialog(pLayoutContainer, buttons);

//  NGL_ASSERT(buttons != 0);
  NGL_ASSERT(mpGlobalGrid);
  
  mpGlobalGrid->SetCell(0,0, pLabel);
}


void nuiDialog::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Decoration")), nuiUnitName,
                nuiAttribute<const nglString&>::GetterDelegate(this, &nuiDialog::GetDecorationName),
                nuiAttribute<const nglString&>::SetterDelegate(this, &nuiDialog::SetDecoration)));
}


nuiDialog::~nuiDialog()
{
}

//  virtual 
void nuiDialog::SetDecoration(const nglString& rName)
{
  if (mpPane)
    mpPane->SetDecoration(rName);
}

//  virtual 
const nglString& nuiDialog::GetDecorationName()
{
  if (mpPane)
    return mpPane->GetDecorationName();
  return nglString::Null;
}


void nuiDialog::CreateGrids()
{
  mpGlobalGrid = new nuiGrid(1, 3); 
  mpLayoutContainer->AddChild(mpGlobalGrid);
  mpGlobalGrid->SetColumnExpand(0,  nuiExpandShrinkAndGrow);
  mpGlobalGrid->SetRowExpand(0,     nuiExpandShrinkAndGrow);
  mpGlobalGrid->SetObjectName(_T("GlobalGrid"));

  mpButtonsGrid = new nuiGrid(1, 1);
  mpButtonsGrid->SetRowExpand(0, nuiExpandShrinkAndGrow);
  mpButtonsGrid->SetPosition(nuiRight);
  mpGlobalGrid->SetCell(0,2, mpButtonsGrid);
  mpButtonsGrid->SetObjectName(_T("ButtonsGrid"));
}

void nuiDialog::SetContents(nuiWidget * pWidget, nuiPosition Position)
{
  NGL_ASSERT(mpGlobalGrid);
  mpGlobalGrid->SetCell(0, 1, pWidget, Position);
}

void nuiDialog::SetDefaultButtons(uint32 buttons)
{
  if (buttons == eDialogButtonCustom)
  {
    if (!mpGlobalGrid)
    CreateGrids();
      return;
  }

  if (buttons & eDialogButtonOk)
  {
    AddButton(_T("Ok"), eDialogButtonOk);
  }
  if (buttons & eDialogButtonCancel)
  {
    AddButton(_T("Cancel"), eDialogButtonCancel);
  }
  if (buttons & eDialogButtonApply)
  {
    AddButton(_T("Apply"), eDialogButtonApply);
  }
}

void nuiDialog::SetDefaultButton(DialogButtonType button)
{
  switch (button)
  {
    case eDialogButtonOk:
      mpDefaultButton = mpButtonOk;
      break;
    case eDialogButtonCancel:
      mpDefaultButton = mpButtonCancel;
      break;
    case eDialogButtonApply:
      mpDefaultButton = mpButtonApply;
      break;
    case eDialogButtonCustom:
      mpDefaultButton = NULL;
      break;
  }
}

nuiButton* nuiDialog::AddButton(const nglString& rButtonLabel, DialogButtonType button, bool IsDefaultButton)
{
  nuiButton* pButton = new nuiButton(rButtonLabel);
//  pButton->SetObjectName(rButtonLabel);
  pButton->SetObjectName(_T("nuiDialog::Button"));

  if (button & eDialogButtonCustom)
  {
    AddButton((nuiButton*)pButton, eDialogButtonCustom, IsDefaultButton);
  }
  else if (button & eDialogButtonOk)
  {
    AddButton((nuiButton*)pButton, eDialogButtonOk, IsDefaultButton);
  }
  else if (button & eDialogButtonCancel)
  {
    AddButton((nuiButton*)pButton, eDialogButtonCancel, IsDefaultButton);
  }
  else if (button & eDialogButtonApply)
  {
    AddButton((nuiButton*)pButton, eDialogButtonApply, IsDefaultButton);
  }
  return pButton;
}

void nuiDialog::AddButton(nuiButton* pButton, DialogButtonType button, bool IsDefaultButton)
{ // respect this order   ... eDialogButtonCustom / eDialogButtonApply / eDialogButtonOk / eDialogButtonCancel
  if (!mpGlobalGrid)
    CreateGrids();

  uint col, row;
  mpButtonsGrid->GetGridSize(col, row);

  if (button & eDialogButtonCustom)
  {
    mpButtonsGrid->AddColumns(0,1);
    mpButtonsGrid->SetCell(0,0, pButton);
  }
  else if (button & eDialogButtonOk)
  { 
    NGL_ASSERT(!mpButtonOk);
    mpButtonOk = pButton;
    mSink.Connect( pButton->Activated, &nuiDialog::OnOk );

    if (mpButtonCancel)
      col -= 1;
    mpButtonsGrid->AddColumns(col, 1);
    mpButtonsGrid->SetCell(col,0, pButton);
    if (!mpDefaultButton)
      mpDefaultButton = pButton;
  }
  else if (button & eDialogButtonCancel)
  {
    NGL_ASSERT(!mpButtonCancel);
    mpButtonCancel = pButton;
    mSink.Connect( pButton->Activated, &nuiDialog::OnCancel );
    

    mpButtonsGrid->AddColumns(col, 1);
    mpButtonsGrid->SetCell(col,0, pButton);
    if (!mpDefaultButton)
      mpDefaultButton = pButton;
  }
  else if (button & eDialogButtonApply)
  {
    NGL_ASSERT(!mpButtonApply);
    mpButtonApply = pButton;
    mSink.Connect( pButton->Activated, &nuiDialog::OnApply );

    if (mpButtonCancel && mpButtonOk)
      col -= 2;
    else if (mpButtonCancel || mpButtonOk)
      col -= 1;
    mpButtonsGrid->AddColumns(col, 1);
    mpButtonsGrid->SetCell(col, 0, pButton);
  }
  
  if (IsDefaultButton)
    mpDefaultButton = pButton;
}

nuiButton* nuiDialog::GetButton(DialogButtonType type)
{
  if (type & eDialogButtonApply)
  {
    return mpButtonApply;
  }
  else if (type & eDialogButtonOk)
  {
    return mpButtonOk;
  }
  else if (type & eDialogButtonCancel)
  {
    return mpButtonCancel;
  }
  return NULL; 
}

nuiGrid* nuiDialog::GetButtonsGrid()
{
  return mpButtonsGrid;
}


void nuiDialog::SetDefaultAnimations()
{
  nuiMetaAnimation* pMetaAnim; 

  pMetaAnim = new nuiMetaAnimation();
  nuiAnimation* pShowFade = new nuiFadeInWidgetAnim(this, 0.3, false);
  pMetaAnim->AddAnimation(pShowFade);
  nuiAnimation* pShowScale = new nuiScaleMatrixWidgetAnim(this, 0.3, 0, 1);
  pMetaAnim->AddAnimation(pShowScale);

  pShowScale->SetEasing(&nuiEasingSinusStartFast);
  pShowFade->SetEasing(&nuiEasingSinusStartSlow);
  
//  nuiMatrix m;
//  m.Scale(.5f,.5f, 1);
//  SetMatrix(m);
//  SetAlpha(0);

  AddAnimation(_T("SHOW"), pMetaAnim);

  pMetaAnim = new nuiMetaAnimation();
  nuiAnimation* pHideFade = new nuiFadeOutWidgetAnim(this, 0.3, false);
  pMetaAnim->AddAnimation(pHideFade);
  nuiAnimation* pHideScale = new nuiScaleMatrixWidgetAnim(this, 0.3, 1, 2.f);
  pMetaAnim->AddAnimation(pHideScale);

  pHideScale->SetEasing(&nuiEasingSinusStartSlow);
  pHideFade->SetEasing(&nuiEasingSinusStartFast);
  
  AddAnimation(_T("TRASH"), pMetaAnim);
}

nuiSimpleContainer* nuiDialog::CreateDefaultLayout()
{
  mpPane = new nuiSimpleContainer();
  mpPane->SetObjectClass(_T("nuiDialog::Pane"));
  mpPane->SetObjectName(_T("nuiDialog::Pane"));
  mpPane->SetColor(eNormalTextFg, nuiColor(0,0,0));
  
  nuiDefaultDecoration::Dialog(mpPane);
  
  return mpPane;
}

void nuiDialog::SetDefaultPos()
{ // middle pos
  NGL_ASSERT(mpLayoutContainer);
  
  const nuiRect& rTopRect = GetTopLevel()->GetRect();
  const nuiRect& rIdealRect = mpLayoutContainer->GetIdealRect();
  mpLayoutContainer->SetUserPos((nuiSize)ToNearest(rTopRect.GetWidth()/2.f - rIdealRect.GetWidth()/2.f), (nuiSize)ToNearest(rTopRect.GetHeight()/2.f - rIdealRect.GetHeight()/2.f));
}

void nuiDialog::SetUserPos(nuiSize X, nuiSize Y)
{
  NGL_ASSERT(mpLayoutContainer);
  
  const nuiRect& rTopRect = GetTopLevel()->GetRect();
  const nuiRect& rIdealRect = mpLayoutContainer->GetIdealRect();
  
  nuiSize x = ( X + rIdealRect.GetWidth()  > rTopRect.GetWidth()  ? rTopRect.GetWidth()  - rIdealRect.GetWidth()  : X ),
          y = ( Y + rIdealRect.GetHeight() > rTopRect.GetHeight() ? rTopRect.GetHeight() - rIdealRect.GetHeight() : Y );

  mpLayoutContainer->SetUserPos(x, y);
}

nuiSimpleContainer* nuiDialog::GetLayoutContainer()
{
  return mpLayoutContainer;
}

void nuiDialog::SetLayoutContainer(nuiSimpleContainer* pContainer, bool saveChilds)
{
  std::list<nuiWidget*> pChilds;
  NGL_ASSERT(pContainer);

  if (mpLayoutContainer)
  { // remove current Layout Decoration
    if (saveChilds)
    { // save childs
      int count = mpLayoutContainer->GetChildrenCount();
      for (int i = 0; i < count; i++)
      {
        nuiWidget* pWidget = mpLayoutContainer->GetChild(i);
        NGL_ASSERT(pWidget);
        pChilds.push_back(pWidget);
      }
      mpLayoutContainer->Clear();
    }
    NGL_ASSERT(dynamic_cast<nuiFixed*>(mpLayoutContainer->GetParent())); // the decorative layout container is always in a nuiFixed
    mpLayoutContainer->GetParent()->Trash();
  }
  
  nuiFixed* pFixed = new nuiFixed();
  AddChild(pFixed);
  pFixed->AddChild(pContainer);
  mpLayoutContainer = pContainer;

  if (saveChilds && !pChilds.empty())
  {
    std::list<nuiWidget*>::const_iterator end = pChilds.end();
    std::list<nuiWidget*>::const_iterator it = pChilds.begin();
    for (; it != end; ++it)
      pContainer->AddChild(*it);
  }
}

void nuiDialog::OnOk(const nuiEvent& rEvent)
{
  mResult = eDialogAccepted;
  if (!DialogDone())
  {
    ExitDialog(rEvent);
  }
}

void nuiDialog::OnAccept(const nuiEvent& rEvent)
{
  if (!mpDefaultButton)
    return;
  
  if (mpDefaultButton == mpButtonOk)
  {
    OnOk(rEvent);
    return;
  }
  else if (mpDefaultButton == mpButtonCancel)
  {
    OnCancel(rEvent);
    return;    
  }
  else if (mpDefaultButton == mpButtonApply)
  {
    OnApply(rEvent);
    return;
  }

  if (mpDefaultButton->Activated())
    rEvent.Cancel();
}

void nuiDialog::OnCancel(const nuiEvent& rEvent)
{ 
  mResult = eDialogCanceled;
  if (!DialogDone())
  {
    ExitDialog(rEvent);
  }
}

void nuiDialog::OnApply(const nuiEvent& rEvent)
{ 
  DialogApplied();
  rEvent.Cancel();
}


void nuiDialog::ExitDialog(const nuiEvent& rEvent)
{
  Trash();
}

nuiDialog::DialogResult nuiDialog::GetResult()
{
  return mResult;
}

