/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiNetworkHost;

class nuiSocket
{
public:
  virtual ~nuiSocket();
  
  int GetSocket() const;
  
  bool GetLocalHost(nuiNetworkHost& rHost) const;
  bool GetDistantHost(nuiNetworkHost& rHost) const;
  
protected:
  nuiSocket(int Socket);
  nuiSocket(int domain, int type, int protocol);
  int mSocket;
};

