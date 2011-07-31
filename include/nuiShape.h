/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiShape_h__
#define __nuiShape_h__

//#include "nui.h"
#include "nuiFlags.h"
#include "nuiPathGenerator.h"
#include "nuiSpline.h"

class nuiRenderArray;
class nuiRenderObject;
class nuiContour;

enum nuiShapeMode
{
  eStrokeShape=0,
  eFillShape,
  eStrokeAndFillShape,
  eDefault
};

class NUI_API nuiShape : public nuiPathGenerator
{
public:
  nuiShape();
  virtual ~nuiShape();

  void Clear();

  /** @name Adding special contours */
  //@{
  void AddCircle(float X, float Y, float Radius, bool CCW = false, float Quality = 0.5f);
  void AddEllipse(float X, float Y, float XRadius, float YRadius, bool CCW = false, float Quality = 0.5f);
  void AddRoundRect(const nuiRect& rRect, float Radius, bool CCW = false, float Quality = 0.5f);
  void AddRect(const nuiRect& rRect, bool CCW = false);
  void AddArc(float X, float Y, float rX, float rY, float Theta1InDegree, float Theta2InDegree, float Phi, float Quality = 0.5f); ///< Add an ellipse arc from a center/radii/angles description instead of the SVG start vertex/end vertex one.
  //@}

  /** @name Adding elements to the last added countour */
  //@{
  void ArcTo(float X, float Y, float XRadius, float YRadius, float Angle, bool LargeArc, bool Sweep);
  void AddLines(const nuiPath& rVertices); ///< Add the given lines to the current contour. Only create a new contour if there is none in the shape.
  void AddPath(const nuiPath& rVertices); ///< This method add the given points to the shape, creating new contours accodring the types of the nuiPoints encountered.
  void LineTo(const nuiPoint& rVertex);
  void AddSpline(const nuiSpline& rSpline);
  void AddPathGenerator(nuiPathGenerator* pPath);
  //@}

  /** @name Contour management */
  //@{
  void AddContour(nuiContour* pContour);
  void AddContour();
  nuiContour* GetContour(uint Index) const;
  uint GetContourCount() const;
  void CloseContour();
  //@}

  nuiRect GetRect();

  virtual bool Tessellate(nuiPath& rVertices, float Quality = 0.5f) const;
  virtual nuiPoint GetStartPoint() const;
  virtual nuiPoint GetEndPoint() const;

  nuiRenderObject* Fill(float Quality);
  nuiRenderObject* Outline(float Quality, float LineWidth, nuiLineJoin LineJoin = nuiLineJoinMiter, nuiLineCap LineCap = nuiLineCapBut, float MiterLimit = 0.5f);

  enum Winding 
  {
    eNone, ///< In this case the default winding rule is used.
    eOdd        = GLU_TESS_WINDING_ODD,
    eNonZero    = GLU_TESS_WINDING_NONZERO,
    ePositive   = GLU_TESS_WINDING_POSITIVE,
    eNegative   = GLU_TESS_WINDING_NEGATIVE,
    eAbsGEqTwo  = GLU_TESS_WINDING_ABS_GEQ_TWO
  };

  Winding GetWinding() const; ///< Set the Winding rule of this shape. The default winding rule is set to eNone (it will use the active winding rule of the draw context).
  void SetWinding(Winding Rule); ///< Get the Winding rule of this shape. 

  void EmptyCaches();

  //nuiSimpleEventSource<nuiChanged> Changed; ///< This event is fired each time the Shape is changed.

  float GetDistanceFromPoint(float X, float Y, float Quality = 0.5f) const;
  
private:
  std::vector<nuiContour*> mpContours;
  nuiShape(const nuiShape& rShape);
  nuiShape& operator=(const nuiShape& rShape);

  Winding mWinding;

  void ElementChanged(const nuiEvent& rEvent);
  nuiEventSink<nuiShape> mEventSink;
};


#endif // __nuiShape_h__
