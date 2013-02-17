/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"



/* Scale factor for video stream length and frame rate */
#define RATE_SCALE 1000

/* Header flags */
#define AVIF_HASINDEX           0x00000010
#define AVIF_MUSTUSEINDEX       0x00000020
#define AVIF_ISINTERLEAVED      0x00000100
#define AVIF_TRUSTCKTYPE        0x00000800
#define AVIF_WASCAPTUREFILE     0x00010000
#define AVIF_COPYRIGHTED        0x00020000

/* Index flags */
#define AVIIF_LIST              0x00000001
#define AVIIF_KEYFRAME          0x00000010
#define AVIIF_NOTIME            0x00000100
#define AVIIF_COMPUSE           0x0FFF0000




nuiAVIwriter::nuiAVIwriter(nglOStream& stream, uint32 width, uint32 height, uint32 bpp, float frame_rate)
{
  mOwnStream = false;
  Init(&stream, width, height, bpp, frame_rate);
}


nuiAVIwriter::nuiAVIwriter(const nglPath& output, uint32 width, uint32 height, uint32 bpp, float frame_rate)
{
  nglOFile* file = new nglOFile(output, eOFileCreate);
  if (!file->IsOpen())
    return;
  
  mOwnStream = true;
  Init(file, width, height, bpp, frame_rate);
}

void nuiAVIwriter::Init(nglOStream* stream, uint32 width, uint32 height, uint32 bpp, float frame_rate)
{
  mpStream = stream;
  mpStream->SetEndian(eEndianLittle);
  
  mWidth = width;
  mHeight = height;
  mBpp = bpp;
  mFramerate = frame_rate;
  mFrameCount = 0;
  mClosed = false;
  
  /* Write out everything we need to before the movie data can start... */
  PushHeader("AVI ");
  WriteHeaderList();
  PushList("movi");
  
  /* Right here, at the beginning of the 'movi' list, is where index
   * offsets are measured from.
   */
  mIndexOriginOffset = mpStream->GetPos();
  
}

nuiAVIwriter::~nuiAVIwriter()
{
  Close();
}



void nuiAVIwriter::AddFrame(unsigned char* pFrame)
{
  int x, y;
  uint8 bgr[3];
  uint8 *pixels = (uint8*)pFrame;
  uint8 *row, *pixel;
  int width = mWidth;
  int height = mHeight;
  int rowstride = (mBpp / 8 * mWidth); // TODO : fix

  int n_channels = mBpp / 8;
  int padding = 0;
  
  NGL_ASSERT(width == mWidth);
  NGL_ASSERT(height == mHeight);
  
  /* Start an uncompressed video frame */
  PushChunk("00db");
  
  /* Write out our image data bottom-up in BGR order. Yuck.
   * Each row is padded to the next 4-byte boundary.
   */
  row = pixels + rowstride * (height-1);
  for (y = mHeight-1; y >= 0; y--) 
  {
    pixel = row;
    padding = 0;
    
    for (x=0; x < mWidth; x++) 
    {
	    bgr[2] = pixel[0];
	    bgr[1] = pixel[1];
	    bgr[0] = pixel[2];
      
	    mpStream->Write(bgr, 1, 3);
	    pixel += n_channels;
	    padding = (padding - 3) & 3;
    }
    
    if (padding) {
	    bgr[0] = bgr[1] = bgr[2] = 0;
	    mpStream->Write(bgr, 1, padding);
    }
    row -= rowstride;
  }
  
  PopChunkWithIndex(AVIIF_KEYFRAME);
  mFrameCount++;
}

void nuiAVIwriter::Close() 
{
  if (mClosed)
    return;
  
  mClosed = true;
  PopChunk();     /* Close the "movi" list */
  WriteIndex();
  PopChunk();     /* Close the "AVI" chunk */
  
  /* At this point, there should be no more open chunks */
  NGL_ASSERT(mChunckStack.size() == 0);
  
  /* Fix up frame count and stream length */
  mpStream->SetPos(mFrameCountOffset);
  mpStream->WriteInt32(&mFrameCount);
  mpStream->SetPos(mLengthOffset);
  int32 nb = mFrameCount * mFramerate * RATE_SCALE;
  mpStream->WriteInt32(&nb);
  
  while (!mIndexQueue.empty())
    mIndexQueue.pop();
  while (!mChunckStack.empty())
    mChunckStack.pop_back();
  
  if (mOwnStream)
  {
    nglOFile* pFile = (nglOFile*)mpStream;
    pFile->Close();
  }
}



void nuiAVIwriter::WriteHeaderList() 
{
  PushList("hdrl");
  
  WriteMainHeader();
  
  PushList("strl");
  WriteStreamHeader();
  WriteStreamFormat();
  PopChunk();
  
  PopChunk();
}



void nuiAVIwriter::WriteMainHeader() 
{
  PushChunk("avih");
  
  /* microseconds per frame */
  int32 tmp = 1000000 / mFramerate;
  mpStream->WriteInt32(&tmp);
  
  /* max bytes per second */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* padding granularity */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* flags (AVIF_* constants) */
  tmp = AVIF_HASINDEX;
  mpStream->WriteInt32(&tmp);
  
  /* total frames (we fill this in later) */
  mFrameCountOffset = mpStream->GetPos();
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* inital frames */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* number of streams */
  tmp = 1;
  mpStream->WriteInt32(&tmp);
  
  /* suggested buffer size */
  tmp = mWidth * mHeight * 3 + 1024;
  mpStream->WriteInt32(&tmp);
  
  /* width and height */
  mpStream->WriteInt32(&mWidth);
  mpStream->WriteInt32(&mHeight);
  
  /* reserved (4) */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  mpStream->WriteInt32(&tmp);
  mpStream->WriteInt32(&tmp);
  mpStream->WriteInt32(&tmp);
  
  PopChunk();
}




void nuiAVIwriter::WriteStreamHeader()
{
  PushChunk("strh");
  
  /* data type video stream */
  WriteFourcc("vids");
  
  /* data handler (video codec) */
  WriteFourcc("DIB ");
  
  /* flags */
  int32 tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* priority */
  int16 tmp16 = 1;
  mpStream->WriteInt16(&tmp16);
  
  /* language */
  tmp16 = 0;
  mpStream->WriteInt16(&tmp16);
  
  /* initial frames */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* scale followed by rate. For video streams, (rate/scale) is the frame rate. */
  tmp = RATE_SCALE;
  mpStream->WriteInt32(&tmp);
  tmp = mFramerate * RATE_SCALE;
  mpStream->WriteInt32(&tmp);
  
  /* start */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* length (we fill this in later) */
  mLengthOffset = mpStream->GetPos();
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* suggested buffer size */
  tmp = mWidth * mHeight * 3 + 1024;
  mpStream->WriteInt32(&tmp);
  
  /* quality */
  tmp = 10000;
  mpStream->WriteInt32(&tmp);
  
  /* sample size */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* frame position and size (left, top, right, bottom) */
  tmp16 = 0;
  mpStream->WriteInt16(&tmp16);
  tmp16 = 0;
  mpStream->WriteInt16(&tmp16);
  tmp16 = mWidth - 1;
  mpStream->WriteInt16(&tmp16);
  tmp16 = mHeight - 1;
  mpStream->WriteInt16(&tmp16);
  
  PopChunk();
}




void nuiAVIwriter::WriteStreamFormat() 
{
  PushChunk("strf");
  /* This is a BITMAPINFO structure for video streams */
  
  /* BITMAPINFOHEADER size */
  int32 tmp = 0x28;
  mpStream->WriteInt32(&tmp);
  
  /* width and height */
  mpStream->WriteInt32(&mWidth);
  mpStream->WriteInt32(&mHeight);
  
  /* Number of planes, "must be set to 1" */
  int16 tmp16 = 1;
  mpStream->WriteInt16(&tmp16);
  
  /* bits per pixel */
  tmp16 = 24;
  mpStream->WriteInt16(&tmp16);
  
  /* compression */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* size, in bytes, of the image */
  tmp = mWidth * mHeight * 3;
  mpStream->WriteInt32(&tmp);
  
  /* horizontal/vertical pixels per meter (75 dpi) */
  tmp = 2952;
  mpStream->WriteInt32(&tmp);
  tmp = 2952;
  mpStream->WriteInt32(&tmp);
  
  /* Color table size and number of important colors */
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  PopChunk();
}





/************************************************************************************/
/********************************************************************** RIFF Chunks */
/************************************************************************************/

void nuiAVIwriter::WriteFourcc(const char *fourcc) 
{
  mpStream->WriteUInt8((uint8*)fourcc, 4);
}




void nuiAVIwriter::PushChunk(const char *fourcc) 
{
  /* Write a chunk's RIFF header, with a dummy size, and add
   * it to the chunk_stack. The file pointer will be positioned
   * at the beginning of the chunk's data.
   */
  ChunkStackEntry new_chunk;
  
  WriteFourcc(fourcc);
  strncpy((char*)&new_chunk.fourcc, fourcc, 4);
  new_chunk.fourcc[4] = '\0';
  
  /* Write a dummy size field, pointing to it for later updating */
  new_chunk.size_field = mpStream->GetPos();
  int32 tmp = 0;
  mpStream->WriteInt32(&tmp);
  
  /* Point to the beginning of the chunk's data */
  new_chunk.data_start = mpStream->GetPos();
  
  mChunckStack.push_back(new_chunk);
}



void nuiAVIwriter::PopChunk() 
{
  /* Pop a chunk off of the chunk_stack, updating the chunk's size.
   * Assumes that the file pointer is placed just after the chunk data.
   */
  long after_data = mpStream->GetPos();
  ChunkStackEntry old_chunk;
  
  /* Pop this chunk off our stack */
  old_chunk = mChunckStack.back();
  mChunckStack.pop_back();
  
  /* Update its header's size */
  mpStream->SetPos(old_chunk.size_field);
  int32 tmp = after_data - old_chunk.data_start;
  mpStream->WriteInt32(&tmp);
  
  mpStream->SetPos(after_data);
}


void nuiAVIwriter::PushHeader(const char *fileType) 
{
  PushChunk("RIFF");
  WriteFourcc(fileType);
}


void nuiAVIwriter::PushList(const char *listType) 
{
  PushChunk("LIST");
  WriteFourcc(listType);
}


/************************************************************************************/
/************************************************************************ AVI Index */
/************************************************************************************/

/* Write the "idx1" list, using the queued chunk positions from our index FIFO */
void nuiAVIwriter::WriteIndex()
{
  
  PushChunk("idx1");
  
  /* Write all IndexQueueEntry nodes, freeing them as we go */
  while (!mIndexQueue.empty()) 
  {
    IndexQueueEntry current_entry = mIndexQueue.front();
    mIndexQueue.pop();
    
    WriteFourcc(current_entry.fourcc);
    mpStream->WriteInt32(&current_entry.flags);
    mpStream->WriteInt32((int32*)&current_entry.offset);
    mpStream->WriteInt32((int32*)&current_entry.size);
  }
  
  PopChunk();
}







/* This is a version of PopChunk() that adds the finished chunk
 * to this AVI file's index, using the provided flags.
 */
void nuiAVIwriter::PopChunkWithIndex(int index_flags)
{
  IndexQueueEntry new_entry;
  ChunkStackEntry current_chunk = mChunckStack.back();
  long current_offset = mpStream->GetPos();
  
  memcpy(&new_entry.fourcc[0], &current_chunk.fourcc[0], 5);
  new_entry.flags = index_flags;
  new_entry.offset = current_chunk.data_start - mIndexOriginOffset - 4;
  new_entry.size = current_offset - current_chunk.data_start;
  
  mIndexQueue.push(new_entry);
  PopChunk();
}


