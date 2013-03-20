/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


///////////////////////////////
nuiCSSAction_SetAttribute::nuiCSSAction_SetAttribute(const nglString& rAttribute, const nglString& rValue, int32 i0, int32 i1)
{
  mAttribute = rAttribute;
  mValue = rValue;
  mValueIsGlobal = rValue[0] == '$';
  if (mValueIsGlobal)
    mValue.DeleteLeft(1);
  mIndex0 = i0;
  mIndex1 = i1;
}

nuiCSSAction_SetAttribute::~nuiCSSAction_SetAttribute()
{
  
}

void nuiCSSAction_SetAttribute::ApplyAction(nuiObject* pObject)
{
  //NGL_OUT(_T("CSS Action on class %s attrib[%s] <- '%s'\n"), pObject->GetObjectClass().GetChars(), mAttribute.GetChars(), mValue.GetChars());
  
  nuiAttribBase Attribute = pObject->GetAttribute(mAttribute);  
  if (Attribute.IsValid())
  {
    nglString v;
    if (!mValueIsGlobal)
      v = mValue;
    else
      v = nuiObject::GetGlobalProperty(mValue);

    if (mIndex0 < 0)
      Attribute.FromString(v);
    else if (mIndex1 < 0)
      Attribute.FromString(mIndex0, v);
    else
      Attribute.FromString(mIndex0, mIndex1, v);
  }
  else
  {
    pObject->SetProperty(mAttribute, mValue);
  }
}

/*
 Target syntax for the nui3 CSS language:
 Doc = Decl*
 Decl = Matchers { Actions }
 Matchers = [Matcher/]*Matcher
 Matcher = [ClassName, "InstanceName", *](StateDesc, PropertyDesc)
 Action = UserRect, UserSize, UserPos, Font, Border, OverDraw, Decoration, Animation, Color
*/
// Lexer:
class cssLexer
{
public:
  cssLexer(nglIStream* pStream, nuiCSS& rCSS, const nglPath& rSourcePath)
  : mrCSS(rCSS)
  {
    mpStream = pStream;
    mSourcePath = rSourcePath;
    if (mSourcePath.IsLeaf())
      mSourcePath = mSourcePath.GetParent();
    mChar = _T(' ');
    mColumn = 1;
    mLine = 1;
  }
  
  virtual ~cssLexer()
  {
  }

  int32 GetLine() const
  {
    return mLine;
  }
  
  int32 GetColumn() const
  {
    return mColumn;
  }
  
  const nglString& GetErrorStr() const
  {
    return mErrorString;
  }
  
  bool Load()
  {
    uint8 start[16];
    if (mpStream->Peek(start, 10, 1))
    {
      // Skip the unicode BOM:
      //    Bytes    Encoding Form
      //      00 00 FE FF   UTF-32, big-endian
      //      FF FE 00 00   UTF-32, little-endian
      //      FF FE   UTF-16, little-endian
      //      FE FF   UTF-16, big-endian
      //      EF BB BF   UTF-8
      if (start[0] == 0x00)
      {
        if (mpStream->ReadUInt8() != 0x00) return false;
        if (mpStream->ReadUInt8() != 0x00) return false;
        if (mpStream->ReadUInt8() != 0xFE) return false;
        if (mpStream->ReadUInt8() != 0xFF) return false;
        // big-endian UTF-32
      }
      else if (start[0] == 0xFF)
      {
        if (mpStream->ReadUInt8() != 0xFF) return false;
        if (mpStream->ReadUInt8() != 0xFE) return false;
        if (start[2] == 0x00)
        {
          if (mpStream->ReadUInt8() != 0x00) return false;
          if (mpStream->ReadUInt8() != 0x00) return false;
          // little endian UTF-32
        }
        else
        {
          // little endian UTF-16
        }
      }
      else if (start[0] == 0xFE)
      {
        if (mpStream->ReadUInt8() != 0xFE) return false;
        if (mpStream->ReadUInt8() != 0xFF) return false;
        // big endian UTF-16
      }
      else if (start[0] == 0xEF)
      {
        if (mpStream->ReadUInt8() != 0xEF) return false;
        if (mpStream->ReadUInt8() != 0xBB) return false;
        if (mpStream->ReadUInt8() != 0xBF) return false;
        // UTF-8
      }
    }

    while (true)
    {
      if (!SkipBlank())
        return true;
      if (!ReadDesc())
        return !mError;
    }
    
    return !mError;
  }

  
  bool PeekChar()
  {
    nglFileOffset ofs = mpStream->GetPos();
    bool res = GetChar();
    mpStream->SetPos(ofs);
    return res;
  }
  
  bool GetChar()
  {
    nglChar previous = mChar;
    // Parse an utf-8 char sequence:
    uint8 c = 0;
    if (1 != mpStream->ReadUInt8(&c, 1))
    {
      mChar = 0;
      return false;
    }
    
    if (!(c & 0x80))
    {
      mChar = c;
    }
    else
    {
      //  0xC0 // 2 bytes
      //  0xE0 // 3
      //  0xF0 // 4
      //  0xF8 // 5
      //  0xFC // 6
      uint32 count = 0;
      if ((c & 0xFC) == 0xFC)
      {
        mChar = c & (~0xFC);
        count = 5;
      }
      else if ((c & 0xF8) == 0xF8)
      {
        mChar = c & (~0xF8);
        count = 4;
      }
      else if ((c & 0xF0) == 0xF0)
      {
        mChar = c & (~0xF0);
        count = 3;
      }
      else if ((c & 0xE0) == 0xE0)
      {
        mChar = c & (~0xE0);
        count = 2;
      }
      else if ((c & 0xC0) == 0xC0)
      {
        mChar = c & (~0xC0);
        count = 1;
      }
      
      for (uint32 i = 0; i < count; i++)
      {
        if (1 != mpStream->ReadUInt8(&c, 1))
          return false;
        mChar <<= 6;
        mChar |= c & 0x3F;
      }
    }
    
    if ((mChar == 0xa && previous != 0xd) || (mChar == 0xd && previous != 0xa) )
    {
      mColumn = 1;
      mLine++;
    }

    //NGL_OUT(_T("%lc"), mChar);
    
    return true;
  }
  
  inline bool IsBlank(nglChar c)
  {
    return (c==' ' || c == 0xa || c == 0xd || c == '\t');
  }
  
  bool SkipBlank()
  {
    bool res = true;
    while ((IsBlank(mChar) || mChar == '/') && res) 
    {
      if (mChar == '/') // Is this a comment?
      {
        res = PeekChar();
        
        if (!res)
          return false;
        
        if (mChar == '/')
        {
          res = GetChar();
          // Skip to the end of the line:
          res = GetChar();
          while (res && mChar != 0xa && mChar != 0xd)
          {
            res = GetChar();
          }
        }
      }
      
      res = GetChar();
    }
    return res;
  }
  
  bool GetQuoted(nglString& rResult)
  {
    mAccumulator.clear();
    if (!SkipBlank())
    {
      rResult.Nullify();
      return false;
    }
    
    if (mChar != _T('\"'))
    {
      rResult.Nullify();
      return false;
    }
    
    bool skip = false;
    if (!GetChar())
    {
      rResult.Copy(&mAccumulator[0], mAccumulator.size());
      return false;
    }

    while (mChar != _T('\"'))
    {
      if (mChar == _T('\\'))
      {
        if (!GetChar())
        {
          rResult.Copy(&mAccumulator[0], mAccumulator.size());
          return false;
        }
        
        if (mChar == _T('u'))
        {
          if (!GetChar())
          {
            rResult.Copy(&mAccumulator[0], mAccumulator.size());
            return false;
          }
          
          nglString hex;
          while (nglIsHexDigit(mChar))
          {
            hex += mChar;

            if (!GetChar())
            {
              rResult.Copy(&mAccumulator[0], mAccumulator.size());
              return false;
            }
          }
          
          nglUChar hexnum = hex.GetCInt(16);
          hex.Wipe();
          rResult.Append(hexnum);
          skip = true;
        }
        else if (mChar != _T('\"'))
        {
          rResult.Copy(&mAccumulator[0], mAccumulator.size());
          return false;
        }
        
      }

      if (!skip)
      {
        mAccumulator.push_back(mChar);
        if (!GetChar())
        {
          rResult.Copy(&mAccumulator[0], mAccumulator.size());
          return false;
        }
      }
    }

    GetChar();
    rResult.Copy(&mAccumulator[0], mAccumulator.size());
    return true;
  }
  
  bool GetSymbol(nglString& rResult)
  {
    mAccumulator.clear();
    if (!SkipBlank())
    {
      rResult.Nullify();
      return false;
    }
    
    while (IsValidInSymbol(mChar))
    {
      mAccumulator.push_back(mChar);
      GetChar();
    }

    if (!mAccumulator.empty())
      rResult.Copy(&mAccumulator[0], mAccumulator.size());
    return !rResult.IsEmpty();
  }
  
  bool GetValue(nglString& rResult, bool AllowBlank = false)
  {
    mAccumulator.clear();
    if (!SkipBlank())
    {
      rResult.Nullify();
      return false;
    }
    
    while ((AllowBlank && IsBlank(mChar)) || IsValidInValue(mChar))
    {
      mAccumulator.push_back(mChar);
      if (!GetChar())
      {
        rResult.Copy(&mAccumulator[0], mAccumulator.size());
        return true;
      }
    }
    
    rResult.Copy(&mAccumulator[0], mAccumulator.size());
    return true;
  }

  bool GetIntValue(nglString& rResult)
  {
    mAccumulator.clear();
    if (!SkipBlank())
    {
      rResult.Nullify();
      return false;
    }
    
    while (mChar == _T('-') || (mChar >= _T('0') && mChar <= _T('9') ))
    {
      mAccumulator.push_back(mChar);
      if (!GetChar())
      {
        rResult.Copy(&mAccumulator[0], mAccumulator.size());
        return true;
      }
    }
    
    rResult.Copy(&mAccumulator[0], mAccumulator.size());
    return true;
  }
  
  
  nglIStream* mpStream;
  nglPath mSourcePath;
  nglChar mChar;
  int32 mColumn;
  int32 mLine;
  bool mError;
  nuiCSS& mrCSS;
  nglString mErrorString;
  
  void SetError(const nglString& rError)
  {
    mErrorString = rError;
    mError = true;
  }
  
  bool IsValidInSymbol(nglChar c) const
  {
    switch (c)
    {
      case _T('/'):
      case _T('='):
      case _T('!'):
      case _T('@'):
      case _T('#'):
      case _T('$'):
      case _T('%'):
      case _T('<'):
      case _T('>'):
      case _T('*'):
      case _T('?'):
      case _T('+'):
      case _T('-'):
      case _T('&'):
      case _T('~'):
      case _T(':'):
      case _T(';'):
      case _T('|'):
      case _T('['):
      case _T(']'):
      case _T('{'):
      case _T('}'):
      case _T('\\'):
      case _T('('):
      case _T(')'):
      case _T('.'):
      case _T(','):
      case _T('\n'):
      case _T('\r'):
      case _T('\t'):
      case _T(' '):
        return false;
        break;
      default:  
        break;
    }
    
    return true;
  }
  
  bool IsValidInValue(nglChar c) const
  {
    switch (c)
    {
      case _T('!'):
      case _T('@'):
      case _T('#'):
      case _T('$'):
      case _T('%'):
      case _T('<'):
      case _T('>'):
      case _T('*'):
      case _T('?'):
      case _T('+'):
      case _T('-'):
      case _T('&'):
      case _T('~'):
      case _T('|'):
      case _T('['):
      case _T(']'):
      case _T('{'):
      case _T('}'):
      case _T('\\'):
      case _T('('):
      case _T(')'):
      case _T('.'):
      case _T(','):
        return true;
        break;
      default:  
        break;
    }
    
    return IsValidInSymbol(c);
  }
  
  void Clear()
  {
    std::vector<nuiWidgetMatcher*>::iterator mit = mMatchers.begin();
    std::vector<nuiWidgetMatcher*>::iterator mend = mMatchers.end();
    
    while (mit != mend)
    {
      delete *mit;
      ++mit;
    }
    
    std::vector<nuiCSSAction*>::iterator ait = mActions.begin();
    std::vector<nuiCSSAction*>::iterator aend = mActions.end();
    
    while (ait != aend)
    {
      delete *ait;
      ++ait;
    }
    
    mMatchers.clear();
    mActions.clear();
  }

  void ApplyActionsToObject(nuiObject* pObj)
  {
    // Apply the actions to the object:
    int32 i = 0;
    for (i = 0; i < (int32)mActions.size(); i++)
    {
      nuiCSSAction* pAction = mActions[i];
      pAction->ApplyAction(pObj);
    }
  }
  
  nuiObject* CreateObject(const nglString& rType, const nglString& rName)
  {
    nuiObject* pObj = mrCSS.CreateObject(rType, rName);

    if (!pObj && (rType == _T("nuiFont") || rType.Compare(_T("font"), false) == 0))
    {
      nuiFontRequest fontrequest;
      ApplyActionsToObject(&fontrequest);
      nuiFont* pFont = nuiFontManager::GetManager().GetFont(fontrequest, rName);
      Clear();
      return pFont;
    }
    
    if (!pObj)
    {
      Clear();
      return NULL;
    }
    
    // Apply the actions to the object:
    int32 i = 0;
    for (i = 0; i < (int32)mActions.size(); i++)
    {
      nuiCSSAction* pAction = mActions[i];
      pAction->ApplyAction(pObj);
    }
        
    Clear();
    return pObj;
  }
  
  
  bool CreateColor(const nglString& rName)
  {
    if (!GetChar())
      return false;
    if (!SkipBlank())
      return false;
    
    nglString colorCode;
    if (!GetValue(colorCode, true/*AllowBlank*/))
      return false;
    
    if (!SkipBlank())
      return false;
    if (mChar != _T(';'))
      return false;
    if (!GetChar())
      return false;

    nuiColor::SetColor(rName, nuiColor(colorCode));
    return true;  
  }
  
  bool CreateVariable(const nglString& rName)
  {
    if (!GetChar())
      return false;
    if (!SkipBlank())
      return false;
    
    nglString value;
    if (!GetValue(value, true/*AllowBlank*/))
      return false;
    
    if (!SkipBlank())
      return false;
    if (mChar != _T(';'))
      return false;
    if (!GetChar())
      return false;

    nuiObject::SetGlobalProperty(rName, value);
    return true;  
  }
  
  bool ReadTextureAtlas(const nglString& rName)
  {
    if (!GetChar()) // eat =
      return false;
    if (!SkipBlank())
      return false;

    if (mChar != '{')
      return false;
    if (!GetChar())
      return false;
    if (!SkipBlank())
      return false;
    
    uint32 count = 0;
    nglString AtlasPath;
    nglString TextureName;
    nuiRect Rect;
    bool Rotated = false;
    bool AutoTrim = false;
    int32 MaxTextureSize = 128;
    int32 AtlasSize = 1024;
    
    while (mChar != _T('}'))
    {
      nglString symbol;
      if (!GetSymbol(symbol))
        return false;
      
      if (!SkipBlank())
        return false;

      if (mChar != ':')
        return false;
      if (!GetChar())
        return false;
      if (!SkipBlank())
        return false;

      
      nglString value;
      if (mChar == _T('"'))
      {
        if (!GetQuoted(value))
          return false;
      }
      else
      {
        if (!GetValue(value, true/*AllowBlank*/))
          return false;
      }
      
      if (!SkipBlank())
        return false;
      
      if (mChar != _T(';'))
        return false;
      if (!GetChar())
        return false;
      if (!SkipBlank())
        return false;
      
      if (symbol == _T("Path"))
      {
        AtlasPath = value;
      }
      else if (symbol == _T("TextureName"))
      {
        if (count)
        {
          nuiTexture::CreateTextureProxy(TextureName, AtlasPath, Rect, Rotated);
        }

        TextureName = value;
        count++;
      }
      else if (symbol == _T("Alias"))
      {
        if (count)
        {
          nuiTexture::CreateTextureProxy(value, AtlasPath, Rect, Rotated);
        }
      }
      else if (symbol == _T("Rect"))
      {
        Rect.SetValue(value);
      }
      else if (symbol == _T("Rotated"))
      {
        Rotated = nuiGetBool(value, false);
      }
      else if (symbol == _T("AutoTrim"))
      {
        AutoTrim = nuiGetBool(value, false);
      }
      else if (symbol == _T("MaxTextureSize"))
      {
        MaxTextureSize = value.GetCInt();
      }
      else if (symbol == _T("Size"))
      {
        AtlasSize = value.GetCInt();
      }
      else if (symbol == _T("AutoScan"))
      {
        nuiTexture::CreateAtlasFromPath(value, MaxTextureSize, AtlasSize, AutoTrim);
      }
    }

    if (count)
    {
      nuiTexture::CreateTextureProxy(TextureName, AtlasPath, Rect, Rotated);
    }
    
    if (!SkipBlank())
      return false;
    if (mChar != _T('}'))
      return false;
    if (!GetChar())
      return false;
    
    return true;  
  }
  
  
  bool ReadDesc()
  {
    if (!SkipBlank())
      return false;
    
    mMatchers.clear();
    mActions.clear();
    
    
    //******************************************
    //
    // look for the "#include" directive
    //       
    // 
    if (mChar == _T('#'))
    {
      bool res = GetChar();
      if (!res)
        return false;
      
      nglString type;
      if (!GetSymbol(type))
        return false;
      if (type.Compare(_T("include")))
        return false;
      
      if (!SkipBlank())
        return false;

      // retrieve quoted filename
      nglString name;
      if (!GetQuoted(name))
        return false;
      
      name.Trim(_T('\"'));
      
      if (name.IsEmpty())
      {
        //NGL_OUT(_T("a css included filename is empty!\n"));
        return false;
      }
      
      nglPath filepath(name);
      nglPath includePath;
        
      // check filename
      if (!filepath.IsAbsolute())
        includePath = mSourcePath + nglPath(name);
      else
        includePath = filepath;
      
      // check file
      if (!includePath.Exists())
      {
        NGL_OUT(_T("Could not find CSS source file '%s'\n"), includePath.GetChars());
        return false;
      }
      
      // open the file
      nglIStream* pF = includePath.OpenRead();
      if (!pF)
      {
        NGL_OUT(_T("Unable to open CSS source file '%s'\n"), includePath.GetChars());
        return false;
      }

      //NGL_OUT(_T("CSS Include: '%s'\n"), includePath.GetChars());
      // launch included file parsing
      nglFileOffset s = pF->Available();
      std::vector<uint8> cache;
      cache.resize(s);
      pF->Read(&cache[0], s, 1);
      delete pF;
      nglIMemory mem(&cache[0], s);
      
      uint32 c = mrCSS.GetRulesCount();
      cssLexer lexer(&mem, mrCSS, includePath);
      if (!lexer.Load())
      {
        nglString tmp;
        tmp.CFormat(_T("Error (file '%s') line %d (%d): %s"), includePath.GetChars(), lexer.GetLine(), lexer.GetColumn(), lexer.GetErrorStr().GetChars() );
        SetError(tmp);
        return false;
      }
      uint32 cc = mrCSS.GetRulesCount();
      //NGL_OUT(_T("\tNew css rules: %d\n"), cc - c);
      return true;
    }
    else if (mChar == _T('+'))
    {
      // Create a widget creator
      // Eat the +
      bool res = GetChar();
      if (!res)
        return false;
      
      nuiWidgetCreator* pCreator = ReadWidgetCreator();
      if (!pCreator)
        return false;
      nuiBuilder::Get().SetHandler(pCreator->GetObjectName(), pCreator);
      return true;
    }
    else if (mChar == _T('@'))
    {
      //******************************************
      //
      // Read a resource creator
      //    
      return ReadResourceCreator();
    }
    else if (mChar == _T('$'))
    {
      //******************************************
      //
      // Read a Global Variable
      //    
      return ReadGlobalVariable();
    }
    else
    {
      nuiCSSRule* pRule = ReadRule();
      if (pRule)
      {
        mrCSS.AddRule(pRule);
        return true;
      }

      return false;
    }
  }

  nuiCSSRule* ReadRule()
  {
    if (!ReadMatchers(mMatchers, _T('{')))
      return NULL;
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      Clear();
      return NULL;
    }
    
    bool res = ReadActionList();
    
    // Create the rule from the matchers and the actions:
    nuiCSSRule* pRule = new nuiCSSRule();
    int32 i = (int32)mMatchers.size();
    for (; i > 0; i--)
      pRule->AddMatcher(mMatchers[i-1]);
    for (i = 0; i < (int32)mActions.size(); i++)
      pRule->AddAction(mActions[i]);
        
    mMatchers.clear();
    mActions.clear();
    return pRule;
  }
  
  
  bool ReadGlobalVariable()
  {
    // Eat the global variable $
    if (!GetChar())
    {
      SetError(_T("unexpected end of file"));
      return false;
    }
    
    nglString variable;
    if (!GetSymbol(variable))
    {
      SetError(_T("expected a symbol"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return false;        
    }
    
    if (mChar != _T('='))
    {
      SetError(_T("expected ="));
      return false;        
    }
    
    if (!GetChar())
    {
      SetError(_T("unexpected end of file"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return false;        
    }
    
    bool res = false;
    nglString rvalue;
    if (mChar == _T('"'))
    {
      res = GetQuoted(rvalue);
    }
    else
    {
      res = GetValue(rvalue, true);
    }
    
    if (!res)
    {
      SetError(_T("Error while looking for symbol or string action"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return false;        
    }
    
    if (mChar != _T(';'))
    {
      SetError(_T("expected ';'"));
      return false;        
    }
    
    if (!GetChar())
    {
      SetError(_T("unexpected end of file"));
      return false;
    }
    
    nuiObject::SetGlobalProperty(variable, rvalue);
    
    return true;
  }
  
  nuiObject* ReadObject(const nglString& rName = nglString::Null)
  {
    // Read Type:
    nglString type;
    if (!GetSymbol(type))
    {
      SetError(_T("expected a symbol"));
      return NULL;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return NULL;
    }
    
    nglString name;
    
    if (mChar == '{' && !rName.IsNull())
    {
      name = rName;
    }
    else if (!GetSymbol(name))
    {
      SetError(_T("expected a symbol"));
      return NULL;
    }
    else
    {
      if (!rName.IsNull())
        name = rName;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return NULL;
    }
    
    // special case. create color
    if (mChar == _T('='))
    {
      if ((type == _T("nuiColor")) || !type.Compare(_T("color"), false))
      {
        if (!CreateColor(name))
        {
          nglString str;
          str.CFormat(_T("Unable to parse a color with name '%s'"), name.GetChars());
          SetError(str);
          return NULL;
        }
      }
      else if (!type.Compare(_T("var"), false))
      {
        if (!CreateVariable(name))
        {
          nglString str;
          str.CFormat(_T("Unable to parse a variable with name '%s'"), name.GetChars());
          SetError(str);
          return NULL;
        }
      }
      else if (!type.Compare(_T("textureatlas"), false) || !type.Compare(_T("atlas"), false))
      {
        if (!ReadTextureAtlas(name))
        {
          nglString str;
          str.CFormat(_T("Unable to parse an atlas definition for %s"), name.GetChars());
          SetError(str);
          return NULL;
        }
      }
      
      return (nuiObject*)-1;
    }
    
    // create an object
    if (mChar != _T('{'))
    {
      SetError(_T("'{' expected"));
      return NULL;
    }
    
    if (!ReadActionList())
    {
      Clear();
      return NULL;
    }
    
    // Create the object from the type and the name
    nuiObject* pObj = CreateObject(type, name);
    if (!pObj)
    {
      nglString str;
      str.CFormat(_T("Unable to create an object of type '%s' and name '%s'"), type.GetChars(), name.GetChars());
      SetError(str);
      return NULL;
    }
    
    // Clear all
    mMatchers.clear();
    mActions.clear();
    
    return pObj;
  }

  bool ReadResourceCreator()
  {
    // Eat the creator @
    bool res = GetChar();
    if (!res)
      return false;

    nuiObject* pObject = ReadObject();
    
    return NULL != pObject;
  }
  
  bool ReadDictionary(std::map<nglString, nglString>& rDict)
  {
    do
    {
      if (!GetChar())
      {
        SetError(_T("Unexpected end of file"));
        return false;
      }
      
      if (!SkipBlank())
      {
        SetError(_T("Unexpected end of file"));
        return false;
      }      
      
      // Read Symbol
      nglString key;
      if (!GetSymbol(key))
      {
        SetError(_T("error while looking for a symbol"));
        return false;
      }
      
      if (!SkipBlank())
      {
        SetError(_T("Unexpected end of file"));
        return false;
      }
      
      // Read ':'
      if (mChar != _T(':'))
      {
        SetError(_T("Expected ':'"));
        return false;
      }
      
      if (!GetChar())
      {
        SetError(_T("Unexpected end of file"));
        return false;
      }
      
      if (!SkipBlank())
      {
        SetError(_T("Unexpected end of file"));
        return false;
      }
      
      // Read Value
      bool res = false;
      nglString val;
      if (mChar == _T('"'))
      {
        res = GetQuoted(val);
      }
      else
      {
        res = GetValue(val, true);
      }

      if (!res)
      {
        SetError(_T("error while looking for a value"));
        return false;
      }
      
      // SkipBlank
      if (!SkipBlank())
      {
        SetError(_T("expected ')' or ';'"));
        return false;
      }
      
      rDict[key] = val;
      
      if (mChar == _T(')'))
        break;
    } while (mChar == _T(';') || mChar == _T(','));
      
    if (!GetChar() && !SkipBlank())
    {
      SetError(_T("Unexpected end of file"));
      return false;
    }
    
    return true;
  }
  
  nuiWidgetCreator* ReadWidgetCreator(uint32 level = 0)
  {
    bool res = true;
    nglString type;
    nglString name;
    std::map<nglString, nglString> dict;
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return NULL;        
    }
    
    if (!GetSymbol(type))
    {
      SetError(_T("expected a widget type"));
      return NULL;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return NULL;        
    }

    // create an object if the declaration is empty
    if (mChar == _T(';'))
    {
      if (!level)
        return NULL;
      if (!GetChar())
        return NULL;
      nuiWidgetCreator* pCreator = new nuiWidgetCreator(type, name);
      pCreator->SetDefaultDictionary(dict);
      return pCreator;
    }

    // Try to read an optional name (the name is not optionnal for the root of the tree)
    if (mChar != _T('{'))
    {
      if (!GetSymbol(name))
      {
        if (mChar == _T('(')) // We couldn't read a symbol, try a param list
        {
          if (!ReadDictionary(dict))
            return NULL;
        }
        else
        {
          SetError(_T("syntax error while looking for a symbol or '('"));
          return NULL;        
        }
      }
      else
      {
        if (!SkipBlank())
        {
          SetError(_T("unexpected end of file"));
          return NULL;        
        }

        if (mChar == _T('(')) // We couldn't read a symbol, try a param list
        {
          if (!ReadDictionary(dict))
            return NULL;
        }
      }

      if (!SkipBlank())
      {
        SetError(_T("unexpected end of file"));
        return NULL;        
      }
    }
    else if (!level)
    {
      // The root widget creator MUST have a name
      SetError(_T("The root widget creator MUST have a name"));
      return NULL;
    }
    
    // create an object if the declaration is empty (but with a name)
    if (mChar == _T(';'))
    {
      if (!level)
        return NULL;
      if (!GetChar())
        return NULL;
      nuiWidgetCreator* pCreator = new nuiWidgetCreator(type, name);
      pCreator->SetDefaultDictionary(dict);
      return pCreator;
    }

    if (mChar != _T('{'))
    {
      SetError(_T("'{' expected"));
      return NULL;        
    }

    // Skip {
    if (!GetChar() || !SkipBlank())
    {
      SetError(_T("unexpected end of file while looking for }"));
      return NULL;
    }

    nuiWidgetCreator* pCreator = new nuiWidgetCreator(type, name);
    pCreator->SetDefaultDictionary(dict);

    while (mChar != _T('}'))
    {
      if (!SkipBlank())
      {
        SetError(_T("unexpected end of file"));
        delete pCreator;
        return NULL;        
      }
      
      if (mChar == _T('+'))
      {
        if (!GetChar())
        {
          SetError(_T("expecting [ or a symbol"));
          delete pCreator;
          return NULL;
        }
        
        if (!SkipBlank())
        {
          SetError(_T("unexpected end of file"));
          delete pCreator;
          return NULL;        
        }
        
        uint32 nparams = 0;
        uint32 param1 = 0;
        uint32 param2 = 0;
        
        if (mChar == _T('['))
        {
          nparams = 1;
          
          // Read first param
          nglString val;
          GetIntValue(val);
          param1 = val.GetCInt();

          // Skip blank
          if (!SkipBlank())
          {
            SetError(_T("unexpected end of file while looking for indices"));
            delete pCreator;
            return NULL;        
          }
          
          if (mChar != _T(']'))
          {
            // Read second param
            GetIntValue(val);
            param2 = val.GetCInt();
            
            // Skip blank
            if (!SkipBlank())
            {
              SetError(_T("unexpected end of file while looking for ]"));
              delete pCreator;
              return NULL;        
            }
          }
          
          // Eat ]
          if (!GetChar())
          {
            SetError(_T("expecting ]"));
            delete pCreator;
            return NULL;
          }
          
          // SkipBlank
          if (!SkipBlank())
          {
            SetError(_T("unexpected end of file while looking for ]"));
            delete pCreator;
            return NULL;        
          }
        }

        nuiWidgetCreator* pChild = ReadWidgetCreator(level + 1);
        if (!pChild)
        {
          delete pCreator;
          return false;
        }

        if (nparams == 0)
          pCreator->AddChild(pChild);
        else if (nparams == 1)
          pCreator->SetCell(param1, pChild);
        else if (nparams == 2)
          pCreator->SetCell(param1, param2, pChild);
        
        if (!SkipBlank())
        {
          SetError(_T("unexpected end of file"));
          delete pCreator;
          return NULL;        
        }
      }
      else
      {
        nglString LValue;
        nglString RValue;
        nglChar Operator;
        int32 i0;
        int32 i1;
        res = ReadAction(LValue, RValue, Operator, i0, i1);
        if (!res)
        {
          SetError(_T("error while reading an assignment"));
          delete pCreator;
          return NULL;
        }
        
        if (Operator == _T(':')) // attribute assignment
        {
          pCreator->SetAttribute(LValue, RValue, i0, i1);
        }
        else if (Operator == _T('=')) // property assignment
        {
          pCreator->SetProperty(LValue, RValue);
        }

        if (!SkipBlank())
        {
          SetError(_T("unexpected end of file"));
          delete pCreator;
          return NULL;        
        }
        
      }

    }
    
    if (!GetChar()) // Eat the }
    {
      SetError(_T("Missing widget creator declaration end"));
      delete pCreator;
      return NULL;
    }
    
    return pCreator;
  }
  
  bool ReadMatchers(std::vector<nuiWidgetMatcher*>& rMatchers, nglChar EndChar)
  {
    //******************************************
    //
    // Read a matcher
    //        
    do
    {
      nuiWidgetMatcher* pMatcher = ReadMatcher();
      if (!pMatcher)
      {
        Clear();
        return false;
      }
      else
      {
        int32 i = (int32)rMatchers.size();
        uint32 priority = pMatcher->GetPriority();
        if (i && priority)
        {
          uint32 testpriority = 0;
          do
          {
            i--;
            testpriority = rMatchers[i]->GetPriority();
          } while ( (i > 0) && testpriority && (priority < testpriority) );
          if (!testpriority)
            i++;
        }
        
        rMatchers.insert(rMatchers.begin() + i, pMatcher);
      }
      
      if (!SkipBlank())
        return false;
    }
    while (mChar != EndChar);
    
    return true;
  }
  
  nuiWidgetMatcher* ReadMatcher()
  {
    nuiWidgetMatcher* pMatcher = NULL;
    
    if (!SkipBlank())
      return NULL;
    
    if (mChar == '"')
    {
      // Read a quoted name
      nglString str;
      if (!GetQuoted(str))
      {
        SetError(_T("Error while looking for a quoted string"));
        return NULL;
      }
      
      pMatcher = new nuiWidgetNameMatcher(str);
    }
    else if (mChar == '$')
    {
      // Read a global variable name
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a global variable name"));
        return NULL;
      }
      
      nglString variable;
      if (!GetSymbol(variable))
      {
        SetError(_T("Error while looking for a global variable name"));
        return NULL;
      }
        
      if (!SkipBlank())
        return NULL;
                
      if (mChar != _T('='))
      {
        SetError(_T("Missing global variable matching operator ('=')"));
        return NULL;
      }
        
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a global variable value"));
        return NULL;
      }
        
      if (!SkipBlank())
        return NULL;
      
      nglString str;
      if (mChar == _T('"'))
      {
        if (!GetQuoted(str))
        {
          SetError(_T("Error while looking for a quoted string"));
          return NULL;
        }
      }
      else
      {
        if (!GetSymbol(str))
        {
          SetError(_T("Error while looking for a match value"));
          return NULL;
        }
      }
        
      if (!SkipBlank())
        return NULL;      

      pMatcher = new nuiGlobalVariableMatcher(variable, str);
        
    }
    else if (mChar == _T('['))
    {
      // Read a symbol (-> property name)
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a property name"));
        return NULL;
      }

      if (!SkipBlank())
        return NULL;
      
      while (mChar != _T(']'))
      {
        nglString property;
        nglChar op = 0;

        if (mChar == _T('$'))
        {
          op = _T('$');
          if (!GetChar())
          {
            SetError(_T("End of file while looking for a global variable name"));
            return NULL;
          }
          
        }
        
        if (!GetSymbol(property))
        {
          SetError(_T("Error while looking for a property name"));
          return NULL;
        }
        
        if (!SkipBlank())
          return NULL;
        
        if (!op)
        {
          if (mChar == _T('='))
            op = mChar;
          if (mChar == _T(':'))
            op = mChar;
        }
        
        if (!op)
        {
          SetError(_T("Missing matching operator ('=' or ':')"));
          return NULL;
        }
        
        if (!GetChar())
        {
          SetError(_T("End of file while looking for a propery value"));
          return NULL;
        }
        
        if (!SkipBlank())
          return NULL;
        
        nglString str;
        if (mChar == _T('"'))
        {
          if (!GetQuoted(str))
          {
            SetError(_T("Error while looking for a quoted string"));
            return NULL;
          }
        }
        else
        {
          if (!GetSymbol(str))
          {
            SetError(_T("Error while looking for a match value"));
            return NULL;
          }
        }
        
        if (op == _T('=') || op == _T(':'))
          pMatcher = new nuiWidgetAttributeMatcher(property, str, true, false);
        else if (op == _T('$'))
          pMatcher = new nuiGlobalVariableMatcher(property, str);
        
        if (!SkipBlank())
          return NULL;
      }
      
      if (!GetChar())
      {
        SetError(_T("End of file while looking for the end of a propery checker"));
        return NULL;
      }
      
    }
    else if (mChar == _T('*'))
    {
      // Pass all wigdets
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a matcher expression"));
        return NULL;
      }
      pMatcher = new nuiWidgetJokerMatcher();
    }
    else if (mChar == _T('.'))
    {
      // Go to parent matcher
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a matcher expression"));
        return NULL;
      }
      pMatcher = new nuiWidgetParentMatcher();
    }
    else if (mChar == _T('('))
    {
      // Find a parent that matches the test
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a matcher expression"));
        return NULL;
      }
      
      if (!SkipBlank())
        return NULL;
      
      std::vector<nuiWidgetMatcher*> Matchers;
      if (!ReadMatchers(Matchers, _T(')')))
        return false;;

      pMatcher = new nuiWidgetParentConditionMatcher(Matchers);

      if (mChar != _T(')'))
      {
        SetError(_T("missing parenthesis at the end of a matcher expression"));
        return NULL;
      }

      if (!GetChar())
      {
        SetError(_T("End of file while looking for a matcher expression"));
        return NULL;
      }
      
      if (!SkipBlank())
        return NULL;
    }
    else if (mChar == _T('!'))
    {
      // Read a static test matcher
      if (!GetChar())
      {
        SetError(_T("End of file while looking for a matcher expression"));
        return NULL;
      }
      pMatcher = new nuiWidgetStaticMatcher();
    }
    else
    {
      // Read a symbol (-> class name)
      nglString str;
      if (!GetSymbol(str))
      {
        SetError(_T("Error while looking for a widget class name"));
        return NULL;
      }
      pMatcher = new nuiWidgetClassMatcher(str);
    }
    
    return pMatcher;
  }
  
  
  bool ReadActionList()
  {
    if (!SkipBlank())
    {
      SetError(_T("Missing action list"));
      return false;
    }
    
    if (mChar != _T('{'))
    {
      SetError(_T("Missing action list"));
      return false;
    }
    
    if (!GetChar())
    {
      SetError(_T("Missing action list or '}'"));
      return false;
    }
    
    while (mChar != _T('}'))
    {
      if (!ReadAction())
      {
        SetError(_T("Expecting an action or '}'"));
        return false;
      }
      if (!SkipBlank())
      {
        SetError(_T("Unexpected end of file while looking for an action or '}'"));
        return false;
      }
    }
    
    if (!GetChar())
    {
      SetError(_T("Missing action list end"));
      return false;
    }
    
    return true;
  }

  bool ReadAction()
  {
    nglChar op = 0;
    nglString symbol;
    nglString rvalue;
    int32 i0;
    int32 i1;
    
    if (!SkipBlank())
    {
      SetError(_T("Missing action"));
      return false;
    }
    
    bool res = GetSymbol(symbol);
    if (res)
    {
      if (!SkipBlank())
      {
        SetError(_T("Missing '{' or action."));
        return false;
      }
      
      if (mChar == _T('{'))
      {
        //Read an action list
      }

    }
    
    if (!ReadAction(symbol, rvalue, op, i0, i1))
      return false;
    
    if (op == ':' ||  op == '=')
    {
      nuiCSSAction_SetAttribute* pAction = new nuiCSSAction_SetAttribute(symbol, rvalue, i0, i1);
      mActions.push_back(pAction);
    }

    return true;
  }

  bool ReadAction(nglString& rLValue, nglString& rRValue, nglChar& rOperator, int32& index0, int32& index1)
  {
    bool res = false;
    index0 = -1;
    index1 = -1;
    
    if (!SkipBlank())
    {
      SetError(_T("Missing action"));
      return false;
    }
    
    nglString symbol(rLValue);
    if (symbol.IsNull())
    {
      res = GetSymbol(symbol);
      if (!res)
      {
        SetError(_T("Missing symbol name (l-value)"));
        return false;
      }
    }

    if (!SkipBlank())
    {
      SetError(_T("Missing action operator ('=')"));
      return false;
    }
    
    if (mChar == _T('['))
    {
      if (!GetChar() || !SkipBlank())
      {
        SetError(_T("End of file while looking for a index ([x])"));
        return false;
      }
 
      if (mChar == _T(','))
      {
        if (!GetChar() || !SkipBlank())
        {
          SetError(_T("End of file while looking for a index ([x])"));
          return false;
        }
      }
      
      nglString idx;
      if (!GetIntValue(idx))
      {
        SetError(_T("Error while looking for a index ([x])"));
        return false;
      }

      index0 = idx.GetCInt();
      
      if (!SkipBlank())
      {
        SetError(_T("End of file while looking for an index or ']'"));
        return false;
      }

      if (mChar != _T(']'))
      {
        nglString idx;
        if (!GetIntValue(idx))
        {
          SetError(_T("Error while looking for an index ([x])"));
          return false;
        }
        
        index1 = idx.GetCInt();
        
        if (!SkipBlank())
        {
          SetError(_T("End of file while looking for ']'"));
          return false;
        }
      }

      if (mChar != _T(']'))
      {
        SetError(_T("Error while looking for ']'"));
        return false;
      }
 
      if (!GetChar() || !SkipBlank())
      {
        SetError(_T("Error while looking for an operator"));
        return false;
      }
      
    }
    
    nglChar op = 0;
    if (mChar == _T('='))
      op = mChar;
    if (mChar == _T(':'))
      op = mChar;
    
    if (!op)
    {
      SetError(_T("Missing action operator ('=' or ':')"));
      return false;
    }
    
    if (!GetChar()) // Eat the equal sign
    {
      SetError(_T("Missing action right side"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("Missing action right side"));
      return false;
    }
    
    nglString rvalue;
    if (mChar == _T('"'))
    {
      res = GetQuoted(rvalue);
    }
    else
    {
      res = GetValue(rvalue, true);
    }
    
    if (!res)
    {
      SetError(_T("Error while looking for symbol or string action"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("Missing action separator (';')"));
      return false;
    }    
    
    if (mChar != _T(';'))
    {
      SetError(_T("Missing semi colon at the end of the action"));
      return false;
    }
    
    if (!GetChar()) // Eat the semi colon
    {
      SetError(_T("Missing semi colon at the end of the action"));
      return false;
    }

    rLValue = symbol;
    rRValue = rvalue;
    rOperator = op;
    return true;
  }
  
  
  std::vector<nuiWidgetMatcher*> mMatchers;
  std::vector<nuiCSSAction*> mActions;
  std::vector<nglChar> mAccumulator;
};




// Class nuiCSSAction
nuiCSSAction::nuiCSSAction()
{
  
}

nuiCSSAction::~nuiCSSAction()
{
  
}


//class nuiCSSRule

nuiCSSRule::nuiCSSRule()
{
  mMatchersTag = 0;
}

nuiCSSRule::~nuiCSSRule()
{
  {
    std::vector<nuiWidgetMatcher*>::iterator it = mMatchers.begin();
    std::vector<nuiWidgetMatcher*>::iterator end = mMatchers.end();
    while (it != end)
    {
      delete *it;
      ++it;
    }
  }

  {
    std::vector<nuiCSSAction*>::iterator it = mActions.begin();
    std::vector<nuiCSSAction*>::iterator end = mActions.end();
    while (it != end)
    {
      delete *it;
      ++it;
    }
  }
}

void nuiCSSRule::AddMatcher(nuiWidgetMatcher* pMatcher)
{
  mMatchers.push_back(pMatcher);
  mMatchersTag |= pMatcher->GetTag();
}

void nuiCSSRule::AddAction(nuiCSSAction* pAction)
{
  mActions.push_back(pAction);
}

bool nuiCSSRule::Match(nuiWidget* pWidget, uint32 MatchersMask)
{
  if (!(MatchersMask & GetMatchersTag()))
    return false;

  std::vector<nuiWidgetMatcher*>::iterator it = mMatchers.begin();
  std::vector<nuiWidgetMatcher*>::iterator end = mMatchers.end();
  while (it != end && pWidget)
  {
    nuiWidgetMatcher* pMatcher = *it;
    if (!pMatcher->Match(pWidget))
      return false;
    ++it;
  }
  return pWidget != NULL;
}

void nuiCSSRule::ApplyRule(nuiWidget* pWidget, uint32 MatchersTag)
{
  if (!Match(pWidget, MatchersTag))
    return;
  if (pWidget)
  {
    std::vector<nuiCSSAction*>::iterator it = mActions.begin();
    std::vector<nuiCSSAction*>::iterator end = mActions.end();
    while (it != end)
    {
      nuiCSSAction* pAction = *it;
      pAction->ApplyAction(pWidget);
      ++it;
    }
  }
}

uint32 nuiCSSRule::GetMatchersTag() const
{
  return mMatchersTag;
}

void nuiCSSRule::ApplyAction(nuiObject* pObject)
{
  nuiWidget* pWidget = dynamic_cast<nuiWidget*> (pObject);
  ApplyRule(pWidget, -1);
}



// class nuiCSS

nuiCSS::nuiCSS()
{
  
}

nuiCSS::~nuiCSS()
{
  std::vector<nuiCSSRule*>::iterator it = mRules.begin();
  std::vector<nuiCSSRule*>::iterator end = mRules.end();
  
  while (it != end)
  {
    delete (*it);
    ++it;
  }
}

bool nuiCSS::Load(nglIStream& rStream, const nglPath& rSourcePath)
{
  mErrorString.Wipe();
  nglFileOffset s = rStream.Available();
  std::vector<uint8> cache;
  cache.resize(s);
  rStream.Read(&cache[0], s, 1);
  nglIMemory mem(&cache[0], s);
  cssLexer lexer(&mem, *this, rSourcePath);
  if (!lexer.Load())
  {
    mErrorString.CFormat(_T("Error line %d (%d): %s"), lexer.GetLine(), lexer.GetColumn(), lexer.GetErrorStr().GetChars() );
    NGL_OUT(_T("Error loading css:\n%s\n"), mErrorString.GetChars());
    return false;
  }
  //NGL_OUT(_T("Loaded %d css rules\n"), GetRulesCount());
  return true;
}




const nglString& nuiCSS::GetErrorString() const
{
  return mErrorString;
}


bool nuiCSS::Serialize(nglOStream& rStream)
{
  return false;
}

void nuiCSS::ApplyRules(nuiWidget* pWidget, uint32 MatchersTag)
{
  int32 count = (int32)mRules.size();
  for (int32 i = 0; i < count; i++)
  {
    nuiCSSRule* pRule = mRules[i];
    pRule->ApplyRule(pWidget, MatchersTag);
  }
  pWidget->IncrementCSSPass();
}

bool nuiCSS::GetMatchingRules(nuiWidget* pWidget, std::vector<nuiCSSRule*>& rMatchingRules, uint32 MatchersTag)
{
  rMatchingRules.clear();
  for (int32 i = 0; i < (int32)mRules.size(); i++)
  {
    if (mRules[i]->Match(pWidget, MatchersTag))
    {
      rMatchingRules.push_back(mRules[i]);
    }
  }
  
  return !rMatchingRules.empty();
 }

void nuiCSS::AddRule(nuiCSSRule* pRule)
{
  mRules.push_back(pRule);
}

nuiObject* nuiCSS::CreateObject(const nglString& rType, const nglString& rName)
{
  return nuiDecoration::CreateDecoration(rType, rName);
}

uint32 nuiCSS::GetRulesCount() const
{
  return mRules.size();
}

const std::vector<nuiCSSRule*> nuiCSS::GetRules() const
{
  return mRules;
}

nuiObject* nuiCSS::CreateObject(const nglString& rDesc)
{
  std::string str(rDesc.GetStdString());
  str.append(" ");
  nglIMemory mem(&str[0], str.size());
  
  nuiCSS css;
  cssLexer lexer(&mem, css, "inline");

  lexer.GetChar();
  lexer.SkipBlank();
  nuiObject* pObj = lexer.ReadObject(rDesc);
  if (pObj == (nuiObject*)-1)
    return NULL;
  return pObj;
}


