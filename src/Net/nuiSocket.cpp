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
: mSocket(Socket), mpPool(NULL)
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
  if (mpPool)
    mpPool->Del(this);

  if (mSocket > 0)
  {
#ifdef WIN32
    //DisconnectEx(mSocket, NULL, 0, 0);
    closesocket(mSocket);
#else
    close(mSocket);
#endif
  }
}

nuiSocket::SocketType nuiSocket::GetSocket() const
{
  return mSocket;
}

void nuiSocket::SetPool(nuiSocketPool* pPool)
{
  mpPool = pPool;
}

bool nuiSocket::GetLocalHost(nuiNetworkHost& rHost) const
{
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int res = getsockname(mSocket, (struct sockaddr*)&addr, &addrlen);
  if (res != 0)
    return false;

  nuiNetworkHost h(ntohl(addr.sin_addr.s_addr), addr.sin_port, rHost.mProtocol);
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
  fcntl(mSocket, F_SETFL, flags);
#else
  /* Otherwise, use the old way of doing it */
  flags = set ? 1 : 0;
  ioctl(mSocket, FIOBIO, &flags);
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
    case W(EWOULDBLOCK): error = "Would block."; break;

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

void nuiSocketPool::Add(nuiSocket* pSocket, TriggerMode Mode)
{
  pSocket->SetPool(this);
  struct kevent ev;
  memset(&ev, 0, sizeof(struct kevent));
  ev.ident = pSocket->GetSocket();
  ev.filter = EVFILT_READ;
  ev.flags = EV_ADD | EV_ENABLE;
//  if (Mode != eStateChange)
//    ev.flags |= EV_CLEAR;
  ev.udata = pSocket;
  mChangeset.push_back(ev);

  ev.filter = EVFILT_WRITE;
  mChangeset.push_back(ev);

  mEvents.resize(mEvents.size() + 2);
}

void nuiSocketPool::Del(nuiSocket* pSocket)
{
  pSocket->SetPool(NULL);
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
		NGL_LOG("socket", NGL_LOG_ERROR, "kqueue::waitForEvents : kevent : %s (errno %d)\n", strerror(err), err);
		return err;
	}

	if (res == 0)
		return EWOULDBLOCK;

  for (int i = 0; i < res; i++)
  {
    nuiSocket* pSocket = (nuiSocket*)mEvents[i].udata;
    // dispatch events:
    switch (mEvents[i].filter)
    {
      case EVFILT_READ:
        if (mEvents[i].flags & EV_EOF)
          pSocket->OnReadClosed();
        else
          pSocket->OnCanRead();
        break;

      case EVFILT_WRITE:
        if (mEvents[i].flags & EV_EOF)
          pSocket->OnWriteClosed();
        else
          pSocket->OnCanWrite();
        break;
    };
  }

	return 0;
}

#endif //NGL_KQUEUE


#ifdef NGL_EPOLL
nuiSocketPool::nuiSocketPool()
{
  mEPoll = epoll_create(100);
  mEventCount = 0;
}

nuiSocketPool::~nuiSocketPool()
{
  ///
}

void nuiSocketPool::Add(nuiSocket* pSocket, TriggerMode Mode)
{
NGL_OUT("nuiSocketPool::Add(%p, %d)\n", pSocket, Mode);
  pSocket->SetPool(this);
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
  if (Mode == eStateChange)
    ev.events |= EPOLLET;
  ev.data.ptr = pSocket;

  epoll_ctl(mEPoll, EPOLL_CTL_ADD, pSocket->GetSocket(), &ev);
  mEventCount++;
}

void nuiSocketPool::Del(nuiSocket* pSocket)
{
  NGL_LOG("socket", NGL_LOG_ERROR, "nuiSocketPool::Del() %p\n", pSocket);
  epoll_ctl(mEPoll, EPOLL_CTL_DEL, pSocket->GetSocket(), NULL);
  mEventCount--;
  if (IsInDispatch())
    mDeletedFromPool.insert(pSocket);
}

int nuiSocketPool::DispatchEvents(int timeout_millisec)
{
  SetInDispatch(true);

  if (mEventCount > mEvents.size())
    mEvents.resize(mEventCount);

  int res = epoll_wait(mEPoll, &mEvents[0], mEventCount, timeout_millisec);

	if(res == -1)
  {
		int err = errno;
		NGL_LOG("socket", NGL_LOG_ERROR, "epoll::WaitForEvents : %s (errno %d)\n", strerror(err), err);
    SetInDispatch(false);
    return err;
	}

	if (res == 0)
  {
    SetInDispatch(false);
  	return EWOULDBLOCK;
  }

  for (int i = 0; i < res; i++)
  {
    nuiSocket* pSocket = (nuiSocket*)mEvents[i].data.ptr;
    uint32_t events = mEvents[i].events;

    // dispatch events:
    bool skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    if ((events & EPOLLIN) && !skip)
    {
      NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLIN %p\n", pSocket);
      pSocket->OnCanRead();
      skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    }

    if ((events & EPOLLOUT) && !skip)
    {
      NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLOUT %p\n", pSocket);
      pSocket->OnCanWrite();
      skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    }

    if ((events & EPOLLRDHUP) && !skip)
    {
      //NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLRDHUP %p\n", pSocket);
      pSocket->OnReadClosed();
      skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    }

    if ((events & EPOLLHUP) && !skip)
    {
      NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLHUP %p\n", pSocket);
      pSocket->OnReadClosed();
      pSocket->OnWriteClosed();
      skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    }

    if ((events & EPOLLERR) && !skip)
    {
      NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLERR %p\n", pSocket);
      pSocket->OnReadClosed();
      pSocket->OnWriteClosed();
      skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    }
  }

  SetInDispatch(false);
	return 0;
}

#endif //NGL_EPOLL

bool nuiSocketPool::IsInDispatch() const
{
  return ngl_atomic_read(mInDispatch) != 0;
}

void nuiSocketPool::SetInDispatch(bool set)
{
  if (set)
    ngl_atomic_compare_and_swap(mInDispatch, 0, 1);
  else
    ngl_atomic_compare_and_swap(mInDispatch, 1, 0);
}



