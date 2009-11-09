/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
#include "nglDataObjects.h"

/*
 * nglDataObject
 */
nglDataObject::nglDataObject(const nglString& rMimeType)
: mMimeType(rMimeType)
{
  /*bool valid = */App->GetDataTypesRegistry().GetRegisteredFormatType(rMimeType, mNativeType);
  //NGL_ASSERT(valid);
}

nglDataObject::~nglDataObject()
{}
      
const nglString& nglDataObject::GetMimeType()
{ 
  return mMimeType;
}

nglNativeObjectType nglDataObject::GetNativeType()
{ 
  return mNativeType;
}

/*
 * nglDataFilesObject
 */
nglDataFilesObject::nglDataFilesObject(const nglString& rMimeType)
: nglDataObject(rMimeType), mNumberOfFiles(0)
{}

nglDataFilesObject::~nglDataFilesObject()
{}

nglDataObject* nglDataFilesObject::Create(const nglString& rMimeType)
{
  return new nglDataFilesObject(rMimeType);
}


const std::list<nglString>& nglDataFilesObject::GetFiles()
{ 
  return mFilePaths;
}

nglSize nglDataFilesObject::GetDataSize()
{
  uint32 size = 0;
  for (std::list<nglString>::iterator i = mFilePaths.begin(); i != mFilePaths.end(); i++)
    size += ((*i).GetLength()+1) * (sizeof (nglChar));
  return size;
}

void nglDataFilesObject::SetNumberOfFiles(size_t num)
{
  mNumberOfFiles = num;
}

/*
 * nglDataTextObject
 */
nglDataTextObject::nglDataTextObject(const nglString& rMimeType) 
: nglDataObject(rMimeType), mTextSize(0)
{}

nglDataTextObject::~nglDataTextObject()
{}

nglDataObject* nglDataTextObject::Create(const nglString& rMimeType)
{
  return new nglDataTextObject(rMimeType);
}

void nglDataTextObject::SetData(const nglString& rDataText)
{ 
  mDataText = rDataText; 
}

void nglDataTextObject::AppendData(const nglString& rDataText)
{ 
  mDataText.Append(rDataText); 
}

const nglString& nglDataTextObject::GetData()
{
  return mDataText; 
}

nglSize nglDataTextObject::GetDataSize()
{ 
  if (mTextSize)
    return mTextSize;
  return (mDataText.GetLength()+1) * (sizeof (char)); 
}

void nglDataTextObject::SetTextSize(uint32 size)
{
  mTextSize = size;
}
