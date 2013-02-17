/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "float.h"

//#define NUI_SVG_USE_LISTS

class nuiSVGElement : public nuiShape
{
public:
  nuiSVGElement(nuiSVGElement* pParent);
  virtual ~nuiSVGElement();

  bool Load(nuiXMLNode* pNode); ///< Load an nuiSVGElement from a an XML description
  void Draw(nuiDrawContext* pContext, nuiShapeMode Mode, bool Antialiased);
  nuiRect GetBoundingRect();
private:
  nuiSVGElement* mpParent;
  
  bool LoadTransforms(nuiXMLNode* pNode);
  bool LoadPathData(nuiXMLNode* pNode);
  bool LoadStyle(const nglString& rStyleDesc);

  std::list<nuiSVGElement*> mpChildren;

  nuiShapeMode mShapeMode;
  nuiColor mStrokeColor;
  bool mUseStrokeColor;
  nuiColor mFillColor;
  bool mUseFillColor;

  float mFillOpacity;
  float mStrokeOpacity;
  float mStrokeWidth;
  float mFontSize;

  nglString mID;

  nuiMatrix mMatrix;

  nuiRect mViewBox;
  bool mUseViewBox;

  nuiRect mSelfRect;

  nglString mText;
  nuiSize mTextX, mTextY;

  nuiTextLayout* mpLayout;
  nuiFont* mpFont;
};

nuiSVGElement::nuiSVGElement(nuiSVGElement* pParent)
{
  mShapeMode = eDefault;
  mUseStrokeColor = false;
  mUseFillColor = false;
  mUseViewBox = false;

  mpFont = NULL;
  mpLayout = NULL;
  mFontSize = 10;

  mpParent = pParent;
  if (mpParent)
  {
    SetWinding(mpParent->GetWinding());
    mFillOpacity = mpParent->mFillOpacity; // Inherit the fill opacity
    mStrokeOpacity = mpParent->mStrokeOpacity; // Inherit the stroke opacity
    mStrokeWidth = mpParent->mStrokeWidth; // Inherit the stroke width
  }
  else
  {
    SetWinding(nuiShape::eNonZero);
    mFillOpacity = 1;
    mStrokeOpacity = 1;
    mStrokeWidth = 1;
  }

//  EnableCache(true);
}

nuiSVGElement::~nuiSVGElement()
{
  std::list<nuiSVGElement*>::iterator it;
  std::list<nuiSVGElement*>::iterator end = mpChildren.end();

  for (it = mpChildren.begin(); it!=end; ++it)
    delete (*it);

  if (mpFont)
    mpFont->Release();
  if (mpLayout)
    delete mpLayout;
}


int FindNext(const nglString& path, const nglString& set, int index)
{
  int len = path.GetLength();
  int setlen = set.GetLength();
  while (index < len)
  {
    int i;
    nglChar ch = path[index];
    for (i = 0; i < setlen; i++)
    {
      if (ch == set[i])
      {
        return index;
      }
    }
    index++;
  }
  return -1;
}

int GetParams(const nglString& path, int& pos, int count, std::vector<float> &params)
{
  nglString str;
  params.clear();
  int i,next;
  int length = path.GetLength();

  // Skip blank:
  while (path[pos]==' ' || path[pos]==10 || path[pos]==13 || path[pos] == '\t' || path[pos] == ',' && pos<length)
    pos++;

  for (i=0; i<count-1; i++)
  {
    next = FindNext(path, _T(" \13\10\t,"), pos);
    if (next == -1)
      next = path.GetLength();
    str = path.Extract(pos,next-pos);
    params.push_back(str.GetCFloat());
    pos = next;

    // Skip blank:
    while (path[pos]==' ' || path[pos]==10 || path[pos]==13 || path[pos] == '\t' || path[pos] == ',' && pos<length)
      pos++;

  }

  next = FindNext(path, _T("MmAaZzLlVvHhCcSsQqTt \13\10\t,"), pos);
  if (next == -1)
    next = path.GetLength();
  str = path.Extract(pos,next-pos);
  params.push_back(str.GetCFloat());
  pos = next;

  // Skip blank:
  while (path[pos]==' ' || path[pos]==10 || path[pos]==13 || path[pos] == '\t' || path[pos] == ',' && pos<length)
    pos++;

  return pos;
}

bool nuiSVGElement::LoadTransforms(nuiXMLNode* pNode)
{
  nglString transforms = pNode->GetAttribute("transform");
  std::vector<nglString> commands;
  transforms.Tokenize(commands, _T(";"));
  uint i;
  for (i=0; i<commands.size(); i++)
  {
    commands[i].Trim();
    int pos = commands[i].Find('(',0);
    nglString param = commands[i].Extract(pos+1);
    nglString command = commands[i].Extract(0,pos);
    param.TrimRight(')');

    if (command == _T("translate"))
    {
      std::vector<nglString> params;
      param.Tokenize(params, _T(", \10\13\t"));
      float x =0, y = 0;
      if (params.size()>=2)
      {
        x = params[0].GetCFloat();
        y = params[1].GetCFloat();
      }
      mMatrix.Translate(x,y,0);
    }
    else if (command == _T("scale"))
    {
      std::vector<nglString> params;
      param.Tokenize(params, _T(", \10\13\t"));
      float x,y,z;
      x = 1;
      y = 1;
      z = 1;
      if (params.size()>=3)
        z = params[2].GetCFloat();
      if (params.size()>=2)
        y = params[1].GetCFloat();
      if (params.size()>=1)
        x = params[0].GetCFloat();
      mMatrix.Scale(x,y,z);
    }
    else if (command == _T("viewBox"))
    {
      std::vector<nglString> params;
      param.Tokenize(params, _T(", \10\13\t"));
      float x,y,w,h;
      x = 0;
      y = 0;
      w = 1;
      h = 1;
      if (params.size()>=4)
        h = params[3].GetCFloat();
      if (params.size()>=3)
        w = params[2].GetCFloat();
      if (params.size()>=2)
        y = params[1].GetCFloat();
      if (params.size()>=1)
        x = params[0].GetCFloat();
      mViewBox.Set(x,y,w,h);
      mUseViewBox = true;
    }
  }

  return true;
}

bool nuiSVGElement::Load(nuiXMLNode* pNode)
{  
  // Generic stuff:
  mID = nuiGetString(pNode, _T("id")); // SVG ID tag
  // Look for transformations:
  if (pNode->HasAttribute("transform"))
    LoadTransforms(pNode);

  if (pNode->HasAttribute("style"))
    LoadStyle(pNode->GetAttribute("style"));

  if (pNode->HasAttribute("fill"))
  {
    nglString color = pNode->GetAttribute("fill");
    color.Trim();
    if (color == _T("none"))
    {
      mUseFillColor = false;
    }
    else if (mFillColor.SetValue(color))
    {
      mUseFillColor = true;
    }
  }

  if (pNode->HasAttribute("stroke"))
  {
    nglString color = pNode->GetAttribute("stroke");
    color.Trim();

    if (color == _T("none"))
    {
      mUseStrokeColor = false;
    }
    else if (mStrokeColor.SetValue(color))
    {
      mUseStrokeColor = true;
    }
  }

  if (pNode->HasAttribute("fill-rule"))
  {
    nglString fillrule = pNode->GetAttribute("fill-rule");
    fillrule.Trim();
    if (fillrule ==_T( "nonzero"))
      SetWinding(nuiShape::eNonZero);
    else if (fillrule == _T("evenodd"))
      SetWinding(nuiShape::eOdd);
  }

  if (pNode->HasAttribute("fill-opacity"))
  {
    nglString opac = pNode->GetAttribute("fill-opacity");
    opac.Trim();
    mFillOpacity = opac.GetCFloat();
    mFillColor.SetOpacity(mFillOpacity);
  }

  if (pNode->HasAttribute("stroke-opacity"))
  {
    nglString opac = pNode->GetAttribute("stroke-opacity");
    opac.Trim();
    mStrokeOpacity = opac.GetCFloat();
    mStrokeColor.SetOpacity(mStrokeOpacity);
  }

  if (pNode->HasAttribute("stroke-width"))
  {
    nglString width = pNode->GetAttribute("stroke-width");
    width.Trim();
    mStrokeWidth = width.GetCFloat();
  }

  if (mUseStrokeColor)
    mShapeMode = eStrokeShape;

  if (mUseFillColor)
    mShapeMode = eFillShape;
  
  if (mUseStrokeColor && mUseFillColor)
    mShapeMode = eStrokeAndFillShape;


  if (pNode->GetName() == _T("g")) // SVG Group tag
  {
    uint i, count = pNode->GetChildrenCount();
    for (i=0; i < count; i++)
    {
      nuiXMLNode* pChild = pNode->GetChild(i);
      nuiSVGElement* pNewElement = new nuiSVGElement(this);
      mpChildren.push_back(pNewElement);
      if (!pNewElement->Load(pChild))
        return false;
    }
  }
  else if (pNode->GetName() == _T("polyline") || pNode->GetName() == _T("polygone"))
  {
    float maxx,maxy,minx,miny;
    maxx = maxy = FLT_MIN;
    minx = miny = FLT_MAX;
    nglString points = pNode->GetAttribute("points");
    std::vector<nglString> tokens;
    points.Tokenize(tokens);
    uint i, count = tokens.size();
    /* unused: bool polygon = pNode->GetName() == "polygone"; */

    nuiContour* pContour = new nuiContour();
    nuiPoint Point;
    nuiPoint FirstPoint;
    bool first = true;

    AddContour(pContour);

    for (i=0; i<count; i++)
    {
      std::vector<nglString> coords;
      tokens[i].Tokenize(coords, _T(','));
      if (coords.size() < 2)
        return false; // Invalid coordinates

      Point[0] = coords[0].GetCFloat();
      Point[1] = coords[1].GetCFloat();
      if (coords.size() > 2)
        Point[2] = coords[2].GetCFloat(); // Try to interpret a Z coordinate
      else
        Point[2]=0;

      pContour->LineTo(Point);

      if (first)
      {
        FirstPoint = Point;
        first = false;
      }

      maxx = MAX(maxx,Point[0]);
      maxy = MAX(maxy,Point[1]);
      minx = MIN(minx,Point[0]);
      miny = MIN(miny,Point[1]);
    }

    mSelfRect.Set(minx,miny,maxx,maxy,false);

  }
  else if (pNode->GetName() == _T("path"))
  {
    LoadPathData(pNode);
  }
  else if (pNode->GetName() == _T("defs"))
  {
    // Load definitions
  }
  else if (pNode->GetName() == _T("rect"))
  {
    // Load a rectangle
    float x,y,w,h;
    x = nuiGetVal(pNode, _T("x"), 0.0f);
    y = nuiGetVal(pNode, _T("y"), 0.0f);
    w = nuiGetVal(pNode, _T("width"), 0.0f);
    h = nuiGetVal(pNode, _T("height"), 0.0f);
    nuiContour* pContour = new nuiContour();
    pContour->AddRect(nuiRect(x, y, w, h));

    AddContour(pContour);
  }
  else if (pNode->GetName() == _T("text"))
  {
    // Load a rectangle
    mTextX = nuiGetVal(pNode, _T("x"), 0.0f);
    mTextY = nuiGetVal(pNode, _T("y"), 0.0f);
    mText = pNode->GetChild(_T("##text"))->GetValue();
    if (!mpFont)
      mpFont = nuiFont::GetFont(mFontSize);
    mpLayout = new nuiTextLayout(mpFont);
    mpLayout->Layout(mText);
  }
  else if (pNode->GetName() == _T("circle"))
  {
    // Load a circle
    // Load a rectangle
    float x,y,r;
    x = nuiGetVal(pNode, _T("cx"), 0.0f);
    y = nuiGetVal(pNode, _T("cy"), 0.0f);
    r = nuiGetVal(pNode, _T("r"), 0.0f);
    AddCircle(x, y, r); 
  }

  return true;
}

#define NUI_TAN_CONST_RATIO (1.0f)

bool nuiSVGElement::LoadPathData(nuiXMLNode* pNode)
{
  float maxx,maxy,minx,miny;
  maxx = maxy = FLT_MIN;
  minx = miny = FLT_MAX;
  nglString path= pNode->GetAttribute("d");

  nuiContour* pContour = NULL;
  nuiSpline* pSpline = NULL;
  nuiPoint Point;
  nuiPoint FirstPoint;

  int pos = 0;
  int length = path.GetLength();
  nglChar mode = 0;
  nglChar newchar = 0;
  bool skipblank = true;
  std::vector<float> params;

  while (pos < length)
  {
    skipblank = true;
    while (skipblank && pos < length)
    {
      newchar = path.GetChar(pos);
      switch (newchar)
      {
        // Get commands:
      case 'Z':
      case 'z':
        // Reset the contour:
        if (pSpline)
        {
          // Changing mode, finish the current spline if any
          pContour->AddSpline(*pSpline);
          delete pSpline;
          pSpline = NULL;
        }
        pContour->Close();
        pContour = new nuiContour();
        AddContour(pContour);
      case 'M':
      case 'm':
      case 'L':
      case 'l':
      case 'H':
      case 'h':
      case 'V':
      case 'v':
      case 'A':
      case 'a':
        mode = newchar;
        pos++;
        // Changing mode, finish the current spline if any
        if (pSpline)
        {
          //pSpline->SetCubicBezierMode();
          pContour->AddSpline(*pSpline);
          delete pSpline;
          pSpline = NULL;
        }
        break;
      case 'Q':
      case 'q':
      case 'T':
      case 't':
        // going to spline mode, create a new spline.
        if (pSpline && pSpline->GetMode() != nuiSplineModeBezierQuadratic)
        {
          pContour->AddSpline(*pSpline);
          delete pSpline;
          pSpline = NULL;
        }

        if (!pSpline)
        {
          pSpline = new nuiSpline(); 
          pSpline->SetQuadraticBezierMode();
        }
        mode = newchar;
        pos++;
        break;

      case 'C':
      case 'c':
      case 'S':
      case 's':
        if (pSpline && pSpline->GetMode() != nuiSplineModeBezierCubic)
        {
          pContour->AddSpline(*pSpline);
          delete pSpline;
          pSpline = NULL;
        }
        if (!pSpline)
        {
          // going to spline mode, create a new spline.
          pSpline = new nuiSpline(); 
          pSpline->SetCubicBezierMode();
        }
        mode = newchar;
        pos++;
        break;

        // Skip over blank:
      case ' ':
      case 10:
      case 13:
      case '\t':
      case ',':
        pos++;
        break;
      default:
        // something else (not command, not blank), let's go on with the actual data:
        skipblank = false;
        break;
      }
    }

    if (pos < length) // Are we over yet? 
    {
      switch (mode)
      {
        // Move to (x y)+
      case 'M':
      case 'm':
        {
          GetParams(path, pos, 2,params);
          if (params.size() != 2)
            return false;

          if (mode == 'M')
          {
            Point[0] = params[0];
            Point[1] = params[1];
          }
          else
          {
            Point[0] += params[0];
            Point[1] += params[1];
          }
          FirstPoint = Point;
          pContour = new nuiContour();
          AddContour(pContour);
          pContour->LineTo(Point);
        }
        break;
        // Close path (no args) [all ready handled while skipping blank):
      //case 'Z':
      //case 'z':
      //  break;

        // LINES:
        // Line to (x y)+

      case 'L':
      case 'l':
        {
          GetParams(path, pos, 2,params);
          if (params.size() != 2)
            return false;

          if (mode =='L')
          {
            Point[0] = params[0];
            Point[1] = params[1];
          }
          else
          {
            Point[0] += params[0];
            Point[1] += params[1];
          }
          pContour->LineTo(Point);
        }
        break;
        // Horizontal Line to x+
      case 'H':
      case 'h':
        {
          GetParams(path, pos, 1,params);
          if (params.size() != 1)
            return false;

          if (mode =='H')
            Point[0] = params[0];
          else
            Point[0] += params[0];
          pContour->LineTo(Point);
        }
        break;
        // Vertical Line to y+
      case 'V':
      case 'v':
        {
          GetParams(path, pos, 1,params);
          if (params.size() != 1)
            return false;

          if (mode =='V')
            Point[1] = params[0];
          else
            Point[1] += params[0];
          pContour->LineTo(Point);
        }
        break;


        // CURVES:
        // Cubic Bezier Curves:
        // Curve to (x1 y1 y2 y2 x y)+
      case 'C':
      case 'c':
        {
          GetParams(path, pos, 6,params);
          if (params.size() != 6)
            return false;

          nuiSplineNode node(0,0,0);

          float outx,outy;
          if (pSpline->GetSize()==0)
          {
            // No point yet? Add one.
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            pSpline->AddNode(node);
          }

          nuiSplineNode* pNode = NULL;
          pNode = pSpline->GetNode(pSpline->GetSize()-1);


          if (mode =='C')
          {
            // Set the outgoing tangent on the last node:
            outx = params[0] - Point[0];
            outy = params[1] - Point[1];

            pNode->SetOutgoingTangent(nuiVector(outx* NUI_TAN_CONST_RATIO, outy* NUI_TAN_CONST_RATIO, 0));

            // And then insert new node with the right tangeant.            
            Point[0] = params[4];
            Point[1] = params[5];
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            float inx,iny;
            inx = params[2]-Point[0];
            iny = params[3]-Point[1];
            node.SetIncomingTangent(nuiVector(inx* NUI_TAN_CONST_RATIO, iny* NUI_TAN_CONST_RATIO,0));
          }
          else
          {
            // Set the outgoing tangent on the last node:
            outx = params[0];
            outy = params[1];

            pNode->SetOutgoingTangent(nuiVector(outx * NUI_TAN_CONST_RATIO, outy * NUI_TAN_CONST_RATIO, 0));
            
            // And then insert new node with the right tangeant (relative to the last point).
            float inx,iny;
            inx = Point[0] + params[2];
            iny = Point[1] + params[3];

            Point[0] += params[4];
            Point[1] += params[5];
            node.SetPosition(nuiVector(Point[0],Point[1],0));

            inx -= Point[0];
            iny -= Point[1];
            node.SetIncomingTangent(nuiVector(inx* NUI_TAN_CONST_RATIO, iny* NUI_TAN_CONST_RATIO,0));
          }
          pSpline->AddNode(node);
        }
        break;
        // Smooth Curve to (x2 y2 x y)+
      case 'S':
      case 's':
        {
          GetParams(path, pos, 4,params);
          if (params.size() != 4)
            return false;

          nuiSplineNode node(0,0,0);

          if (pSpline->GetSize()==0)
          {
            // No point yet? Add one.
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            pSpline->AddNode(node);
          }

          nuiSplineNode* pNode = NULL;
          pNode = pSpline->GetNode(pSpline->GetSize()-1);

          if (mode =='S')
          {
            // Set the outgoing tangent on the last node:
            float x,y;
            x = -pNode->GetIncomingTangent()[0];
            y = -pNode->GetIncomingTangent()[1];
            pNode->SetOutgoingTangent(nuiVector(x, y, 0));

            // And then insert new node with the right tangeant.
            Point[0] = params[2];
            Point[1] = params[3];
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            node.SetIncomingTangent(nuiVector(params[0]-Point[0], params[1]-Point[1], 0));
          }
          else
          {
            // Set the outgoing tangent on the last node:
            float x,y;
            x = -pNode->GetIncomingTangent()[0];
            y = -pNode->GetIncomingTangent()[1];
            pNode->SetOutgoingTangent(nuiVector(x, y, 0));

            float inx, iny;
            inx = params[0] + Point[0];
            iny = params[1] + Point[1];

            // And then insert new node with the right tangeant (relative to the last point).
            Point[0] += params[2];
            Point[1] += params[3];
            node.SetPosition(nuiVector(Point[0] ,Point[1] ,0));
            
            inx -= Point[0];
            iny -= Point[1];
            node.SetIncomingTangent(nuiVector(inx * NUI_TAN_CONST_RATIO, iny * NUI_TAN_CONST_RATIO, 0));
          }
          pSpline->AddNode(node);
        }
        break;

        // Quadratic Bezier Curves:
        // Curve to (x1 y1 x y)+
      case 'Q':
      case 'q':
        {
          GetParams(path, pos, 4, params);
          if (params.size() != 4)
            return false;

          nuiSplineNode node(0,0,0);

          float outx,outy;
          if (pSpline->GetSize()==0)
          {
            // No point yet? Add one.
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            pSpline->AddNode(node);
          }

          nuiSplineNode* pNode = NULL;
          pNode = pSpline->GetNode(pSpline->GetSize()-1);

          // Set the outgoing tangent on the last node:
          outx = params[0] - Point[0];
          outy = params[1] - Point[1];

          pNode->SetOutgoingTangent(nuiVector(outx* NUI_TAN_CONST_RATIO, outy* NUI_TAN_CONST_RATIO, 0));

          if (mode =='Q')
          {
            // And then insert new node with the right tangeant.            
            Point[0] = params[2];
            Point[1] = params[3];
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            node.SetIncomingTangent(nuiVector(outx* NUI_TAN_CONST_RATIO, outy* NUI_TAN_CONST_RATIO,0));
          }
          else
          {
            // And then insert new node with the right tangeant (relative to the last point).
            Point[0] += params[2];
            Point[1] += params[3];
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            node.SetIncomingTangent(nuiVector(outx* NUI_TAN_CONST_RATIO, outy* NUI_TAN_CONST_RATIO,0));
          }
          pSpline->AddNode(node);
        }
        break;
        // Smooth Curve to (x y)+
      case 'T':
      case 't':
        {
          GetParams(path, pos, 2,params);
          if (params.size() != 2)
            return false;

          nuiSplineNode node(0,0,0);

          if (pSpline->GetSize()==0)
          {
            // No point yet? Add one.
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            pSpline->AddNode(node);
          }

          nuiSplineNode* pNode = NULL;
          pNode = pSpline->GetNode(pSpline->GetSize()-1);

          // Set the outgoing tangent on the last node:
          float x,y;
          x = pNode->GetIncomingTangent()[0];
          y = pNode->GetIncomingTangent()[1];
          pNode->SetOutgoingTangent(nuiVector(x, -y, 0));

          if (mode =='T')
          {
            // And then insert new node with the right tangent.
            Point[0] = params[0];
            Point[1] = params[1];
            node.SetPosition(nuiVector(Point[0],Point[1],0));
            node.SetIncomingTangent(nuiVector(x, y, 0));
          }
          else
          {
            // And then insert new node with the right tangent (relative to the last point).
            node.SetPosition(nuiVector(Point[0] + params[0],Point[1] + params[1],0));
            Point[0] += params[0];
            Point[1] += params[1];
            node.SetIncomingTangent(nuiVector(x, y, 0));
          }
          pSpline->AddNode(node);
        }
        break;

        // Elliptical arc Curves:
        // Arc curve (rx ry x-axis-rotation large-arc-flag sweep-flag x y)+
      case 'A':
      case 'a':
        GetParams(path, pos, 7, params);
        if (mode =='A')
        {
          // And then insert new arc
          Point[0] = params[5];
          Point[1] = params[6];
          pContour->ArcTo(Point, params[0], params[1], params[2], params[3] != 0.0f, params[4] != 0.0f);
        }
        else
        {
          // And then insert new arc
          Point[0] += params[5];
          Point[1] += params[6];
          pContour->ArcTo(Point, params[0], params[1], params[2], params[3] != 0.0f, params[4] != 0.0f);
        }
        break;
      default:
        return false;
        break;
      }

      maxx = MAX(maxx,Point[0]);
      maxy = MAX(maxy,Point[1]);
      minx = MIN(minx,Point[0]);
      miny = MIN(miny,Point[1]);
      mSelfRect.Set(minx,miny,maxx,maxy,false);
    }
  }

  if (pSpline)
  {
    // Changing mode, finish the current spline if any
    pContour->AddSpline(*pSpline);
    delete pSpline;
    pSpline = NULL;
  }

  return true;
}

bool nuiSVGElement::LoadStyle(const nglString& rStyleDesc)
{
  std::vector<nglString> tokens;
  rStyleDesc.Tokenize(tokens, _T(";"));
  uint i, count = tokens.size();
  nuiColor col;

  for (i = 0; i < count; i++)
  {
    nglString command = tokens[i];
    command.Trim();
    if (command.Extract(0,5) == _T("fill:"))
    {
      nglString color = command.Extract(command.Find(':')+1);
      color.Trim();
      if (color == _T("none"))
      {
        mUseFillColor = false;
      }
      else if (col.SetValue(color))
      {
        col.SetOpacity(mFillOpacity);
        mFillColor = col;
        mUseFillColor = true;
      }
    }
    else if (command.Extract(0,7) == _T("stroke:"))
    {
      nglString color = command.Extract(command.Find(':')+1);
      color.Trim();
      
      if (color == _T("none"))
      {
        mUseStrokeColor = false;
      }
      else if (col.SetValue(color))
      {
        col.SetOpacity(mStrokeOpacity);
        mStrokeColor = col;
        mUseStrokeColor = true;
      }
    }
    else if (command.Find(_T("fill-rule")) != -1)
    {
      nglString fillrule = command.Extract(command.Find(':')+1);
      fillrule.Trim();
      if (fillrule == _T("nonzero"))
        SetWinding(nuiShape::eNonZero);
      else if (fillrule == _T("evenodd"))
        SetWinding(nuiShape::eOdd);
    }
    else if (command.Find(_T("fill-opacity")) != -1)
    {
      nglString opac = command.Extract(command.Find(':')+1);
      opac.Trim();
      mFillOpacity = opac.GetCFloat();
      mFillColor.SetOpacity(mFillOpacity);
    }
    else if (command.Find(_T("stroke-opacity")) != -1)
    {
      nglString opac = command.Extract(command.Find(':')+1);
      opac.Trim();
      mStrokeOpacity = opac.GetCFloat();
      mStrokeColor.SetOpacity(mStrokeOpacity);
    }
    else if (command.Find(_T("stroke-width")) != -1)
    {
      nglString width = command.Extract(command.Find(':')+1);
      width.Trim();
      mStrokeWidth = width.GetCFloat();
    }
    else if (command.Find(_T("font-size")) != -1)
    {
      nglString size = command.Extract(command.Find(':')+1);
      size.Trim();
      mFontSize = size.GetCFloat();
      mpFont = nuiFont::GetFont(mFontSize);
    }

    // Let's skip any other style info element for the moment

  }

  if (mUseStrokeColor)
    mShapeMode = eStrokeShape;

  if (mUseFillColor)
    mShapeMode = eFillShape;
  
  if (mUseStrokeColor && mUseFillColor)
    mShapeMode = eStrokeAndFillShape;

  return true;
}

void nuiSVGElement::Draw(nuiDrawContext* pContext, nuiShapeMode Mode, bool Antialiased)
{
  pContext->PushMatrix();
  pContext->MultMatrix(mMatrix);
  if (mUseViewBox)
  {
    float x,y,w,h;
    x = mSelfRect.Left() - mViewBox.Left();
    y = mSelfRect.Top() - mViewBox.Top();
    w = mSelfRect.GetWidth() / mViewBox.GetWidth();
    h = mSelfRect.GetHeight() / mViewBox.GetHeight();

    pContext->Translate(x,y);
    pContext->Scale(w,h);
  }

  if (mUseFillColor)
    pContext->SetFillColor(mFillColor);
  if (mUseStrokeColor)
    pContext->SetStrokeColor(mStrokeColor);

  if (mShapeMode != eDefault)
    Mode = mShapeMode;

  if (GetContourCount())
  {

    pContext->SetLineWidth(mStrokeWidth);
//    pContext->SetStrokeColor(nuiColor(0,0,0,1));
//    Mode = eStrokeShape;
    if (Antialiased && Mode != eFillShape) // Do antialiasing
    {
      pContext->EnableAntialiasing(true);
    }

    pContext->DrawShape(this,Mode);

    if (Antialiased && Mode == eFillShape) // Do antialiasing filled shapes directly if needed.
    {
      pContext->EnableAntialiasing(true);
      nuiColor temp = pContext->GetStrokeColor();
      nuiColor fill = pContext->GetFillColor();
      pContext->SetStrokeColor(fill);
      pContext->SetLineWidth(0.5f);
      pContext->DrawShape(this,eStrokeShape);
      pContext->SetStrokeColor(temp);
    }
    pContext->EnableAntialiasing(false);
  }

  if (mpLayout)
  {
    pContext->SetFont(mpFont);
    pContext->DrawText(mTextX, mTextY, *mpLayout);
  }

  std::list<nuiSVGElement*>::iterator it;
  std::list<nuiSVGElement*>::iterator end = mpChildren.end();

  for (it = mpChildren.begin(); it!=end; ++it)
  {
    (*it)->Draw(pContext,Mode, Antialiased);
  }

  pContext->PopMatrix();
}


nuiRect nuiSVGElement::GetBoundingRect()
{
  nuiRect rect;
  nuiRect tmprect;

  std::list<nuiSVGElement*>::iterator it;
  std::list<nuiSVGElement*>::iterator end = mpChildren.end();

  rect = mSelfRect;
  for (it = mpChildren.begin(); it!=end; ++it)
  {
    tmprect = rect;
    rect.Union(tmprect,(*it)->GetBoundingRect());
  }

  nuiVector point,res;

  point[0] = rect.mLeft;
  point[1] = rect.mTop;
  point[2] = 0;
  point[3] = 1.0f;
  res = mMatrix * point;
  rect.mLeft = res[0];
  rect.mTop  = res[1];

  point[0] = rect.mRight;
  point[1] = rect.mBottom;
  point[2] = 0;
  point[3] = 1.0f;
  res = mMatrix * point;
  rect.mRight  = res[0];
  rect.mBottom = res[1];

  return rect;
}



/////////////////////////////////////
nuiSVGShape::nuiSVGShape()
{
  mTmpMode = eStrokeShape;
  mList = 0;
  mValidBRect = false;
}

nuiSVGShape::~nuiSVGShape()
{
  std::list<nuiSVGElement*>::iterator it;
  std::list<nuiSVGElement*>::iterator end = mpChildren.end();

  for (it = mpChildren.begin(); it!=end; ++it)
  {
    delete (*it);
  }
}

bool nuiSVGShape::Load(nuiXMLNode* pNode)
{
  if (pNode->GetName() != _T("svg"))
    return false;

  if (pNode->HasAttribute("width") && pNode->HasAttribute("height"))
  {
    float w,h;
    w = nuiGetVal(pNode, _T("width"), 0.0f);
    h = nuiGetVal(pNode, _T("height"), 0.0f);
    mBRect.Set(0.0f, 0.0f, w, h);
  }

  uint i, count = pNode->GetChildrenCount();
  for (i =0; i < count; i++)
  {
    nuiXMLNode* pChild = pNode->GetChild(i);
    if (pChild->GetName() == _T("title")) // Is it the title of the svg shape?
      mTitle = pChild->GetValue();
    else
    {
      nuiSVGElement* pElement = new nuiSVGElement(NULL);
      mpChildren.push_back(pElement);
      if (!pElement->Load(pChild)) // 
        return false;
    }
  }

  return true;
}

void nuiSVGShape::Draw(nuiDrawContext* pContext, bool Antialias)
{
#ifdef NUI_SVG_USE_LISTS
  bool listoptims = false && (NULL == dynamic_cast<nuiMemoryDrawContext*>(pContext));
  
  if (listoptims)
  {
    if (mList)
    {
      glCallList(mList);
      return;
    }

    mList = glGenLists(1);
    glNewList(mList, GL_COMPILE_AND_EXECUTE);
  }
#endif

  std::list<nuiSVGElement*>::iterator it;
  std::list<nuiSVGElement*>::iterator end = mpChildren.end();

  for (it = mpChildren.begin(); it!=end; ++it)
  {
    (*it)->Draw(pContext,eFillShape, Antialias);
  }

#ifdef NUI_SVG_USE_LISTS
  if (listoptims)
  {
    glEndList();
  }
#endif

}


const nglString& nuiSVGShape::GetTitle()
{
  return mTitle;
}

nuiRect nuiSVGShape::GetBoundingRect()
{
  if (mValidBRect)
    return mBRect;

  nuiRect rect;
  nuiRect tmprect;

  std::list<nuiSVGElement*>::iterator it;
  std::list<nuiSVGElement*>::iterator end = mpChildren.end();

  for (it = mpChildren.begin(); it!=end; ++it)
  {
    tmprect = rect;
    rect.Union(tmprect,(*it)->GetBoundingRect());
  }
  mBRect = rect;
  mValidBRect = true;
  return rect;
}


