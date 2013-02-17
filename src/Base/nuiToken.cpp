/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

template<> nuiTokenType nuiTokenTrait<int32>::GetType()
{
  return eTokenInt32;
}

template<> nuiTokenType nuiTokenTrait<uint32>::GetType()
{
  return eTokenUInt32;
}

template<> nuiTokenType nuiTokenTrait<int64>::GetType()
{
  return eTokenInt64;
}

template<> nuiTokenType nuiTokenTrait<uint64>::GetType()
{
  return eTokenUInt64;
}

template<> nuiTokenType nuiTokenTrait<float>::GetType()
{
  return eTokenFloat;
}

template<> nuiTokenType nuiTokenTrait<double>::GetType()
{
  return eTokenDouble;
}

template<> nuiTokenType nuiTokenTrait<nglString>::GetType()
{
  return eTokenString;
}

