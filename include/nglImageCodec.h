/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglImageCodec.h
\brief Base class for nglImage pluggable loader/exporter
*/

#ifndef __nglImageCodec__
#define __nglImageCodec__

//#include "nui.h"
#include "nglError.h"
#include "nglString.h"

class nglIStream;
class nglOStream;

class nglImage;
class nglImageInfo;
class nglImageCodec;

//! Image codec description
/*!
Every image codec (loader and/or writer) registers itself to nglImage
passing its description via this class.
*/
class NGL_API nglImageCodecInfo
{
public:
  nglImageCodecInfo();
  virtual ~nglImageCodecInfo();
  virtual bool ExtensionMatch(nglString& rFileName);
  virtual nglImageCodec* CreateInstance() = 0; ///< Creates an instance of the corresponding codec

  bool mCanSave;                  ///< True if a codec can encode to a byte stream
  bool mCanLoad;                  ///< True if the codec can decode a byte stream
  nglString mName;                ///< The name of the codec (ie. 'JPEG', 'PNG', 'TGA', etc)
  std::vector<nglString> mExtensions;  ///< List of file name extensions for this kind of data
  nglString mInfo;                ///< Whatever ! (copyright info, user manual, etc)
};


//! Base class for nglImage pluggable loader/exporter
/*!
FIXME
*/
class NGL_API nglImageCodec 
{
public:
  virtual ~nglImageCodec();
  virtual bool Init(nglImage* pImage);      ///< Codec init is decoupled from construction
  virtual bool Probe(nglIStream* pIStream) = 0;  ///< Check for a known signature at stream start. Returns true if it wants to take the job
  virtual bool Feed(nglIStream* pOStream) = 0; 
  virtual bool Save(nglOStream* pOStream) = 0; 
  virtual float GetCompletion() = 0; 

protected:
  nglImage* mpImage;

  void SendInfo(nglImageInfo& rInfo);  ///< Send image description to image object. The image object will allocate the image buffer.
  void SendData(float Completion);  ///< Acknowledge that more data was decoded to image buffer
  bool SendError();  ///< Signals an encoding/decoding error
};

#endif // __nglImageCodec_h__
