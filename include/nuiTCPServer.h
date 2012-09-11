/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiSocket.h"

class nuiTCPClient;

class nuiTCPServer : public nuiSocket
{
public:
  nuiTCPServer();
  ~nuiTCPServer();
  
  bool Bind(const nglString& rHost, int16 port);
  bool Bind(uint32 ipaddress, int16 port);
  bool Bind(const nuiNetworkHost& rHost);
  
  bool Listen(int backlog = 10);
  
  nuiTCPClient* Accept();
  
private:
  virtual nuiTCPClient* OnCreateClient(nuiSocket::SocketType sock);

  nglString GetDesc() const;

  int64 mAcceptedCount;

};

