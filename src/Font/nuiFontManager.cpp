/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiFontManager.h"
#include "nglPath.h"
#include "nglIStream.h"
#include "nuiVBox.h"

#ifdef _UIKIT_
#ifdef __IPHONE_3_2
#include <CoreText/CoreText.h>
#endif
#include <CoreFoundation/CoreFoundation.h>
#include <sys/xattr.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_TRUETYPE_TABLES_H

//#define FONT_TEST_HACK

#ifndef FONT_TEST_HACK
  #ifdef _WIN32_
  #define _WIN32_FONTS_
  #endif

  #ifdef _CARBON_
  #define _CARBON_FONTS_
  #endif

  #ifdef _COCOA_
  #define _CARBON_FONTS_
  #endif

#else
  #define _WIN32_FONTS_
#endif

static FT_Library gFTLibrary = NULL;  // Global FT library instance
nglTextEncoding nglGetCharMapEncoding (FT_CharMap CharMap);

std::multimap<nglString, nglString> nuiFontRequest::gFontsForGenericNames;
std::map<nglString, nglString> nuiFontRequest::gGenericNamesForFonts;
std::map<nglString, nglString> nuiFontRequest::gDefaultFontsForGenericNames;

void nuiFontRequest::AddGenericNameForFont(const nglString& rGenericName, const nglString& rFamilyName)
{
  nglString genericname(rGenericName);
  genericname.ToLower();
  nglString familyname(rFamilyName);
  familyname.ToLower();

  nuiFontRequest::gFontsForGenericNames.insert(std::pair<nglString, nglString>(genericname, familyname));
  nuiFontRequest::gGenericNamesForFonts[familyname] = genericname;
}

nglString nuiFontRequest::GetGenericNameForFont(const nglString& rName)
{
  nglString name(rName);
  name.ToLower();
  if (nuiFontRequest::gGenericNamesForFonts.find(name) != nuiFontRequest::gGenericNamesForFonts.end())
  {
    return nuiFontRequest::gGenericNamesForFonts[name];
  }
  return nglString::Null;
}

void nuiFontRequest::GetFontsForGenericName(const nglString& rName, std::vector<nglString>& rFonts)
{
  nglString name(rName);
  name.ToLower();
  rFonts.clear();
  std::pair<std::multimap<nglString, nglString>::iterator, std::multimap<nglString, nglString>::iterator> its = nuiFontRequest::gFontsForGenericNames.equal_range(name);
  std::multimap<nglString, nglString>::iterator end = its.second;
  for (std::multimap<nglString, nglString>::iterator it = its.first; it != end; ++it)
  {
    rFonts.push_back(it->second);
  }
}

///! Font Request class:
nuiFontRequest::nuiFontRequest(nuiFontBase* pOriginalFont, bool ForcePanoseOnlyFonts)
:
  mName(nglString::Null),
  mGenericName(nglString::Null),
  mStyle(nglString::Null),
  mFace(0),
  mItalic(false),
  mBold(false),
  mMonospace(false),
  mScalable(true),
  mMustHaveGlyphs(std::set<nglUChar>()),
  mMustHaveEncoding(std::set<nglTextEncoding>()),
  mMustHaveSizes(std::set<int32>()),
  mPanose(nuiPanose())
{
  if (SetObjectClass(_T("nuiFontRequest")))
    InitAttributes();
  
  if (nuiFontRequest::gGenericNamesForFonts.empty())
  {
    // Standard generic font family names from CSS specification
    // http://www.w3.org/TR/2002/WD-css3-fonts-20020802/#generic-font-families
    
    // latin serif
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Times"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Times New Roman"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Bodoni"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Garamond"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Minion Web"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("ITC Stone Serif"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Georgia"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Bitstream Cyberbit"));
    // cyrillic serif
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Adobe Minion Cyrillic"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Excelcior Cyrillic Upright"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Monotype Albion 70"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("ER Bukinst"));
    // hebrew serif
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("New Peninim"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Raanana"));
    // japanese serif
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Ryumin Light-KL"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Kyokasho ICA"));
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Futo Min A101"));
    // cherokee serif
    nuiFontRequest::AddGenericNameForFont(_T("serif"), _T("Lo Cicero Cherokee"));
    
    // latin sans-serif
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Trebuchet"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("ITC Avant Garde Gothic"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Arial"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Verdana"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Univers"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Futura"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("ITC Stone Sans"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Gill Sans"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Akzidenz Grotesk"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Helvetica"));
    // greek sans-serif
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Attika"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Typiko New Era"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Tahoma"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Monotype Gill Sans 571"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Helvetica Greek"));
    // cyrillic sans-serif
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Helvetica Cyrillic"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("ER Univers"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Lucida Sans Unicode"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Bastion"));
    // hebrew sans-serif
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Arial Hebrew"));
    // japanese sans-serif
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Shin Go"));
    nuiFontRequest::AddGenericNameForFont(_T("sans-serif"), _T("Heisei Kaku Gothic W5"));
    
    // latin cursive
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Caflisch Script"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Comic Sans MS"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Adobe Poetica"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Sanvito"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Ex Ponto"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Snell Roundhand"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Apple Chancery"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Zapf-Chancery"));
    // cyrillic cursive
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("ER Architekt"));
    // hebrew cursive
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Corsiva"));
    // arabic cursive
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("DecoType Naskh"));
    nuiFontRequest::AddGenericNameForFont(_T("cursive"), _T("Monotype Urdu 507"));
    
    // latin fantasy
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("Alpha Geometrique"));
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("Critter"));
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("Cottonwood"));
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("FB Reactor"));
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("Studz"));
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("Nina"));
    nuiFontRequest::AddGenericNameForFont(_T("fantasy"), _T("Papyrus"));
    
    // latin monospace
    nuiFontRequest::AddGenericNameForFont(_T("monospace"), _T("Courier"));
    nuiFontRequest::AddGenericNameForFont(_T("monospace"), _T("Courier New"));
    nuiFontRequest::AddGenericNameForFont(_T("monospace"), _T("Prestige"));
    nuiFontRequest::AddGenericNameForFont(_T("monospace"), _T("Everson Mono"));
    // cyrillic monospace
    nuiFontRequest::AddGenericNameForFont(_T("monospace"), _T("ER Kurier"));
    // japanese monospace
    nuiFontRequest::AddGenericNameForFont(_T("monospace"), _T("Osaka Monospaced"));
    
    // add default fonts for each system
#ifdef _WIN32_FONTS_
    nuiFontRequest::gDefaultFontsForGenericNames[_T("serif")] = _T("Times New Roman");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("sans-serif")] = _T("Arial");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("cursive")] = _T("Comic Sans MS");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("fantasy")] = _T("Nina");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("monospace")] = _T("Courier New");
#endif
#ifdef _CARBON_FONTS_
    nuiFontRequest::gDefaultFontsForGenericNames[_T("serif")] = _T("Times");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("sans-serif")] = _T("Helvetica");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("cursive")] = _T("Apple Chancery");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("fantasy")] = _T("Papyrus");
    nuiFontRequest::gDefaultFontsForGenericNames[_T("monospace")] = _T("Courier");
#endif
  }
  
  /*mItalic.mScore = 0.05f;
  mBold.mScore = 0.05f;
  mMonospace.mScore = 0.05f;*/

  mFace.mElement = 0;

  if (pOriginalFont)
  {
    if (pOriginalFont->HasPanoseInfo())
    {
      MustBeSimilar(pOriginalFont->GetPanoseBytes(), 20.0f);
      if (mItalic.mScore > 0.0)
        mPanose.mElement.SetItalic(pOriginalFont->IsItalic());
      if (mBold.mScore > 0.0)
        mPanose.mElement.SetBold(pOriginalFont->IsBold());
    }
      
    if (!ForcePanoseOnlyFonts || !pOriginalFont->HasPanoseInfo())
    {
      SetName(pOriginalFont->GetFamilyName(), 3.0f);
      SetStyle(pOriginalFont->GetStyleName(), 1.5f);
      SetItalic(pOriginalFont->IsItalic(), 1.0f);
      SetBold(pOriginalFont->IsBold(), 1.0f);
      if (pOriginalFont->IsScalable())
        SetScalable(10.f, true);
      if (pOriginalFont->IsMonospace())
        SetMonospace(1.0f);
      MustHaveSize(pOriginalFont->GetSize(), 1.0f);
    }
  }
  
}

nuiFontRequest::nuiFontRequest(const nuiFontRequest& rOriginal)
:
  mName(nglString::Null),
  mGenericName(nglString::Null),
  mStyle(nglString::Null),
  mFace(0),
  mItalic(false),
  mBold(false),
  mMonospace(false),
  mScalable(true),
  mMustHaveGlyphs(std::set<nglUChar>()),
  mMustHaveEncoding(std::set<nglTextEncoding>()),
  mMustHaveSizes(std::set<int32>()),
  mPanose(nuiPanose())
{
#define COPY(X) X = rOriginal.X;
  COPY(mName);
  COPY(mGenericName);
  COPY(mStyle);
  COPY(mFace);
  COPY(mItalic);
  COPY(mBold);
  COPY(mMonospace);
  COPY(mScalable);
  COPY(mMustHaveGlyphs);
  COPY(mMustHaveEncoding);
  COPY(mMustHaveSizes);
  COPY(mPanose);
#undef COPY  
}


void nuiFontRequest::InitAttributes()
{
  // Name
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Name")), nuiUnitName,
                nuiFastDelegate0<const nglString&>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetName)));
  
  // GenericName
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("GenericName")), nuiUnitName,
                nuiFastDelegate0<const nglString&>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetGenericName)));
    
  // Style
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Style")), nuiUnitName,
                nuiFastDelegate0<const nglString&>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetStyle)));
  
  // Face
  AddAttribute(new nuiAttribute<int32>
               (nglString(_T("Face")), nuiUnitNone,
                nuiFastDelegate0<int32>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetFace)));
  
  // Size
  AddAttribute(new nuiAttribute<int32>
               (nglString(_T("Size")), nuiUnitPixels,
                nuiFastDelegate0<int32>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetSize)));
  
  // Italic
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Italic")), nuiUnitName,
                nuiFastDelegate0<bool>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetItalic)));
  
  // Bold
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Bold")), nuiUnitName,
                nuiFastDelegate0<bool>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetBold)));
  
  // Proportionnal
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Proportionnal")), nuiUnitName,
                nuiFastDelegate0<bool>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetProportionnal)));
  
  // Monospace
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Monospace")), nuiUnitName,
                nuiFastDelegate0<bool>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetMonospace)));
  
  // Scalable
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("Scalable")), nuiUnitName,
                nuiFastDelegate0<bool>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetScalable)));
  
  // Panose
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Panose")), nuiUnitName,
                nuiFastDelegate0<const nglString&>(),
                nuiMakeDelegate(this, &nuiFontRequest::_SetPanose)));
}

nuiFontRequest::~nuiFontRequest()
{
  
}

void nuiFontRequest::SetName(const nglString& rName, float Score, bool Strict)
{
  mOriginalName = rName;
  mName.mElement = rName;
  mName.mElement.ToLower();
  mName.mScore = Score;  
  mName.mStrict = Strict;  
}

void nuiFontRequest::SetGenericName(const nglString& rName, float Score, bool Strict)
{
  mGenericName.mElement = rName;
  mGenericName.mElement.ToLower();
  mGenericName.mScore = Score;  
  mGenericName.mStrict = Strict;  
}

void nuiFontRequest::SetStyle(const nglString& rStyle, float Score, bool Strict)
{
  mStyle.mElement = rStyle;
  mStyle.mElement.ToLower();
  mStyle.mScore = Score;  
  mStyle.mStrict = Strict;  
}

void nuiFontRequest::SetFace(int32 Face, float Score, bool Strict)
{
  mFace.mElement = Face;
  mFace.mScore = Score;
  mFace.mStrict = Strict;  
}

void nuiFontRequest::SetItalic(bool Italic, float Score, bool Strict)
{
  mItalic.mElement = Italic;
  mItalic.mScore = Score;
  mItalic.mStrict = Strict;  
}

void nuiFontRequest::SetBold(bool Bold, float Score, bool Strict)
{
  mBold.mElement = Bold;
  mBold.mScore = Score;  
  mBold.mStrict = Strict;  
}

void nuiFontRequest::MustHaveGlyphs(const std::set<nglUChar>& rGlyphs, float Score, bool Strict)
{
  mMustHaveGlyphs.mElement = rGlyphs;
  mMustHaveGlyphs.mScore = Score;
  mMustHaveGlyphs.mStrict = Strict;  
}

void nuiFontRequest::MustHaveGlyph(nglUChar Glyph, float Score, bool Strict)
{
  mMustHaveGlyphs.mElement.insert(Glyph);
  mMustHaveGlyphs.mScore = Score;
  mMustHaveGlyphs.mStrict = Strict;  
}

void nuiFontRequest::SetProportionnal(float Score, bool Strict)
{
  mMonospace.mElement = false;
  mMonospace.mScore = Score;
  mMonospace.mStrict = Strict;  
}

void nuiFontRequest::SetMonospace(float Score, bool Strict)
{
  mMonospace.mElement = true;
  mMonospace.mScore = Score;
  mMonospace.mStrict = Strict;  
}

void nuiFontRequest::SetScalable(float Score, bool Strict)
{
  mScalable.mElement = true;
  mScalable.mScore = Score;
  mScalable.mStrict = Strict;  
}

void nuiFontRequest::MustHaveEncoding(nglTextEncoding Encoding, float Score, bool Strict)
{
  mMustHaveEncoding.mElement.insert(Encoding);
  mMustHaveEncoding.mScore = Score;
  mMustHaveEncoding.mStrict = Strict;  
}

void nuiFontRequest::MustHaveSize(int32 size, float Score, bool Strict)
{
  mMustHaveSizes.mElement.insert(size);
  mMustHaveSizes.mScore = Score;
  mMustHaveSizes.mStrict = Strict;  
}

void nuiFontRequest::MustBeSimilar(const nuiPanose& rPanose, float Score, bool Strict)
{
  mPanose.mElement = rPanose;
  mPanose.mScore = Score;
  mPanose.mStrict = Strict;
}

void nuiFontRequest::_SetName(const nglString& rName)
{
  SetName(rName, 10.0);
}

void nuiFontRequest::_SetGenericName(const nglString& rName)
{
  SetGenericName(rName, 10.0);
}

void nuiFontRequest::_SetStyle(const nglString& rStyle)
{
  SetStyle(rStyle, 1.0);
}

void nuiFontRequest::_SetFace(int32 Face)
{
  SetFace(Face, 1.0);
}

void nuiFontRequest::_SetItalic(bool Italic)
{
  SetItalic(Italic, 1.0);
}

void nuiFontRequest::_SetBold(bool Bold)
{
  SetBold(Bold, 1.0);
}

void nuiFontRequest::_SetProportionnal(bool set)
{
  if (set)
    SetProportionnal(3.0f);
  else
    SetMonospace(3.0f);
}

void nuiFontRequest::_SetMonospace(bool set)
{
  if (set)
    SetMonospace(3.0f);
  else
    SetProportionnal(3.0f);
}

void nuiFontRequest::_SetScalable(bool set)
{
  SetScalable(1.0f);
}

void nuiFontRequest::_SetSize(int32 size)
{
  MustHaveSize(size, 1.0f);
}

void nuiFontRequest::ClearMustHaveGlyphs()
{
  mMustHaveGlyphs.Clear();
  mMustHaveGlyphs.mElement.clear();
}

void nuiFontRequest::ClearMustHaveEncoding()
{
  mMustHaveEncoding.Clear();
  mMustHaveEncoding.mElement.clear();
}

void nuiFontRequest::ClearMustHaveSize()
{
  mMustHaveSizes.Clear();
  mMustHaveSizes.mElement.clear();
}

void nuiFontRequest::ClearMustBeSimilar()
{
  mPanose.Clear();
}

void nuiFontRequest::_SetPanose(const nglString& rPanose)
{
  std::vector<nglString> tokens;
  rPanose.Tokenize(tokens);
  if (tokens.size() != 10)
  {
    NGL_LOG("font", NGL_LOG_ERROR, "Panose information MUST be 10 numbers from 0 to 255");
    return;
  }
  
  uint8 PanoseBytes[10];
  for (uint32 i = 0; i < 10; i++)
  {
    if (!tokens[i].IsInt())
    {
      NGL_LOG("font", NGL_LOG_ERROR, "Each of the 10 Panose values must be a number from 0 to 255");
      return;
    }
    PanoseBytes[i] = tokens[i].GetCInt();
  }
  nuiPanose panose(PanoseBytes);
  MustBeSimilar(panose, 1.0f);
}



///! FontRequestResult:
nuiFontRequestResult::nuiFontRequestResult(const nglPath& rPath, int32 Face, float Score, nuiFontDesc* pFontDesc)
{
  mPath = rPath;
  mFace = Face;
  mScore = Score;
  mpFontDesc = pFontDesc;
}

nuiFontRequestResult::~nuiFontRequestResult()
{
}

const nglPath& nuiFontRequestResult::GetPath() const
{
  return mPath;
}

int32 nuiFontRequestResult::GetFace() const
{
  return mFace;
}

float nuiFontRequestResult::GetScore() const
{
  return mScore;
}

const nuiFontDesc* nuiFontRequestResult::GetFontDesc() const
{
  return mpFontDesc;
}



///! Font Manager class:
nuiFontManager::nuiFontManager()
{
  //std::map<nglString, nglPath> mFontFolders;
}

nuiFontManager::~nuiFontManager()
{
  Clear();
}

void nuiFontManager::AddFolder(const nglString& rId, const nglPath& rPath)
{
  mFontFolders[rId] = rPath;
}

void nuiFontManager::AddSystemFolders()
{
  GetSystemFolders(mFontFolders);
}

void nuiFontManager::GetSystemFolders(std::map<nglString, nglPath>& rFolders)
{
#ifdef FONT_TEST_HACK
  rFolders[_T("System0")] = _T("/System/Library/Fonts/Cache/");
#elif (defined _CARBON_) || (defined _COCOA_)
  rFolders[_T("System0")] = _T("/System/Library/Fonts/");
  rFolders[_T("System1")] = _T("/Library/Fonts/");
  rFolders[_T("System2")] = _T("~/Library/Fonts/");
#elif TARGET_IPHONE_SIMULATOR
  rFolders[_T("System0")] = _T("/System/Library/Fonts/");
  //rFolders[_T("System1")] = _T("/Library/Fonts/");
#elif (defined _UIKIT_)
  rFolders[_T("System0")] = _T("/System/Library/Fonts/");
  rFolders[_T("System1")] = _T("/System/Library/Fonts/Cache/");
#elif (defined _WIN32_)
  nglChar p[MAX_PATH];
  HRESULT hr = SHGetFolderPath(NULL, CSIDL_FONTS, NULL, 0, p);
  if (hr == S_OK)
  {
    rFolders[_T("System0")] = p;
    
    //NGL_DEBUG( NGL_LOG("font", NGL_LOG_INFO, "Adding System0 font folder: '%s'\n", p); )
  }
  else
  {
    rFolders[_T("System0")] = _T("/c:/windows/fonts/");
  }
#elif (defined _LINUX_)
  int count = 0;
  Display* pDisplay = XOpenDisplay(NULL);
  char** pPathes = XGetFontPath(pDisplay, &count);
  for (uint i = 0; i < count; i++)
  {
    nglString str;
    str.CFormat(_T("System%d"), i);
    rFolders[str] = nglString(pPathes[i]);
  }
  XFreeFontPath(pPathes);
  XCloseDisplay(pDisplay);
#endif
}

void nuiFontManager::DelFolder(const nglString& rId)
{
  std::map<nglString, nglPath>::iterator it = mFontFolders.find(rId);
  if (it != mFontFolders.end())
    mFontFolders.erase(it);
}

void nuiFontManager::DelFolder(const nglPath& rPath)
{
  std::map<nglString, nglPath>::iterator it = mFontFolders.begin();
  std::map<nglString, nglPath>::iterator end = mFontFolders.end();
  while (it != mFontFolders.end())
  {
    if (it->second == rPath)
      mFontFolders.erase(it++);
    else
      ++it;
  }
}

const nglPath& nuiFontManager::GetFolder(const nglString& rId) const
{
  std::map<nglString, nglPath>::const_iterator it = mFontFolders.find(rId);
  NGL_ASSERT(it != mFontFolders.end());
  return it->second;
}

static nuiMainWindow* gpWin = NULL;
static nuiLabel* gpFontPathLabel = NULL;

void nuiFontManager::ScanFolders(bool rescanAllFolders /* = false */)
{
  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  nglWindowInfo Info;
  
  Info.Flags = nglWindow::NoBorder | nglWindow::NoResize;
  Info.Width = 320;
  Info.Height = 60;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = _T("nui test");
  Info.XPos = 0;
  Info.YPos = 0;

#if 0
#ifndef _UIKIT_
  gpWin = new nuiMainWindow(ContextInfo, Info);
  nuiVBox* pBox = new nuiVBox();
  pBox->SetPosition(nuiCenter);
  nuiLabel* pLabel = new nuiLabel(_T("Please wait. Scaning fonts..."));
  gpFontPathLabel = new nuiLabel(_T("...searching..."));
  //pLabel->SetTextColor(nuiColor(255, 255, 255));
  pBox->AddCell(pLabel, nuiCenter);
  pBox->AddCell(gpFontPathLabel, nuiCenter);
  gpWin->AddChild(pBox);
  gpWin->SetState(nglWindow::eShow);
#endif  
#endif  
  
  NGL_DEBUG( NGL_LOG("font", NGL_LOG_INFO, "Scan system fonts....\n"); )
  nglTime start_time;
  
  Clear();
  
  // Scan each path:
  std::map<nglString, nglPath>::const_iterator it = mFontFolders.begin();
  std::map<nglString, nglPath>::const_iterator end = mFontFolders.end();
  
  if (rescanAllFolders)
  {
    mScanedFolders.clear();
  }

  
  while (it != end)
  {
    nglPath path(it->second);
    
    if (mScanedFolders.find(path) == mScanedFolders.end())
    {
      mScanedFolders.insert(path);
      ScanSubFolder(path);
    }
    
    ++it;
  }

  nglTime end_time;
  
  double t = end_time - start_time;
  NGL_LOG("font", NGL_LOG_INFO, "Scaning the system fonts took %f seconds\n", t);

  delete gpWin;
  gpWin = NULL;
  
  nglOStream* pStream = mSavePath.OpenWrite();
  if (pStream)
    Save(*pStream);
  delete pStream;
}

bool nuiFontManager::ScanSubFolder(const nglPath& rBasePath)
{
  std::list<nglPath> children;
  rBasePath.GetChildrenTree(children);
  
  std::list<nglPath>::const_iterator cit = children.begin();
  std::list<nglPath>::const_iterator cend = children.end();
  
  while (cit != cend)
  {
    // enumerate all the faces of all the fonts:
    const nglPath& rPath(*cit);
    
    if (gpWin)
    {
      gpFontPathLabel->SetText(nglString().Add(_T("Found ")).Add((int32)mpFonts.size()));
      gpWin->ForceRepaint();
    }
    
    if (rPath.IsLeaf())
    {
      bool cont = true;
      int32 face = 0;

      NGL_ASSERT(!gFTLibrary);
      FT_Error error;
      error = FT_Init_FreeType(&gFTLibrary);

      while (cont)
      {
        
        nuiFontDesc* pFontDesc = new nuiFontDesc(rPath, face);
                
        if (pFontDesc->IsValid())
        {
          mpFonts.push_back(pFontDesc);
        }
        else
        {
          delete pFontDesc;
          cont = false;
        }
        face++;
      }
      
      FT_Done_FreeType(gFTLibrary);
      gFTLibrary = NULL;

    }
    else
    {
      ScanSubFolder(rPath);
    }
    
    ++cit;
  }
  
  return true;
}

void nuiFontManager::GetFolderList(std::vector<nglString>& rList) const
{
  rList.clear();
  std::map<nglString, nglPath>::const_iterator it = mFontFolders.begin();
  std::map<nglString, nglPath>::const_iterator end = mFontFolders.end();
  while (it != mFontFolders.end())
  {
    rList.push_back(it->first);
    ++it;
  }
}

void nuiFontManager::GetFonts(std::vector<nuiFontDesc*>& rList) const
{
  rList = mpFonts;
}

template <class T> float intersection_score(const std::set<T>& rRequest, const std::set<T>& rReference)
{
  float f = 1.0f / (float)rRequest.size();
  float score = 0.f;
  
  typename std::set<T>::const_iterator it = rRequest.begin();
  typename std::set<T>::const_iterator end = rRequest.end();
  
  while (it != end)
  {
    typename std::set<T>::const_iterator found = rReference.find(*it);
    if (found != rReference.end())
      score += f;
    ++it;
  }
  
  return score;
}

static bool greater_score(const nuiFontRequestResult& rLeft, const nuiFontRequestResult& rRight)
{
  float l = rLeft.GetScore(),
        r = rRight.GetScore();
  if (l == r)
  {
    // If this the same score is found for variants of the same font then try to place non italic and non bold fonts first:
    const nuiFontDesc* pL = rLeft.GetFontDesc();
    const nuiFontDesc* pR = rRight.GetFontDesc();
    
    if (pL->GetName() == pR->GetName())
    {
      int score1 = 0;
      int score2 = 0;
      score1 += pL->GetItalic() ? 2 : 0;
      score1 += pL->GetBold() ? 1 : 0;
      score2 += pR->GetItalic() ? 2 : 0;
      score2 += pR->GetBold() ? 1 : 0;

      return score1 < score2;
    }
  }
  return l > r;
}

static void SetScore(float& score, float& sscore, float request_score, bool strict, bool condition)
{
  if (strict)
  {
    if (condition)
      sscore *= request_score;
    else
      sscore = 0.0f;
  }
  else
  {
    if (condition)
      score += request_score;
    else if (request_score == 0.0f)
      sscore *= .95f; // Lower the sscore if the feature was not particularly requested
  }
}

#define SET_SCORE(X) SetScore(score, sscore, rRequest.m##X.mScore, rRequest.m##X.mStrict, pFontDesc->Get##X() == rRequest.m##X.mElement);
#define SET_SCORE2(X) SetScore(score, sscore, rRequest.m##X.mScore, rRequest.m##X.mStrict, pFontDesc->Get##X().Compare(rRequest.m##X.mElement, false) == 0);


void nuiFontManager::RequestFont(nuiFontRequest& rRequest, std::list<nuiFontRequestResult>& rFoundFonts) const
{
  rFoundFonts.clear();
  
  if (nuiFontRequest::gFontsForGenericNames.find(rRequest.mName.mElement) != nuiFontRequest::gFontsForGenericNames.end())
  {
    // given name is actually a generic name
    rRequest.mGenericName = rRequest.mName;
    rRequest.mName.mElement = nuiFontRequest::gDefaultFontsForGenericNames[rRequest.mGenericName.mElement];
  }
  
  std::vector<nuiFontDesc*>::const_iterator it = mpFonts.begin();
  std::vector<nuiFontDesc*>::const_iterator end = mpFonts.end();
  
  nglString dump;
  uint32 glyphcount = rRequest.mMustHaveGlyphs.mElement.size();

  while (it != end)
  {
    float score = 1.f;
    float sscore = 1.f;
    nuiFontDesc* pFontDesc = *it;
    //pFontDesc->GetPath()
    if (pFontDesc->GetName().Compare("LastResort", false) == 0)
      score *= 0.1;
    SET_SCORE2(Name);
    SET_SCORE2(Style);
    SET_SCORE(Face);
    SET_SCORE(Bold);
    SET_SCORE(Italic);
    SET_SCORE(Scalable);
    SET_SCORE(Monospace);
    
    {
      nglString genName = nuiFontRequest::GetGenericNameForFont(pFontDesc->GetName());
      if (!genName.IsNull())
      {
        if (rRequest.mGenericName.mStrict)
        {
          if (genName.Compare(rRequest.mGenericName.mElement) == 0)
          {
            sscore *= rRequest.mGenericName.mScore;
          }
          else
          {
            sscore = 0.f;
          }
        }
        else
        {
          if (genName.Compare(rRequest.mGenericName.mElement) == 0)
          {
            score += rRequest.mGenericName.mScore;
          }
        }
      }
    }
    
    {
      const std::set<nglTextEncoding>& rEncodings(pFontDesc->GetEncodings());
      float _s = rRequest.mMustHaveEncoding.mScore * intersection_score(rRequest.mMustHaveEncoding.mElement, rEncodings);
      if (rRequest.mMustHaveEncoding.mStrict)
      {
        if (_s > 0.f)
          sscore *= _s;
        else
          sscore = 0.f;
      }
      else
        score += _s;
    }
    
    if (glyphcount)
    {
      uint32 count = 0;  
      std::set<nglUChar>::const_iterator it = rRequest.mMustHaveGlyphs.mElement.begin();
      std::set<nglUChar>::const_iterator end = rRequest.mMustHaveGlyphs.mElement.end();
      
      while (it != end)
      {
        const nglUChar glyph = *it;
        if (pFontDesc->HasGlyph(glyph))
          count++;
        ++it;
      }
      
      float f = (float)count / (float)glyphcount;
      float _s = rRequest.mMustHaveGlyphs.mScore * f;
      if (rRequest.mMustHaveGlyphs.mStrict)
      {
          sscore *= _s;
      }
      else
      {
        score += _s;
      }
    }
    
    SetScore(score, sscore, rRequest.mPanose.mScore * rRequest.mPanose.mElement.GetNormalizedDistance(pFontDesc->GetPanoseBytes()), rRequest.mPanose.mStrict, true);
    
    if (!pFontDesc->GetScalable())
    {
      if (rRequest.mMustHaveSizes.mStrict)
      {
        sscore *= rRequest.mMustHaveSizes.mScore;
      }
      else
      {
        score += rRequest.mMustHaveSizes.mScore;
      }
    }
    else
    {
      const std::set<int32>& rSizes(pFontDesc->GetSizes());
      float _s = rRequest.mMustHaveSizes.mScore * intersection_score(rRequest.mMustHaveSizes.mElement, rSizes);
      if (rRequest.mMustHaveSizes.mStrict)
      {
        if (_s > 0.f)
          sscore *= _s;
        else
          sscore = 0.f;
      }
      else
        score += _s;
    }
    
    score *= sscore;
    
    //#FIXME: hacky-hack to avoid bitmap fonts
    if (score > 0.f)
      if (pFontDesc->GetScalable())
        rFoundFonts.push_back(nuiFontRequestResult(pFontDesc->GetPath(), pFontDesc->GetFace(), score, pFontDesc));
    
    ++it;
  }
  
  rFoundFonts.sort(greater_score);
  
  if (0)
  {
    std::list<nuiFontRequestResult>::const_iterator it = rFoundFonts.begin();
    std::list<nuiFontRequestResult>::const_iterator end = rFoundFonts.end();
    
    while (it != end)
    {
      const nuiFontRequestResult& r(*it);
      const nuiFontDesc* pDesc = r.GetFontDesc();
    NGL_LOG("font", NGL_LOG_INFO, "font '%s' bold: %s italic: %s (%f)\n", pDesc->GetName().GetChars(), pDesc->GetBold()?"Y":"N", pDesc->GetItalic()?"Y":"N", r.GetScore());
      ++it;
    }
  }
}


nuiFontManager nuiFontManager::gManager;

nuiFontManager& nuiFontManager::GetManager(bool InitIfNeeded)
{
  nuiFontBase::Init();

  const bool FORCE_FONT_ENUM = false;
  if (FORCE_FONT_ENUM || InitIfNeeded && gManager.mpFonts.empty())// && gManager.mFontFolders.empty())
  {
    App->AddExit(nuiFontManager::ExitManager);
    gManager.AddSystemFolders();
    gManager.ScanFolders();
#if defined(_UIKIT_)
    if (gManager.GetFontCount())
    {
      nglPath fontdb(ePathUserAppSettings);
      fontdb += nglString(NUI_FONTDB_PATH);
      nglOFile db(fontdb, eOFileCreate);
      if (db.IsOpen())
        gManager.Save(db);
      
      // Prevent file from being backed-up on iCloud...
      uint8 attrValue = 1;
      int result = setxattr(fontdb.GetPathName().GetStdString().c_str(), "com.apple.MobileBackup", &attrValue, sizeof (attrValue), 0, 0);
      NGL_ASSERT(result == 0);
    }
#endif
    
  }
  
  return gManager;
}

void nuiFontManager::ExitManager()
{
  gManager.Clear();
}

void nuiFontManager::InitManager(const nglPath& rSavePath)
{
  gManager.mSavePath = rSavePath;
}

nuiFontManager& nuiFontManager::LoadManager(nglIStream& rStream, double lastscantime)
{
  if (gManager.mFontFolders.empty())
  {
    gManager.AddSystemFolders();
  }
  
  gManager.Clear();
  gManager.Load(rStream, lastscantime);

  return gManager;
}


#define NUI_FONTDB_MARKER "nuiFontDatabase5"

bool nuiFontManager::Save(nglOStream& rStream)
{
  rStream.SetEndian(eEndianLittle);
  
  int s = strlen(NUI_FONTDB_MARKER) + 1;
  if (s != rStream.Write(NUI_FONTDB_MARKER, s, 1))
    return false;
  
  std::vector<nuiFontDesc*>::iterator it = mpFonts.begin(); 
  std::vector<nuiFontDesc*>::iterator end = mpFonts.end(); 
  
  while (it != end)
  {
    nuiFontDesc* pFontDesc = *it;
    
    pFontDesc->Save(rStream);    
    
    ++it;
  }
  
  
  return true;
}

bool nuiFontManager::Load(nglIStream& rStream, double lastscantime)
{
  rStream.SetEndian(eEndianLittle);
  
  int s = strlen(NUI_FONTDB_MARKER) + 1;
  char* marker = new char[s];
  if (s != rStream.Read(marker, s, 1))
  {
    delete marker;
    
    return false;
  }
  
  if (strcmp(NUI_FONTDB_MARKER, marker))
  {
    delete marker;
    
    return false;
  }
  
  delete[] marker;
  
  Clear();

  // compile list of font files
  std::set<nglPath> fontFiles;
  std::set<nglPath>::iterator itf;
  std::map<nglString, nglPath>::iterator it;
  
  bool scanfolders = true;
  
  if (scanfolders)
  {
    // All OSes 
    for (it = mFontFolders.begin(); it != mFontFolders.end(); ++it)
    {
      const nglString& str = it->first;
      const nglPath& pth = it->second;
      NGL_LOG("font", NGL_LOG_INFO, "scanning font folder '%s' '%s' for font files\n", str.GetChars(), pth.GetChars());
      
      std::list<nglPath> children;
      std::list<nglPath>::iterator itc;
      pth.GetChildrenTree(children);
      
      for (itc = children.begin(); itc != children.end(); ++itc)
      {
        const nglPath& path = *itc;
        if (path.IsLeaf())
        {
          fontFiles.insert(path);
          NGL_LOG("font", NGL_LOG_INFO, "font file found '%s'\n", path.GetChars());
        }
        else
        {
          NGL_LOG("font", NGL_LOG_INFO, "skip '%s'\n", path.GetChars());
        }
      }
    }
  }
  
  while (rStream.GetState() == eStreamReady)
  {
    nuiFontDesc* pFontDesc = new nuiFontDesc(rStream);
    
    if (pFontDesc->IsValid() && pFontDesc->GetScalable())
    {
      // check font file existence
      if (!pFontDesc->CheckPath())
      {
        NGL_LOG("font", NGL_LOG_INFO, "remove font from database '%s'\n", pFontDesc->GetPath().GetChars());
        
        continue;
      }
      
      // register the font 
      mpFonts.push_back(pFontDesc);
      
      // remove it from the compiled list
      itf = fontFiles.find(pFontDesc->GetPath());
      if (itf != fontFiles.end())
        fontFiles.erase(itf);
      
    }
    else
    {
      delete pFontDesc;
    }
  }
  
  // now, the files that are still in the compiled list are supposed to be newly installed fonts
  // let's add'em to the font database
  FT_Error error;
  error = FT_Init_FreeType(&gFTLibrary);
  for (itf = fontFiles.begin(); itf != fontFiles.end(); ++itf)
  {
    const nglPath& path = *itf;
   
#ifndef NUI_IOS // On iOS the db is never saved so we need to scan every times...
    if (path.GetLastMod() > lastscantime)
#else
    if (1)
#endif
    {
      bool cont = true;
      int32 face = 0;
      while (cont)
      {
        NGL_ASSERT(gFTLibrary);
        
        nuiFontDesc* pFontDesc = new nuiFontDesc(path, face);
        
        
        if (pFontDesc->IsValid())
        {
          mpFonts.push_back(pFontDesc);
          
          NGL_LOG("font", NGL_LOG_INFO, "add new font in database '%s'\n", path.GetChars());
        }
        else
        {
          delete pFontDesc;
          cont = false;
        }
        face++;
      }
    }
    else
    {
      NGL_LOG("font", NGL_LOG_INFO, "skip already scanned font '%s'\n", path.GetChars());
    }
    
  }
  
  FT_Done_FreeType(gFTLibrary);
  gFTLibrary = NULL;  
  return true;
}

void nuiFontManager::UpdateFonts()
{
  // compile list of font files
  std::set<nglPath> fontFiles;
  std::set<nglPath>::iterator itf;
  std::map<nglString, nglPath>::iterator it;
  
  bool scanfolders = true;
  
  if (scanfolders)
  {
    // All OSes 
    for (it = mFontFolders.begin(); it != mFontFolders.end(); ++it)
    {
      const nglString& str = it->first;
      const nglPath& pth = it->second;
      
      std::list<nglPath> children;
      std::list<nglPath>::iterator itc;
      pth.GetChildrenTree(children);
      
      for (itc = children.begin(); itc != children.end(); ++itc)
      {
        const nglPath& path = *itc;
        if (path.IsLeaf())
          fontFiles.insert(path);
      }
    }
  }
  
  // now, the files that are still in the compiled list are supposed to be newly installed fonts
  // let's add'em to the font database
  FT_Error error;
  error = FT_Init_FreeType(&gFTLibrary);
  for (itf = fontFiles.begin(); itf != fontFiles.end(); ++itf)
  {
    const nglPath& path = *itf;
    
    bool cont = true;
    int32 face = 0;
    while (cont)
    {
      NGL_ASSERT(gFTLibrary);
      
      nuiFontDesc* pFontDesc = new nuiFontDesc(path, face);
      
      
      if (pFontDesc->IsValid())
      {
        mpFonts.push_back(pFontDesc);
        
        NGL_LOG("font", NGL_LOG_INFO, "add new font in database '%s'\n", path.GetChars());
      }
      else
      {
        delete pFontDesc;
        cont = false;
      }
      face++;
    }
    
  }
  
  FT_Done_FreeType(gFTLibrary);
  gFTLibrary = NULL;  
}

void nuiFontManager::Clear()
{
  std::vector<nuiFontDesc*>::iterator it = mpFonts.begin(); 
  std::vector<nuiFontDesc*>::iterator end = mpFonts.end(); 
  
  while (it != end)
  {
    nuiFontDesc* pFontDesc = *it;
    
    delete pFontDesc;
    
    ++it;
  }
  
  mpFonts.clear();
  
}

uint32 nuiFontManager::GetFontCount() const
{
  return mpFonts.size();
}


bool nuiFontManager::FindFontInFolders(const nglString& rFontFileName, nglPath& rResultFontPath) const
{
  std::map<nglString, nglPath>::const_iterator it = mFontFolders.begin();
  std::map<nglString, nglPath>::const_iterator end = mFontFolders.end();
  while (it != mFontFolders.end())
  {
    nglPath p(it->second);
    p += rFontFileName;
    if (p.Exists() && p.IsLeaf())
    {
      rResultFontPath = p;
      return true;
    }
    ++it;
  }
  
  return false;
}

bool nuiFontManager::FindFontInSystemFolders(const nglString& rFontFileName, nglPath& rResultFontPath)
{
  std::map<nglString, nglPath>::const_iterator it = mFontFolders.begin();
  std::map<nglString, nglPath>::const_iterator end = mFontFolders.end();
  while (it != end)
  {
    nglPath p(it->second);
    p += rFontFileName;
    if (p.Exists() && p.IsLeaf())
    {
      rResultFontPath = p;
      return true;
    }
    ++it;
  }
  
  return false;
}

nuiFont* nuiFontManager::GetFont(nuiFontRequest& rRequest, const nglString& rID) const
{
  float size = 14.f;
  if (!rRequest.mMustHaveSizes.mElement.empty())
    size = *(rRequest.mMustHaveSizes.mElement.begin());
  
  NGL_LOG("font", NGL_LOG_INFO, "Loading font %s\n", rRequest.mName.mElement.GetChars());
  nuiFont* pFont = nuiFont::GetFont(rRequest.mOriginalName, size, rRequest.mFace.mElement, rID);
  if (pFont)
    return pFont;
  
  std::list<nuiFontRequestResult> Fonts;
  RequestFont(rRequest, Fonts);

  if (0)
  {
    std::list<nuiFontRequestResult>::iterator it = Fonts.begin();
    std::list<nuiFontRequestResult>::iterator end = Fonts.end();
    while (it != end)
    {
      const nuiFontRequestResult& res(*it);
      NGL_LOG("font", NGL_LOG_INFO, "font result [%s] %f\n", res.GetPath().GetChars(), res.GetScore());
      ++it;
    }
  }  
  
  if (Fonts.empty())
  {
    NGL_LOG("font", NGL_LOG_ERROR, "font request for '%s'failed, loading default font\n", rRequest.mName.mElement.GetChars());
    return nuiFont::GetFont(size);
  }
  
  const nuiFontRequestResult& rFont(*(Fonts.begin()));
  NGL_LOG("font", NGL_LOG_INFO, "found font '%s' (%s)\n", rFont.GetFontDesc()->GetName().GetChars(), rFont.GetFontDesc()->GetPath().GetChars());
  
  pFont = nuiFont::GetFont(rFont.GetPath(), size, rFont.GetFace(), rID);
  return pFont;
}
