/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiTranslator::nuiTranslator(const nglPath& rLanguagesFilesFolder)
{
  mLearning = false;
  LoadLanguages(rLanguagesFilesFolder);
}

nuiTranslator::~nuiTranslator()
{
}


bool nuiTranslator::LoadLanguages(const nglPath& rLanguageFilesFolder)
{
  mFiles.clear();
  
  std::list<nglPath> Children;
  rLanguageFilesFolder.GetChildren(&Children);
  
  std::list<nglPath>::iterator it = Children.begin();
  std::list<nglPath>::iterator end = Children.end();
  
  while (it != end)
  {
    const nglPath& rPath(*it);
    NGL_OUT(_T("Localization file: %s\n"), rPath.GetChars());
    if (rPath.GetExtension() == _T("loc"))
    {
      nglPath p(rPath.GetNodeName());
      nglString n(p.GetRemovedExtension());
      mFiles[n] = rPath;
    }
    
    ++it;
  }
  
  return mFiles.empty();
}

bool nuiTranslator::LoadLanguage(const nglPath& rLanguageFile)
{
  nglIStream* pStream = rLanguageFile.OpenRead();
  if (!pStream)
    return false;
  
  bool res = LoadLanguage(pStream);
  
  delete pStream;
  
  return res;
}

bool nuiTranslator::LoadLanguage(nglIStream* pStream)
{
  nuiCSV csv(_T(','));
  bool res = csv.Load(pStream);
  
  if (!res)
    return false;
    
  const std::vector<std::vector<nglString> >& rDoc(csv.GetDocument());
  
  uint32 count = 0;
  
  for (uint32 i = 0; i < rDoc.size(); i++)
  {
    const std::vector<nglString>& rLine(rDoc[i]);
    if (rLine.size() >= 3)
    {
      AddSentence(rLine[0], rLine[1], rLine[2]);
      count++;
    }
  }
  
  NGL_OUT(_T("Loaded %d translated sentences from %d lines\n"), count, rDoc.size());
  
  return true;
}

bool nuiTranslator::LoadLanguage(const nglString& rLangID)
{
  LanguageFiles::const_iterator it = mFiles.find(rLangID);
  if (it == mFiles.end())
    return false;
  
  const nglPath& rPath(it->second);
  return LoadLanguage(rPath);
}

bool nuiTranslator::SaveLanguage(const nglPath& rOutputLanguageFile) const
{
  nglOStream* pStream = rOutputLanguageFile.OpenWrite(true);
  bool res = SaveLanguage(pStream);
  delete pStream;
  return res;
}

bool nuiTranslator::SaveLanguage(nglOStream* pOutputLanguageStream) const
{
  nuiCSV csv(_T(','));
  
  DomainMap::const_iterator it = mDomains.begin();
  DomainMap::const_iterator end = mDomains.end();
  
  while (it != end)
  {
    const nglString& rDomain(it->first);
    const SentenceMap& rSentences(it->second);
    
    SentenceMap::const_iterator its = rSentences.begin();
    SentenceMap::const_iterator ends = rSentences.end();
    
    while (its != ends)
    {
      const nglString& src(its->first);
      const nglString& dst(its->second);
      
      std::vector<nglString> line;
      line.push_back(rDomain);
      line.push_back(src);
      line.push_back(dst);
      
      csv.InsertLine(line);
      
      ++its;
    }
    
    ++it;
  }
  
  csv.Save(pOutputLanguageStream);
  return false;
}


void nuiTranslator::Clear()
{
  mDomains.clear();
}


uint32 nuiTranslator::GetNbLanguages() const
{
  return mFiles.size();
}

void nuiTranslator::GetLanguages(std::vector<nglString>& rLanguages) const
{
  LanguageFiles::const_iterator it = mFiles.begin();
  LanguageFiles::const_iterator end = mFiles.end();
  
  while (it != end)
  {
    rLanguages.push_back(it->first);
    ++it;
  }
}

const nglString& nuiTranslator::Translate(const nglString& rStringToTranslate, const nglString& rDomain) const
{
//  NGL_OUT(_T("nuiTranslator::Translate [%s] '%s'\n"), rDomain.GetChars(), rStringToTranslate.GetChars());
  
  DomainMap::const_iterator it = mDomains.find(rDomain);
  if (it == mDomains.end())
  {
    if (!rDomain.IsNull())
    {
      bool old = mLearning;
      mLearning = false;
      const nglString& res = Translate(rStringToTranslate); // Try to look up the sentence in all domains if the requested domain doesn't exists.
      mLearning = old;

      // Add the result to the data base so that we can save it if we are in learn mode:
      if (mLearning)
        InternalAddSentence(rDomain, rStringToTranslate, res);
    }
    
    // Add the result to the data base so that we can save it if we are in learn mode:
    if (mLearning)
      InternalAddSentence(rDomain, rStringToTranslate, rStringToTranslate);
    return rStringToTranslate;
  }
  
  const SentenceMap& rSentences(it->second);
  SentenceMap::const_iterator its = rSentences.find(rStringToTranslate);
  if (its == rSentences.end())
  {
    // We have't found the sentence
    if (rDomain.IsNull()) // If we are already searching the Null domain, let's finish here with the default
    {
      if (mLearning)
        InternalAddSentence(rDomain, rStringToTranslate, rStringToTranslate);
      return rStringToTranslate;
    }
    
    // Try to look for the string in the default domain:
    bool old = mLearning;
    mLearning = false;
    const nglString& rResult = Translate(rStringToTranslate); // Try to look up the sentence in all domains if the requested domain doesn't exists.
    mLearning = old;
    
    // Add the result to the data base so that we can save it if we are in learn mode:
    if (mLearning)
      InternalAddSentence(rDomain, rStringToTranslate, rResult);
    return rResult;
  }
  
  return its->second;
}


void nuiTranslator::AddSentence(const nglString& rDomain, const nglString& rId, const nglString& rTranslatedSentence)
{
  mDomains[rDomain][rId] = rTranslatedSentence;
}

void nuiTranslator::InternalAddSentence(const nglString& rDomain, const nglString& rId, const nglString& rTranslatedSentence) const
{
  const_cast<nuiTranslator*>(this)->mDomains[rDomain][rId] = rTranslatedSentence;
}

void nuiTranslator::SetLearning(bool set)
{
  mLearning = set;
}

bool nuiTranslator::IsLearning() const
{
  return mLearning;
}

nuiTranslator* nuiTranslator::gpTranslator = NULL;

nuiTranslator& nuiTranslator::GetTranslator()
{
  if (!gpTranslator)
    gpTranslator = new nuiTranslator();
    
  return *gpTranslator;
}

const nglString& nuiTR(const nglString& rStringToTranslate, const nglString& rDomain)
{
  return nuiTranslator::GetTranslator().Translate(rStringToTranslate, rDomain);
}

const nglString nuiTR(const char* pStringToTranslate, const char* pDomain)
{
  return nuiTranslator::GetTranslator().Translate(nglString(pStringToTranslate), pDomain ? nglString(pDomain) : nglString::Null);
}

