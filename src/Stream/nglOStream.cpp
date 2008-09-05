/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

int64 nglOStream::WriteUInt8   (const uint8*  pData, int64 Count)
{
  return Write (pData, Count, sizeof(uint8));
}

int64 nglOStream::WriteUInt16  (const uint16* pData, int64 Count)
{
  return Write (pData, Count, sizeof(uint16));
}

int64 nglOStream::WriteUInt32  (const uint32* pData, int64 Count)
{
  return Write (pData, Count, sizeof(uint32));
}

int64 nglOStream::WriteUInt64  (const uint64* pData, int64 Count)
{
  return Write (pData, Count, sizeof(uint64));
}

int64 nglOStream::WriteInt8   (const int8*  pData, int64 Count)
{
  return Write (pData, Count, sizeof(int8));
}

int64 nglOStream::WriteInt16  (const int16* pData, int64 Count)
{
  return Write (pData, Count, sizeof(int16));
}

int64 nglOStream::WriteInt32  (const int32* pData, int64 Count)
{
  return Write (pData, Count, sizeof(int32));
}

int64 nglOStream::WriteInt64  (const int64* pData, int64 Count)
{
  return Write (pData, Count, sizeof(int64));
}

int64 nglOStream::WriteFloat  (const float*  pData, int64 Count)
{
  return Write (pData, Count, sizeof(float));
}

int64 nglOStream::WriteDouble (const double* pData, int64 Count)
{
  return Write (pData, Count, sizeof(double));
}

#define BUFFER_SIZE 4096

int64 nglOStream::WriteText (const nglChar* pData, int64 Count)
{
  nglString text(pData);
  int64 done;

  //text.Manage((nglChar*)pData); // We won't touch *pData, we promiss :)
  done = WriteText(text);
  //text.Manage(NULL);

  return done;
}

int64 nglOStream::WriteText (const nglString& rData)
{
  if (rData.IsEmpty() ||
    ((GetState() != eStreamReady) && (GetState() != eStreamEnd)))
    return 0;

  // Make sure we have a text conversion context
  if (!GetOConv())
    return 0;

  char buffer[BUFFER_SIZE];
  int64 written = 0;
  int32 out_offset = 0;

  do
  {
    int64 bytes;
    const char* data;
    int64 todo;
	int32 buffer_free = BUFFER_SIZE;

    // FIXME!!!!!!!!!!!
    rData.Export(out_offset, buffer, buffer_free, *mpConv);
    switch (mpConv->GetState())
    {
    case eStringConv_OK:
    case eStringConv_NeedOutput:
      break;

    default:
      //SetError() FIXME
      return written;
    }

    data = buffer;
    todo = BUFFER_SIZE - buffer_free;

    // Now write data[0 .. todo-1] with line-ending conversion if necessary
    if (mTextFormat == eTextNone)
    {
      bytes = Write(data, todo, 1);
      if (bytes > 0) written += bytes;
      if (bytes < todo) return written;
    }
    else
    {
      int64 done = 0;
      int64 last_ending, ending = -1;

      while (ending < todo)
      {
        last_ending = ending;
        do
        {
          ending++;
        }
        while ((ending < todo) && (data[ending] != '\n'));

        int64 line_size = ending - last_ending - 1;
        bytes = Write(&(data[last_ending + 1]), line_size, 1);
        if (bytes > 0)
        {
          done += bytes;
          written += bytes;
        }
        if (bytes < line_size) return written;

        if (done < todo)
        {
          switch (mTextFormat)
          {
          case eTextDOS : bytes = Write("\r\n", 1, 2) * 2; break;
          case eTextUnix: bytes = Write("\n", 1, 1); break;
          case eTextMac : bytes = Write("\r", 1, 1); break;
          case eTextZero: bytes = Write("\0", 1, 1); break;
          case eTextNone: break;
          }
          if (bytes > 0)
          {
            done++;
            written += bytes;
          }
          if (bytes < 1) return written;
        }
      }
    }
  }
  while (mpConv->GetState() == eStringConv_NeedOutput);

  return written;
}


/*
* Internals
*/

bool nglOStream::GetOConv()
{
  if (!mpConv)
  {
    mpConv = nglString::GetStringConv(nglEncodingPair(eEncodingInternal, mTextEncoding)); // internal -> mTextEncoding (export)
    if (!mpConv || mpConv->GetState())
    {
      // SetError(?? FIXME)
      return false;
    }
  }

  return true;
}
