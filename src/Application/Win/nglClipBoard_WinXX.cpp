/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglLog.h"
#include "nglClipBoard.h"


void nglClipBoard::Clear()
{
  // Open the clipboard and empty it
  if( !OpenClipboard(App->mHWnd) ) 
  {
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't open clipboard") ); 
    return;
  }
  if( !EmptyClipboard() ) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't empty clipboard") );
    (void)CloseClipboard();
    return; 
  }

  // Close the clipboard
  if(!CloseClipboard()) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't close clipboard"));
  }
}

void nglClipBoard::SetData(const nglMimeSource &src)
{
  // Open the clipboard and empty it
  if(!OpenClipboard(App->mHWnd)) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't open clipboard")); 
    return; 
  }
  
  if( !EmptyClipboard() ) ///empty it first
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't empty clipboard") );
    (void)CloseClipboard();
    return; 
  }

  HGLOBAL hglobal = GlobalAlloc(GMEM_MOVEABLE, src.GetDataSize()+1);
  if( !hglobal ) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't allocate global memory"));
    (void)CloseClipboard();
    return; 
  }

  byte* pGlobal = reinterpret_cast<byte*>( GlobalLock(hglobal) );
  if( !pGlobal ) 
  {
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't lock global memory"));
    (void)CloseClipboard();
    return;
  }

  // Copy our data to the global memory object, then unlock the global memory object
  const uint8 *pData = src.GetData();
  memcpy(pGlobal, pData, src.GetDataSize());
  if(!GlobalUnlock(hglobal) && GetLastError()) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't unlock global memory"));
  }

  // Place the handle on the clipboard
  if(!SetClipboardData(src.GetMimeType(), hglobal)) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't set clipboard data"));
  }

  // Close the clipboard
  if(!CloseClipboard()) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't close clipboard"));
  }
}

bool nglClipBoard::IsAvailable(const nglMimeSource::nglMimeDataType &mimeType)
{
  if(!IsClipboardFormatAvailable(mimeType))
  {
    return false;
  }
  return true;
}

void nglClipBoard::GetData(nglMimeSource &dst)
{
  if(!IsClipboardFormatAvailable(dst.GetMimeType()))
  {
    DWORD error = GetLastError();
    return;
  }

  // Open the clipboard
  if( !OpenClipboard(App->mHWnd) ) 
  {
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't open clipboard"));
    return; 
  }

  // Get the clipboard's global memory object
  HGLOBAL hglobal = GetClipboardData(dst.GetMimeType());
  if(!hglobal) 
  {
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't get clipboard data")); 
    (void)CloseClipboard(); 
    return;
  }

  // Lock the global memory object and get its size
  const byte* pGlobal = reinterpret_cast<const byte*>(GlobalLock(hglobal));
  if(!pGlobal) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't lock global memory"));
    (void)CloseClipboard();
    return;
  }


  // Copy the data into our object object, then unlock the global memory object
  uint size = GlobalSize(hglobal);
  if(!size) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't get global memory object size"));
    (void)GlobalUnlock(hglobal);
    (void)CloseClipboard();
    return;
  }

  dst.SetData(pGlobal, size);

  if( !GlobalUnlock(hglobal) && GetLastError() ) 
  { 
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't unlock global memory")); 
  }

  // Close the clipboard
  if( !CloseClipboard() ) 
  {
    NGL_LOG(_T("nglClipBoard"), NGL_LOG_ERROR, _T("Can't close clipboard")); 
  }
  return;
}

