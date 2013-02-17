/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


//class nuiParser

nuiParser::nuiParser(nglIStream* pStream, const nglPath& rSourcePath)
{
  mpStream = pStream;
  mSourcePath = rSourcePath;
  if (mSourcePath.IsLeaf())
    mSourcePath = mSourcePath.GetParent();
  mChar = _T(' ');
  mColumn = 1;
  mLine = 1;
  
  SetValidInValue(_T("!@#$%<>*?'+-&~|[]{}\().,"));
  SetValidInSymbolStart(_T("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"));
  SetValidInSymbol(_T("0123456789"));
  SetValidInBlank(_T(" \t\r\n"));
}

int32 nuiParser::GetLine() const
{
  return mLine;
}

int32 nuiParser::GetColumn() const
{
  return mColumn;
}

const nglString& nuiParser::GetErrorStr() const
{
  return mErrorString;
}

bool nuiParser::PeekChar()
{
  nglFileOffset ofs = mpStream->GetPos();
  bool res = NextChar();
  mpStream->SetPos(ofs);
  return res;
}

bool nuiParser::PeekString(uint32 len, nglString& rResult)
{
  nglFileOffset ofs = mpStream->GetPos();
  rResult.Wipe();
  while (len && NextChar())
  {
    rResult.Add(GetChar());
    len--;
  }
  mpStream->SetPos(ofs);
  return !len;
}


bool nuiParser::IsDone() const
{
  return mpStream->GetState() != eStreamReady;
}

bool nuiParser::NextChar()
{
  nglChar previous = mChar;
  // Parse an utf-8 char sequence:
  uint8 c = 0;
  if (1 != mpStream->ReadUInt8(&c, 1))
  {
    mChar = 0;
    return false;
  }
  if (!(c & 0x80))
  {
    mChar = c;
  }
  else
  {
    //  0xC0 // 2 bytes
    //  0xE0 // 3
    //  0xF0 // 4
    //  0xF8 // 5
    //  0xFC // 6
    uint32 count = 0;
    if ((c & 0xFC) == 0xFC)
    {
      mChar = c & ~0xFC;
      count = 5;
    }
    else if ((c & 0xF8) == 0xF8)
    {
      mChar = c & ~0xF8;
      count = 4;
    }
    else if ((c & 0xF0) == 0xF0)
    {
      mChar = c & ~0xF0;
      count = 3;
    }
    else if ((c & 0xE0) == 0xE0)
    {
      mChar = c & ~0xE0;
      count = 2;
    }
    else if ((c & 0xC0) == 0xC0)
    {
      mChar = c & ~0xC0;
      count = 1;
    }
    
    for (uint32 i = 0; i < count; i++)
    {
      if (1 != mpStream->ReadUInt8(&c, 1))
        return false;
      mChar <<= 6;
      mChar |= c & 0x3F;
    }
  }
  
  if ((mChar == 0xa && previous != 0xd) || (mChar == 0xd && previous != 0xa) )
  {
    mColumn = 1;
    mLine++;
  }
  
  //wprintf(_T("%lc"), mChar);
  
  return true;
}

nglChar nuiParser::GetChar() const
{
  return mChar;
}

bool nuiParser::IsBlank(nglChar c) const
{
  return mBlanks.find(c) != mBlanks.end();
}

bool nuiParser::SkipBlank()
{
  bool res = true;
  while ((IsBlank(mChar) || mChar == '/') && res) 
  {
    if (mChar == '/') // Is this a comment?
    {
      res = PeekChar();
      
      if (!res)
        return false;
      
      if (mChar == '/')
      {
        res = NextChar();
        // Skip to the end of the line:
        res = NextChar();
        while (res && mChar != 0xa && mChar != 0xd)
        {
          res = NextChar();
        }
      }
    }
    
    res = NextChar();
  }
  return res;
}

bool nuiParser::SkipToNextLine()
{
  bool res = NextChar();
  if (!res)
    return false;
  // Skip to the end of the line:
  while (res && mChar != 0xa && mChar != 0xd)
    res = NextChar();
  return res;
}


bool nuiParser::GetQuoted(nglString& rResult)
{
  if (!SkipBlank())
    return false;
  
  if (mChar != _T('\"'))
    return false;
  
  while (GetChar() && mChar != _T('\"'))
  {
    if (mChar == _T('\\'))
    {
      if (!GetChar())
        return false;
      
      if (mChar != _T('\"'))
        return false;
      
    }
    
    rResult.Add(mChar);
  }
  
  NextChar();
  return true;
}

bool nuiParser::GetSymbol(nglString& rResult)
{
  rResult.Nullify();
  if (!SkipBlank())
    return false;
  
  if (!IsValidInSymbolStart(mChar))
    return false;
  rResult.Add(mChar);
  
  if (!NextChar())
    return false;
  
  while (IsValidInSymbol(mChar))
  {
    rResult.Add(mChar);
    if (!NextChar())
      return false;
  }
  
  return !rResult.IsEmpty();
}

bool nuiParser::GetValue(nglString& rResult, bool AllowBlank)
{
  rResult.Nullify();
  if (!SkipBlank())
    return false;
  
  while ((AllowBlank && IsBlank(mChar)) || IsValidInValue(mChar))
  {
    rResult.Add(mChar);
    NextChar();
  }
  
  return true;
}

void nuiParser::SetError(const nglString& rError)
{
  mErrorString = rError;
  mError = true;
}

bool nuiParser::GetFloat(float& rResult)
{
  double r;
  bool res = GetFloat(r);
  rResult = r;
  return res;
}

bool nuiParser::GetFloat(double& rResult)
{
  bool neg = false;
  double value = 0;
  //  bool dotfound = false;
  
  if (mChar == '-')
  {
    neg = true;
    if (!NextChar())
      return false;
  }
  
  uint8 c = 0;
  if (!GetNumberDigit(c, mChar, 10))
    return false;
  while (mChar && c <= 9)
  {
    value = value * 10 + c;
    if (!NextChar())
      return false;
    if (!GetNumberDigit(c, mChar, 10))
      return false;
  }
  
  // Read decimal dot:
  if (mChar == '.')
  {
    if (!NextChar())
      return false;
    double count = 1, v = 0;
    if (!GetNumberDigit(c, mChar, 10))
      return false;
    while (mChar && c <= 9) // Read decimal part:
    {
      v = v * 10 + c;
      count *= 10;
      if (!NextChar())
        return false;
      if (!GetNumberDigit(c, mChar, 10))
        return false;
    }
    
    if (count)
    {
      value += v / count;
    }
  }
  
  if (mChar == 'e' || mChar == 'E')
  {
    if (!NextChar())
      return false;
    int32 exponent;
    if (!GetInteger(exponent, 10))
      return false;
    value *= pow(10.0, exponent);
  }
  
  return neg ? -value : value;
  
}


bool nuiParser::GetInteger(uint8&  rResult, uint8 Base)
{
  uint64 r;
  bool res = GetInteger(r, Base);
  rResult = r;
  return res && (r < (1 << 8));
}

bool nuiParser::GetInteger(uint16& rResult, uint8 Base)
{
  uint64 r;
  bool res = GetInteger(r, Base);
  rResult = r;
  return res && (r < (1 << 16));
}

bool nuiParser::GetInteger(uint32& rResult, uint8 Base)
{
  uint64 r;
  bool res = GetInteger(r, Base);
  rResult = r;
  bool res2 = r == rResult;
  return res && res2;
}

bool nuiParser::GetInteger(uint64& rResult, uint8 Base)
{
  uint64 old = 0;
  rResult = 0;
  uint8 digit = 0;
  
  while (GetNumberDigit(digit, mChar, Base))
  {
    rResult *= Base;
    old = rResult;
    rResult += digit;
    
    if ((rResult - digit) != old)
      return false;
    
    if (!NextChar())
      return false;
  }
  return true;
}

bool nuiParser::GetInteger(int8&  rResult, uint8 Base)
{
  uint64 r;
  bool res = GetInteger(r, Base);
  rResult = r;
  return res && (r < (1 << 8));
}

bool nuiParser::GetInteger(int16& rResult, uint8 Base)
{
  uint64 r;
  bool res = GetInteger(r, Base);
  rResult = r;
  return res && (r < (1 << 16));
}

bool nuiParser::GetInteger(int32& rResult, uint8 Base)
{
  uint64 r;
  bool res = GetInteger(r, Base);
  rResult = r;
  return res && (r < (1LL << 32));
}

bool nuiParser::GetInteger(int64& rResult, uint8 Base)
{
  int64 old = 0;
  rResult = 0;
  uint8 digit = 0;
  bool neg = false;
  if (mChar == '-')
  {
    neg = true;
    NextChar();
  }
  
  while (GetNumberDigit(digit, mChar, Base))
  {
    rResult *= Base;
    rResult += digit;
    
    if ((rResult - digit) != (old * Base))
      return false;
  }
  
  if (neg)
    rResult = -rResult;
  return true;
}


void nuiParser::SetValidInSymbolStart(const nglString& rValidChars)
{
  for (uint32 i = 0; i < rValidChars.GetLength(); i++)
    mValidInSymbolStart.insert(rValidChars[i]);
}

void nuiParser::SetValidInSymbol(const nglString& rValidChars)
{
  for (uint32 i = 0; i < rValidChars.GetLength(); i++)
    mValidInSymbol.insert(rValidChars[i]);
}

void nuiParser::SetValidInValue(const nglString& rValidChars)
{
  for (uint32 i = 0; i < rValidChars.GetLength(); i++)
    mValidInValue.insert(rValidChars[i]);
}

bool nuiParser::IsValidInValue(nglChar c) const
{
  if (mValidInValue.find(c) != mValidInValue.end())
    return true;
  return IsValidInSymbol(c);
}

void nuiParser::SetValidInBlank(const nglString& rValidChars)
{
  for (uint32 i = 0; i < rValidChars.GetLength(); i++)
    mBlanks.insert(rValidChars[i]);
}

bool nuiParser::IsValidInSymbolStart(nglChar c) const
{
  return (mValidInSymbolStart.find(c) != mValidInSymbolStart.end());
}

bool nuiParser::IsValidInSymbol(nglChar c) const
{
  if (mValidInSymbol.find(c) != mValidInSymbol.end())
    return true;
  return IsValidInSymbolStart(c);
}

bool nuiParser::IsNumberDigit(nglChar c, uint32 Base) const
{
  uint8 d = 0;
  return GetNumberDigit(d, c, Base);
}

bool nuiParser::GetNumberDigit(uint8& res, nglChar c, uint32 Base) const
{
  if (c >= '0' && c <= '9')
    c -= '0';
  else if (c >= 'a' && c <= 'z')
  {
    c -= 'a';
    c += 10;
  }
  else if (c >= 'A' && c <= 'Z')
  {
    c -= 'A';
    c += 10;
  }
  res = c;
  return res < Base;
}

bool nuiParser::Expect(const nglString& rString, bool CaseSensitive)
{
  nglString tmp;
  if (!PeekString(rString.GetLength(), tmp))
    return false;
  return tmp.Compare(rString, CaseSensitive);
}

bool nuiParser::Expect(nglChar ch, bool CaseSensitive)
{
  if (CaseSensitive)
  {
    if (GetChar() == ch)
    {
      NextChar();
      return true;
    }
  }
  else
  {
    if (tolower(GetChar()) == tolower(ch))
    {
      NextChar();
      return true;
    }
  }
  return false;
}
