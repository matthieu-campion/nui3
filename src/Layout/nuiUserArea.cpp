/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiUserArea.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"


nuiUserArea::nuiUserArea()
 : nuiWidget() 
{
  SetObjectClass(_T("nuiUserArea"));
  mInterceptMouse = false;
  mPrepareGLContext = false;
  
  EnableRenderCache(false);

  NUI_ADD_EVENT(UserDraw);
  NUI_ADD_EVENT(ClickedMouse);
  NUI_ADD_EVENT(UnclickedMouse);
  NUI_ADD_EVENT(MovedMouse);
}

bool nuiUserArea::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  SetObjectClass(_T("nuiUserArea"));

  mInterceptMouse = nuiGetBool ( pNode, _T("InterceptMouse"), false);
  mPrepareGLContext = nuiGetBool ( pNode, _T("PrepareGLContext"), false);
  
  EnableRenderCache(false);

  NUI_ADD_EVENT(UserDraw);
  NUI_ADD_EVENT(ClickedMouse);
  NUI_ADD_EVENT(UnclickedMouse);
  NUI_ADD_EVENT(MovedMouse);
  
  return true;
}

nuiUserArea::~nuiUserArea()
{
}


bool nuiUserArea::Draw(nuiDrawContext* pContext)
{
  GLint OldRect[4];
  nuiContainerPtr pRoot = GetRoot();
  if (!pRoot)
    return false;

  if (mPrepareGLContext)
  {
  //  pContext->PushState();
  #ifndef __NUI_NO_GL__
#ifndef _UIKIT_
//    glPushAttrib(GL_ALL_ATTRIB_BITS);
#endif
  #endif
    pContext->PushState();
    pContext->ResetState();
    nuiRenderArray dummy(GL_POINTS);
    pContext->DrawArray(dummy);

    nuiRect Rect = mRect.Size(), RootRect = pRoot->GetRect();

    LocalToGlobal(Rect);

  #ifndef __NUI_NO_GL__

//    glGetIntegerv(GL_VIEWPORT, OldRect);
//    glViewport((int)Rect.mLeft, (int)(RootRect.mBottom - Rect.mBottom), (int)Rect.GetWidth(), (int)Rect.GetHeight());

//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    glTranslatef(-1.0f, 1.0f, 0.0f);
//    glScalef(2.0f/Rect.GetWidth(), -2.0f/Rect.GetHeight(), 1.0f);

//    glMatrixMode (GL_MODELVIEW);
//    glLoadIdentity();
    
//    glDisable(GL_SCISSOR_TEST);
  #endif

    pContext->PushProjectionMatrix();
    pContext->Set2DProjectionMatrix(Rect);
  
  }

  UserDraw(pContext);

  if (mPrepareGLContext)
  {
  #ifndef __NUI_NO_GL__
//    glViewport(
//      OldRect[0],
//      OldRect[1],
//      OldRect[2],
//      OldRect[3]);
//
//    glMatrixMode (GL_PROJECTION);
//    glPopMatrix();
//
//    glMatrixMode (GL_MODELVIEW);
#ifndef _UIKIT_
//    glPopAttrib();
#endif  
#endif  

    pContext->PopProjectionMatrix();
    pContext->PopState();

    //pContext->ResetState();
    nuiRenderArray dummy(GL_POINTS);
    pContext->DrawArray(dummy);
  }
  return true;
}

bool nuiUserArea::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  ClickedMouse(X,Y,Button);
  return mInterceptMouse;
}

bool nuiUserArea::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  UnclickedMouse(X,Y,Button);
  return mInterceptMouse;
}

bool nuiUserArea::MouseMoved  (nuiSize X, nuiSize Y)
{
  MovedMouse(X,Y);
  return mInterceptMouse;
}

void nuiUserArea::SetInterceptMouse(bool intercept)
{
  mInterceptMouse = intercept;
}

bool nuiUserArea::GetInterceptMouse()
{
  return mInterceptMouse;
}

void nuiUserArea::EnableGLInit(bool Value)
{
  mPrepareGLContext = Value;
}

bool nuiUserArea::IsGLInitEnabled()
{
  return mPrepareGLContext;
}
