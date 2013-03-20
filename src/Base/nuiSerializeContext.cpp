/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

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