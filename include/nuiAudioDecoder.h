/*
 *  nuiAudioDecoder.h
 *  nui3
 *
 *  Created by mat on 12/18/08.
 *  Copyright 2008 Mxp4. All rights reserved.
 *
 */

#pragma once

#include "nui.h"
#include "nuiSampleInfo.h"

#ifdef _MACOSX_
#include <QuickTime/QuickTime.h>
#endif

class nuiAudioDecoder 
{
public:
  nuiAudioDecoder(nglIStream& rStream);
  virtual ~nuiAudioDecoder();
  
  bool Init();
  
  bool GetInfo(nuiSampleInfo& rInfo) const;
  uint32 Read(std::vector<float*> buffers, uint32 SampleFrames);
  
  bool SetPosition(uint64 pos);
  const uint64 GetPosition() const;
  
  bool BytesToSampleFrames(uint64 inBytes, uint64& outSampleFrames) const;
  bool SampleFramesToBytes(uint64 inSampleFrames, uint64& outBytes) const;
  
private:
  bool ReadInfo();
  bool Seek(uint64 SampleFrame);
  void Clear();
  
  bool mInitialized;
  nglIStream& mrStream; 
  uint64 mPosition;
  nuiSampleInfo mInfo;
  
#ifdef _MACOSX_
  uint8* mpInStreamData;
  Movie mMovie;
  MovieAudioExtractionRef mExtractionSessionRef;
  Handle mInStreamDataHandle;
  
  Handle createPointerDataRefWithExtensions( void *data, Size dataSize, Str255 fileName, OSType fileType, StringPtr mimeTypeString);
  OSStatus PtrDataRef_AddFileNameExtension( ComponentInstance dataRefHandler, Str255 fileName);
  OSStatus PtrDataRef_AddFileTypeExtension( ComponentInstance dataRefHandler, OSType fileType);
  OSStatus PtrDataRef_AddMIMETypeExtension(ComponentInstance dataRefHandler, StringPtr mimeType);
  Handle MyCreatePointerReferenceHandle(void *data, Size dataSize);
  bool CreateQuickTimeMovie(Handle dataHandle);
#endif
};
