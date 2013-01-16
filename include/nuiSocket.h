/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

#if (defined __APPLE__) || (defined _DARWIN_) || (defined _FREEBSD_)
#define NGL_KQUEUE
#elif (defined _LINUX_) || (defined _ANDROID_)
#define NGL_EPOLL
#endif

class nuiNetworkHost;
class nuiSocketPool;
class nuiSocket;

#define __FUNC__ "%s:%d",__FILE__,__LINE__

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

  static void DumpError(const nuiSocket* pSocket, int err);
  static void DumpError(const nuiSocket* pSocket, int err, const char* msg, ...);

  const nglString& GetName() const;
  void SetName(const nglString& rName);
  virtual nglString GetDesc() const;

  // Socket introspection:
  static int64 GetSocketCount();
  static void VisitSockets(const nuiFastDelegate1<nuiSocket*>& rDelegate);

  static void GetStatusReport(nglString& rResult);

  bool IsIdle();
  void SetMaxIdleTime(int32 set);
  int32 GetMaxIdleTime() const;
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

  nglString mName;

  static void AddSocket(nuiSocket* pSocket);
  static void DelSocket(nuiSocket* pSocket);
  static int64 gmSocketCount;
  static nglCriticalSection gmCS;
  static std::set<nuiSocket*> gmAllSockets;

  int32 mMaxIdleTime;
  nglTime mLastOperationTime;
  void UpdateIdle();
};


class nuiSocketPool
{
public:
  nuiSocketPool();
  virtual ~nuiSocketPool();

  enum TriggerMode
  {
    eContinuous,
    eStateChange
  };

  void Add(nuiSocket* pSocket, TriggerMode Mode);
  void Del(nuiSocket* pSocket);

  int DispatchEvents(int timeout_millisec);
private:
  nglCriticalSection mCS;
  std::set<nuiSocket*> mDeletedFromPool;
  std::set<nuiSocket*> mSockets;

  int mNbSockets;

  void HandleIdleSockets();

#ifdef NGL_KQUEUE
  // Kernel queue implementation (FreeBSD, Darwin...)
  std::vector<struct kevent> mEvents;
  int mQueue;
#endif

#ifdef NGL_EPOLL
  //std::vector<struct epoll_event> mEvents;
  int mEPoll;
#endif
  
};

