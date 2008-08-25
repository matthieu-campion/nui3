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
#include "nglString.h"


const nglChar* gpStringConvErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Parameter out of range"),
/*  2 */ _T("Unsupported conversion"),
/*  3 */ _T("Conversion incomplete (truncated input sequence)"),
/*  4 */ _T("Conversion incomplete (insufficient output buffer)"),
         NULL
};


nglChar nglStringConv::mUnknown = _T('?');


