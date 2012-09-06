/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiFont.h"
#include "nuiPanose.h"


class nuiFontRequest : public nuiObject
{
public:
  nuiFontRequest(nuiFontBase* pOriginalFont = NULL, bool ForcePanoseOnlyFonts = false);
  nuiFontRequest(const nuiFontRequest& rOriginal);
  ~nuiFontRequest();
  
  /* generic font names are defined in the CSS standard
   http://www.w3.org/TR/2002/WD-css3-fonts-20020802/#generic-font-families
   A generic name is one of "serif", "sans-serif", "cursive", "fantasy", "monospace"
   The generic name can be set as:
    1) a parameter of the font request: the generic name is checked against the fonts-to-generic-names map (gGenericNamesForFonts)
                                        the intersection is used to calculate the score of the request
    2) the name of the font in the font request : the generic name is checked against the default font map (gDefaultFontsForGenericNames)
                                                  the genreic name of the font is replaced by the name of the default font for this operating system
   */
  static void AddGenericNameForFont(const nglString& rGenericName, const nglString& rFamilyName);
  static nglString GetGenericNameForFont(const nglString& rName);
  static void GetFontsForGenericName(const nglString& rName, std::vector<nglString>& rFonts);
  
  void SetName(const nglString& rName, float Score, bool Strict = false);
  void SetGenericName(const nglString& rName, float Score, bool Strict = false);
  void SetStyle(const nglString& rStyle, float Score, bool Strict = false);
  void SetFace(int32 Face, float Score, bool Strict = false);
  void SetItalic(bool Italic, float Score, bool Strict = false);
  void SetBold(bool Bold, float Score, bool Strict = false);
  void MustHaveGlyphs(const std::set<nglUChar>& rGlyphs, float Score, bool Strict = false);
  void MustHaveGlyph(nglUChar rGlyphs, float Score, bool Strict = false);
  void SetProportionnal(float Score, bool Strict = false);
  void SetMonospace(float Score, bool Strict = false);
  void SetScalable(float Score, bool Strict = false);
  void MustHaveEncoding(nglTextEncoding Encoding, float Score, bool Strict = false);
  void MustHaveSize(int32 size, float Score, bool Strict = false);
  void MustBeSimilar(const nuiPanose& rPanose, float Score, bool Strict = false);

  void ClearMustHaveGlyphs();
  void ClearMustHaveEncoding();
  void ClearMustHaveSize();
  void ClearMustBeSimilar();

  template <class T> class ScoredElement
  {
  public:
    ScoredElement(const T& rInit)
    {
      mElement = rInit;
      Clear();
    }

    ScoredElement(const ScoredElement<T>& rElement)
    : mScore(rElement.mScore),
      mStrict(rElement.mStrict),
      mElement(rElement.mElement)
    {
    }
    
    ScoredElement<T>& operator = (const ScoredElement<T>& rElement)
    {
      mScore = rElement.mScore;
      mStrict = rElement.mStrict;
      mElement = rElement.mElement;
      return *this;
    }
    
    void Clear()
    {
      mScore = 0.0f;
      mStrict = false;
    }
    
    float mScore;
    bool mStrict;
    T mElement;
  };
  
  
  ScoredElement<nglString>  mName;
  nglString                 mOriginalName;
  ScoredElement<nglString>  mGenericName;
  ScoredElement<nglString>  mStyle;
  ScoredElement<int32>      mFace;
  ScoredElement<bool>       mItalic;
  ScoredElement<bool>       mBold;
  ScoredElement<bool>       mMonospace;
  ScoredElement<bool>       mScalable;
  ScoredElement<std::set<nglUChar> >           mMustHaveGlyphs;
  ScoredElement<std::set<nglTextEncoding> >   mMustHaveEncoding;
  ScoredElement<std::set<int32> >             mMustHaveSizes;
  ScoredElement<nuiPanose>  mPanose;
  
  static std::multimap<nglString, nglString> gFontsForGenericNames;
  static std::map<nglString, nglString> gGenericNamesForFonts;
  static std::map<nglString, nglString> gDefaultFontsForGenericNames;

private:
  friend class nuiFontManager;

  void InitAttributes();
  
  void _SetName(const nglString& rName);
  void _SetGenericName(const nglString& rName);
  void _SetStyle(const nglString& rStyle);
  void _SetFace(int32 Face);
  void _SetItalic(bool Italic);
  void _SetBold(bool Bold);
  void _SetProportionnal(bool set);
  void _SetMonospace(bool set);
  void _SetScalable(bool set);
  void _SetSize(int32 size);  
  void _SetPanose(const nglString& rPanose);
};

class nuiFontRequestResult
{
public:
  nuiFontRequestResult(const nglPath& rPath, int32 Face, float Score, nuiFontDesc* pFontDesc);
  ~nuiFontRequestResult();
  
  const nglPath& GetPath() const;
  int32 GetFace() const;
  float GetScore() const;
  const nuiFontDesc* GetFontDesc() const;
private:
  nglPath mPath;
  int32 mFace;
  float mScore;
  nuiFontDesc* mpFontDesc;
};

class nuiFontManager
{
public:
  nuiFontManager();
  virtual ~nuiFontManager();
  
  void            AddFolder(const nglString& rId, const nglPath& rPath);
  void            DelFolder(const nglString& rId);
  void            DelFolder(const nglPath& rPath);
  const nglPath&  GetFolder(const nglString& rId) const;
  void AddSystemFolders();
  void GetSystemFolders(std::map<nglString, nglPath>& rFolders);
  void ScanFolders(bool rescanAllFolders = false);
  
  void GetFolderList(std::list<nglString>& rList) const;
  void GetFolderList(std::vector<nglString>& rList) const;
  bool FindFontInFolders(const nglString& rFontFileName, nglPath& rResultFontPath) const; 
  bool FindFontInSystemFolders(const nglString& rFontFileName, nglPath& rResultFontPath); 

  void RequestFont(nuiFontRequest& rRequest, std::list<nuiFontRequestResult>& rFoundFonts) const;
  nuiFont* GetFont(nuiFontRequest& rRequest, const nglString& rID = nglString::Null) const;
  void GetFonts(std::vector<nuiFontDesc*>& rList) const;
  
  uint32 GetFontCount() const;
  
  static void InitManager(const nglPath& rSavePath);
  static nuiFontManager& GetManager(bool InitIfNeeded = true);
  static void ExitManager();
  static nuiFontManager& LoadManager(nglIStream& rStream, double lastscantime = 0);
  
  bool Save(nglOStream& rStream);
  bool Load(nglIStream& rStream, double lastscantime = 0);
  
  void Clear();
private:
  std::map<nglString, nglPath> mFontFolders;
  std::vector<nuiFontDesc*> mpFonts;
  std::set<nglPath> mScanedFolders;
  
  static nuiFontManager gManager;
  
  nglPath mSavePath;
  bool ScanSubFolder(const nglPath& rPath);
  void UpdateFonts();
};

