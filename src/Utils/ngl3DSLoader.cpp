/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"


#define DUMP

// 3DS header chunk
#define TDS_MAINPRJ       0xC23D
#define TDS_MDATA         0x3D3D
#define TDS_MAIN3DS       0x4D4D

// sub chunks of TDS_MAIN
#define TDS_VERSION       0x0002
#define TDS_STRANGE00     0x0008
#define TDS_EDIT          0x3D3D
#define TDS_KEYFRAMER     0xB000

// sub chunks of TDS_EDIT
#define TDS_EDIT_VERSION  0x3D3E
#define TDS_EDIT_MATERIAL 0xAFFF
#define TDS_EDIT_MSCALE   0x0100
#define TDS_EDIT_BGCOLOR  0x1200
#define TDS_EDIT_AMBIENT  0x2100
#define TDS_EDIT_FOG      0x2200
#define TDS_EDIT_USEFOG   0x2201
#define TDS_EDIT_FOGBGND  0x2210
#define TDS_EDIT_LFOG     0x2302
#define TDS_EDIT_USELFOG  0x2303
#define TDS_EDIT_OBJECT   0x4000

// sub chunks of TDS_EDIT_MATERIAL
#define TDS_MAT_NAME      0xA000
#define TDS_MAT_AMBIENT   0xA010
#define TDS_MAT_DIFFUSE   0xA020
#define TDS_MAT_SPECULAR  0xA030
#define TDS_MAT_SHINY     0xA040
#define TDS_MAT_SHINY_STR 0xA041
#define TDS_MAT_TRANS     0xA050
#define TDS_MAT_TRANS_OFF 0xA052
#define TDS_MAT_REFBLUR   0xA053
#define TDS_MAT_SELFILLUM 0xA084
#define TDS_MAT_SHADING   0xA100
#define TDS_MAT_TEXTURE1  0xA200
#define TDS_MAT_TEXNAME   0xA300
#define TDS_MAT_TEXFLAGS  0xA351
#define TDS_MAT_VSCALE    0xA354
#define TDS_MAT_USCALE    0xA356
#define TDS_MAT_UOFFSET   0xA358
#define TDS_MAT_VOFFSET   0xA35A
#define TDS_MAT_TEXTURE2  0xA33A

// sub chunks of TDS_EDIT_OBJECT
#define TDS_OBJ_MESH      0x4100
#define TDS_OBJ_LIGHT     0x4600
#define TDS_OBJ_CAMERA    0x4700

// sub chunks of TDS_OBJ_LIGHT
#define TDS_LIT_OFF       0x4620
#define TDS_LIT_SPOT      0x4610

// sub chunks of TDS_OBJ_MESH
#define TDS_MESH_VERTICES 0x4110
#define TDS_MESH_FACEL2   0x4111
#define TDS_MESH_FACEL1   0x4120
#define TDS_MESH_MATERIAL 0x4130
#define TDS_MESH_MAPPING  0x4140
#define TDS_MESH_SMOOTH   0x4150
#define TDS_MESH_LOCAL    0x4160
#define TDS_MESH_COLOR    0x4165

// light sub chunks
#define TDS_LIGHT_SPOT    0x4610
#define TDS_LIGHT_RSTART  0x4659
#define TDS_LIGHT_REND    0x465A
#define TDS_LIGHT_MULT    0x465B

// light sub chunks
#define TDS_SPOT_ROLL     0x4656

// color sub chunks
#define TDS_COL_FLOAT     0x0010
#define TDS_COL_INT       0x0011
#define TDS_COL_INTG      0x0012
#define TDS_COL_FLOATG    0x0013

// value cunks
#define TDS_PERCENT_INT   0x0030
#define TDS_PERCENT_FLOAT 0x0031


const char* pg3DSShadingStr[5] = {
  "Wire",
  "Flat",
  "Gouraud",
  "Phong",
  "Metal"
};


ngl3DSLoader::ngl3DSLoader (const nglPath& rFileName)
{
  mpFile = new nglFile (rFileName, eFileRead);
  mpFile->SetEndian(eEndianIntel);
  mOwnFile = true;
}

ngl3DSLoader::ngl3DSLoader (nglFile* pFile)
{
  mpFile = pFile;
  mOwnFile = false;
  if (mpFile)
  {
    mSavedEndian = mpFile->GetEndian();
    mpFile->SetEndian (eEndianIntel);
  }
}

ngl3DSLoader::~ngl3DSLoader()
{
  if (!mpFile) return;
  if (mOwnFile)
    delete mpFile;
  else
    mpFile->SetEndian (mSavedEndian);
}

bool ngl3DSLoader::Load (int VerboseLevel)
{
  mCameraCnt   = 0;
  mLightCnt    = 0;
  mMaterialCnt = 0;
  mVertexCnt   = 0;
  mFaceCnt     = 0;

  if ((!mpFile) || (!mpFile->IsOpen())) return false;
  mVerbose = VerboseLevel;
  if (ReadHeader())
  {
#ifdef _DEBUG_
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("%d camera%s, %d light%s, %d material%s, %d vertices, %d faces"),
      mCameraCnt, PLURAL(mCameraCnt),
      mLightCnt, PLURAL(mLightCnt),
      mMaterialCnt, PLURAL(mMaterialCnt),
      mVertexCnt,
      mFaceCnt);
#endif
    return true;
  }
  return false;
}



bool ngl3DSLoader::OnHeader(bool Is3DS, nglSize Size)
{
  if (Is3DS)
  {
    Log (0,"Recognized 3DS file (%d bytes)", Size);
    return true;
  }
  else
  {
    Log (0, "not a 3DS file !");
    return false;
  }
}

bool ngl3DSLoader::OnMaterial (const ngl3DSMaterial& rMat)
{
  if (!(mVerbose & NGL_3DSLOADER_LOGITEM)) return true;
  if (mVerbose & NGL_3DSLOADER_LOGCHUNK) NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("[OnMaterial]\n"));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("name    : '%s'\n"), rMat.mpName);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("ambient : %s\n"), Print(rMat.mAmbient));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("diffuse : %s\n"), Print(rMat.mDiffuse));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("specular: %s\n"), Print(rMat.mSpecular));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("shininess   : factor=%.3f  strength=%.3f\n"), rMat.mShiny, rMat.mShinyStrength);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("transparency: factor=%.3f  falloff=%.3f\n"), rMat.mTransp, rMat.mTranspFalloff);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("shading : %s\n"), pg3DSShadingStr[rMat.mShading]);
  int i;
  for (i=0; i<2; i++)
  if (*rMat.mpTexture[i].mpName != 0)
  {
    const ngl3DSTexture& tex = rMat.mpTexture[i];

    if (tex.mUsed)
    {
      NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("texture[%d]:\n"), i);
      NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  name     : '%s'\n"), tex.mpName);
      NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  UV scale : %.3f x %.3f\n"), tex.mUScale, tex.mVScale);
      NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  UF offset: %.3f , %.3f\n"), tex.mUOffset, tex.mVOffset);
    }
  }
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));
  return true;
}

bool ngl3DSLoader::OnMesh (const ngl3DSMesh& rMesh)
{
  uint i;

  if (!(mVerbose & NGL_3DSLOADER_LOGITEM)) return true;
  if (mVerbose & NGL_3DSLOADER_LOGCHUNK) NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("[OnMesh]\n"));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("name     : '%s'\n"), rMesh.mpName);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("vertices : %d\n"), rMesh.mVertexCnt);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("faces    : %d\n"), rMesh.mFaceCnt);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("materials: %d\n"), rMesh.mMaterialCnt);
  if (rMesh.mpSmoothing)
  {
    uint32 accum = 0;
    uint count = 0;

    for (i = 0; i < rMesh.mFaceCnt; i++) accum |= rMesh.mpSmoothing[i];
    for (i = 0; i < 32; i++) if ((accum >> i) & 0x1) count++;
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("smoothing: %d group%s\n"), count, (count > 1) ? _T("s") : _T(""));
  }
  for (i=0; i < rMesh.mMaterialCnt; i++)
  {
    ngl3DSFaceMaterial& fmat = rMesh.mpMaterial[i];

    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  '%s': %d faces\n"), fmat.mpMaterialName, fmat.mFaceCnt);
  }
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("local ref     :\n"));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  X: %s\n"), Print(rMesh.mpLocalRef[0]));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  Y: %s\n"), Print(rMesh.mpLocalRef[1]));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  Z: %s\n"), Print(rMesh.mpLocalRef[2]));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  O: %s\n"), Print(rMesh.mpLocalRef[3]));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));
  return true;
}

bool ngl3DSLoader::OnLight (const ngl3DSLight& rLight)
{
  if (!(mVerbose & NGL_3DSLOADER_LOGITEM)) return true;
  if (mVerbose & NGL_3DSLOADER_LOGCHUNK) NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("[OnLight]\n"));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("name      : '%s'\n"), rLight.mpName);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("type      : %s\n"), (rLight.mType == e3DSLightOmni) ? "omni" : "spot");
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("pos       : %s\n"), Print(rLight.mPos));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("color     : %s\n"), Print(rLight.mColor));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("range     : %.1f -> %.1f\n"), rLight.mRangeStart, rLight.mRangeEnd);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("multiplier: %.1f\n"), rLight.mMultiplier);
  if (rLight.mType == e3DSLightSpot)
  {
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("target    : %s\n"), Print(rLight.mTarget));
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("hotspot   : %.1f\n"), rLight.mHotspot);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("falloff   : %.1f\n"), rLight.mFalloff);
  }
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));
  return true;
}

bool ngl3DSLoader::OnCamera (const ngl3DSCamera& rCam)
{
  if (!(mVerbose & NGL_3DSLOADER_LOGITEM)) return true;
  if (mVerbose & NGL_3DSLOADER_LOGCHUNK) NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("[OnCamera]\n"));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("name  : '%s'\n"), rCam.mpName);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("pos   : %s\n"), Print(rCam.mPos));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("target: %s\n"), Print(rCam.mTarget));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("roll  : %.1f\n"), rCam.mRoll);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("lens  : %.1f mm\n"), rCam.mLens);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));
  return true;
}

bool ngl3DSLoader::OnScene (const ngl3DSScene& rScene)
{
  const char* layertype_str[3] = {
    "None",
    "Bottom",
    "Top"
  };

  if (!(mVerbose & NGL_3DSLOADER_LOGITEM)) return true;
  if (mVerbose & NGL_3DSLOADER_LOGCHUNK) NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("[OnScene]\n"));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("version     : 3DSr%d\n"), rScene.mVersion);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("master scale: %.3f\n"), rScene.mMasterScale);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("ambient     : %s\n"), Print(rScene.mAmbient));
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("background  : %s - %s\n"), Print(rScene.mpBgColor[0]), Print(rScene.mpBgColor[1]));

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("fog         :%s\n"), rScene.mFog.mUsed ? "" : " none");
  if (rScene.mFog.mUsed)
  {
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  near plane  : %.3f\n"), rScene.mFog.mNearPlane);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  near density: %.1f%%\n"), rScene.mFog.mNearDensity);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  far plane   : %.3f\n"), rScene.mFog.mFarPlane);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  far density : %.1f%%\n"), rScene.mFog.mFarDensity);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  backgnd     : %s\n"), rScene.mFog.mIsBackground ? "yes" : "no");
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  color       : %s\n"), Print(rScene.mFog.mColor));
  }

  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("layer fog   :%s\n"), rScene.mLayerFog.mUsed ? "" : " none");
  if (rScene.mLayerFog.mUsed)
  {
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  bottom : %.1f%%\n"), rScene.mLayerFog.mBottom);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  top    : %.1f%%\n"), rScene.mLayerFog.mTop);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  density: %.1f%%\n"), rScene.mLayerFog.mDensity);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  falloff: %s\n"), layertype_str[rScene.mLayerFog.mFalloff]);
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  backgnd: %s\n"), rScene.mLayerFog.mIsBackground ? "yes" : "no");
    NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("  color  : %s\n"), Print(rScene.mLayerFog.mColor));
  }
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("\n"));
  return true;
}



/*
 *
 */

void ngl3DSLoader::Log (uint Level, const char* pText, ...)
{
  const char* level_str[8] = {
    "",
    "  ",
    "    ",
    "      ",
    "        ",
    "          ",
    "            ",
    "              "
  };

  va_list args;
  char buffer[4096];

  if (!(mVerbose & NGL_3DSLOADER_LOGCHUNK)) return;
  va_start (args, pText);
  vsprintf(buffer, pText, args);
  va_end (args);
  NGL_LOG(_T("3ds"), NGL_LOG_INFO, _T("%s%s\n"), level_str[Level], buffer);
}

char* ngl3DSLoader::Print (const ngl3DSColor& rColor)
{
  sprintf(mBuffer, "%3d %3d %3d", (uint)(rColor.mR*255.0), (uint)(rColor.mG*255.0), (uint)(rColor.mB*255.0));
  return mBuffer;
}

char* ngl3DSLoader::Print (const ngl3DSVertex& rVertex)
{
  sprintf(mBuffer, "[% 8.3f % 8.3f % 8.3f]", rVertex.mX, rVertex.mY, rVertex.mZ);
  return mBuffer;
}


/*
 * Chunk tools
 */

uint16 ngl3DSLoader::ReadChunkHeader(uint32* pSize)
{
  uint16 id;
  uint32 size;

  if ((mpFile->Read(id) < 1) || (mpFile->Read(size) < 1))
  {
    Log (0, ("Unexpected end of file\n"));
    return 0;
  }
  if (pSize) *pSize = size;
  return size < 6 ? 0 : id;
}

void ngl3DSLoader::ReadUnknown(uint Level, uint16 ID, uint32 DataSize, bool IsLog)
{
  NGL_DEBUG( if (IsLog) Log (Level, ("?? 0x%.4X (size=%d)"), ID, DataSize); )
  mpFile->SetPos (DataSize, eFileForward);
}


/*
 * Basic chunk reading
 */

uint32 ngl3DSLoader::ReadString (char* pString, uint32 DataSize)
{
  char c;
  uint count = 0;

  do
  {
    if (mpFile->Read(pString) < 1) return 0;
    c = *pString;
    pString++;
    count++;
  }
  while (c && ((DataSize == 0) || (count < DataSize)));
  if (c) *pString = 0; /* always terminate */
  if (DataSize > 0) mpFile->SetPos (DataSize - count, eFileForward);
  return count; /* bytes read */
}

bool ngl3DSLoader::ReadColorInt (ngl3DSColor& rColor)
{
  uint8 color[3];

  if (mpFile->Read(color, 3) < 3) return false;
  rColor.mR = (float)(color[0] / 255.0);
  rColor.mG = (float)(color[1] / 255.0);
  rColor.mB = (float)(color[2] / 255.0);
  return true;
}

bool ngl3DSLoader::ReadColorFloat (ngl3DSColor& rColor)
{
  float color[3];

  if (mpFile->Read(color, 3) < 3) return false;
  rColor.mR = color[0];
  rColor.mG = color[1];
  rColor.mB = color[2];
  return true;
}

bool ngl3DSLoader::ReadColors (uint Level, ngl3DSColor* pColor, uint32 DataSize)
{
  uint16 id;
  uint32 size;
  uint i = 0;

  id = ReadChunkHeader(&size);
  switch (id)
  {
    case 0 : return false;
    case TDS_COL_FLOAT:
    case TDS_COL_FLOATG:
      if (!ReadColorFloat (pColor[i])) return false; size = 6+12; break;
    case TDS_COL_INT:
    case TDS_COL_INTG:
      if (!ReadColorInt (pColor[i])) return false; size = 6+3; break;
    default:
      ReadUnknown (Level, id, size-6);
  }
  DataSize -= size;

  if (DataSize > 0) mpFile->SetPos (DataSize, eFileForward);
  return true;
}

bool ngl3DSLoader::ReadValue(uint Level, float& rValue, uint32 DataSize)
{
  uint16 id;
  uint32 size;
  uint16 val;

  id = ReadChunkHeader(&size);
  switch (id)
  {
    case 0 : return false;
    case TDS_PERCENT_INT:
      if (mpFile->Read(val) < 1) return false;
      rValue = (float)(val / 100.0);
      break;
    case TDS_PERCENT_FLOAT:
      if (mpFile->Read(rValue) < 1) return false;
      rValue /= 100.0;
      break;
    default:
      ReadUnknown(Level, id, size-6);
  }
  DataSize -= size;
  return (DataSize == 0);
}

bool ngl3DSLoader::ReadVertex (ngl3DSVertex& rVertex)
{
  float vertex[3];

  if (mpFile->Read(vertex, 3) < 3) return false;
  rVertex.mX =  vertex[0];
  rVertex.mY =  vertex[2];
  rVertex.mZ = -vertex[1];
  return true;
}


/*
 * Data initialisation 
 */

void ngl3DSLoader::ZeroColor (ngl3DSColor& rColor)
{
  rColor.mR = 0.0;
  rColor.mG = 0.0;
  rColor.mB = 0.0;
}

void ngl3DSLoader::IdMatrix (ngl3DSVertex mat[4])
{
  mat[0].mX = 1;  mat[0].mY = 0;  mat[0].mZ = 0;
  mat[1].mX = 0;  mat[1].mY = 1;  mat[1].mZ = 0;
  mat[2].mX = 0;  mat[2].mY = 0;  mat[2].mZ = 1;
  mat[3].mX = 0;  mat[3].mY = 0;  mat[3].mZ = 0;
}

void ngl3DSLoader::ZeroTexture  (ngl3DSTexture& rTex)
{
  rTex.mUsed = false;
  *rTex.mpName = 0;
  rTex.mUScale = 1.0;
  rTex.mVScale = 1.0;
  rTex.mUOffset = 0.0;
  rTex.mVOffset = 0.0;
}


/*
 * Main chunks
 */

bool ngl3DSLoader::ReadHeader()
{
  uint16 id;
  uint32 size;
  nglFileSize DataSize;

  DataSize = mpFile->GetSize();
  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_MAINPRJ : if (!ReadProject(size-6)) return false; break;
      case TDS_MAIN3DS : if (!OnHeader ((id == TDS_MAIN3DS), size) || !ReadMain(size-6)) return false; break;
      default:
        ReadUnknown (0, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}

bool ngl3DSLoader::ReadProject(uint32 DataSize)
{
  uint16 id;
  uint32 size;

  Log (0, "Project");
  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_MDATA: if (!ReadEditor(size-6)) return false; break;
      default:
        ReadUnknown (1, id, size-6, false);
    }
    DataSize -= size;
  }
  return true;
}

bool ngl3DSLoader::ReadMain(uint32 DataSize)
{
  uint16 id;
  uint32 size;
  uint32 version;

  Log (0, "Main");
  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_VERSION  : if (mpFile->Read(version) < 1) return false; NGL_DEBUG( Log (1, ("version: 3DSr%d"), version); ) break;
      case TDS_EDIT     : if (!ReadEditor(size-6)) return false; break;
      case TDS_KEYFRAMER: if (!ReadKeyframer(size-6)) return false; break;
      default:
        ReadUnknown (1, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}

bool ngl3DSLoader::ReadEditor(uint32 DataSize)
{
  uint16 id;
  uint32 size;
  ngl3DSScene scene;

  ZeroColor(scene.mpBgColor[0]);
  ZeroColor(scene.mpBgColor[1]);
  scene.mAmbient.mR = .5f; scene.mAmbient.mG = .5f; scene.mAmbient.mB = .5f;
  scene.mFog.mUsed = false;
  scene.mLayerFog.mUsed = false;

  Log (1, ("Editor"));
  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_EDIT_VERSION : if (mpFile->Read(scene.mVersion) < 1) return false; NGL_DEBUG( Log (2, ("version: 3DSr%d"), scene.mVersion); ) break;
      case TDS_EDIT_MATERIAL: if (!ReadMaterial(size-6)) return false; break;
      case TDS_EDIT_OBJECT  : if (!ReadObject(size-6)) return false; break;
      
      /* globals */
      case TDS_EDIT_MSCALE  : if (mpFile->Read(scene.mMasterScale) < 1) return false; NGL_DEBUG( Log (2, ("master scale: %.3f"), scene.mMasterScale); ) break;
      case TDS_EDIT_AMBIENT : if (!ReadColors (2, &scene.mAmbient, size-6)) return false; NGL_DEBUG( Log (2, ("ambient : %s"), Print(scene.mAmbient)); ) break;
      case TDS_EDIT_BGCOLOR : if (!ReadColors (2, scene.mpBgColor, size-6)) return false; NGL_DEBUG( Log (2, ("background : %s - %s"), Print(scene.mpBgColor[0]), Print(scene.mpBgColor[1])); ) break;
      case TDS_EDIT_FOG     : if (!ReadFog (scene.mFog, size-6)) return false; break;
      case TDS_EDIT_USEFOG  : scene.mFog.mUsed = true; break;
      case TDS_EDIT_LFOG    : if (!ReadLayerFog (scene.mLayerFog, size-6)) return false; break;
      case TDS_EDIT_USELFOG : scene.mLayerFog.mUsed = true; break;
      default:
        ReadUnknown (2, id, size-6);
    }
    DataSize -= size;
  }
  return OnScene (scene);
}


/*
 * Fog
 */

bool ngl3DSLoader::ReadFog (ngl3DSFog& rFog, uint32 DataSize)
{
  uint16 id;
  uint32 size;

  Log (2, "Fog");

  rFog.mIsBackground = false;
  if ((mpFile->Read(rFog.mNearPlane) < 1) ||
      (mpFile->Read(rFog.mNearDensity) < 1) ||
      (mpFile->Read(rFog.mFarPlane) < 1) ||
      (mpFile->Read(rFog.mFarDensity) < 1)) return false;
  DataSize -= sizeof(float) * 4;

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_COL_FLOAT   : if (!ReadColorFloat (rFog.mColor)) return false; break;
      case TDS_EDIT_FOGBGND: rFog.mIsBackground = true; break;
      default:
        ReadUnknown (3, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}

bool ngl3DSLoader::ReadLayerFog (ngl3DSLayerFog& rFog, uint32 DataSize)
{
  uint16 x, y;

  Log (2, ("Layer fog"));

  if ((mpFile->Read(rFog.mBottom) < 1) ||
      (mpFile->Read(rFog.mTop) < 1) ||
      (mpFile->Read(rFog.mDensity) < 1) ||
      (mpFile->Read(x) < 1) ||
      (mpFile->Read(y) < 1)) return false;
  rFog.mFalloff = (ngl3DSLayerType)x;
  rFog.mIsBackground = (y == 1);
  DataSize -= sizeof(float) * 3 + sizeof(uint16) * 2;

  if (!ReadColors (3, &rFog.mColor, DataSize)) return false;
  return true;
}


/*
 * Keyframer
 */

bool ngl3DSLoader::ReadKeyframer(uint32 DataSize)
{
  Log (1, ("Keyframer (not implemented)"));
  mpFile->SetPos (DataSize, eFileForward);
  return true;
}


/*
 * Material
 */

bool ngl3DSLoader::ReadMaterial(uint32 DataSize)
{
  uint16 id;
  uint32 size;
  ngl3DSMaterial mat;
  uint16 shading;

  Log (2, ("Material"));
  mat.mpName[0] = 0;
  ZeroColor(mat.mAmbient);
  ZeroColor(mat.mDiffuse);
  ZeroColor(mat.mSpecular);
  mat.mShiny = 0.0;
  mat.mShinyStrength = 0.0;
  mat.mTransp = 0.0;
  mat.mTranspFalloff = 0.0;
  mat.mShading = e3DSShadingFlat;
  ZeroTexture (mat.mpTexture[0]);
  ZeroTexture (mat.mpTexture[1]);

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_MAT_NAME     : if (!ReadString (mat.mpName, size-6)) return false;            NGL_DEBUG( Log (3, ("name          : '%s'"), mat.mpName); ) break;
      case TDS_MAT_AMBIENT  : if (!ReadColors (4, &mat.mAmbient, size-6)) return false;      NGL_DEBUG( Log (3, ("ambient color : %s"), Print(mat.mAmbient)); ) break;
      case TDS_MAT_DIFFUSE  : if (!ReadColors (4, &mat.mDiffuse, size-6)) return false;      NGL_DEBUG( Log (3, ("diffuse color : %s"), Print(mat.mDiffuse)); ) break;
      case TDS_MAT_SPECULAR : if (!ReadColors (4, &mat.mSpecular, size-6)) return false;     NGL_DEBUG( Log (3, ("specular color: %s"), Print(mat.mSpecular)); ) break;
      case TDS_MAT_SHINY    : if (!ReadValue (4, mat.mShiny, size-6)) return false;          NGL_DEBUG( Log (3, ("shin. factor  : %.1f%%"), mat.mShiny*100.0); ) break;
      case TDS_MAT_SHINY_STR: if (!ReadValue (4, mat.mShinyStrength, size-6)) return false;  NGL_DEBUG( Log (3, ("shin. strength: %.1f%%"), mat.mShinyStrength*100.0); ) break;
      case TDS_MAT_TRANS    : if (!ReadValue (4, mat.mTransp, size-6)) return false;         NGL_DEBUG( Log (3, ("transparency  : %.1f%%"), mat.mTransp*100.0); ) break;
      case TDS_MAT_TRANS_OFF: if (!ReadValue (4, mat.mTranspFalloff, size-6)) return false;  NGL_DEBUG( Log (3, ("trans. falloff: %.1f%%"), mat.mTranspFalloff*100.0); ) break;
      case TDS_MAT_SHADING  : if (mpFile->Read(shading) < 1) return false; mat.mShading = (ngl3DSShading)shading;  NGL_DEBUG( Log (3, ("shading       : %s"), pg3DSShadingStr[mat.mShading]); ) break;
      case TDS_MAT_TEXTURE1 : if (!ReadTexture (mat.mpTexture[0], size-6)) return false; break;
      case TDS_MAT_TEXTURE2 : if (!ReadTexture (mat.mpTexture[1], size-6)) return false; break;
      default:
        ReadUnknown (3, id, size-6);
    }
    DataSize -= size;
  }

  mMaterialCnt++;
  OnMaterial (mat);
  return true;
}

bool ngl3DSLoader::ReadTexture (ngl3DSTexture& rTex, uint32 DataSize)
{
  uint16 id;
  uint32 size;
  uint32 bytes_read;

  Log (3, "Texture");
  rTex.mUsed = true;
  if (!(bytes_read = ReadString (rTex.mpName, 0))) return false;
  DataSize -= bytes_read;
  if (DataSize == 0)
  {
    NGL_DEBUG( Log (4, ("name: '%s'"), rTex.mpName); )
    return true;
  }

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_STRANGE00   : if (mpFile->IsEOF()) return false; size = 6; break;
      case TDS_MAT_TEXNAME : if (!ReadString (rTex.mpName, size-6)) return false; NGL_DEBUG( Log (4, ("name    : '%s'"), rTex.mpName); ) break;
      case TDS_MAT_USCALE  : if (mpFile->Read(rTex.mUScale) < 1) return false;    NGL_DEBUG( Log (4, ("U scale : %.3f"), rTex.mUScale); ) break;
      case TDS_MAT_VSCALE  : if (mpFile->Read(rTex.mVScale) < 1) return false;    NGL_DEBUG( Log (4, ("V scale : %.3f"), rTex.mVScale); ) break;
      case TDS_MAT_UOFFSET : if (mpFile->Read(rTex.mUOffset) < 1) return false;   NGL_DEBUG( Log (4, ("U offset: %.3f"), rTex.mUOffset); ) break;
      case TDS_MAT_VOFFSET : if (mpFile->Read(rTex.mVOffset) < 1) return false;   NGL_DEBUG( Log (4, ("V offset: %.3f"), rTex.mVOffset); ) break;
      default:
        ReadUnknown (4, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}

bool ngl3DSLoader::ReadObject(uint32 DataSize)
{
  uint16 id;
  uint32 size;
  int bytes_read;
  char name[TDS_SHORT_STR+1];

  Log (2, ("Object"));
  if (!(bytes_read = ReadString (name, 0))) return false;
  NGL_DEBUG( Log (3, ("name: '%s'"), name); )
  DataSize -= bytes_read;

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_OBJ_MESH  : if (!ReadMesh (name, size-6)) return false; break;
      case TDS_OBJ_LIGHT : if (!ReadLight (name, size-6)) return false; break;
      case TDS_OBJ_CAMERA: if (!ReadCamera (name, size-6)) return false; break;
      default:
        ReadUnknown (3, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}


/*
 * Mesh
 */

bool ngl3DSLoader::ReadMesh(char* name, uint32 DataSize)
{
  uint16 id;
  uint32 size;
  ngl3DSMesh mesh;
  bool failed = false;
  bool user;

  strcpy (mesh.mpName, name);
  mesh.mVertexCnt = 0;
  mesh.mpVertex = NULL;
  mesh.mpVertexUV = NULL;
  mesh.mFaceCnt = 0;
  mesh.mpFace = NULL;
  mesh.mpSmoothing = NULL;
  mesh.mMaterialCnt = 0;
  mesh.mpMaterial = NULL;
  IdMatrix (mesh.mpLocalRef);

  Log (3, "Mesh");
  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_MESH_VERTICES: if (!ReadVertices (mesh, size-6)) failed = true; break;
      case TDS_MESH_MAPPING : if (!ReadMapping (mesh, size-6)) failed = true; break;
      case TDS_MESH_FACEL1  : if (!ReadFaces (mesh, size-6)) failed = true; break;
/*      case TDS_MESH_FACEL2  : if (!ReadVertexGroup (mesh, size-6)) failed = true; break;*/
      case TDS_MESH_LOCAL   : if (mpFile->Read((float*)&mesh.mpLocalRef, 12) < 12) return false; break;
/*      case TDS_MESH_COLOR: ReadInt8 (&void); if (IsEOF()) return false; NGL_DEBUG( Log (4, "color: %d", void); ) break;*/
      default:
        ReadUnknown (4, id, size-6);
    }
    if (failed)
    {
      FreeMesh (mesh);
      return false;
    }
    DataSize -= size;
  }

  mVertexCnt += mesh.mVertexCnt;
  mFaceCnt   += mesh.mFaceCnt;
  user = OnMesh (mesh);
  FreeMesh (mesh);

  return user;
}

void ngl3DSLoader::FreeMesh (ngl3DSMesh& rMesh)
{
  if (rMesh.mpVertex)    free (rMesh.mpVertex);
  if (rMesh.mpVertexUV)  free (rMesh.mpVertexUV);
  if (rMesh.mpFace)      free (rMesh.mpFace);
  if (rMesh.mpSmoothing) free (rMesh.mpSmoothing);
  if (rMesh.mpMaterial)
  {
    uint i;

    for (i = 0; i < rMesh.mMaterialCnt; i++)
      if (rMesh.mpMaterial[i].mpFace) free (rMesh.mpMaterial[i].mpFace);
    free (rMesh.mpMaterial);
  }
}

bool ngl3DSLoader::ReadVertices (ngl3DSMesh& rMesh, uint32 DataSize)
{
  uint16 count;
  ngl3DSVertex* vertices = NULL;
  uint i;

  if (mpFile->Read(count) < 1) return false;
  Log (4, ("%d vertices"), count);
  
  vertices = (ngl3DSVertex*)malloc (sizeof(ngl3DSVertex) * count);
  for (i = 0; i < count; i++)
    if (!ReadVertex (vertices[i]))
    {
      free (vertices);
      return false;
    }
  rMesh.mVertexCnt = count;
  rMesh.mpVertex = vertices;
  return true;
}

/*
bool ngl3DSLoader::ReadVertexGroup (ngl3DSMesh& rMesh, uint32 DataSize)
{
  uint16 count;
  uint16 point;
  uint i;

  mpFile->ReadInt16(&count);
  if (IsEOF()) return false;
  Log (4, "%d point flags", count);
  
  for (i = 0; i < count; i++)
  {
    mpFile->ReadInt16 (&point);
    if (IsEOF()) return false;
    nglDebug("%.4X ", point);
    if (!((i+1) % 8)) nglDebug("\n");
  }
  nglDebug("\n");
  return true;
}
*/

bool ngl3DSLoader::ReadMapping(ngl3DSMesh& rMesh, uint32 DataSize)
{
  uint16 count;
  ngl3DSMapping* uv = NULL;

  if (mpFile->Read(count) < 1) return false;
  Log (4, ("%d mapping coordinates"), count);
  if (count != rMesh.mVertexCnt)
    Log (5, ("!! Does not match mpVertex count (%d) !!"), rMesh.mVertexCnt);
  
  uv = (ngl3DSMapping*) malloc (sizeof(ngl3DSMapping) * count);
  if (mpFile->Read((float*)&uv, count*2) < (count*2))
  {
    delete uv;
    return false;
  }
  rMesh.mpVertexUV = (ngl3DSMapping*)uv;
  return true;
}

bool ngl3DSLoader::ReadFaces(ngl3DSMesh& rMesh, uint32 DataSize)
{
  uint16 count;
  ngl3DSFace* faces;
  int i;

  if (mpFile->Read(count) < 1) return false;
  Log (4, ("%d faces"), count);
  
  faces = (ngl3DSFace*) malloc (sizeof(ngl3DSFace)*count);
  for (i=0; i < count; i++)
  {
    uint16 flags;

    if ((mpFile->Read(faces[i].mpVertex, 3) < 3) ||
        (mpFile->Read(flags) < 1))
    {
      free (faces);
      return false;
    }
  }
  rMesh.mFaceCnt = count;
  rMesh.mpFace = faces;
  DataSize -= 2 + 4 * 2 * count;

  while (DataSize > 0)
  {
    uint16 id;
    uint32 size;

    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_MESH_MATERIAL: if (!ReadFacesMaterial (rMesh, size-6)) return false; break;
      case TDS_MESH_SMOOTH: if (!ReadSmoothing (rMesh, size-6)) return false; break;
      default:
        ReadUnknown (5, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}

bool ngl3DSLoader::ReadFacesMaterial(ngl3DSMesh& rMesh, uint32 DataSize)
{
  char name[TDS_LONG_STR+1];
  uint16 count;

  if ((!ReadString (name, 0)) ||
      (mpFile->Read(&count) < 1)) return false;
  Log (5, ("%d faces assigned to material '%s'"), count, name);

  rMesh.mMaterialCnt++;
  rMesh.mpMaterial = (ngl3DSFaceMaterial*) realloc (rMesh.mpMaterial, sizeof(ngl3DSFaceMaterial) * rMesh.mMaterialCnt);
  ngl3DSFaceMaterial& fmat = rMesh.mpMaterial[rMesh.mMaterialCnt-1];

  strcpy (fmat.mpMaterialName, name);
  fmat.mFaceCnt = count;
  fmat.mpFace = (uint16*) malloc (sizeof(uint16) * count);
  return (mpFile->Read(fmat.mpFace, count) == count);
}

bool ngl3DSLoader::ReadSmoothing (ngl3DSMesh& rMesh, uint32 DataSize)
{
  uint count;
  uint32* smoothing;
  Log (5, ("%d faces in smoothing groups"), DataSize/4);

  count = DataSize/4;
  if (count != rMesh.mFaceCnt)
    Log (6, ("!! Does not match face count (%d) !!"), rMesh.mFaceCnt);

  smoothing = (uint32*) malloc (sizeof(uint32) * count);
  if ((uint)mpFile->Read(smoothing, count) < count)
  {
    free (smoothing);
    return false;
  }
  rMesh.mpSmoothing = smoothing;
  return true;
}


/*
 * Light
 */

bool ngl3DSLoader::ReadLight(char* name, uint32 DataSize)
{
  uint16 id;
  uint32 size;
  ngl3DSLight light;

  Log (3, ("Light"));
  strcpy(light.mpName, name);
  light.mType = e3DSLightOmni;
  if (!ReadVertex (light.mPos)) return false;
  Log (4, ("pos        : %s"), Print(light.mPos));
  DataSize -= 3*4;

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      case TDS_COL_FLOAT   : if (!ReadColorFloat (light.mColor)) return false;    NGL_DEBUG( Log(4, ("color      : %s"), Print(light.mColor)); ) break;
      case TDS_LIGHT_RSTART: if (mpFile->Read(light.mRangeStart) < 1) return false; NGL_DEBUG( Log(4, ("range start: %.3f"), light.mRangeStart); ) break;
      case TDS_LIGHT_REND  : if (mpFile->Read(light.mRangeEnd) < 1) return false;   NGL_DEBUG( Log(4, ("range end  : %.3f"), light.mRangeEnd); ) break;
      case TDS_LIGHT_MULT  : if (mpFile->Read(light.mMultiplier) < 1) return false; NGL_DEBUG( Log(4, ("multiplier : %.3f"), light.mMultiplier); ) break;
      case TDS_LIGHT_SPOT  : if (!ReadSpot(light, size-6)) return false; break;
      default:
        ReadUnknown (4, id, size-6);
    }
    DataSize -= size;
  }

  mLightCnt++;
  return OnLight (light);
}

bool ngl3DSLoader::ReadSpot(ngl3DSLight& rLight, uint32 DataSize)
{
  uint16 id;
  uint32 size;

  Log (4, "Spot");
  rLight.mType = e3DSLightSpot;

  if ((!ReadVertex (rLight.mTarget)) ||
      (mpFile->Read(rLight.mHotspot) < 1) ||
      (mpFile->Read(rLight.mFalloff) < 1)) return false;
  NGL_DEBUG( Log (5, ("target : %s"), Print(rLight.mTarget)); )
  NGL_DEBUG( Log (5, ("hotspot: %.1f"), rLight.mHotspot); )
  NGL_DEBUG( Log (5, ("falloff: %.1f"), rLight.mFalloff); )
  DataSize -= sizeof(float) * 5;

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
//      case TDS_SPOT_ROLL : if (!mpFile->ReadFloat(&rLight.mRoll)) return false;  NGL_DEBUG( Log(5, "roll   : %.1f", rLight.mRoll); ) break;
      default:
        ReadUnknown (5, id, size-6);
    }
    DataSize -= size;
  }
  return true;
}


/*
 * Camera
 */

bool ngl3DSLoader::ReadCamera(char* name, uint32 DataSize)
{
  uint16 id;
  uint32 size;
  ngl3DSCamera cam;

  Log (3, ("Camera"));
  strcpy (cam.mpName, name);
  if ((!ReadVertex (cam.mPos)) ||
      (!ReadVertex (cam.mTarget)) ||
      (mpFile->Read(cam.mRoll) < 1) ||
      (mpFile->Read(cam.mLens) < 1)) return false;
  NGL_DEBUG( Log (4, ("pos   : %s"), Print(cam.mPos)); )
  NGL_DEBUG( Log (4, ("target: %s"), Print(cam.mTarget)); )
  NGL_DEBUG( Log (4, ("roll  : %.1f"), cam.mRoll); )
  NGL_DEBUG( Log (4, ("lens  : %.1f mm"), cam.mLens); )
  DataSize -= sizeof(float) * (2 * 3 + 2);

  while (DataSize > 0)
  {
    id = ReadChunkHeader(&size);
    switch (id)
    {
      case 0 : return false;
      default:
        ReadUnknown (4, id, size-6);
    }
    DataSize -= size;
  }

  mCameraCnt++;
  return OnCamera (cam);
}
