/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiRichText.h"

//class nuiRichText : public nuiSimpleContainer
nuiRichText::nuiRichText()
{
  mWidth = 800;
}

nuiRichText::~nuiRichText()
{
  for (uint32 i = 0; i < mpOperations.size(); i++)
    delete mpOperations[i];
}

bool nuiRichText::Draw(nuiDrawContext* pContext)
{
  for (uint32 i = 0; i < mpOperations.size(); i++)
    mpOperations[i]->Draw(pContext);
  return true;
}

nuiRect nuiRichText::CalcIdealSize()
{
  FlowState state(mWidth);
  
  for (uint32 i = 0; i < mpOperations.size(); i++)
    mpOperations[i]->Flow(state);
  
  return nuiRect(0.0f, 0.0f, state.mWidth, state.mHeight);
}

bool nuiRichText::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  
  FlowState state(rRect.GetWidth());
  
  for (uint32 i = 0; i < mpOperations.size(); i++)
    mpOperations[i]->Flow(state);
  
  return true;
}


// FlowState    
nuiRichText::FlowState::FlowState(float width)
{
  // Text attributes:
  mBold = false;
  mItalic = false;
  mUnderline = false;
  mWeight = 14;
  mpFont = NULL;
  
  mX = 0;
  mY = 0;
  mWidth = width;
  mHeight = 0;
}
        
    
//class Operation
nuiRichText::Operation::Operation()
{
  
}

nuiRichText::Operation::~Operation()
{
  
}

void nuiRichText::Operation::Draw(nuiDrawContext* pContext)
{
  // Default = don't draw anything
}

void nuiRichText::Operation::Flow(FlowState& rState)
{
  // Default = don't draw anything
}


