/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiMesh.h
\brief A widget to display 3D meshes
*/

#ifndef __nuiMesh_h__
#define __nuiMesh_h__

#include "nglVector.h"
#include "nuiWidget.h"
#include "nuiContainer.h"
#include "nuiDrawContext.h"
#include "nuiUserArea.h"
#include "nuiMeshEngine.h"

//! Three dimensional object viewer
/*!
This widget can load and draw moderately complex 3D models from 3D Studio files. It also
embeds some basic navigation capabilities.

If left mouse button is pressed :

- the mouse moves the camera around its target point
- with the \e Control key held, pans the camera left/right and up/down
- with the \e Shift key held, zoom the camera in/out

The mouse wheel button cycles through cameras (if several are defined).

And here are the hot keys :

- D   : demo mode, ie. continuous rotation around a vertical axis
- +/- : adjust demo rotation speed
- L   : turn on/off lighting
- M   : turn on/off material rendering
- W   : turn on/off wireframe
*/
class NUI_API nuiMesh : public nuiUserArea
{
public:
  nuiMesh();
  virtual bool Load(const nuiXMLNode* pNode);
  virtual ~nuiMesh();

  bool Load (const nglPath& rPath);
  /*!< Load a mesh file
    \param rPath path to a 3DS file (.3ds or .prj extension)
    \return true if the file was succesfully loaded

    This method can be called any number of times (ie. it will automatically replace
    a previously loaded mesh), that is a nuiMesh instance can be re-used at will.
  */
  void SetEngine (nuiMeshEngine* pEngine);
  /*!< Provide a user-defined loading and rendering engine
    \param pEngine an instance of a nuiMeshEngine based object

    The actual loading and rendering happens in the nuiMeshEngine helper class.
    It is possible to support alternate loading or rendering methods : provide
    your own nuiMeshEngine implementation here.

    The engine can be changed at any time, any numbe of times. When Load() is
    called and no engine was supplied by the user, an instance of nuiMeshEngine
    is used.
  */

protected:
  enum ViewMode
  {
    eViewNone,
    eViewRotate,
    eViewZoom,
    eViewPan
  };

  nuiMeshEngine* mpEngine;
  ViewMode       mViewMode;
  nglVectorf     mHorizon;
  nglVectorf     mUp;

  virtual bool RequestSize (nuiRect& rRect, nuiDrawContext* pContext);
  virtual bool SetSize (const nuiRect& rRect);

  virtual bool KeyDown (const nglKeyEvent& rEvent);
  virtual bool KeyUp   (const nglKeyEvent& rEvent);

  virtual bool MouseClicked   (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved (nuiSize X, nuiSize Y);

  bool OnUserDraw (const nuiEvent& rEvent);

  virtual void StartView (nuiSize X, nuiSize Y);
  virtual bool UpdateView(float DeltaX, float DeltaY);
  virtual void EndView   ();

private:
  nuiEventSink<nuiMesh> mUserDrawSink;

  bool           mDemo;
  float          mDemoSpeed;
  nglTime        mDemoStamp;
  float          mXRef, mYRef;
  float          mXScale, mYScale;
  nglVectorf     mEyeRef;
  nglVectorf     mTargetRef;
  float          mGimbalAngle;

  void Init();
};


#endif // __nuiMesh_h__
