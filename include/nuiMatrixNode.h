/*
 *  nuiMatrixNode.h
 *  nui3
 *
 *  Created by Sébastien Métrot on 10/15/10.
 *  Copyright 2010 libNUI. All rights reserved.
 *
 */

#pragma once

#include "nui.h"
#include "nuiEvent.h"

class nuiMatrixNode : public nuiObject
{
public:
  nuiMatrixNode();
  nuiMatrixNode(const nuiMatrix& rMatrix);
  virtual ~nuiMatrixNode();
  
  virtual void Apply(nuiMatrix& rMatrix) const; ///< Override this method to create your own matrix node operations
  virtual void Update() const; ///< Override this method to update the local mMatrix from your own params.

  const nuiMatrix& GetMatrix() const;
  void SetMatrix(const nuiMatrix& rMatrix);

  nuiSimpleEventSource<0> Changed;
protected:
  mutable nuiMatrix mMatrix;
  mutable bool mChanged;
  void SetChanged();
  
private:
  void Init();
};

class nuiMatrixNode_Rotation : public nuiMatrixNode
{
public:
  nuiMatrixNode_Rotation();
  nuiMatrixNode_Rotation(float Angle, const nglVectorf& rAxis);
  nuiMatrixNode_Rotation(float Angle, float Xaxis, float Yaxis, float Zaxis);
  
  virtual void Update() const;
  
  void SetRotation(float Angle, const nglVectorf& rAxis);
  void SetRotation(float Angle, float Xaxis, float Yaxis, float Zaxis);

  void SetAngle(float Angle);
  float GetAngle() const;
  void SetAxis(const nglVectorf& rAxis);
  const nglVectorf& GetAxis() const;
  
protected:
  void Init();
  float mAngle;
  nglVectorf mAxis;
  
};

class nuiMatrixNode_Translation : public nuiMatrixNode
{
public:
  nuiMatrixNode_Translation();
  nuiMatrixNode_Translation(float X, float Y, float Z);
  
  virtual void Update() const;

  void Set(float X, float Y, float Z);
  
  void SetVector(const nglVectorf& rVector);
  const nglVectorf& GetVector() const;

  void SetX(float set);
  float GetX() const;
  void SetY(float set);
  float GetY() const;
  void SetZ(float set);
  float GetZ() const;


protected:
  void Init();
  nglVectorf mVector;
};

class nuiMatrixNode_Scale : public nuiMatrixNode
{
public:
  nuiMatrixNode_Scale();
  nuiMatrixNode_Scale(float Scale);
  nuiMatrixNode_Scale(float X, float Y, float Z);
  
  virtual void Update() const;
  
  void Set(float X, float Y, float Z);
  void SetScale(float XYZ);
  float GetScale() const;
  void SetScaleVector(const nglVectorf& rVector);
  const nglVectorf& GetScaleVector() const;
  
  void SetX(float set);
  float GetX() const;
  void SetY(float set);
  float GetY() const;
  void SetZ(float set);
  float GetZ() const;
  
  
protected:
  void Init();
  nglVectorf mVector;
};

class nuiMatrixNode_Pivot : public nuiMatrixNode_Rotation
{
public:
  nuiMatrixNode_Pivot();
  nuiMatrixNode_Pivot(float Angle, const nglVectorf& rPivot, const nglVectorf& rAxis);
  nuiMatrixNode_Pivot(float Angle, float X, float Y, float Z, float Xaxis, float Yaxis, float Zaxis);
  
  virtual void Update() const;
  
  void Set(float Angle, const nglVectorf& rPivot, const nglVectorf& rAxis);
  void Set(float Angle, float X, float Y, float Z, float Xaxis, float Yaxis, float Zaxis);
  
  void SetPivot(const nglVectorf& rPivot);
  const nglVectorf& GetPivot() const;
  
protected:
  void Init();
  nglVectorf mPivot;
};

