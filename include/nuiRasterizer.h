/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nui.h"

class nuiRasterizer
{
private:  

  template <class VertexType>
  void ClipSegmentX(VertexType& rStart, VertexType& rEnd, const VertexType& rIncr)
  {
    ifp32 xdiff = rStart.X() - mClipX0;
    if (xdiff < 0)
    {
      VertexType t = rIncr;
      t.Mul(-xdiff);
      rStart.Add(t);
    }

    xdiff = mClipX1 - rEnd.X();
    if (xdiff < 0)
    {
      VertexType t = rIncr;
      t.Mul(xdiff);
      rEnd.Add(t);
    }
  }

  template <class VertexType>
  bool ClipSegmentY(VertexType& rStart, VertexType& rEnd, const VertexType& rIncr)
  {
    if (rEnd.Y() < mClipY0 || rStart.Y() > mClipY1)
      return false;
    
    ifp32 ydiff = rStart.Y() - mClipY0;
    if (ydiff < 0)
    {
      VertexType t(rIncr);
      t.Mul(-ydiff);
      rStart.Add(t);
    }

    ydiff = mClipY1 - rEnd.Y();
    if (ydiff < 0)
    {
      VertexType t(rIncr);
      t.Mul(ydiff);
      rEnd.Add(t);
    }
    
    return true;
  }

  template <class VertexType, bool AcceptHorizontalLines>
  bool SetupSegment(VertexType& r0, VertexType& r1, VertexType& rIncr)
  {
    int32 height  = r1.Y() - r0.Y();
    
    if (height == 0)
    {
      if (AcceptHorizontalLines)
      {
        rIncr.Clear();
        return ClipSegmentY(r0, r1, rIncr);
      }
      else
      {
        return false;
      }
    }
    
    rIncr = r1;
    rIncr.Sub(r0);
    rIncr.Div(height);

    rIncr.SetY(NUI_FP_ONE);

    if (!ClipSegmentY(r0, r1, rIncr))
      return false;
    
    int32 diff = (r0.CeiledY() << NUI_FP_SHIFT) - r0.Y();
    VertexType temp(rIncr);
    temp.Mul(diff);
    r0.Add(temp);
    
    return true;
  }
  
  template <class PixelBlender, class VertexType>
  void DrawSection(VertexType& Left0, VertexType& Left1, VertexType& LeftIncr, VertexType& Right0, VertexType& Right1, VertexType& RightIncr)
  {
    int32 start = ToAbove(MAX(Left0.Y(), Right0.Y()));
    int32 end = ToAbove(MIN(Left1.Y(), Right1.Y()));
    int32 y = start;
    
    while (y < end)
    {
      DrawHLine<PixelBlender>(Left0, Right0);
      Left0.Add(LeftIncr);
      Right0.Add(RightIncr);
      y++;
    }
  }
  
  template <class PixelBlender, class VertexType>
  void DrawLineSection(VertexType& Start, VertexType& End, VertexType& Incr)
  {
    int32 start = Start.CeiledY();
    int32 end = End.CeiledY();
    int32 y = start;
    
    if (y == end) // Horizontal
    {
      End.SetX(End.X() + NUI_FP_ONE);
      DrawHLine<PixelBlender>(Start, End);
    }
    else
    {
      VertexType Left(Start);
      VertexType Right(Start);
      
      if (Incr.X() != 0)
      {
        double c = Incr.X();
        c /= (double)NUI_FP_ONE;
        c *= c;
        c = sqrt(c);
        c *= (double)NUI_FP_ONE;
        Right.SetX(Right.X() + ToNearest(c));
      }
      else
      {
        Right.SetX(Right.X() + NUI_FP_ONE - 1);
      }

      while (y <= end)
      {
        DrawHLine<PixelBlender>(Left, Right);
        Left.Add(Incr);
        Right.Add(Incr);
        y++;
      }
    }
  }
  

  template <class PixelBlender, class VertexType>
  void DrawHLine(VertexType& Left, VertexType& Right)
  {
    VertexType v0(Left);
    VertexType v1(Right);
    int32 width = v1.X() - v0.X();
    if (!width)
      return;
    
    VertexType incr(v1);
    incr.Sub(v0);
    incr.Div(width);
    
    incr.SetX(NUI_FP_ONE);
    incr.SetY(0);
    
    ClipSegmentX(v0, v1, incr);
    
    int32 y = v0.CeiledY();
    int32 x = ToAbove(v0.X());
    int32 end = ToAbove(v1.X());

    width = end - x;
    if (width <= 0)
      return;
    
    uint32* pBuffer = mpBuffer + (y * mWidth + x);

    incr.DrawHLine<PixelBlender>(pBuffer, v0, width);
  }
  
public:
  nuiRasterizer(uint32 Width, uint32 Height, uint32* pBuffer = NULL)
  {
    mWidth = Width;
    mHeight = Height;
    mpBuffer = NULL;
    mpBufferVector = NULL;
    
    mClipX0 = 0;
    mClipY0 = 0;
    mClipX1 = 0;
    mClipY1 = 0;
    
    SetClipRect(0, 0, Width, Height);
    Resize(Width, Height, pBuffer);
  }
  
  ~nuiRasterizer()
  {
    delete mpBufferVector;
  }
  
  void SetClipRect(int32 X0, int32 Y0, int32 X1, int32 Y1)
  {
    mClipX0 = X0 << NUI_FP_SHIFT;
    mClipY0 = Y0 << NUI_FP_SHIFT;
    mClipX1 = X1 << NUI_FP_SHIFT;
    mClipY1 = Y1 << NUI_FP_SHIFT;
  }
  
  void ClearColor(uint32 color)
  {
    if (
        (mClipX0 == 0) &&
        (mClipY0 == 0) &&
        (ToBelow(mClipX1) == mWidth) &&
        (ToBelow(mClipY1) == mHeight)
       )
    {
      for (int32 i = 0; i < mWidth * mHeight; i++)
      {
        mpBuffer[i] = color;
      }
    }
    else
    {
      int32 height = ToBelow(mClipY1 - mClipY0);
      int32 width = ToBelow(mClipX1 - mClipX0);
      uint32* pBuffer = mpBuffer + ToBelow(mClipY0) * mWidth + ToBelow(mClipX0);
      for (int32 i = 0; i < height; i++)
      {
        for (int32 j = 0; j < width; j++)
        {
          pBuffer[j] = color;
        }
        pBuffer += mWidth;
      }
    }
  }  
  
  void ClearStencil(uint8 value)
  {
    if (
        (mClipX0 == 0) &&
        (mClipY0 == 0) &&
        (ToBelow(mClipX1) == mWidth) &&
        (ToBelow(mClipY1) == mHeight)
        )
    {
      memset(&mStencilBuffer[0], value, mWidth * mHeight * sizeof(uint8));
    }
    else
    {
      int32 height = ToBelow(mClipY1 - mClipY0);
      int32 width = ToBelow(mClipX1 - mClipX0);
      uint8* pBuffer = &mStencilBuffer[0] + ToBelow(mClipY0) * mWidth + ToBelow(mClipX0);
      for (int32 i = 0; i < height; i++)
      {
        memset(pBuffer, value, width * sizeof(uint8));
        pBuffer += mWidth;
      }
    }
  }  

  template <class PixelBlender, class VertexType>
  void DrawLine(const VertexType& rVertex0, const VertexType& rVertex1)
  {    
    std::vector<VertexType> Line;
    Line.push_back(rVertex0);
    Line.push_back(rVertex1);
    
    std::sort(Line.begin(), Line.end(), &nuiSortVertexPointers<VertexType>);
    
    VertexType Start(Line[0]);
    VertexType End(Line[1]);
    VertexType Incr;
    
    bool clipped = false;
    
    clipped |= !SetupSegment<VertexType, true>(Start, End, Incr);
    
    if (!clipped)
      DrawLineSection<PixelBlender>(Start, End, Incr);
  }  
  
  template <class PixelBlender, class VertexType>
  void DrawTriangle(const VertexType& rVertex0, const VertexType& rVertex1, const VertexType& rVertex2)
  {
    /*
     
     4 possible cases:
     0) 0 -> 2 and 1 -> 2

     1) 0 -> 1 and 0 -> 2
     
     2) top: 0 -> 1 and 0 -> 2
        bot: 1 -> 2 and 0 -> 2
     
     3) top: 0 -> 2 and 0 -> 1
        bot: 0 -> 2 and 1 -> 2
     
     */    
    
    bool fillbottom = true;
    
    std::vector<VertexType> Triangle;
    Triangle.clear();
    Triangle.push_back(rVertex0);
    Triangle.push_back(rVertex1);
    Triangle.push_back(rVertex2);

    std::sort(Triangle.begin(), Triangle.end(), &nuiSortVertexPointers<VertexType>);
    
    if (Triangle[0].CeiledY() == Triangle[2].CeiledY())
      return;

    int topl = 0;
    int topr = 0;
    int midl = 0;
    int midr = 0;
    int botl = 0;
    int botr = 0;

    if (Triangle[0].Y() == Triangle[1].Y())
    {
      fillbottom = false;
      topl = 0;
      midl = 2;
      topr = 1;
      midr = 2;
    }
    else if (Triangle[1].Y() == Triangle[2].Y())
    {
      fillbottom = false;
      topl = 0;
      midl = 1;
      topr = 0;
      midr = 2;
    }
//     else if (Triangle[1].X() < Triangle[2].X())
//     {
//       topl = 0;
//       midl = 1;
//       botl = 2;
//       topr = 0;
//       midr = 2;
//       botr = 2;
//     }
    else
    {
      topl = 0;
      midl = 2;
      botl = 2;
      topr = 0;
      midr = 1;
      botr = 2;
    }
    
    VertexType Left0(Triangle[topl]);
    VertexType Left1(Triangle[midl]);
    VertexType LeftIncr;
    
    VertexType Right0(Triangle[topr]);
    VertexType Right1(Triangle[midr]);
    VertexType RightIncr;
    
    bool clipped = false;
    
    clipped |= !SetupSegment<VertexType, false>(Left0, Left1, LeftIncr);
    clipped |= !SetupSegment<VertexType, false>(Right0, Right1, RightIncr);

    if (!((Left0.X() <= Right0.X()) && (Left1.X() <= Right1.X())))
    {
      std::swap(Left0, Right0);
      std::swap(Left1, Right1);
      std::swap(LeftIncr, RightIncr);
      std::swap(topl, topr);
      std::swap(midl, midr);
      std::swap(botl, botr);
    }
    
    if (!clipped)
      DrawSection<PixelBlender>(Left0, Left1, LeftIncr, Right0, Right1, RightIncr);

    if (!fillbottom)
      return;
    
    clipped = false;
    
    if (midr != botr)
    {
      Right0 = Triangle[midr];
      Right1 = Triangle[botr];
      clipped = !SetupSegment<VertexType, false>(Right0, Right1, RightIncr);
    }
    else
    {
      Left0 = Triangle[midl];
      Left1 = Triangle[botl];
      clipped = !SetupSegment<VertexType, false>(Left0, Left1, LeftIncr);
    }
        
    if (Left0.Y() > Left1.Y() || Right0.Y() > Right1.Y())
      return;
//    if ((Left0.X() > Right0.X()) || (Left1.X() > Right1.X()))
//      return;
//    
//    NGL_ASSERT(Left0.X() <= Right0.X());
//    NGL_ASSERT(Left1.X() <= Right1.X());
//     NGL_ASSERT(Left0.Y() == Right0.Y());
//     NGL_ASSERT(Left1.Y() == Right1.Y());

    if (!clipped)
      DrawSection<PixelBlender>(Left0, Left1, LeftIncr, Right0, Right1, RightIncr);
  }
      
  
  template <class PixelBlender, class VertexType>
  void DrawRectangle(const VertexType& rVertex0, const VertexType& rVertex1, const VertexType& rVertex2, const VertexType& rVertex3)
  {
    std::vector<VertexType> Rectangle;
    Rectangle.clear();
    Rectangle.push_back(rVertex0);
    Rectangle.push_back(rVertex1);
    Rectangle.push_back(rVertex2);
    Rectangle.push_back(rVertex3);
    
    std::sort(Rectangle.begin(), Rectangle.end(), &nuiSortVertexPointers<VertexType>);
        
    VertexType Left0(Rectangle[0]);
    VertexType Left1(Rectangle[2]);
    VertexType LeftIncr;
    
    VertexType Right0(Rectangle[1]);
    VertexType Right1(Rectangle[3]);
    VertexType RightIncr;
    
    bool clipped = false;
    
    clipped |= !SetupSegment<VertexType, false>(Left0, Left1, LeftIncr);
    clipped |= !SetupSegment<VertexType, false>(Right0, Right1, RightIncr);
    
    if (!clipped)
      DrawSection<PixelBlender>(Left0, Left1, LeftIncr, Right0, Right1, RightIncr);
  }
  
  void Resize(int32 width, int32 height, uint32* pBuffer = NULL)
  {
    mWidth = width;
    mHeight = height;
    
    mpBuffer = pBuffer;
    if (!mpBuffer)
    {
      if (!mpBufferVector)
        mpBufferVector = new std::vector<uint32>;

      mpBufferVector->resize(width * height);
      mpBuffer = &(*mpBufferVector)[0];
    }
    else
    {
      delete mpBufferVector;
      mpBufferVector = NULL;
    }
    
    mStencilBuffer.resize(width * height);
  }
  
  uint32* GetBuffer() const
  {
    return mpBuffer;
  }
  
  void ReverseBufferABGR()
  {
    for (int32 i = 0; i < mWidth * mHeight; i++)
    {
      uint32 col = mpBuffer[i];
      uint8* pSrc = (uint8*)(&col);
      uint8* pDst = (uint8*)(mpBuffer + i);
      
      pDst[0] = pSrc[3];
      pDst[1] = pSrc[2];
      pDst[2] = pSrc[1];
      pDst[3] = pSrc[0];
    }
  }
  
  void ReverseBufferBGRA()
  {
    for (int32 i = 0; i < mWidth * mHeight; i++)
    {
      uint32 col = mpBuffer[i];
      uint8* pSrc = (uint8*)(&col);
      uint8* pDst = (uint8*)(mpBuffer + i);
      
      pDst[0] = pSrc[2];
      pDst[1] = pSrc[1];
      pDst[2] = pSrc[0];
      pDst[3] = pSrc[3];
    }
  }
  
protected:
  
  uint32* mpBuffer;
  std::vector<uint32>* mpBufferVector;
  std::vector<uint8> mStencilBuffer;
  int32 mWidth;
  int32 mHeight;
  
  int32 mClipX0;
  int32 mClipY0;
  int32 mClipX1;
  int32 mClipY1;
  
};

