/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglMimeSource.h
\brief Portable MIME data handling
*/

#ifndef __nglMimeSource_h__
#define __nglMimeSource_h__

#include "nglString.h"

class NGL_API nglMimeSource
{
  friend class nglClipBoard;

public:
#ifdef _WIN32_
    typedef UINT        nglMimeDataType;
#elif defined(_UNIX_)
    typedef uint        nglMimeDataType;
#elif defined(_CARBON_)
    typedef FlavorType  nglMimeDataType;
#else
    typedef int         nglMimeDataType;
#endif

    static const nglMimeDataType TextMimeData;

    static nglMimeSource::nglMimeDataType Register(const nglString &mimeType);

public:
  nglMimeSource(const nglMimeSource::nglMimeDataType &mimeType);
  virtual ~nglMimeSource();

  const nglMimeDataType &GetMimeType() const { return mMimeType; } ///< Returns the mime source's globally unique MIME id.

protected:

  virtual const uint8 *GetData() const = 0;
  virtual nglSize GetDataSize() const = 0;
  virtual void SetData(const uint8 *data, nglSize length) = 0;

private:
  nglMimeDataType mMimeType;
};


class NGL_API nglMimeTextSource : public nglMimeSource
{
public:
  nglMimeTextSource(const nglString & str = nglString::Empty);
  virtual ~nglMimeTextSource();

  virtual nglString GetText() const { return nglString(mTextUtf8, eUTF8); }

  virtual void SetText(const nglString& str);

protected:

  virtual const uint8 *GetData() const ;
  virtual nglSize GetDataSize() const;
  virtual void SetData(const uint8 *data, nglSize length);

private:
  char* mTextUtf8;
  nglSize mLength;
};


#endif // __nglMimeSource_h__

