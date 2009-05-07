/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nglIStream.h"


/// allows to load and save data in CSV format
class NUI_API nuiCSV
{
public:
  nuiCSV(nglChar separationChar);
  ~nuiCSV();
  
  bool Load(nglIStream* pStream, bool CheckNbColumns = true); ///< load the cvs contents from an input stream
  bool Save(nglOStream* oStream); ///< save the formated csv contents to an output stream
  nglString Dump(); ///< return a string with the formated csv contents

  void InsertLine(const std::vector<nglString>& rLine); ///< insert a new line at the end of the document. each nglString is a text column.


  void EnableComments(bool set, nglChar commentTag = _T('#'));
  /*!< allows to insert comments into the csv document. By default, the comments option is disabled.
  \param set true or false to enable/disable comments in the csv. 
  \param commentTag the character that will be used as a comment line recognizer (in Load(), Dump() and Save())

  By default, the comments are disabled in the nuiCSV object. The CSV syntaxe doesn't define anything about comments.
  We choosed to add this option in case you need it inside your own application. Most of the csv importers (Excel...) allows to manually skip the first line(s) if thoses ones
  don't provide any csv data. But remember there is no official rule about comments.
  Once the comments are enabled : 
  - you can use the InsertComment() method to add some comments in your csv output
  - the Load() method is able to parse the comments correctly when reading the input stream
  */
  
  void InsertComment(const nglString& rComment);
  /*!< insert comments as a new line at the end of the current document
  \param rComment a simple text string. it will be formatted as a comments line by the nuiCSV object.

  You have to call EnabledComments(true) before being able to insert comments in the csv document.
  */
    
  const std::vector<std::vector<nglString> >& GetDocument(); ///< return the whole unformated document.
  
  bool IsCommented(uint32 linenum); ///< return true if the given line is commented
  
private : 

  std::vector<std::vector<nglString> > mDocument;
  nglChar mSeparationChar;
  nglChar mCommentTag;
  bool mCommentsEnabled;
};

