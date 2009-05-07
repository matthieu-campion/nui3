/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
*/

class nuiParser
{
public:
  nuiParser(nglIStream* pStream, const nglPath& rSourcePath);

  bool IsDone() const; ///< Returns true if all the source data has been consumed
  bool PeekChar(); ///< Read the next char but don't advance the read index.
  bool NextChar(); ///< Read the next char and advance the read index.
  nglChar GetChar() const; ///< Get the current char.

  bool SkipBlank(); ///< Advance the read pointer until the current char is not a blank.  Returns false is there is a problem.
  bool SkipToNextLine(); ///< Advance the read pointer until the start of the next line.  Returns false is there is a problem.
  bool GetQuoted(nglString& rResult); ///< Read a Quoted string. Returns false is there is a problem.
  bool GetSymbol(nglString& rResult); ///< Read a symbol. Returns false is there is a problem.
  bool GetValue(nglString& rResult, bool AllowBlank = false); ///< Read a value. Returns false is there is a problem.
  bool GetFloat(float& rResult); ///< Read a float. Returns false is there is a problem.
  bool GetFloat(double& rResult); ///< Read a float. Returns false is there is a problem.
  bool GetInteger(uint8&  rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(uint16& rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(uint32& rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(uint64& rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(int8&   rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(int16&  rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(int32&  rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  bool GetInteger(int64&  rResult, uint8 Base = 10); ///< Read an integer. Returns false is there is a problem.
  
  void SetError(const nglString& rError); ///< Change the error message.
  int32 GetLine() const; ///< Return the current line number
  int32 GetColumn() const; ///< Return the current column.
  const nglString& GetErrorStr() const; ///< Return the error string stored in the parser.
  
  void SetValidInSymbolStart(const nglString& rValidChars); ///< Set the characters that are valid as the first character of a symbol (usualy all latin letters + underscore).
  void SetValidInSymbol(const nglString& rValidChars); ///< Set the characters that are valid in a symbol (usualy all latin characters + underscore + latin numbers).
  void SetValidInValue(const nglString& rValidChars); ///< Set the characters that are valid in a value.
  void SetValidInBlank(const nglString& rValidChars); ///< Set the characters that are considered blanks.

  bool IsValidInSymbolStart(nglChar c) const; ///< Returns true if the given char is valid as a symbol start
  bool IsValidInSymbol(nglChar c) const; ///< Returns true if the given char is valid in a symbol
  bool IsValidInValue(nglChar c) const; // Returns true is the given char is valid in a value
  bool IsBlank(nglChar c) const; ///< Returns true if the given char is a blank.
  bool IsNumberDigit(nglChar c, uint32 Base = 10) const; ///< Returns true if the given char is a valid number digit for the given base.
  bool GetNumberDigit(uint8& res, nglChar c, uint32 Base) const; ///< Returns true if the given char is a valid number digit for the given base. In this case res contains the converted digit as a number.

protected:
  nglIStream* mpStream;
  nglPath mSourcePath;
  nglChar mChar;
  int32 mColumn;
  int32 mLine;
  bool mEnableCppComments;
  bool mError;
  nglString mErrorString;
  std::set<nglChar> mValidInSymbolStart;
  std::set<nglChar> mValidInSymbol;
  std::set<nglChar> mValidInValue;
  std::set<nglChar> mLineCommentStarters;
  std::set<nglChar> mBlanks;
};
