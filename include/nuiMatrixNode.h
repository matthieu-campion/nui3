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

protected:
  mutable nuiMatrix mMatrix;
  mutable bool mChanged;
  void Changed();
  
private:
  void Init();
};

class nuiMatrixNode_Rotation :  nuiMatrixNode
{
  nuiMatrixNode_Rotation();

  virtual void Update() const;
  
  void SetRotation(float Angle, const nglVectorf& rAxis);

  void SetAngle(float Angle);
  float GetAngle() const;
  void SetAxis(const nglVectorf& rAxis);
  const nglVectorf& GetAxis() const;
  
protected:
  float mAngle;
  nglVectorf mAxis;
  
};

class nuiMatrixNode_Translation :  nuiMatrixNode
{
  nuiMatrixNode_Translation();
  
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
  nglVectorf mVector;
};

class nuiMatrixNode_Scale :  nuiMatrixNode
{
  nuiMatrixNode_Scale();
  
  virtual void Update() const;
  
  void Set(float X, float Y, float Z);
  
  void SetScale(const nglVectorf& rVector);
  const nglVectorf& GetScale() const;
  
  void SetX(float set);
  float GetX() const;
  void SetY(float set);
  float GetY() const;
  void SetZ(float set);
  float GetZ() const;
  
  
protected:
  nglVectorf mVector;
};

