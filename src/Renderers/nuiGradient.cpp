/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

// nuiGradient.cpp

#include "nui.h"

nuiGradient::nuiGradient()
{
}

nuiGradient::nuiGradient(const nuiGradient& rGradient)
{
  mStops.assign(rGradient.GetStopList().begin(), rGradient.GetStopList().end());
}

nuiGradient::~nuiGradient()
{
}

static bool PositionIsLesser(const nuiGradientPair& rElem1, const nuiGradientPair& rElem2)
{
  return rElem1.first < rElem2.first;
}

void nuiGradient::AddStop(const nuiColor& rColor, nuiSize position)
{
  mStops.push_back(nuiGradientPair(position, rColor));
  mStops.sort(PositionIsLesser);
}

const nuiGradientStopList& nuiGradient::GetStopList() const
{
  return mStops;
}

const nuiColor nuiGradient::GetColorAt(nuiSize position) const
{
  if (mStops.size() == 0)
  {
    return nuiColor(0.f, 0.f, 0.f);
  }
  
  nuiGradientStopList::const_iterator stopIt = mStops.begin();
  nuiGradientStopList::const_iterator stopEnd = mStops.end();
  if (position <= (*stopIt).first)
  {
    return (*stopIt).second;
  }
  
  bool found = false;
  nuiGradientPair leftStop;
  for (; stopIt != stopEnd && !found; ++stopIt)
  {
    if (position > (*stopIt).first)
    {
      found = true;
      leftStop = *stopIt;
    }
  }
  
    if (stopIt == stopEnd)
    {
      return leftStop.second;
    }
    else
    {
      nuiGradientPair rightStop = *stopIt;
      float ratio = (position - leftStop.first) / (rightStop.first - leftStop.first);
      float red = ((rightStop.second.Red() - leftStop.second.Red()) * ratio) + leftStop.second.Red();
      float green = ((rightStop.second.Green() - leftStop.second.Green()) * ratio) + leftStop.second.Green();
      float blue = ((rightStop.second.Blue() - leftStop.second.Blue()) * ratio) + leftStop.second.Blue();
      float alpha = ((rightStop.second.Alpha() - leftStop.second.Alpha()) * ratio) + leftStop.second.Alpha();
      return nuiColor(red, green, blue, alpha);
    }
}

nuiTexture* nuiGradient::CreateTexture(int32 size, nglImagePixelFormat format) const
{
  //  int32 bpp = 32;
  //  switch (format)
  //  {
  //    case eImagePixelRGB:
  //      {
  //        bpp = 24;
  //      }
  //      break;
  //    case eImagePixelRGBA:
  //      {
  //        bpp = 32;
  //      }
  //      break;
  //    case eImagePixelAlpha:
  //    case eImagePixelLum:
  //      {
  //        bpp = 8;
  //      }
  //      break;
  //    case eImagePixelLumA:
  //      {
  //        bpp = 16;
  //      }
  //      break;
  //  }
  
  format = eImagePixelRGBA;  
  nglImageInfo imageinfo(size, 1, 32);
  imageinfo.mPixelFormat = format;
  imageinfo.AllocateBuffer();
  
  uint32* pBuffer = (uint32*)imageinfo.mpBuffer;

  nuiGradientStopList::const_iterator it = mStops.begin();
  nuiGradientStopList::const_iterator end = mStops.end();
  
  if (it == end)
    memset(pBuffer, 0, size * 4);
  float pos0 = 0;
  nuiColor col0 = it->second;
  
  
  while (it != end)
  {
    float pos1 = it->first;
    nuiColor col1 = it->second;
    
    int32 ipos0 = ToBelow(pos0 * size);
    int32 ipos1 = ToBelow(pos1 * size);
    
    float r = 0;
    const float incr = 1.0f / (ipos1 - ipos0);
    for (int32 i = ipos0; i < ipos1; i++)
    {
      nuiColor col(col1);
      col.Mix(col0, r);
      r += incr;
      *pBuffer = col.GetRGBA();
      pBuffer++;
    }
    
    pos0 = pos1;
    col0 = col1;
    
    ++it;
  }
  
  if (pos0 < 1)
  {
    // fill to the end of the line:
    float pos1 = 1;
    
    int32 ipos0 = ToBelow(pos0 * size);
    int32 ipos1 = ToBelow(pos1 * size);
    
    float r = 0;
    const float incr = 1.0f / (ipos1 - ipos0);
    for (int32 i = ipos0; i < ipos1; i++)
    {
      *pBuffer = col0.GetRGBA();
      pBuffer++;
    }
  }
  
  nglImage* pImage = new nglImage(imageinfo);
  nuiTexture* pTexture = nuiTexture::GetTexture(pImage, true);
  return pTexture;
}

//////////////

nuiReflection::nuiReflection(float Intensity, float Center)
{
  mColor = nuiColor(1.f,1.f,1.f, Intensity);
  mCenter = Center;
  mpSkyGradient = NULL;
  mpGroundGradient = NULL;
  mRecalc = true;
}

nuiReflection::~nuiReflection()
{
  delete mpSkyGradient;
  delete mpGroundGradient;
}

void nuiReflection::Draw(nuiDrawContext* pContext, const nuiRect& rRect, nuiShape* pShp)
{
  if (!(mRect == rRect))
    mRecalc = true;

  mRect = rRect;
  float x = mRect.Left();
  float y = mRect.Top();

  float h = mRect.GetHeight();
  if (mRecalc)
    Recalc();

  // Draw the sky:
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->DrawGradient(*mpSkyGradient, mRect, x, y, x, y+mCenter * h);


  // Draw the ground:
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->DrawGradient(*mpGroundGradient, mRect, x, y + mCenter * h, x, y + h);
}

void nuiReflection::Recalc()
{
  float x = mRect.Left();
  float y = mRect.Top();
  float w = mRect.GetWidth();
  float h = mRect.GetHeight();
  float h_2 = h * mCenter;

  mRecalc = false;

  delete mpSkyGradient;
  delete mpGroundGradient;
  mpSkyGradient = new nuiGradient();
  mpGroundGradient = new nuiGradient();

  nuiColor tmp;
  
  // Sky:
  tmp = mColor;
  float alpha = mColor.Alpha();
  tmp.SetOpacity(alpha * .2f);
  tmp.Crop();
  mpSkyGradient->AddStop(tmp, 0); 

  mpSkyGradient->AddStop(nuiColor(1.f,1.f,1.f, alpha), 0.1f); 

  tmp.SetOpacity(alpha * .6f);
  tmp.Crop();
  mpSkyGradient->AddStop(tmp, .4f); 

  tmp.SetOpacity(alpha * .4f);
  tmp.Crop();
  mpSkyGradient->AddStop(tmp, 1); 

  // Ground:
  mpGroundGradient = new nuiGradient();
  tmp.SetOpacity(alpha * .1f);
  tmp.Crop();
  mpGroundGradient->AddStop(tmp, 0); 

  tmp.SetOpacity(alpha * .4f);
  tmp.Crop();
  mpGroundGradient->AddStop(tmp, .8f); 

  tmp.SetOpacity(alpha *.6f);
  tmp.Crop();
  mpGroundGradient->AddStop(tmp, 1); 
}

