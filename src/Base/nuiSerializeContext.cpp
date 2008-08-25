/*
 *  nuiSerializeContext.cpp
 *  nui3
 *
 *  Created by Sebastien Metrot on 13/2/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "nui.h"
#include "nuiSerializeContext.h"

nuiSerializeContext::nuiSerializeContext(nuiXMLNode* pNode, nuiSerializeOperation Operation, bool recursive)
: nuiObject()
{
  SetObjectClass(_T("nuiSerializeContext"));
  mpNode = pNode;
  mRecursive = recursive;
  mOperation = Operation;
}

nuiSerializeContext::nuiSerializeContext(const nuiSerializeContext& rContext)
{
  mpNode = rContext.mpNode;
  mRecursive = rContext.mRecursive;
  mOperation = rContext.mOperation;
  
  CopyProperties(rContext);
}

nuiSerializeContext::~nuiSerializeContext()
{
  // Nothing special to do...
}

nuiXMLNode* nuiSerializeContext::operator->() const
{
  return mpNode;
}

nuiSerializeContext& nuiSerializeContext::operator=(nuiXMLNode* pNode)
{
  mpNode = pNode;
  return *this;
}

nuiSerializeContext::operator nuiXMLNode*() const
{
  return mpNode;
}

bool nuiSerializeContext::IsRecursive() const
{
  return mRecursive;
}

void nuiSerializeContext::SetRecursive(bool set)
{
  mRecursive = set;
}

const nglPath& nuiSerializeContext::GetPath() const
{
  return mPath;
}

const nglPath& nuiSerializeContext::GetResourcePath() const
{
  return mResourcePath;
}

void nuiSerializeContext::SetPath(const nglPath& rPath)
{
  mPath = rPath;
}

void nuiSerializeContext::SetResourcePath(const nglPath& rPath)
{
  mResourcePath = rPath;
}

bool nuiSerializeContext::IsSaveAs() const
{
  return mOperation == eSaveAs;
}

nuiSerializeOperation nuiSerializeContext::GetOperation() const
{
  return mOperation;
}

void nuiSerializeContext::AddErrorComment(const nglString& error)
{
  mErrorComment += error;
}

const nglString& nuiSerializeContext::GetErrorComments() const
{
  return mErrorComment;
}