/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglMimeSource.h"

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
  mTextUtf8 = str.Export(eUTF8);
  mLength = strlen(mTextUtf8);
}


nglMimeTextSource::~nglMimeTextSource()
{
  free(mTextUtf8);
}

void nglMimeTextSource::SetText(const nglString& str)
{
  free(mTextUtf8);
  mTextUtf8 = str.Export(eUTF8);
  mLength = strlen(mTextUtf8);
}

const uint8 *nglMimeTextSource::GetData() const
{
  return (uint8*)mTextUtf8;
}

nglSize nglMimeTextSource::GetDataSize() const
{
  return mLength;
}

void nglMimeTextSource::SetData(const uint8 *data, nglSize length)
{
  memcpy(mTextUtf8, data, length);
}
