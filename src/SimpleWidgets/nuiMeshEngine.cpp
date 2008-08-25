/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiMeshEngine.h"
#include "nglMatrix.h"

using namespace std;

/* TODO
* ~ auto camera when there's none
* ~ auto light when there's none
*/

const uint nuiMeshEngine::ModeNoLight     = 1 << 0;
const uint nuiMeshEngine::ModeNoMaterial  = 1 << 1;
const uint nuiMeshEngine::ModeWireFrame   = 1 << 2;
const uint nuiMeshEngine::ModeShowNormals = 1 << 3;


/*
* nuiMeshEngine private loader
*/

class nuiMeshLoader : public ngl3DSLoader
{
public:
  nuiMeshLoader(const nglPath& rPath, nuiMeshEngine* pEngine) : ngl3DSLoader(rPath)
  {
    mpEngine = pEngine;
  }

  bool OnHeader    (bool Is3DS, nglSize Size)   { return mpEngine->OnHeader(Is3DS, Size); }
  bool OnScene     (const ngl3DSScene& rScene)  { return mpEngine->OnScene(rScene); }
  bool OnMaterial  (const ngl3DSMaterial& rMat) { return mpEngine->OnMaterial(rMat); }
  bool OnLight     (const ngl3DSLight& rLight)  { return mpEngine->OnLight(rLight); }
  bool OnCamera    (const ngl3DSCamera& rCam)   { return mpEngine->OnCamera(rCam); }
  bool OnMesh      (const ngl3DSMesh& rMesh)    { return mpEngine->OnMesh(rMesh); }

private:
  nuiMeshEngine* mpEngine;
};



/*
* Life cycle
*/

nuiMeshEngine::nuiMeshEngine (nglContext* pContext)
{
  mpContext = pContext;

  Init();
};

nuiMeshEngine::~nuiMeshEngine()
{
  Release();
}

void nuiMeshEngine::Init(ngl3DSLoader* pLoader)
{
  mpLoader = pLoader;

  mMode = 0;
  mCurrentCamera = 0;
  mMinima = nglVectorf(1E30f, 1E30f, 1E30f);
  mMaxima = nglVectorf(-1E30f, -1E30f, -1E30f);
}

void nuiMeshEngine::Release()
{
  uint i;

#ifndef __NUI_NO_GL__
  for (i = 0; i < mElement.size(); i++)
    glDeleteLists(mElement[i].mList, 1);
#endif

  mElement.clear();
  mMaterial.clear();
  mLight.clear();
  mCamera.clear();

  if (mpLoader)
    delete mpLoader;
}


/*
* Loader
*/

bool nuiMeshEngine::Load (const nglPath& rPath, int VerboseLevel)
{
  // Allow multiple Load() calls, clear any previously loaded data, and re-init
  bool res = true;
  Release();
  Init(new nuiMeshLoader(rPath, this));

  /* Create a default material, some meshes are not assigned to one.
  * Some white dull default stuff will do.
  */
  {
    ngl3DSMaterial mat;

    strcpy(mat.mpName, "[default]");
    mat.mAmbient.mR  = .5f;  mat.mAmbient.mG  = .5f;  mat.mAmbient.mB  = .5f;
    mat.mDiffuse.mR  = 1.f;  mat.mDiffuse.mG  = 1.f;  mat.mDiffuse.mB  = 1.f;
    mat.mSpecular.mR = 0.f;  mat.mSpecular.mG = 0.f;  mat.mSpecular.mB = 0.f;
    mat.mShiny = .25f;
    mat.mShinyStrength = 0.25f;
    mat.mTransp = 0.f;
    mat.mTranspFalloff = 0.f;
    mat.mShading = e3DSShadingPhong;
    mat.mpTexture[0].mUsed = false;
    mat.mpTexture[1].mUsed = false;
    mMaterial.push_back(mat);
  }

  /* We don't check if Load() succeeds, because we continue even if a
  * only a partial load occured (we make provisions for what could be
  * missing a little further).
  */
  if (!mpLoader->Load(VerboseLevel))
  {
    NGL_DEBUG( NGL_LOG(_T("3ds"), NGL_LOG_WARNING, _T("loading incomplete")); )
      res = false;
  }

  if (!mCamera.size())
  {
    // No camera, build a fitting one
    ngl3DSCamera cam;
    nglVectorf c = (mMaxima + mMinima) * .5f; // This is the center of the scene bbox
    nglVectorf d = (mMaxima - mMinima); // Each component holds its respective bbox dimension
    float xy_max = MAX(d[0], d[1]) * 1.8f;
    float z_max  = d[2] * 2.2f;

    cam.mPos.mX    = c[0];
    cam.mPos.mY    = c[1];
    cam.mPos.mZ    = c[2] + MAX(xy_max, z_max);
    cam.mTarget.mX = c[0];
    cam.mTarget.mY = c[1];
    cam.mTarget.mZ = c[2];
    cam.mRoll      = 0.f;  // degree
    cam.mLens      = 65.f; // mm
    mCamera.push_back(cam);
  }

  if (!mLight.size())
  {
    // No light, let's create one 'somewhere' (FIXME: needs scene bbox)
    ngl3DSLight light;

    light.mType = e3DSLightOmni;
    light.mPos.mX = 50.f;
    light.mPos.mY = 50.f;
    light.mPos.mZ = 250.f;
    light.mColor.mR = 1.f;
    light.mColor.mG = 1.f;
    light.mColor.mB = 1.f;
    light.mMultiplier = 1.f;
    mLight.push_back(light);
  }

  return res;
}

/*
* Renderer
*/

void nuiMeshEngine::Draw (nuiDrawContext* pContext, const nuiRect& rViewPort)
{
  if (!mCamera.size())
    return;

  // Setup context
  // Setup background
  ngl3DSColor& bg = mScene.mpBgColor[0];
#ifndef __NUI_NO_GL__
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(bg.mR, bg.mG, bg.mB, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
#endif

  // Save projection and movelview matrices
#ifndef __NUI_NO_GL__
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
#endif

  // Camera
  SetupCamera(rViewPort.GetWidth(), rViewPort.GetHeight());

  // Lighting
  bool has_light = !(mMode & ModeNoLight);
  if (has_light)
  {
    glEnable(GL_LIGHTING);
    SetupLighting(pContext);
  }
  else
    glDisable(GL_LIGHTING);

  // Setup default material once in 'no material' mode
  if (mMode & ModeNoMaterial)
    SetupMaterial(0);

  // Setup fill mode (FIXME: we don't restore this context bit)
#ifndef __NUI_NO_GL__
  glPolygonMode(GL_FRONT, (mMode & ModeWireFrame) ? GL_LINE : GL_FILL);
#endif

  OnDraw(pContext);

  // Restore projection and modelview matrices
#ifndef __NUI_NO_GL__
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  // Restore model view matrix after the projection one so that we stay in this mode afterwards
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glPopAttrib();
#endif
}

uint nuiMeshEngine::GetMode()
{
  return mMode;
}

void nuiMeshEngine::SetMode (uint Mode)
{
  mMode = Mode;
}


/*
* Camera setup
*/

bool nuiMeshEngine::SetCamera (uint Index)
{
  if (Index < mCamera.size())
  {
    mCurrentCamera = Index;
    return true;
  }
  return false;
}

uint nuiMeshEngine::GetCamera()
{
  return mCurrentCamera;
}

uint nuiMeshEngine::GetCameraCnt()
{
  return (uint)mCamera.size();
}

void nuiMeshEngine::SetCamera (const nglVectorf* pEye, const nglVectorf* pLookAt)
{
  if (!mCamera.size())
    return;
  ngl3DSCamera& cam = mCamera[mCurrentCamera];

  if (pEye)
  {
    cam.mPos.mX = (*pEye)[0];
    cam.mPos.mY = (*pEye)[1];
    cam.mPos.mZ = (*pEye)[2];
  }
  if (pLookAt)
  {
    cam.mTarget.mX = (*pLookAt)[0];
    cam.mTarget.mY = (*pLookAt)[1];
    cam.mTarget.mZ = (*pLookAt)[2];
  }
}

void nuiMeshEngine::GetCamera (nglVectorf* pEye, nglVectorf* pLookAt)
{
  if (!mCamera.size())
    return;
  ngl3DSCamera& cam = mCamera[mCurrentCamera];

  if (pEye)
    *pEye = nglVectorf(cam.mPos.mX, cam.mPos.mY, cam.mPos.mZ);
  if (pLookAt)
    *pLookAt = nglVectorf(cam.mTarget.mX, cam.mTarget.mY, cam.mTarget.mZ);
}

uint nuiMeshEngine::GetLightCnt()
{
  return (uint)mLight.size();
}

void nuiMeshEngine::GetLight (uint Light, nglVectorf* pSource, nglVectorf* pTarget)
{
  if (Light >= mLight.size())
    return;
  ngl3DSLight& light = mLight[Light];

  if (pSource)
    *pSource = nglVectorf(light.mPos.mX, light.mPos.mY, light.mPos.mZ);
  if (pTarget)
    *pTarget = nglVectorf(light.mTarget.mX, light.mTarget.mY, light.mTarget.mZ);
}

void nuiMeshEngine::SetLight (uint Light, const nglVectorf* pSource, const nglVectorf* pTarget)
{
  if (Light >= mLight.size())
    return;
  ngl3DSLight& light = mLight[Light];

  if (pSource)
  {
    light.mPos.mX = (*pSource)[0];
    light.mPos.mY = (*pSource)[1];
    light.mPos.mZ = (*pSource)[2];
  }
  if (pTarget)
  {
    light.mTarget.mX = (*pTarget)[0];
    light.mTarget.mY = (*pTarget)[1];
    light.mTarget.mZ = (*pTarget)[2];
  }
}


/*
* ngl3DSLoader callbacks
*/

bool nuiMeshEngine::OnHeader (bool Is3DS, nglSize Size)
{
  return Is3DS;
}

bool nuiMeshEngine::OnScene (const ngl3DSScene& rScene)
{
  mScene = rScene; return true;
}

bool nuiMeshEngine::OnMaterial (const ngl3DSMaterial& rMat)
{
  mMaterial.push_back(rMat); return true;
}

bool nuiMeshEngine::OnLight (const ngl3DSLight& rLight)
{
  mLight.push_back(rLight); return true;
}

bool nuiMeshEngine::OnCamera (const ngl3DSCamera& rCam)
{
  mCamera.push_back(rCam); return true;
}

bool nuiMeshEngine::OnMesh (const ngl3DSMesh& rMesh)
{
  uint i, e;
  uint elt_cnt = MAX(1, rMesh.mMaterialCnt);
  GLuint list_base = 0;

#ifndef __NUI_NO_GL__
  list_base = glGenLists(elt_cnt);
#endif
  if (!list_base)
  {
    NGL_DEBUG( NGL_LOG(0, NGL_LOG_WARNING, _T("nuiMeshEngine::OnMesh: glGenLists(%d) failed"), rMesh.mMaterialCnt); )
      return true; // Continue anyway
  }

#if 0
  nglMatrixf pos;

  // First column, X axis
  pos.Elt.M11 = rMesh.mpLocalRef[0].mX;
  pos.Elt.M21 = rMesh.mpLocalRef[0].mY;
  pos.Elt.M31 = rMesh.mpLocalRef[0].mZ;
  pos.Elt.M41 = 0.0f;
  // Second column, Y axis
  pos.Elt.M12 = rMesh.mpLocalRef[1].mX;
  pos.Elt.M22 = rMesh.mpLocalRef[1].mY;
  pos.Elt.M32 = rMesh.mpLocalRef[1].mZ;
  pos.Elt.M42 = 0.0f;
  // Third column, X axis
  pos.Elt.M13 = rMesh.mpLocalRef[2].mX;
  pos.Elt.M23 = rMesh.mpLocalRef[2].mY;
  pos.Elt.M33 = rMesh.mpLocalRef[2].mZ;
  pos.Elt.M43 = 0.0f;
  // Fourth column, origin (translation)
  pos.Elt.M14 = rMesh.mpLocalRef[3].mX;
  pos.Elt.M24 = rMesh.mpLocalRef[3].mY;
  pos.Elt.M34 = rMesh.mpLocalRef[3].mZ;
  pos.Elt.M44 = 1.0f;
#endif

  /* Compute face normals
  */
  nglVector3f* face_n = new nglVector3f[rMesh.mFaceCnt];

  for (i = 0; i < rMesh.mFaceCnt; i++)
  {
    ngl3DSFace& face = rMesh.mpFace[i];
    ngl3DSVertex& a = rMesh.mpVertex[face.mpVertex[0]];
    ngl3DSVertex& b = rMesh.mpVertex[face.mpVertex[1]];
    ngl3DSVertex& c = rMesh.mpVertex[face.mpVertex[2]];

    face_n[i] =  nglVector3f(b.mX - a.mX, b.mY - a.mY, b.mZ - a.mZ);
    face_n[i] ^= nglVector3f(c.mX - a.mX, c.mY - a.mY, c.mZ - a.mZ);
    face_n[i].Normalize();
  }

  /* Storage for per-vertex normals (smoothing only)
  */
  nglVector3f* vertex_n = rMesh.mpSmoothing ? new nglVector3f[rMesh.mVertexCnt] : NULL;

  /* Decompose mesh into elements, each element having its own material
  */
  for (e = 0; e < elt_cnt; e++)
  {
    ElementType elt;
    ngl3DSFaceMaterial* mat = NULL;

    elt.mList = list_base + e;   // Store the triangle list name
    if (rMesh.mMaterialCnt)
    {
      mat = &rMesh.mpMaterial[e];

      elt.mMaterial = FindMaterial(mat->mpMaterialName);  // Fetch material ID
    }
    else
      elt.mMaterial = 0; //  Use (our) default material (see Load() for its settings)

    /* Build a GL list which pushes the element's polygons
    */
#ifndef __NUI_NO_GL__
    glNewList(elt.mList, GL_COMPILE);
    {
      uint face;
      uint face_cnt = mat ? mat->mFaceCnt : rMesh.mFaceCnt;
      bool wire = (mMaterial[elt.mMaterial].mShading == e3DSShadingWire);

      if (!wire)
        glBegin(GL_TRIANGLES);

      OnElement(rMesh, elt);

      if (rMesh.mpSmoothing)
      {
        /* Smoothing groups are defined. We make one pass per group,
        * computing per-vertex normals on each pass
        */
        uint32 group;
        nglVector3f zero;

        // Parse the 32 groups, going through the bitfield from right to left
        for (group = 1; group; group <<= 1)
        {
          // Initialize per-vertex normals (to zero)
          for (i = 0; i < rMesh.mVertexCnt; i++)
            vertex_n[i] = zero;

          // Cumulate this group's faces participation to vertex normals
          for (face = 0; face < face_cnt; face++)
          {
            uint16 face_id = mat ? mat->mpFace[face] : face;

            if (rMesh.mpSmoothing[face_id] & group)
            {
              ngl3DSFace&  face_def = rMesh.mpFace[face_id];
              nglVector3f& face_normal = face_n[face_id];
              uint v;

              /* #face_id's normal is added to its 3 vertices' normals
              * (we will renormalize vertex normals later)
              */
              for (v = 0; v < 3; v++)
              {
                uint16 vertex_id = face_def.mpVertex[v];

                vertex_n[vertex_id] += face_normal;
              }
            }
          }

          // Normalize normals (yes, sounds funny)
          for (i = 0; i < rMesh.mVertexCnt; i++)
            vertex_n[i].Normalize();

          // Output faces from this group
          for (face = 0; face < face_cnt; face++)
          {
            uint16 face_id = mat ? mat->mpFace[face] : face;

            if (rMesh.mpSmoothing[face_id] & group)
              OnFace(rMesh, face_id, vertex_n, /*smoothing*/ true, /*wireframe*/ wire);
          }
        } /* group */
      }
      else
      {
        /* No smoothing groups, use per-face normals
        */
        for (face = 0; face < face_cnt; face++)
          OnFace(rMesh, mat ? mat->mpFace[face] : face, face_n, /*smoothing*/ false, /*wireframe*/ wire);
      }

      if (!wire)
        glEnd(); // GL_TRIANGLES
    }
    glEndList();
#endif

    mElement.push_back(elt);
  }

  if (vertex_n) delete[] vertex_n;
  if (face_n) delete[] face_n;

  return true;
}


/*
* Internals
*/

/* OnDraw
*
* Called inside the drawing loop, when the modelview an projection matrixes are
* properly setup. This one contain the 'regular' rendering pass.
*/
void nuiMeshEngine::OnDraw (nuiDrawContext* pContext)
{
  uint i;

  for (i = 0; i < mElement.size(); i++)
  {
    ElementType& elt = mElement[i];

    if (!(mMode & ModeNoMaterial))
      SetupMaterial(elt.mMaterial);
#ifndef __NUI_NO_GL__
    glCallList(elt.mList);
#endif

  }
}

/* OnElement
*
* This callback is invoked inside a glBegin/glEnd pair (except for wireframe materials)
* for every element (a set of polygons with the same material). This call only occurs
* once, at load time, when display lists are created.
*/
void nuiMeshEngine::OnElement (const ngl3DSMesh& rMesh, const ElementType& rElement)
{
}

/* OnFace
*
* - if Smoothing is false, pNormals is a face-indexed array (ie range is [0 .. rMesh.mFaceCnt[)
*   and face normals are expected
*
* - if Smoothing is true, pNormals is a vertex-indexed array (ie range is [0 .. rMesh.mVertecCnt[)
*   and per-vertex normals are expected
*/
void nuiMeshEngine::OnFace (const ngl3DSMesh& rMesh, uint16 FaceID, nglVector3f* pNormals, bool Smoothing, bool WireFrame)
{
  ngl3DSFace& face = rMesh.mpFace[FaceID];
  uint v;

#ifndef __NUI_NO_GL__
  if (WireFrame)
    glBegin(GL_LINE_LOOP);

  // If we have no smoothing groups, we use the same normal for all 3 vertices
  if (!Smoothing)
    glNormal3fv(pNormals[FaceID].Elt);

  for (v = 0; v < 3; v++)
  {
    uint16        vertex_id = face.mpVertex[v];
    ngl3DSVertex& vertex    = rMesh.mpVertex[vertex_id];

    if (Smoothing)
      glNormal3fv(pNormals[vertex_id].Elt); // We have per-vertex normals (smoothing)

    glVertex3fv(&vertex.mX);

    // Update bounding values
    if (vertex.mX < mMinima[0]) mMinima[0] = vertex.mX;
    if (vertex.mY < mMinima[1]) mMinima[1] = vertex.mY;
    if (vertex.mZ < mMinima[2]) mMinima[2] = vertex.mZ;
    if (vertex.mX > mMaxima[0]) mMaxima[0] = vertex.mX;
    if (vertex.mY > mMaxima[1]) mMaxima[1] = vertex.mY;
    if (vertex.mZ > mMaxima[2]) mMaxima[2] = vertex.mZ;
  }

  if (WireFrame)
    glEnd(); // GL_LINE_LOOP
#endif

}

uint nuiMeshEngine::FindMaterial (const char* pName)
{
  uint i;

  for (i = (uint)mMaterial.size(); i > 0; i--)
    if (!strcmp(mMaterial[i].mpName, pName))
      break;

  return i; // If not found, returns 0 as a default (it's built in Load())
}

void nuiMeshEngine::SetupMaterial (uint Index)
{
  ngl3DSMaterial& mat = mMaterial[Index];
  nglVectorf v;
  GLenum face = GL_FRONT;

#ifndef __NUI_NO_GL__
  v = nglVectorf(mat.mAmbient.mR, mat.mAmbient.mG, mat.mAmbient.mB, 1.0f);
  glMaterialfv(face, GL_AMBIENT, v.Elt);
  v = nglVectorf(mat.mDiffuse.mR, mat.mDiffuse.mG, mat.mDiffuse.mB, 1.0f);
  glMaterialfv(face, GL_DIFFUSE, v.Elt);
  v = nglVectorf(mat.mSpecular.mR, mat.mSpecular.mG, mat.mSpecular.mB, 1.0f);
  glMaterialfv(face, GL_SPECULAR, v.Elt);

  glMaterialf(face, GL_SHININESS, mat.mShiny * 128.f);

  switch (mat.mShading)
  {
  case e3DSShadingWire:
  case e3DSShadingFlat:
    glShadeModel(GL_FLAT);
    break;

  case e3DSShadingGouraud:
  case e3DSShadingPhong:
  case e3DSShadingMetal:
    glShadeModel(GL_SMOOTH);
    break;
  }
#endif
}

void nuiMeshEngine::SetupCamera (float Width, float Height)
{
  ngl3DSCamera& cam = mCamera[mCurrentCamera];
  nglVectorf eye   (cam.mPos.mX, cam.mPos.mY, cam.mPos.mZ);
  nglVectorf target(cam.mTarget.mX, cam.mTarget.mY, cam.mTarget.mZ);
  nglVectorf axis = target - eye;
  axis.Normalize();

  // This is the film diagonal in mm, seems that 3DS uses 'classic' 24*36
#define FILM_DIAGONAL 42.2f
  GLfloat ratio = Width / Height;
  GLfloat fov = 2.f * (GLfloat)(atan((FILM_DIAGONAL / 2.f) / cam.mLens) * 180.f / M_PI);

  /* Find Z range using the scene boudings, projected along the camera axis
  */
#define Z_HARD_MIN 0.1f
  uint i;
  float zmin = 1E30f, zmax = -1E30f;
  for (i = 0; i < 8; i++)
  {
    // This is one of the 8 vertices from a cube built on the extrema values
    nglVectorf vertex(
      i & 1 ? mMinima[0] : mMaxima[0],
      i & 2 ? mMinima[1] : mMaxima[1],
      i & 4 ? mMinima[2] : mMaxima[2]);
    // Go in the camera referential
    vertex -= eye;
    // Project this vertex on the camera axis
    float z = vertex * axis;
    // Fetch extrema
    if (z < zmin) zmin = z;
    if (z > zmax) zmax = z;
  }
  if (zmin < Z_HARD_MIN) zmin = Z_HARD_MIN;

#ifndef __NUI_NO_GL__
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, ratio, zmin, zmax);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cam.mPos.mX, cam.mPos.mY, cam.mPos.mZ,
    cam.mTarget.mX, cam.mTarget.mY, cam.mTarget.mZ,
    0.f, 1.f, 0.f); // FIXME: need to use cam.mRoll here
#endif
}

void nuiMeshEngine::SetupLighting (nuiDrawContext* pContext)
{
  uint i;

  // Ambient light setup
#ifndef __NUI_NO_GL__
  if (mLight.size())
  {
    nglVectorf ambient(mScene.mAmbient.mR, mScene.mAmbient.mG, mScene.mAmbient.mB, 1.0f);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient.Elt);
  }

  // Specular is computed from eye coordinates (or should be ? FIXME)
  GLint on = 1;
  glLightModeliv(GL_LIGHT_MODEL_LOCAL_VIEWER, &on);
#endif

  // Omni and spots setup
  for (i = 0; i < mLight.size(); i++)
    SetupLight(pContext, i);
}

void nuiMeshEngine::SetupLight (nuiDrawContext* pContext, uint Index)
{
  ngl3DSLight& light = mLight[Index];
  GLenum light_id = GL_LIGHT0 + Index;
  nglVectorf v;

  glEnable(light_id);

#ifndef __NUI_NO_GL__
  nglVectorf pos(light.mPos.mX, light.mPos.mY, light.mPos.mZ, 1.0f);
  glLightfv(light_id, GL_POSITION, pos.Elt);

  // The first light gives the ambient term
  v = Index ?
    nglVectorf(0.f, 0.f, 0.f, 1.0f) :
    nglVectorf(mScene.mAmbient.mR, mScene.mAmbient.mG, mScene.mAmbient.mB, 1.0f);
  glLightfv(light_id, GL_AMBIENT, v.Elt);

  v = nglVectorf(light.mColor.mR, light.mColor.mG, light.mColor.mB, 1.0f);
  glLightfv(light_id, GL_DIFFUSE, v.Elt);

  // We lack '3DS shininess' informations (FIXME)
  v = nglVectorf(1.0f, 1.0f, 1.0f, 1.0f) * 0.f;
  glLightfv(light_id, GL_SPECULAR, v.Elt);

  glLightf(light_id, GL_SPOT_EXPONENT, 1.f);

  // No attenuation
  glLightf(light_id, GL_CONSTANT_ATTENUATION, 1.f);
  glLightf(light_id, GL_LINEAR_ATTENUATION, 0.f);
  glLightf(light_id, GL_QUADRATIC_ATTENUATION, 0.f);

  if (light.mType == e3DSLightSpot)
  {
    nglVectorf dir(light.mTarget.mX, light.mTarget.mY, light.mTarget.mZ, 1.0f);
    dir -= pos;
    glLightfv(light_id, GL_SPOT_DIRECTION, dir.Elt);

    glLightf(light_id, GL_SPOT_CUTOFF, light.mFalloff / 2.f);
  }
  else
    glLighti(light_id, GL_SPOT_CUTOFF, 180);
#endif
}
