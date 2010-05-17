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
#ifdef WIN32
  typedef SOCKET SocketType;
#else
  typedef int SocketType;
#endif

  virtual ~nuiSocket();
  
  SocketType GetSocket() const;
  
  bool GetLocalHost(nuiNetworkHost& rHost) const;
  bool GetDistantHost(nuiNetworkHost& rHost) const;

  bool IsValid() const;
  
protected:
  nuiSocket(SocketType Socket = -1);
  bool Init(int domain, int type, int protocol);
  
  struct addrinfo* GetAddrInfo(const nuiNetworkHost& rHost) const;
  void DumpError(int err) const;
  
  SocketType mSocket;
};

