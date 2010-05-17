/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiNetworkHost.h"
#include "nuiTCPServer.h"
#include "nuiTCPClient.h"

#ifdef WIN32
#include <Ws2tcpip.h>
#undef GetAddrInfo

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif

nuiTCPServer::nuiTCPServer()
{
}

nuiTCPServer::~nuiTCPServer()
{
}

bool nuiTCPServer::Bind(const nglString& rHost, int16 port)
{
  nuiNetworkHost host(rHost, port);
  return Bind(host);
}

bool nuiTCPServer::Bind(uint32 ipaddress, int16 port)
{
  nuiNetworkHost host(ipaddress, port);
  return Bind(host);
}

bool nuiTCPServer::Bind(const nuiNetworkHost& rHost)
{
  if (!Init(AF_INET, SOCK_STREAM, 0))
    return false;
  
  struct addrinfo* addr = nuiSocket::GetAddrInfo(rHost);
  int res = bind(mSocket, addr->ai_addr, addr->ai_addrlen);
  if (res)
    DumpError(errno);
  
  freeaddrinfo(addr);
  
  return res == 0;
}

bool nuiTCPServer::Listen(int backlog)
{
  return listen(mSocket, backlog) == 0;
}

nuiTCPClient* nuiTCPServer::Accept()
{
  int s = accept(mSocket, NULL, NULL);
  nuiTCPClient* pClient = new nuiTCPClient(s);
  return pClient;
}

bool nuiTCPServer::Close()
{
#ifdef WIN32
  return 0 == closesocket(mSocket);
#else
  return 0 == close(mSocket);
#endif
}

