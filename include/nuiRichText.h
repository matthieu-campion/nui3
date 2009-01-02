/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiRichText : public nuiSimpleContainer
{
public:
  nuiRichText();
  virtual ~nuiRichText();
  
  bool Draw(nuiDrawContext* pContext);
  nuiRect CalcIdealSize();
  bool SetRect(const nuiRect& rRect);
  
private:
  typedef struct FlowState
  {
    FlowState(float width);
    
    // Text attributes:
    bool mBold;
    bool mItalic;
    bool mUnderline;
    nuiColor mTextColor;
    float mWeight;
    nuiFont* mpFont;
    
    // Canvas:
    float mX; ///< Cursor X position
    float mY; ///< Cursor Y position
    float mWidth;
    float mHeight;    
  };
  
  class Operation
  {
  public:
    Operation();
    virtual ~Operation();
    
    virtual void Draw(nuiDrawContext* pContext);
    virtual void Flow(FlowState& rState); ///< compute the layout of this operation.
  };
  
  std::vector<Operation*> mpOperations;
  float mWidth;
};

