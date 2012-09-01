/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

#if (defined __APPLE__) || (defined _DARWIN_) || (defined _FREEBSD_)
#include <sys/event.h>
#define NGL_KQUEUE
#elif (defined _LINUX_) || (defined _ANDROID_)
#include <sys/epoll.h>
#define NGL_EPOLL
#endif

class nuiNetworkHost;
class nuiSocketPool;

class nuiSocket
{
public:
  typedef nuiFastDelegate1<nuiSocket&> EventDelegate;

#ifdef WIN32
  typedef SOCKET SocketType;
#else
  typedef int SocketType;
#endif

  virtual ~nuiSocket();

  SocketType GetSocket() const;

  bool GetLocalHost(nuiNetworkHost& rHost) const;
  bool GetDistantHost(nuiNetworkHost& rHost) const;

  bool IsValid() const;

  void SetNonBlocking(bool set);
  bool IsNonBlocking() const;

  void SetCanReadDelegate(const EventDelegate& rDelegate);
  void SetCanWriteDelegate(const EventDelegate& rDelegate);
  void SetReadClosedDelegate(const EventDelegate& rDelegate);
  void SetWriteClosedDelegate(const EventDelegate& rDelegate);

  virtual void OnCanRead();
  virtual void OnCanWrite();
  virtual void OnReadClosed();
  virtual void OnWriteClosed();

  virtual void Close();

  static void DumpError(int err, const char* msg = NULL);
protected:
  friend class nuiSocketPool;
  nuiSocket(SocketType Socket = -1);
  bool Init(int domain, int type, int protocol);
  void SetPool(nuiSocketPool* pPool);

  struct addrinfo* GetAddrInfo(const nuiNetworkHost& rHost) const;

  SocketType mSocket;
  EventDelegate mReadDelegate;
  EventDelegate mWriteDelegate;
  EventDelegate mReadCloseDelegate;
  EventDelegate mWriteCloseDelegate;
  bool mNonBlocking;
  nuiSocketPool* mpPool;
};

class nuiSocketPool
{
public:
  enum TriggerMode
  {
    eContinuous,
    eStateChange
  };

  nuiSocketPool();
  virtual ~nuiSocketPool();

  void Add(nuiSocket* pSocket, TriggerMode Mode);
  void Del(nuiSocket* pSocket);

  int DispatchEvents(int timeout_millisec);
private:
  int mNbSockets;
#ifdef NGL_KQUEUE
  // Kernel queue implementation (FreeBSD, Darwin...)
  std::vector<struct kevent> mEvents;
  int mQueue;
#endif

#ifdef NGL_EPOLL
  int mEPoll;
  std::vector<struct epoll_event> mEvents;
#endif

  std::set<nuiSocket*> mDeletedFromPool;
  nglAtomic mInDispatch;

  bool IsInDispatch() const;
  void SetInDispatch(bool set);
};
