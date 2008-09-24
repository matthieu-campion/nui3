/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHyperLink.h"
#include "nuiURL.h"

nuiHyperLink::nuiHyperLink(const nglString& rURL, const nglString& rLabel)
  : nuiLabel(rLabel.IsNull() ? rURL : rLabel),
    mURL(rURL)
{
  SetObjectClass(_T("nuiHyperLink"));
  SetTextColor(nuiColor(_T("nuiHyperLink")));
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
  }
  return true;
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
    }
  }
  return true;
}

void nuiHyperLink::SetHover(bool Hover)
{
  SetUnderline(Hover);
}

