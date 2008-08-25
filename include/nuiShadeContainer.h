/*
 *  nuiShadeContainer.h
 *  libnui
 */

#ifndef __nuiShadeContainer_h__
#define __nuiShadeContainer_h__

#include "nuiContainer.h"

class nuiShadeContainer : public nuiSimpleContainer
{
public:
  nuiShadeContainer(nuiSize shadeSize)
  : nuiSimpleContainer(), mShadeSize(shadeSize)
  {
  }
  
  virtual ~nuiShadeContainer()
  {
  }
  
  nuiRect CalcIdealSize()
  {
    nuiRect idealRect = nuiSimpleContainer::CalcIdealSize();
    idealRect.SetSize(idealRect.GetWidth()+2*mShadeSize, idealRect.GetHeight()+mShadeSize);
    return idealRect; 
  }

  virtual bool SetRect(const nuiRect& rRect)
  {
    nuiWidget::SetRect(rRect);
    
    nuiRect rect(rRect.Size());
    rect.mLeft    += mShadeSize;
    rect.mRight   -= mShadeSize;
    rect.mBottom  -= mShadeSize;
    
    IteratorPtr pIt;
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      if (mCanRespectConstraint)
        pItem->SetLayoutConstraint(mConstraint);
      pItem->GetIdealRect();
      pItem->SetLayout(rect);
    }
    delete pIt;
    
    DebugRefreshInfo();
    return true;  
  }

  virtual bool Draw(nuiDrawContext* pContext)
  {
    nuiRect rect = GetRect().Size();
    
    rect.mLeft    += mShadeSize;
    rect.mRight   -= mShadeSize;
    rect.mBottom  -= mShadeSize;
    
    nuiRect shadeRect = GetRect().Size();
    
    pContext->DrawShade(rect,shadeRect);
    
    DrawChildren(pContext);
    
    return true;
  }

protected:
  nuiSize mShadeSize;
};

#endif //ifndef __nuiShadeContainer_h__

