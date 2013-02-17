/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiUserArea::nuiUserArea()
 : nuiWidget() 
{
  SetObjectClass(_T("nuiUserArea"));
  mInterceptMouse = false;
  mPrepareGLContext = false;
  
  NUI_ADD_EVENT(UserDraw);
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
    nuiRenderArray* pDummy = new nuiRenderArray(GL_POINTS);
    pContext->DrawArray(pDummy);

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
    nuiRenderArray* pDummy = new nuiRenderArray(GL_POINTS);
    pContext->DrawArray(pDummy);
  }
  return true;
}

bool nuiUserArea::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return mInterceptMouse;
}

bool nuiUserArea::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  return mInterceptMouse;
}

bool nuiUserArea::MouseMoved  (nuiSize X, nuiSize Y)
{
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
