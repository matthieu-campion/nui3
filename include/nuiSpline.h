/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSpline_h__
#define __nuiSpline_h__

//#include "nui.h"
#include "nglMath.h"
#include "nglVector.h"
#include "nglMatrix.h"
#include "nuiEvent.h"
#include "nuiRect.h"
#include "nuiPoint.h"
#include "nuiPathGenerator.h"

class nuiSpline;
class nuiSplineNode;
class nuiSplinePoint;

enum nuiSplineMode
{
  nuiSplineModeFree,
  nuiSplineModeCatmullRom,
  nuiSplineModeCardinal,
  nuiSplineModeTCB,
  nuiSplineModeBezierCubic,
  nuiSplineModeBezierQuadratic
};

class NUI_API nuiSplineNode
{
public:
  /** @name Life */
  //@{
  nuiSplineNode(float x, float y, float z = 0);
  nuiSplineNode(const nuiSplineNode& rNode);
  virtual ~nuiSplineNode();
  //@}

  void SetPosition(const nuiVector& rVector); ///< Change the position of the control point.
  const nuiVector& GetPosition() const; ///< Get the position of the control point.

  /** @name TCB parameters */
  //@{
  void SetTension(float Tension);        ///< Set the tension parameter of this node.
  float GetTension() const;                    ///< Return the bias parameter of this node.
  void SetBias(float Bias);              ///< Set the bias parameter of this node.
  float GetBias() const;                       ///< Return the bias parameter of this node.
  void SetContinuity(float Continuity);  ///< Set the continuity parameter of this node.
  float GetContinuity() const;                 ///< Return the continuity parameter of this node.
  //@}

  /** @name Low level tangent access */
  //@{
  void SetTangent(const nuiVector& rTangent);   ///< Set the tangent parameter of this node (will update both the incoming and outgoing tangents).
  void SetIncomingTangent(const nuiVector& rTangent);   ///< Set the incoming tangent parameter of this node.
  void SetOutgoingTangent(const nuiVector& rTangent);   ///< Set the outgoing tangent parameter of this node.
  nuiVector GetTangent() const;                       ///< Return the tangent parameter of this node (equivalent to GetIncommingTangent).
  nuiVector GetIncomingTangent() const;               ///< Return the incoming tangent parameter of this node.
  nuiVector GetOutgoingTangent() const;               ///< Return the outgoing tangent parameter of this node.
  void RotateTangent(float angle, float x, float y, float z); ///< Rotate both tangents. 
  void RotateIncomingTangent(float angle, float x, float y, float z); ///< Rotate the incoming tangents. 
  void RotateOutgoingTangent(float angle, float x, float y, float z); ///< Rotate the outgoing tangents. 
  void ScaleTangent(float x, float y, float z); ///< Scale both tangents. 
  void ScaleIncomingTangent(float x, float y, float z); ///< Scale the incoming tangents. 
  void ScaleOutgoingTangent(float x, float y, float z); ///< Scale the outgoing tangents. 
  //@}

  /** @name Copy Operator */
  //@{
  nuiSplineNode& operator = (const nuiSplineNode& rNode);  ///< Copy operator.
  const nuiVector& operator = (const nuiVector& rVector); ///< Copy operator.
  //@}

  /** @name Outgoing events */
  //@{
  mutable nuiSimpleEventSource<nuiChanged> Changed; ///< This event is fired each time the Spline is changed.
  //@}

protected:
  nuiVector mPosition;
  mutable nuiVector mTangentIn;
  mutable nuiVector mTangentOut;
  float mTension;
  float mContinuity;
  float mBias;

  void SetTangent(const nuiVector& rTangent) const;
  void SetIncomingTangent(const nuiVector& rTangent) const;
  void SetOutgoingTangent(const nuiVector& rTangent) const;
  friend class nuiSpline;
};


class NUI_API nuiSpline : public nuiPathGenerator
{
public:
  nuiSpline();
  nuiSpline(const nuiSpline& rSpline); ///< Copy constructor.
  virtual ~nuiSpline();

  void InsertNode(const nuiSplineNode& rNode, uint32 index = 0); ///< Insert a Node @param rNode at @param index in the Spline.
  void InsertNodes(std::vector<nuiSplineNode> rNodes, uint32 index = 0); ///< Insert the Nodes in the list @param rNodes at @param index in the Spline.

  void AddNode(const nuiSplineNode& rNode); ///< Add a Node @param rNode at the end of the Spline.
  void AddNodes(std::vector<nuiSplineNode> rNodes); ///< Add the Nodes in the list @param rNodes at the end of the Spline.

  void DeleteNode(uint32 index); ///< Delete the Node at the given @param index.
  void DeleteNodes(uint32 index,uint32 count); ///< Delete @param count Nodes starting at Node @param index.
  void DeleteNodes(std::vector<uint32> indices); ///< Delete the Node whose index is in the list @param indices.
  void DeleteAll(); ///< Delete all Nodes.

  nuiSplineNode* GetNode(uint32 index); ///< Return the Node at index @param index in the Spline.
  nuiSplineNode* GetNode(nuiVector vector, float maxdist = 5 ); ///< Return a reference to the closest Node to the given @param vector.
  const nuiSplineNode* GetNode(uint32 index) const; ///< Return the Node at index @param index in the Spline.
  const nuiSplineNode* GetNode(nuiVector vector, float maxdist = 5 ) const; ///< Return a reference to the closest Node to the given @param vector.
  uint32 GetNodes(std::vector<nuiSplineNode>&rNodes, uint32 index = 0, int32 count = -1) const; ///< Copy @param count Nodes from the index @index in the list to the list @param rNodes. If count == -1 then it will copy all the Nodes. Return the number of Nodes actually copied.

  virtual nuiSplinePoint Evaluate(float Value) const; ///< Evaluate the spline at the given Value. The evaluation goes from trunc(Value) to trunc(Value)+1.
  virtual void Evaluate(std::vector<nuiSplinePoint>& rPoints, uint steps, bool optimize = true) const; ///< Evaluate the spline as steps point and put them in the given list.
  virtual void PrecalcTangents() const; ///< Prepare the spline for evaluation. Call this method at least once before calling Evaludate(float Value). The other Evaluate method calls it directly.

  uint32 GetSize() const; ///< Return the number of Nodes actually contained in the Spline.

  nuiRect GetRect(); ///< Return the bounding rect around the Nodes.

  void Translate(const nuiVector& rTranslation); ///< Translate the Nodes along the given Node.
  void Rotate(float angle, float x, float y, float z);  ///< Rotate the Nodes (see glRotate for the significance of the parameters).
  void Scale(float x, float y, float z);  ///< Scale the Nodes along each axis given the respecting values.
  void Multiply(const nuiMatrix& matrix); ///< Multiply every Node of the Spline by the given matrix.

  nuiSimpleEventSource<nuiChanged> Changed; ///< This event is fired each time the Spline is changed.

  void SetCardinalMode(float tightness); ///< Set the spline in Cardinal mode with the given tightness.
  void SetCatmullRomMode(); ///< Set the spline in Catmull-Rom mode (equivalent to SetCardinalMode(0.5) ).
  void SetTCBMode(); ///< Set the spline in Tension-Continuity-Bias mode. 
  void SetFreeMode(); ///< Set the spline in free mode: the tangents won't be recalculated automatically.
  void SetQuadraticBezierMode(); ///< Set the spline in Bezier mode: the out tangents won't be recalculated automatically and they are in fact the second control points in a quadratic bezier spline.
  void SetCubicBezierMode(); ///< Set the spline in Bezier mode: the tangents won't be recalculated automatically and they are in fact the second and third control points in a cubic bezier spline.
  nuiSplineMode GetMode() const;

  nuiSpline& operator=(const nuiSpline& rSpline);

  virtual bool Tessellate(nuiPath& rVertices, float Quality = 0.5f) const;

  virtual nuiPoint GetStartPoint() const;
  virtual nuiPoint GetEndPoint() const;

protected: 
  std::vector<nuiSplineNode> mNodes;
  nuiSplineMode mMode;
  float mCardinalTightness;

  mutable bool mChanged;
  mutable nuiSimpleEventSource<nuiChanged> InternalChanged; ///< This event is fired each time the Spline is changed internally.
  void ElementChanged(const nuiEvent& rEvent);
  nuiEventSink<nuiSpline> mEventSink;

  void CalculateCardinalTangents(float tightness) const; ///< Calculate the Tangents with the Cardinal algorithm.
  void CalculateCatmullRomTangents() const; ///< Calculate the Tangents with the Cardinal algorithm (cardinal with a tightness of 0.5).
  void CalculateTCBTangents() const; ///< Calculate the Tangents with the TCB algorithm.

private:
  void InvalidateCache(const nuiEvent& rEvent);

  mutable bool mCacheValid;
  mutable float mLastQuality;
  mutable nuiPath mTessCache;
};


class NUI_API nuiSplinePoint: public nuiPoint
{
public:
  nuiSplinePoint() : nuiPoint()
  { 
  }

  /// Initialize with values
  nuiSplinePoint(float X, float Y, float Z, float W = 0.0f) : nuiPoint(X,Y,Z,W)
  {
  }

  /// Initialize with a two dimensional vector. Third element is zero, fourth is one.
  nuiSplinePoint(const nuiVector2& rVector) : nuiPoint(rVector)
  { 
  }

  /// Initialize with a two dimensional vector. Fourth element is one.
  nuiSplinePoint(const nuiVector3& rVector)  : nuiPoint(rVector)
  { 
  }

  /// Initialize with a four dimensional vector.
  nuiSplinePoint(const nuiVector& rVector)  : nuiPoint(rVector)
  { 
  }

  /// Initialize with a four dimensional vector.
  nuiSplinePoint(const nuiSplineNode& rNode)  : nuiPoint(rNode.GetPosition())
  { 
  }

  /// Copy constructor
  nuiSplinePoint(const nuiSplinePoint& rPoint)  : nuiPoint(rPoint)
  {
  }

  /// Destructor
  virtual ~nuiSplinePoint() ///< Destructor.
  {
  }
};

#endif // __nuiSpline_h__
