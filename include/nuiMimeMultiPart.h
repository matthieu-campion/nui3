/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

class nuiHTTPMessage;

class nuiMimeMultiPart
{
public:
  nuiMimeMultiPart(const nglString& rName = _T("Default"), const nglString& rType = _T("multipart/form-data"), const nglString& rBoundary = nglString::Null);
  virtual ~nuiMimeMultiPart();
  
  void AddVariable(const nglString& rName, const nglString& rValue);
  void AddFile(nglIStream* pStream, const nglString& rVarName, const nglString& rFileName);
  void AddFile(const nglPath& rPath, const nglString& rVarName, const nglString& rFileName = nglString::Null); // If rFileName is a Null string then we use the rPath node name as the filename.
  
  void Dump(nglOStream* pStream);
  void Dump(nuiHTTPMessage* pMessage);
  
protected:
  nglString mName;
  nglString mType;
  nglString mBoundary;

  std::map<nglString, nglString> mVariables;

  class File
  {
  public:
    File(const std::vector<uint8>& rData, const nglString& rVarName, const nglString& rFileName, const nglString& rType = _T("multipart/mixed"))
    : mVarName(rVarName), mType(rType), mFileName(rFileName), mContents(rData)
    {
    }
    
    File(nglIStream* pStream, const nglString& rVarName, const nglString& rFileName, const nglString& rType = _T("multipart/mixed"))
    : mVarName(rVarName), mType(rType), mFileName(rFileName)
    {
      uint32 len = pStream->Available();
      mContents.resize(len);
      pStream->Read(&mContents[0], len, 1);
      delete pStream;
    }
    
    nglString mVarName;
    nglString mType;
    nglString mFileName;
    std::vector<uint8> mContents;
  };
  std::vector<File*> mpFiles;
};