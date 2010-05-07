/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiTCPClient.h"
#include "nuiNetworkHost.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


nuiTCPClient::nuiTCPClient()
{
}

nuiTCPClient::~nuiTCPClient()
{
}


bool nuiTCPClient::Connect(const nuiNetworkHost& rHost)
{
  if (IsValid())
    return false;

  if (!Init(AF_INET, SOCK_STREAM, 0))
    return false;

  struct addrinfo* addr = GetAddrInfo(rHost);
  int res = connect(mSocket, addr->ai_addr, addr->ai_addrlen);
  freeaddrinfo(addr);

  return res == 0;
}

bool nuiTCPClient::Connect(const nglString& rHost, int16 port)
{
  return Connect(nuiNetworkHost(rHost, port));
}

bool nuiTCPClient::Connect(uint32 ipaddress, int16 port)
{
  return Connect(nuiNetworkHost(ipaddress, port));
}


bool nuiTCPClient::Send(const std::vector<uint8>& rData)
{
  if (!IsValid())
    return false;
  
  return false;
}

bool nuiTCPClient::Receive(const std::vector<uint8>& rData, int maxdata)
{
  if (!IsValid())
    return false;
  
  return false;
}


bool nuiTCPClient::Close()
{
  if (!IsValid())
    return false;
  
  return false;
}

