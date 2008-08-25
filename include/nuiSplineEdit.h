/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSplineEdit_h__
#define __nuiSplineEdit_h__

#include "nuiWidget.h"
#include "nuiSpline.h"

class NUI_API nuiSplineEdit : public nuiWidget
{
public:
  /** @name Life */
  //@{
  nuiSplineEdit( nuiSpline* pSpline = NULL, bool OwnSpline = true );
  virtual bool Load(const nuiXMLNode* pNode); 
  virtual ~nuiSplineEdit();
  //@}

  /** @name Serialization */
  //@{
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive = false) const;
  //@}

  /** @name Object size management */
  //@{
  virtual nuiRect CalcIdealSize();
  //@}
  
  /** @name Rendering */
  //@{
  virtual bool Draw(nuiDrawContext* pContext); ///< This method asks the object to draw itself. It returns false in case of error. You must call Validate() once in this method if you decide to override it. You must not draw the widget if it is not visible (check the result of IsVisible() before drawing anything but after having called Validate()).
  //@}

  /** @name Incomming mouse events */
  //@{
  // Beware: these three methods receive the mouse coordinates in the repair the object!
  virtual bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved    (nuiSize X, nuiSize Y);
  //@}

  /** @name Spline display properties */
  //@{
  void DisplayTangents(bool display); ///< Enables or disables tangent display in the view.
  bool AreTangentDisplayed(); ///< Returns the display status of the tangents.
  void DisplayControlPoints(bool display); ///< Enables or disables control points display in the view.
  bool AreControlPointsDisplayed(); ///< Returns the display status of the control points.
  void SetStep(float Step = 0.05f); ///< Change the evaluation step (default = 0.05f).
  float GetStep(); ///< Retreive the current evaluation step.
  void SetSplineColor(const nuiColor& rColor); ///< Change color of the spline.
  nuiColor GetSplineColor(); ///< Retreive the color of the spline.
  void SetTangentColor(const nuiColor& rColor); ///< Change color of the tangents.
  nuiColor GetTangentColor(); ///< Retreive the color of the tangents.
  void SetPointColor(const nuiColor& rColor); ///< Change color of the control points.
  nuiColor GetPointColor(); ///< Retreive the color of the control points.
  void SetSelectedPointColor(const nuiColor& rColor); ///< Change color of the selected control points.
  nuiColor GetSelectedPointColor(); ///< Retreive the color of the selected control points.

  //@}

  /** @name Edition Behaviour */
  //@{
  void SetEditable(bool editable); ///< Enable or disable user edition in this widget.
  bool IsEditable(); ///< Return true if the user can edit the spline with the mouse.
  void SetCanAddPoints(bool add); ///< Can the user add points to the spline?
  bool GetCanAddPoints(); ///< Can the user add points to the spline?
  void SetCanDelPoints(bool add); ///< Can the user add points to the spline?
  bool GetCanDelPoints(); ///< Can the user delete points to the spline?
  void SetCanMergePoints(bool add); ///< Can the user add points to the spline?
  bool GetCanMergePoints(); ///< Can the user automagically delete points that are close to their neighbours?
  void SetMergeDistance(float distance = 10.0f); ///< Set the distance at which two consecutive points can be merged (default distance is 10 pixels).
  float GetMergeDistance(); ///< Get the distance at which two consecutive points can be merged.
  //@}


protected:
  bool SplineChanged(const nuiEvent& rEvent); ///< This call back is fired whenever the spline is changed.
  nuiEventSink<nuiSplineEdit> mEventsink;


  nuiSpline* mpSpline;
  bool mOwnSpline;
  bool mClicked;
  bool mEditable;
  bool mEditTangents;
  bool mDisplayTangents;
  bool mDisplayControlPoints;

  bool mCanAddPoints;
  bool mCanDelPoints;
  bool mCanMergePoints;
  float mMergeDistance;

  float mControlPointSize;
  nuiSize mOldX; ///< Last mouse position
  nuiSize mOldY; ///< Last mouse position
  nuiSplineNode* mpSelectedNode;
  float mStep;
  nuiColor mPointColor;
  nuiColor mSelectedPointColor;
  nuiColor mSplineColor;
  nuiColor mTangentColor;
  nuiColor mSelectedTangentColor;
};

#endif // __nuiSplineEdit_h__
