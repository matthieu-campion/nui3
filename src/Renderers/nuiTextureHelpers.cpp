/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

void nuiRadialBlur(nuiDrawContext* pContext, nuiTexture* pTexture, const nuiRect& rDestinationBase, float ScaleCoef, const nuiVector2& Direction, uint Iterations, float Strength, float PreMultiplyAlpha)
{
  nuiRect Current = rDestinationBase, Source;
  //float tx = 1.0f,ty = 1.0f;

  nglImage* pImage = pTexture->GetImage();

  Source.Set(0.0f,0.0f,(nuiSize)pImage->GetWidth(),(nuiSize)pImage->GetHeight());
  nuiSize w,h, nw,nh, x, y;
  x = Current.Left();
  y = Current.Top();
  nw = w = Current.GetWidth();

  nh = h = Current.GetHeight();
  float cw = nw * ScaleCoef;
  float ch = nh * ScaleCoef;

  pContext->SetTexture(pTexture);
  pContext->EnableBlending(true);
  pContext->EnableTexturing(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pTexture->SetEnvMode(GL_MODULATE);

  float a = 1;
  uint i = Iterations;

  float ratio = (1.0f / Iterations);
  while (i)
  {
    nuiVector2 v = Direction;
    float r = ((float)Iterations-(float)i) * ratio;
    v *= r;
    nw = (1.0f - r) * w + r * cw;
    nh = (1.0f - r) * h + r * ch;
    Current.Set(v[0] + x - ((nw - w)/2.0f), v[1] + y - ((nh - h)/2.0f), nw, nh);

    pContext->SetFillColor(nuiColor(1.f,1.f,1.f,PreMultiplyAlpha * a));
    pContext->DrawImage(Current, Source);
    a *= Strength;

    i--;
  }

/*
  pContext->SetStrokeColor(nuiColor(1,0,0));
  pContext->DrawRect(rDestinationBase, eStrokeShape);
  pContext->SetStrokeColor(nuiColor(0,0,0));
  pContext->DrawRect(Current, eStrokeShape);
*/
}

