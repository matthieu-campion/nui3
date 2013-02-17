/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2011 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

const char* nuiGetLayoutAttributeName(nuiLayoutAttribute attrib)
{
  switch (attrib)
  {
    case eLayoutAttribute_Left:     return "Left";
    case eLayoutAttribute_Right:    return "Right";
    case eLayoutAttribute_Top:      return "Top";
    case eLayoutAttribute_Bottom:   return "Bottom";
    case eLayoutAttribute_Width:    return "Width";
    case eLayoutAttribute_Height:   return "Height";
    case eLayoutAttribute_CenterX:  return "CenterX";
    case eLayoutAttribute_CenterY:  return "CenterY";
  }
  
  NGL_ASSERTR(0, "error");
}

const char* nuiGetLayoutRelationName(nuiLayoutRelation relation)
{
  switch (relation)
  {
    case eLayoutRelation_Equals:          return "Equals";
    case eLayoutRelation_LessThanOrEqual: return "LessThanOrEqual";
    case eLayoutRelation_MoreThanOrEqual: return "MoreThanOrEqual";
  }
  
  NGL_ASSERTR(0, "error");
}

const char* nuiGetLayoutPriorityName(nuiLayoutPriority priority)
{
  switch (priority)
  {
    case eLayoutPriority_FromWidget:  return "FromWidget";
    case eLayoutPriority_Low:         return "Low";
    case eLayoutPriority_Normal:      return "Normal";
    case eLayoutPriority_High:        return "High";
    case eLayoutPriority_Highest:        return "Highest";
  }
  
  NGL_ASSERTR(0, "error");
}


////////////////
// Contraint:
nuiLayout::Constraint::Constraint(nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, nuiLayoutRelation Relation, double Multiplier, double Constant, nuiLayoutPriority Priority)
:
  mpRefWidget(pRefWidget),
  mRefAttrib(RefAttrib),
  mRelation(Relation),
  mMultiplier(Multiplier),
  mConstant(Constant),
  mPriority(Priority)
{
}

nuiLayout::Constraint::~Constraint()
{
}


//////////////
// nuiLayout:
nuiLayout::nuiLayout()
{
  if (SetObjectClass("nuiLayout"))
  {
    //
  }
}

nuiLayout::~nuiLayout()
{
}

bool nuiLayout::AddConstraint(const nglString& rConstraintString)
{
  nuiWidget* pWidget = NULL;
  double value = 0;
  nuiLayoutAttribute attrib = eLayoutAttribute_None;

  const double DEFAULT_SPACING = 12;
  int32 consumed = 0;
  int32 i = 0;
  nglUChar ch = rConstraintString.GetNextUChar(i);
  for (; consumed < rConstraintString.GetLength();)
  {
    printf("new char: %c\n", (char)ch);
    
    if (ch == '|')
    {
      if (pWidget != NULL)
      {
        AddConstraint(pWidget, eLayoutAttribute_Right, eLayoutRelation_Equals, this, eLayoutAttribute_Right, 1.0f, -value);
        // It has to be the end!
        return true;
      }
      else
      {
        pWidget = this;
        attrib = eLayoutAttribute_Left;
      }
      consumed++;
      ch = rConstraintString.GetNextUChar(i);
      printf("0    new char: %c\n", (char)ch);
    }
    else if (ch == '-')
    {
      ch = rConstraintString.GetNextUChar(i);
      printf("1    new char: %c\n", (char)ch);
      value = 0;
      bool v = false;
      while (nglIsDigit(ch))
      {
        value *= 10;
        value += ch - '0';
        v = true;
        consumed++;
        ch = rConstraintString.GetNextUChar(i);
        printf("2    new char: %c\n", (char)ch);
      }
      
      if (v) // expect the second dash after the value
      {
        if (ch != '-')
        {
          return false;
        }
        consumed++;
        ch = rConstraintString.GetNextUChar(i);
        printf("3    new char: %c\n", (char)ch);
      }
      else
      {
        value = DEFAULT_SPACING;
      }
    }
    else if (ch == '[')
    {
      // look for a widget name:
      ch = rConstraintString.GetNextUChar(i);
      printf("4    new char: %c\n", (char)ch);
      if (nglIsAlpha(ch))
      {
        nglString str;
        while (nglIsAlphaNum(ch))
        {
          str += ch;
          consumed++;
          ch = rConstraintString.GetNextUChar(i);
          printf("5    new char: %c\n", (char)ch);
        }
        
        nuiWidget* pRightWidget = nuiContainer::GetChild(str);
        if (!pRightWidget)
        {
          return false;
        }
        
        // We know the right widget so we can connect it to the left:
        if (pWidget && attrib != eLayoutAttribute_None)
          AddConstraint(pRightWidget, eLayoutAttribute_Left, eLayoutRelation_Equals, pWidget, attrib, 1, value);
        
        pWidget = pRightWidget;
        attrib = eLayoutAttribute_Right;
        value = 0;
        
        if (ch == '(')
        {
          // Parse options
        }
        
        if (ch != ']')
        {
          return false;
        }
        consumed++;
        ch = rConstraintString.GetNextUChar(i);
        printf("6    new char: %c\n", (char)ch);
      }
    }
  }
  
  return true;
}


bool nuiLayout::AddConstraint(nuiWidget* pWidget, nuiLayoutAttribute Attrib, nuiLayoutRelation Relation, nuiWidget* pRefWidget, nuiLayoutAttribute RefAttrib, double Multiplier, double Constant, nuiLayoutPriority Priority)
{
  Widget* pW = &mWidgets[pWidget];
  pW->mpWidget = pWidget;
  //pW->mRefs++;
  
  Constraint constraint(pRefWidget, RefAttrib, Relation, Multiplier, Constant, Priority);
  
  LayoutValue* pValue = NULL;
  DependencySet* pDeps = NULL;

  int32 deps = 1;

  switch (Attrib)
  {
    case eLayoutAttribute_Left:
      pValue = & pW->mLeft;
      deps = 0;
      break;
    case eLayoutAttribute_Right:
      pValue = & pW->mRight;
      deps = 0;
      break;
    case eLayoutAttribute_Width:
      pValue = & pW->mWidth;
      deps = 0;
      break;
    case eLayoutAttribute_CenterX:
      pValue = & pW->mCenterX;
      deps = 0;
      break;
     
      
    case eLayoutAttribute_Top:
      pValue = & pW->mTop;
      break;
    case eLayoutAttribute_Bottom:
      pValue = & pW->mBottom;
      break;
    case eLayoutAttribute_Height:
      pValue = & pW->mHeight;
      break;
    case eLayoutAttribute_CenterY:
      pValue = & pW->mCenterY;
      break;
  }
  
  pDeps = & pW->mDeps[deps];
  pValue->mConstraints.push_back(constraint);
  
  if (pRefWidget)
  {
    pDeps->insert(pRefWidget);
    Widget* pR = &mWidgets[pRefWidget];
    pR->mpWidget = pRefWidget;
    pR->mRefs++;
    pR->mIncommingDeps[deps]++;
  }
  
  return true;
}

void nuiLayout::AddDependency(nuiWidget* pFromWidget, nuiWidget* pToWidget)
{
}


bool nuiLayout::DoLayout(const nuiRect& rRect)
{
  // Topological sort:
  /* Algorithm from wikipedia:
  L ← Empty list that will contain the sorted elements
  S ← Set of all nodes with no incoming edges
  while S is non-empty do
    remove a node n from S
    insert n into L
    for each node m with an edge e from n to m do
      remove edge e from the graph
      if m has no other incoming edges then
        insert m into S
  
  if graph has edges then
    output error message (graph has at least one cycle)
  else 
    output message (proposed topologically sorted order: L)
   */
  
  std::set<Widget*> startnodes[2];
  std::list<Widget*> nodes[2];
  {
    // Fill startnodes:
    Widgets::iterator it = mWidgets.begin();
    Widgets::iterator end = mWidgets.end();
    
    while (it != end)
    {
      Widget* pWidget = &it->second;

      // fill the list of start nodes:
      int32 inx = pWidget->mIncommingDeps[0];
      int32 iny = pWidget->mIncommingDeps[1];
      if (inx == 0)
        startnodes[0].insert(pWidget);

      if (iny == 0)
        startnodes[1].insert(pWidget);

      // reset layout:
      nuiRect r;
      nuiWidget* pW = it->first;
      if (pW != this)
      {
        r = pW->GetIdealRect().Size();
      }
      else
        r = pW->GetRect().Size();
      
      pW->LocalToLocal(this, r);
      pWidget->Reset(r); // Use the ideal rect as the reference for all widgets except this one
      
      ++it;
    }
  }
  
  for (int32 c = 0; c < 2; c++)
  {
    // Now start the actual topological sort:
    while (!startnodes[c].empty())
    {
      // remove a node pNode from the list of start nodes:
      std::set<Widget*>::iterator it = startnodes[c].begin();
      Widget* pNode = *it;
      startnodes[c].erase(it);
      
      // Insert this node into the node list:
      nodes[c].push_back(pNode);
      
      // for each node referenced by pNode:
      DependencySet::iterator deps_it = pNode->mDeps[c].begin();
      DependencySet::iterator deps_end = pNode->mDeps[c].end();
      while (deps_it != deps_end)
      {
        nuiWidget* pRef = *deps_it;
        Widget* pRefWidget = &mWidgets[pRef];
        
        pRefWidget->mVisits[c]++;
        if (pRefWidget->mVisits[c] == pRefWidget->mIncommingDeps[c])
        {
          startnodes[c].insert(pRefWidget);
        }
        
        ++deps_it;
      }
    }


    // Now we can calculate the layout, node by node, going from the one everyone depends on to the star nodes:
    nodes[c].reverse();
    
    std::list<Widget*>::iterator it = nodes[c].begin();
    std::list<Widget*>::iterator end = nodes[c].end();
    
    while (it != end)
    {
      Widget* pNode(*it);
      //NGL_OUT("Laying out '%s' [%p]\n", pNode->mpWidget->GetObjectName().GetChars(), pNode->mpWidget);
      
      for (DependencySet::const_iterator deps_it = pNode->mDeps[c].begin(); deps_it != pNode->mDeps[c].end(); ++deps_it)
      {
        // Evaluate constraints:
        
        if (c == 0)
        {
          // Horizontal
          EvaluateConstraints(pNode->mLeft);
          EvaluateConstraints(pNode->mRight);
          EvaluateConstraints(pNode->mWidth);
          EvaluateConstraints(pNode->mCenterX);

          nuiRect& r(pNode->mRect);
          // Width
          if (pNode->mWidth.mSet)
            r.SetWidth(pNode->mWidth.mValue);
          
          // Left / Right:
          if (pNode->mLeft.mSet)
          {
            if (pNode->mRight.mSet)
              r.Left() = pNode->mLeft.mValue;
            else
              r.MoveTo(pNode->mLeft.mValue, r.Top());
          }
          
          if (pNode->mRight.mSet)
          {
            if (pNode->mLeft.mSet)
              r.Right() = pNode->mRight.mValue;
            else
              r.MoveTo(pNode->mRight.mValue - r.GetWidth(), r.Top());
          }
          
          // Center X:
          if (pNode->mCenterX.mSet)
            r.MoveTo(pNode->mCenterX.mValue - r.GetWidth() * .5, r.Top());
          
        }
        else
        {
          // Vertical
          EvaluateConstraints(pNode->mTop);
          EvaluateConstraints(pNode->mBottom);
          EvaluateConstraints(pNode->mHeight);
          EvaluateConstraints(pNode->mCenterY);

          nuiRect& r(pNode->mRect);
          // Height:
          if (pNode->mHeight.mSet)
            r.SetHeight(pNode->mHeight.mValue);
          
          // Top / Bottom:
          if (pNode->mTop.mSet)
          {
            if (pNode->mBottom.mSet)
              r.Top() = pNode->mTop.mValue;
            else
              r.MoveTo(r.Left(), pNode->mTop.mValue);
          }
          
          if (pNode->mBottom.mSet)
          {
            if (pNode->mTop.mSet)
              r.Bottom() = pNode->mBottom.mValue;
            else
              r.MoveTo(r.Left(), pNode->mBottom.mValue - r.GetHeight());
          }
          
          // Center Y:
          if (pNode->mCenterY.mSet)
            r.MoveTo(r.Left(), pNode->mCenterY.mValue - r.GetHeight() * .5);
          
        }
        
        //NGL_OUT("\t\tupdated layout rect: %s\n", pNode->mRect.GetValue().GetChars());
      }
      
      ++it;
    }
    
  }

  bool res = true;
//  if (!edges.empty())
//  {
//    // There is a cycle in the graph!
//    NGL_OUT("\n\nERROR: There is a cycle in the graph!\n\n");
//    res = false;
//  }
  

  
  //// We're almost there!
  // Now we have the constraints all set, let's put them back into the widgets:
  {
    Widgets::iterator it  = mWidgets.begin();
    Widgets::iterator end = mWidgets.end();
    
    while (it != end)
    {
      nuiWidget* pWidget = it->first;
      if (pWidget != this)
      {
        Widget& rWidget = it->second;
        nuiRect r = rWidget.mRect;
        //r = pWidget->GetBorderLessRect(r);
        pWidget->SetLayout(r);
      }
      
      ++it;
    }
    
  }
  
  
  return res;
}

void nuiLayout::EvaluateConstraints(LayoutValue& rValue)
{
  // Apply all constraints that go from this node:
  for (int32 i = 0; i < rValue.mConstraints.size(); i++)
  {
    Constraint& rConstraint(rValue.mConstraints[i]);
    
    double val = 0;
    if (rConstraint.mpRefWidget)
    {
      //NGL_OUT("\twith RefWidget = %s (%s)\n", rConstraint.mpRefWidget->GetObjectName().GetChars(), nuiGetLayoutAttributeName(rConstraint.mRefAttrib));
      
      Widget& W(mWidgets[rConstraint.mpRefWidget]);
      
      switch (rConstraint.mRefAttrib)
      {
        case eLayoutAttribute_Left:     val = W.mRect.Left(); break;
        case eLayoutAttribute_Right:    val = W.mRect.Right(); break;
        case eLayoutAttribute_Width:    val = W.mRect.GetWidth(); break;
        case eLayoutAttribute_CenterX:  val = W.mRect.Left() + W.mRect.GetWidth() * .5; break;
          
        case eLayoutAttribute_Top:      val = W.mRect.Top(); break;
        case eLayoutAttribute_Bottom:   val = W.mRect.Bottom(); break;
        case eLayoutAttribute_Height:   val = W.mRect.GetHeight(); break;
        case eLayoutAttribute_CenterY:  val = W.mRect.Top() + W.mRect.GetHeight() * .5; break;
      }
    }
    //NGL_OUT("\tConstraint[%d] %s (%f * %f + %f)\n", i, nuiGetLayoutRelationName(rConstraint.mRelation), val, rConstraint.mMultiplier, rConstraint.mConstant);
    
    val = val * rConstraint.mMultiplier + rConstraint.mConstant;
    
    switch (rConstraint.mRelation)
    {
      case eLayoutRelation_Equals:
        if (rValue.mPriorityMin <= rConstraint.mPriority)
        {
          rValue.mMin = val;
          rValue.mPriorityMin = rConstraint.mPriority;
        }
        
        if (rValue.mPriorityMax <= rConstraint.mPriority)
        {
          rValue.mMax = val;
          rValue.mPriorityMax = rConstraint.mPriority;
        }
        break;
      case eLayoutRelation_LessThanOrEqual:
        if (rValue.mPriorityMax <= rConstraint.mPriority)
        {
          rValue.mMax = val;
          rValue.mPriorityMax = rConstraint.mPriority;
        }
        break;
      case eLayoutRelation_MoreThanOrEqual:
        if (rValue.mPriorityMin <= rConstraint.mPriority)
        {
          rValue.mMin = val;
          rValue.mPriorityMin = rConstraint.mPriority;
        }
        break;
    }
    
    rValue.mValue = nuiClamp(rValue.mValue, rValue.mMin, rValue.mMax);
    rValue.mSet = true;
  }
}

bool nuiLayout::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  //NGL_OUT("nuiLayout::SetRect(%s)\n", rRect.GetValue().GetChars());
  
  nuiRect r(rRect.Size());
  DoLayout(r);
  
  return true;
}


