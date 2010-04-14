/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiMimeMultiPart.h"
#include "nuiHTTP.h"

nuiMimeMultiPart::nuiMimeMultiPart(const nglString& rName, const nglString& rContentType, const nglString& rBoundary)
: mName(rName), 
  mType(rContentType),
  mBoundary(rBoundary)
{
  if (mBoundary.IsEmpty())
  {
    mBoundary.Add(_T("NuiBoundary"));
    mBoundary.Add((uint32)nglTime(), 16);
  }
}

nuiMimeMultiPart::~nuiMimeMultiPart()
{
  for (uint32 i = 0; i > mpFiles.size(); i++)
    delete mpFiles[i];
}

void nuiMimeMultiPart::AddFile(const nglString& rVarName, const nglString& rFileName, nglIStream* pStream)
{
  File* pFile = new File(pStream, rVarName, rFileName);
  mpFiles.push_back(pFile);
}

void nuiMimeMultiPart::AddFile(const nglString& rVarName, const nglString& rFileName, const nglPath& rPath)
{
  nglIStream* pFile = rPath.OpenRead();
  if (!pFile)
    return;
  AddFile(rVarName, rFileName, pFile);
}

void nuiMimeMultiPart::AddVariable(const nglString& rName, const nglString& rValue)
{
  mVariables[rName] = rValue;
}


void nuiMimeMultiPart::Dump(nglOStream* pStream)
{
  nglString varname(_T("MyParam"));
  nglString value(_T("MyValue"));
  
  nglString fileref(_T("MyFile"));
  nglString filename(_T("prout.txt"));
  //  nglString filename(_T(""));
  
  nglString mimetype("plain/text");
  const uint8* data = (const uint8*)"YATTA!";
  uint32 datalen = strlen((const char*)data);
  //////////////////////////////////////
  
  nglString str;

  // prepare start and end boundaries:
  nglString start;
  start.CFormat(_T("--%ls"), mBoundary.GetChars());
  
  nglString end;
  end.Add(_T("\n"));
  end.Add(start);
  end.Add(_T("--\n"));
  start.Add(_T("\n"));
  
  
  {
    std::map<nglString, nglString>::const_iterator it = mVariables.begin();
    std::map<nglString, nglString>::const_iterator end = mVariables.end();
    while (it != end)
    {
      pStream->WriteText(start);
      
      str.CFormat(_T("Content-Disposition: form-data; name=\"%ls\"\n\n"), it->first.GetChars());
      pStream->WriteText(str);
      pStream->WriteText(it->second);
      pStream->WriteText(_T("\n"));
      
      ++it;
    }
  }

  {  
    for (uint32 i = 0; i < mpFiles.size(); i++)
    {
      pStream->WriteText(start);
      str.CFormat(_T("Content-Disposition: form-data; name=\"%ls\"; filename=\"%ls\"\n"), mpFiles[i]->mVarName.GetChars(), mpFiles[i]->mFileName.GetChars());
      pStream->WriteText(str);
      
      str.CFormat(_T("Content-Type: %ls\n\n"), mpFiles[i]->mType.GetChars());
      pStream->WriteText(str);
      pStream->WriteUInt8(&mpFiles[i]->mContents[0], mpFiles[i]->mContents.size());
    }
  }

  if (mVariables.empty() && mpFiles.empty())
    pStream->WriteText(start);
    
  pStream->WriteText(end);
  
  
  //mem.WriteUInt8((const uint8*)"\0", 1); // Add final 0 for printf
  //nglString enc(mem.GetBufferData(),  mem.GetSize());
  //NGL_OUT(_T("Mime encoded:\n%ls\n"), enc.GetChars());
}

void nuiMimeMultiPart::Dump(nuiHTTPMessage* pMessage)
{
  // Change the content-type header:
  nglString header;
  header.CFormat(_T("multipart/form-data; boundary=%ls"), mBoundary.GetChars());
  pMessage->AddHeader(_T("Content-Type"), header);
  
  // Output multi part mime to memory:
  nglOMemory mem;
  mem.SetTextFormat(eTextDOS);
  mem.SetTextEncoding(eUTF8);
  Dump(&mem);
  
  // Set the body of the message from memory
  pMessage->SetBody(mem.GetBufferData(), mem.GetSize());
}
                                                  
                                                  
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