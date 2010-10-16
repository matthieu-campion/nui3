/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiMatrixNode.h"

//class nuiMatrixNode : public nuiObject
nuiMatrixNode::nuiMatrixNode()
{
  Init();
}

nuiMatrixNode::nuiMatrixNode(const nuiMatrix& rMatrix)
{
  Init();
  mMatrix = rMatrix;
}

nuiMatrixNode::~nuiMatrixNode()
{
  // Nothing special here
}

void nuiMatrixNode::Init()
{
  if (SetObjectClass(_T("nuiMatrixNode")))
  {
    // Create attributes
    AddAttribute(new nuiAttribute<const nuiMatrix&>
                 (nglString(_T("Matrix")), nuiUnitMatrix,
                  nuiMakeDelegate(this, &nuiMatrixNode::GetMatrix),
                  nuiMakeDelegate(this, &nuiMatrixNode::SetMatrix)));
  }
  
  mChanged = false;
}

//void nuiMatrixNode::Apply(nuiMatrix& rMatrix) const = 0; ///< Override this method to create your own matrix node operations

const nuiMatrix& nuiMatrixNode::GetMatrix() const
{
  if (mChanged)
  {
    Update();
    mChanged = false;
  }
  return mMatrix;
}

void nuiMatrixNode::SetMatrix(const nuiMatrix& rMatrix)
{
  mMatrix = rMatrix;
}


void nuiMatrixNode::Apply(nuiMatrix& rMatrix) const
{
  if (mChanged)
  {
    Update();
    mChanged = false;
  }

  rMatrix *= mMatrix;
}

void nuiMatrixNode::Update() const
{
}

void nuiMatrixNode::Changed()
{
  mChanged = true;
}

///////////////////////////////////////
nuiMatrixNode_Rotation::nuiMatrixNode_Rotation()
{
  if (SetObjectClass(_T("nuiMatrixNode_Rotation")))
  {
    // Create attributes
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("Angle")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiMatrixNode_Rotation::GetAngle),
                  nuiMakeDelegate(this, &nuiMatrixNode_Rotation::SetAngle)));

    AddAttribute(new nuiAttribute<const nglVectorf&>
                 (nglString(_T("Axis")), nuiUnitVector,
                  nuiMakeDelegate(this, &nuiMatrixNode_Rotation::GetAxis),
                  nuiMakeDelegate(this, &nuiMatrixNode_Rotation::SetAxis)));
  }    

  mChanged = false;
  mAngle = 0;
  // the default nglVectorf should be ok
}

void nuiMatrixNode_Rotation::Update() const
{
  mMatrix.SetRotation(mAngle, mAxis);
}


void nuiMatrixNode_Rotation::SetRotation(float Angle, const nglVectorf& rAxis)
{
  SetAngle(Angle);
  SetAxis(rAxis);
}

void nuiMatrixNode_Rotation::SetAngle(float Angle)
{
  if (mAngle == Angle)
    return;
  mAngle = Angle;
  Changed();
}

float nuiMatrixNode_Rotation::GetAngle() const
{
  return mAngle;
}

void nuiMatrixNode_Rotation::SetAxis(const nglVectorf& rAxis)
{
  if (mAxis == rAxis)
    return;
  mAxis = rAxis;
  mChanged = true;
}

const nglVectorf& nuiMatrixNode_Rotation::GetAxis() const
{
  return mAxis;
}


/////////////////////////////////////////////
//class nuiMatrixNode_Translation :  nuiMatrixNode
nuiMatrixNode_Translation::nuiMatrixNode_Translation()
{
  if (SetObjectClass(_T("nuiMatrixNode_Translation")))
  {
    AddAttribute(new nuiAttribute<const nglVectorf&>
                 (nglString(_T("Vector")), nuiUnitVector,
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::GetVector),
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::SetVector)));

    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("X")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::GetX),
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::SetX)));
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("Y")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::GetY),
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::SetY)));
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("Z")), nuiUnitVector,
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::GetZ),
                  nuiMakeDelegate(this, &nuiMatrixNode_Translation::SetZ)));
  }
}

void nuiMatrixNode_Translation::Update() const
{
  mMatrix.SetTranslation(mVector);
}


void nuiMatrixNode_Translation::Set(float X, float Y, float Z)
{
  mVector.Set(X, Y, Z);
  Changed();
}

void nuiMatrixNode_Translation::SetVector(const nglVectorf& rVector)
{
  mVector = rVector;
  Changed();
}

const nglVectorf& nuiMatrixNode_Translation::GetVector() const
{
  return mVector;
}

void nuiMatrixNode_Translation::SetX(float set)
{
  mVector[0] = set;
  Changed();
}

float nuiMatrixNode_Translation::GetX() const
{
  return mVector[0];
}


void nuiMatrixNode_Translation::SetY(float set)
{
  mVector[1] = set;
  Changed();
}

float nuiMatrixNode_Translation::GetY() const
{
  return mVector[1];
}


void nuiMatrixNode_Translation::SetZ(float set)
{
  mVector[2] = set;
  Changed();
}

float nuiMatrixNode_Translation::GetZ() const
{
  return mVector[2];
}



/////////////////////////////////////////////
//class nuiMatrixNode_Scale :  nuiMatrixNode
nuiMatrixNode_Scale::nuiMatrixNode_Scale()
{
  if (SetObjectClass(_T("nuiMatrixNode_Scale")))
  {
    AddAttribute(new nuiAttribute<const nglVectorf&>
                 (nglString(_T("Scale")), nuiUnitVector,
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::GetScale),
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::SetScale)));
    
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("X")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::GetX),
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::SetX)));
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("Y")), nuiUnitNone,
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::GetY),
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::SetY)));
    AddAttribute(new nuiAttribute<float>
                 (nglString(_T("Z")), nuiUnitVector,
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::GetZ),
                  nuiMakeDelegate(this, &nuiMatrixNode_Scale::SetZ)));
  }
}

void nuiMatrixNode_Scale::Update() const
{
  mMatrix.SetScaling(mVector);
}


void nuiMatrixNode_Scale::Set(float X, float Y, float Z)
{
  mVector.Set(X, Y, Z);
  Changed();
}

void nuiMatrixNode_Scale::SetScale(const nglVectorf& rVector)
{
  mVector = rVector;
  Changed();
}

const nglVectorf& nuiMatrixNode_Scale::GetScale() const
{
  return mVector;
}

void nuiMatrixNode_Scale::SetX(float set)
{
  mVector[0] = set;
  Changed();
}

float nuiMatrixNode_Scale::GetX() const
{
  return mVector[0];
}


void nuiMatrixNode_Scale::SetY(float set)
{
  mVector[1] = set;
  Changed();
}

float nuiMatrixNode_Scale::GetY() const
{
  return mVector[1];
}


void nuiMatrixNode_Scale::SetZ(float set)
{
  mVector[2] = set;
  Changed();
}

float nuiMatrixNode_Scale::GetZ() const
{
  return mVector[2];
}

