/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiGearWin.h"
#include "nuiApplication.h"

#include "nuiButton.h"
#include "nuiPopupMenu.h"
#include "nuiUserArea.h"
#include "nuiLabel.h"
#include "nuiSlider.h"
#include "nuiFixed.h"
#include "nuiSplitter.h"
#include "nuiHBox.h"

/*
typedef struct BlendMode
{
  nglChar* Description;
  GLenum Source;
  GLenum Destination;
} BlendMode;

BlendMode ModeDesc[]=
{
  {_T("Addition"),GL_SRC_ALPHA,GL_ONE},
  {_T("Transparent"),GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA},
  {_T("Weird Colors"),GL_SRC_ALPHA,GL_SRC_COLOR},
  {_T("Shaded addition"),GL_SRC_ALPHA,GL_DST_COLOR},
  {_T("Return of the Weirdos!"),GL_ONE_MINUS_SRC_ALPHA,GL_DST_COLOR},
  {_T("Inverted"),GL_SRC_ALPHA_SATURATE,GL_ONE_MINUS_DST_COLOR},
  {_T("Nice blending... (try the alpha scroller with this one)"),GL_SRC_ALPHA_SATURATE,GL_ONE},
  {NULL,0,0}
};
*/

typedef struct BlendMode
{
  nglChar* Description;
  nuiBlendFunc Func;
} BlendMode;

#define SETFUNC(X) { (nglChar*) _T(#X), nuiBlend##X}

BlendMode ModeDesc[]=
{
  SETFUNC(TranspAdd),
  SETFUNC(Transp),
  SETFUNC(Source),
  SETFUNC(Clear),
  SETFUNC(Dest),
  SETFUNC(Over),
  SETFUNC(OverRev),
  SETFUNC(In),
  SETFUNC(InRev),
  SETFUNC(Out),
  SETFUNC(OutRev),
  SETFUNC(Top),
  SETFUNC(TopRev),
  SETFUNC(XOR),
  SETFUNC(Add),
  SETFUNC(Saturate),

  SETFUNC(TranspClear),
  SETFUNC(TranspOver),
  SETFUNC(TranspInRev),
  
  {NULL,(nuiBlendFunc)0}
};

void SetImageMode(nuiImage* pImage, int mode)
{
  NGL_OUT(_T("Setting blending mode %d: %s\n"),mode,ModeDesc[mode].Description);
  pImage->SetBlendFunc(ModeDesc[mode].Func);
}



nuiGearWin::nuiGearWin(const nuiRect& rRect)
: nuiWindow(rRect, nuiWindow::DecoratedBackground),
  mGearWinSink(this)
{
  nuiSplitter* pSplitter = new nuiSplitter(nuiHorizontal);
  AddChild(pSplitter);
  pSplitter->SetMasterChild(true);
  //LBDEBUG pSplitter->SetFixed(true);
//  nuiContainerPtr pSplitter = this;

  nuiHBox* pBox = new nuiHBox();
  pSplitter->AddChild(pBox);
//  pBox->SetUserRect(nuiRect(5.0f,5.0f,mRect.GetWidth()-10,25.0f));

  /* Packing in this pBox : pButton1, a nuiSlider, pButton2, mpFPSLabel
   */
  nuiButton* pButton1 = new nuiButton(nglString("Toggle Animation"));
  pBox->AddCell(pButton1);
  mGearWinSink.Connect(pButton1->ButtonPressed, &nuiGearWin::ToggleAnimation);

  mpFPSLabel = new nuiLabel(nglString("Alpha:"));
  pBox->AddCell(mpFPSLabel);
  mpAlphaBar = new nuiSlider(nuiHorizontal,nuiRange(0.5f, 0.0f, 1.0f, 0.1f, 0.2f, 0.0f));
  pBox->AddCell(mpAlphaBar);
  nglPath HandlePic("../data/HorizontalSliderHandle.png");
  if (HandlePic.Exists() && HandlePic.IsLeaf())
  {
    nuiImage* pHandle = new nuiImage(HandlePic);
    pHandle->SetPosition(nuiCenter);
    mpAlphaBar->SetHandle(pHandle);
  }

  nuiFixed* pFixed = new nuiFixed();
  pBox->AddCell(pFixed);
  mpFPSLabel = new nuiLabel(nglString("-"));
  pFixed->AddChild(mpFPSLabel);
  mpFPSLabel->SetUserRect(nuiRect(0.0f,0.0f,44.0f,18.0f));
  mLastFPSMeasure = nglTime();
  mFrameCnt = 0;

  /* Free space to draw animated gears (composited with some images).
   */
  nuiSimpleContainer* pContainer = new nuiSimpleContainer();
  pSplitter->AddChild(pContainer);
  pContainer->AddChild(new nuiImage(_T("../data/nui.png") ));
  nuiLabel* pLabel = new nuiLabel(_T("Testing multilayered containers!\nThis text is rendered on top of the nui logo,\nand under the rotating gears."));
  pContainer->AddChild(pLabel);
  pLabel->SetPosition(nuiCenter);

  mpUserArea = new nuiUserArea();
  pContainer->AddChild(mpUserArea);
  mpUserArea->EnableGLInit(true);
  mpImage = new nuiImage(_T("../data/test2.png") );
  pContainer->AddChild(mpImage);
  mpImage->SetPosition(nuiCenter);
//  mpImage->SetFillRule(nuiCenter);
  SetImageMode(mpImage,0);

  pLabel = new nuiLabel(_T("This text label is on the top layer\nand is thus not affected by other elements"));
  pContainer->AddChild(pLabel);
  pLabel->SetTextColor(nuiColor(1.0f,.2f,.2f,.5f));
  pLabel->SetPosition(nuiBottom);

  mGearWinSink.Connect(Clicked, &nuiGearWin::ShowMenu, NULL);
  mGearWinSink.Connect(mpUserArea->UserDraw, &nuiGearWin::UserDraw, NULL);
  mpUserArea->StartAutoDraw(60);
  mpFPSLabel->StartAutoDraw(2);

  mpGears = new Gears(GetDrawContext(),10,10,false);
  mLastDraw = nglTime(0);
  mAnimate = true;

  mpTimer = new nuiTimer(0.5);
  mGearWinSink.Connect(mpTimer->Tick, &nuiGearWin::UpdateFPS);
  mpTimer->Start(false);
}

nuiGearWin::~nuiGearWin()
{
  delete mpGears;
  delete mpTimer;
}

bool nuiGearWin::ShowMenu(const nuiEvent& rEvent)
{
  const nuiMouseClickedEvent& rMEvent((const nuiMouseClickedEvent&)rEvent);

  NGL_OUT(_T("Mouse Clicked: %f - %f\n"), rMEvent.mX, rMEvent.mY);
 
  if (rMEvent.mButton == nglMouseInfo::ButtonMiddle)
  {
    nuiRect rect(rMEvent.mX+2,rMEvent.mY+2,0.0f,0.0f);

    nuiTreeNodePtr pTree = new nuiTreeNode(_T("Demo Menu"));
    
    for (uint i = 0; i < 5; i++)
    {
      nglString txt1;
      txt1.Format("%c", 'A' + i);
      nuiTreeNode* pSubTree = new nuiTreeNode(txt1);
      
      for (uint j = 0; j < 16; j++)
      {
        nglString txt2;
        txt2.Format("%d", j);
        nuiTreeNode* pNode = new nuiTreeNode(txt2);
        for (uint k = 0; k < 8; k++)
        {
          nglString txt3;
          txt3.Format("%d", k);
          nuiTreeNode* pSubNode = new nuiTreeNode(txt3);
          
          pNode->AddChild(pSubNode);
        }
        
        pSubTree->AddChild(pNode);
      }
      pTree->AddChild(pSubTree);
    }


    pTree->AddChild(new nuiTreeNode(new nuiScrollBar(nuiHorizontal,nuiRange(0.5f, 0.0f, 1.3f, 0.1f, 0.3f, 0.3f))));
    pTree->AddChild(new nuiTreeNode(new nuiSlider(nuiHorizontal,nuiRange(0.5f, 0.0f, 1.3f, 0.1f, 0.3f, 0.0f))));

    pTree->AddChild(new nuiTreeNode(_T("This item is a label")));
    pTree->AddChild(new nuiTreeNode(_T("This item is another label")));
    pTree->AddChild(new nuiTreeNode(_T("This item is a third label")));

    nuiImage* pImage = new nuiImage(nglPath(_T("../data/small_ngl.png")));
    pImage->SetPosition(nuiCenter);
    pTree->AddChild(new nuiTreeNode(pImage));

    pImage = new nuiImage(nglPath(_T("../data/gnu.png")));
    pImage->SetPosition(nuiTopRight);
    pTree->AddChild(new nuiTreeNode(pImage));

    pImage = new nuiImage(nglPath(_T("../data/gnu.png")));
    pImage->SetFillRule(nuiFill);
    pTree->AddChild(new nuiTreeNode(pImage));

    pImage = new nuiImage(nglPath(_T("../data/gnu.png")));
    pImage->SetPosition(nuiLeft);
    pTree->AddChild(new nuiTreeNode(pImage));

    pTree->AddChild(new nuiTreeNode(_T("Testing menu")));
  
    pTree->AddChild(new nuiTreeNode(_T("Testing menu 2 blablabla")));
    
    nuiPopupMenu* pMenu = new nuiPopupMenu(this, pTree, rect);
    UNUSED(pMenu);
  }
  else if (rMEvent.mButton == nglMouseInfo::ButtonRight)
  {
    nuiRect rect(rMEvent.mX+2,rMEvent.mY+2,0.0f,0.0f);

    nuiTreeNode* pTree = new nuiTreeNode(_T("Do not forget to try the middle button for the other menu example ;-)!!!"));

    int i;
    for (i = 0; ModeDesc[i].Description; i++)
    {
      pTree->AddChild(new nuiTreeNode(ModeDesc[i].Description));
    }

    nuiPopupMenu* pMenu = new nuiPopupMenu(this, pTree, rect);
    mGearWinSink.Connect(pMenu->MenuDone, &nuiGearWin::ItemSelected, (void*)pMenu);

  }
  return false;
}

bool nuiGearWin::ItemSelected(const nuiEvent& rEvent)
{
  nuiPopupMenu* pMenu = (nuiPopupMenu*)(rEvent.mpUser);
  nuiTreeNode* pNode = pMenu->GetSelectedNode();
  if (pNode)
    SetImageMode(mpImage,(pMenu->GetIdInTree(pNode)));
  Invalidate();
  return false;
}

bool nuiGearWin::UserDraw(const nuiEvent& rEvent)
{
  nuiRect rect = mpUserArea->GetRect();
  float Alpha = mpAlphaBar->GetRange().GetValue();

  glClear(GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  mpGears->Reshape ((int)rect.GetWidth(),(int)rect.GetHeight());

  nglTime now;
  if (mAnimate && mLastDraw > nglTime(0))
    mpGears->Draw(GetDrawContext(),now - mLastDraw,Alpha);
  else
    mpGears->Draw(GetDrawContext(),nglTime(0),Alpha);

  mLastDraw = now;

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();


  // Print FPS
  mFrameCnt++;
  if (mFrameCnt > 10)
  {
    mFPS = (double)mFrameCnt / (now - mLastFPSMeasure);
    mLastFPSMeasure = now;
    mFrameCnt = 0;
  }

  if (mAnimate)
    Invalidate();
  return false;
}

bool nuiGearWin::ToggleAnimation(const nuiEvent& rEvent)
{
  mAnimate = !mAnimate;
  if (mAnimate)
  {
    mpUserArea->StartAutoDraw(60);
    //mpFPSLabel->StartAutoDraw(2);
  }
  else
  {
    mpUserArea->StopAutoDraw();
    mpFPSLabel->StopAutoDraw();
  }

  Invalidate();
  return false;
}


bool nuiGearWin::UpdateFPS(const nuiEvent& rEvent)
{
  nglString temp;
  temp.Format("%.1f fps", mFPS);
  mpFPSLabel->SetText(temp);
  return false;
}
