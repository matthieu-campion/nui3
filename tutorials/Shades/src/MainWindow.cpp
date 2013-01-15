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
#include "nuiFontManager.h"
#include "nuiClampedValueAttributeEditor.h"

/*
 * MainWindow
 */

static nuiAttributeEditor* GetBlurAttributeEditor(void* pTarget, nuiAttribute<float>* pAttribute)
{
  return new nuiClampedValueAttributeEditor<float>(nuiAttrib<float>(nuiAttribBase(pTarget, pAttribute)), nuiRange(0, 0, 1, .1, .1, 0));
}

static nuiAttributeEditor* GetBlurAttributeEditor2(void* pTarget, nuiAttribute<float>* pAttribute)
{
  return new nuiClampedValueAttributeEditor<float>(nuiAttrib<float>(nuiAttribBase(pTarget, pAttribute)), nuiRange(0, 0, 10, .1, .1, 0));
}

static nuiAttributeEditor* GetBlurIntAttributeEditor(void* pTarget, nuiAttribute<float>* pAttribute)
{
  return new nuiClampedValueAttributeEditor<float>(nuiAttrib<float>(nuiAttribBase(pTarget, pAttribute)), nuiRange(1, 1, 32, 1, 1, 0));
}


MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  if (SetObjectClass(_T("MainWindow")))
  {
    nuiAttribute<float>* pMeanAttrib = new nuiAttribute<float>
    (nglString(_T("BlurMean")), nuiUnitSize,
     nuiMakeDelegate(this, &MainWindow::GetMean),
     nuiMakeDelegate(this, &MainWindow::SetMean)
     );
    pMeanAttrib->SetEditor(&GetBlurAttributeEditor);
    AddAttribute(pMeanAttrib);
    
    nuiAttribute<float>* pAmountAttrib = new nuiAttribute<float>
    (nglString(_T("BlurAmount")), nuiUnitSize,
     nuiMakeDelegate(this, &MainWindow::GetAmount),
     nuiMakeDelegate(this, &MainWindow::SetAmount)
     );
    pAmountAttrib->SetEditor(&GetBlurAttributeEditor);
    AddAttribute(pAmountAttrib);
    
    nuiAttribute<float>* pVarianceAttrib = new nuiAttribute<float>
    (nglString(_T("BlurVariance")), nuiUnitSize,
     nuiMakeDelegate(this, &MainWindow::GetVariance),
     nuiMakeDelegate(this, &MainWindow::SetVariance)
     );
    pVarianceAttrib->SetEditor(&GetBlurAttributeEditor);
    AddAttribute(pVarianceAttrib);

    nuiAttribute<float>* pDeviationAttrib = new nuiAttribute<float>
    (nglString(_T("BlurDeviation")), nuiUnitSize,
     nuiMakeDelegate(this, &MainWindow::GetDeviation),
     nuiMakeDelegate(this, &MainWindow::SetDeviation)
     );
    pDeviationAttrib->SetEditor(&GetBlurAttributeEditor);
    AddAttribute(pDeviationAttrib);

    {
      nuiAttribute<float>* pAttrib = new nuiAttribute<float>
      (nglString(_T("BlurCountX")), nuiUnitSize,
       nuiMakeDelegate(this, &MainWindow::GetCountX),
       nuiMakeDelegate(this, &MainWindow::SetCountX)
       );
      pAttrib->SetEditor(&GetBlurIntAttributeEditor);
      AddAttribute(pAttrib);
    }
    {
      nuiAttribute<float>* pAttrib = new nuiAttribute<float>
      (nglString(_T("BlurCountY")), nuiUnitSize,
       nuiMakeDelegate(this, &MainWindow::GetCountY),
       nuiMakeDelegate(this, &MainWindow::SetCountY)
       );
      pAttrib->SetEditor(&GetBlurIntAttributeEditor);
      AddAttribute(pAttrib);
    }
    {
      nuiAttribute<float>* pAttrib = new nuiAttribute<float>
      (nglString(_T("BlurScaleX")), nuiUnitSize,
       nuiMakeDelegate(this, &MainWindow::GetScaleX),
       nuiMakeDelegate(this, &MainWindow::SetScaleX)
       );
      pAttrib->SetEditor(&GetBlurAttributeEditor2);
      AddAttribute(pAttrib);
    }
    {
      nuiAttribute<float>* pAttrib = new nuiAttribute<float>
      (nglString(_T("BlurScaleY")), nuiUnitSize,
       nuiMakeDelegate(this, &MainWindow::GetScaleY),
       nuiMakeDelegate(this, &MainWindow::SetScaleY)
       );
      pAttrib->SetEditor(&GetBlurAttributeEditor2);
      AddAttribute(pAttrib);
    }
  }
  
  mClicked = false;
  mMouseX = GetWidth() / 2;
  mMouseY = GetHeight() / 2;
  
  mpSurface1 = NULL;
  mpTexture1 = NULL;
  mpSurface2 = NULL;
  mpTexture2 = NULL;
  mpSurface3 = NULL;
  mpTexture3 = NULL;

  mMean = .95;
  mAmount = 0.1;
  mDeviation = .7;
  mVariance = .28;
  
  mCountX = 16;
  mCountY = 16;
  mScaleX = 1;
  mScaleY = 1;
  
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  LoadCSS(_T("rsrc:/css/main.css"));
  
  nuiVBox* pBox = new nuiVBox;
  AddChild(pBox);
  pBox->AddCell(GetAttribute(_T("BlurMean")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurAmount")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurVariance")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurDeviation")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurCountX")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurScaleX")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurCountY")).GetEditor());
  pBox->AddCell(GetAttribute(_T("BlurScaleY")).GetEditor());
}

MainWindow::~MainWindow()
{
  if (mpSurface1)
    mpSurface1->Release();
  if (mpSurface2)
    mpSurface2->Release();
  if (mpSurface3)
    mpSurface3->Release();
}

void MainWindow::UpdateSurfaces()
{
  int32 w = mRect.GetWidth();
  int32 h = mRect.GetHeight();
  const int32 DIV = 2;
  int32 w2 = w / DIV;
  int32 h2 = h / DIV;
  if (mpSurface1 && mpSurface1->GetWidth() == w && mpSurface1->GetHeight() == h)
    return;
  
  if (mpSurface1)
    mpSurface1->Release();
  if (mpSurface2)
    mpSurface2->Release();
  if (mpSurface3)
    mpSurface3->Release();
  
  mpSurface1 = nuiSurface::CreateSurface(_T("TextSurface"), w, h, eImagePixelRGBA);
  mpSurface2 = nuiSurface::CreateSurface(_T("HBlur"), w2, h2, eImagePixelRGBA);
  mpSurface3 = nuiSurface::CreateSurface(_T("VBlur"), w2, h2, eImagePixelRGBA);
  
  mpTexture1 = mpSurface1->GetTexture();
  mpTexture2 = mpSurface2->GetTexture();
  mpTexture3 = mpSurface3->GetTexture();
}

float Blur(float x, float mean, float deviation, float variance)
{
  if (variance <= 0)
  {
    variance = deviation * deviation;
    if (variance <= 0)
    {
      variance = 0.1;
    }
  }
  return powf(exp(-(((x - mean) * (x - mean)) / ((2 * variance)))), 1 / (deviation * sqrt(2 * M_PI)));
}

bool MainWindow::Draw(nuiDrawContext* pContext)
{  
  UpdateSurfaces();
  
  int32 w = mpSurface1->GetWidth();
  int32 h = mpSurface1->GetHeight();
  int32 w2 = mpSurface2->GetWidth();
  int32 h2 = mpSurface2->GetHeight();
  nuiRect r(w, h);
  nuiRect r2(w2, h2);
  
  // Prepare the first surface:
  mpSurface1->Wipe();
  mpSurface1->ResetState();
  mpSurface1->ResetClipRect();
  mpSurface1->SetStrokeColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface1->SetFillColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface1->LoadMatrix(nglMatrixf());
  mpSurface1->Set2DProjectionMatrix(r);
  
  // clear the surface with transparent black:
  mpSurface1->SetClearColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface1->Clear();  
  
  mpSurface1->SetBlendFunc(nuiBlendTransp);
  mpSurface1->EnableBlending(true);
  
  nuiFontRequest request;
  request.SetName(_T("Helvetica"), 1);
  request.MustHaveSize(250, 1);
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(request); //  nuiFont::GetFont(250);
  printf("Font: %p", pFont);
  mpSurface1->SetFont(pFont);
  mpSurface1->SetTextColor(nuiColor(255, 255, 255, 255, true));
  mpSurface1->DrawText(50, 240, _T("Text"));
  DrawChildren(mpSurface1);
  mpSurface1->SetDirty(true);
  
  // Vertical Blur the first surface into the second:
  // Prepare the first surface:
  mpSurface2->Wipe();
  mpSurface2->ResetState();
  mpSurface2->ResetClipRect();
  mpSurface2->SetStrokeColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface2->SetFillColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface2->LoadMatrix(nglMatrixf());
  mpSurface2->Set2DProjectionMatrix(r2);
  mpSurface2->SetClearColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface2->Clear();  
  mpSurface2->SetBlendFunc(nuiBlendTransp);
  mpSurface2->EnableBlending(true);
  
  mpSurface2->SetClearColor(nuiColor(0,0,0));
  mpSurface2->Clear();
  mpSurface2->nuiDrawContext::SetTexture(mpTexture1);
  mpSurface2->EnableBlending(true);
  mpSurface2->EnableTexturing(true);
  mpSurface2->SetBlendFunc(nuiBlendTransp);
  
  pContext->DrawImage(nuiRect(512, 512), nuiRect(512, 512));
  
  for (int32 i = 0; i < mCountY; i++)
  {
    float a = mAmount * Blur((float)i / (float) mCountY, 0.5, mDeviation, mVariance);
    nuiColor c(a, a, a, a, true);
    mpSurface2->SetFillColor(c);
    mpSurface2->DrawImage(nuiRect(0, (int32)(mScaleY * (i - (mCountY / 2))), w2, h2), r);
  }
  
  // Horizontal Blur the second surface into the third:
  mpSurface3->Wipe();
  mpSurface3->ResetState();
  mpSurface3->ResetClipRect();
  mpSurface3->SetStrokeColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface3->SetFillColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface3->LoadMatrix(nglMatrixf());
  mpSurface3->Set2DProjectionMatrix(r2);
  mpSurface3->SetClearColor(nuiColor(0.0f, 0.0f, 0.0f, 0.0f));
  mpSurface3->Clear();  
  mpSurface3->SetBlendFunc(nuiBlendTransp);
  mpSurface3->EnableBlending(true);

  mpSurface3->SetClearColor(nuiColor(0,0,0));
  mpSurface3->Clear();
  mpSurface3->nuiDrawContext::SetTexture(mpTexture2);
  mpSurface3->EnableBlending(true);
  mpSurface3->EnableTexturing(true);
  mpSurface3->SetBlendFunc(nuiBlendTransp);
  
  for (int32 i = 0; i < mCountX; i++)
  {
    float a = mAmount * Blur((float)i / (float) mCountX, .5, mDeviation, mVariance);
    nuiColor c(a, a, a, a, true);
    mpSurface3->SetFillColor(c);
    mpSurface3->DrawImage(nuiRect((int32)(mScaleX * (i - (mCountX / 2))), 0, w2, h2), r2);
  }
  
  // Draw the final surface on screen:
  
  pContext->SetClearColor(nuiColor(255,255,255));
  pContext->Clear();  
  pContext->SetFillColor(nuiColor(0, 0, 0, 255));
  pContext->EnableBlending(true);
  pContext->EnableTexturing(true);

  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture1);
  pContext->DrawImage(nuiRect(0, 0, w, h), r);

  float a = mMean;
  float b = 1.0f;
  pContext->SetFillColor(nuiColor(b, b, b, a));
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture3);
  pContext->DrawImage(nuiRect(w, h), r2);

  
  pContext->SetFillColor(nuiColor(255,255,255));
  pContext->SetBlendFunc(nuiBlendSource);
  pContext->SetTexture(mpTexture1);
  pContext->DrawImage(nuiRect(0, h/2, w/2, h/2), r);

  pContext->SetBlendFunc(nuiBlendSource);
  pContext->SetTexture(mpTexture2);
  pContext->DrawImage(nuiRect(w/2, h/2, w/2, h/2), r);
  return true;
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

bool MainWindow::MouseClicked(const nglMouseInfo& rInfo)
{
  if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    nuiWidget::Grab();
    return true;
  }
  return false;
}

bool MainWindow::MouseUnclicked(const nglMouseInfo& rInfo)
{
  if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
  {
    mClicked = false;
    nuiWidget::Ungrab();
    return true;
  }
  return false;
}

bool MainWindow::MouseMoved(const nglMouseInfo& rInfo)
{
  if (mClicked)
  {
    mMouseX = rInfo.X;
    mMouseY = rInfo.Y;
    Invalidate();
  }
  return true;
}

float MainWindow::GetMean() const
{
  return mMean;
}

float MainWindow::GetAmount() const
{
  return mAmount;
}

float MainWindow::GetDeviation() const
{
  return mDeviation;
}

float MainWindow::GetVariance() const
{
  return mVariance;
}

void MainWindow::SetMean(float v)
{
  mMean = v;
  Invalidate();
}

void MainWindow::SetAmount(float v)
{
  mAmount = v;
  Invalidate();
}

void MainWindow::SetDeviation(float v)
{
  mDeviation = v;
  Invalidate();
}

void MainWindow::SetVariance(float v)
{
  mVariance = v;
  Invalidate();
}

float MainWindow::GetCountX() const
{
  return mCountX;
}

void MainWindow::SetCountX(float v)
{
  mCountX = v;
  Invalidate();
}

float MainWindow::GetCountY() const
{
  return mCountY;
}

void MainWindow::SetCountY(float v)
{
  mCountY = v;
  Invalidate();
}

float MainWindow::GetScaleX() const
{
  return mScaleX;
}

void MainWindow::SetScaleX(float v)
{
  mScaleX = v;
  Invalidate();
}

float MainWindow::GetScaleY() const
{
  return mScaleY;
}

void MainWindow::SetScaleY(float v)
{
  mScaleY = v;
  Invalidate();
}

