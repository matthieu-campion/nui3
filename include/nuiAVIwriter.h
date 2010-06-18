/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"


class nuiAVIwriter 
{
public:
  
  nuiAVIwriter(nglOStream& stream, uint32 width, uint32 height, uint32 bpp,  float frame_rate); 
  nuiAVIwriter(const nglPath& output, uint32 width, uint32 height, uint32 bpp, float frame_rate); 
  ~nuiAVIwriter();

  void AddFrame(char* pFrame);
  void Close();
  
  
protected:
  
  typedef struct 
  {
    char fourcc[5];   /* The chunk's FOURCC code */
    long size_field;  /* The file offset of the beginning of the chunk header's size field */
    long data_start;  /* The file offset where data starts, for measuring chunk size */
  } ChunkStackEntry;
  
  typedef struct 
  {
    char fourcc[5];   /* The chunk's FOURCC code */
    int flags;        /* AVIIF_* flags */
    long offset;      /* File offset- number of bytes between the end of "movi" and the end
                       * of the chunk's FOURCC code. The documentation is fuzzy, but this
                       * is how mencoder seems to treat the field.
                       */
    long size;        /* Size of the indexed chunk- should match the size stored in the
                       * chunk's header after it's fixed up.
                       */
  } IndexQueueEntry;
  
  
private:

  void Init(nglOStream* stream, uint32 width, uint32 height,uint32 bpp,  float frame_rate); 

  void WriteHeaderList();
  void WriteMainHeader();
  void WriteStreamHeader();
  void WriteStreamFormat();
  void WriteIndex();
  
  void WriteFourcc(const char *fourcc);
  
  void PushChunk(const char *fourcc);
  void PopChunk();
  void PushHeader(const char *fileType);
  void PushList(const char *listType);
  void PopChunkWithIndex(int index_flags);
  
  
  nglOStream* mpStream;
  bool mOwnStream;
  int32 mBpp;
  int32 mWidth;
  int32 mHeight;
  float mFramerate;
  int32 mFrameCount;
  bool mClosed;
  
  std::list<ChunkStackEntry> mChunckStack; //< A stack of RIFF chunks that need their sizes fixed
  std::queue<IndexQueueEntry> mIndexQueue; //< A FIFO of index chunks to write later
  uint64 mIndexOriginOffset;
  
  // Offsets of particular things we need to fix later
  uint64 mFrameCountOffset;
  uint64 mLengthOffset;
};


