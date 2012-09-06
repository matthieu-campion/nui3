/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

//#include "nui.h"
#include "nuiFontBase.h"
#include "nuiRect.h"
#include "nuiObject.h"

class nuiDrawContext;

/// This class implements a basic Font widget.
class NUI_API nuiFont : public nuiFontBase
{
public:
  // Constructors and destructors are protected!
  //  static nuiFont* GetFont (nglIStream* pInput); ///< Create a font from an input stream.
  static nuiFont* GetFont (const nglPath& rPath, nuiSize Size, int Face = 0, const nglString& rID = nglString::Empty); ///< Create a font from a path.
  static nuiFont* GetFont (const nglString& rName, uint8* pBuffer, uint BufferSize, nuiSize Size, int Face = 0, const nglString& rID = nglString::Empty); ///< Create a font from a buffer in memory and give it a name.
  static nuiFont* GetFont (nuiXMLNode* pNode, const nglString& rID = nglString::Empty); ///< Create a font from an xml description.
  static nuiFont* GetFont (const nglString& rId); ///< Get a font from its unique ID
  static nuiFont* GetFont (nuiSize Size, const nglString& rID = nglString::Empty); ///< Create the default NGL font.

  static nglString GetUniqueID(const nglString& rFace, nuiSize size, int face); ///< Get a text unique id for this font definition.

  static void GetFontIDs(std::vector<nglString>& rFontIDs);
  static void GetFonts(std::vector<nuiFont*>& rFonts);

  static nuiSimpleEventSource<0> FontListChanged;
  
  static void ClearAll();
protected:
  //nuiFont (nglIStream* pInput); ///< Create a font from an input stream.
  nuiFont (const nglString& rName, uint8* pBuffer, uint BufferSize, nuiSize Size, int Face = 0, const nglString& rID = nglString::Empty); ///< Create a font from a buffer in memory and give it a name.
  nuiFont (const nglPath& rPath, nuiSize size, int face, const nglString& rID = nglString::Empty); ///< Create a font from a path.
  nuiFont (nuiXMLNode* pNode, const nglString& rID = nglString::Empty); ///< Create a font from an xml description.
  nuiFont (nuiSize Size, int Face = 0, const nglString& rID = nglString::Empty); ///< Create the default NGL font.
  
  virtual ~nuiFont();
  friend class nuiDrawContext;
  virtual bool Apply(nuiDrawContext* pContext); ///< Set this Font as the current opengl Font. 

  static std::map<nglString,nuiFont*,nglString::LessFunctor> mpFonts;

  bool mPermanent;
};

