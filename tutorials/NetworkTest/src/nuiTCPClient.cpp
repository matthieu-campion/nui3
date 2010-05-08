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
#include <sys/ioctl.h>


nuiTCPClient::nuiTCPClient()
{
}

nuiTCPClient::~nuiTCPClient()
{
}

void DumpError(int err)
{
  nglString error;
  
  switch (err)
  {
    case EACCES: error = "The destination address is a broadcast address and the socket option SO_BROADCAST is not set."; break;
    case EADDRINUSE: error = "The address is already in use."; break;
    case EADDRNOTAVAIL: error = "The specified address is not available on this machine."; break;
    case EAFNOSUPPORT: error = "Addresses in the specified address family cannot be used with this socket."; break;
    case EALREADY: error = "The socket is non-blocking and a previous connection attempt has not yet been completed."; break;
    case EBADF: error = "socket is not a valid descriptor."; break;
    case ECONNREFUSED: error = "The attempt to connect was ignored (because the target is not listening for connections) or explicitly rejected."; break;
    case EFAULT: error = "The address parameter specifies an area outside the process address space."; break;
    case EHOSTUNREACH: error = "The target host cannot be reached (e.g., down, disconnected)."; break;
    case EINPROGRESS: error = "The socket is non-blocking and the connection cannot be completed immediately.  It is possible to select(2) for completion by selecting the socket for writing."; break;
    case EINTR: error = "Its execution was interrupted by a signal."; break;
    case EINVAL: error = "An invalid argument was detected (e.g., address_len is not valid for the address family, the specified address family is invalid)."; break;
    case EISCONN: error = "The socket is already connected."; break;
    case ENETDOWN: error = "The local network interface is not functioning."; break;
    case ENETUNREACH: error = "The network isn't reachable from this host."; break;
    case ENOBUFS: error = "The system call was unable to allocate a needed memory buffer."; break;
    case ENOTSOCK: error = "socket is not a file descriptor for a socket."; break;
    case EOPNOTSUPP: error = "Because socket is listening, no connection is allowed."; break;
    case EPROTOTYPE: error = "address has a different type than the socket that is bound to the specified peer address."; break;
    case ETIMEDOUT: error = " Connection establishment timed out without establishing a connection."; break;
    case ECONNRESET: error = "Remote host reset the connection request."; break;
  }
  
  NGL_OUT(_T("Socket Error: %ls\n"), error.GetChars());
}

bool nuiTCPClient::Connect(const nuiNetworkHost& rHost)
{
  if (IsValid())
    return false;

  if (!Init(AF_INET, SOCK_STREAM, 0))
    return false;

  struct addrinfo* addr = GetAddrInfo(rHost);
  int res = connect(mSocket, addr->ai_addr, addr->ai_addrlen);
  DumpError(errno);
  
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
  return Send(&rData[0], rData.size());
}

bool nuiTCPClient::Send(const uint8* pData, int len)
{
  if (!IsValid())
    return false;
  
  send(mSocket, pData, len, 0);
  return false;
}


bool nuiTCPClient::Receive(std::vector<uint8>& rData)
{
  if (!IsValid())
    return false;

  int PendingBytes = 0;
  int result = ioctl(mSocket, FIONREAD, &PendingBytes);
  if (!PendingBytes || result != 0)
    return false;
  
  rData.resize(PendingBytes);
  int res = recv(mSocket, &rData[0], rData.size(), 0);
  if (res < PendingBytes)
    rData.resize(res);
  return res != 0;
}


bool nuiTCPClient::Close()
{
  if (!IsValid())
    return false;

  
#ifdef WIN32
  DisconnectEx(mSocket, NULL, 0, 0);
  closesocket(mSocket)
#else
  close(mSocket);
#endif
  
  mSocket = -1;
  return true;
}

