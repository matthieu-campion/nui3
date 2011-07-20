/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiContour_h__
#define __nuiContour_h__

//#include "nui.h"
#include "nuiPathGenerator.h"
#include "nuiSpline.h"

class NUI_API nuiContour : public nuiPathGenerator
{
public:
  nuiContour();
  virtual ~nuiContour();

  void AddArc(const nuiPoint& rStartVertex, const nuiPoint& rStopVertex, float XRadius, float YRadius, float Angle, bool LargeArc, bool Sweep); ///< Add an arc to the contour in the SVG fashion (start/stop points).
  void AddArc(float cX, float cY, float rX, float rY, float Theta1InDegree, float Theta2InDegree, float Phi); ///< Add an arc to the contour in a center/angle fashion.
  void ArcTo(const nuiPoint& rStopVertex, float XRadius, float YRadius, float AngleInDegree, bool LargeArc, bool Sweep);
  void AddLines(const nuiPath& rVertices);
  void LineTo(const nuiPoint& rVertex);
  void AddRect(const nuiRect& rRect, bool CCW = false);
  void AddPathGenerator(nuiPathGenerator* pPath);

  void AddSpline(const nuiSpline& rSpline);

  void Close();

  virtual bool Tessellate(nuiPath& rVertices, float Quality = 0.5f) const;
  uint GetElementsCount() const;
  const std::list<nuiPathGenerator*>* GetElements() const;

  virtual nuiPoint GetStartPoint() const;
  virtual nuiPoint GetEndPoint() const;

  nuiRect GetRect();


  nuiSimpleEventSource<nuiChanged> Changed; ///< This event is fired each time the Contour is changed.

private:
  std::list<nuiPathGenerator*> mpElements;
  nuiContour(const nuiContour& rContour);
  void ElementChanged(const nuiEvent& rEvent);
  nuiEventSink<nuiContour> mEventSink;
};

#endif // __nuiContour_h__
