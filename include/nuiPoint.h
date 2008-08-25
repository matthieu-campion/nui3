/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPoint_h__
#define __nuiPoint_h__

//#include "nui.h"
#include "nuiXML.h"
#include <vector>

enum nuiPointType 
{
  nuiPointTypeNormal,
  nuiPointTypeStop
};

class NUI_API nuiPoint: public nuiVector
{
public:
  nuiPoint() : nuiVector(), mType(nuiPointTypeNormal)
  {
  }

  nuiPoint(nuiXMLNode* pNode) : nuiVector(), mType(nuiPointTypeNormal)
  { 
    if (pNode->GetName() != _T("nuiPoint"))
      return;

    Elt[0] = nuiGetVal(pNode,_T("X"), 0.0f);
    Elt[1] = nuiGetVal(pNode,_T("Y"), 0.0f);
    Elt[2] = nuiGetVal(pNode,_T("Z"), 0.0f);
    if (nuiGetBool(pNode, _T("Stop"), false))
      SetType(nuiPointTypeStop);

    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
    const nuiXMLNodeList& children = pNode->GetChildren();

    nuiXMLNodeList::const_iterator it;
    nuiXMLNodeList::const_iterator end = children.end();
  }

  /** @name Serialisation */
  //@{
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode) const ///< This method saves the state in the given XML Node.
  {
    nuiXMLNode* pNode = NULL;
    if (pParentNode)
      pNode = new nuiXMLNode(_T("nuiPoint"),pParentNode);
    else
      pNode = new nuiXML(_T("nuiPoint"));

//    pNode->SetAttribute(_T("Name"),GetProperty(_T("Name")));
    pNode->SetAttribute(_T("X"),Elt[0]);
    pNode->SetAttribute(_T("Y"),Elt[1]);
    pNode->SetAttribute(_T("Z"),Elt[2]);
    if (GetType() == nuiPointTypeStop)
      pNode->SetAttribute("Stop", true);
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
    return pNode;
  }
  //@}

  /// Initialize with values
  nuiPoint(float X, float Y, float Z = 0.0f, float W = 1.f) : nuiVector(X,Y,Z,W), mType(nuiPointTypeNormal)
  {
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a two dimensional vector. Third element is zero, fourth is one.
  nuiPoint(const nuiVector2& rVector) : nuiVector(rVector), mType(nuiPointTypeNormal)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a two dimensional vector. Fourth element is one.
  nuiPoint(const nuiVector3& rVector)  : nuiVector(rVector), mType(nuiPointTypeNormal)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a four dimensional vector.
  nuiPoint(const nuiVector& rVector)  : nuiVector(rVector), mType(nuiPointTypeNormal)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Initialize with a special point type.
  nuiPoint(nuiPointType type)  : nuiVector(), mType(type)
  { 
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  /// Copy constructor
  nuiPoint(const nuiPoint& rPoint)  : nuiVector(rPoint), mType(rPoint.mType)
  {
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
  }

  nuiPoint& operator=(const nuiPoint& rPoint)
  {
    *((nuiVector*)this) = (nuiVector)rPoint;
    SetType(rPoint.mType);
    NGL_ASSERT(finite(Elt[0]) && finite(Elt[1]) && finite(Elt[2]));
    return *this;
  }

  bool operator==(const nuiPoint& rPoint) const
  {
    return (nuiVector::operator==(rPoint)) && mType == rPoint.mType;
  }

  /// Destructor
  virtual ~nuiPoint() ///< Destructor.
  {
  }

  void SetType(nuiPointType type)
  {
    mType = type;
    if (mType == nuiPointTypeStop)
    {
      Elt[0] = Elt[1] = Elt[2] = Elt[3] = 0.0f;
    }
  }

  nuiPointType GetType() const
  {
    return mType;
  }

  static const nuiPoint Zero;
  static const nuiPoint One;

  bool IsValid() const
  {
    bool f0 = finite(Elt[0]) != 0;
    bool f1 = finite(Elt[1]) != 0;
    bool f2 = finite(Elt[2]) != 0;
    bool f3 = finite(Elt[3]) != 0;

    bool res = f0 && f1 && f2 && f3;

/*
    if (!res)
    {
      NGL_OUT(_T("Invalid point: {%f, %f, %f, %f}\n"), Elt[0], Elt[1], Elt[2], Elt[3]);
      _asm
      {
        int 3;
      }
    }
*/
    return res;
  }
protected:
  nuiPointType mType;
};

#endif // __nuiPoint_h__
