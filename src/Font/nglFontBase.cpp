/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#ifdef HAVE_FREETYPE

#include "nglKernel.h"
#include "nglVideoMode.h"
#include "nglFontBase.h"
#include "nglFontInstance.h"
#include "nglFontLayout.h"
#include "nglMath.h"
#include "ngl_default_font.h"

using namespace std;

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_TRUETYPE_TABLES_H

extern float NUI_SCALE_FACTOR;
extern float NUI_INV_SCALE_FACTOR;

/* Globals
 */
FT_Library     gFTLibrary      = NULL;  // Global FT library instance
FTC_Manager    gFTCacheManager = NULL;  // Global FT cache manager
FTC_CMapCache  gFTCMapCache    = NULL;  // Charmap lookup cache
FTC_ImageCache gFTImageCache   = NULL;  // Generic client-side glyph cache
FTC_SBitCache  gFTSBitmapCache = NULL;

static const nglChar* gpFontBaseErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("FreeType library initialisation failed"),
/*  2 */ _T("Couldn't load font ressource"),
          NULL
};

/* Pick a default pixel size which is very legible for the default (embedded) font
 */
float nglFontBase::DefaultPixelSize = 14.f;

const nglFontBase::RenderMode nglFontBase::AntiAliasing = (1 << 0);
const nglFontBase::RenderMode nglFontBase::Hinting      = (1 << 1);


/* FreeType cache settings
 *
 * Note on 'MAX_FACES': nglFontBase keep a reference to a face object (mpFace->Face), which
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

  
  FaceHandle()
    : Face(NULL), mpFontInstance(new nglFontInstance(nglPath(), 0))
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
      nglFontInstance::Uninstall(mpFontInstance);
      mpFontInstance->Release();
    }
  }
  
  void SetFontInstance(nglFontInstance* pFontInstance)
  {
    pFontInstance->Acquire();
    mpFontInstance->Release();
    mpFontInstance = pFontInstance;
  }
  
  nglFontInstance* GetFontInstance() const
  {
    return mpFontInstance;
  }
private:
  nglFontInstance* mpFontInstance;

};


/* Opaque structure which is only used as a pointer (via GlyphHandle)
 */
class GlyphData
{
public:
};


const nglChar* nglGetCharMapName (FT_CharMap CharMap); // Charmap to human readable description


/*
 * nglFontDesc
 */

nglFontDesc::nglFontDesc()
{
  Name      = "default";
  IsBold    = false;
  IsMonospace = false;
  IsItalic  = false;
  PixelSize = 0;
}

nglFontDesc::nglFontDesc(const nglFontDesc& rDesc)
{
  Name      = rDesc.Name;
  IsBold    = rDesc.IsBold;
  IsMonospace = rDesc.IsMonospace;
  IsItalic  = rDesc.IsItalic;
  PixelSize = rDesc.PixelSize;
  mPath     = rDesc.mPath;
}

nglPath nglFontDesc::GetPath() const
{
  return mPath;
}

bool nglFontDesc::Find(const nglFontDesc& rDesc)
{
  // FIXME
  return false;
}


/*
 * nglFontInfo
 */

const nglChar* nglFontInfo_UnitName(nglFontUnit Unit)
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

nglFontInfo::nglFontInfo()
{
  pFont = NULL;
}

nglFontInfo::~nglFontInfo()
{
}

void nglFontInfo::Dump (uint Level) const
{
  if (!pFont)
  {
    NGL_LOG(_T("font"), Level, _T("Invalid font info"));
    return;
  }

  NGL_LOG(_T("font"), Level, _T("Font info"));
  nglFontInstance* ifont = nglFontInstance::Lookup(pFont->mpFace->Desc.face_id);
  if (ifont)
  {
    NGL_LOG(_T("font"), Level, _T("  source   : %ls"), ifont->GetPath().GetChars());
    NGL_LOG(_T("font"), Level, _T("  face     : %d"), ifont->GetFace());
  }
  NGL_LOG(_T("font"), Level, _T("  family   : %ls"), FamilyName.GetChars());
  NGL_LOG(_T("font"), Level, _T("  style    : %ls"), StyleName.GetChars());
  NGL_LOG(_T("font"), Level, _T("  scalable : %ls"), YESNO(IsScalable));
  NGL_LOG(_T("font"), Level, _T("  bold     : %ls"), YESNO(IsBold));
  NGL_LOG(_T("font"), Level, _T("  monospace: %ls"), YESNO(IsMonospace));
  NGL_LOG(_T("font"), Level, _T("  italic   : %ls"), YESNO(IsItalic));
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
  NGL_LOG(_T("font"), Level, _T("  charmaps :%ls"), avail_map.GetChars());

  NGL_LOG(_T("font"), Level, _T("  Global metrics, in `%ls' units"), nglFontInfo_UnitName(Unit));
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
  NGL_LOG(_T("font"), Level, _T("    charmap: %ls"), pFont->GetCharMapName(pFont->GetCharMap()));
}


/*
 * nglGlyphInfo
 */

nglGlyphInfo::nglGlyphInfo()
{
  Index = 0;
}

nglGlyphInfo::~nglGlyphInfo()
{
}

void nglGlyphInfo::Dump (uint Level) const
{
  if (Index == 0)
  {
    NGL_LOG(_T("font"), Level, _T("Invalid glyph info"));
    return;
  }

  if (Index < 0)
  {
    NGL_LOG(_T("font"), Level, _T("Multiple glyph info (%d item%ls) :"), -Index, PLURAL(-Index));
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
 * nglFontBase
 */

/*
 * Life cycle
 */

nglFontBase::nglFontBase()
{
  Defaults();
  Init();
  Load((FT_Byte*)gpDefaultFontBase, gDefaultFontSize, 0, true);
}

nglFontBase::nglFontBase(uint8* pBuffer, uint32 BufferSize, uint Face, bool StaticBuffer)
{
  Defaults();
  Init();
  Load((FT_Byte*)pBuffer, BufferSize, Face, StaticBuffer);
}

nglFontBase::nglFontBase (const nglPath& rFontFile, uint Face)
{
  Defaults();
  Init();
  Load(rFontFile, Face);
}

nglFontBase::nglFontBase (const nglFontBase& rFont)
{
  mpFace = new FaceHandle();

  // mpFace->Desc will be setup by Load() later

  mSize        = rFont.mSize;
  mResolution  = rFont.mResolution;
  mpConv       = rFont.mpConv;
  mRenderMode  = rFont.mRenderMode;

  Init();
  Load(rFont.mpFace->Desc.face_id);
}

nglFontBase::~nglFontBase()
{
  delete mpFace;
}


/*
 * Goblal font info & metrics
 */

bool nglFontBase::GetInfo (nglFontInfo& rInfo, nglFontUnit Unit) const
{
  if (!mpFace->Face)
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
  rInfo.FaceCount  = mpFace->Face->num_faces;
  
  /* The following fields should contain valuable information only for scalable fonts.
   * We copy them anyway.
   */
  float ratio = 0.f;
  switch (Unit)
  {
    case eFontUnitEM    : ratio = NUI_INV_SCALE_FACTOR * 1.0f; break;
    case eFontUnitPoint : ratio = EMToPoint(1.0f); break;
    case eFontUnitPixel : ratio = EMToPixel(1.0f); break;
  }
  rInfo.BBoxMinX       = mpFace->Face->bbox.xMin * ratio;
  rInfo.BBoxMinY       = mpFace->Face->bbox.yMin * ratio;
  rInfo.BBoxMaxX       = mpFace->Face->bbox.xMax * ratio;
  rInfo.BBoxMaxY       = mpFace->Face->bbox.yMax * ratio;
  rInfo.Ascender       = mpFace->Face->ascender * ratio;
  rInfo.Descender      = mpFace->Face->descender * ratio;
  rInfo.Height         = mpFace->Face->height * ratio;
  rInfo.AdvanceMaxW    = mpFace->Face->max_advance_width * ratio;
  rInfo.AdvanceMaxH    = mpFace->Face->max_advance_height * ratio;
  rInfo.UnderlinePos   = mpFace->Face->underline_position * ratio;
  rInfo.UnderlineThick = mpFace->Face->underline_thickness * ratio;

  return true;
}

nglString nglFontBase::GetFamilyName() const
{
  return mFamilyName;
}

nglString nglFontBase::GetStyleName() const
{
  return mStyleName;
}

bool nglFontBase::IsScalable() const
{
  return mpFace->Face ? FT_IS_SCALABLE(mpFace->Face) : false;
}

bool nglFontBase::IsBold() const
{
  return mpFace->Face ? ((mpFace->Face->style_flags & FT_STYLE_FLAG_BOLD) != 0) : false;
}

bool nglFontBase::IsMonospace() const
{
  return mpFace->Face ? ((mpFace->Face->face_flags & FT_FACE_FLAG_FIXED_WIDTH) != 0) : false;
}

bool nglFontBase::IsItalic() const
{
  return mpFace->Face ? ((mpFace->Face->style_flags & FT_STYLE_FLAG_ITALIC) != 0) : false;
}

uint nglFontBase::GetFaceCount() const
{
  return mpFace->Face ? mpFace->Face->num_faces : 0;
}

float nglFontBase::GetHeight (nglFontUnit Unit, float DefaultSpacing) const
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

float nglFontBase::GetAscender(nglFontUnit Unit) const
{
  float ratio = 0.f;
  switch (Unit)
  {
    case eFontUnitEM    : ratio = NUI_INV_SCALE_FACTOR * 1.0f; break;
    case eFontUnitPoint : ratio = EMToPoint(1.0f); break;
    case eFontUnitPixel : ratio = EMToPixel(1.0f); break;
  }
  return mpFace->Face->ascender * ratio;
}

float nglFontBase::GetDescender(nglFontUnit Unit) const
{
  float ratio = 0.f;
  switch (Unit)
  {
    case eFontUnitEM    : ratio = NUI_INV_SCALE_FACTOR * 1.0f; break;
    case eFontUnitPoint : ratio = EMToPoint(1.0f); break;
    case eFontUnitPixel : ratio = EMToPixel(1.0f); break;
  }
  return mpFace->Face->ascender * ratio;
}

uint nglFontBase::GetGlyphCount() const
{
  return mpFace->Face ? (uint)mpFace->Face->num_glyphs : 0;
}

void nglFontBase::GetGlyphs(std::set<nglChar>& rGlyphs) const
{
  rGlyphs.clear();

  FT_ULong  charcode = 0;
  FT_UInt   gindex = 0;
    
  charcode = FT_Get_First_Char( mpFace->Face, &gindex );
  while ( gindex != 0 )
  {
    //rGlyphs.insert(charcode);
    charcode = FT_Get_Next_Char( mpFace->Face, charcode, &gindex );     
  }
}

/*
 * Conversion methods
 */

float nglFontBase::EMToPixel (float Value) const
{
  return  (Value * mSize) / mUnitsPerEM;
}

float nglFontBase::EMToPoint (float Value) const
{
  return (Value * mSize * 72.0f / mResolution) / mUnitsPerEM;
}

float nglFontBase::PixelToPoint (float Value) const
{
  return Value * 72.0f / mResolution;
}

float nglFontBase::PointToPixel (float Value) const
{
  return Value * mResolution / 72.0f;
}


/*
 * Glyph metrics
 */

float nglFontBase::GetSize (nglFontUnit Unit) const
{
  switch (Unit)
  {
    case eFontUnitEM    : return 0.0f; // Does not really makes sense
    case eFontUnitPoint : return PixelToPoint(mSize);
    case eFontUnitPixel : return mSize;
  }

  return mSize;
}

bool nglFontBase::SetSize (float Size, nglFontUnit Unit)
{
  if (!mpFace->Face)
    return false;

  FTC_ScalerRec ftscaler;
  FT_Size       ftsize;
  FT_UShort     pixels = 0;

  // Fetch font ID
  ftscaler.face_id  = mpFace->Desc.face_id;
  
  // Fetch font size in pixel units
  ftscaler.pixel = 1; // TRUE
  switch (Unit)
  {
    case eFontUnitEM    : pixels = (FT_UInt)roundf (EMToPixel(Size)); break;
    case eFontUnitPoint : pixels = (FT_UInt)roundf (PointToPixel(Size)); break;
    case eFontUnitPixel : pixels = (FT_UInt)roundf (Size); break;
  }

  pixels *= NUI_SCALE_FACTOR;

  NGL_ASSERT(pixels > 0);
  
  if (IsScalable())
  {
    ftscaler.width  = pixels;
    ftscaler.height = pixels;
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
        ftscaler.width  = size.width;
        ftscaler.height = size.height;
        break;
      }
    }
    if (i >= mpFace->Face->num_fixed_sizes)
      return false;
  }

  if (FTC_Manager_LookupSize (gFTCacheManager, &ftscaler, &ftsize))
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Couldn't change font size (to %dpx)"), pixels); )
    return false;
  }

  mpFace->Face = ftsize->face;
  mpFace->Desc.face_id = ftscaler.face_id;
  mpFace->Desc.width   = pixels;
  mpFace->Desc.height  = pixels;
  mSize = pixels * NUI_INV_SCALE_FACTOR;

  return true;
}

float nglFontBase::GetResolution() const
{
  return mResolution;
}

void nglFontBase::SetResolution (float Resolution)
{
  mResolution = Resolution;
}


bool nglFontBase::GetMetrics (nglGlyphInfo& rInfo, nglChar Char, GlyphType Type) const
{
  uint index;

  return
    GetGlyphIndexes(&Char, 1, &index, 1) == 1 &&
    GetGlyphInfo(rInfo, index, Type);
}

bool nglFontBase::GetGlyphInfo (nglGlyphInfo& rInfo, uint Index, GlyphType Type) const
{
  FT_Glyph glyph;

  if (!(glyph = (FT_Glyph)GetGlyph(Index, Type)))
    return false;

  switch (glyph->format)
  {
    case FT_GLYPH_FORMAT_BITMAP:
    {
      FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyph;
      rInfo.Width    = NUI_INV_SCALE_FACTOR * (float)bitmap->bitmap.width;
      rInfo.Height   = NUI_INV_SCALE_FACTOR * (float)bitmap->bitmap.rows;
      rInfo.BearingX = NUI_INV_SCALE_FACTOR * (float)bitmap->left;
      rInfo.BearingY = NUI_INV_SCALE_FACTOR * (float)bitmap->top;
    }
    break;

    case FT_GLYPH_FORMAT_OUTLINE:
    {
      FT_BBox bbox;

      FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_pixels, &bbox);
      rInfo.Width    = NUI_INV_SCALE_FACTOR * (float)(bbox.xMax - bbox.xMin);
      rInfo.Height   = NUI_INV_SCALE_FACTOR * (float)(bbox.yMax - bbox.yMin);
      rInfo.BearingX = NUI_INV_SCALE_FACTOR * (float)bbox.xMin;
      rInfo.BearingY = NUI_INV_SCALE_FACTOR * (float)bbox.yMax;
    }
    break;

    default:
      return false;
  }

  rInfo.Index = Index;
  rInfo.AdvanceX = NUI_INV_SCALE_FACTOR * glyph->advance.x / 65536.0f;
  rInfo.AdvanceY = NUI_INV_SCALE_FACTOR * glyph->advance.y / 65536.0f;

  return true;
}

bool nglFontBase::GetKerning (uint Left, uint Right, float& rX, float& rY) const
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

  rX *= NUI_INV_SCALE_FACTOR;
  rY *= NUI_INV_SCALE_FACTOR;

  return true;
}


/*
 * Character maps (encoding)
 */

int nglFontBase::GetCharMapCount() const
{
  return mpFace->Face ? mpFace->Face->num_charmaps : 0;
}

int nglFontBase::GetCharMap() const
{
  return mCharMap;
}

bool nglFontBase::SetCharMap (int Index)
{
  if (!mpFace->Face || (Index < -1) || (Index >= mpFace->Face->num_charmaps))
    return false;

  if (FT_Set_Charmap (mpFace->Face, mpFace->Face->charmaps[Index]) != 0)
    return false;

  mCharMap = Index;
  return true;
}

const nglChar* nglFontBase::GetCharMapName ()  const
{
  return mpFace->Face ? ::nglGetCharMapName(mpFace->Face->charmap) : NULL;
}

const nglChar* nglFontBase::GetCharMapName (int Index)  const
{
  if (!mpFace->Face || (Index < 0) || (Index >= mpFace->Face->num_charmaps))
    return NULL;

  return ::nglGetCharMapName(mpFace->Face->charmaps[Index]);
}


//#define DBG_INDEX

int nglFontBase::GetGlyphIndexes (const nglChar* pSource, int SourceLength, uint* pIndexes, int IndexesLength) const
{
  if (!mpFace->Face)
    return -1;

  /*
   * The internal LoadFinish() method automatically tries to find a Unicode charmap,
   * and decides if a charset conversion (Unicode to locale or locale to Unicode) must be
   * performed (using mpConv). We must also handle the case where the font has no charmap.
   */

  int todo = MIN(SourceLength, IndexesLength);

  /* No selected or available charmap, or charmap conversion turned off : do a dumb copy
   */
  if (!mpFace->Face->charmap ||
      (mpFace->Face->charmap->encoding == ft_encoding_none))
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
      NGL_OUT(_T(" %d -> %d"), pSource[-1], pIndexes[-1]);
#endif
      todo--;
    }
  }

  return done;
}


int32 nglFontBase::GetGlyphIndex(nglChar Source) const
{
  if (!mpFace->Face)
    return -1;
  
  /* No selected or available charmap, or charmap conversion turned off : do a dumb copy
   */
  if (!mpFace->Face->charmap ||
      (mpFace->Face->charmap->encoding == ft_encoding_none))
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

nglFontBase::RenderMode nglFontBase::GetRenderMode() const
{
  return mRenderMode;
}

bool nglFontBase::SetRenderMode (nglFontBase::RenderMode Mode)
{
  if (!mpFace->Face || !IsScalable())
    return false;

  FT_Int32 flags = FT_LOAD_DEFAULT;

  if (!(Mode & AntiAliasing))
    flags |= FT_LOAD_TARGET_MONO;

  if (!(Mode & Hinting))
    flags |= FT_LOAD_NO_HINTING;

  mpFace->Desc.flags = flags;
  mRenderMode = Mode;

  return true;
}


/*
 * Protected interface
 */

nglFontBase::GlyphHandle nglFontBase::GetGlyph (uint Index, GlyphType Type) const
{
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

nglFontBase::GlyphType nglFontBase::GetGlyphType (GlyphHandle Glyph) const
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

bool nglFontBase::GetGlyphBitmap (GlyphHandle Glyph, GlyphBitmap& rBitmap) const
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
const uint32 nglFontBase::mBitLUT[16] =
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

bool nglFontBase::GetBitmap8 (const GlyphBitmap& rSrc, GlyphBitmap& rDst) const
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


/*
 * Internals
 */

void nglFontBase::Defaults()
{
  /* The face object (is managed by the cache)
   */
  mpFace = new FaceHandle();

  /* A nglFont is described by mpFace->Desc, this is used to query the
   * face or glyph objects through the cache manager. Face IDs
   * are managed via nglFontInstance.
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
}

#define NGL_FTCACHE_MAX_FACES 5000
#define NGL_FTCACHE_MAX_SIZES 400
#define NGL_FTCACHE_MAX_BYTES (1024*1024*100)

bool nglFontBase::Init()
{
  FT_Error error;

  // Build a FT library instance if necessary
  if (!gFTLibrary)
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Initializing FreeType library")); )
    error = FT_Init_FreeType (&gFTLibrary);
    if (error)
    {
      SetError (_T("font"), NGL_FONT_EINIT);
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
    error = FTC_Manager_New (gFTLibrary, NGL_FTCACHE_MAX_FACES, NGL_FTCACHE_MAX_SIZES, NGL_FTCACHE_MAX_BYTES, nglFontInstance::FaceRequestHandler, NULL, &gFTCacheManager);
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

bool nglFontBase::Load (const nglPath& rPath, uint Face)
{
  // Load face from file spec
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Loading logical font '%ls' (face %d)"), rPath.GetNodeName().GetChars(), Face); )
  mpFace->SetFontInstance(new nglFontInstance(rPath, Face));
  mpFace->Desc.face_id = nglFontInstance::Install(mpFace->GetFontInstance());

  return LoadFinish();
}

bool nglFontBase::Load (const uint8* pBase, int32 Size, uint Face, bool StaticBuffer)
{
  // Load face from memory
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Loading logical font at %p (face %d, %d bytes)"), pBase, Face, Size); )
  mpFace->SetFontInstance(new nglFontInstance(pBase, Size, Face, StaticBuffer));
  mpFace->Desc.face_id = nglFontInstance::Install(mpFace->GetFontInstance());
  
  return LoadFinish();
}

bool nglFontBase::Load (FaceID ID)
{
  // Load face from FreeType cache ID
#ifdef _DEBUG_
  nglFontInstance* ifont = nglFontInstance::Lookup((FTC_FaceID)ID);
  if (ifont)
    NGL_LOG(_T("font"), NGL_LOG_INFO, _T("Copying logical font '%ls' (face %d)"), ifont->GetPath().GetNodeName().GetChars(), ifont->GetFace());
#endif

  mpFace->Desc.face_id = (FTC_FaceID)ID;

  return LoadFinish();
}

bool nglFontBase::LoadFinish()
{
  /* Fetch generic (maybe unsized) face
   */
  if (FTC_Manager_LookupFace (gFTCacheManager, mpFace->Desc.face_id, &mpFace->Face) != FT_Err_Ok)
  {
    SetError(_T("font"), NGL_FONT_ELOAD);
    return false;
  }

  /* Initialize some global info fields
   */
  mFamilyName.Import (mpFace->Face->family_name);
  if (mFamilyName == _T("LastResort"))
    mLastResort = true;
  mStyleName.Import (mpFace->Face->style_name);
  mUnitsPerEM = (float)mpFace->Face->units_per_EM;
  mGlobalHeight = (float)mpFace->Face->height; // Only valid for scalable fonts (see GetHeight())

  // Get Panose information from the TT OS/2 tables
  TT_OS2* pOS2 = (TT_OS2*)FT_Get_Sfnt_Table(mpFace->Face, ft_sfnt_os2);
  if (pOS2)
  {
    memcpy(&mPanoseBytes, pOS2->panose, 10);
    mHasPanoseInfo = true;
  }
  else
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Warning: font '%ls (%ls)' has no panose information.\n"), mFamilyName.GetChars(), mStyleName.GetChars()); )
    memset(&mPanoseBytes, 0, 10);
    mHasPanoseInfo = false;
  }
  
  /* Select a default size
   */
  if (IsScalable())
  {
    if (!SetSize(DefaultPixelSize, eFontUnitPixel))
      return false;
    SetRenderMode(AntiAliasing | Hinting);
  }
  else
  {
    if (mpFace->Face->num_fixed_sizes > 0)
    {
      // Take first available size
      FT_Bitmap_Size* size = mpFace->Face->available_sizes;

      if (!SetSize ((float)size->height, eFontUnitPixel))
        return false;
    }
#ifdef _DEBUG_
    else
    {
      NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("Oddity: fixed font with no available sizes"));
    }
#endif // _DEBUG_
    // No SetRenderMode() since this is a bitmap font
  }

  /* Select a default charmap
   *
   * (see GetGlyphindexes for more info)
   */
  if (FT_Select_Charmap (mpFace->Face, ft_encoding_unicode) != FT_Err_Ok)
  {
#ifdef USE_WCHAR
    // We can't select Unicode output, we'll do nglChar -> locale and use the first map (if available)
    mpConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, eEncodingNative));
#else
    // We expect that the first charmap is actually the right one for the locale's encoding
#endif
    FT_Set_Charmap (mpFace->Face, mpFace->Face->charmaps[0]);
  }
#ifndef USE_WCHAR
  else
    // We have a Unicode charmap, we'll do locale -> UCS-2 conversion
    mpConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, eUCS2));
#endif
  mCharMap = FT_Get_Charmap_Index(mpFace->Face->charmap);

  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("  selected charmap   : %ls (#%d)"), GetCharMapName(), GetCharMap()); )
  NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_DEBUG, _T("  charmap conversion : %ls"), YESNO(mpConv)); )

  return true;
}

const nglChar* nglGetCharMapName (FT_CharMap CharMap)
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

nglTextEncoding nglGetCharMapEncoding (FT_CharMap CharMap)
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

void nglFontBase::GetEncodings(std::set<nglTextEncoding>& rEncodings)
{
  rEncodings.clear();
  
  int32 count = GetCharMapCount();
  
  for (int32 i = 0; i < count; i++)
  {
    FT_CharMap charmap = mpFace->Face->charmaps[i];
    rEncodings.insert(nglGetCharMapEncoding(charmap));
  }
}

void nglFontBase::GetSizes(std::set<int32>& rSizes)
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

void nglFontBase::OnExit()
{
  nglFontInstance::OnExit();

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

const nglChar* nglFontBase::OnError (uint& rError) const
{
  return FetchError(gpFontBaseErrorTable, NULL, rError);
}

const nuiFontPanoseBytes& nglFontBase::GetPanoseBytes() const
{
  return mPanoseBytes;
}

bool nglFontBase::HasPanoseInfo() const
{
  return mHasPanoseInfo;
}

bool nglFontBase::IsLastResort() const
{
  return mLastResort;
}

#endif // HAVE_FREETYPE
