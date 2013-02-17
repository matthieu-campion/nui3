/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

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
  mAcceptedCount = 0;
}

nuiTCPServer::~nuiTCPServer()
{
}

bool nuiTCPServer::Bind(const nglString& rHost, int16 port)
{
  nuiNetworkHost host(rHost, port, nuiNetworkHost::eTCP);
  return Bind(host);
}

bool nuiTCPServer::Bind(uint32 ipaddress, int16 port)
{
  nuiNetworkHost host(ipaddress, port, nuiNetworkHost::eTCP);
  return Bind(host);
}

bool nuiTCPServer::Bind(const nuiNetworkHost& rHost)
{
  if (!Init(AF_INET, SOCK_STREAM, 0))
    return false;

  int option = 1;
  int res = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  if (res)
  {
    printf("setsockopt error %d\n", errno);
    DumpError(this, res, __FUNC__);
    return false;
  }
  struct addrinfo* addr = nuiSocket::GetAddrInfo(rHost);
  res = bind(mSocket, addr->ai_addr, addr->ai_addrlen);
  if (res)
    DumpError(this, res, __FUNC__);

  freeaddrinfo(addr);

  return res == 0;
}

bool nuiTCPServer::Listen(int backlog)
{
  return listen(mSocket, backlog) == 0;
}

nuiTCPClient* nuiTCPServer::Accept()
{
  int n = 1;
  int s = accept(mSocket, NULL, NULL);

  mAcceptedCount++;

  if (s >= 0)
  {
    //printf("%x accept %d\n", this, s);
    UpdateIdle();
    nuiTCPClient* pClient = OnCreateClient(s);
    return pClient;
  }

  return NULL;
}

nuiTCPClient* nuiTCPServer::OnCreateClient(nuiSocket::SocketType sock)
{
  UpdateIdle();
  return new nuiTCPClient(sock);
}

nglString nuiTCPServer::GetDesc() const
{
  nuiNetworkHost source(0, 0, nuiNetworkHost::eTCP);
  GetLocalHost(source);
  uint32 S = source.GetIP();
  uint8* s = (uint8*)&S;

  nglString str;
  str.CFormat("%5d: %s - bound %d.%d.%d.%d:%d (%d clients) [ %s ]",
              GetSocket(),
              IsNonBlocking() ? "NoBlock" : "Block  ",
              s[0], s[1], s[2], s[3], ntohs(source.GetPort()),
              mAcceptedCount,
              mName.GetChars());
  return str;
}
