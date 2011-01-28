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

#include "nuiAudioDevice.h"
/*
  You can create a texture atlas for nui with Zwoptex. Just use the following export script. For the moment you will have to replace the Path of the actual atlas texture un the css file as there is no way to export the texture file name from this tool (contacted the author about it).
 
@TextureAtlas atlas =
{
Path: "{{ metadata.name }}";

{% for sprite in sprites %}
///////////////////////////////// Texture {{ sprite.name }}
TextureName: "{{ sprite.name }}";
Rect: "{ {{sprite.textureRectX}}, {{sprite.textureRectY}}, {{sprite.textureRectWidth}}, {{sprite.textureRectHeight}} }";
{% for alias in sprite.aliases %}Alias: "{{ alias.name }}"; {% /for %}
{% /for %}
}
 
 
 */


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiTexture* pTex1 = nuiTexture::CreateTextureProxy(_T("Tex1"), _T("rsrc:/decorations/Atlas.png"), nuiRect(0, 0, 128, 32));
  nuiTexture* pTex2 = nuiTexture::CreateTextureProxy(_T("Tex2"), _T("rsrc:/decorations/Atlas.png"), nuiRect(128, 0, 128, 32));
  nuiTexture* pTex3 = nuiTexture::CreateTextureProxy(_T("Tex3"), _T("rsrc:/decorations/Atlas.png"), nuiRect(0, 32, 128, 32));
  nuiTexture* pTex4 = nuiTexture::CreateTextureProxy(_T("Tex4"), _T("rsrc:/decorations/Atlas.png"), nuiRect(128, 32, 128, 32));
  nuiImage* pImage1 = new nuiImage(pTex1);
  nuiImage* pImage2 = new nuiImage(pTex2);
  nuiImage* pImage3 = new nuiImage(pTex3);
  nuiImage* pImage4 = new nuiImage(pTex4);
  
  pImage1->SetPosition(nuiTopLeft);
  pImage2->SetPosition(nuiTopRight);
  pImage3->SetPosition(nuiBottomLeft);
  pImage4->SetPosition(nuiBottomRight);
  
  AddChild(pImage1);
  AddChild(pImage2);
  AddChild(pImage3);
  AddChild(pImage4);
  
  nuiFixed* pBox = new nuiFixed();
  
  AddChild(pBox);
  
  nuiButton *aButton=new nuiButton(_T("Click test"));
  aButton->AddMatrixNode(new nuiMatrixNode_Translation(100,150,0));
  aButton->AddMatrixNode(new nuiMatrixNode_Pivot(20,-20,-20,0,0,0,1));
  aButton->AddMatrixNode(new nuiMatrixNode_Scale(3.0,3.0,1.0));
  
  aButton->SetAutoClip(false);
  pBox->AddChild(aButton);
}







void MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
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
