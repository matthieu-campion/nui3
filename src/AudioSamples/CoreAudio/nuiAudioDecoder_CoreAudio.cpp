/*
 *  nuiAudioDecoder_OSX.cpp
 *  nui3
 *
 *  Created by mat on 12/18/08.
 *  Copyright 2008 Mxp4. All rights reserved.
 *
 */

#include "nuiAudioDecoder.h"
#include <QuickTime/QuickTime.h>
#include "nuiAudioConvert.h"

bool CstrToPascal(const char* Cstr, Str255 PascalStr)
{
  int size = strlen(Cstr);
  if (size > 255)
    return false;
  
  PascalStr[0] = size;
  memcpy(PascalStr+1, Cstr, size);
  
  return true;
}


class nuiAudioDecoderPrivate
  {
  public:
    nuiAudioDecoderPrivate(nglIStream& rStream);
    virtual ~nuiAudioDecoderPrivate();
    
    bool Init(const char* pFileName);
    void Clear();
    
    nglIStream& mrStream;
    nglFileSize mStreamSize;
    bool mInitialized;
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
  };

nuiAudioDecoderPrivate::nuiAudioDecoderPrivate(nglIStream& rStream) :
mInitialized(false),
mrStream(rStream),
mpInStreamData(NULL),
mStreamSize(0)
{
  mrStream.SetPos(0);
  mStreamSize  = mrStream.Available(1);
  mpInStreamData    = new uint8[mStreamSize];
  mrStream.ReadUInt8(mpInStreamData, mStreamSize);
}

nuiAudioDecoderPrivate::~nuiAudioDecoderPrivate()
{
  if (mInitialized)
    Clear();
  
  if (mpInStreamData)
    delete mpInStreamData;
}

bool nuiAudioDecoderPrivate::Init(const char* pFileName)
{
  if (mInitialized)
  {
    Clear();
    mInitialized = false;
  }
  
  Str255 PascalFileName;
  CstrToPascal(pFileName, PascalFileName);
  mInStreamDataHandle = createPointerDataRefWithExtensions(mpInStreamData, mStreamSize, PascalFileName, NULL, NULL);
  if (mInStreamDataHandle)
  {
    if (CreateQuickTimeMovie(mInStreamDataHandle))
    {
      //Begin Extraction
      OSStatus err  = MovieAudioExtractionBegin(mMovie, 0, &mExtractionSessionRef);
      if (err == noErr)
        mInitialized = true;
    }
  }
  
  return mInitialized;
}

void nuiAudioDecoderPrivate::Clear()
{
  if (mInitialized)
  {
    MovieAudioExtractionEnd(mExtractionSessionRef);
    DisposeMovie(mMovie);
    
    if (mInStreamDataHandle)
      DisposeHandle(mInStreamDataHandle);
  }
  
  mInitialized = false;
}


void nuiAudioDecoder::Clear()
{
  if (mpPrivate)
    delete mpPrivate;
}


bool nuiAudioDecoder::Seek(uint64 SampleFrame)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return false;
  
  OSStatus err = noErr;
  TimeRecord timeRec;
  timeRec.scale       = GetMovieTimeScale(mpPrivate->mMovie);
  timeRec.base        = NULL;
  timeRec.value.hi    = 0;
  timeRec.value.lo    = SampleFrame / mInfo.GetSampleRate() * timeRec.scale;
  
  // Set the extraction current time.  The duration will 
  // be determined by how much is pulled.
  err = MovieAudioExtractionSetProperty(mpPrivate->mExtractionSessionRef, kQTPropertyClass_MovieAudioExtraction_Movie, kQTMovieAudioExtractionMoviePropertyID_CurrentTime, sizeof(TimeRecord), &timeRec);
  return (err == noErr);
}


bool nuiAudioDecoder::ReadInfo()
{
  //Init QuickTime
  if (mpPrivate)
  {
    delete mpPrivate;
  }
  mpPrivate = new nuiAudioDecoderPrivate(mrStream);
  
  EnterMovies();
  
  //pass a fake file name to the init method, quicktime will try to recognize this file type extension, if not, try another.....
  std::vector<const char*> FileExtensions;
  FileExtensions.push_back("blabla.mp3");
  FileExtensions.push_back("blabla.m4a");
  FileExtensions.push_back("blabla.wma");
  FileExtensions.push_back("blabla.wav");
  FileExtensions.push_back("blabla.aif");
  
  bool ready = false;
  for (uint32 i = 0; i < FileExtensions.size() && !ready; i++)
  {
    if (mpPrivate->Init(FileExtensions[i]))
    {
      ready = true;
    }
    else
    {
      ready = false;
      mpPrivate->Clear();
    }
  }
  if (!ready)
    return false;
  
  
  OSStatus err = noErr;
  
  //Set audio extraction properties
  AudioStreamBasicDescription asbd;
  
  // Get the default audio extraction ASBD
  err = MovieAudioExtractionGetProperty(mpPrivate->mExtractionSessionRef, kQTPropertyClass_MovieAudioExtraction_Audio, kQTMovieAudioExtractionAudioPropertyID_AudioStreamBasicDescription, sizeof (asbd), &asbd, nil);
  if (err != noErr)
    return false;
  
  // Convert the ASBD to return non-interleaved Float32.
  uint32 nbChannels     = asbd.mChannelsPerFrame;
  double SampleRate     = asbd.mSampleRate;
  uint32 BytesPerSample = sizeof(Float32);
  uint32 BitsPerSample  = BytesPerSample * 8;
  

  //We want non-interleaved audio samples,
  //so, in this case, the asbd object represents the samples format of only one channel
  //the extraction method will fill an AudioBufferList structure which contains one AudioBuffer per channel
  //since the following asbd description is relative to one AudioBuffer structure
  //that's why, here, there is only one channel by frame (nb of bits per channel = nb of bits per frame)
  asbd.mFormatFlags     = kAudioFormatFlagIsFloat | kAudioFormatFlagIsNonInterleaved | kAudioFormatFlagsNativeEndian;
  asbd.mBitsPerChannel  = BitsPerSample;
  asbd.mBytesPerFrame   = BytesPerSample;
  asbd.mBytesPerPacket  = asbd.mBytesPerFrame;
  
  // Set the new audio extraction ASBD
  err = MovieAudioExtractionSetProperty(mpPrivate->mExtractionSessionRef, kQTPropertyClass_MovieAudioExtraction_Audio, kQTMovieAudioExtractionAudioPropertyID_AudioStreamBasicDescription, sizeof (asbd), &asbd);
  if (err != noErr)
    return false;
  
  //retrieve the length of the stream
  TimeValue maxDuration = 0;
  UInt8 i;
  SInt32 trackCount = GetMovieTrackCount(mpPrivate->mMovie);
  NGL_ASSERT(trackCount);
  for (i = 1; i < trackCount + 1; i++)
  {
    Track aTrack = GetMovieIndTrackType(mpPrivate->mMovie, i, SoundMediaType, movieTrackMediaType);
    if (aTrack) 
    {
      TimeValue aDuration = GetTrackDuration(aTrack);
      if (aDuration > maxDuration) 
        maxDuration = aDuration;
    }
  }
  
  uint64 SampleFrames = (Float64)maxDuration / (Float64)GetMovieTimeScale(mpPrivate->mMovie) * SampleRate;
  
  mInfo.SetChannels(nbChannels);
  mInfo.SetSampleRate(SampleRate);
  mInfo.SetBitsPerSample(BitsPerSample);
  mInfo.SetSampleFrames(SampleFrames);
  
  return true;
}

//virtual uint32 ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);
//virtual uint32 ReadIN(void* pBuffer, uint32 sampleframes, nuiSampleBitFormat format = eSampleFloat32);

uint32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, uint32 sampleframes, nuiSampleBitFormat format)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return 0;
  
  uint32 nbChannels = mInfo.GetChannels();
  if (buffers.size() != nbChannels)
    return 0;
  
  uint64 BytestoRead = SampleFramesToBytes(sampleframes);
  
  std::vector<float*> temp(nbChannels);
  for (uint32 c = 0; c < nbChannels; c++)
  {
    if (format == eSampleFloat32)
    {
      temp[c] = (float*)(buffers[c]);
    }
    else
    {
      temp[c] = new float[sampleframes];
    }
  }
  
  OSStatus err = noErr;
  uint32 listSize = sizeof(AudioBufferList) + sizeof(AudioBuffer) * (nbChannels - 1);
  AudioBufferList* pBufList = reinterpret_cast<AudioBufferList*>(new Byte[listSize]);
  pBufList->mNumberBuffers = nbChannels; // we query non-interleaved samples, so we need as many buffers as channels
  
  for (int ch = 0; ch < pBufList->mNumberBuffers; ch++)
  {
    //each AudioBuffer object represents one channel since we want non-interleaved samples
    pBufList->mBuffers[ch].mNumberChannels  = 1;
    pBufList->mBuffers[ch].mDataByteSize    = BytestoRead / nbChannels;
    pBufList->mBuffers[ch].mData            = temp[ch];
  }
  
  UInt32 flags      = 0;
  UInt32 numFrames  = sampleframes;
  err = MovieAudioExtractionFillBuffer(mpPrivate->mExtractionSessionRef, &numFrames, pBufList, &flags);  //Extract
  if (err != noErr)
    return 0;
  
  if (format == eSampleInt16)
  {
    //convert from float 32 bits to int 16bits
    for (uint32 c = 0; c < nbChannels; c++)
    {
      nuiAudioConvert_FloatBufferTo16bits(temp[c], (int16*)(buffers[c]), numFrames);
      delete[] temp[c];
    }
  }
  
  uint32 SampleFramesRead = numFrames;
  mPosition  += SampleFramesRead;
  
  delete[] pBufList;
  
  return SampleFramesRead;
}



Handle nuiAudioDecoderPrivate::createPointerDataRefWithExtensions( void *data, Size dataSize, Str255 fileName, OSType fileType, StringPtr mimeTypeString)
{
  OSStatus  err = noErr;
  Handle dataRef = NULL;
  ComponentInstance dataRefHandler = NULL;
  
  // First create a data reference handle for our data
  dataRef = MyCreatePointerReferenceHandle(data, dataSize);
  if (!dataRef) goto bail;
  
  //  Get a data handler for our data reference
  err = OpenADataHandler(dataRef /* data reference */, PointerDataHandlerSubType /* data ref. type */, NULL /* anchor data ref. */, (OSType)0 /* anchor data ref. type */, NULL /* time base for data handler */, kDataHCanRead /* flag for data handler usage */, &dataRefHandler);           /* returns the data handler */
  if (err) goto bail;
  
  // We can add the filename to the data ref to help
  // importer finding process. Find uses the extension.
  // If we add a filetype or mimetype we must add a
  // filename -- even if it is an empty string
  if (fileName || fileType || mimeTypeString)
  {
    err = PtrDataRef_AddFileNameExtension(dataRefHandler /* data ref. handler */, fileName /* file name for extension */);
    
    if (err) goto bail;
  }
  
  // The pointer data handler can also be told the
  // filetype and/or MIME type by adding data ref
  // extensions. These help the importer finding process.
  // NOTE: If you add either of these, you MUST add
  // a filename first -- even if it is an empty Pascal
  // string. Any data ref extensions will be ignored.
  
  // to add file type, you add a classic atom followed
  // by the Mac OS filetype for the kind of file
  
  if (fileType)
  {
    err = PtrDataRef_AddFileTypeExtension(dataRefHandler /* data ref. handler */, fileType /* file type for extension */);
    if (err) goto bail;
  }
  
  
  // to add MIME type information, add a classic atom followed by
  // a Pascal string holding the MIME type
  
  if (mimeTypeString)
  {
    err = PtrDataRef_AddMIMETypeExtension (dataRefHandler /* data ref. handler */, mimeTypeString /* mime string for extension */);    
    if (err) goto bail;
  }
  
  /* dispose old data ref handle because
   it does not contain our new changes */
  DisposeHandle(dataRef);
  dataRef = NULL;
  
  /* re-acquire data reference from the
   data handler to get the new
   changes */
  err = DataHGetDataRef(dataRefHandler, &dataRef);
  if (err) goto bail;
  
  CloseComponent(dataRefHandler);
  
  return dataRef;
  
bail:
  if (dataRefHandler)
  {
    CloseComponent(dataRefHandler);
  }
  
  if (dataRef)
  {
    // make sure and dispose the data reference handle
    // once we are done with it
    DisposeHandle(dataRef);
  }
  
  return NULL;
}



//////////
//
// PtrDataRef_AddFileNameExtension
//
// Tell the data handler to set
// the file name extension in the
// data reference.
//
//////////

OSStatus nuiAudioDecoderPrivate::PtrDataRef_AddFileNameExtension( ComponentInstance dataRefHandler /* data ref. handler */, Str255 fileName /* file name for extension */)
{
  OSStatus anErr = noErr;
  unsigned char myChar = 0;
  Handle fileNameHndl = NULL;
  
  /* create a handle with our file name string */
  
  /* if we were passed a null string, then we
   need to add this null string (a single 0
   byte) to the handle */
  
  if (fileName == NULL)
    anErr = PtrToHand(&myChar, &fileNameHndl, sizeof(myChar));
  else
    anErr = PtrToHand(fileName, &fileNameHndl, fileName[0] + 1);
  if (anErr != noErr) goto bail;
  
  /* set the data ref extension for the
   data ref handler */
  anErr = DataHSetDataRefExtension(dataRefHandler /* data ref. handler */, fileNameHndl /* data ref. extension to add */, kDataRefExtensionFileName);
  
bail:
  if (fileNameHndl)
  /* no longer need this */
    DisposeHandle(fileNameHndl);
  
  return anErr;
  
}

//////////
//
// PtrDataRef_AddFileTypeExtension
//
// Tell the data handler to set
// the file type extension in the
// data reference.
//
//////////

OSStatus nuiAudioDecoderPrivate::PtrDataRef_AddFileTypeExtension( ComponentInstance dataRefHandler /* data ref. handler */,  OSType fileType /* file type for extension */)
{
  Handle      fileTypeHndl = NULL;
  OSStatus    anErr        = noErr;
  OSType      myType;
  
  myType = EndianU32_NtoB(fileType);
  
  anErr = PtrToHand(&myType, &fileTypeHndl, sizeof(OSType));
  if (anErr != noErr) goto bail;
  
  /* set the data ref extension for the
   data ref handler */
  anErr = DataHSetDataRefExtension(dataRefHandler /* data ref. handler */, fileTypeHndl /* data ref. extension to add */, kDataRefExtensionMacOSFileType);
  
bail:
  
  if (fileTypeHndl)
  /* no longer need this */
    DisposeHandle(fileTypeHndl);
  
  return anErr;
}

//////////
//
// PtrDataRef_AddMIMETypeExtension
//
// Tell the data handler to set
// the mime type extension in the
// data reference.
//
//////////

OSStatus nuiAudioDecoderPrivate::PtrDataRef_AddMIMETypeExtension(ComponentInstance dataRefHandler /* data ref. handler */, StringPtr mimeType /* mime type for extension */)
{
  OSStatus anErr = noErr;
  Handle mimeTypeHndl = NULL;
  
  if (mimeType == NULL)
    return paramErr;
  
  anErr = PtrToHand(mimeType, &mimeTypeHndl, mimeType[0] + 1);
  if (anErr != noErr) goto bail;
  
  /* set the data ref extension for the
   data ref handler */
  anErr = DataHSetDataRefExtension(dataRefHandler /* data ref. handler */, mimeTypeHndl /* data ref. extension to add */, kDataRefExtensionMIMEType);
  
bail:
  
  if (mimeTypeHndl)
  /* no longer need this */
    DisposeHandle(mimeTypeHndl);
  
  return anErr;
}


//////////
//
// MyCreatePointerReferenceHandle
// Create a pointer data reference handle.
//
// The pointer data reference handle contains
// a record specifying a pointer to a block of
// movie data along with a size value.
//
//////////

Handle nuiAudioDecoderPrivate::MyCreatePointerReferenceHandle(void *data, Size dataSize)
{
  Handle dataRef = NULL;
  PointerDataRefRecord ptrDataRefRec;
  OSErr err;
  
  
  ptrDataRefRec.data = data;
  ptrDataRefRec.dataLength = dataSize;
  
  // create a data reference handle for our data
  err = PtrToHand( &ptrDataRefRec, &dataRef, sizeof(PointerDataRefRecord));
  
  return dataRef;
}


bool nuiAudioDecoderPrivate::CreateQuickTimeMovie(Handle dataHandle)
{
  OSErr err = noErr;
  
  short id = 0;
  err = NewMovieFromDataRef(&mMovie, newMovieActive, &id, dataHandle, PointerDataHandlerSubType);
  
  return (err == noErr);
}



