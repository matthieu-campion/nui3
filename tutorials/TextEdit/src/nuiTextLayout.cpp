/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiTextLayout.h"
#include "nuiFontManager.h"

#include "../../../deps/harfbuzz/hb.h"


void TextLayoutTest(const nglString& txt)
{
  nuiFontRequest request;
  request.MustHaveSize(14, 2);
  request.SetName("Times", 2);
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(request);
  printf("Requested font: %s\n", pFont->GetFamilyName().GetChars());
  nuiTextLayout layout(pFont);
  layout.LayoutText(txt);
}


/////////////
nuiTextRun::nuiTextRun(nuiUnicodeScript script, const nglString& rString, int32 Position, int32 Length)
: mpFont(NULL),
  mString(rString),
  mPosition(Position),
  mLength(Length),
  mScript(script)
{
}

nuiTextRun::~nuiTextRun()
{
  mpFont->Release();
}

void nuiTextRun::SetFont(nuiFont* pFont)
{
  NGL_ASSERT(!mpFont);
  pFont->Acquire();
  mpFont = pFont;
}

nuiUnicodeScript nuiTextRun::GetScript() const
{
  return mScript;
}


void nuiTextRun::AddGlyph(int32 Index, float X, float Y)
{
  nuiGlyphLayout glyph;
  glyph.X = X;
  glyph.Y = Y;
  mpFont->PrepareGlyph(Index, glyph, true);
  mGlyphs.push_back(glyph);
}

int32 nuiTextRun::GetPosition() const
{
  return mPosition;
}

int32 nuiTextRun::GetLength() const
{
  return mLength;
}


////////////
//nuiTextLine
nuiTextLine::nuiTextLine(float X, float Y)
{
}

nuiTextLine::~nuiTextLine()
{
  for (uint32 i = 0; i < mpRuns.size(); i++)
    mpRuns[i]->Release();
}

/** @name Drawing the Line */
//@{
void nuiTextLine::Draw(nuiDrawContext* pContext)
{
}
//@}

const std::vector<nuiTextRun*>& nuiTextLine::GetGlyphRuns() const
{
  return mpRuns;
}

float nuiTextLine::GetX() const
{
  return mX;
}

float nuiTextLine::GetY() const
{
  return mY;
}

void nuiTextLine::SetPosition(float X, float Y)
{
  mX = X;
  mY = Y;
}

void nuiTextLine::AddRun(nuiTextRun* pRun)
{
  mpRuns.push_back(pRun);
}

uint32 nuiTextLine::size() const
{
  return mpRuns.size();
}

nuiTextRun* nuiTextLine::operator[](uint32 index) const
{
  return mpRuns[index];
}


/////////////////
// nuiTextLayout
nuiTextLayout::nuiTextLayout(nuiFont* pFont)
{
  pFont->Acquire();
  mpFont = pFont;
}

nuiTextLayout::~nuiTextLayout()
{
  mpFont->Release();

  for (uint32 p = 0; p < mpParagraphs.size(); p++)
  {
    Paragraph* pParagraph = mpParagraphs[p];
    for (uint32 l = 0; l < pParagraph->size(); l++)
    {
      nuiTextLine* pLine = (*pParagraph)[l];
      for (uint32 r = 0; r < pLine->size(); r++)
      { 
        nuiTextRun* pRun = (*pLine)[r];
        pRun->Release();
      }
      delete pLine;
    }
    
    delete pParagraph;
  }
}

bool nuiTextLayout::LayoutText(const nglString& rString)
{
  // General algorithm:
  // 1. Split text into paragraphs (LayoutText)
  // 2. Split paragraphs into ranges (LayoutParagraph)
  // 3. Split ranges into fonts
  // 4. Split ranges into lines / words if needed

  int32 start = 0;
  int32 position = 0;
  int32 len = rString.GetLength();
  while (position < len)
  {
    // Scan through the text and look for end of line markers
    nglUChar ch = rString.GetNextUChar(position);
    if (ch == '\n')
    {
      // Found a paragraph
      LayoutParagraph(rString, start, position - start);
      start = position;
    }
  }
  
  // Find the needed fonts for each script:
  std::map<nuiUnicodeScript, nuiFont*> FontSet;
  {
    std::map<nuiUnicodeScript, std::set<nglUChar> >::iterator it = mCharsets.begin();
    std::map<nuiUnicodeScript, std::set<nglUChar> >::iterator end = mCharsets.end();
    while (it != end)
    {
      printf("%s -> ", nuiGetUnicodeScriptName(it->first).GetChars());
      const std::set<nglUChar>& charset(it->second);
      nuiFont* pFont = NULL;
      // First try the requested font
      {
        std::set<nglUChar>::const_iterator it = charset.begin();
        std::set<nglUChar>::const_iterator end = charset.end();
        
        while (it != end && mpFont->GetGlyphIndex(*it) > 0)
          ++it;
        
        // If all the glyphs are available in the font we're done...
        if (it == end)
          pFont = mpFont;
        else
          printf("[couldn't find glyph %d '%c' in requested font] ", *it, *it);
      }

      // If the requested font doesn't work, try to find one that does:
      if (!pFont)
      {
        nuiFontRequest request(mpFont);
        request.MustHaveGlyphs(charset, 10);
        pFont = nuiFontManager::GetManager().GetFont(request);
      }
      
      FontSet[it->first] = pFont;
      
      printf("%s\n", pFont->GetFamilyName().GetChars());
      
      ++it;
    }
  }

  uint32 i = 0;
  // Assign the correct font to each run
  for (uint32 p = 0; p < mpParagraphs.size(); p++)
  {
    Paragraph* pParagraph = mpParagraphs[p];
    for (uint32 l = 0; l < pParagraph->size(); l++)
    {
      nuiTextLine* pLine = (*pParagraph)[l];
      for (uint32 r = 0; r < pLine->size(); r++)
      { 
        nuiTextRun* pRun = (*pLine)[r];
        nuiFont* pFont = FontSet[pRun->GetScript()];
        pRun->SetFont(pFont);
        
        printf("\trange %d <%d.%d.%d> (%d - %d) (%s --> %s)\n", i, p, l, r, pRun->GetPosition(), pRun->GetLength(), nuiGetUnicodeScriptName(pRun->GetScript()).GetChars(), pFont->GetFamilyName().GetChars());

        i++;
      }
    }
  }
  
  
  mCharsets.clear();
  return true;
}

bool nuiTextLayout::LayoutParagraph(const nglString& rString, int32 start, int32 length)
{
  printf("new paragraph: %d + %d\n", start, length);

  mpParagraphs.push_back(new Paragraph());

  nuiTextLine* pLine = new nuiTextLine(0, 0);
  mpParagraphs.back()->push_back(pLine);
  
  // Split the paragraph into ranges:
  nuiTextRangeList ranges;
  nuiSplitText(rString, ranges, nuiST_ScriptChange, start, length);

  {
    nuiTextRangeList::iterator it = ranges.begin();
    nuiTextRangeList::iterator end = ranges.end();
    int32 origin = start;
    int32 i = 0;
    while (it != end)
    {
      const nuiTextRange& range(*it);
      int32 len = range.mLength;
      int32 pos = origin;
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
      
      std::set<nglUChar>& charset(mCharsets[range.mScript]);
      {
        while (pos < origin + len)
        {
          nglUChar ch = rString.GetNextUChar(pos);
          charset.insert(ch);
        }
      }

      origin += len;
      ++it;
      i++;
    }
  }
  

  {
    nuiTextRangeList::iterator it = ranges.begin();
    nuiTextRangeList::iterator end = ranges.end();
    uint32 i = 0;
    uint32 pos = 0;
    while (it != end)
    {
      const nuiTextRange& range(*it);
      uint32 len = range.mLength;
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
         
      nuiTextRun* pRun = new nuiTextRun(range.mScript, rString, pos, len);
      pLine->AddRun(pRun);
      
      
      pos += len;
      ++i;
      ++it;
    }
  }
  
  return true;
}

void nuiTextLayout::SetJustification(bool set)
{
  mJustify = set;
}

bool nuiTextLayout::GetJustification() const
{
  return mJustify;
}

void nuiTextLayout::SetFlush(float set)
{
  mFlush = set;
}

float nuiTextLayout::GetFlush() const
{
  return mFlush;
}

#if 0
FT_Face ft_face = cairo_ft_scaled_font_lock_face (scaled_font);
hb_face_t *hb_face = hb_ft_face_create_cached (ft_face);
hb_font_t *hb_font = hb_ft_font_create (ft_face, NULL);
hb_buffer_t *hb_buffer;
cairo_glyph_t *cairo_glyphs;
hb_glyph_info_t *hb_glyph;
hb_glyph_position_t *hb_position;
unsigned int num_glyphs, i;
hb_position_t x;

if (len < 0)
len = strlen (text);
hb_buffer = hb_buffer_create (len);

hb_buffer_set_unicode_funcs (hb_buffer, hb_glib_get_unicode_funcs ());

hb_buffer_add_utf8 (hb_buffer, text, len, 0, len);
if (script)
hb_buffer_set_script (hb_buffer, hb_script_from_string (script));
if (language)
hb_buffer_set_language (hb_buffer, hb_language_from_string (language));

hb_shape (hb_font, hb_face, hb_buffer, features, num_features);

num_glyphs = hb_buffer_get_length (hb_buffer);
hb_glyph = hb_buffer_get_glyph_infos (hb_buffer, NULL);
hb_position = hb_buffer_get_glyph_positions (hb_buffer, NULL);
cairo_glyphs = cairo_glyph_allocate (num_glyphs + 1);
x = 0;
for (i = 0; i < num_glyphs; i++)
{
  cairo_glyphs[i].index = hb_glyph->codepoint;
  cairo_glyphs[i].x = (hb_position->x_offset + x) * (1./64);
  cairo_glyphs[i].y = -(hb_position->y_offset)    * (1./64);
  x += hb_position->x_advance;
  
  hb_glyph++;
  hb_position++;
}
cairo_glyphs[i].x = x * (1./64);
hb_buffer_destroy (hb_buffer);
hb_font_destroy (hb_font);
hb_face_destroy (hb_face);
#endif

