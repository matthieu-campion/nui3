/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiViewController.h"



nuiViewController::nuiViewController()
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiViewController"));
  mAnimated = false;
  
}



nuiViewController::~nuiViewController()
{
}

// virtual 
void nuiViewController::ConnectTopLevel()
{
  ViewDidAppear();
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


