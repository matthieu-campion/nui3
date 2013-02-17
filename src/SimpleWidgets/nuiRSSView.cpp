/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

//class nuiRSSView : public nuiSimpleContainer
nuiRSSView::nuiRSSView(const nglString& rURL, int32 SecondsBetweenUpdates, nglIStream* pOriginalStream, bool ForceNoHTML)
: mViewSink(this)
{
  if (SetObjectClass(_T("nuiRSSView")))
  {
    InitAttributes();
  }
  
  mTextColor = nuiColor(0,0,0);
  mFont = nglString::Null;
  mMaxItems = -1;
  
  mForceNoHTML = ForceNoHTML;
  mpRSS = new nuiRSS(rURL, SecondsBetweenUpdates, pOriginalStream);
  mpBox = new nuiVBox();
  mpBox->SetExpand(mExpand);
  AddChild(mpBox);
  mViewSink.Connect(mpRSS->UpdateDone, &nuiRSSView::Update);
  Update(nuiEvent());
}

nuiRSSView::~nuiRSSView()
{
  delete mpRSS;
}

nuiRect nuiRSSView::CalcIdealSize()
{
  return nuiSimpleContainer::CalcIdealSize();
}

bool nuiRSSView::SetRect(const nuiRect& rRect)
{
  nuiSimpleContainer::SetRect(rRect);
  return true;
}

void nuiRSSView::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Font")), nuiUnitName,
                nuiMakeDelegate(this, &nuiRSSView::_GetFont), 
                nuiMakeDelegate(this, &nuiRSSView::_SetFont)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("TextColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiRSSView::GetTextColor), 
                nuiMakeDelegate(this, &nuiRSSView::SetTextColor)));
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("URL")), nuiUnitName,
                nuiMakeDelegate(this, &nuiRSSView::GetURL), 
                nuiMakeDelegate(this, &nuiRSSView::SetURL)));
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("ItemWidget")), nuiUnitName,
                nuiMakeDelegate(this, &nuiRSSView::GetItemWidget), 
                nuiMakeDelegate(this, &nuiRSSView::SetItemWidget)));
  
  AddAttribute(new nuiAttribute<int32>
               (nglString(_T("MaxItems")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiRSSView::GetMaxItems), 
                nuiMakeDelegate(this, &nuiRSSView::SetMaxItems)));
  
  AddAttribute(new nuiAttribute<nuiExpandMode>
               (nglString(_T("Expand")), nuiUnitPixels,
                nuiMakeDelegate(this, &nuiRSSView::GetExpand), 
                nuiMakeDelegate(this, &nuiRSSView::SetExpand)));
  
  AddAttribute(new nuiAttribute<int32>
               (nglString(_T("RefreshRate")), nuiUnitSeconds,
                nuiMakeDelegate(this, &nuiRSSView::GetRefreshRate), 
                nuiMakeDelegate(this, &nuiRSSView::SetRefreshRate)));
  
}



void nuiRSSView::Update(const nuiEvent& rEvent)
{
  mpBox->Trash();
  mpBox = new nuiVBox();
  mpBox->SetExpand(mExpand);
  AddChild(mpBox);

  int32 count = mpRSS->GetItemCount();
  count = mMaxItems >= 0 ? MIN(mMaxItems, count) : count;
  
  for (uint32 i = 0; i < count; i++)
  {
    const nuiRSSItem& rItem(mpRSS->GetItem(i));
    //printf("%s / %s\n", rItem.GetLink().GetChars(), rItem.GetTitle().GetChars());

    nglString desc(rItem.GetDescription());
    
    nuiHTML html(true);
    bool res = !desc.IsNull();
    if (res)
    {
      std::string str(desc.GetStdString());
      nglIMemory mem(&str[0], str.size());
      bool res = html.Load(mem);
    }
    
    nglString text;
    if (res)
    {
      // Contents is valid HTML
      html.GetSimpleText(text);
      //NGL_OUT(_T("%d - Could parse HTML tags:\n%s\n"), i, text.GetChars());
    }
    else
    {
      text = rItem.GetDescription();
      //NGL_OUT(_T("%d - Couldn't parse HTML tags:\n%s\n"), i, text.GetChars());
    }

    
    if (mItemWidget.IsEmpty())
    {
      nuiHyperLink* pLink = new nuiHyperLink(rItem.GetLink(), rItem.GetTitle());
      pLink->UseEllipsis(true);
      nuiFolderPane* pPane = new nuiFolderPane(pLink, true);
      
      
  //    nuiLabel* pLabel = new nuiLabel(text);
  //    pLabel->SetObjectName(_T("nuiRSSView::Description"));
  //    pLabel->SetWrapping(true);
      if (!mForceNoHTML && res)
      {
        nuiHTMLView* pLabel = new nuiHTMLView(480);
        pLabel->SetText(desc);
        pLabel->SetObjectClass(_T("nuiRSSContents"));
        if (!mFont.IsNull())
          pLabel->_SetFont(mFont);
        pLabel->SetTextColor(mTextColor);
        pPane->AddChild(pLabel);
      }
      else
      {
        nuiLabel* pLabel = new nuiLabel(text);
        pLabel->SetObjectClass(_T("nuiRSSContents"));
        pLabel->SetWrapping(true);
        pPane->AddChild(pLabel);
      }
      mpBox->AddCell(pPane);
    }
    else
    {
//      if (!mForceNoHTML && res)
//      {
//        text = desc;
//      }

      NGL_OUT(_T("AGENDA: %s\n"), rItem.GetTitle().GetChars());
      std::map<nglString, nglString> dictionnary;
      dictionnary[_T("ItemText")] = text;
      dictionnary[_T("ItemHTML")] = desc;
      dictionnary[_T("Title")] = rItem.GetTitle();
      dictionnary[_T("Link")] = rItem.GetLink();
      dictionnary[_T("Description")] = rItem.GetDescription();
      dictionnary[_T("Author")] = rItem.GetAuthor();
      dictionnary[_T("Category")] = rItem.GetCategory();
      dictionnary[_T("CategoryDomain")] = rItem.GetCategoryDomain();
      dictionnary[_T("Comments")] = rItem.GetComments();
      dictionnary[_T("EnclosureURL")] = rItem.GetEnclosureURL();
      dictionnary[_T("EnclosureType")] = rItem.GetEnclosureType();
      dictionnary[_T("GUID")] = rItem.GetGUID();
      dictionnary[_T("PublishingDate")] = rItem.GetPublishingDate();
      dictionnary[_T("SourceURL")] = rItem.GetSourceURL();
      dictionnary[_T("ImageURL")] = rItem.GetImageURL();
      if (!rItem.GetImageURL().IsEmpty())
      {
        nglString imagehtml;
        imagehtml.CFormat(_T("<img src=\"%s\" alt=\"%s\"/>"), rItem.GetImageURL().GetChars(), rItem.GetImageTitle().GetChars());
        NGL_OUT(_T("ImageHTML: %s\n"), imagehtml.GetChars());
        dictionnary[_T("ImageHTML")] = imagehtml;
      }
      else
      {
        nglString imagehtml;
        imagehtml.CFormat(_T("<img src=\"%s\" alt=\"%s\"/>"), rItem.GetLink().GetChars(), _T(""));
        imagehtml.Replace(_T("https"), _T("http"));
        NGL_OUT(_T("ImageHTML (from link): %s\n"), imagehtml.GetChars());
        dictionnary[_T("ImageHTML")] = imagehtml;
      }
      if (!rItem.GetEnclosureURL().IsEmpty())
      {
        nglString enclosurehtml;
        enclosurehtml.CFormat(_T("<img src=\"%s\" alt=\"enclosure\"/>"), rItem.GetEnclosureURL().GetChars());
        NGL_OUT(_T("EnclosureHTML: %s\n"), enclosurehtml.GetChars());
        dictionnary[_T("EnclosureHTML")] = enclosurehtml;
      }
      nuiWidget* pWidget = nuiBuilder::Get().CreateWidget(mItemWidget, dictionnary);
      if (pWidget)
        mpBox->AddCell(pWidget);
    }
    
  }
}

void nuiRSSView::ForceUpdate()
{
  mpRSS->ForceUpdate();
}

void nuiRSSView::SetURL(const nglString& rURL)
{
  mpRSS->SetURL(rURL);
}

const nglString& nuiRSSView::GetURL() const
{
  return mpRSS->GetURL();
}

void nuiRSSView::SetItemWidget(const nglString& rWidget)
{
  mItemWidget = rWidget;
  Update(nuiEvent());
}

const nglString& nuiRSSView::GetItemWidget() const
{
  return mItemWidget;
}

void nuiRSSView::SetMaxItems(int32 maxshown)
{
  mMaxItems = maxshown;
  Update(nuiEvent());
}

int32 nuiRSSView::GetMaxItems() const
{
  return mMaxItems;
}


void nuiRSSView::_SetFont(const nglString& rFontSymbol)
{
  mFont = rFontSymbol;
  ForceUpdate();  
}


const nglString& nuiRSSView::_GetFont() const
{
  return mFont;
}


const nuiColor& nuiRSSView::GetTextColor() const
{
  return mTextColor;
}


void nuiRSSView::SetTextColor(const nuiColor& Color)
{
  mTextColor = Color;
  ForceUpdate();  
}


void nuiRSSView::SetExpand(nuiExpandMode expand)
{
  mExpand = expand;
  if (mpBox)
    mpBox->SetExpand(mExpand);
}

nuiExpandMode nuiRSSView::GetExpand() const
{
  return mExpand;
}

void nuiRSSView::SetRefreshRate(int32 SecondsBetweenUpdates)
{
  return mpRSS->SetRefreshRate(SecondsBetweenUpdates);
}

int32 nuiRSSView::GetRefreshRate() const
{
  return mpRSS->GetRefreshRate();
}
