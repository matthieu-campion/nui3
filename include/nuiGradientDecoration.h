/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiDecoration.h"



class nuiGradientDecoration : public nuiDecoration
{
public:

  enum GradientType
  {
    nuiGradient2Colors=0,
    nuiGradient4Colors
  };

  nuiGradientDecoration(const nglString& rName);
  
  // stroke parameters as default parameters
  nuiGradientDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rColor1, const nuiColor& rColor2, nuiOrientation orientation = nuiVertical, uint32 strokeSize=0, const nuiColor& rStrokeColor = nuiColor(0,0,0), nuiShapeMode shapeMode = eFillShape);
  nuiGradientDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rColor1, const nuiColor& rColor2, const nuiColor& rColor3, const nuiColor& rColor4, nuiOrientation orientation = nuiVertical, uint32 strokeSize=0, const nuiColor& rStrokeColor = nuiColor(0,0,0), nuiShapeMode shapeMode = eFillShape);

  // clientRect as default parameters
  nuiGradientDecoration(const nglString& rName, const nuiColor& rColor1, const nuiColor& rColor2, uint32 strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode = eFillShape, nuiOrientation orientation = nuiVertical, const nuiRect& rClientRect=nuiRect(0,0,0,0));
  nuiGradientDecoration(const nglString& rName, const nuiColor& rColor1, const nuiColor& rColor2, const nuiColor& rColor3, const nuiColor& rColor4, uint32 strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode = eFillShape, nuiOrientation orientation = nuiVertical, const nuiRect& rClientRect=nuiRect(0,0,0,0));
  
  void Init();
  
  virtual ~nuiGradientDecoration();
  
  virtual nuiSize GetBorder(nuiPosition position, const nuiWidget* pWidget) const;
  virtual void GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const;
  virtual nuiRect GetIdealClientRect(const nuiWidget* pWidget) const;
  
  void SetSourceClientRect(const nuiRect& rRect);
  const nuiRect& GetSourceClientRect() const;
    
	void GetColors(nuiColor& color1, nuiColor& color2) const;
	void SetColors(const nuiColor& color1, const nuiColor& color2);

  void GetColors(nuiColor& color1, nuiColor& color2, nuiColor& color3, nuiColor& color4) const;
  void SetColors(const nuiColor& color1, const nuiColor& color2, const nuiColor& color3, const nuiColor& color4);
  
  void SetOrientation(nuiOrientation orientation);
  nuiOrientation GetOrientation() const;
  
  const nuiColor& GetColor1() const;
	void SetColor1(const nuiColor& color1);
  const nuiColor& GetColor2() const;
	void SetColor2(const nuiColor& color1);
  const nuiColor& GetColor3() const;
	void SetColor3(const nuiColor& color1);
  const nuiColor& GetColor4() const;
	void SetColor4(const nuiColor& color1);
  const nuiColor& GetStrokeColor() const;
	void SetStrokeColor(const nuiColor& color);
  
  void InitOffsets();
  double GetOffset1() const;
  double GetOffset2() const;
  double GetOffset3() const;
  double GetOffset4() const;
  void SetOffset1(double offset);
  void SetOffset2(double offset);
  void SetOffset3(double offset);
  void SetOffset4(double offset);

  void SetStrokeSize(uint32 size);
  uint32 GetStrokeSize() const;
  
  void SetShapeMode(nuiShapeMode mode);
  nuiShapeMode GetShapeMode() const;
  
	
protected:

  virtual void Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect);

private:
  GradientType mGradientType;
  nuiColor mColor1;
  nuiColor mColor2;
  nuiColor mColor3;
  nuiColor mColor4;
  double mOffset1;
  double mOffset2;
  double mOffset3;
  double mOffset4;
  bool mUserOffsets;
  nuiRect mClientRect;
  uint32 mStrokeSize;
  nuiColor mStrokeColor;
  nuiShapeMode mShapeMode;
  nuiOrientation mOrientation;
  

  void InitAttributes();
};

