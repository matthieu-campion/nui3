/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiAudioDecoder.h"
#include "nuiAudioConvert.h"

#include "mpg123.h"

#define DECODER_INPUT_SIZE 256

//
//
// nuiAudioDecoderPrivate
//
//

class nuiAudioDecoderPrivate
{
public:
  nuiAudioDecoderPrivate();
  virtual ~nuiAudioDecoderPrivate();

  bool Init();
  void Clear();

  mpg123_handle* mpHandle;

  static int sMpg123Counter;
};

int nuiAudioDecoderPrivate::sMpg123Counter = 0;

nuiAudioDecoderPrivate::nuiAudioDecoderPrivate()
: mpHandle(NULL)
{
}

nuiAudioDecoderPrivate::~nuiAudioDecoderPrivate()
{
	Clear();
}


bool nuiAudioDecoderPrivate::Init()
{
  int err = MPG123_OK;
  if (sMpg123Counter == 0)
  {
    // init mpg123 library
    err = mpg123_init();
    if (err != MPG123_OK)
      return false;
    sMpg123Counter++;
  }

  // new handle
  mpHandle = mpg123_new(NULL, &err);
  if (!mpHandle)
    return false;

  // open decoder and prepare for direct feeding
  err = mpg123_open_feed(mpHandle);
  if (err != MPG123_OK)
  {
    mpg123_delete(mpHandle);
    mpHandle = NULL;
    return false;
  }

	return true;
}

void nuiAudioDecoderPrivate::Clear()
{
  // delete decoder handle
  mpg123_delete(mpHandle);
  mpHandle = NULL;

  sMpg123Counter--;
  if (sMpg123Counter == 0)
  {
    // close mpg123 library
    mpg123_exit();
  }
}




//
//
// nuiAudioDecoder
//
//

void nuiAudioDecoder::Clear()
{
  if (mpPrivate)
    delete mpPrivate;
}

bool nuiAudioDecoder::CreateAudioDecoderPrivate()
{
  mpPrivate = new nuiAudioDecoderPrivate();
  bool res = mpPrivate->Init();

  if (!res)
  {
    delete mpPrivate;
    mpPrivate = NULL;
  }
  return res;
}

bool nuiAudioDecoder::ReadInfo()
{
  if (!mpPrivate)
  {
    return false;
  }

  int err;

  int read;
  int toread = DECODER_INPUT_SIZE;
  unsigned char* pInput = new unsigned char[toread];
  do
  {
    read = mrStream.ReadUInt8(pInput, toread);

    size_t done = 0;
    err = mpg123_decode(mpPrivate->mpHandle, pInput, read, NULL, 0, &done);
//    err = mpg123_feed(mpPrivate->mpHandle, pInput, read);
  } while (err != MPG123_NEW_FORMAT && read > 0);

  mpg123_frameinfo frameinfo;
  err = mpg123_info(mpPrivate->mpHandle, &frameinfo);
  if (err != MPG123_OK)
  {
    return false;
  }

  int channels;
  if (frameinfo.mode == MPG123_M_MONO)
    channels = 1;
  else
    channels = 2;

  double samplerate = 44100;
  int encoding = MPG123_ENC_SIGNED_16;

  int supportedChannelConfig = mpg123_format_support(mpPrivate->mpHandle, samplerate, encoding);
  if (supportedChannelConfig == 0)
  {
    return false;
  }

  int channelConfig;
  bool mono = (supportedChannelConfig & MPG123_MONO) && (channels == 1);
  bool stereo = (supportedChannelConfig & MPG123_STEREO) && (channels == 2);
  if (mono)
    channelConfig = MPG123_MONO;
  else if (stereo)
    channelConfig = MPG123_STEREO;
  else
  {
    return false;
  }

  mpg123_format_none(mpPrivate->mpHandle);

  err = mpg123_format(mpPrivate->mpHandle, samplerate, channelConfig, encoding);
  if (err != MPG123_OK)
  {
    return false;
  }

  off_t sampleframes = mpg123_length(mpPrivate->mpHandle);
  if (sampleframes <= 0)
  {
    int bitrate = frameinfo.bitrate;
    nglFileSize streamSize = mrStream.Available();
    sampleframes = (streamSize / (bitrate * 1000.f  / 8.f)) * samplerate;
  }

  int BitsPerSample;
  if (encoding == MPG123_ENC_FLOAT_32)
    BitsPerSample = 32;
  else if (encoding == MPG123_ENC_SIGNED_16)
    BitsPerSample = 16;
  else
  {
    return false;
  }

  mInfo.SetSampleFrames(sampleframes);
  mInfo.SetSampleRate(samplerate);
  mInfo.SetChannels(channels);
  mInfo.SetBitsPerSample(BitsPerSample);
  mInfo.SetFileFormat(eAudioCompressed);
  mInfo.SetStartFrame(0);
  mInfo.SetStopFrame(mInfo.GetSampleFrames());

  return true;
}

bool nuiAudioDecoder::Seek(int64 SampleFrame)
{
  off_t streamPosition;
  off_t res = mpg123_feedseek(mpPrivate->mpHandle, SampleFrame, SEEK_SET, &streamPosition);
  if (res < 0)
    return false;

  mrStream.SetPos(streamPosition);
  return true;
}






int32 nuiAudioDecoder::ReadIN(void* pBuffer, int32 sampleframes, nuiSampleBitFormat format)
{
  if (!mInitialized)
    return 0;

  SetPosition(mPosition);

  int32 BitsPerSample  = mInfo.GetBitsPerSample();
  int32 channels       = mInfo.GetChannels();
  int32 frameSize      = channels * (mInfo.GetBitsPerSample() / 8.f);
  int32 outBytes       = sampleframes * frameSize;

  unsigned char* pTemp;
  bool allocated = false;

  if (BitsPerSample != 16 && BitsPerSample != 32)
  {
    NGL_ASSERT(0);
    return 0;
  }
  else if ( ((format == eSampleFloat32) && (BitsPerSample == 32)) || ((format == eSampleInt16) && (BitsPerSample == 16)) )
  {
    pTemp = (unsigned char*)pBuffer;
  }
  else
  {
    pTemp = new unsigned char[outBytes];
    allocated = true;
  }

  unsigned char* pIn = NULL;

  int32 bytesDone = 0;
  int err = MPG123_OK;
  int32 bytesRead = -1;
  while (outBytes && err != MPG123_DONE && bytesRead != 0)
  {
    size_t outBytesDone = 0;
    unsigned char* pOut = pTemp + bytesDone;
    err = mpg123_decode(mpPrivate->mpHandle, NULL, 0, pOut, outBytes, &outBytesDone);
    outBytes  -= outBytesDone;
    bytesDone += outBytesDone;

    if (err == MPG123_NEW_FORMAT)
    {
      long r;
      int c;
      int e;
      mpg123_getformat(mpPrivate->mpHandle, &r, &c, &e);
    }

    // feed decoder if needed
    bytesRead = -1;
    while (err == MPG123_NEED_MORE && bytesRead != 0)
    {
      int32 inBytes = DECODER_INPUT_SIZE;
      if (!pIn)
      {
        pIn = new unsigned char[inBytes];
      }
      bytesRead = mrStream.ReadUInt8(pIn, inBytes);
      err = mpg123_decode(mpPrivate->mpHandle, pIn, bytesRead, NULL, 0, &outBytesDone);
    }

    if (err != MPG123_OK && err != MPG123_DONE)
    {
      NGL_LOG("nuiAudioDecoder", NGL_LOG_ERROR, "mpg123 error while decoding: %s", mpg123_strerror(mpPrivate->mpHandle));
    }
  }

  int32 frames = bytesDone / frameSize;

  if (format == eSampleFloat32 && BitsPerSample == 16)
  {
    // convert '16 bits int' samples in pTemp to '32 bits float' samples in pBuffer
    int16* pSrc = (int16*)pTemp;
    float* pCopy  = (float*)( ((int16*)pBuffer) + frames * channels );
    float* pFloat = (float*)pBuffer;

    // copy int16 data to the second half of the output buffer
    // => nuiAudioConvert_16bitsBufferToFloat converts in place
    // 'int16' samples atored in the second half of the buffer are converted in 'float' samples filling all the buffer (sizeof(float) == 2 * sizeof(int16))
    memcpy(pCopy, pSrc, frames * channels * sizeof(int16));

    nuiAudioConvert_16bitsBufferToFloat(pFloat, frames * channels); // convert in place (int16 to float)
  }
  else if (format == eSampleInt16 && BitsPerSample == 32)
  {
    // convert '32 bits float' samples in pTemp to '16 bits int' samples in pBuffer
    float* pFloat = (float*)pTemp;
    int16* pInt16 = (int16*)pBuffer;

    nuiAudioConvert_FloatBufferTo16bits(pFloat, pInt16, frames * channels);
  }

  if (allocated)
  {
    delete[] pTemp;
  }
  if (pIn)
  {
    delete[] pIn;
  }

  mPosition += frames;
  return frames;
}


int32 nuiAudioDecoder::ReadDE(std::vector<void*> buffers, int32 sampleframes, nuiSampleBitFormat format)
{
  //don't increment mPosition: it's already done in ReadIN
  int32 channels = mInfo.GetChannels();  
  int16* pInterleaved = NULL;
  float* pInterleavedFloat = NULL;
  
  int32 read = 0;
  switch (format)
  {
    case eSampleInt16:
      pInterleaved = new int16[sampleframes * channels];
      read = ReadIN((void*)pInterleaved, sampleframes, format);

      for (int32 c = 0; c < channels; c++)
      {
        int16* pDst = (int16*)(buffers[c]);
        int16* pSrc = pInterleaved + c;
        for (int32 i = 0; i < read; i++)
        {
          *(pDst++) = *pSrc;
          pSrc += channels;
        }
      }

      delete[] pInterleaved;
      break;

    case eSampleFloat32:
      pInterleavedFloat = new float[sampleframes * channels];
      read = ReadIN((void*)pInterleavedFloat, sampleframes, format);

      for (int32 c = 0; c < channels; c++)
      {
        float* pDst = (float*)(buffers[c]);
        float* pSrc = pInterleavedFloat + c;
        for (int32 i = 0; i < read; i++)
        {
          *(pDst++) = *pSrc;
          pSrc += channels;
        }
      }

      delete[] pInterleavedFloat;
      break;

    default:
      break;
  }


  return read;
}







