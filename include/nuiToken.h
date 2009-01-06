/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

enum nuiTokenType
{
  eTokenUnkown,
  eTokenInt32,
  eTokenInt64,
  eTokenUInt32,
  eTokenUInt64,
  eTokenFloat,
  eTokenDouble,
  eTokenPointer,
  eTokenString
};

template<class T> class nuiTokenTrait
{
public:
  static nuiTokenType GetType()
  {
    return eTokenUnkown;
  }
};

class nuiTokenBase
{
public:
  nuiTokenBase()
  {
    mType = eTokenUnkown;
  }

  virtual ~nuiTokenBase()
  {
  }
  
  nuiTokenType GetType() const
  {
    return mType;
  }
  
protected:
  nuiTokenType mType;
};

template <class T> class nuiToken : public nuiTokenBase
{
public:
  nuiToken(const T& rToken)
  : Token(rToken)
  {
    mType = nuiTokenTrait<T>::GetType();
  }
  
  const T Token; 
};

template <typename T> class nuiToken<T*> : public nuiTokenBase
{
public:
  nuiToken(T* pToken)
  : Token(pToken)
  {
    mType = eTokenPointer;
  }
  
  T* const Token; 
};


class nuiTokenHolder
{
public:
  nuiTokenHolder()
  {
    mpToken = NULL;
  }
  
  virtual ~nuiTokenHolder()
  {
    delete mpToken;
  }
  
  void SetToken(nuiTokenBase* pToken)
  {
    if (mpToken != pToken)
      delete mpToken;
    mpToken = pToken;
  }
  
  nuiTokenBase* GetToken() const
  {
    return mpToken;  
  }
private:
  nuiTokenBase* mpToken;
};


 
template <class T>
bool nuiGetTokenValue (nuiTokenBase* pTokenBase, T& rValue)
{
  nuiToken<T>* pToken = dynamic_cast<nuiToken<T>*>(pTokenBase);
  
  if (pToken)
  {
    rValue = pToken->Token;
    return true;
  }
  
  return false;
}

