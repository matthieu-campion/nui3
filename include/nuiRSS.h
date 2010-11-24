/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiTimer;
class nuiXML;
class nglThreadDelegate;

class nuiRSSItem
{
public:
  nuiRSSItem(const nuiXMLNode* pNode);
  virtual ~nuiRSSItem();
  
  const nglString& GetTitle() const;
  const nglString& GetLink() const;
  const nglString& GetDescription() const;
  const nglString& GetAuthor() const;
  const nglString& GetCategory() const;
  const nglString& GetCategoryDomain() const;
  const nglString& GetComments() const;
  const nglString& GetEnclosureURL() const;
  int32 GetEnclosureLength() const;
  const nglString& GetEnclosureType() const;
  const nglString& GetGUID() const;
  const nglString& GetPublishingDate() const;
  const nglString& GetSourceURL() const;
  const nglString& GetImageTitle() const;
  const nglString& GetImageURL() const;
  const nglString& GetImageLink() const;
  
private:
  
  nglString mTitle;
  nglString mLink;
  nglString mDescription;
  nglString mAuthor;
  nglString mCategory;
  nglString mCategoryDomain;
  nglString mComments;
  nglString mEnclosureURL;
  int32 mEnclosureLength;
  nglString mEnclosureType;
  nglString mGUID;
  nglString mPublishingDate;
  nglString mSourceURL;
  nglString mImageURL;
  nglString mImageTitle;
  nglString mImageLink;
};

class nuiRSS
{
public:
  nuiRSS(const nglString& rURL, int32 SecondsBetweenUpdates = (30*60), nglIStream* pOriginalStream = NULL);
  virtual ~nuiRSS();
  
  nglTime GetLastUpdate() const;
  nglTime GetNextUpdate() const;
  
  const nglString& GetTitle() const;
  const nglString& GetURL() const;
  const nglString& GetDescription() const;
  const nglString& GetLanguage() const;
  const nglString& GetCopyright() const;
  const nglString& GetManagingEditor() const;
  const nglString& GetWebMaster() const;
  const nglString& GetPublishingDate() const;
  const nglString& GetLastBuildDate() const;
  const nglString& GetCategory() const;
  const nglString& GetGenerator() const;
  const nglString& GetDocs() const;
  //const nglString& GetCloud() const;
  const nglString& GetTimeToLive() const;
  const nglString& GetImageTitle() const;
  const nglString& GetImageURL() const;
  const nglString& GetImageLink() const;
  const nglString& GetRating() const;
  //const nglString& GetTextInput() const;
  const nglString& GetSkipHours() const;
  const nglString& GetSkipDays() const;
  
  int32 GetItemCount() const;
  const nuiRSSItem& GetItem(int32 index) const;
  
  bool GetItemRead(int32 index) const;
  void SetItemRead(int32 index, bool set);
  
  void SetURL(const nglString& rURL);
  const nglString& GetRSSURL() const;
  bool ForceUpdate();

  void SetRefreshRate(int32 SecondsBetweenUpdates);
  int32 GetRefreshRate() const;
  
  
  nuiSimpleEventSource<0> UpdateStarted;
  nuiSimpleEventSource<0> UpdateDone;
protected:
  nglString mRSSURL;
  nuiTimer* mpUpdateTimer;
  void TimeToUpdate(const nuiEvent& rEvent);
  
  nuiTimer* mpNotificationTimer;
  void TimeToNotify(const nuiEvent& rEvent);

  nglThreadDelegate* mpHTTPThread;
  void StartHTTPThread();
  
  nglString mTitle;
  nglString mURL;
  nglString mDescription;
  nglString mLanguage;
  nglString mCopyright;
  nglString mManagingEditor;
  nglString mWebMaster;
  nglString mPublishingDate;
  nglString mLastBuildDate;
  nglString mCategory;
  nglString mGenerator;
  nglString mDocs;
  nglString mTimeToLive;
  nglString mImageTitle;
  nglString mImageURL;
  nglString mImageLink;
  nglString mRating;
  nglString mSkipHours;
  nglString mSkipDays;
  
  std::vector<nuiRSSItem> mItems;

  bool UpdateFromXML(nuiXMLNode* pNode);
  
  nuiEventSink<nuiRSS> mSink;
  
  bool mUpdating;
  nuiXML* mpXML;
  
  int32 mRefreshRate;
};

