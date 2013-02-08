/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiArchive
{
public:
  nuiArchive(bool Loading);
  virtual ~nuiArchive();
  
  virtual bool OpenGroup(const nglString& rName) = 0;
  virtual void CloseGroup() = 0;
  
  virtual void Bind(bool& rValue, const nglString& rName) = 0;

  virtual void Bind(float& rValue, const nglString& rName) = 0;
  virtual void Bind(double& rValue, const nglString& rName) = 0;
  
  virtual void Bind(int8& rValue, const nglString& rName) = 0;
  virtual void Bind(int16& rValue, const nglString& rName) = 0;
  virtual void Bind(int32& rValue, const nglString& rName) = 0;
  virtual void Bind(int64& rValue, const nglString& rName) = 0;

  virtual void Bind(uint8& rValue, const nglString& rName) = 0;
  virtual void Bind(uint16& rValue, const nglString& rName) = 0;
  virtual void Bind(uint32& rValue, const nglString& rName) = 0;
  virtual void Bind(uint64& rValue, const nglString& rName) = 0;
  
  virtual void Bind(nglString& rValue, const nglString& rName) = 0;
  
  template <typename Type>
  void Bind(Type& rValue, const nglString& rName)
  {
    if (!rName.IsNull())
    {
      if (!OpenGroup(rName))
        return;
    }

    rValue.Bind(*this);
  
    if (!rName.IsNull())
    {
      CloseGroup();
    }
  }
  
  bool IsLoading() const;
  bool IsSaving() const;
  
protected:
  bool mLoading;
};


class nuiArchive_XML_Loader : public nuiArchive
{
public:
  
  nuiArchive_XML_Loader(nuiXMLNode* pRootNode) : nuiArchive(false)
  {
    mpRootNode = pRootNode;
    mpCurrentNode = pRootNode;
  }
  
  virtual ~nuiArchive_XML_Loader()
  {
    
  }
  
  virtual bool OpenGroup(const nglString& rName)
  {
    nuiXMLNode* pNode = mpCurrentNode->GetChild(mChildIndex);

    if (pNode && pNode->GetName() == rName)
    {
      mStack.push_back(mChildIndex);
      mpCurrentNode = pNode;
      mChildIndex = 0;
      return true;
    }

    return false;
  }
  
  virtual void CloseGroup()
  {
    mpCurrentNode = mpCurrentNode->GetParent();
    mChildIndex = mStack.back();
    mChildIndex++;
    mStack.pop_back();
  }
  
  virtual void Bind(bool& rValue, const nglString& rName)
  {
    nuiGetBool(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(float& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(double& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }


  virtual void Bind(int8& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(int16& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(int32& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(int64& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }


  virtual void Bind(uint8& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(uint16& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(uint32& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }

  virtual void Bind(uint64& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }


  virtual void Bind(nglString& rValue, const nglString& rName)
  {
    nuiGet(mpCurrentNode, rName, rValue);
  }


  template <typename Type>
  void Bind(Type& rValue, const nglString& rName)
  {
    if (!rName.IsNull())
    {
      if (!OpenGroup(rName))
      {
        NGL_LOG("nuiArchive_XML_Loader", NGL_LOG_ERROR, "'%s' element not found", rName.GetChars());
        return;
      }
    }
    
    rValue.Bind(*this);
    
    if (!rName.IsNull())
      CloseGroup();
  }
  

protected:
  nuiXMLNode* mpRootNode;
  nuiXMLNode* mpCurrentNode;
  uint32 mChildIndex;
  std::list<uint32> mStack;
};

class nuiArchive_XML_Saver : public nuiArchive
{
public:

  nuiArchive_XML_Saver(nuiXMLNode* pRootNode) : nuiArchive(false)
  {
    mpRootNode = pRootNode;
    mpCurrentNode = pRootNode;
  }

  virtual ~nuiArchive_XML_Saver()
  {
  }
  
  virtual bool OpenGroup(const nglString& rName)
  {
    nuiXMLNode* pGroup = new nuiXMLNode(rName);
    mpCurrentNode->AddChild(pGroup);
    mpCurrentNode = pGroup;
    return true;
  }
  
  virtual void CloseGroup()
  {
    nuiXMLNode* pGroup = mpCurrentNode->GetParent();
    mpCurrentNode = pGroup;
  }
  
  virtual void Bind(bool& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(float& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }

  virtual void Bind(double& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  
  virtual void Bind(int8& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(int16& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(int32& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(int64& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  
  virtual void Bind(uint8& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(uint16& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(uint32& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  virtual void Bind(uint64& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  
  virtual void Bind(nglString& rValue, const nglString& rName)
  {
    mpCurrentNode->SetAttribute(rName, rValue);
  }
  
  template <typename Type>
  void Bind(Type& rValue, const nglString& rName)
  {
    if (!rName.IsNull())
    {
      OpenGroup(rName);
    }
    
    rValue.Bind(*this);
    
    if (!rName.IsNull())
    {
      CloseGroup();
    }
  }
  

protected:
  nuiXMLNode* mpRootNode;
  nuiXMLNode* mpCurrentNode;
  
};

