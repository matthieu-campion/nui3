/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
#include "nglDataObjects.h"


void nglDataFilesObject::AddFile(const nglString& rPath)
{
  nglPath path(rPath);

// LBDEBUG FIXME see forward into the PromiseFiles question
//  if (GetNativeType() == kDragFlavorTypePromiseHFS)
//  {
//    nglPath folderPath(ePathUser);
//    char str[1024];
//    int32 len = 1024;
//    int32 start = 0;
//    folderPath.GetPathName().Export(start, str, len, eUTF8);
//    str[folderPath.GetPathName().GetLength()] = 0;
//    
//    FSRef destFolder;
//    OSStatus err = FSPathMakeRef(reinterpret_cast<const UInt8*>(str), &destFolder, NULL);
//    NGL_ASSERT(err == noErr);
//    
//    // replace tmp path with new one
//    nglPath newPath = folderPath + path.GetNodeName();
//    path = newPath.GetChars();
//    
//    FSRef fsref;
//    len = 1024;
//    start = 0;
//    path.GetPathName().Export(start, str, len, eUTF8);
//    str[path.GetPathName().GetLength()] = 0;
//    err = FSPathMakeRef(reinterpret_cast<const UInt8*>(str), &fsref, NULL);
//    NGL_ASSERT(err == noErr);
//    err = FSCopyObjectSync(&fsref, &destFolder, NULL, NULL, 0);
//    NGL_ASSERT(err == noErr);
//  }
  
  mFilePaths.push_back(path.GetPathName());
}
  

void nglDataFilesObject::AddDragItemFlavor(DragRef dragRef, DragItemRef& itemRef, nglNativeObjectType Type)
{
  OSErr err = noErr;
  size_t n = mNumberOfFiles ? mNumberOfFiles : mFilePaths.size();
    for (size_t i = 0; i<n; i++)
    {
      err = ::AddDragItemFlavor(dragRef, itemRef, kDragFlavorTypeHFS, NULL, 0, 0);
      mItemRefs.push_back(itemRef++);
      NGL_ASSERT(!err);
    }
}

void nglDataFilesObject::SetDragItemFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)
{
  OSErr err = noErr;
  nglString file;
  std::list<DragItemRef>::iterator iRef = mItemRefs.begin();
  for (std::list<nglString>::const_iterator i = GetFiles().begin(); i != GetFiles().end() && iRef != mItemRefs.end(); ++i, ++iRef)
  {
    if (itemRef == *iRef)
    {
      HFSFlavor hfsInfo;
      FSRef fileRef;
      int32 start = 0;
      int32 len = 1023;
      char str[1024];
      memset(str, 0, 1024);
      i->Export(start, str, len, eUTF8);
      err = FSPathMakeRef((const UInt8*)str, &fileRef, NULL);
      NGL_ASSERT(!err);
      
      FSCatalogInfo catInfo;
      err = FSGetCatalogInfo(&fileRef, kFSCatInfoFinderInfo, &catInfo, NULL, &(hfsInfo.fileSpec), NULL);
      NGL_ASSERT(!err);
      
      FileInfo* finfo = (FileInfo*) &catInfo.finderInfo;
      hfsInfo.fdFlags = finfo->finderFlags;
      hfsInfo.fileType = finfo->fileType;
      hfsInfo.fileCreator = finfo->fileCreator;
        
      err = ::SetDragItemFlavorData(dragRef, itemRef, kDragFlavorTypeHFS, &hfsInfo, sizeof (hfsInfo), 0);
      NGL_ASSERT(!err);
        
      //NGL_OUT("Adding file to drag data, itemRef is %d: %s\n", itemRef, (*i).GetChars());
      return;
    }
  }
}

void nglDataFilesObject::GetFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)
{
	if (flavorType == kDragFlavorTypeHFS)
	{
		OSErr err = noErr;
		Size dataSize;
		err = ::GetFlavorDataSize (dragRef, itemRef, flavorType, &dataSize);
		
		NGL_ASSERT(!err);
		
		HFSFlavor hfsInfo;
		err = ::GetFlavorData(dragRef, itemRef, flavorType, &hfsInfo, &dataSize, 0);
		
		NGL_ASSERT(!err);
		
		FSRef fileRef;
		err = FSpMakeFSRef(&(hfsInfo.fileSpec), &fileRef);
		
		char path_str[255];
		err = FSRefMakePath(&fileRef, (UInt8*)path_str, 254);
		
		NGL_ASSERT(!err);
		//NGL_OUT("HFS Type: file: %s\n", path_str);
   
		AddFile(nglString(path_str));
	}
	else if (flavorType == kDragFlavorTypePromiseHFS)
	{
		OSErr err = noErr;
    
		PromiseHFSFlavor hfsInfo;
    Size dataSize = sizeof(PromiseHFSFlavor);
		err = ::GetFlavorData(dragRef, itemRef, flavorType, &hfsInfo, &dataSize, 0);
    
    // check for a drop from find file
		if (hfsInfo.promisedFlavor == kDragPromisedFlavorFindFile) 
		{
			// from find file, no need to set the file location... 
		}
		else
		{
      FSRef fsref;
      
			// we'll put promised files in temp dir
			err = FSFindFolder(kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &fsref);
    
      NGL_ASSERT(!err);
    
      AEDesc dropLocation;
      err = AECreateDesc(typeFSRef, &fsref, sizeof(FSRef), &dropLocation);
    
      NGL_ASSERT(!err);
    
      err = SetDropLocation(dragRef, &dropLocation);
    
      NGL_ASSERT(!err);
    
      AEDisposeDesc(&dropLocation);
    }
    
    
    FSSpec fsspec;
		dataSize = sizeof(FSSpec);
		err = ::GetFlavorData(dragRef, itemRef, hfsInfo.promisedFlavor, &fsspec, &dataSize, 0);
		
		NGL_ASSERT(!err);
		
		FSRef fileRef;
		err = FSpMakeFSRef(&fsspec, &fileRef);
		
		NGL_ASSERT(!err);
		
		char path_str[255];
		err = FSRefMakePath(&fileRef, (UInt8*)path_str, 254);
		
		NGL_ASSERT(!err);
		//NGL_OUT("HFS Type: file: %s\n", path_str);
      
    AddFile(nglString(path_str));
    
		/*err = GetFlavorDataSize (dragRef, itemRef, flavorType, &dataSize);
		
		NGL_ASSERT(!err);
		
		PromiseHFSFlavor hfsInfo;
		err = ::GetFlavorData(dragRef, itemRef, flavorType, &hfsInfo, &dataSize, 0);
		
		NGL_ASSERT(!err);
		
		FSRef fsref;
		err = ::FSPathMakeRef((const UInt8*)("/tmp"), &fsref, NULL);
		
		NGL_ASSERT(!err);
		
		FSSpec folder;
		err = ::FSGetCatalogInfo(&fsref, kFSCatInfoNone, NULL, NULL, &folder, NULL);
		
		NGL_ASSERT(!err);
		
		err = ::SetDropFolder(dragRef, &folder);
		
		NGL_ASSERT(!err);
								
		FSSpec fsspec;
		dataSize = sizeof(FSSpec);
		err = ::GetFlavorData(dragRef, itemRef, hfsInfo.promisedFlavor, &fsspec, &dataSize, 0);
		
		NGL_ASSERT(!err);
		
		FSRef fileRef;
		err = FSpMakeFSRef(&fsspec, &fileRef);
		
		NGL_ASSERT(!err);
		
		char path_str[255];
		err = FSRefMakePath(&fileRef, (UInt8*)path_str, 254);
		
		NGL_ASSERT(!err);
		//NGL_OUT("HFS Type: file: %s\n", path_str);
		
		AddFile(nglString((char*)path_str));*/
	}
}

///////////////////////////
void nglDataTextObject::AddDragItemFlavor(DragRef dragRef, DragItemRef& itemRef, nglNativeObjectType Type)
{
  OSErr err = noErr;
  err = ::AddDragItemFlavor(dragRef, itemRef++, 'TEXT', NULL, 0, 0);
  //PrintErr(err);
  NGL_ASSERT(!err);
}

void nglDataTextObject::SetDragItemFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)
{
  Size dataSize = GetDataSize();
        
  void* dataPtr = new char[dataSize--]; // the '\0' Null terminated character is counted within GetDataSize
  memcpy(dataPtr, (const void*)(GetData().GetChars()), dataSize);
  ((char*)dataPtr)[dataSize] = '\0';
  ::SetDragItemFlavorData(dragRef, itemRef, flavorType, dataPtr, dataSize, 0);
  //NGL_OUT("DragSendDataFunction() Size: %d Text: [%s]\n", dataSize, (char*)dataPtr);
}

void nglDataTextObject::GetFlavorData(DragRef dragRef, DragItemRef& itemRef, FlavorType flavorType)
{
  OSErr err = noErr;
  Size dataSize;
  err = GetFlavorDataSize (dragRef, itemRef, flavorType, &dataSize);
  
  NGL_ASSERT(!err);
  
  void* dataPtr = (void*)(new char[dataSize+1]);
 
  //NGL_OUT("ReceiveData: dataSize: %d\n", dataSize);
  err = ::GetFlavorData(dragRef, itemRef, flavorType, dataPtr, &dataSize, 0);
  
  NGL_ASSERT(!err);
  
  ((char*)dataPtr)[dataSize] = '\0';
          
  AppendData(nglString((char*)dataPtr));
  
  NGL_ASSERT(dataPtr);
  
  delete (char*)dataPtr;
}

