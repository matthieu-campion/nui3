/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiTranslator
{
public:
  static nuiTranslator& GetTranslator();
  
  nuiTranslator(const nglPath& rLanguagesFilesFolder = _T("rsrc:/localization/"));
  virtual ~nuiTranslator();
  
  bool LoadLanguages(const nglPath& rLanguagesFilesFolder); ///< Load the list of language definition files form the given folder path
  bool LoadLanguage(const nglPath& rLanguageFile); ///< Load a language from a path
  bool LoadLanguage(nglIStream* pStream); ///< Load a language definition file from a stream
  bool LoadLanguage(const nglString& rLangID); ///< Load a language by its locale id (i.e. fr_FR)
  bool SaveLanguage(const nglPath& rOutputLanguageFile) const; ///< Load a language from a path
  bool SaveLanguage(nglOStream* pOutputLanguageStream) const; ///< Load a language definition file from a stream
  
  void Clear(); ///< Remove all the translations
  
  uint32 GetNbLanguages() const;
  void GetLanguages(std::vector<nglString>& rLanguages) const;
  
  const nglString& Translate(const nglString& rStringToTranslate, const nglString& rDomain = nglString::Null) const;
  
  void AddSentence(const nglString& rDomain, const nglString& rId, const nglString& rTranslatedSentence);
  
  void SetLearning(bool set); ///< If in learning mode any sentence not found in the database will be automatically added to it
  bool IsLearning() const;
  
protected:
  typedef std::map<nglString, nglPath>      LanguageFiles;
  typedef std::map<nglString, nglString>    SentenceMap;
  typedef std::map<nglString, SentenceMap>  DomainMap;
  
  LanguageFiles mFiles;
  mutable DomainMap mDomains;
  mutable bool mLearning;

  void InternalAddSentence(const nglString& rDomain, const nglString& rId, const nglString& rTranslatedSentence) const;
  
  static nuiTranslator* gpTranslator;
};

const nglString& nuiTR(const nglString& rStringToTranslate, const nglString& rDomain = nglString::Null);
const nglString nuiTR(const char* pStringToTranslate, const char* pDomain = NULL);

