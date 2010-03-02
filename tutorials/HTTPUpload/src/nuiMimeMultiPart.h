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
  nuiMimeMultiPart();
  
  void AddVariable(const nglString& rName, const nglString& rValue);
  void AddPart(nuiMimeMultiPart* pPart);
  void AddFile(const nglString& rVarName, const nglString& rFileName, nglIStream* pStream);
  void AddFile(const nglString& rVarName, const nglString& rFileName, const nglPath& rPath);
  
  void SetContents(nglIStream* pStream);
  void SetContents(const uint8* pBuffer, uint32 size);
  void SetContents(const std::vector<uint8>& rData);
  
  void Dump(nglOStream* pStream);
  void Dump(nuiHTTPMessage* pMessage);
  
protected:
  nglString mName;
  nglString mType;
  nglString mBoundary;

  std::vector<uint8> mContents;
  
  std::vector<nuiMimeMultiPart*> mpParts;
  std::vector<nuiMimeMultiPart*> mpFiles;
};