/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#ifdef _WIN32_
  const nglMimeSource::nglMimeDataType nglMimeSource::TextMimeData = CF_TEXT;
#elif defined(_CARBON_)
  const nglMimeSource::nglMimeDataType nglMimeSource::TextMimeData = 'TEXT';
#else
  const nglMimeSource::nglMimeDataType nglMimeSource::TextMimeData = 0;
#endif


nglMimeSource::nglMimeSource(const nglMimeDataType &mimeType)
: mMimeType(mimeType)
{
}

nglMimeSource::~nglMimeSource()
{
}

nglMimeTextSource::nglMimeTextSource(const nglString & str)
: nglMimeSource(nglMimeSource::TextMimeData)
{
  mpTextUtf8 = str.Export(eUTF8);
  mLength = strlen(mpTextUtf8);
}


nglMimeTextSource::~nglMimeTextSource()
{
  delete mpTextUtf8;
}

void nglMimeTextSource::SetText(const nglString& str)
{
  delete mpTextUtf8;
  mpTextUtf8 = str.Export(eUTF8);
  mLength = strlen(mpTextUtf8);
}

const uint8 *nglMimeTextSource::GetData() const
{
  return (uint8*)mpTextUtf8;
}

nglSize nglMimeTextSource::GetDataSize() const
{
  return mLength;
}

void nglMimeTextSource::SetData(const uint8 *data, nglSize length)
{
  delete mpTextUtf8;
  mpTextUtf8 = new char[length + 1];

  memcpy(mpTextUtf8, data, length);
  mpTextUtf8[length] = 0;
  mLength = length;
}

nglString nglMimeTextSource::GetText() const
{
  return nglString(mpTextUtf8, eUTF8);
}

