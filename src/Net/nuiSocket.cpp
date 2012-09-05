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

#define __FUNC__ "%s:%d",__FILE__,__LINE__

void nuiSocket::DumpError(int err, const char* msg, ...)
{
  if (!err)
    return;

  nglString error(strerror(errno));

  if (msg)
  {
    nglString m;
    va_list args;

    va_start(args, msg);
    m.Formatv(msg, args);
    va_end(args);

    NGL_OUT(_T("[%s] Socket Error: %s\n"), m.GetChars(), error.GetChars());
  }
  else
    NGL_OUT(_T("Socket Error: %s\n"), error.GetChars());
}

void nuiSocket::DumpError(int err)
{
  DumpError(err, "?");
}



nuiSocket::nuiSocket(nuiSocket::SocketType Socket)
: mSocket(Socket), mpPool(NULL)
{
  mNonBlocking = false;
#if (!defined _LINUX_)
  if (Socket != -1)
  {
    int n = 0;
    int res = setsockopt(mSocket, SOL_SOCKET, SO_NOSIGPIPE, &n, sizeof(n));
    DumpError(res, __FUNC__);
  }
#endif
}

bool nuiSocket::Init(int domain, int type, int protocol)
{
  mSocket = socket(domain, type, protocol);
#if (!defined _LINUX_)
  int n = 0;
  int res = setsockopt(mSocket, SOL_SOCKET, SO_NOSIGPIPE, &n, sizeof(n));
  DumpError(res, __FUNC__);
#endif
  return mSocket >= 0;
}

nuiSocket::~nuiSocket()
{
  Close();
}

void nuiSocket::Close()
{
  if (mpPool)
    mpPool->Del(this);

  if (mSocket > 0)
  {
    int res = 0;
//    int res = shutdown(mSocket, SHUT_RDWR);
//    DumpError(res, __FUNC__);
#ifdef WIN32
    //DisconnectEx(mSocket, NULL, 0, 0);
    res = closesocket(mSocket);
#else
    res = close(mSocket);
#endif
    DumpError(res, __FUNC__);
  }

  mSocket = -1;
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
  DumpError(res, __FUNC__);

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
  DumpError(res, __FUNC__);

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
  int res = ioctl(mSocket, FIOBIO, &flags);
  DumpError(res, __FUNC__);
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















//////////////////////////////////////////////////////////////////
//class nuiSocketPool
#ifdef NGL_KQUEUE
nuiSocketPool::nuiSocketPool()
{
  mQueue = kqueue();
  mNbSockets = 0;
  mInDispatch = 0;
}

nuiSocketPool::~nuiSocketPool()
{
  if (mQueue > 0)
    close(mQueue);
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
  int res = kevent(mQueue, &ev, 1, NULL, 0, 0);
  nuiSocket::DumpError(res, "nuiSocketPool::Add 1");

  ev.filter = EVFILT_WRITE;
  res = kevent(mQueue, &ev, 1, NULL, 0, 0);
  nuiSocket::DumpError(res, "nuiSocketPool::Add 2");

  mNbSockets++;
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

  int res = kevent(mQueue, &ev, 1, NULL, 0, 0);
  nuiSocket::DumpError(res, "nuiSocketPool::Del 1");

  ev.filter = EVFILT_WRITE;
  res = kevent(mQueue, &ev, 1, NULL, 0, 0);
  nuiSocket::DumpError(res, "nuiSocketPool::Del 2");

  mNbSockets--;

  {
    //nglCriticalSectionGuard g(mCS);
    mDeletedFromPool.insert(pSocket);
  }
}

int nuiSocketPool::DispatchEvents(int timeout_millisec)
{
  SetInDispatch(true);

  mEvents.resize(mNbSockets * 2);

  struct timespec to;

  if (timeout_millisec >= 0)
  {
    to.tv_sec = timeout_millisec / 1000;
    to.tv_nsec = (timeout_millisec % 1000) * 1000000; // nanosec
  }

  int res = kevent(mQueue, NULL, 0, &mEvents[0], mEvents.size(), (timeout_millisec >= 0) ? &to : (struct timespec *) 0);

  if(res == -1)
  {
    nuiSocket::DumpError(res, "kqueue::waitForEvents");
    SetInDispatch(false);
    if (errno == EINTR)
    {
      //mQueue = kqueue();
      return 0;
    }
    return -1;
  }

  if (res == 0)
    return EWOULDBLOCK;

  std::set<nuiSocket*>::iterator it;
  std::set<nuiSocket*>::iterator end;
  {
    //nglCriticalSectionGuard g(mCS);
    end = mDeletedFromPool.end();
  }

  for (int i = 0; i < res; i++)
  {
    nuiSocket* pSocket = (nuiSocket*)mEvents[i].udata;

    {
      //nglCriticalSectionGuard g(mCS);
      it = mDeletedFromPool.find(pSocket);
    }

    if (it == end)
    {
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
      }
    }
  }

  {
    //nglCriticalSectionGuard g(mCS);
    mDeletedFromPool.clear();
  }

  SetInDispatch(false);
  return 0;
}

#endif //NGL_KQUEUE


#ifdef NGL_EPOLL
nuiSocketPool::nuiSocketPool()
{
  NGL_OUT("nuiSocketPool::nuiSocketPool()");
  mEPoll = epoll_create(100);
  NGL_OUT("nuiSocketPool::nuiSocketPool() Create OK");
  mNbSockets = 0;
  mInDispatch = 0;
  NGL_OUT("nuiSocketPool::nuiSocketPool() DONE OK");
}

nuiSocketPool::~nuiSocketPool()
{
  if (mEPoll > 0)
    close(mEPoll);
}

void nuiSocketPool::Add(nuiSocket* pSocket, TriggerMode Mode)
{
  NGL_OUT("nuiSocketPool::Add(%p, %d, %d)\n", pSocket, Mode, pSocket->GetSocket());
  pSocket->SetPool(this);
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
  if (Mode == eStateChange)
    ev.events |= EPOLLET;
  ev.data.ptr = pSocket;

  int res = epoll_ctl(mEPoll, EPOLL_CTL_ADD, pSocket->GetSocket(), &ev);

  if (res != 0)
  {
    nuiSocket::DumpError(res, "epoll::Add");
  }

  mNbSockets++;
}

void nuiSocketPool::Del(nuiSocket* pSocket)
{
  pSocket->SetPool(NULL);
  NGL_LOG("socket", NGL_LOG_ERROR, "nuiSocketPool::Del(%p, %d)\n", pSocket, pSocket->GetSocket());

  int res = epoll_ctl(mEPoll, EPOLL_CTL_DEL, pSocket->GetSocket(), NULL);

  if (res != 0)
  {
    nuiSocket::DumpError(res, "epoll::Del");
  }


  mNbSockets--;
  if (IsInDispatch())
  {
    //nglCriticalSectionGuard g(mCS);
    mDeletedFromPool.insert(pSocket);
  }
}

int nuiSocketPool::DispatchEvents(int timeout_millisec)
{
  SetInDispatch(true);

  if (mNbSockets > mEvents.size())
    mEvents.resize(mNbSockets);

  //struct epoll_event evt;
  //int res = epoll_wait(mEPoll, &mEve

  //int res = epoll_wait(mEPoll, &evt, 1, timeout_millisec);
  int res = epoll_wait(mEPoll, &mEvents[0], mEvents.size(), timeout_millisec);

  if(res == -1)
  {
    nuiSocket::DumpError(res, "epoll::WaitForEvents");
    SetInDispatch(false);
    {
      //nglCriticalSectionGuard g(mCS);
      mDeletedFromPool.clear();
    }
    return errno;
  }

  if (res == 0)
  {
    SetInDispatch(false);
    {
      //nglCriticalSectionGuard g(mCS);
      mDeletedFromPool.clear();
    }

    return EWOULDBLOCK;
  }

  //NGL_LOG("socket", NGL_LOG_ERROR, "epoll::WaitForEvents got %d events\n", res);

  for (int i = 0; i < res; i++)
  {
    nuiSocket* pSocket = (nuiSocket*)mEvents[i].data.ptr;
    uint32_t events = mEvents[i].events;

    //NGL_OUT("socket", NGL_LOG_INFO, "epoll event s = %d  p = %p f = %x", mEvents[i].data.fd, pSocket, events);

    //NGL_LOG("socket", NGL_LOG_ERROR, "epoll_wait [%d] %d/%d %p\n", res, pSocket->GetSocket(), mEvents[i].data.fd, pSocket);

    // dispatch events:
    bool skip = false;
    {
      //nglCriticalSectionGuard g(mCS);
      skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
    }

    if ((events & EPOLLIN) && !skip)
    {
      //NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLIN %p, %d\n", pSocket, pSocket->GetSocket());
      pSocket->OnCanRead();

      {
        //nglCriticalSectionGuard g(mCS);
        skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
      }
    }

    if ((events & EPOLLOUT) && !skip)
    {
      //NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLOUT %p, %d\n", pSocket, pSocket->GetSocket());
      pSocket->OnCanWrite();
      {
        //nglCriticalSectionGuard g(mCS);
        skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
      }
    }

    if ((events & EPOLLRDHUP) && !skip)
    {
      //NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLRDHUP %p, %d\n", pSocket, pSocket->GetSocket());
      pSocket->OnReadClosed();
      {
        //nglCriticalSectionGuard g(mCS);
        skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
      }
    }

    if ((events & EPOLLHUP) && !skip)
    {
      //NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLHUP %p, %d\n", pSocket, pSocket->GetSocket());
      pSocket->OnReadClosed();
      pSocket->OnWriteClosed();
      {
        //nglCriticalSectionGuard g(mCS);
        skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
      }
    }

    if ((events & EPOLLERR) && !skip)
    {
      //NGL_LOG("socket", NGL_LOG_ERROR, "EPOLLERR %p, %d\n", pSocket, pSocket->GetSocket());
      pSocket->OnReadClosed();
      pSocket->OnWriteClosed();
      {
        //nglCriticalSectionGuard g(mCS);
        skip = (mDeletedFromPool.find(pSocket) != mDeletedFromPool.end());
      }
    }
  }

  SetInDispatch(false);
  {
    //nglCriticalSectionGuard g(mCS);
    mDeletedFromPool.clear();
  }
  return 0;
}

#endif //NGL_EPOLL

bool nuiSocketPool::IsInDispatch() const
{
  //return ngl_atomic_read(mInDispatch) != 0;
  return true;
}

void nuiSocketPool::SetInDispatch(bool set)
{
  return;
/*
  if (set)
    ngl_atomic_compare_and_swap(mInDispatch, 0, 1);
  else
    ngl_atomic_compare_and_swap(mInDispatch, 1, 0);
    */
}


