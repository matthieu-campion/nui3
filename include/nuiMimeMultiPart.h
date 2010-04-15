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

  enum ContentTransfertEncoding
  {
    e7bit, ///< the contents is 7bit: the data is all represented as short lines of US-ASCII data.
    e8bit, ///< the contents is 8bit: the lines are short, but there may be non-ASCII characters (octets with the high-order bit set).
    eBinary, ///< the contents is binary: means that not only may non-ASCII characters be present, but also that the lines are not necessarily short enough for SMTP transport.
    eBase64, ///< the contents is base64: the data is all represented as a base64 stream of ASCII characters.
    eConvertToBase64, ///< the contents you give is binary but you need it to be transcoded to base64 for the transfert. 
    //eQuotedPrintable  NOT SUPPPORTED!
  };

  void AddFile(nglIStream* pStream, const nglString& rVarName, const nglString& rFileName, ContentTransfertEncoding encoding = eBinary);
  void AddFile(const nglPath& rPath, const nglString& rVarName, const nglString& rFileName = nglString::Null, ContentTransfertEncoding encoding = eBinary); // If rFileName is a Null string then we use the rPath node name as the filename.
  
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
    File(const std::vector<uint8>& rData, const nglString& rVarName, const nglString& rFileName, const nglString& rType = _T("multipart/mixed"), const nglString rContentTransfertEncoding = _T("binary"))
    : mVarName(rVarName), mType(rType), mFileName(rFileName), mContents(rData), mContentTransfertEncoding(rContentTransfertEncoding)
    {
    }
    
    File(nglIStream* pStream, const nglString& rVarName, const nglString& rFileName, const nglString& rType = _T("multipart/mixed"), const nglString rContentTransfertEncoding = _T("binary"))
    : mVarName(rVarName), mType(rType), mFileName(rFileName), mContentTransfertEncoding(rContentTransfertEncoding)
    {
      uint32 len = pStream->Available();
      mContents.resize(len);
      pStream->Read(&mContents[0], len, 1);
      delete pStream;
    }
    
    nglString mVarName;
    nglString mType;
    nglString mFileName;
    nglString mContentTransfertEncoding;
    std::vector<uint8> mContents;
  };
  std::vector<File*> mpFiles;
};