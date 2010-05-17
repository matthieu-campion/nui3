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
  bool Send(const uint8* pData, int len);
  bool ReceiveAvailable(std::vector<uint8>& rData); ///< Fill the rData vector with the data that is already available to read on the socket.
  bool Receive(std::vector<uint8>& rData); ///< Fill the buffer up to its current size.
  
  bool IsConnected() const;
  int32 GetAvailable() const;
  bool CanWrite() const;
  
  bool Close();
  
protected:
  friend class nuiTCPServer;
  nuiTCPClient(int sock);
  bool mConnected;
};

