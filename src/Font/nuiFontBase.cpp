/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"

#include "nuiFontBase.h"

#include "nglKernel.h"
#include "nglMath.h"
#include "nuiTexture.h"

#include "nuiTextLayout.h"
#include "nglImage.h"

#include "nuiDrawContext.h"
#include "nglBitmapTools.h"

#include "nuiFontManager.h"

#include "nglKernel.h"
#include "nglVideoMode.h"
#include "nuiFontBase.h"
#include "nuiFontInstance.h"
#include "nglMath.h"
#include "ngl_default_font.h"

#include "nuiUnicode.h"
#include "hb.h"
#include "hb-ft.h"
#include "hb_nui.h"

using namespace std;

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_TRUETYPE_TABLES_H

/* Globals
 */
FT_Library     gFTLibrary      = NULL;  // Global FT library instance
FTC_Manager    gFTCacheManager = NULL;  // Global FT cache manager
FTC_CMapCache  gFTCMapCache    = NULL;  // Charmap lookup cache
FTC_ImageCache gFTImageCache   = NULL;  // Generic client-side glyph cache
FTC_SBitCache  gFTSBitmapCache = NULL;

/* Pick a default pixel size which is very legible for the default (embedded) font
 */
float nuiFontBase::DefaultPixelSize = 14.f;

const nuiFontBase::RenderMode nuiFontBase::AntiAliasing = (1 << 0);
const nuiFontBase::RenderMode nuiFontBase::Hinting      = (1 << 1);


/* FreeType cache settings
 *
 * Note on 'MAX_FACES': nuiFontBase keep a reference to a face object (mpFace->Face), which
 * can be invalid if more than MAX_FACES faces are loaded through the FreeType cache
 * manager (the LRU algorithm will delete the least used face object and create a
 * a dangling FT_Face somewhere). This number should be considered to be set as
 * 'infinity'. Note that the FT_Face storage payload is not accounted for against
 * 'MAX_BYTES', this is only relevant for charmaps and glyphs objects.
 */
//#define FTCACHE_MAX_FACES  65535
//#define FTCACHE_MAX_SIZES  64
//#define FTCACHE_MAX_BYTES  (1024*1024)
//

#define FTCACHE_MAX_FACES  10
#define FTCACHE_MAX_SIZES  10
#define FTCACHE_MAX_BYTES  (10*1024)



/* Opaque structure which encapsulate FreeType's specific types
 */
class FaceHandle
{
public:
  FT_Face          Face;  // FreeType face object handler
  FTC_ImageTypeRec Desc;  // Font description, for image cache (pixel size, rendering mode)
  FTC_ScalerRec ftscaler;  

  FaceHandle()
  : Face(NULL),
    mpFontInstance(new nuiFontInstance(nglPath(), 0))
  {
    Desc.face_id = 0;
    Desc.width = 0;
    Desc.height = 0;
    Desc.flags = 0;
  }
  
  ~FaceHandle()
  {
    if (mpFontInstance)
    {
      nuiFontInstance::Uninstall(mpFontInstance);
      mpFontInstance->Release();
    }
  }
  
  void SetFontInstance(nuiFontInstance* pFontInstance)
  {
    pFontInstance->Acquire();
    mpFontInstance->Release();
    mpFontInstance = pFontInstance;
  }
  
  nuiFontInstance* GetFontInstance() const
  {
    return mpFontInstance;
  }
  

  bool GetSize(FT_Size& ftsize)
  {
    return FTC_Manager_LookupSize(gFTCacheManager, &ftscaler, &ftsize) != 0;
  }
  
  FT_Face GetFace()
  {
    FT_Size ftsize;
    bool res = GetSize(ftsize);
    if (res)
    {
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, "Couldn't change font size"); )
      return NULL;
    }
    
    Face = ftsize->face;
    Desc.face_id = ftscaler.face_id;

    return ftsize->face;
  }
  
private:
  nuiFontInstance* mpFontInstance;
};


/* Opaque structure which is only used as a pointer (via GlyphHandle)
 */
class GlyphData
{
public:
};

const nglChar* nuiGetCharMapName (FT_CharMap CharMap)
{
  switch (CharMap->encoding)
  {
    case FT_ENCODING_NONE           : return _T("None");
    case FT_ENCODING_UNICODE        : return _T("Unicode");
    case FT_ENCODING_MS_SYMBOL      : return _T("Symbol");
    case FT_ENCODING_MS_SJIS        : return _T("SJIS (Japanese)");
    case FT_ENCODING_MS_GB2312      : return _T("GB2312 (Simplified Chinese)");
    case FT_ENCODING_MS_BIG5        : return _T("Big5 (Traditional Chinese)");
    case FT_ENCODING_MS_WANSUNG     : return _T("Wansung (Korean)");
    case FT_ENCODING_MS_JOHAB       : return _T("Johab (Korean, complete Hangeul)");
    case FT_ENCODING_ADOBE_LATIN_1  : return _T("Latin 1");
    case FT_ENCODING_ADOBE_STANDARD : return _T("Adobe Standard");
    case FT_ENCODING_ADOBE_EXPERT   : return _T("Adobe Expert");
    case FT_ENCODING_ADOBE_CUSTOM   : return _T("Adobe Custom");
    case FT_ENCODING_APPLE_ROMAN    : return _T("Apple Roman");
    default:
      return _T("Unknown");
  }
}

nglTextEncoding nuiGetCharMapEncoding (FT_CharMap CharMap)
{
  switch (CharMap->encoding)
  {
    case FT_ENCODING_NONE           : return eEncodingUnknown;
    case FT_ENCODING_UNICODE        : return eUCS4;
    case FT_ENCODING_MS_SYMBOL      : return eEncodingUnknown;
    case FT_ENCODING_MS_SJIS        : return eSJIS;
    case FT_ENCODING_MS_GB2312      : return eGB2312;
    case FT_ENCODING_MS_BIG5        : return eBig5;
    case FT_ENCODING_MS_WANSUNG     : return eEncodingUnknown;
    case FT_ENCODING_MS_JOHAB       : return eEncodingUnknown;
    case FT_ENCODING_ADOBE_LATIN_1  : return eISO8859_1;
    case FT_ENCODING_ADOBE_STANDARD : return eEncodingUnknown;
    case FT_ENCODING_ADOBE_EXPERT   : return eEncodingUnknown;
    case FT_ENCODING_ADOBE_CUSTOM   : return eEncodingUnknown;
    case FT_ENCODING_APPLE_ROMAN    : return eAppleRoman;
    default:
      return eEncodingUnknown;
  }
}



using namespace std;

static const nglChar* gpFontErrorTable[] =
{
  /*  0 */ _T("No error"),
  /*  1 */ _T("Something bad happened"),
  NULL
};


///! nuiFontDesc
//class nuiFontDesc
nuiFontDesc::nuiFontDesc(const nglPath& rPath, int32 Face)
{
  //  NGL_OUT(_T("Scanning font '%s' face %d\n"), rPath.GetChars(), Face);
  FT_Byte* pBuffer = NULL;
  
  mValid = false;
  
  mPath = rPath;
  mFace = Face;
  
  
  FT_Error error = 0;
  FT_Face pFace = NULL;
  
  error = FT_New_Face(gFTLibrary, rPath.GetChars(), Face, &pFace);

  if (error)
  {
    nglIStream* pStream = rPath.OpenRead();
    
    if (!pStream)
    {
      NGL_LOG("font", NGL_LOG_ERROR, "Error Scanning font '%s' face %d\n", rPath.GetChars(), Face);
      return;
    }
    
    uint32 size = pStream->Available();
    pBuffer = new FT_Byte[size];
    pStream->ReadUInt8(pBuffer, size);
    delete pStream;
    
    error = FT_New_Memory_Face(gFTLibrary, pBuffer, size, Face, &pFace);
  }
  
  if (error || pFace->num_faces <= Face)
  {
    if (!Face)
    {
      NGL_LOG("font", NGL_LOG_ERROR, "ERROR Scanning font '%s' face %d\n", rPath.GetChars(), Face);
    }

    delete pBuffer;
    return;
  }
  
  NGL_LOG("font", NGL_LOG_INFO, "Scanning font '%s' face %d name '%s' style '%s'\n", rPath.GetChars(), Face, pFace->family_name, pFace->style_name);
  
  NGL_ASSERT(pFace->num_faces > Face);
  
  mValid = true;
  
  mName.Import (pFace->family_name);
  mStyle.Import (pFace->style_name);
  
  mBold = ((pFace->style_flags & FT_STYLE_FLAG_BOLD) != 0);
  mItalic = ((pFace->style_flags & FT_STYLE_FLAG_ITALIC) != 0);
  mMonospace = ((pFace->face_flags & FT_FACE_FLAG_FIXED_WIDTH) != 0);
  mScalable = FT_IS_SCALABLE(pFace);
  
  // Get Panose information from the TT OS/2 tables
  TT_OS2* pOS2 = (TT_OS2*)FT_Get_Sfnt_Table(pFace, ft_sfnt_os2);
  if (pOS2)
  {
    memcpy(&mPanoseBytes, pOS2->panose, 10);
    nglString str;
    uint8* pBytes = (uint8*)&mPanoseBytes;
    for (uint i = 0; i < 10; i++)
      str.Add(pBytes[i]).Add(_T(" "));
    //NGL_DEBUG( NGL_OUT(_T("\tpanose bytes: %s\n"), str.GetChars()); )
  }
  else
  {
    //NGL_DEBUG( NGL_OUT(_T("Warning: font '%s' has no panose information.\n"), mPath.GetChars()); )
    memset(&mPanoseBytes, 0, 10);
  }
  
  // Get encodings:
  for (int32 i = 0; i < pFace->num_charmaps; i++)
  {
    FT_CharMap charmap = pFace->charmaps[i];
    mEncodings.insert(nuiGetCharMapEncoding(charmap));
  }
  
  // Get sizes
  for (int32 i = 0; i < pFace->num_fixed_sizes; i++)
  {
    FT_Bitmap_Size& size = pFace->available_sizes[i];
    
    mSizes.insert(size.height);
  }
  
  // Get glyphs:
  FT_ULong  charcode = 0;
  int32  prevcharcode = -1;
  int32  rangestart = -1;
  uint32 rangecount = 0;
  uint32 glyphcount = 0;
  FT_UInt   gindex = 0;
  
  FT_Select_Charmap(pFace, FT_ENCODING_UNICODE);// == FT_Err_Ok);

  
  charcode = FT_Get_First_Char(pFace, &gindex);

//  std::vector<nglUChar> tmp;
//  tmp.reserve(10000);
  
  while ( gindex != 0 )
  {
    glyphcount++;
    if (rangestart >= 0 && (prevcharcode != charcode - 1))
    {
      //NGL_OUT(_T("\nrange: %d to %d (%d glyphs)\n"), rangestart, prevcharcode, prevcharcode - rangestart + 1);
      rangecount++;
      
      mGlyphs.push_back(std::make_pair(rangestart, prevcharcode));
      rangestart = charcode;
    }
    prevcharcode = charcode;

    if (rangestart < 0)
      rangestart = charcode;
    
    charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
  }

  if (prevcharcode > 0)
  {
    //NGL_OUT(_T("last range: %d to %d (%d glyphs)\n"), rangestart, prevcharcode, prevcharcode - rangestart + 1);
    rangecount++;
    mGlyphs.push_back(std::make_pair(rangestart, prevcharcode));
  }

  NGL_DEBUG( NGL_LOG("font", NGL_LOG_INFO, "\t%d glyph ranges (%d glyphs)\n", rangecount, glyphcount); )
  
  FT_Done_Face(pFace);
  delete pBuffer;
}

nuiFontDesc::nuiFontDesc(nglIStream& rStream)
{
  mValid = Load(rStream);
}

nuiFontDesc::~nuiFontDesc()
{
}

bool nuiFontDesc::IsValid() const
{
  return mValid;
}

const nglPath& nuiFontDesc::GetPath() const
{
  return mPath;
}

bool nuiFontDesc::CheckPath()
{
  if (mPath.Exists())
    return true;
  nglString p(mPath.GetPathName());
  nglString pp(p);
  if (p.IsEmpty())
    return false;
  
  // try to find a .ttc instead of a .ttf...
  p[p.GetLength() - 1] = 'c';
  mPath = p;
  if (mPath.Exists())
  {
    NGL_LOG("font", NGL_LOG_INFO, "found '%s' instead of '%s'\n", p.GetChars(), pp.GetChars());
    return true;
  }
  
#ifdef NUI_IOS
  // try to find a filename prefixed with '_H_' (iOS)
  {
    nglPath base(pp);
    nglString node("_H_");
    node += base.GetNodeName();
    base = base.GetParent();
    mPath = base + node;
    
    if (mPath.Exists())
    {
      NGL_LOG("font", NGL_LOG_INFO, "found '%s' instead of '%s'\n", p.GetChars(), pp.GetChars());
      return true;
    }
    
    p = mPath.GetPathName();
    
    // try to find a .ttc instead of a .ttf... (again)
    p[p.GetLength() - 1] = 'c';
    mPath = p;
    if (mPath.Exists())
    {
      NGL_LOG("font", NGL_LOG_INFO, "found '%s' instead of '%s'\n", p.GetChars(), pp.GetChars());
      return true;
    }
  }
#endif
  return mPath.Exists();
}

const nglString& nuiFontDesc::GetName() const
{
  return mName;
}

const nglString& nuiFontDesc::GetStyle() const
{
  return mStyle;
}

int32 nuiFontDesc::GetFace() const
{
  return mFace;
}

bool nuiFontDesc::GetBold() const
{
  return mBold;
}

bool nuiFontDesc::GetItalic() const
{
  return mItalic;
}

bool nuiFontDesc::GetMonospace() const
{
  return mMonospace;
}

bool nuiFontDesc::GetScalable() const
{
  return mScalable;
}

bool nuiFontDesc::HasEncoding(nglTextEncoding Encoding) const
{
  std::set<nglTextEncoding>::const_iterator it = mEncodings.find(Encoding);
  return (it != mEncodings.end());
}

bool nuiFontDesc::HasGlyph(nglUChar Glyph) const
{
#if 0
  // Dichotomic lookup of the charcode:
  
  int32 len = mGlyphs.size();
  int32 start = 0;
  int32 end = len - 1;
  int32 middle = (start + end) >> 1;
  
  while (len > 0)
  {
    const GlyphRange& middleglyph = mGlyphs[middle];
    const GlyphRange& firstglyph = mGlyphs[start];
    const GlyphRange& lastglyph = mGlyphs[end];
    
    if (Glyph < firstglyph.first || lastglyph.second < Glyph)
      return false;
    
    if (middleglyph.first <= Glyph && Glyph <= middleglyph.second)
      return true;
    
    if (middleglyph.first < Glyph) 
    {
      // Return right hand part
      start = middle + 1;
    }
    else
    {
      // Return Left hand part
      end = middle - 1;
    }
    
    len = end - start;
    middle = (start + end) >> 1;
  }
  return false;
#else
  
  for (uint32 i = 0; i < mGlyphs.size(); i++)
  {
    if (mGlyphs[i].first <= Glyph && Glyph <= mGlyphs[i].second)
      return true;
  }
  
  return false;
#endif
}

bool nuiFontDesc::HasSize(int32 Size) const
{
  std::set<int32>::const_iterator it = mSizes.find(Size);
  return (it != mSizes.end());
}

const std::set<nglTextEncoding>& nuiFontDesc::GetEncodings() const
{
  return mEncodings;
}

const std::set<int32>& nuiFontDesc::GetSizes() const
{
  return mSizes;
}

const nuiFontPanoseBytes& nuiFontDesc::GetPanoseBytes() const
{
  return mPanoseBytes;
}

bool nuiFontDesc::Save(nglOStream& rStream)
{
  uint32 s = 0;
  const char* pPath = mPath.GetPathName().Export(eUTF8);
  const char* pName = mName.Export(eUTF8);
  const char* pStyle = mStyle.Export(eUTF8);
  
  // Write the path of the font:
  s = strlen(pPath);
  rStream.WriteUInt32(&s);
  rStream.Write(pPath, s, 1);
  
  // Write the name of the font face:
  s = strlen(pName);
  rStream.WriteUInt32(&s);
  rStream.Write(pName, s, 1);
  
  // Write the style of the font face:
  s = strlen(pStyle);
  rStream.WriteUInt32(&s);
  rStream.Write(pStyle, s, 1);
  
  
  rStream.WriteInt32(&mFace);
  
  uint32 boolean = 0;
  
  boolean = mBold ? 1 : 0;
  rStream.WriteUInt32(&boolean);
  boolean = mItalic ? 1 : 0;
  rStream.WriteUInt32(&boolean);
  boolean = mMonospace ? 1 : 0;
  rStream.WriteUInt32(&boolean);
  boolean = mScalable ? 1 : 0;
  rStream.WriteUInt32(&boolean);
  
  s = mEncodings.size();
  rStream.WriteUInt32(&s);
  if (s)
  {
    std::vector<uint32> Encodings(mEncodings.begin(), mEncodings.end());
    rStream.WriteUInt32(&Encodings[0], s);
  }
  
  s = mGlyphs.size();
  rStream.WriteUInt32(&s);
  if (s)
    rStream.Write(&mGlyphs[0], s, sizeof(GlyphRange));
  
  s = mSizes.size();
  rStream.WriteUInt32(&s);
  if (s)
  {
    std::vector<uint32> Sizes(mSizes.begin(), mSizes.end());  
    rStream.WriteUInt32(&Sizes[0], s);
  }
  
  // Write the panose bytes for this font:
  rStream.Write(&mPanoseBytes, 10, 1);
  
  delete[] pPath;
  delete[] pName;
  delete[] pStyle;
  
  
  
  return true;
}

bool nuiFontDesc::Load(nglIStream& rStream)
{
  uint32 s = 0;
  std::vector<char> chars;
  
  // Write the path of the font:
  rStream.ReadUInt32(&s);
  chars.resize(s);
  rStream.Read(&chars[0], s, 1);
  mPath = nglString(&chars[0], s, eUTF8);
  
  // Read the name of the font face:
  rStream.ReadUInt32(&s);
  chars.resize(s);
  rStream.Read(&chars[0], s, 1);
  mName = nglString(&chars[0], s, eUTF8);
  
  // Read the style of the font face:
  rStream.ReadUInt32(&s);
  chars.resize(s);
  rStream.Read(&chars[0], s, 1);
  mStyle = nglString(&chars[0], s, eUTF8);
  
  
  rStream.ReadInt32(&mFace);
  
  uint32 boolean = 0;
  
  rStream.ReadUInt32(&boolean);
  mBold = boolean != 0;
  rStream.ReadUInt32(&boolean);
  mItalic = boolean != 0;
  rStream.ReadUInt32(&boolean);
  mMonospace = boolean != 0;
  rStream.ReadUInt32(&boolean);
  mScalable = boolean != 0;
  
  rStream.ReadUInt32(&s);
  std::vector<uint32> Encodings;
  Encodings.resize(s);
  if (s)
  {
    rStream.ReadUInt32(&Encodings[0], s);
    mEncodings.clear();
    for (uint i = 0; i < Encodings.size(); i++)
      mEncodings.insert((nglTextEncoding)Encodings[i]);
  }
  
  rStream.ReadUInt32(&s);
  mGlyphs.resize(s);
  if (s)
    rStream.Read(&mGlyphs[0], s, sizeof(GlyphRange));
  
  rStream.ReadUInt32(&s);
  std::vector<uint32> Sizes;
  Sizes.resize(s);
  if (s)
  {
    rStream.ReadUInt32(&Sizes[0], s);
    mSizes.clear();
    mSizes.insert(Sizes.begin(), Sizes.end());
  }
  
  // Read the panose bytes for this font:
  rStream.Read(&mPanoseBytes, 10, 1);
  
  NGL_LOG("font", NGL_LOG_INFO, "Load FontDesc: '%s' / '%s' (%s)\n", mName.GetChars(), mStyle.GetChars(), mPath.GetChars());
  
  return true;
}

/*
 * nuiFontInfo
 */

const nglChar* nuiFontInfo_UnitName(nuiFontUnit Unit)
{
  switch (Unit)
  {
    case eFontUnitEM   : return _T("font unit (EM)");
    case eFontUnitPoint: return _T("point");
    case eFontUnitPixel: return _T("pixel");
    default:
      return _T("?");
  }
}

nuiFontInfo::nuiFontInfo()
{
  pFont = NULL;
}

nuiFontInfo::~nuiFontInfo()
{
}

void nuiFontInfo::Dump (uint Level) const
{
  if (!pFont)
  {
    NGL_LOG(_T("font"), Level, _T("Invalid font info"));
    return;
  }
  
  NGL_LOG(_T("font"), Level, _T("Font info"));
  nuiFontInstance* ifont = nuiFontInstance::Lookup(pFont->mpFace->Desc.face_id);
  if (ifont)
  {
    NGL_LOG(_T("font"), Level, _T("  source   : %s"), ifont->GetPath().GetChars());
    NGL_LOG(_T("font"), Level, _T("  face     : %d"), ifont->GetFace());
  }
  NGL_LOG(_T("font"), Level, _T("  family   : %s"), FamilyName.GetChars());
  NGL_LOG(_T("font"), Level, _T("  style    : %s"), StyleName.GetChars());
  NGL_LOG(_T("font"), Level, _T("  scalable : %s"), YESNO(IsScalable));
  NGL_LOG(_T("font"), Level, _T("  bold     : %s"), YESNO(IsBold));
  NGL_LOG(_T("font"), Level, _T("  monospace: %s"), YESNO(IsMonospace));
  NGL_LOG(_T("font"), Level, _T("  italic   : %s"), YESNO(IsItalic));
  NGL_LOG(_T("font"), Level, _T("  glyph cnt: %d"), GlyphCount);
  
  int i = 0;
  const nglChar* map_name;
  nglString avail_map;
  while ((map_name = pFont->GetCharMapName(i)))
  {
    if (i > 0)
      avail_map += _T(',');
    avail_map += _T(' ');
    avail_map += map_name;
    i++;
  }
  if (i == 0)
    avail_map += _T(" (none)");
  NGL_LOG(_T("font"), Level, _T("  charmaps :%s"), avail_map.GetChars());
  
  NGL_LOG(_T("font"), Level, _T("  Global metrics, in `%s' units"), nuiFontInfo_UnitName(Unit));
  if (IsScalable)
  {
    NGL_LOG(_T("font"), Level, _T("    bbox     : [%.1f %.1f]x[%.1f %.1f]"), BBoxMinX, BBoxMaxX, BBoxMinY, BBoxMaxY);
    NGL_LOG(_T("font"), Level, _T("    ascender : %.1f"), Ascender);
    NGL_LOG(_T("font"), Level, _T("    descender: %.1f"), Descender);
    NGL_LOG(_T("font"), Level, _T("    height   : %.1f"), Height);
    NGL_LOG(_T("font"), Level, _T("    adv max W: %.1f"), AdvanceMaxW);
    NGL_LOG(_T("font"), Level, _T("    adv max H: %.1f"), AdvanceMaxH);
    NGL_LOG(_T("font"), Level, _T("    under pos: %.1f"), UnderlinePos);
    NGL_LOG(_T("font"), Level, _T("    under thk: %.1f"), UnderlineThick);
  }
  else
  {
    NGL_LOG(_T("font"), Level, _T("    height : %.1f"), Height);
  }
  
  NGL_LOG(_T("font"), Level, _T("  Current settings"));
  NGL_LOG(_T("font"), Level, _T("    res.   : %.0fdpi"), pFont->GetResolution());
  NGL_LOG(_T("font"), Level, _T("    size   : %.1fpt / %.0fpx"), pFont->GetSize(eFontUnitPoint), pFont->GetSize(eFontUnitPixel));
  NGL_LOG(_T("font"), Level, _T("    charmap: %s"), pFont->GetCharMapName(pFont->GetCharMap()));
}


/*
 * nuiGlyphInfo
 */

nuiGlyphInfo::nuiGlyphInfo()
{
  Index = 0;
}

nuiGlyphInfo::~nuiGlyphInfo()
{
}

void nuiGlyphInfo::Dump (uint Level) const
{
  if (Index == 0)
  {
    NGL_LOG(_T("font"), Level, _T("Invalid glyph info"));
    return;
  }
  
  if (Index < 0)
  {
    NGL_LOG(_T("font"), Level, _T("Multiple glyph info (%d item%s) :"), -Index, PLURAL(-Index));
  }
  else
  {
    NGL_LOG(_T("font"), Level, _T("Glyph #%d info :"), Index);
  }
  NGL_LOG(_T("font"), Level, _T("  size    : %.1f x %.1f"), Width, Height);
  NGL_LOG(_T("font"), Level, _T("  bearing : %.1f x %.1f"), BearingX, BearingY);
  NGL_LOG(_T("font"), Level, _T("  advance : %.1f x %.1f"), AdvanceX, AdvanceY);
}



/*
 * nuiFontBase
 */

/*
 * Life cycle
 */

nuiFontBase::nuiFontBase(float Size)
{
  Defaults();
  Init();
  Load((FT_Byte*)gpDefaultFontBase, gDefaultFontSize, 0, true, Size);
}

nuiFontBase::nuiFontBase(uint8* pBuffer, uint32 BufferSize, uint Face, bool StaticBuffer, float Size)
{
  Defaults();
  Init();
  Load((FT_Byte*)pBuffer, BufferSize, Face, StaticBuffer, Size);
}

nuiFontBase::nuiFontBase (const nglPath& rFontFile, uint Face, float Size)
{
  Defaults();
  Init();
  Load(rFontFile, Face, Size);
}

nuiFontBase::nuiFontBase (const nuiFontBase& rFont)
{
  mpFace = new FaceHandle();
  
  // mpFace->Desc will be setup by Load() later
  
  mSize        = rFont.mSize;
  mResolution  = rFont.mResolution;
  mpConv       = rFont.mpConv;
  mRenderMode  = rFont.mRenderMode;
  
  Init();
  Load(rFont.mpFace->Desc.face_id, mSize);
}

nuiFontBase::~nuiFontBase()
{
  delete mpFace;

  //NGL_OUT(_T("DestroyFont: %p\n"), this);
  Textures::iterator it;
  Textures::iterator end = mTextures.end();
  for(it = mTextures.begin(); it != end; ++it)
  {
    nuiTexture* pTexture = *it;
    //NGL_OUT(_T("DestroyFontTexture: %p / %p\n"), this, pTexture);
    pTexture->Release();
  }
  mTextures.clear();
  mGlyphLocationLookupTable.clear();
}


/*
 * Goblal font info & metrics
 */

bool nuiFontBase::GetInfo (nuiFontInfo& rInfo, nuiFontUnit Unit) const
{
  FT_Face face = mpFace->GetFace();
  if (!face)
    return false;
  
  rInfo.pFont = this;
  
  rInfo.FamilyName = mFamilyName;
  rInfo.StyleName  = mStyleName;
  rInfo.IsScalable = IsScalable();
  rInfo.IsBold     = IsBold();
  rInfo.IsMonospace = IsMonospace();
  rInfo.IsItalic   = IsItalic();
  rInfo.GlyphCount = GetGlyphCount();
  rInfo.Unit       = Unit;
  rInfo.FaceCount  = face->num_faces;
  
  /* The following fields should contain valuable information only for scalable fonts.
   * We copy them anyway.
   */
  float ratio = 0.f;
  switch (Unit)
  {
    case eFontUnitEM    : ratio = nuiGetInvScaleFactor() * 1.0f; break;
    case eFontUnitPoint : ratio = EMToPoint(1.0f); break;
    case eFontUnitPixel : ratio = EMToPixel(1.0f); break;
  }
  rInfo.BBoxMinX       = face->bbox.xMin * ratio;
  rInfo.BBoxMinY       = face->bbox.yMin * ratio;
  rInfo.BBoxMaxX       = face->bbox.xMax * ratio;
  rInfo.BBoxMaxY       = face->bbox.yMax * ratio;
  rInfo.Ascender       = face->ascender * ratio;
  rInfo.Descender      = face->descender * ratio;
  rInfo.Height         = face->height * ratio;
  rInfo.AdvanceMaxW    = face->max_advance_width * ratio;
  rInfo.AdvanceMaxH    = face->max_advance_height * ratio;
  rInfo.UnderlinePos   = face->underline_position * ratio;
  rInfo.UnderlineThick = face->underline_thickness * ratio;
  
  return true;
}

nglString nuiFontBase::GetFamilyName() const
{
  return mFamilyName;
}

nglString nuiFontBase::GetStyleName() const
{
  return mStyleName;
}

bool nuiFontBase::IsScalable() const
{
  FT_Face face = mpFace->GetFace();
  return face ? FT_IS_SCALABLE(face) : false;
}

bool nuiFontBase::IsBold() const
{
  FT_Face face = mpFace->GetFace();
  return face ? ((face->style_flags & FT_STYLE_FLAG_BOLD) != 0) : false;
}

bool nuiFontBase::IsMonospace() const
{
  FT_Face face = mpFace->GetFace();
  return face ? ((face->face_flags & FT_FACE_FLAG_FIXED_WIDTH) != 0) : false;
}

bool nuiFontBase::IsItalic() const
{
  FT_Face face = mpFace->GetFace();
  return face ? ((face->style_flags & FT_STYLE_FLAG_ITALIC) != 0) : false;
}

uint nuiFontBase::GetFaceCount() const
{
  FT_Face face = mpFace->GetFace();
  return face ? face->num_faces : 0;
}

float nuiFontBase::GetHeight (nuiFontUnit Unit, float DefaultSpacing) const
{
  if (IsScalable())
  {
    switch (Unit)
    {
      case eFontUnitEM    : return mGlobalHeight; break;
      case eFontUnitPoint : return EMToPoint(mGlobalHeight); break;
      case eFontUnitPixel : return roundf(EMToPixel(mGlobalHeight)); break;
    }
  }
  else
  {
    switch (Unit)
    {
      case eFontUnitEM    : return 0.0f; break; // Undefined !
      case eFontUnitPoint : return PixelToPoint(mSize) + DefaultSpacing; break;
      case eFontUnitPixel : return roundf(mSize + DefaultSpacing); break;
    }
  }
  
  return 0.0f;
}

float nuiFontBase::GetAscender(nuiFontUnit Unit) const
{
  FT_Face face = mpFace->GetFace();
  float ratio = 0.f;
  switch (Unit)
  {
    case eFontUnitEM    : ratio = nuiGetInvScaleFactor() * 1.0f; break;
    case eFontUnitPoint : ratio = EMToPoint(1.0f); break;
    case eFontUnitPixel : ratio = EMToPixel(1.0f); break;
  }
  return face->ascender * ratio;
}

float nuiFontBase::GetDescender(nuiFontUnit Unit) const
{
  FT_Face face = mpFace->GetFace();
  float ratio = 0.f;
  switch (Unit)
  {
    case eFontUnitEM    : ratio = nuiGetInvScaleFactor() * 1.0f; break;
    case eFontUnitPoint : ratio = EMToPoint(1.0f); break;
    case eFontUnitPixel : ratio = EMToPixel(1.0f); break;
  }
  return face->ascender * ratio;
}

uint nuiFontBase::GetGlyphCount() const
{
  FT_Face face = mpFace->GetFace();
  return face ? (uint)face->num_glyphs : 0;
}

void nuiFontBase::GetGlyphs(std::set<nglUChar>& rGlyphs) const
{
  FT_Face face = mpFace->GetFace();
  rGlyphs.clear();
  
  FT_ULong  charcode = 0;
  FT_UInt   gindex = 0;
  
  charcode = FT_Get_First_Char( face, &gindex );
  while ( gindex != 0 )
  {
    //rGlyphs.insert(charcode);
    charcode = FT_Get_Next_Char( face, charcode, &gindex );     
  }
}

/*
 * Conversion methods
 */

float nuiFontBase::EMToPixel (float Value) const
{
  return  (Value * mSize) / mUnitsPerEM;
}

float nuiFontBase::EMToPoint (float Value) const
{
  return (Value * mSize * 72.0f / mResolution) / mUnitsPerEM;
}

float nuiFontBase::PixelToPoint (float Value) const
{
  return Value * 72.0f / mResolution;
}

float nuiFontBase::PointToPixel (float Value) const
{
  return Value * mResolution / 72.0f;
}


/*
 * Glyph metrics
 */

float nuiFontBase::GetSize (nuiFontUnit Unit) const
{
  switch (Unit)
  {
    case eFontUnitEM    : return 0.0f; // Does not really makes sense
    case eFontUnitPoint : return PixelToPoint(mSize);
    case eFontUnitPixel : return mSize;
  }
  
  return mSize;
}

bool nuiFontBase::SetSize (float Size, nuiFontUnit Unit)
{
  if (!mpFace->Face)
    return false;
  
  FT_Size       ftsize;
  FT_UShort     pixels = 0;
  
  // Fetch font ID
  mpFace->ftscaler.face_id  = mpFace->Desc.face_id;
  
  // Fetch font size in pixel units
  mpFace->ftscaler.pixel = 1; // TRUE
  switch (Unit)
  {
    case eFontUnitEM    : pixels = (FT_UInt)roundf (EMToPixel(Size)); break;
    case eFontUnitPoint : pixels = (FT_UInt)roundf (PointToPixel(Size)); break;
    case eFontUnitPixel : pixels = (FT_UInt)roundf (Size); break;
  }
  
  pixels *= nuiGetScaleFactor();
  
  NGL_ASSERT(pixels > 0);
  
  if (IsScalable())
  {
    mpFace->ftscaler.width  = pixels;
    mpFace->ftscaler.height = pixels;
  }
  else
  {
    int i;
    
    /* Search if we have a match in available fixed sizes.
     * We consider the 'pixels' size descriptor has being the font height.
     */
    for (i = 0; i < mpFace->Face->num_fixed_sizes; i++)
    {
      FT_Bitmap_Size& size = mpFace->Face->available_sizes[i];
      
      if (pixels == size.height)
      {
        mpFace->ftscaler.width  = size.width;
        mpFace->ftscaler.height = size.height;
        break;
      }
    }
    if (i >= mpFace->Face->num_fixed_sizes)
      return false;
  }
  
  mpFace->Desc.width   = pixels;
  mpFace->Desc.height  = pixels;
  mSize = pixels * nuiGetInvScaleFactor();
  return true;
}

float nuiFontBase::GetResolution() const
{
  return mResolution;
}

void nuiFontBase::SetResolution (float Resolution)
{
  mResolution = Resolution;
}


bool nuiFontBase::GetMetrics (nuiGlyphInfo& rInfo, nglUChar Char, GlyphType Type) const
{
  uint index;
  bool res = GetGlyphIndexes(&Char, 1, &index, 1) == 1 && GetGlyphInfo(rInfo, index, Type);
  return res;
}

bool nuiFontBase::GetGlyphInfo (nuiGlyphInfo& rInfo, uint Index, GlyphType Type) const
{
  FT_Glyph glyph;
  
  if (!(glyph = (FT_Glyph)GetGlyph(Index, Type)))
    return false;
  
  const float f = nuiGetInvScaleFactor();
  switch (glyph->format)
  {
    case FT_GLYPH_FORMAT_BITMAP:
    {
      FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyph;
      rInfo.Width    = f * (float)bitmap->bitmap.width;
      rInfo.Height   = f * (float)bitmap->bitmap.rows;
      rInfo.BearingX = f * (float)bitmap->left;
      rInfo.BearingY = f * (float)bitmap->top;
    }
      break;
      
    case FT_GLYPH_FORMAT_OUTLINE:
    {
      FT_BBox bbox;
      
      FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_pixels, &bbox);
      rInfo.Width    = f * (float)(bbox.xMax - bbox.xMin);
      rInfo.Height   = f * (float)(bbox.yMax - bbox.yMin);
      rInfo.BearingX = f * (float)bbox.xMin;
      rInfo.BearingY = f * (float)bbox.yMax;
    }
      break;
      
    default:
      return false;
  }
  
  rInfo.Index = Index;
  rInfo.AdvanceX = f * glyph->advance.x / 65536.0f;
  rInfo.AdvanceY = f * glyph->advance.y / 65536.0f;
  
  return true;
}

bool nuiFontBase::GetKerning (uint Left, uint Right, float& rX, float& rY) const
{
  if (!mpFace->Face || !mpFace->Face->size)
    return false;
  
  FT_Vector vec;
  
  if (!FT_Get_Kerning(mpFace->Face, Left, Right, FT_KERNING_UNFITTED, &vec))
  {
    rX = (float)vec.x;
    rY = (float)vec.y;
    
    if (IsScalable())
    {
      rX /= 64.0f;
      rY /= 64.0f;
    }
  }
  
  rX *= nuiGetInvScaleFactor();
  rY *= nuiGetInvScaleFactor();
  
  return true;
}


/*
 * Character maps (encoding)
 */

int nuiFontBase::GetCharMapCount() const
{
  FT_Face face = mpFace->GetFace();
  return face ? face->num_charmaps : 0;
}

int nuiFontBase::GetCharMap() const
{
  return mCharMap;
}

bool nuiFontBase::SetCharMap (int Index)
{
  FT_Face face = mpFace->GetFace();
  if (!face || (Index < -1) || (Index >= face->num_charmaps))
    return false;
  
  if (FT_Set_Charmap (face, face->charmaps[Index]) != 0)
    return false;
  
  mCharMap = Index;
  return true;
}

const nglChar* nuiFontBase::GetCharMapName ()  const
{
  FT_Face face = mpFace->GetFace();
  return face ? ::nuiGetCharMapName(face->charmap) : NULL;
}

const nglChar* nuiFontBase::GetCharMapName (int Index)  const
{
  FT_Face face = mpFace->GetFace();
  if (!face || (Index < 0) || (Index >= face->num_charmaps))
    return NULL;
  
  return ::nuiGetCharMapName(face->charmaps[Index]);
}


//#define DBG_INDEX

int nuiFontBase::GetGlyphIndexes (const nglChar* pSource, int SourceLength, uint* pIndexes, int IndexesLength) const
{
  nglUChar* pChars = (nglUChar*)nglString(pSource, SourceLength).Export(sizeof(nglUChar) == 2 ? eUCS2 : eUCS4);
  int res = GetGlyphIndexes(pChars, wcslen(pChars), pIndexes, IndexesLength);
  free(pChars);
  return res;
}

int nuiFontBase::GetGlyphIndexes (const nglUChar* pSource, int SourceLength, uint* pIndexes, int IndexesLength) const
{
  FT_Face face = mpFace->GetFace();
  if (!face)
    return -1;
  
  /*
   * The internal LoadFinish() method automatically tries to find a Unicode charmap,
   * and decides if a charset conversion (Unicode to locale or locale to Unicode) must be
   * performed (using mpConv). We must also handle the case where the font has no charmap.
   */
  
  int todo = MIN(SourceLength, IndexesLength);
  
  /* No selected or available charmap, or charmap conversion turned off : do a dumb copy
   */
  if (!face->charmap ||
      (face->charmap->encoding == ft_encoding_none))
  {
#ifdef DBG_INDEX
    NGL_OUT(_T("GetGlyphIndexes => Font encoding = none, using chars as glyph indices\n"));
#endif
    int i;
    
    for (i = 0; i < todo; i++)
      *pIndexes++ = (uint) *pSource++;
    
    return todo;
  }
  
  /* We have a charmap, prepare lookup cache info
   */
  FTC_FaceID face_id = mpFace->Desc.face_id;
  int done = 0;
  
#ifdef DBG_INDEX
  NGL_OUT(_T("GetGlyphIndexes('"));
  int i;
  for (i = 0; i < SourceLength; i++) NGL_OUT(_T("%lc"), pSource[i]);
  NGL_OUT(_T("' [%d], 0x%p [%d])\n"), SourceLength, pIndexes, IndexesLength);
#endif
  
  if (mpConv)
  {
    // nglChar -> locale
    int to_write0 = SourceLength;
    char* buffer = (char*) alloca(to_write0);
    
    const char* source   = (const char*) pSource;
    int         to_read  = SourceLength;
    char*       dest     = (char*) buffer;
    int         to_write = to_write0;
    
#ifdef DBG_INDEX
    NGL_OUT(_T("  to_write: %d bytes (buffer: 0x%p)\n"), to_write, dest);
#endif
    mpConv->Process(source, to_read, dest, to_write);
    
    // Convert from byte count to glyph count
    todo = to_write0 - to_write;
    
    todo /= sizeof(nglChar);
    
#ifdef DBG_INDEX
    NGL_OUT(_T("  todo    : %d glyphs\n"), todo);
    NGL_OUT(_T("  codes   :"));
    for (i = 0; i < todo; i++) NGL_OUT(_T(" %d"), buffer[i]);
    NGL_OUT(_T("\n"));
    
    NGL_OUT(_T("  indexes :"));
#endif
    done = todo;
    while (todo > 0)
    {
      *pIndexes++ = FTC_CMapCache_Lookup (gFTCMapCache, face_id, mCharMap, *buffer++);
#ifdef DBG_INDEX
      NGL_OUT(_T(" %d"), pIndexes[-1]);
#endif
      todo--;
    }
#ifdef DBG_INDEX
    NGL_OUT(_T("\n"));
#endif
  }
  else
  {
    done = todo;
    while (todo > 0)
    {
      *pIndexes++ = FTC_CMapCache_Lookup (gFTCMapCache, face_id, mCharMap, *pSource++);
#ifdef DBG_INDEX
      //      NGL_OUT(_T(" %d -> %d"), pSource[-1], pIndexes[-1]);
#endif
      todo--;
    }
  }
  
  return done;
}


int32 nuiFontBase::GetGlyphIndex(nglUChar Source, nglUChar VariationSelector) const
{
  FT_Face face = mpFace->GetFace();
  if (!face)
    return -1;
  
  /* No selected or available charmap, or charmap conversion turned off : do a dumb copy
   */
  if (!face->charmap ||
      (face->charmap->encoding == ft_encoding_none))
  {
#ifdef DBG_INDEX
    NGL_OUT(_T("GetGlyphIndexes => Font encoding = none, using chars as glyph indices\n"));
#endif
    return Source;
  }
  
  /* We have a charmap, prepare lookup cache info
   */
  FTC_FaceID face_id = mpFace->Desc.face_id;
  int done = 0;
  
  return FTC_CMapCache_Lookup (gFTCMapCache, face_id, mCharMap, Source);
}

/*
 * Render mode
 */

nuiFontBase::RenderMode nuiFontBase::GetRenderMode() const
{
  return mRenderMode;
}

bool nuiFontBase::SetRenderMode (nuiFontBase::RenderMode Mode)
{
  FT_Face face = mpFace->GetFace();
  if (!face || !IsScalable())
    return false;
  
  FT_Int32 flags = FT_LOAD_DEFAULT;
  
  if (!(Mode & AntiAliasing))
    flags |= FT_LOAD_TARGET_MONO;
  
  if (!(Mode & Hinting))
    flags |= FT_LOAD_NO_HINTING;
  
  //flags |= FT_LOAD_TARGET_LCD;
  
  mpFace->Desc.flags = flags;
  mRenderMode = Mode;
  
  return true;
}


/*
 * Protected interface
 */

nuiFontBase::GlyphHandle nuiFontBase::GetGlyph (uint Index, GlyphType Type) const
{
  FT_Face face = mpFace->GetFace();
  FT_Glyph glyph;
  FTC_ImageTypeRec desc = mpFace->Desc; // Our query, with .flags = FT_LOAD_DEFAULT
  
  switch (Type)
  {
    case eGlyphNative : break;
    case eGlyphOutline: break;
    case eGlyphBitmap : desc.flags |= FT_LOAD_RENDER; break;
  }
  
  if (FTC_ImageCache_Lookup(gFTImageCache, &desc, Index, &glyph, NULL) != FT_Err_Ok)
    return NULL;
  
  switch (Type)
  {
    case eGlyphNative : break;
    case eGlyphOutline: if (glyph->format != FT_GLYPH_FORMAT_OUTLINE) return NULL; break;
    case eGlyphBitmap : if (glyph->format != FT_GLYPH_FORMAT_BITMAP) return NULL; break;
  }
  return (GlyphHandle)glyph;
}

nuiFontBase::GlyphType nuiFontBase::GetGlyphType (GlyphHandle Glyph) const
{
  FT_Glyph glyph = (FT_Glyph)Glyph;
  
  switch (glyph->format)
  {
    case FT_GLYPH_FORMAT_BITMAP : return eGlyphBitmap;
    case FT_GLYPH_FORMAT_OUTLINE: return eGlyphOutline;
    default:
      return eGlyphNative; // Actually this is an error
  }
}

bool nuiFontBase::GetGlyphBitmap (GlyphHandle Glyph, GlyphBitmap& rBitmap) const
{
  FT_Glyph glyph = (FT_Glyph)Glyph;
  
  if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
    return false;
  
  FT_BitmapGlyph bglyph = (FT_BitmapGlyph)glyph;
  FT_Bitmap* bmp = &bglyph->bitmap;
  
  rBitmap.Width  = bmp->width;
  rBitmap.Height = bmp->rows;
  rBitmap.Top    = bglyph->top;
  rBitmap.Left   = bglyph->left;
  rBitmap.pData  = bmp->buffer;
  rBitmap.Pitch  = bmp->pitch;
  switch (bmp->pixel_mode)
  {
    case FT_PIXEL_MODE_MONO: rBitmap.Depth = 1; break;
    case FT_PIXEL_MODE_GRAY: rBitmap.Depth = 8; break;
    default:
      rBitmap.Depth = 0;
  }
  
  return true;
}

/* This lookup table is designed to expand a single bit to a 'set byte' (0xFF)
 * as fast as possible. It can proceed by quartets : 4 bit input (ie. 0..15),
 * 4 byte output per lookup. See GetBitmap8().
 */
const uint32 nuiFontBase::mBitLUT[16] =
{
#if (BYTE_ORDER == LITTLE_ENDIAN) // x86 and co
  /* 0000 */ 0x00000000,
  /* 0001 */ 0xFF000000,
  /* 0010 */ 0x00FF0000,
  /* 0011 */ 0xFFFF0000,
  /* 0100 */ 0x0000FF00,
  /* 0101 */ 0xFF00FF00,
  /* 0110 */ 0x00FFFF00,
  /* 0111 */ 0xFFFFFF00,
  /* 1000 */ 0x000000FF,
  /* 1001 */ 0xFF0000FF,
  /* 1010 */ 0x00FF00FF,
  /* 1011 */ 0xFFFF00FF,
  /* 1100 */ 0x0000FFFF,
  /* 1101 */ 0xFF00FFFF,
  /* 1110 */ 0x00FFFFFF,
  /* 1111 */ 0xFFFFFFFF
#else
  /* 0000 */ 0x00000000,
  /* 0001 */ 0x000000FF,
  /* 0010 */ 0x0000FF00,
  /* 0011 */ 0x0000FFFF,
  /* 0100 */ 0x00FF0000,
  /* 0101 */ 0x00FF00FF,
  /* 0110 */ 0x00FFFF00,
  /* 0111 */ 0x00FFFFFF,
  /* 1000 */ 0xFF000000,
  /* 1001 */ 0xFF0000FF,
  /* 1010 */ 0xFF00FF00,
  /* 1011 */ 0xFF00FFFF,
  /* 1100 */ 0xFFFF0000,
  /* 1101 */ 0xFFFF00FF,
  /* 1110 */ 0xFFFFFF00,
  /* 1111 */ 0xFFFFFFFF
#endif
};

bool nuiFontBase::GetBitmap8 (const GlyphBitmap& rSrc, GlyphBitmap& rDst) const
{
  if (!rSrc.pData || (rSrc.Depth != 1) || !rDst.pData)
    return false;
  
  uint8* dst_row = rDst.pData;
  uint8* src_row = rSrc.pData;
  uint i, j;
  
  for (j = 0; j < rSrc.Height; j++)
  {
    uint8* src_p = src_row;
    uint32* dst_p = (uint32*) dst_row;
    
    for (i = 0; i < rSrc.Width; i += 8)
    {
#if (BYTE_ORDER == LITTLE_ENDIAN) // x86 and co
      *dst_p++ = mBitLUT[*src_p >> 4]; // First quartet -> first four bytes
      *dst_p++ = mBitLUT[*src_p & 15]; // Second quartet -> last four bytes
#else
      *dst_p++ = mBitLUT[*src_p & 15];
      *dst_p++ = mBitLUT[*src_p >> 4];
#endif
      src_p++;
    }
    src_row += rSrc.Pitch;
    dst_row += rDst.Pitch;
  }
  
  return true;
}

bool nuiFontBase::IsValid() const
{
  return mValid;
}


/*
 * Internals
 */

void nuiFontBase::Defaults()
{
  mValid = false;

  /* The face object (is managed by the cache)
   */
  mpFace = new FaceHandle();
  
  /* A nuiFont is described by mpFace->Desc, this is used to query the
   * face or glyph objects through the cache manager. Face IDs
   * are managed via nuiFontInstance.
   */
  mpFace->Desc.face_id = 0;
  mpFace->Desc.width = 0;
  mpFace->Desc.height = 0;
  
  // Scalable fonts : we'll set default size once the face is loaded
  // Fixed fonts : we'll take the first available fixed size
  mSize = 0.0f;
  
  // Setup default resolution
  mResolution = 72.0f; // dot <-> pixel identity
  
  // Will be set at Load() time
  mCharMap    = 0;
  mpConv      = NULL;
  mRenderMode = 0;
  
  mLastResort = false;

  SetAlphaTest();
  
  mCurrentX = 0;
  mCurrentY = 0;
  mCurrentTexture = 0;
  
  mRowMaxWidth = 0;
  
  nuiTexture *texture = AllocateTexture(nuiFontBase::TEXTURE_SIZE);
  mTextures.push_back(texture);
}

#define NGL_FTCACHE_MAX_FACES 5000
#define NGL_FTCACHE_MAX_SIZES 400
#define NGL_FTCACHE_MAX_BYTES (1024*1024*100)

bool nuiFontBase::Init()
{
  FT_Error error;
  
  // Build a FT library instance if necessary
  if (!gFTLibrary)
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Initializing FreeType library")); )
    error = FT_Init_FreeType (&gFTLibrary);
    if (error)
    {
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Unable to initialize the FreeType library")); )
      //SetError (_T("font"), NGL_FONT_EINIT);
      return false;
    }
    App->AddExit (OnExit);
#ifdef _DEBUG_
    FT_Int major, minor, patch;
    FT_Library_Version(gFTLibrary, &major, &minor, &patch);
    NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("  version %d.%d.%d"), major, minor, patch);
#endif // _DEBUG_
    
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Initializing FreeType cache (%d KB)"), FTCACHE_MAX_BYTES/1024); )
    // Use freetype defaults : (library, max_faces, max_sizes, max_bytes, [...])
    error = FTC_Manager_New (gFTLibrary, NGL_FTCACHE_MAX_FACES, NGL_FTCACHE_MAX_SIZES, NGL_FTCACHE_MAX_BYTES, nuiFontInstance::FaceRequestHandler, NULL, &gFTCacheManager);
    NGL_DEBUG( if (error) NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Couldn't create face cache manager (FreeType error: %d)"), error); )
    
    if (gFTCacheManager)
    {
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO + 1, _T("  adding charmap cache")); )
      error = FTC_CMapCache_New (gFTCacheManager, &gFTCMapCache);
      NGL_DEBUG( if (error) NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Couldn't create charmap cache (FreeType error: %d)"), error); )
      
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO + 1, _T("  adding image cache")); )
      error = FTC_ImageCache_New (gFTCacheManager, &gFTImageCache);
      NGL_DEBUG( if (error) NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Couldn't create image cache (FreeType error: %d)"), error); )
      
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO + 1, _T("  adding small bitmap cache")); )
      error = FTC_SBitCache_New(gFTCacheManager, &gFTSBitmapCache);
      NGL_DEBUG( if (error) NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Couldn't create small bitmap cache (FreeType error: %d)"), error); )
    }
  }
  
  return true;
}

bool nuiFontBase::Load (const nglPath& rPath, uint Face, float Size)
{
  // Load face from file spec
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Loading logical font '%s' (face %d)"), rPath.GetNodeName().GetChars(), Face); )
  mpFace->SetFontInstance(new nuiFontInstance(rPath, Face));
  mpFace->Desc.face_id = nuiFontInstance::Install(mpFace->GetFontInstance());
  
  return LoadFinish(Size);
}

bool nuiFontBase::Load (const uint8* pBase, int32 BufferSize, uint Face, bool StaticBuffer, float Size)
{
  // Load face from memory
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Loading logical font at %p (face %d, %d bytes)"), pBase, Face, Size); )
  mpFace->SetFontInstance(new nuiFontInstance(pBase, BufferSize, Face, StaticBuffer));
  mpFace->Desc.face_id = nuiFontInstance::Install(mpFace->GetFontInstance());
  
  return LoadFinish(Size);
}

bool nuiFontBase::Load (FaceID ID, float Size)
{
  // Load face from FreeType cache ID
#ifdef _DEBUG_
  nuiFontInstance* ifont = nuiFontInstance::Lookup((FTC_FaceID)ID);
  if (ifont)
    NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Copying logical font '%s' (face %d)"), ifont->GetPath().GetNodeName().GetChars(), ifont->GetFace());
#endif
  
  mpFace->Desc.face_id = (FTC_FaceID)ID;
  
  return LoadFinish(Size);
}

bool nuiFontBase::LoadFinish(float Size)
{
  mValid = false;

  /* Fetch generic (maybe unsized) face
   */
  if (FTC_Manager_LookupFace (gFTCacheManager, mpFace->Desc.face_id, &mpFace->Face) != FT_Err_Ok)
  {
    //SetError(_T("font"), NGL_FONT_ELOAD);
    return false;
  }
  
  /* Select a default size
   */
  if (!SetSize(Size, eFontUnitPixel))
    return false;
  if (IsScalable())
    SetRenderMode(AntiAliasing | Hinting);
  

  FT_Face face = mpFace->Face;
  if (!face)
    return false;
  
  /* Initialize some global info fields
   */
  mFamilyName.Import (face->family_name);
  if (mFamilyName == _T("LastResort"))
    mLastResort = true;
  mStyleName.Import (face->style_name);
  mUnitsPerEM = (float)face->units_per_EM;
  mGlobalHeight = (float)face->height; // Only valid for scalable fonts (see GetHeight())
  
  // Get Panose information from the TT OS/2 tables
  TT_OS2* pOS2 = (TT_OS2*)FT_Get_Sfnt_Table(face, ft_sfnt_os2);
  if (pOS2)
  {
    memcpy(&mPanoseBytes, pOS2->panose, 10);
    mHasPanoseInfo = true;
  }
  else
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Warning: font '%s (%s)' has no panose information.\n"), mFamilyName.GetChars(), mStyleName.GetChars()); )
    memset(&mPanoseBytes, 0, 10);
    mHasPanoseInfo = false;
  }
  
  /* Select a default charmap
   *
   * (see GetGlyphindexes for more info)
   */
  if (FT_Select_Charmap (face, FT_ENCODING_UNICODE) != FT_Err_Ok)
  {
#ifdef USE_WCHAR
    // We can't select Unicode output, we'll do nglChar -> locale and use the first map (if available)
    mpConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, eEncodingNative));
#else
    // We expect that the first charmap is actually the right one for the locale's encoding
#endif
    FT_Set_Charmap (face, face->charmaps[0]);
  }
#ifndef USE_WCHAR
  else
    // We have a Unicode charmap, we'll do locale -> UCS-2 conversion
    mpConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, eUCS2));
#endif
  mCharMap = FT_Get_Charmap_Index(face->charmap);
  
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("  selected charmap   : %s (#%d)"), GetCharMapName(), GetCharMap()); )
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("  charmap conversion : %s"), YESNO(mpConv)); )
  
  mValid = true;

  return true;
}

void nuiFontBase::GetEncodings(std::set<nglTextEncoding>& rEncodings)
{
  FT_Face face = mpFace->GetFace();
  rEncodings.clear();
  
  int32 count = GetCharMapCount();
  
  for (int32 i = 0; i < count; i++)
  {
    FT_CharMap charmap = face->charmaps[i];
    rEncodings.insert(nuiGetCharMapEncoding(charmap));
  }
}

void nuiFontBase::GetSizes(std::set<int32>& rSizes)
{
  rSizes.clear();
  
  for (int32 i = 0; i < mpFace->Face->num_fixed_sizes; i++)
  {
    FT_Bitmap_Size& size = mpFace->Face->available_sizes[i];
    
    rSizes.insert(size.height);
  }
}


/*
 * Callbacks
 */

void nuiFontBase::OnExit()
{
  nuiFontInstance::OnExit();
  
  if (gFTCacheManager)
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Releasing FreeType cache")); )
    FTC_Manager_Done (gFTCacheManager);
    gFTCacheManager = NULL;
  }
  if (gFTLibrary)
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Releasing FreeType library")); )
    FT_Done_FreeType (gFTLibrary);
    gFTLibrary = NULL;
  }
}

const nuiFontPanoseBytes& nuiFontBase::GetPanoseBytes() const
{
  return mPanoseBytes;
}

bool nuiFontBase::HasPanoseInfo() const
{
  return mHasPanoseInfo;
}

bool nuiFontBase::IsLastResort() const
{
  return mLastResort;
}



nuiTexture *nuiFontBase::AllocateTexture(int size)
{
  size *= nuiGetScaleFactor();
  nglImageInfo ImageInfo(false);
  ImageInfo.mBufferFormat = eImageFormatRaw;
  ImageInfo.mPixelFormat = eImagePixelAlpha;
  ImageInfo.mWidth = size;
  ImageInfo.mHeight = size;
  ImageInfo.mBitDepth = 8;
  ImageInfo.mBytesPerPixel = 1;
  ImageInfo.mBytesPerLine = ImageInfo.mWidth * ImageInfo.mBytesPerPixel;
  ImageInfo.mpBuffer = NULL;

  ImageInfo.AllocateBuffer();
  memset(ImageInfo.mpBuffer, 0, ImageInfo.mBytesPerLine * ImageInfo.mHeight);

  nuiTexture *pTexture = nuiTexture::GetTexture(ImageInfo, true);
  pTexture->SetRetainBuffer(true);

  pTexture->SetEnvMode(GL_MODULATE);
//  pTexture->SetMinFilter(GL_NEAREST);
//  pTexture->SetMagFilter(GL_NEAREST);
  pTexture->SetMinFilter(GL_LINEAR);
  pTexture->SetMagFilter(GL_LINEAR);

#ifdef _OPENGL_ES_
  pTexture->SetWrapS(GL_CLAMP_TO_EDGE);
  pTexture->SetWrapT(GL_CLAMP_TO_EDGE);
#else
  pTexture->SetWrapS(GL_CLAMP);
  pTexture->SetWrapT(GL_CLAMP);
#endif

  return pTexture;
}

void nuiFontBase::Blit8BitsBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY)
{
  nglImage *Image = pTexture->GetImage();
  char *pBuffer = Image->GetBuffer();
  uint32 x,y;

  uint32 w = pTexture->GetWidth();
  
  for (y = 0; y < rBitmap.Height; y++)
  {
    uint32 DstOffset = OffsetX + (y + OffsetY )* w;
    uint32 SrcOffset = y * rBitmap.Pitch;

    uint8* pSrc = rBitmap.pData + SrcOffset;
    uint8* pDst = (uint8*)pBuffer + DstOffset;

    for (x = 0; x < rBitmap.Width; x++) 
    {
      *pDst++ = *pSrc++;
    }
  }
  pTexture->ForceReload();
}

bool nuiFontBase::CopyBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY)
{
  int32 Width = rBitmap.Width;
  int32 Height = rBitmap.Height;
  switch (rBitmap.Depth)
  {
  case 1:
    {
      /* 
      * We can't load it as a bitmap texture because it will be used as an alpha map.
      * Convert every bit to a full byte into a temporary buffer.
      */
      GlyphBitmap bmp8 = rBitmap;
      bmp8.Pitch = (Width + 7) & ~7; // row length in bytes, 8-bytes aligned
      bmp8.pData = new uint8[bmp8.Pitch * Height];

      if (GetBitmap8(rBitmap, bmp8))
      {
        Blit8BitsBitmapToTexture(bmp8, pTexture, OffsetX, OffsetY);
      }
      delete bmp8.pData;
    }
    break;

  case 8:
    {
      Blit8BitsBitmapToTexture(rBitmap, pTexture, OffsetX, OffsetY);
    }
    break;

  default:
    {
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("nuiFont: unsupported input bitmap pixel depth (%d)"), rBitmap.Depth); )
        return false;
    }
  }
  return true;
}

bool nuiFontBase::FindNextGlyphLocation(int Width, int Height, int &rOffsetX, int &rOffsetY)
{
  if ((rOffsetX + Width) < TEXTURE_SIZE)
  {
    if ((rOffsetY + Height) < TEXTURE_SIZE)
    {
      // there is enough space available
      mRowMaxWidth = max(mRowMaxWidth, Width);
      return true; 
    }
    
    // Go back to the top of the texture, on the next column:
    rOffsetX += mRowMaxWidth + 2;
    rOffsetY = 0;     

    if ((rOffsetX + Width) < TEXTURE_SIZE)
    {
      if ((rOffsetY + Height) < TEXTURE_SIZE)
      {
        mRowMaxWidth = Width;
        return true;
      }
      else // should move to another texture
      {
        return false;
      }
    }
  }
  return false; // Move to another texture, this one is full...
}

uint32 POT(uint32 i)
{
  uint32 t = 1;
  while (t < i)
  {
    t *= 2;
  }
  
  return t;
}

void nuiFontBase::AddCacheGlyph(int Index, nuiFontBase::GlyphLocation &rGlyphLocation)
{
  // Fetch rendered glyph
  GlyphHandle glyph = GetGlyph(Index, eGlyphBitmap);
  GlyphBitmap bmp;
  GetGlyphBitmap(glyph, bmp);

  int OffsetX = mCurrentX;
  int OffsetY = mCurrentY;

  if (FindNextGlyphLocation(bmp.Width + 1, bmp.Height + 1, OffsetX, OffsetY)) 
  {
    OffsetX;
    OffsetY;
    nuiTexture *pCurrentTexture = mTextures[mCurrentTexture];

    rGlyphLocation = GlyphLocation (OffsetX + 1, OffsetY + 1, bmp.Width, bmp.Height, mCurrentTexture);
    mGlyphLocationLookupTable.insert(std::pair<int, nuiFontBase::GlyphLocation>(Index, rGlyphLocation));

    CopyBitmapToTexture(bmp, pCurrentTexture, OffsetX + 1, OffsetY + 1);

    mCurrentX = OffsetX;
    mCurrentY = OffsetY + bmp.Height + 2;
  }
  else
  {
    mCurrentTexture++;
    nuiTexture *texture = AllocateTexture(MAX(TEXTURE_SIZE, POT(MAX(bmp.Width, bmp.Height))));
    mTextures.push_back(texture);

    rGlyphLocation = GlyphLocation (1, 1, bmp.Width, bmp.Height, mCurrentTexture);
    mGlyphLocationLookupTable.insert(std::pair<int, nuiFontBase::GlyphLocation>(Index, rGlyphLocation));

    CopyBitmapToTexture(bmp, texture, 1, 1);

    mCurrentX = 0;
    mCurrentY = bmp.Height + 2;
    mRowMaxWidth = bmp.Width;
  }
}

void nuiFontBase::GetCacheGlyph(int Index, nuiFontBase::GlyphLocation &rGlyphLocation)
{
  GlyphLocationLookupTableIterator iterator = mGlyphLocationLookupTable.find(Index);
  if (iterator == mGlyphLocationLookupTable.end())
  {
    AddCacheGlyph(Index, rGlyphLocation); 
  }
  else
  {
    rGlyphLocation = iterator->second;
  }
}

void nuiFontBase::SetAlphaTest(float Threshold)
{
  mAlphaTest = Threshold;
}

void nuiFontBase::Print(nuiDrawContext *pContext, float X, float Y, const nglString& rText, bool AlignGlyphPixels)
{
  nuiTextLayout layout(this);
  layout.Layout(rText);
  layout.Print(pContext, X, Y, AlignGlyphPixels);
}

bool nuiFontBase::PrepareGlyph(float X, float Y, nuiTextGlyph& rGlyph)
{
  // Fetch rendered glyph
  GlyphHandle glyph = GetGlyph(rGlyph.Index, eGlyphBitmap);
  
  // If we don't have this glyph, assert it has not been rendered
  if (!glyph)
    return false;
  
  GlyphBitmap bmp;
  if (!GetGlyphBitmap(glyph, bmp))
    return false;
  
  nuiFontBase::GlyphLocation GlyphLocation;
  GetCacheGlyph(rGlyph.Index, GlyphLocation);
  
  float w = GlyphLocation.mWidth;
  float h = GlyphLocation.mHeight;
  
  float x = rGlyph.mX + bmp.Left * nuiGetInvScaleFactor();
  float y = rGlyph.mY - bmp.Top * nuiGetInvScaleFactor();
  
  rGlyph.mpTexture = mTextures[GlyphLocation.mOffsetTexture];
  
  float ww = w * nuiGetInvScaleFactor();
  float hh = h * nuiGetInvScaleFactor();
  
  rGlyph.mDestRect.Set(X + x - 1, Y + y - 1, ww + 2, hh + 2);
  rGlyph.mSourceRect.Set(GlyphLocation.mOffsetX - nuiGetScaleFactor(), GlyphLocation.mOffsetY - nuiGetScaleFactor(), w + 2 * nuiGetScaleFactor(), h + 2 * nuiGetScaleFactor());
  
  return true;
}


nuiFontBase::GlyphLocation::GlyphLocation()
{
}

nuiFontBase::GlyphLocation::GlyphLocation(int OffsetX, int OffsetY, int Width, int Height, int OffsetTexture)
: mOffsetX(OffsetX), mOffsetY(OffsetY), mWidth(Width), mHeight(Height), mOffsetTexture(OffsetTexture)
{
}

nuiFontBase::GlyphLocation::~GlyphLocation()
{
}


////////////


void nuiFontBase::Shape(nuiTextRun* pRun)
{
  if (pRun->IsDummy())
    return;
  NGL_ASSERT(this == pRun->mStyle.GetFont());
  
  FT_Face ft_face = mpFace->GetFace();
  hb_font_t *hb_font = hb_ft_font_create(ft_face, NULL);
  hb_buffer_t *hb_buffer;
  hb_glyph_info_t *hb_glyph;
  hb_glyph_position_t *hb_position;
  unsigned int num_glyphs, i;
  hb_position_t x;
  const nglUChar* text = NULL;
  int32 len = 0;

  text = pRun->GetUnicodeChars();
  len = pRun->GetLength();
  
  hb_buffer = hb_buffer_create(len);
  
  hb_buffer_set_unicode_funcs(hb_buffer, nui_hb_get_unicode_funcs());
  
  //hb_buffer_set_direction(hb_buffer, HB_DIRECTION_TTB);
  hb_buffer_set_script(hb_buffer, hb_get_script_from_nui(pRun->GetScript()));
  hb_buffer_add_utf32(hb_buffer, (const uint32_t *)text, len, 0, len);
  //if (language)
  //  hb_buffer_set_language(hb_buffer, hb_language_from_string(language));
  
  hb_feature_t* features = NULL;
  int32 num_features = 0;
  
  hb_shape(hb_font, hb_buffer, features, num_features);
  
  num_glyphs = hb_buffer_get_length(hb_buffer);
  hb_glyph = hb_buffer_get_glyph_infos(hb_buffer, NULL);
  hb_position = hb_buffer_get_glyph_positions(hb_buffer, NULL);
  //cairo_glyphs = cairo_glyph_allocate(num_glyphs + 1);
  pRun->mGlyphs.clear();
  pRun->mGlyphs.resize(num_glyphs);
  x = 0;
  
  //printf("Shape %p\n", pRun);
  for (i = 0; i < num_glyphs; i++)
  {
    GetGlyphInfo(pRun->mGlyphs[i], hb_glyph->codepoint, eGlyphNative);
    pRun->mGlyphs[i].mCluster = hb_glyph->cluster;
    pRun->mGlyphs[i].mX = (hb_position->x_offset + x) * (1./64);
    pRun->mGlyphs[i].mY = -(hb_position->y_offset)    * (1./64);
    x += hb_position->x_advance;
    
    //printf("%d - %d (%d, %d) ##", hb_glyph->codepoint, hb_glyph->cluster,  ToNearest(pRun->mGlyphs[i].mX), ToNearest(pRun->mGlyphs[i].mY));

    hb_glyph++;
    hb_position++;

  }

  //printf("\n");
  
  pRun->mAdvanceX = x * (1./64);
  hb_buffer_destroy(hb_buffer);
  hb_font_destroy(hb_font);
}

