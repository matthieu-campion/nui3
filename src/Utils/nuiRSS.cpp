/*
 *  nuiRSS.cpp
 *  nui3
 *
 *  Created by meeloo on 6/9/09.
 *  Copyright 2009 mxp4. All rights reserved.
 *
 */

#include "nui.h"
#include "nuiRSS.h"
#include "nuiHTTP.h"
#include "nglThread.h"

#define STARTELEM for (int32 i = 0; i < pNode->GetChildrenCount(); i++) { nuiXMLNode* pN = pNode->GetChild(i); const nglString& name(pN->GetName()); 
#define GETELEM(X, STR) if (name == _T(X)) { nuiXMLNode* pp = pN->GetChild(0); STR = pp->GetValue(); printf(X ": '%ls'\n", STR.GetChars()); } else
#define GETELEMATTR(X, Y, STR) if (name == _T(X)) STR = pN->GetAttribute(_T(Y)); else
#define ENDELEM {} }

//class nuiRSSItem
nuiRSSItem::nuiRSSItem(const nuiXMLNode* pNode)
{
  STARTELEM;
  GETELEM("title", mTitle);
  GETELEM("link", mLink);
  GETELEM("description", mDescription);
  GETELEM("author", mAuthor);
  GETELEM("category", mCategory);
  //GETSUBELEM("category", "domain", mCategoryDomain);
  GETELEM("comments", mComments);
  //GETSUBELEM("enclosure", "url", mEnclosureURL);
  //GETSUBELEM("enclosure", "length", mEnclosureLength);
  //GETSUBELEM("enclosure", "type", mEnclosureType);
  GETELEM("guid", mGUID);
  GETELEM("pubDate", mPublishingDate);
  GETELEM("source", mSourceURL);
  ENDELEM;
}

nuiRSSItem::~nuiRSSItem()
{
  
}

const nglString& nuiRSSItem::GetTitle() const
{
  return mTitle;
}

const nglString& nuiRSSItem::GetLink() const
{
  return mLink;
}

const nglString& nuiRSSItem::GetDescription() const
{
  return mDescription;
}

const nglString& nuiRSSItem::GetAuthor() const
{
  return mAuthor;
}

const nglString& nuiRSSItem::GetCategory() const
{
  return mCategory;
}

const nglString& nuiRSSItem::GetCategoryDomain() const
{
  return mCategoryDomain;
}

const nglString& nuiRSSItem::GetComments() const
{
  return mComments;
}

const nglString& nuiRSSItem::GetEnclosureURL() const
{
  return mEnclosureURL;
}

int32 nuiRSSItem::GetEnclosureLength() const
{
  return mEnclosureLength;
}

const nglString& nuiRSSItem::GetEnclosureType() const
{
  return mEnclosureType;
}

const nglString& nuiRSSItem::GetGUID() const
{
  return mGUID;
}

const nglString& nuiRSSItem::GetPublishingDate() const
{
  return mPublishingDate;
}

const nglString& nuiRSSItem::GetSourceURL() const
{
  return mSourceURL;
}



/////////////////////////////////////////////////////////////////////////
// nuiRSS
nuiRSS::nuiRSS(const nglString& rURL, int32 SecondsBetweenUpdates, nglIStream* pOriginalStream)
: mSink(this), mRSSURL(rURL), mUpdating(false)
{
  mpXML = NULL;
  mpHTTPThread = NULL;
  if (pOriginalStream)
  {
    nuiXML xml;
    if (xml.Load(*pOriginalStream))
    {
      UpdateFromXML(&xml);
    }
  }
  
  mpUpdateTimer = new nuiTimer(SecondsBetweenUpdates);
  mpNotificationTimer = new nuiTimer(0.2); // Process notifications every X seconds
  
  mSink.Connect(mpUpdateTimer->Tick, &nuiRSS::TimeToUpdate);
  mSink.Connect(mpNotificationTimer->Tick, &nuiRSS::TimeToNotify);
  
  mpUpdateTimer->Start();
}

nuiRSS::~nuiRSS()
{
  delete mpUpdateTimer;
  delete mpNotificationTimer;
}

nglTime nuiRSS::GetLastUpdate() const
{
  return 0;
}

nglTime nuiRSS::GetNextUpdate() const
{
  return 0;
}

const nglString& nuiRSS::GetTitle() const
{
  return mTitle;
}

const nglString& nuiRSS::GetURL() const
{
  return mURL;
}

const nglString& nuiRSS::GetDescription() const
{
  return mDescription;
}

const nglString& nuiRSS::GetLanguage() const
{
  return mLanguage;
}

const nglString& nuiRSS::GetCopyright() const
{
  return mCopyright;
}

const nglString& nuiRSS::GetManagingEditor() const
{
  return mManagingEditor;
}

const nglString& nuiRSS::GetWebMaster() const
{
  return mWebMaster;
}

const nglString& nuiRSS::GetPublishingDate() const
{
  return mPublishingDate;
}

const nglString& nuiRSS::GetLastBuildDate() const
{
  return mLastBuildDate;
}

const nglString& nuiRSS::GetCategory() const
{
  return mCategory;
}

const nglString& nuiRSS::GetGenerator() const
{
  return mGenerator;
}

const nglString& nuiRSS::GetDocs() const
{
  return mDocs;
}

const nglString& nuiRSS::GetTimeToLive() const
{
  return mTimeToLive;
}

const nglString& nuiRSS::GetImageTitle() const
{
  return mImageTitle;
}

const nglString& nuiRSS::GetImageURL() const
{
  return mImageURL;
}

const nglString& nuiRSS::GetImageLink() const
{
  return mImageLink;
}

const nglString& nuiRSS::GetRating() const
{
  return mRating;
}

const nglString& nuiRSS::GetSkipHours() const
{
  return mSkipHours;
}

const nglString& nuiRSS::GetSkipDays() const
{
  return mSkipDays;
}


int32 nuiRSS::GetItemCount() const
{
  return mItems.size();
}

const nuiRSSItem& nuiRSS::GetItem(int32 index) const
{
  return mItems[index];
}

bool nuiRSS::GetItemRead(int32 index) const
{
  // #FIXME
  return false;
}

void nuiRSS::SetItemRead(int32 index, bool set)
{
  // #FIXME
}

bool nuiRSS::UpdateFromXML(nuiXMLNode* pXML)
{
  if (!pXML)
    return false;

  if (pXML->GetName() != _T("rss"))
    return false;
  
  mItems.clear();
  for (int32 j = 0; j < pXML->GetChildrenCount(); j++)
  {
    nuiXMLNode* pNode = pXML->GetChild(j);
    if (!pNode)
      return false;

    printf("xml item: '%ls'\n", pNode->GetName().GetChars());
    if (pNode->GetName() == _T("channel"))
    {
      
      STARTELEM;
      GETELEM("title", mTitle);
      GETELEM("url", mURL);
      GETELEM("description", mDescription);
      GETELEM("language", mLanguage);
      GETELEM("copyright", mCopyright);
      GETELEM("managingEditor", mManagingEditor);
      GETELEM("webMaster", mWebMaster);
      GETELEM("pubDate", mPublishingDate);
      GETELEM("lastBuildDate", mLastBuildDate);
      GETELEM("category", mCategory);
      GETELEM("generator", mGenerator);
      GETELEM("docs", mDocs);
      GETELEM("ttl", mTimeToLive);
      //GETSUBELEM("image", "title", mImageTitle);
      //GETSUBELEM("image", "url", mImageURL);
      //GETSUBELEM("image", "link", mImageLink);
      GETELEM("rating", mRating);
      GETELEM("skipHours", mSkipHours);
      GETELEM("skipDays", mSkipDays);
      if (name == _T("item"))
      {
        mItems.push_back(nuiRSSItem(pN));
      }
      ENDELEM;

      printf("%d item found", mItems.size());
      
      return true;
    }
    
  }
  return true;
}

void nuiRSS::StartHTTPThread()
{
  mUpdating = true;
  nglString url(mRSSURL);
  url.Replace(_T("feed://"), _T("http://"));
  nuiHTTPRequest request(url);
  nuiHTTPResponse* pResponse = request.SendRequest();
  nuiXML* pXML = new nuiXML();
  nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
  if (!pXML->Load(mem))
  {
    delete pXML;
    return;
  }
  mpXML = pXML;
}

bool nuiRSS::TimeToUpdate(const nuiEvent& rEvent)
{
  ForceUpdate();
  return false;
}

bool nuiRSS::ForceUpdate()
{
  if (mpHTTPThread)
    return false;
  
  UpdateStarted();
  mpNotificationTimer->Start();
  mpHTTPThread = new nglThreadDelegate(nuiMakeDelegate(this, &nuiRSS::StartHTTPThread));
  mpHTTPThread->Start();
  return true;
}

bool nuiRSS::TimeToNotify(const nuiEvent& rEvent)
{
  if (mUpdating && mpXML)
  {
    UpdateFromXML(mpXML);
    
    delete mpXML;
    mpXML = NULL;
    
    delete mpHTTPThread;
    mpHTTPThread = NULL;
    mUpdating = false;
    UpdateDone();
  }
  return false;
}

void nuiRSS::SetURL(const nglString& rURL)
{
  if (rURL != GetURL())
  {
    mRSSURL = rURL;
    ForceUpdate();
  }
}
