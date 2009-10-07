/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h" 

/*
* Input methods (pull mode)
*/

int64 nglIStream::ReadUInt8   (uint8*  pData, int64 Count)
{
  return Read (pData, Count, sizeof(uint8));
}

int64 nglIStream::ReadUInt16  (uint16* pData, int64 Count)
{
  return Read (pData, Count, sizeof(uint16));
}

int64 nglIStream::ReadUInt32  (uint32* pData, int64 Count)
{
  return Read (pData, Count, sizeof(uint32));
}

int64 nglIStream::ReadUInt64  (uint64* pData, int64 Count)
{
  return Read (pData, Count, sizeof(uint64));
}

int64 nglIStream::ReadInt8   (int8*  pData, int64 Count)
{
  return Read (pData, Count, sizeof(int8));
}

int64 nglIStream::ReadInt16  (int16* pData, int64 Count)
{
  return Read (pData, Count, sizeof(int16));
}

int64 nglIStream::ReadInt32  (int32* pData, int64 Count)
{
  return Read (pData, Count, sizeof(int32));
}

int64 nglIStream::ReadInt64  (int64* pData, int64 Count)
{
  return Read (pData, Count, sizeof(int64));
}

int64 nglIStream::ReadFloat  (float*  pData, int64 Count)
{
  return Read (pData, Count, sizeof(float));
}

int64 nglIStream::ReadDouble (double* pData, int64 Count)
{
  return Read (pData, Count, sizeof(double));
}

#if 0
#define BUFFER_SIZE 128

int64 nglIStream::ReadLine (nglString& rLine, nglTextFormat* pFormat)
{
  int32 bytes_total = 0;
  int32 in_offset = 0;

  rLine.Wipe();
  if (pFormat)
    *pFormat = eTextNone;

  // Make sure we have a text conversion context
  if (!GetIConv())
    return 0;

  char buffer[BUFFER_SIZE];

  while (GetState() == eStreamReady)
  {
    nglTextFormat format = eTextNone;
    int32 parsed = 0, ending = 0, bytes;

    bytes = (int32)Peek (buffer, BUFFER_SIZE, 1);

    // Search for line endings
    while ((parsed < bytes) && (format == eTextNone))
    {
      ending = parsed;
      if (buffer[parsed] == '\0')
        format = eTextZero;
      else if (buffer[parsed] == '\n')
        format = eTextUnix;
      else if (buffer[parsed] == '\r')
      {
        // DOS/Mac ambiguity, need one more char to decide
        if (parsed < (bytes - 1))
        {
          // Simply fetch from buffer
          if (buffer[parsed + 1] == '\n')
          {
            format = eTextDOS;
            parsed++;
          }
          else
            format = eTextMac;
        }
      }
      parsed++;
    }

    if (format != eTextNone)
    {

      // We got a whole line and its ending convention
      if (pFormat)
        *pFormat = format;

      // Import line text
      int32 to_import = ending;

      rLine.Import(in_offset, buffer, to_import, *mpConv);
      if (mpConv->GetState())
      {
        //SetError(NGL_ISTREAM_???);
        return bytes_total;
      }
      rLine += '\n';

      // Forward : line chars + line ending char(s)
      SetPos(parsed, eStreamForward);
      bytes_total += parsed;
      return bytes_total;
    }
    else if (buffer[parsed - 1] == '\r')
    {
      // DOS/Mac ambiguity
      // Import chars till \r, see you in next loop
      int32 to_import = parsed - 1;

      rLine.Import(in_offset, buffer, to_import, *mpConv);
      if (mpConv->GetState())
      {
        //SetError(NGL_ISTREAM_???);
        return bytes_total;
      }

      // Forward till \r (next loop will start with it)
      SetPos(parsed - 1, eStreamForward);
      bytes_total += parsed - 1;
    }
    else
    {
      // We actually didn't encounter any line ending
      int32 to_import = parsed;

      rLine.Import(in_offset, buffer, to_import, *mpConv);
      switch (mpConv->GetState())
      {
      case eStringConv_OK:
        break;

      case eStringConv_NeedInput:
        // Bad luck, our buffer ends right on a byte sequence, adjust 'parsed'
        parsed -= to_import;
        break;

      default:
        //SetError(NGL_ISTREAM_???);
        return bytes_total;
      }
      SetPos(parsed, eStreamForward);
      bytes_total += parsed;
    }
    //    conv.RecycleBuffer();
  }

  return bytes_total;
}
#endif

int64 nglIStream::ReadLine (nglString& rLine, nglTextFormat* pFormat)
{
  int32 bytes_total = 0;
  
  rLine.Wipe();
  if (pFormat)
    *pFormat = eTextNone;
  
  std::vector<char> buffer;
  
  nglTextFormat format = eTextNone;
  
  bool done = false;
  uint8 c = 0;
  do
  {
    if (ReadUInt8(&c, 1) != 1)
      done = true;
    else
    {
      bytes_total++;
      switch (c)
      {
        case '\0':
          format = eTextZero;
          done = true;
          break;
          
        case '\n':
          format = eTextUnix;
          done = true;
          break;
          
        case '\r':
          {
            uint32 s = Peek(&c, 1, 1);
            if (!s || c != '\n')
            {
              format = eTextMac;
              done = true;
            }
            else
            {
              ReadUInt8(&c, 1);
              bytes_total++;
              format = eTextDOS;
              done = true;
            }
            
          }
          break;
        default:
          buffer.push_back(c);
          break;
      }
    }

  } while (!done && GetState() == eStreamReady);

  if (pFormat)
    *pFormat = format;
  
  int32 s = buffer.size();
  rLine.Import(&buffer[0], s, mTextEncoding);
    
  return bytes_total;
}


// FIXME: optimize ! At least add line-ending weighting
int64 nglIStream::ReadText (nglString& rLine, nglTextFormat* pFormat)
{
  nglString line;
  int64 done = 0, bytes;

  rLine.Wipe();
  while ((bytes = ReadLine(line)))
  {
    rLine += line;
    done += bytes;
  }

  return done;
}


/*
* Input methods, variant (pull mode)
*/

uint8 nglIStream::ReadUInt8 (uint8 Default)
{
  uint8 x;
  return (ReadUInt8(&x) < 1) ? Default : x;
}

uint16 nglIStream::ReadUInt16 (uint16 Default)
{
  uint16 x;
  return (ReadUInt16(&x) < 1) ? Default : x;
}

uint32 nglIStream::ReadUInt32 (uint32 Default)
{
  uint32 x;
  return (ReadUInt32(&x) < 1) ? Default : x;
}

uint64 nglIStream::ReadUInt64 (uint64 Default)
{
  uint64 x;
  return (ReadUInt64(&x) < 1) ? Default : x;
}

int8 nglIStream::ReadInt8 (int8 Default)
{
  int8 x;
  return (ReadInt8(&x) < 1) ? Default : x;
}

int16 nglIStream::ReadInt16 (int16 Default)
{
  int16 x;
  return (ReadInt16(&x) < 1) ? Default : x;
}

int32 nglIStream::ReadInt32 (int32 Default)
{
  int32 x;
  return (ReadInt32(&x) < 1) ? Default : x;
}

int64 nglIStream::ReadInt64 (int64 Default)
{
  int64 x;
  return (ReadInt64(&x) < 1) ? Default : x;
}

float nglIStream::ReadFloat (float Default)
{
  float x;
  return (ReadFloat(&x) < 1) ? Default : x;
}

double nglIStream::ReadDouble (double Default)
{
  double x;
  return (ReadDouble(&x) < 1) ? Default : x;
}


/*
* Default generic Peek() method
*/

int64 nglIStream::Peek (void* pData, int64 WordCount, uint WordSize)
{
  nglFileOffset pos = GetPos();
  nglFileOffset count = Read (pData, WordCount, WordSize);
  SetPos(pos);
  return (int64)count;
}

#define PIPE_BUF_SIZE 4096

int64 nglIStream::PipeTo(nglOStream& rTarget)
{
  uint8 buffer[PIPE_BUF_SIZE];  
  nglStreamState istate = GetState();
  nglStreamState ostate = rTarget.GetState();
  int64 total_piped = 0;  
  int64 piped_in, piped_out;

  while (( istate == eStreamReady || istate == eStreamWait ) && ( ostate == eStreamEnd || ostate == eStreamWait ))
  {    
    piped_in = Read( buffer, PIPE_BUF_SIZE, 1 );
    istate = this->GetState();

      while ( piped_in > 0 && ( ostate == eStreamEnd || ostate == eStreamWait ) )
      {
        piped_out = rTarget.Write( buffer, piped_in, 1 );

        ostate = rTarget.GetState();
        piped_in -= piped_out;
        total_piped += piped_out;
      }
  }

  return total_piped;
}

int64 nglIStream::PipeTo(nglOStream& rTarget, double MaxDuration)
{
  nglTime starttime;
  uint8 buffer[PIPE_BUF_SIZE];  
  nglStreamState istate = GetState();
  nglStreamState ostate = rTarget.GetState();
  int64 total_piped = 0;  
  int64 piped_in, piped_out;
  
  nglTime currenttime;
  while ( ((currenttime - starttime).GetValue() < MaxDuration) && ( istate == eStreamReady || istate == eStreamWait ) && ( ostate == eStreamEnd || ostate == eStreamWait ))
  {    
    piped_in = Read( buffer, PIPE_BUF_SIZE, 1 );
    istate = this->GetState();
    
    while ( piped_in > 0 && ( ostate == eStreamEnd || ostate == eStreamWait ) )
    {
      piped_out = rTarget.Write( buffer, piped_in, 1 );
      
      ostate = rTarget.GetState();
      piped_in -= piped_out;
      total_piped += piped_out;
    }
    nglTime now;
    currenttime = now;
  }
  
  return total_piped;
}


