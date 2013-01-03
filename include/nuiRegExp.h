#pragma once

// From http://www.codeguru.com/cpp/cpp/string/regex/article.php/c2779/

class nglString;
class regexp;

class nuiRegExp
{
public:
  enum { NSUBEXP = 10 };

  nuiRegExp();
  nuiRegExp(const nglChar* exp, bool iCase = false);
  nuiRegExp(const nglString& exp, bool iCase = false);
  nuiRegExp(const nuiRegExp &r );
  ~nuiRegExp();
  const nuiRegExp & operator=(const nuiRegExp& r);

  bool Match(const nglChar* s);
  bool Match(const nglString& rString);
  int SubStrings() const;
  
  const nglString operator[](uint32 i) const;
  int SubStart(uint32 i) const;
  int SubLength(uint32 i) const;

  nglString GetReplaceString(const nglChar* source) const;

  nglString GetErrorString() const;
  bool CompiledOK() const;

  const nglString& GetExpression() const; ///< Return the text of the regular expression code
  
#if defined( _RE_DEBUG )
  void Dump();
#endif
private:
  nglString mExpression;
  const nglChar* mpString; /* used to return substring offsets only */
  nglString mString; /* used to return substring offsets only */
  mutable nglString m_szError;
  regexp * rc;

  void ClearErrorString() const;
  int safeIndex( uint32 i ) const;

};

