/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiViewController.h"
#include "nuiNavigationBar.h"


nuiViewController::nuiViewController()
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiViewController"));
  mAnimated = false;
  mOverlayed = false;
  mLoaded = false;
  
  mpBar = new nuiNavigationBar();
}



nuiViewController::~nuiViewController()
{
}

// virtual 
void nuiViewController::ConnectTopLevel()
{
  if (!mLoaded)
  {
    mLoaded = true;
    ViewDidLoad();
  }
  
  ViewDidAppear();
}


// virtual 
void nuiViewController::ViewDidLoad()
{

}


// virtual 
void nuiViewController::ViewWillAppear()
{
  
}

// virtual 
void nuiViewController::ViewWillDisappear()
{

}


// virtual 
void nuiViewController::ViewDidAppear()
{
  
}


// virtual 
void nuiViewController::ViewDidDisappear()
{
  
}


void nuiViewController::SetTitle(const nglString& rTitle)
{
  mpBar->SetTitle(rTitle);
}

nuiNavigationBar* nuiViewController::GetNavigationBar()
{
  return mpBar;
} 


bool nuiViewController::IsOverlayed() const
{
  return mOverlayed;
}

void nuiViewController::SetOverlayed(bool set)
{
  mOverlayed = set;
}







