/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  ngl3DSLoader.h
\brief Autodesk 3DS (r3/r4) binary file loader
*/

#ifndef __ngl3DSLoader__
#define __ngl3DSLoader__

//#include "nui.h"
#include "nglEndian.h"
class nglPath;
class nglFile;


/*
 * Basic types
 */

//! 3DS RGB color
/*!
There is no generic alpha support in 3DSr4.
*/
class NGL_API ngl3DSColor
{
public:
  float mR;  ///< Red component (0..1)
  float mG;  ///< Green component (0..1)
  float mB;  ///< Blue component (0..1)
};

//! 3DS vertex coordinates
/*!
These coordinates are subject to the global 'master scale', see ngl3DSScene.
*/
class NGL_API ngl3DSVertex
{
public:
  float mX;
  float mY;
  float mZ;
};

//! 3DS mapping coordinates
/*!
The texture image has the following UV coordinates :
- top left corner : 0,0
- bottom right corner : 1,1
Expect values not in the [0..1] range, since scaling and offseting is current
(you must wrap texture coordinates).
*/
class NGL_API ngl3DSMapping
{
public:
  float mU;
  float mV;
};

//! 3DS shading type
/*!
Notes :
- 'Wire' shading is still subjet to material parameters (texture, alpha, etc)
- 'Metal' is a proprietary algorithm of 3DS
*/
enum ngl3DSShading
{
  e3DSShadingWire    = 0,
  e3DSShadingFlat    = 1,
  e3DSShadingGouraud = 2,
  e3DSShadingPhong   = 3,
  e3DSShadingMetal   = 4
};

//! 3DS light model
/*!
There are only two types of light. The 'spot' is an 'omni' with some more infos,
see ngl3DSLight.
*/
enum ngl3DSLightType
{
  e3DSLightOmni,
  e3DSLightSpot
};

//! 3DS layered fog directions
/*!
Layered fog has a density varying along the vertical (Y) axis. A 'bottomed'
layered fog puts your feets in the smoke.
*/
enum ngl3DSLayerType
{
  e3DSLayerNone   = 0,
  e3DSLayerBottom = 1,
  e3DSLayerTop    = 2
};


/*
 * data structures
 */

#define TDS_SHORT_STR      10
#define TDS_LONG_STR       16

//! 3DS texture definition
class NGL_API ngl3DSTexture
{
public:
  bool   mUsed;
  char   mpName[TDS_SHORT_STR+1];  ///< Texture image file name, in 8.3 DOS format, upercased
  float  mUScale;
  float  mVScale;
  float  mUOffset;
  float  mVOffset;
};

//! 3DS material definition
class NGL_API ngl3DSMaterial
{
public:
  char          mpName[TDS_LONG_STR+1];
  ngl3DSColor   mAmbient;
  ngl3DSColor   mDiffuse;
  ngl3DSColor   mSpecular;
  float         mShiny;          ///< Shininess intensity (0..1)
  float         mShinyStrength;  ///< Shininess strength (from 0:flat to 1:sharp)
  float         mTransp;         ///< Transparency (from 0:opaque to 1:translucid)
  float         mTranspFalloff;  ///< Transparency falloff (if non-zero, decrease transparency with lighting)
  ngl3DSShading mShading;
  ngl3DSTexture mpTexture[2];    ///< A material can have up to 2 texture maps (see ngl3DSTexture:mUsed)
};

//! 3DS face description
class NGL_API ngl3DSFace
{
public:
  uint16 mpVertex[3];  ///< Face vertices, in trigonometric order. Values are vertices index in the current mesh vertex list.
};

//! 3DS per-material faces grouping
/*!
All materials applied to a mesh are listed together with a subset of faces mapped with
the given material. The \a mpFace array holds face indexes in the current mesh face list.
*/
class NGL_API ngl3DSFaceMaterial
{
public:
  char    mpMaterialName[TDS_LONG_STR+1];  ///< Material to apply (by name)
  uint    mFaceCnt; 
  uint16* mpFace;
};

//! 3DS mesh object
/*!
A mesh is a simple collection of triangular faces, with some local properties.
A mesh can be composed of many non-connected set of faces. The sub-mesh
notion of 'element' introduced by 3DStudio is not given here.
*/
class NGL_API ngl3DSMesh
{
public:
  char                mpName[TDS_SHORT_STR+1];
  uint                mVertexCnt;
  ngl3DSVertex*       mpVertex;      ///< Always hold \a mVertexCnt vertices
  ngl3DSMapping*      mpVertexUV;    ///< If the mesh is not mapped, is NULL. Else holds \a mVertexCnt mapping coordinates.
  uint                mFaceCnt;
  ngl3DSFace*         mpFace;        ///< Holds \a mFaceCnt face definitions
  uint32*             mpSmoothing;
  /*!< Smoothing groups.
    3DStudio defines 32 smoothing groups per mesh. Each group is represented by
    a bit set in a 32bit word. Each face can be part of 0 to 32 smoothing groups.
    When \a mpSmoothing is non-NULL, it holds \a mFaceCnt 32bit words
    giving every face ownership to respective smoothing groups.
  */
  uint                mMaterialCnt;   ///< A mesh can have any number of materials (including 0)
  ngl3DSFaceMaterial* mpMaterial;     ///< Holds \a mMaterialCnt material definition(s)
  ngl3DSVertex        mpLocalRef[4];  ///< Local coordinates system (X, Y and Z axes, and origin)
};

//! 3DS light object
class NGL_API ngl3DSLight
{
public:
  char            mpName[TDS_SHORT_STR+1];
  ngl3DSLightType mType;
  ngl3DSVertex    mPos;
  ngl3DSColor     mColor;
  float           mRangeStart;  ///< (shadows)
  float           mRangeEnd;    ///< (shadows)
  float           mMultiplier;  ///< Intensity multiplier

  // Spot specific
  ngl3DSVertex    mTarget;   ///< Spot only : \a mTarget - \a mPos gives the main direction
  float           mHotspot;  ///< Hotspot angle, degrees (full cone angle). Light in this cone is at full intensity.
  float           mFalloff;  ///< Falloff angle, degrees (full cone angle). There is no light below this cone.
};


//! 3DS camera object
class NGL_API ngl3DSCamera
{
public:
  char         mpName[TDS_SHORT_STR+1];
  ngl3DSVertex mPos;
  ngl3DSVertex mTarget;
  float        mRoll;   ///< In degree, 0 is 'man standing up along Y axis'
  float        mLens;   ///< Lens in mm (50mm is the human eye)
};


//! 3DS ambient fog settings
class NGL_API ngl3DSFog
{
public:
  bool            mUsed;
  float           mNearPlane;    ///< Start effect from this depth
  float           mNearDensity;  ///< Start effect with this density (from 0:clear to 1:smoky)
  float           mFarPlane;     ///< Stop effect above this depth
  float           mFarDensity;   ///< Reach given density in the background
  bool            mIsBackground; ///< Is rendered over objects or only on background
  ngl3DSColor     mColor;
};

//! 3DS layered fog settings
class NGL_API ngl3DSLayerFog
{
public:
  bool            mUsed;
  float           mBottom;       ///< Fog density at ground level
  float           mTop;          ///< Fog density at sky/roof level
  float           mDensity;      ///< Global density (from 0:clear to 1:smoky)
  ngl3DSLayerType mFalloff;      ///< Direction of attenuation
  bool            mIsBackground; ///< Is rendered over objects or only on background
  ngl3DSColor     mColor;
};

//! 3DS global scene parameters
/*!
Note than only fog type can be defined (ambient or layered). See \a mFog.mUsed and
\a mLayerFog.mUsed.
*/
class NGL_API ngl3DSScene
{
public:
  uint32         mVersion;     ///< 3DS version ('revision', mostly 3 or 4, might be 2)
  float          mMasterScale; ///< Master coordinate scale (used by units setting; often 1)
  ngl3DSColor    mAmbient;     ///< Ambient light (no direction, no attenuation, affects all objects)
  ngl3DSColor    mpBgColor[2]; ///< Background color gradient (from top to bottom - emm... not sure)
  ngl3DSFog      mFog;         ///< Fog settings
  ngl3DSLayerFog mLayerFog;    ///< Layered fog settings
};


#define NGL_3DSLOADER_LOGITEM   (1<<0)
#define NGL_3DSLOADER_LOGCHUNK  (1<<1)


//! Autodesk 3D Studio file loader
/*!
This class can load most static informations (no animations) from Autodesk 3DStudio
files. Format supported are .3DS or .PRJ binary files, from revision 2 to 4.
The loading process is event-driven : when the Load() method is invoked, the
different callbacks are called, once by object instance (mesh, camera, material, light)
and once for global settings (OnScene()).

The loader can currently parse only from a file. It has been tested against large
and various files and is considered robust.

This code does not make any use of the (copyrighted) Autodesk SDK content. It is
based on reverse engineering and consultation of the Wotsit.org documents.
Autodesk and 3D Studio are trade marks registered by their respective owners.
*/
class NGL_API ngl3DSLoader /*: public nglError*/
{
public:
  /** @name Life cycle */
  //@{
  ngl3DSLoader (const nglPath& rFileName);
  ngl3DSLoader (nglFile* pFile);
  virtual ~ngl3DSLoader ();
  //@}

  /** @name Loader interface */
  //@{
  bool Load (int VerboseLevel = 0);
  /*!<
    Start parsing.
    \param VerboseLevel
    - 0 : no output
    - 1 : show item list (with details)
    - 2 : show chunk tree
    - 3 : 1 and 2

    If the file was succesfully parsed, returns true. If an error occured (malformed file,
    unexpected end of file, user break), returns false. The user can stop parsing by
    returning false from any of the callbacks. The output uses the "3ds" log
    domain of the application log.
  */

  virtual bool OnHeader    (bool Is3DS, nglSize Size);
  /*!<
    Check 3DS signature.
    \param Is3DS 3DS signature found
    \param Size data size in bytes

    You can force loading even if \a Is3DS is false by returning true.
  */
  virtual bool OnMaterial  (const ngl3DSMaterial& rMat);  ///< New material
  virtual bool OnMesh      (const ngl3DSMesh& rMesh);     ///< New mesh
  virtual bool OnLight     (const ngl3DSLight& rLight);   ///< New light (omni or spot, NOT ambient)
  virtual bool OnCamera    (const ngl3DSCamera& rCam);    ///< New camera
  virtual bool OnScene     (const ngl3DSScene& rScene);   ///< Global settings (called once)
  //@}

private:
  nglFile*  mpFile;
  int       mOwnFile;
  nglEndian mSavedEndian;
  int       mVerbose;
  char      mBuffer[1024];
  // Stats
  uint      mCameraCnt;
  uint      mLightCnt;
  uint      mMaterialCnt;
  uint      mVertexCnt;
  uint      mFaceCnt;

  ngl3DSLoader (const ngl3DSLoader&) {} // Undefined copy constructor

  void   Log               (uint Level, const char* pText, ...);
  char*  Print             (const ngl3DSColor& rColor);
  char*  Print             (const ngl3DSVertex& rVertex);

  uint16 ReadChunkHeader   (uint32* pSize);
  void   ReadUnknown       (uint Level, uint16 ID, uint32 DataSize, bool IsLog = true);

  uint32 ReadString        (char* pString, uint32 DataSize);
  bool   ReadColorInt      (ngl3DSColor& rColor);
  bool   ReadColorFloat    (ngl3DSColor& rColor);
  bool   ReadColors        (uint Level, ngl3DSColor* pColor, uint32 DataSize);
  bool   ReadValue         (uint Level, float& rValue, uint32 DataSize);
  bool   ReadVertex        (ngl3DSVertex& rVertex);

  void   ZeroColor         (ngl3DSColor& rColor);
  void   IdMatrix          (ngl3DSVertex mat[4]);
  void   ZeroTexture       (ngl3DSTexture& rTex);

  bool   ReadHeader();
  bool   ReadProject       (uint32 DataSize);
  bool   ReadMain          (uint32 DataSize);
  bool   ReadEditor        (uint32 DataSize);
  bool   ReadKeyframer     (uint32 DataSize);

  bool   ReadFog           (ngl3DSFog& rFog, uint32 DataSize);
  bool   ReadLayerFog      (ngl3DSLayerFog& rFog, uint32 DataSize);

  bool   ReadMaterial      (uint32 DataSize);
  bool   ReadTexture       (ngl3DSTexture& rTex, uint32 DataSize);

  bool   ReadObject        (uint32 DataSize);

  bool   ReadMesh          (char* name, uint32 DataSize);
  void   FreeMesh          (ngl3DSMesh& rMesh);
  bool   ReadVertices      (ngl3DSMesh& rMesh, uint32 DataSize);
//  bool   ReadVertexGroup   (ngl3DSMesh& rMesh, uint32 DataSize);
  bool   ReadMapping       (ngl3DSMesh& rMesh, uint32 DataSize);
  bool   ReadFaces         (ngl3DSMesh& rMesh, uint32 DataSize);
  bool   ReadFacesMaterial (ngl3DSMesh& rMesh, uint32 DataSize);
  bool   ReadSmoothing     (ngl3DSMesh& rMesh, uint32 DataSize);

  bool   ReadLight         (char* name, uint32 DataSize);
  bool   ReadSpot          (ngl3DSLight& rLight, uint32 DataSize);

  bool   ReadCamera        (char* name, uint32 DataSize);
};

#endif // __ngl3DSLoader__
