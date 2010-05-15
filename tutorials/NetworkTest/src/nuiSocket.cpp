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

struct addrinfo* nuiSocket::GetAddrInfo(const nuiNetworkHost& rHost) const
{
  return rHost.GetAddrInfo();
}

void nuiSocket::DumpError(int err) const
{
  if (!err)
    return;
  
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
    default: error = "Unknown error "; error.Add(err); break;
  }
  
  NGL_OUT(_T("Socket Error: %ls\n"), error.GetChars());
}

