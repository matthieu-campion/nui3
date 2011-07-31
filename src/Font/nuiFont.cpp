/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiFont.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"

nuiFont* nuiFont::GetFont(const nglPath& rPath, nuiSize size, int face, const nglString& rID)
{
  NGL_ASSERT(size);
  nuiFont* pFont= NULL;
  nglString id = rID.IsEmpty() ? GetUniqueID(rPath.GetPathName(), size, face) : rID;
  NGL_ASSERT(!id.IsEmpty());
  
  //NGL_OUT(_T("nuiFont::GetFont1 %s\n"), id.GetChars());

  if (mpFonts.find(id) == mpFonts.end())
  {
    if (!(rPath.Exists() && rPath.IsLeaf()))
      return NULL;

    pFont = new nuiFont(rPath, size, face, id);
    
    if (!pFont || !pFont->IsValid())
    {
      delete pFont;
      return NULL;
    }

  }
  else
    pFont = mpFonts[id];

  if (!pFont)
  {
    // there was an error loading the font...
    return GetFont(size);
    // Acquire is already called in this GetFont
  }

  pFont->Acquire();
  return pFont;
}

nuiFont* nuiFont::GetFont(const nglString& rName, uint8* pBuffer, uint BufferSize, nuiSize Size, int Face, const nglString& rID)
{
  NGL_ASSERT(Size);
  nuiFont* pFont= NULL;
  nglString id = rID.IsEmpty() ? GetUniqueID(rName, Size, Face) : rID;
  NGL_ASSERT(!id.IsEmpty());

  //NGL_OUT(_T("nuiFont::GetFont2 %s\n"), id.GetChars());

  if (mpFonts.find(id) == mpFonts.end())
  {
    if (!pBuffer || !Size)
      return GetFont(12.0f);
    
    pFont = new nuiFont(rName, pBuffer, BufferSize, Size, Face, id);

    if (!pFont->IsValid())
    {
      delete pFont;
      return GetFont(Size);
    }
  }
  else
    pFont = mpFonts[id];

  NGL_ASSERT(pFont);
  pFont->Acquire();
  return pFont;
}

nuiFont* nuiFont::GetFont(nuiXMLNode* pNode, const nglString& rID)
{
  nuiFont* pFont= NULL;
  nglString id = rID.IsEmpty() ? GetUniqueID(nuiGetString(pNode, _T("Source")), nuiGetVal(pNode, _T("Size"), 0.0f), nuiGetVal(pNode, _T("Face"), 0)) : rID;
  NGL_ASSERT(!id.IsEmpty());

  //NGL_OUT(_T("nuiFont::GetFont3 %s\n"), id.GetChars());

  nglPath path(nuiGetString(pNode, _T("Source")));
  if (mpFonts.find(id) == mpFonts.end())
  {
    if (path.Exists() & path.IsLeaf())
    {
      pFont = new nuiFont(pNode, id);
      
      if (pFont && !pFont->IsValid())
      {
        delete pFont;
        pFont = NULL;
      }
    }
  }
  else
  {
    pFont = mpFonts[id];
  }

  if (pFont)
  {
    pFont->Acquire();
    return pFont;
  }

  // there was an error loading the font...
  delete pFont;
  float s = 10;
  if (pNode->HasAttribute(_T("Size")))
    s = pNode->GetAttribute(_T("Size")).GetCFloat();

  return GetFont(s);
}

nuiFont* nuiFont::GetFont(const nglString& rId)
{
  //NGL_OUT(_T("nuiFont::GetFont4 %s\n"), rId.GetChars());

  std::map<nglString,nuiFont*, nglString::LessFunctor>::iterator it = mpFonts.find(rId);
  if (it == mpFonts.end())
    return NULL;

  nuiFont* pFont = it->second;
  if (pFont)
    pFont->Acquire();
  else
    return GetFont(12.0f);
  return pFont;
}

nuiFont* nuiFont::GetFont(nuiSize size, const nglString& rID)
{
  NGL_ASSERT(size);
  nuiFont* pFont= NULL;
  int face = 0;
  nglString id = rID.IsEmpty()?GetUniqueID(_T("NGL Default Font"), size, face) : rID;
  NGL_ASSERT(!id.IsEmpty());

  //NGL_OUT(_T("nuiFont::GetFont4 %s\n"), id.GetChars());

  if (mpFonts.find(id) == mpFonts.end())
  { 
    pFont = new nuiFont(size, face, id);
    if (!pFont->IsValid())
    {
      delete pFont;
      pFont = NULL;
    }
  }
  else
    pFont = mpFonts[id];

  if (pFont)
    pFont->Acquire();
  
  NGL_ASSERT(pFont);
  return pFont;
}


nuiFont::nuiFont(const nglPath& rPath, nuiSize size, int face, const nglString& rID)
: nuiFontBase(rPath, face, size)
{
  NGL_ASSERT(size);
  NGL_ASSERT(!rID.IsEmpty());
  //  NGL_OUT(_T("Creating nuiFont 0x%x."), this);
  SetObjectClass(_T("nuiFont"));
  SetProperty(_T("FontID"),rID);
  mPermanent = false;

  SetProperty(_T("Source"),rPath.GetPathName());
  nglString tmp;
  tmp.CFormat(_T("%f"),size);
  SetProperty(_T("Size"),tmp);
  tmp.CFormat(_T("%d"),face);
  SetProperty(_T("Face"),tmp);
  mpFonts[rID] = this;
  
  //SetSize(size);

  FontListChanged();
}

nuiFont::nuiFont(const nglString& rName, uint8* pBuffer, uint BufferSize, nuiSize Size, int Face, const nglString& rID)
: nuiFontBase(pBuffer, BufferSize, Face, false, Size)
{
//  NGL_OUT(_T("Creating nuiFont 0x%x."), this);
  NGL_ASSERT(Size);
  NGL_ASSERT(!rID.IsEmpty());
  SetObjectClass(_T("nuiFont"));
  SetProperty(_T("FontID"),rID);
  mPermanent = false;

  SetProperty(_T("Source"),rName);
  nglString tmp;
  tmp.CFormat(_T("%f"),Size);
  SetProperty(_T("Size"),tmp);
  tmp.CFormat(_T("%d"),Face);
  SetProperty(_T("Face"),tmp);
  mpFonts[rID] = this;
  //SetSize(Size);

  FontListChanged();
}

nuiFont::nuiFont (nuiXMLNode* pNode, const nglString& rID)
: nuiFontBase(nglPath(nuiGetString(pNode, _T("Source"))), 0)
{
  NGL_ASSERT(!rID.IsEmpty());
//  NGL_OUT(_T("Creating nuiFont 0x%x."), this);
  nuiSize size = nuiGetVal(pNode, _T("Size"), 0.0f);

  SetObjectClass(_T("nuiFont"));
  SetProperty(_T("FontID"),rID);
  mPermanent = false;
  SetPermanent(nuiGetBool(pNode, _T("Permanent"), false));

  SetProperty(_T("Source"), nuiGetString(pNode, _T("Source")));
  SetProperty(_T("Size"), nuiGetString(pNode, _T("Size"), _T("0")));
  SetProperty(_T("Face"), nuiGetString(pNode, _T("Face"), _T("0")));
  mpFonts[rID] = this;
  SetSize(size);
  FontListChanged();
}

nuiFont::nuiFont (nuiSize size, int face, const nglString& rID)
: nuiFontBase(size)
{
  NGL_ASSERT(size);
  NGL_ASSERT(!rID.IsEmpty());
  //  NGL_OUT(_T("Creating nuiFont 0x%x."), this);
  SetObjectClass(_T("nuiFont"));
  SetProperty(_T("FontID"),rID);
  mPermanent = false;

  SetProperty(_T("Source"), _T("NGL Default Font"));
  nglString tmp;
  tmp.CFormat(_T("%f"),size);
  SetProperty(_T("Size"),tmp);
  tmp.CFormat(_T("%d"),face);
  SetProperty(_T("Face"),tmp);
  mpFonts[rID] = this;

  //SetSize(size);

  FontListChanged();
}



nuiFont::~nuiFont()
{
  mpFonts.erase(GetProperty(_T("FontID")));
  FontListChanged();
}

bool nuiFont::Apply(nuiDrawContext* pContext)
{
  pContext->SetFont(this);
  return true;
}

nglString nuiFont::GetUniqueID(const nglString& rFace, nuiSize size, int face)
{
  nglString id = rFace;
  id.Add(_T(" ")).Add(face).Add(" ").Add(size);
  //id.CFormat(_T("%f %d %s"),size, face, rFace.GetChars());
  NGL_ASSERT(!id.IsEmpty());
  return id;
}

std::map<nglString,nuiFont*, nglString::LessFunctor> nuiFont::mpFonts;

void nuiFont::GetFontIDs(std::vector<nglString>& rFontIDs)
{
  rFontIDs.clear();
  std::map<nglString,nuiFont*,nglString::LessFunctor>::iterator it = mpFonts.begin();
  std::map<nglString,nuiFont*,nglString::LessFunctor>::iterator end = mpFonts.end();
  
  while (it != end)
  {
    rFontIDs.push_back(it->first);
    ++it;
  }
}

void nuiFont::GetFonts(std::vector<nuiFont*>& rFonts)
{
  rFonts.clear();
  std::map<nglString,nuiFont*,nglString::LessFunctor>::iterator it = mpFonts.begin();
  std::map<nglString,nuiFont*,nglString::LessFunctor>::iterator end = mpFonts.end();
  
  while (it != end)
  {
    rFonts.push_back(it->second);
    ++it;
  }
}

nuiSimpleEventSource<0> nuiFont::FontListChanged;

void nuiFont::ClearAll()
{
  std::map<nglString,nuiFont*,nglString::LessFunctor>::iterator it = mpFonts.begin();
  std::map<nglString,nuiFont*,nglString::LessFunctor>::iterator end = mpFonts.end();
  
  while (it != end)
  {
    nuiFont* pFont = it->second;
    int c = pFont->Release();
    it = mpFonts.begin();
  }
  
  mpFonts.clear();
}

