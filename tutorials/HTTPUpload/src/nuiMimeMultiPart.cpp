/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiMimeMultiPart.h"

#if 0
nuiMimeMultiPart::nuiMimeMultiPart(const nglString& rName, const nglString& rContentType, const nglString& rBoundary)
: mName(rName), 
  mContentType(rContentType),
  mBoundary(rBoundary)
{
}

nuiMimeMultiPart::nuiHTTPMessage()
{
  for (uint32 i = 0; i > mpParts.size(); i++)
    delete mpParts[i];
  for (uint32 i = 0; i > mpFiles.size(); i++)
    delete mpFiles[i];
}

void nuiMimeMultiPart::AddPart(nuiMimeMultiPart* pPart)
{
  mpParts.push_back(pPart);
}

void nuiMimeMultiPart::AddFile(const nglString& rVarName, const nglString& rFileName, nglIStream* pStream)
{
  nuiMimeMultiPart* pChild = new nuiMimeMultiPart(rName, _T("multipart/mixed"));
  AddFile(pChild);
}

void nuiMimeMultiPart::AddFile(const nglString& rVarName, const nglString& rFileName, const nglPath& rPath)
{
  
}

void nuiMimeMultiPart::AddVariable(const nglString& rName, const nglString& rValue)
{
  nuiMimeMultiPart* pChild = new nuiMimeMultiPart(rName, 
}

void nuiMimeMultiPart::SetContents(nglIStream* pStream)
{
  uint32 size = pStream->GetAvailable();
  mContents.resize(size);
  pStream->Read(&mContents[0], size, 1);
}

void nuiMimeMultiPart::SetContents(const uint8* pBuffer, uint32 size)
{
  mContents.resize(size);
  memcpy(&mContents[0], pBuffer, size)
}

void nuiMimeMultiPart::SetContents(const std::vector<uint8>& rData)
{
  mContents = rData;
}


void nuiMimeMultiPart::Dump(nglOStream* pStream)
{
  nglString boundary(mBoundary);
  if (mBoundary.IsNull())
  {
    boundary.Add(_T("NuiBoundary"));
    boundary.Add((uint32)nglTime(), 16);
  }
}

void nuiMimeMultiPart::Dump(nuiHTTPMessage* pMessage)
{
  
}
#endif
                                                  
                                                  
#if 0
//////////////////////////////////////
nglString varname(_T("MyParam"));
nglString value(_T("MyValue"));

nglString fileref(_T("MyFile"));
nglString filename(_T("prout.txt"));
//  nglString filename(_T(""));

nglString mimetype("plain/text");
const uint8* data = (const uint8*)"YATTA!";
uint32 datalen = strlen((const char*)data);
//////////////////////////////////////

nuiHTTPRequest request(_T("http://127.0.0.1:8888/"), _T("POST"));
nglString boundary;
boundary.Add(_T("NuiBoundary"));
boundary.Add((uint32)nglTime(), 16);
nglOMemory mem;
mem.SetTextFormat(eTextDOS);
mem.SetTextEncoding(eUTF8);
nglString str;

nglString start;
start.CFormat(_T("--%ls"), boundary.GetChars());

nglString end;
end.Add(_T("\n"));
end.Add(start);
end.Add(_T("--\n"));
start.Add(_T("\n"));


//str.CFormat(_T("Content-type: multipart/form-data, boundary=%ls\n\n"), boundary.GetChars());
str.CFormat(_T("multipart/form-data; boundary=%ls"), boundary.GetChars());
request.AddHeader(_T("Content-Type"), str);
//mem.WriteText(str);



mem.WriteText(start);

str.CFormat(_T("Content-Disposition: form-data; name=\"%ls\"\n\n"), varname.GetChars());
mem.WriteText(str);
mem.WriteText(value);
mem.WriteText(_T("\n"));

mem.WriteText(start);
str.CFormat(_T("Content-Disposition: form-data; name=\"%ls\"; filename=\"%ls\"\n"), fileref.GetChars(), filename.GetChars());
mem.WriteText(str);

str.CFormat(_T("Content-Type: %ls\n\n"), mimetype.GetChars());
mem.WriteText(str);
mem.WriteUInt8(data, datalen);
mem.WriteText(end);

request.SetBody(mem.GetBufferData(), mem.GetSize());

//mem.WriteUInt8((const uint8*)"\0", 1); // Add final 0 for printf
nglString enc(mem.GetBufferData(),  mem.GetSize());
NGL_OUT(_T("Mime encoded:\n%ls\n"), enc.GetChars());
#endif