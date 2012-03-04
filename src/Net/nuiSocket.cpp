/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSocket.h"
#include "nuiNetworkHost.h"

#ifdef WIN32
#include <Ws2tcpip.h>
#undef GetAddrInfo
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif


nuiSocket::nuiSocket(nuiSocket::SocketType Socket)
: mSocket(Socket)
{
#if (!defined _LINUX_)
  int n = 0;
  setsockopt(mSocket, SOL_SOCKET, SO_NOSIGPIPE, &n, sizeof(n));
#endif
  mNonBlocking = false;
}

bool nuiSocket::Init(int domain, int type, int protocol)
{
  mSocket = socket(domain, type, protocol);
#if (!defined _LINUX_)
  int n = 0;
  setsockopt(mSocket, SOL_SOCKET, SO_NOSIGPIPE, &n, sizeof(n));
#endif
  return mSocket >= 0;
}

nuiSocket::~nuiSocket()
{
#ifdef WIN32
  //DisconnectEx(mSocket, NULL, 0, 0);
  closesocket(mSocket);
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

  nuiNetworkHost h(addr.sin_addr.s_addr, addr.sin_port, rHost.mProtocol);
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

  nuiNetworkHost h(ntohl(addr.sin_addr.s_addr), addr.sin_port, rHost.mProtocol);
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

void nuiSocket::SetNonBlocking(bool set)
{
  if (mNonBlocking == set)
    return;
  
  int flags;
  mNonBlocking = set;
  
  /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
  /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
  if (-1 == (flags = fcntl(mSocket, F_GETFL, 0)))
    flags = 0;
  if (set)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;
  return fcntl(mSocket, F_SETFL, flags);
#else
  /* Otherwise, use the old way of doing it */
  flags = set ? 1 : 0;
  return ioctl(mSocket, FIOBIO, &flags);
#endif
}

bool nuiSocket::IsNonBlocking() const
{
  return mNonBlocking;
}

void nuiSocket::SetCanReadDelegate(const EventDelegate& rDelegate)
{
  mReadDelegate = rDelegate;
}

void nuiSocket::SetCanWriteDelegate(const EventDelegate& rDelegate)
{
  mWriteDelegate = rDelegate;
}

void nuiSocket::SetReadClosedDelegate(const EventDelegate& rDelegate)
{
  mReadCloseDelegate = rDelegate;
}

void nuiSocket::SetWriteClosedDelegate(const EventDelegate& rDelegate)
{
  mWriteCloseDelegate = rDelegate;
}

void nuiSocket::OnCanRead()
{
  if (mReadDelegate)
    mReadDelegate(*this);
}

void nuiSocket::OnCanWrite()
{
  if (mWriteDelegate)
    mWriteDelegate(*this);
}

void nuiSocket::OnReadClosed()
{
  if (mReadCloseDelegate)
    mReadCloseDelegate(*this);
}

void nuiSocket::OnWriteClosed()
{
  if (mWriteCloseDelegate)
    mWriteCloseDelegate(*this);
}



#ifdef WIN32
#define W(X) WSA##X
#else
#define W(X) X
#endif

void nuiSocket::DumpError(int err) const
{
  if (!err)
    return;

  nglString error;

  switch (err)
  {
    case EACCES: error = "The destination address is a broadcast address and the socket option SO_BROADCAST is not set."; break;
    case W(EADDRINUSE): error = "The address is already in use."; break;
    case W(EADDRNOTAVAIL): error = "The specified address is not available on this machine."; break;
    case W(EAFNOSUPPORT): error = "Addresses in the specified address family cannot be used with this socket."; break;
    case W(EALREADY): error = "The socket is non-blocking and a previous connection attempt has not yet been completed."; break;
    case W(EBADF): error = "socket is not a valid descriptor."; break;
    case W(ECONNREFUSED): error = "The attempt to connect was ignored (because the target is not listening for connections) or explicitly rejected."; break;
    case W(EFAULT): error = "The address parameter specifies an area outside the process address space."; break;
    case W(EHOSTUNREACH): error = "The target host cannot be reached (e.g., down, disconnected)."; break;
    case W(EINPROGRESS): error = "The socket is non-blocking and the connection cannot be completed immediately.  It is possible to select(2) for completion by selecting the socket for writing."; break;
    case W(EINTR): error = "Its execution was interrupted by a signal."; break;
    case W(EINVAL): error = "An invalid argument was detected (e.g., address_len is not valid for the address family, the specified address family is invalid)."; break;
    case W(EISCONN): error = "The socket is already connected."; break;
    case W(ENETDOWN): error = "The local network interface is not functioning."; break;
    case W(ENETUNREACH): error = "The network isn't reachable from this host."; break;
    case W(ENOBUFS): error = "The system call was unable to allocate a needed memory buffer."; break;
    case W(ENOTSOCK): error = "socket is not a file descriptor for a socket."; break;
    case W(EOPNOTSUPP): error = "Because socket is listening, no connection is allowed."; break;
    case W(EPROTOTYPE): error = "address has a different type than the socket that is bound to the specified peer address."; break;
    case W(ETIMEDOUT): error = " Connection establishment timed out without establishing a connection."; break;
    case W(ECONNRESET): error = "Remote host reset the connection request."; break;
    default: error = "Unknown error "; error.Add(err); break;
  }

  NGL_OUT(_T("Socket Error: %s\n"), error.GetChars());
}













//////////////////////////////////////////////////////////////////
//class nuiSocketPool
#ifdef NGL_KQUEUE
nuiSocketPool::nuiSocketPool()
{
  mQueue = kqueue();
}

nuiSocketPool::~nuiSocketPool()
{
  ///
}

void nuiSocketPool::Add(nuiSocket* pSocket)
{
  struct kevent ev;
  memset(&ev, 0, sizeof(struct kevent));
  ev.ident = pSocket->GetSocket();
  ev.filter = EVFILT_READ;
  ev.flags = EV_ADD | EV_ENABLE;
  ev.udata = pSocket;
  
  mChangeset.push_back(ev);
  ev.filter = EVFILT_WRITE;
  mChangeset.push_back(ev);

  mEvents.resize(mEvents.size() + 2);
}

void nuiSocketPool::Del(nuiSocket* pSocket)
{
  struct kevent ev;
  memset(&ev, 0, sizeof(struct kevent));
  ev.ident = pSocket->GetSocket();
  ev.filter = EVFILT_READ;
  ev.flags = EV_DELETE;
  ev.udata = pSocket;
  
  mChangeset.push_back(ev);
  ev.filter = EVFILT_WRITE;
  mChangeset.push_back(ev);
  
  mEvents.resize(mEvents.size() - 2);
}

int nuiSocketPool::DispatchEvents(int timeout_millisec)
{
	struct timespec to;
  
	if (timeout_millisec >= 0)
  {
		to.tv_sec = timeout_millisec / 1000;
		to.tv_nsec = (timeout_millisec % 1000) * 1000000;	// nanosec
	}
  
	int res = kevent(mQueue, &mChangeset[0], mChangeset.size(), &mEvents[0], mEvents.size(), (timeout_millisec >= 0) ? &to : (struct timespec *) 0);
  
  mChangeset.clear();
	if(res == -1)
  {
		int err = errno;
		NGL_LOG("socket", NGL_LOG_ERROR, "Poller_kqueue::waitForEvents : kevent : %s (errno %d)\n", strerror(err), err);
		return err;
	}

	if (res == 0)
		return EWOULDBLOCK;
  
  for (int i = 0; i < res; i++)
  {
    // dispatch events:
    switch (mEvents[i].filter)
    {
      case EVFILT_READ:
        if (mEvents[i].flags == EV_EOF)
          pSocket->OnReadClosed();
        else
          pSocket->OnCanRead();
        break;
      
      case EVFILT_WRITE:
        if (mEvents[i].flags == EV_EOF)
          pSocket->OnWriteClosed();
        else
          pSocket->OnCanWrite();
        break;
    };
  }
  
	return 0;
}
  
#endif //NGL_KQUEUE

