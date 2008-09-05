/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiMeshEngine.h
\brief Mesh loader and renderer for nuiMesh
*/

#ifndef __nuiMeshEngine_h__
#define __nuiMeshEngine_h__

#include "ngl3DSLoader.h"
#include "nglMatrix.h"
#include "nuiRect.h"
#include "nuiDrawContext.h"


//! Loader and renderer helper for the nuiMesh widget
/*!
This class does the actual job of loading mesh files (using ngl3DSLoader) and
rendering them (using straight forward OpenGL code). Currently supports :

- colored materials (no translucency and no texture for now). If no material is
defined or assigned, a default white dull material is used.

- smoothed surfaces (using smoothing groups, rendering is still Gouraud)

- all camera settings except roll/tilt factor. If no camera is defined, a
default one which fits the whole scene and looks into the Z direction is
automatically set up.

- ambient, omni and spot lights. If no light is defined, an omni is automatically
created. The lighting model is not matching the 3DS one, and specular has been
turned off until a satisfying solution has been found.

A straight forward API lets you manipulate lights and cameras easily. Have a look
at the nuiMesh navigation code for a complete example.
*/
class NUI_API nuiMeshEngine
{
public:
  static const uint ModeNoLight;
  static const uint ModeNoMaterial;
  static const uint ModeWireFrame;
  static const uint ModeShowNormals;

  nuiMeshEngine(nglContext* pContext = NULL);
  virtual ~nuiMeshEngine();

  bool Load (const nglPath& rPath, int VerboseLevel);

  virtual void Draw(nuiDrawContext* pContext, const nuiRect& rViewPort);
  uint GetMode     ();
  void SetMode     (uint Mode);
  bool SetCamera   (uint Index);
  uint GetCamera   ();
  uint GetCameraCnt();
  void GetCamera   (nglVectorf* pEye, nglVectorf* pLookAt);
  void SetCamera   (const nglVectorf* pEye, const nglVectorf* pLookAt);
  uint GetLightCnt  ();
  void GetLight    (uint Light, nglVectorf* pSource, nglVectorf* pTarget);
  void SetLight    (uint Light, const nglVectorf* pSource, const nglVectorf* pTarget);

protected:
  typedef struct
  {
    uint    mMaterial;
    GLuint  mList;
  } ElementType;

  void Init   (ngl3DSLoader* pLoader = NULL);
  void Release();

  // ngl3DSLoader callbacks
  virtual bool OnHeader    (bool Is3DS, nglSize Size);
  virtual bool OnScene     (const ngl3DSScene& rScene);
  virtual bool OnMaterial  (const ngl3DSMaterial& rMat);
  virtual bool OnLight     (const ngl3DSLight& rLight);
  virtual bool OnCamera    (const ngl3DSCamera& rCam);
  virtual bool OnMesh      (const ngl3DSMesh& rMesh);

  virtual void OnDraw   (nuiDrawContext* pContext);
  virtual void OnElement(const ngl3DSMesh& rMesh, const ElementType& rElement);
  virtual void OnFace   (const ngl3DSMesh& rMesh, uint16 FaceID, nglVector3f* pNormals, bool Smoothing, bool WireFrame);

  virtual uint FindMaterial (const char* pName);
  virtual void SetupMaterial(uint Index);
  virtual void SetupCamera  (float Width, float Height);
  virtual void SetupLighting(nuiDrawContext* pContext);
  virtual void SetupLight   (nuiDrawContext* pContext, uint Index);

  nglContext*                 mpContext;
  ngl3DSLoader*               mpLoader;

  ngl3DSScene                 mScene;
  std::vector<ElementType>    mElement;
  std::vector<ngl3DSMaterial> mMaterial;
  std::vector<ngl3DSLight>    mLight;
  std::vector<ngl3DSCamera>   mCamera;
  uint                        mCurrentCamera;
  nglVectorf                  mMinima;
  nglVectorf                  mMaxima;
  uint                        mMode;

  friend class nuiMeshLoader;
};

#endif // __nuiMeshEngine_h__
