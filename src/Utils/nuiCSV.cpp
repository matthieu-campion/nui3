/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define NUICSV_SEPARATION_TAG _T("<nuicsv/>")
#define NUICSV_COMMENT_TAG _T("<nuicsv_comment/>")



nuiCSV::nuiCSV(nglChar separationChar)
{
  mSeparationChar = separationChar;
  mCommentsEnabled = false; // by default
  mCommentTag = _T('#');
}

nuiCSV::~nuiCSV()
{

}





bool nuiCSV::Load(nglIStream* pStream, bool CheckNbColumns)
{  
  if (!pStream)
  {
    NGL_OUT(_T("nuiCSV::Load error : input stream is NULL!\n"));
    return false;
  }

  if (!pStream->Available())
  {
    NGL_OUT(_T("nuiCSV::Load error : nothing to read from the input stream!\n"));
    return false;
  }
  
  
  pStream->SetTextFormat(eTextDOS);
  pStream->SetTextEncoding(eUTF8);

  
  // reset the document
  mDocument.clear();


  nglString line;
  nglString separationString(mSeparationChar);
  std::vector<nglString> tokens;
  uint32 numlines = 0;
  uint32 numcols = 0;


  // for each line from input stream
  while (pStream->ReadLine(line, NULL))
  {
    //NGL_OUT(_T("%3d ReadLine: %s\n"), numlines, line.GetChars());
    numlines++;
    
    // first, handle the comment lines, if the comment option has been enabled
    if (mCommentsEnabled && (line.GetChar(0) == mCommentTag))
    {
      line.DeleteLeft(1);
      InsertComment(line);
      continue;
    }
    
    // now, handle the usual text lines : 
    //
    // first of all, look for the separation chars that would be between ["..."] : they are not separation chars, they're part of the text
    // => replace'em with a special tag, in order to let the tokenization process correctly
    int32 pos1=0;
    int32 pos2,pos3,tmppos;
    while ((pos2 = line.Find(_T('\"'), pos1)) >= 0)
    {
    
      // if it's a double '""', it'a part of the text, don't do anything
      if (line.GetChar(pos2+1) == _T('\"'))
      {
        pos1 = pos2+1;
        continue;
      }
      
      // it'a single '"'. look for the closing occurence
      pos3 = line.Find(_T('\"'), pos2+1);
      if (pos3 < 0)
      {
        NGL_OUT(_T("nuiCSV syntax error : a '\"' char is missing on line %d!\n"), numlines);
        return false;
      }
      // but don't be fooled by the double '""' (it's a part from the text)
      while (line.GetChar(pos3+1) == _T('\"'))
      {
        pos3 = line.Find(_T('\"'), pos3+1);
      }
      if (pos3 < 0)
      {
        NGL_OUT(_T("nuiCSV syntax error : a '\"' char is missing on line %d!\n"), numlines);
        return false;
      }
      
      
      // now [pos2 - pos3] matches '" ... "'. look for the separation chars inside this range and replace'em with the special tag
      while ((tmppos = line.Find(mSeparationChar, pos2, pos3)) >= 0)
      {
        line.Replace(tmppos, 1, NUICSV_SEPARATION_TAG);
        pos3 += nglString(NUICSV_SEPARATION_TAG).GetLength();
        pos2 = tmppos;
      }
      
      // offset for the next cycle
      pos1 = pos3+1;
    }
    
    
    //
    // ok, we can now tokenize the line, without making mistakes
    //
    tokens.clear();
    line.Tokenize(tokens, mSeparationChar, true);
    
    // check number of columns
    uint32 nbcols = tokens.size();
    if (CheckNbColumns)
    {
      if (nbcols < numcols)
      {
        NGL_OUT(_T("nuiCSV syntax error : not enough columns from the csv document in line %d! Could not process it!\n"), numlines);
        return false;
      }
      else if (numcols == 0)
      {
        numcols = nbcols;
      }
      else if (nbcols > numcols)
      {
        NGL_OUT(_T("nuiCSV syntax error : the number of columns in line %d (%d columns) doesn't match (%d columns)! Could not process it!\n"), numlines, nbcols, numcols);
        return false;
      }
    }
    
    // for all columns,
    // remove useless whitespaces,
    // remove '"' from begin and end, unless it's a double '""'
    for (uint i=0; i < nbcols; i++)
    {
      tokens[i].Trim();
      //uint32 length = strlen(tokens[i].GetStdString().c_str());
      uint32 length = tokens[i].GetLength();
      if ((tokens[i].GetChar(0) == _T('\"')) && (tokens[i].GetChar(1) != _T('\"')))
      {
        if ((tokens[i].GetChar(length-1) == _T('\"')) && (tokens[i].GetChar(length-2) != _T('\"')))
        {
          tokens[i].DeleteLeft(1);
          tokens[i].DeleteRight(1);
        }
      }
      
      // finally, replace all double '""' by single '"'
      tokens[i].Replace(_T("\"\""), _T("\""));
      
      // and replace nuicsv special tag by the separation char (once again, here, it's not a separation char, it's a part from the text)
      tokens[i].Replace(NUICSV_SEPARATION_TAG, separationString);
    }   
    
    // now we can add this line to the document
    InsertLine(tokens);
    
    if (pStream->GetState() != eStreamReady)
      return true;
  }
   
  
  return true;
}



bool nuiCSV::Save(nglOStream* oStream)
{
  oStream->SetTextFormat(eTextDOS);
  oStream->SetTextEncoding(eUTF8);

  nglString dump = Dump();

//#FIXME : the following line bugs
//  oStream->WriteText(dump);  

  std::string s = dump.GetStdString();
  const char* cstr = s.c_str();
  oStream->Write((void*)cstr, strlen(cstr), 1);


  return true;
}





nglString nuiCSV::Dump()
{
  nglString csv;
  
  nglString separation;
  separation.Format(_T(" %lc "), mSeparationChar);
    
  for (uint linenum = 0; linenum < mDocument.size(); linenum++)
  {
    const std::vector<nglString>& line = mDocument[linenum];
    size_t nbCols = line.size();
    
    // dump a comment
    if (!line[0].Compare(NUICSV_COMMENT_TAG))
    {
      // only if it has been allowed
      if (!mCommentsEnabled)
        continue;
        
      csv.Append(mCommentTag);
      csv.Append(line[1]);
    }

    // dump a csv text line
    else
    for (size_t colnum = 0; colnum < nbCols; colnum++)
    {
      nglString word = line[colnum];
      
      word.Replace(_T("\""), _T("\"\"")); // " => ""
      
      if ((colnum == 0) && (word.GetChar(0) == mCommentTag))
      {
        word.Prepend(_T('\"'));
        word.Append(_T('\"'));
      }
      
      else if ((word.Find(mSeparationChar)>=0) || (word.GetChar(0) == _T(' ')) || (word.GetChar(word.GetLength()-1) == _T(' ')))
      {
        word.Prepend(_T('\"'));
        word.Append(_T('\"'));
      }
      
      csv.Append(word);
      if (colnum < (nbCols-1))
        csv.Append(separation);
    }
    
    csv.Append(_T("\n"));
  }
  
  return csv;
}



void nuiCSV::InsertComment(const nglString& rComment)
{
  std::vector<nglString> vec;
  
  if (!mCommentsEnabled)
    return;
  
  vec.push_back(NUICSV_COMMENT_TAG);
  vec.push_back(rComment);
  
  InsertLine(vec);
}


void nuiCSV::EnableComments(bool set, nglChar commentTag)
{
  mCommentsEnabled = set;
  mCommentTag = commentTag;
}


void nuiCSV::InsertLine(const std::vector<nglString>& rLine)
{
  mDocument.push_back(rLine);
}


const std::vector<std::vector<nglString> >& nuiCSV::GetDocument()
{
  return mDocument;
}


bool nuiCSV::IsCommented(uint32 linenum)
{
  return !mDocument[linenum][0].Compare(NUICSV_COMMENT_TAG);
}

