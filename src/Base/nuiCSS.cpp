/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiCSS.h"
#include "nglIStream.h"
#include "nglOStream.h"

#include "nuiFrame.h"
#include "nuiMetaDecoration.h"
#include "nuiStateDecoration.h"
#include "nuiBorderDecoration.h"
#include "nuiColorDecoration.h"
#include "nuiGradientDecoration.h"
#include "nuiImageDecoration.h"
#include "nuiTreeHandleDecoration.h"
#include "nuiMetaDecoration.h"
#include "nuiFontManager.h"


///////////////////////////////
nuiCSSAction_SetAttribute::nuiCSSAction_SetAttribute(const nglString& rAttribute, const nglString& rValue)
{
  mAttribute = rAttribute;
  mValue = rValue;
}

nuiCSSAction_SetAttribute::~nuiCSSAction_SetAttribute()
{
  
}

void nuiCSSAction_SetAttribute::ApplyAction(nuiObject* pObject)
{
  //NGL_OUT(_T("CSS Action on class %ls attrib[%ls] <- '%ls'\n"), pObject->GetObjectClass().GetChars(), mAttribute.GetChars(), mValue.GetChars());
  
  nuiAttribBase Attribute = pObject->GetAttribute(mAttribute);  
  if (Attribute.IsValid())
    Attribute.FromString(mValue);
}

/////////////////////////////////
nuiCSSAction_SetProperty::nuiCSSAction_SetProperty(const nglString& rProperty, const nglString& rValue)
{
  mProperty = rProperty;
  mValue = rValue;
}

nuiCSSAction_SetProperty::~nuiCSSAction_SetProperty()
{
  
}

void nuiCSSAction_SetProperty::ApplyAction(nuiObject* pObject)
{
  //NGL_OUT(_T("CSS Action on class %ls proeprty[%ls] <- '%ls'\n"), pWidget->GetObjectClass().GetChars(), mAttribute.GetChars(), mValue.GetChars());
  pObject->SetProperty(mProperty, mValue);
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
    while (true)
    {
      if (!SkipBlank())
        return true;
      if (!ReadDesc())
        return !mError;
    }
    
    return !mError;
  }

  
  
protected:
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
      if (c & 0xFC == 0xFC)
      {
        mChar = c & ~0xFC;
        count = 5;
      }
      else if (c & 0xF8 == 0xF8)
      {
        mChar = c & ~0xF8;
        count = 4;
      }
      else if (c & 0xF0 == 0xF0)
      {
        mChar = c & ~0xF0;
        count = 3;
      }
      else if (c & 0xE0 == 0xE0)
      {
        mChar = c & ~0xE0;
        count = 2;
      }
      else if (c & 0xC0 == 0xC0)
      {
        mChar = c & ~0xC0;
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
    if (!SkipBlank())
      return false;
    
    if (mChar != _T('\"'))
      return false;
    
    while (GetChar() && mChar != _T('\"'))
    {
      if (mChar == _T('\\'))
      {
        if (!GetChar())
          return false;
        
        if (mChar != _T('\"'))
          return false;
        
      }
      
      rResult.Add(mChar);
    }

    GetChar();
    return true;
  }
  
  bool GetSymbol(nglString& rResult)
  {
    rResult.Nullify();
    if (!SkipBlank())
      return false;
    
    while (IsValidInSymbol(mChar))
    {
      rResult.Add(mChar);
      GetChar();
    }
    
    return !rResult.IsEmpty();
  }
  
  bool GetValue(nglString& rResult, bool AllowBlank = false)
  {
    rResult.Nullify();
    if (!SkipBlank())
      return false;
    
    while ((AllowBlank && IsBlank(mChar)) || IsValidInValue(mChar))
    {
      rResult.Add(mChar);
      if (!GetChar())
        return true;
    }
    
    return true;
  }

  bool GetIntValue(nglString& rResult)
  {
    rResult.Nullify();
    if (!SkipBlank())
      return false;
    
    while (mChar == _T('-') || (mChar >= _T('0') && mChar <= _T('9') ))
    {
      rResult.Add(mChar);
      if (!GetChar())
        return true;
    }
    
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
    uint32 i = 0;
    for (i = 0; i < mActions.size(); i++)
    {
      nuiCSSAction* pAction = mActions[i];
      pAction->ApplyAction(pObj);
    }
  }
  
  bool CreateObject(const nglString& rType, const nglString& rName)
  {
    nuiObject* pObj = NULL;
    if (rType == _T("nuiFrame"))
    {
      pObj = new nuiFrame(rName);
    }
    else if (rType == _T("nuiBorderDecoration"))
    {
      pObj = new nuiBorderDecoration(rName);
    }
    else if (rType == _T("nuiColorDecoration"))
    {
      pObj = new nuiColorDecoration(rName);
    }
    else if (rType == _T("nuiImageDecoration"))
    {
      pObj = new nuiImageDecoration(rName);
    }
    else if (rType == _T("nuiGradientDecoration"))
    {
      pObj = new nuiGradientDecoration(rName);
    }
    else if (rType == _T("nuiStateDecoration"))
    {
      pObj = new nuiStateDecoration(rName);
    }
    else if (rType == _T("nuiTreeHandleDecoration"))
    {
      pObj = new nuiTreeHandleDecoration(rName);
    }
    else if (rType == _T("nuiMetaDecoration"))
    {
      pObj = new nuiMetaDecoration(rName);
    }
    else if (rType == _T("nuiFont") || rType.Compare(_T("font"), false) == 0)
    {
      nuiFontRequest fontrequest;
      ApplyActionsToObject(&fontrequest);
      nuiFont* pFont = nuiFontManager::GetManager().GetFont(fontrequest, rName);
      return true;
    }
    
    if (!pObj)
      return false;
    
    // Apply the actions to the object:
    uint32 i = 0;
    for (i = 0; i < mActions.size(); i++)
    {
      nuiCSSAction* pAction = mActions[i];
      pAction->ApplyAction(pObj);
      delete mActions[i];
    }
        
    return true;
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
        NGL_OUT(_T("a css included filename is empty!\n"));
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
        NGL_OUT(_T("Could not find CSS source file '%ls'\n"), includePath.GetChars());
        return false;
      }
      
      // open the file
      nglIStream* pF = includePath.OpenRead();
      if (!pF)
      {
        NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), includePath.GetChars());
        return false;
      }

      // launch included file parsing
      cssLexer lexer(pF, mrCSS, includePath);
      if (!lexer.Load())
      {
        nglString tmp;
        tmp.CFormat(_T("Error (file '%ls') line %d (%d): %ls"), includePath.GetChars(), lexer.GetLine(), lexer.GetColumn(), lexer.GetErrorStr().GetChars() );
        SetError(tmp);
        return false;
      }
            
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
    else
    {
      if (!ReadMatchers(mMatchers, _T('{')))
        return false;
      
      if (!SkipBlank())
      {
        SetError(_T("unexpected end of file"));
        Clear();
        return false;
      }
      
      bool res = ReadActionList();
      
      // Create the rule from the matchers and the actions:
      nuiCSSRule* pRule = new nuiCSSRule();
      uint32 i = mMatchers.size();
      for (; i > 0; i--)
        pRule->AddMatcher(mMatchers[i-1]);
      for (i = 0; i < mActions.size(); i++)
        pRule->AddAction(mActions[i]);
      
      mrCSS.AddRule(pRule);
      
      mMatchers.clear();
      mActions.clear();
      
      return res;
    }
  }
  
  bool ReadResourceCreator()
  {
    // Eat the creator @
    bool res = GetChar();
    if (!res)
      return false;
    
    nglString type;
    if (!GetSymbol(type))
    {
      SetError(_T("expected a symbol"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return false;        
    }
    
    nglString name;
    if (!GetSymbol(name))
    {
      SetError(_T("expected a symbol"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return false;        
    }
    
    // special case. create color
    if (mChar == _T('='))
    {
      if ((type == _T("nuiColor")) || !type.Compare(_T("color"), false))
      {
        if (!CreateColor(name))
        {
          nglString str;
          str.CFormat(_T("Unable to parse a color with name '%ls'"), name.GetChars());
          SetError(str);
          return false;
        }
      }
      else if (!type.Compare(_T("var"), false))
      {
        if (!CreateVariable(name))
        {
          nglString str;
          str.CFormat(_T("Unable to parse a variable with name '%ls'"), name.GetChars());
          SetError(str);
          return false;
        }
      }
      
      return true;
    }
    
    // create an object
    if (mChar != _T('{'))
    {
      SetError(_T("'{' expected"));
      return false;        
    }
    
    res = ReadActionList();
    if (!res)
    {
      Clear();
      return false;
    }
    
    // Create the object from the type and the name
    if (!CreateObject(type, name))
    {
      nglString str;
      str.CFormat(_T("Unable to create an object of type '%ls' and name '%ls'"), type.GetChars(), name.GetChars());
      SetError(str);
      Clear();
      return false;
    }
    
    // Clear all
    mMatchers.clear();
    mActions.clear();
    
    return res;
  }
  
  nuiWidgetCreator* ReadWidgetCreator(uint32 level = 0)
  {
    bool res = true;
    
    if (!SkipBlank())
    {
      SetError(_T("unexpected end of file"));
      return NULL;        
    }
    
    nglString type;
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

    // Try to read an optional name (the name is not optionnal for the root of the tree)
    nglString name;
    if (mChar != _T('{'))
    {
      if (!GetSymbol(name))
      {
        SetError(_T("expected a widget creator name"));
        return NULL;
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

    // create an object if the declaration is empty
    if (mChar == _T(';'))
    {
      GetChar();
      return NULL;
    }
    
    if (mChar != _T('{'))
    {
      SetError(_T("'{' expected"));
      return NULL;        
    }

    // Skip {
    if (!GetChar())
    {
      SetError(_T("unexpected end of file"));
      return NULL;
    }
    
    nuiWidgetCreator* pCreator = new nuiWidgetCreator(type, name);
        
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
        res = ReadAction(LValue, RValue, Operator);
        if (!res)
        {
          SetError(_T("error while reading an assignment"));
          delete pCreator;
          return NULL;
        }
        
        if (Operator == _T(':')) // attribute assignment
        {
          pCreator->SetAttribute(LValue, RValue);
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
        int32 i = rMatchers.size();
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
        
        if (op == _T('='))
          pMatcher = new nuiWidgetPropertyMatcher(property, str, true, false);
        else if (op == _T(':'))
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
    
    if (!ReadAction(symbol, rvalue, op))
      return false;
    
    if (op == _T(':'))
    {
      nuiCSSAction_SetAttribute* pAction = new nuiCSSAction_SetAttribute(symbol, rvalue);
      mActions.push_back(pAction);
    }
    else if (op == _T('='))
    {
      nuiCSSAction_SetProperty* pAction = new nuiCSSAction_SetProperty(symbol, rvalue);
      mActions.push_back(pAction);
    }
    
    return true;
  }

  bool ReadAction(nglString& rLValue, nglString& rRValue, nglChar& rOperator)
  {
    if (!SkipBlank())
    {
      SetError(_T("Missing action"));
      return false;
    }
    
    nglString symbol;
    bool res = GetSymbol(symbol);
    if (!res)
    {
      SetError(_T("Missing symbol name (l-value)"));
      return false;
    }
    
    if (!SkipBlank())
    {
      SetError(_T("Missing action operator ('=')"));
      return false;
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
  cssLexer lexer(&rStream, *this, rSourcePath);
  if (!lexer.Load())
  {
    mErrorString.CFormat(_T("Error line %d (%d): %ls"), lexer.GetLine(), lexer.GetColumn(), lexer.GetErrorStr().GetChars() );
    return false;
  }
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
  uint32 count = mRules.size();
  for (uint32 i = 0; i < count; i++)
  {
    nuiCSSRule* pRule = mRules[i];
    pRule->ApplyRule(pWidget, MatchersTag);
  }
  pWidget->IncrementCSSPass();
}

bool nuiCSS::GetMatchingRules(nuiWidget* pWidget, std::vector<nuiCSSRule*>& rMatchingRules, uint32 MatchersTag)
{
  rMatchingRules.clear();
  for (uint32 i = 0; i < mRules.size(); i++)
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
