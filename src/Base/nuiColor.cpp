/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"



typedef struct static_color
{
  nglChar* name;
  uint8 r,g,b;
} static_color;

static static_color static_colors[] =
{
  // default colors for decoration
  {(nglChar*)_T("nuiDefaultColorPane1"), 240,240,240},
  {(nglChar*)_T("nuiDefaultColorPane2"), 252,252,252},
  {(nglChar*)_T("nuiDefaultColorStroke"), 205,205,205},

  {(nglChar*)_T("nuiDefaultColorLabelText"), 40,40,40},

  {(nglChar*)_T("nuiDefaultColorScrollBar1"), 169,181,200},
  {(nglChar*)_T("nuiDefaultColorScrollBar2"), 91,115,150},
  {(nglChar*)_T("nuiDefaultColorScrollBar3"), 34,66,114},
  {(nglChar*)_T("nuiDefaultColorScrollBarStroke"), 94,117,152},

  {(nglChar*)_T("nuiHyperLink"), 64,64,255},


  // constants (web colors)
  {(nglChar*)_T("aliceblue"), 240, 248, 255}, 
  {(nglChar*)_T("antiquewhite"), 250, 235, 215}, 
  {(nglChar*)_T("aqua"),  0, 255, 255}, 
  {(nglChar*)_T("aquamarine"), 127, 255, 212}, 
  {(nglChar*)_T("azure"), 240, 255, 255}, 
  {(nglChar*)_T("beige"), 245, 245, 220}, 
  {(nglChar*)_T("bisque"), 255, 228, 196}, 
  {(nglChar*)_T("black"),  0, 0, 0}, 
  {(nglChar*)_T("blanchedalmond"), 255, 235, 205}, 
  {(nglChar*)_T("blue"),  0, 0, 255}, 
  {(nglChar*)_T("blueviolet"), 138, 43, 226}, 
  {(nglChar*)_T("brown"), 165, 42, 42}, 
  {(nglChar*)_T("burlywood"), 222, 184, 135}, 
  {(nglChar*)_T("cadetblue"),  95, 158, 160}, 
  {(nglChar*)_T("chartreuse"), 127, 255, 0}, 
  {(nglChar*)_T("chocolate"), 210, 105, 30}, 
  {(nglChar*)_T("coral"), 255, 127, 80}, 
  {(nglChar*)_T("cornflowerblue"), 100, 149, 237}, 
  {(nglChar*)_T("cornsilk"), 255, 248, 220}, 
  {(nglChar*)_T("crimson"), 220, 20, 60}, 
  {(nglChar*)_T("cyan"),  0, 255, 255}, 
  {(nglChar*)_T("darkblue"),  0, 0, 139}, 
  {(nglChar*)_T("darkcyan"),  0, 139, 139}, 
  {(nglChar*)_T("darkgoldenrod"), 184, 134, 11}, 
  {(nglChar*)_T("darkgray"), 169, 169, 169}, 
  {(nglChar*)_T("darkgreen"),  0, 100, 0}, 
  {(nglChar*)_T("darkgrey"), 169, 169, 169}, 
  {(nglChar*)_T("darkkhaki"), 189, 183, 107}, 
  {(nglChar*)_T("darkmagenta"), 139, 0, 139}, 
  {(nglChar*)_T("darkolivegreen"),  85, 107, 47}, 
  {(nglChar*)_T("darkorange"), 255, 140, 0}, 
  {(nglChar*)_T("darkorchid"), 153, 50, 204}, 
  {(nglChar*)_T("darkred"), 139, 0, 0}, 
  {(nglChar*)_T("darksalmon"), 233, 150, 122}, 
  {(nglChar*)_T("darkseagreen"), 143, 188, 143}, 
  {(nglChar*)_T("darkslateblue"),  72, 61, 139}, 
  {(nglChar*)_T("darkslategray"),  47, 79, 79}, 
  {(nglChar*)_T("darkslategrey"),  47, 79, 79}, 
  {(nglChar*)_T("darkturquoise"),  0, 206, 209}, 
  {(nglChar*)_T("darkviolet"), 148, 0, 211}, 
  {(nglChar*)_T("deeppink"), 255, 20, 147}, 
  {(nglChar*)_T("deepskyblue"),  0, 191, 255}, 
  {(nglChar*)_T("dimgray"), 105, 105, 105}, 
  {(nglChar*)_T("dimgrey"), 105, 105, 105}, 
  {(nglChar*)_T("dodgerblue"),  30, 144, 255}, 
  {(nglChar*)_T("firebrick"), 178, 34, 34}, 
  {(nglChar*)_T("floralwhite"), 255, 250, 240}, 
  {(nglChar*)_T("forestgreen"),  34, 139, 34}, 
  {(nglChar*)_T("fuchsia"), 255, 0, 255}, 
  {(nglChar*)_T("gainsboro"), 220, 220, 220}, 
  {(nglChar*)_T("ghostwhite"), 248, 248, 255}, 
  {(nglChar*)_T("gold"), 255, 215, 0}, 
  {(nglChar*)_T("goldenrod"), 218, 165, 32}, 
  {(nglChar*)_T("gray"), 128, 128, 128}, 
  {(nglChar*)_T("grey"), 128, 128, 128}, 
  {(nglChar*)_T("green"),  0, 128, 0}, 
  {(nglChar*)_T("greenyellow"), 173, 255, 47}, 
  {(nglChar*)_T("honeydew"), 240, 255, 240}, 
  {(nglChar*)_T("hotpink"), 255, 105, 180}, 
  {(nglChar*)_T("indianred"), 205, 92, 92}, 
  {(nglChar*)_T("indigo"),  75, 0, 130}, 
  {(nglChar*)_T("ivory"), 255, 255, 240}, 
  {(nglChar*)_T("khaki"), 240, 230, 140}, 
  {(nglChar*)_T("lavender"), 230, 230, 250}, 
  {(nglChar*)_T("lavenderblush"), 255, 240, 245}, 
  {(nglChar*)_T("lawngreen"), 124, 252, 0}, 
  {(nglChar*)_T("lemonchiffon"), 255, 250, 205}, 
  {(nglChar*)_T("lightblue"), 173, 216, 230}, 
  {(nglChar*)_T("lightcoral"), 240, 128, 128}, 
  {(nglChar*)_T("lightcyan"), 224, 255, 255}, 
  {(nglChar*)_T("lightgoldenrodyellow"), 250, 250, 210}, 
  {(nglChar*)_T("lightgray"), 211, 211, 211}, 
  {(nglChar*)_T("lightgreen"), 144, 238, 144}, 
  {(nglChar*)_T("lightgrey"), 211, 211, 211}, 
  {(nglChar*)_T("lightpink"), 255, 182, 193}, 
  {(nglChar*)_T("lightsalmon"), 255, 160, 122}, 
  {(nglChar*)_T("lightseagreen"),  32, 178, 170}, 
  {(nglChar*)_T("lightskyblue"), 135, 206, 250}, 
  {(nglChar*)_T("lightslategray"), 119, 136, 153}, 
  {(nglChar*)_T("lightslategrey"), 119, 136, 153}, 
  {(nglChar*)_T("lightsteelblue"), 176, 196, 222}, 
  {(nglChar*)_T("lightyellow"), 255, 255, 224}, 
  {(nglChar*)_T("lime"),  0, 255, 0}, 
  {(nglChar*)_T("limegreen"),  50, 205, 50}, 
  {(nglChar*)_T("linen"), 250, 240, 230}, 
  {(nglChar*)_T("magenta"), 255, 0, 255}, 
  {(nglChar*)_T("maroon"), 128, 0, 0}, 
  {(nglChar*)_T("mediumaquamarine"), 102, 205, 170}, 
  {(nglChar*)_T("mediumblue"),  0, 0, 205}, 
  {(nglChar*)_T("mediumorchid"), 186, 85, 211}, 
  {(nglChar*)_T("mediumpurple"), 147, 112, 219}, 
  {(nglChar*)_T("mediumseagreen"),  60, 179, 113}, 
  {(nglChar*)_T("mediumslateblue"), 123, 104, 238}, 
  {(nglChar*)_T("mediumspringgreen"),  0, 250, 154}, 
  {(nglChar*)_T("mediumturquoise"),  72, 209, 204}, 
  {(nglChar*)_T("mediumvioletred"), 199, 21, 133}, 
  {(nglChar*)_T("midnightblue"),  25, 25, 112}, 
  {(nglChar*)_T("mintcream"), 245, 255, 250}, 
  {(nglChar*)_T("mistyrose"), 255, 228, 225}, 
  {(nglChar*)_T("moccasin"), 255, 228, 181}, 
  {(nglChar*)_T("navajowhite"), 255, 222, 173}, 
  {(nglChar*)_T("navy"),  0, 0, 128}, 
  {(nglChar*)_T("oldlace"), 253, 245, 230}, 
  {(nglChar*)_T("olive"), 128, 128, 0}, 
  {(nglChar*)_T("olivedrab"), 107, 142, 35}, 
  {(nglChar*)_T("orange"), 255, 165, 0}, 
  {(nglChar*)_T("orangered"), 255, 69, 0}, 
  {(nglChar*)_T("orchid"), 218, 112, 214}, 
  {(nglChar*)_T("palegoldenrod"), 238, 232, 170}, 
  {(nglChar*)_T("palegreen"), 152, 251, 152}, 
  {(nglChar*)_T("paleturquoise"), 175, 238, 238}, 
  {(nglChar*)_T("palevioletred"), 219, 112, 147}, 
  {(nglChar*)_T("papayawhip"), 255, 239, 213}, 
  {(nglChar*)_T("peachpuff"), 255, 218, 185}, 
  {(nglChar*)_T("peru"), 205, 133, 63}, 
  {(nglChar*)_T("pink"), 255, 192, 203}, 
  {(nglChar*)_T("plum"), 221, 160, 221}, 
  {(nglChar*)_T("powderblue"), 176, 224, 230}, 
  {(nglChar*)_T("purple"), 128, 0, 128}, 
  {(nglChar*)_T("red"), 255, 0, 0}, 
  {(nglChar*)_T("rosybrown"), 188, 143, 143}, 
  {(nglChar*)_T("royalblue"),  65, 105, 225}, 
  {(nglChar*)_T("saddlebrown"), 139, 69, 19}, 
  {(nglChar*)_T("salmon"), 250, 128, 114}, 
  {(nglChar*)_T("sandybrown"), 244, 164, 96}, 
  {(nglChar*)_T("seagreen"),  46, 139, 87}, 
  {(nglChar*)_T("seashell"), 255, 245, 238}, 
  {(nglChar*)_T("sienna"), 160, 82, 45}, 
  {(nglChar*)_T("silver"), 192, 192, 192}, 
  {(nglChar*)_T("skyblue"), 135, 206, 235}, 
  {(nglChar*)_T("slateblue"), 106, 90, 205}, 
  {(nglChar*)_T("slategray"), 112, 128, 144}, 
  {(nglChar*)_T("slategrey"), 112, 128, 144}, 
  {(nglChar*)_T("snow"), 255, 250, 250}, 
  {(nglChar*)_T("springgreen"),  0, 255, 127}, 
  {(nglChar*)_T("steelblue"),  70, 130, 180}, 
  {(nglChar*)_T("tan"), 210, 180, 140}, 
  {(nglChar*)_T("teal"),  0, 128, 128}, 
  {(nglChar*)_T("thistle"), 216, 191, 216}, 
  {(nglChar*)_T("tomato"), 255, 99, 71}, 
  {(nglChar*)_T("turquoise"),  64, 224, 208}, 
  {(nglChar*)_T("violet"), 238, 130, 238}, 
  {(nglChar*)_T("wheat"), 245, 222, 179}, 
  {(nglChar*)_T("white"), 255, 255, 255}, 
  {(nglChar*)_T("whitesmoke"), 245, 245, 245}, 
  {(nglChar*)_T("yellow"), 255, 255, 0}, 
  {(nglChar*)_T("yellowgreen"), 154, 205, 50},
  {NULL, 0, 0, 0}
};    

bool nuiColor::SetValue(const nglString& rString)
{
  const float ratio = 1.0f / 255.0f;
  // Search custom colors first:
  ColorMap::iterator it = mCustomColors.find(rString);
  if (it != mCustomColors.end())
  {
    *this = it->second;
    return true;
  }

  if (rString.GetChar(0)=='#')
  {
    nglString str(rString.Extract(1));

    uint col,len;
    col = str.GetUInt(/*base=*/16);
    len = str.GetLength();
    mRed   = ((col>>16) & 0xFF) * ratio;
    mGreen = ((col>>8 ) & 0xFF) * ratio;
    mBlue  = ((col    ) & 0xFF) * ratio;
    mAlpha = 1;
    if (6 < len)
    {
      // If there is an alpha component get it, otherwise it's 1
      mAlpha = ((col>>24) & 0xFF) * ratio;
      Multiply(mAlpha, false);
    }
    return true;
  }
  else if (rString.Extract(0,3) == "rgb")
  {
    nglString str = rString.Extract(rString.Find('(')+1);
    str.TrimRight(')');
    std::vector<nglString> tokens;
    str.Tokenize(tokens, _T(','));
    
    for (uint32 i = 0; i < tokens.size(); i++)
      tokens[i].Trim();
    
    switch (tokens.size())
    {
    case 4:
      // I'm not sure alpha is in the standard or even handled this way (if you know better, tell me):
      mAlpha = (tokens[3].GetCInt() * ratio);
      mRed   = mAlpha * (tokens[0].GetCInt() * ratio);
      mGreen = mAlpha * (tokens[1].GetCInt() * ratio);
      mBlue  = mAlpha * (tokens[2].GetCInt() * ratio);
      return true;
    case 3:
      mAlpha = 1;
      mRed   = (tokens[0].GetCInt() * ratio);
      mGreen = (tokens[1].GetCInt() * ratio);
      mBlue  = (tokens[2].GetCInt() * ratio);
      return true;
      break;
    default:
      return false;
    }
  }
  else
  {
    uint i;
    // try to find a static color name that fit the given color name:
    for (i = 0; static_colors[i].name; i++)
    {
      if (rString.Compare(static_colors[i].name, false) == 0)
      {
        mAlpha = 1; // Default alpha is one. Always. :).
        mRed   = (static_colors[i].r * ratio);
        mGreen = (static_colors[i].g * ratio);
        mBlue  = (static_colors[i].b * ratio);
        return true;
      }
    }
    
    // the color name is unknow.
    //wprintf(_T("error : the color name '%s' is unknown\n"), rString.GetChars());
  }
  
  NGL_LOG(_T("nuiWidgetCreator"), NGL_LOG_WARNING, _T("nuiColor warning : could not find any color definition from '%s'\n"), rString.GetChars());

  return false;
}

void nuiColor::Get(nglString& rString) const
{
  uint32 val = 0;
  nuiColor t(*this);
  t.UnPremultiply();
  val += ToZero(t.mAlpha * 255.0f);
  val<<=8;                    
  val += ToZero(t.mRed   * 255.0f);
  val<<=8;
  val += ToZero(t.mGreen * 255.0f);
  val<<=8;
  val += ToZero(t.mBlue  * 255.0f);
  rString.SetCUInt(val,16); 
  rString.Insert('#',0);
}

nglString nuiColor::GetValue() const
{
  nglString String;
  uint32 val = 0;
  val += (uint32)(mAlpha * 255.0f);
  val<<=8;
  val += (uint32)(mRed   * 255.0f);
  val<<=8;
  val += (uint32)(mGreen * 255.0f);
  val<<=8;
  val += (uint32)(mBlue  * 255.0f);
  String.SetCUInt(val,16); 
  String.Insert('#',0);
  return String;
}

void nuiColor::EnumStandardColorNames(std::vector<nglString>& rStandardColorNames)
{
  for (int i = 0; i < 147; i++)
  {
    rStandardColorNames.push_back(static_colors[i].name);
  }
}

void nuiColor::Crop()
{
#ifdef NUI_USE_SSE
  const float* pElts = &mRed;
  const float one[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  __asm
  {
    mov eax, pElts; // eax = &mRed( pointer to the colors)
    xorps xmm1, xmm1; // xmm1 = 0
    movups xmm0, [eax]; // xmm0 = (red|green|blue|alpha)
    movups xmm2, one;

    maxps xmm0, xmm1;
    minps xmm0, xmm2;
    movups [eax], xmm0; //< Store the values back
  }
#else
  if (std::isnan(mRed))
    mRed = 0;
  if (std::isnan(mGreen))
    mGreen = 0;
  if (std::isnan(mBlue))
    mBlue = 0;
  if (std::isnan(mAlpha))
    mAlpha = 0;
  mRed   = MIN(1.0f, mRed  );
  mRed  =  MAX(0.0f, mRed  );
  mGreen = MIN(1.0f, mGreen);
  mGreen = MAX(0.0f, mGreen);
  mBlue  = MIN(1.0f, mBlue );
  mBlue  = MAX(0.0f, mBlue );
  mAlpha = MIN(1.0f, mAlpha);
  mAlpha = MAX(0.0f, mAlpha);
#endif
}

void nuiColor::EnumCustomizedColorNames(std::vector<nglString>& rUserColorNames)
{
  rUserColorNames.clear();
  rUserColorNames.reserve(mCustomColors.size());
  
  ColorMap::iterator it = mCustomColors.begin();
  ColorMap::iterator end = mCustomColors.end();
  
  while (it != end)
  {
    rUserColorNames.push_back(it->first);
    ++it;
  }
}

void nuiColor::SetColor(const nglString& rName, const nuiColor& rColor)
{
  mCustomColors[rName] = rColor;
}

bool nuiColor::GetColor(const nglString& rName, nuiColor& rColor)
{
  ColorMap::iterator it = mCustomColors.find(rName);
  if (it == mCustomColors.end())
    return false;
  
  rColor = it->second;
  return true;
}

nuiColor::ColorMap nuiColor::mCustomColors;

uint32 nuiColor::GetRGBA() const
{
  uint32 t = 0;
  uint8* pt = (uint8*)&t;
  pt[0] = ToBelow(mRed   * 255.0f);
  pt[1] = ToBelow(mGreen * 255.0f);
  pt[2] = ToBelow(mBlue  * 255.0f);
  pt[3] = ToBelow(mAlpha * 255.0f);
  return t;
}

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
void RGBtoHSV( float r, float g, float b, float& h, float& s, float& v )
{
	float _min, _max, delta;
	_min = MIN(r, MIN(g, b));
	_max = MAX(r, MAX(g, b));
	v = _max;				// v
	delta = _max - _min;
	if (_max != 0)
		s = delta / _max;		// s
	else
  {
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = -1;
		return;
	}
	if (r == _max)
		h = (g - b) / delta;		// between yellow & magenta
	else if( g == _max )
		h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		h = 4 + (r - g) / delta;	// between magenta & cyan
	h *= 60;				// degrees
	while (h < 0)
		h += 360;
	while (h >= 360)
		h -= 360;
  h /= 360;
  
  if (std::isnan(h))
    h = 0;
  if (std::isnan(s))
    s = 0;
  if (std::isnan(v))
    v = 0;
}

void HSVtoRGB( float& r, float& g, float& b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
  h *= 360;
  while (h >= 360)
    h -= 360;
  while (h < 0)
    h += 360;
  
	if (s == 0)
  {
		// achromatic (grey)
		r = g = b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i)
  {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}
  
  if (std::isnan(r))
    r = 0;
  if (std::isnan(g))
    g = 0;
  if (std::isnan(b))
    b = 0;
  
}


void nuiColor::SetHSV(float h, float s, float v, float a)
{
  HSVtoRGB(mRed, mGreen, mBlue, h, s, v);
  mAlpha = a;
  Premultiply();
  Crop();
}

void nuiColor::GetHSV(float& h, float& s, float& v) const
{
  nuiColor c(*this);
  c.UnPremultiply();
  float r = c.Red(), g = c.Green(), b = c.Blue();
  RGBtoHSV(r, g, b, h, s, v);  
}

void nuiColor::SetHSL(float h, float s, float l, float a)
{
  double v;
  double r,g,b;

  r = l;   // default to gray
  g = l;
  b = l;
  v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
  if (v > 0)
  {
    double m;
    double sv;
    int sextant;
    double fract, vsf, mid1, mid2;
    
    m = l + l - v;
    sv = (v - m ) / v;
    h *= 6.0;
    sextant = (int)h;
    fract = h - sextant;
    vsf = v * sv * fract;
    mid1 = m + vsf;
    mid2 = v - vsf;
    switch (sextant)
    {
      case 0:
        r = v;
        g = mid1;
        b = m;
        break;
      case 1:
        r = mid2;
        g = v;
        b = m;
        break;
      case 2:
        r = m;
        g = v;
        b = mid1;
        break;
      case 3:
        r = m;
        g = mid2;
        b = v;
        break;
      case 4:
        r = mid1;
        g = m;
        b = v;
        break;
      case 5:
        r = v;
        g = m;
        b = mid2;
        break;
    }
  }
  
  if (std::isnan(r))
    r = 0;
  if (std::isnan(g))
    g = 0;
  if (std::isnan(b))
    b = 0;
  if (std::isnan(a))
    a = 0;
  
  mRed = r;
  mGreen = g;
  mBlue = b;
  mAlpha = a;

  Premultiply();
  Crop();
}

void nuiColor::GetHSL(float& h, float& s, float& l) const
{
  nuiColor c(*this);
  c.UnPremultiply();
  double r = c.Red();
  double g = c.Green();
  double b = c.Blue();
  double v;
  double m;
  double vm;
  double r2, g2, b2;
  
  h = 0; // default to black
  s = 0;
  l = 0;
  
  v = MAX(r,g);
  v = MAX(v,b);
  m = MIN(r,g);
  m = MIN(m,b);
  l = (m + v) / 2.0;
  
  if (l <= 0.0)
    return;

  vm = v - m;
  s = vm;
  if (s > 0.0)
  {
    s /= (l <= 0.5) ? (v + m ) : (2.0 - v - m) ;
  }
  else
  {
    return;
  }
  r2 = (v - r) / vm;
  g2 = (v - g) / vm;
  b2 = (v - b) / vm;
  if (r == v)
  {
    h = (g == m ? 5.0 + b2 : 1.0 - g2);
  }
  else if (g == v)
  {
    h = (b == m ? 1.0 + r2 : 3.0 - b2);
  }
  else
  {
    h = (r == m ? 3.0 + g2 : 5.0 - r2);
  }
  h /= 6.0;

  if (std::isnan(h))
    h = 0;
  if (std::isnan(s))
    s = 0;
  if (std::isnan(l))
    l = 0;
}

void nuiColor::SetOpacity(float v)
{
  UnPremultiply();
  mAlpha = v;
  Premultiply();
}
