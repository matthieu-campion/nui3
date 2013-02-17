/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiHyperLink::nuiHyperLink()
: nuiLabel(_T("")),
mURL(_T(""))
{
  if (SetObjectClass(_T("nuiHyperLink")))
    InitAttributes();

  SetTextColor(nuiColor(_T("nuiHyperLink")));
}


nuiHyperLink::nuiHyperLink(const nglString& rURL, const nglString& rLabel)
  : nuiLabel(rLabel.IsNull() ? rURL : rLabel),
    mURL(rURL)
{
  if (SetObjectClass(_T("nuiHyperLink")))
    InitAttributes();
    
  SetTextColor(nuiColor(_T("nuiHyperLink")));
}


void nuiHyperLink::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("URL")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHyperLink::GetURL), 
                nuiMakeDelegate(this, &nuiHyperLink::SetURL)));  
}


void nuiHyperLink::SetURL(const nglString& rURL)
{
  mURL = rURL;
  SetToolTip(rURL);
}

const nglString& nuiHyperLink::GetURL() const
{
  return mURL;
}

bool nuiHyperLink::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked++;
    Grab();
    return true;
  }
  return false;
}

bool nuiHyperLink::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mClicked && (Button & nglMouseInfo::ButtonLeft))
  {
    mClicked--;
    Ungrab();
    if (!Activated())
    {
      // Open url in default browser
      nuiURL url(mURL);
      url.OpenBrowser();
      SetUnderline(false);
    }
    return true;
  }
  return false;
}

void nuiHyperLink::SetHover(bool Hover)
{
  SetUnderline(Hover);
}

