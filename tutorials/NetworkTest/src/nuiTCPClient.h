/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSocket.h"

class nuiTCPClient : public nuiSocket
{
public:
  nuiTCPClient();
  ~nuiTCPClient();
  
  bool Connect(const nuiNetworkHost& rHost);
  bool Connect(const nglString& rHost, int16 port);
  bool Connect(uint32 ipaddress, int16 port);
  
  bool Send(const std::vector<uint8>& rData);
  bool Receive(const std::vector<uint8>& rData, int maxdata = -1);
  
  bool Close();
};

