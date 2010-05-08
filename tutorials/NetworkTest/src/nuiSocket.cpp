/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSocket.h"
#include "nuiNetworkHost.h"

nuiSocket::nuiSocket(nuiSocket::SocketType Socket)
: mSocket(Socket)
{
}

bool nuiSocket::Init(int domain, int type, int protocol)
{
  mSocket = socket(domain, type, protocol);
  return mSocket >= 0;
}

nuiSocket::~nuiSocket()
{
#ifdef WIN32
  DisconnectEx(mSocket, NULL, 0, 0);
  closesocket(mSocket)
#else
  close(mSocket);
#endif
}

nuiSocket::SocketType nuiSocket::GetSocket() const
{
  return mSocket;
}

bool nuiSocket::GetLocalHost(nuiNetworkHost& rHost) const
{
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int res = getsockname(mSocket, (struct sockaddr*)&addr, &addrlen);
  if (res != 0)
    return false;
  
  nuiNetworkHost h(addr.sin_addr.s_addr, addr.sin_port);
  rHost = h;
  return true;
}

bool nuiSocket::GetDistantHost(nuiNetworkHost& rHost) const
{
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int res = getpeername(mSocket, (struct sockaddr*)&addr, &addrlen);
  if (res != 0)
    return false;

  nuiNetworkHost h(addr.sin_addr.s_addr, addr.sin_port);
  rHost = h;
  return true;
}


bool nuiSocket::IsValid() const
{
  return mSocket != -1;
}

struct addrinfo* nuiSocket::GetAddrInfo(const nuiNetworkHost& rHost)
{
  return rHost.GetAddrInfo();
}
