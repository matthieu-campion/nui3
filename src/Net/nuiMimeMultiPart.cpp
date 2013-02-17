/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

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

void nuiMimeMultiPart::AddFile(nglIStream* pStream, const nglString& rVarName, const nglString& rFileName, ContentTransfertEncoding encoding)
{
  File* pFile = NULL;

  nglString enc;
  switch (encoding)
  {
  case e7bit: enc = _T("7bit"); break;
  case e8bit: enc = _T("8bit"); break;
  case eBinary: enc = _T("binary"); break;
  case eConvertToBase64: 
  case eBase64: enc = _T("base64"); break;
  }

  if (encoding == eConvertToBase64)
  {
    std::vector<uint8> vec;
    vec.resize(pStream->Available());
    pStream->Read(&vec[0], vec.size(), 1);

    nglString str;
    str.EncodeBase64(&vec[0], vec.size());
    char* pB64 = str.Export();
    nglIMemory* pMem = new nglIMemory(pB64, strlen(pB64));
    pFile = new File(pMem, rVarName, rFileName, enc);
    free(pB64);
  }
  else
  {
    pFile = new File(pStream, rVarName, rFileName, enc);
  }
  mpFiles.push_back(pFile);
}

void nuiMimeMultiPart::AddFile(const nglPath& rPath, const nglString& rVarName, const nglString& rFileName, ContentTransfertEncoding encoding)
{
  nglString name(rFileName);
  if (name.IsNull())
    name = rPath.GetNodeName();
    
  nglIStream* pFile = rPath.OpenRead();
  if (!pFile)
    return;

  AddFile(pFile, rVarName, name, encoding);
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
  start.CFormat(_T("--%s"), mBoundary.GetChars());
  
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
      
      str.CFormat(_T("Content-Disposition: form-data; name=\"%s\"\n\n"), it->first.GetChars());
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
      str.CFormat(_T("Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\n"), mpFiles[i]->mVarName.GetChars(), mpFiles[i]->mFileName.GetChars());
      pStream->WriteText(str);
      str.CFormat(_T("Content-Transfer-Encoding: %s\n"), mpFiles[i]->mContentTransfertEncoding.GetChars());
      pStream->WriteText(str);
      str.CFormat(_T("Content-Type: %s\n\n"), mpFiles[i]->mType.GetChars());
      pStream->WriteText(str);
      pStream->WriteUInt8(&mpFiles[i]->mContents[0], mpFiles[i]->mContents.size());
    }
  }

  if (mVariables.empty() && mpFiles.empty())
    pStream->WriteText(start);
    
  pStream->WriteText(end);
  
  
  //mem.WriteUInt8((const uint8*)"\0", 1); // Add final 0 for printf
  //nglString enc(mem.GetBufferData(),  mem.GetSize());
  //NGL_OUT(_T("Mime encoded:\n%s\n"), enc.GetChars());
}

void nuiMimeMultiPart::Dump(nuiHTTPMessage* pMessage)
{
  // Change the content-type header:
  nglString header;
  header.CFormat(_T("multipart/form-data; boundary=%s"), mBoundary.GetChars());
  pMessage->AddHeader(_T("Content-Type"), header);
  
  // Output multi part mime to memory:
  nglOMemory mem;
  mem.SetTextFormat(eTextDOS);
  mem.SetTextEncoding(eUTF8);
  Dump(&mem);
  
  // Set the body of the message from memory
  pMessage->SetBody(mem.GetBufferData(), mem.GetSize());
}
                                                  
                                                  
