/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#undef GetAddrInfo
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#endif


nuiTCPClient::nuiTCPClient()
{
  mReadConnected = false;
  mWriteConnected = false;
  mAutoDelete = false;
  mpAutoPool = NULL;
  mReceived = 0;
  mSent = 0;
}

nuiTCPClient::nuiTCPClient(int sock)
: nuiSocket(sock)
{
  mReadConnected = true;
  mWriteConnected = true;
  mAutoDelete = false;
  mpAutoPool = NULL;
  mReceived = 0;
  mSent = 0;
}

nuiTCPClient::~nuiTCPClient()
{
  Close();
}

bool nuiTCPClient::Connect(const nuiNetworkHost& rHost)
{
  if (!Init(AF_INET, SOCK_STREAM, 0))
    return false;

  struct addrinfo* addr = nuiSocket::GetAddrInfo(rHost);
  int res = connect(mSocket, addr->ai_addr, addr->ai_addrlen);
  if (res)
    DumpError(this, res, __FUNC__);

  UpdateIdle();

  freeaddrinfo(addr);

  mReadConnected = mWriteConnected = res == 0;

  return mReadConnected;
}

bool nuiTCPClient::Connect(const nglString& rHost, int16 port)
{
  return Connect(nuiNetworkHost(rHost, port, nuiNetworkHost::eTCP));
}

bool nuiTCPClient::Connect(uint32 ipaddress, int16 port)
{
  return Connect(nuiNetworkHost(ipaddress, port, nuiNetworkHost::eTCP));
}

void nuiTCPClient::SetNoDelay(bool set)
{
  int flag = set ? 1 : 0;
  setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
}

int nuiTCPClient::Send(const nglString& rString)
{
  return Send((uint8*)rString.GetChars(), rString.GetLength());
}


int nuiTCPClient::Send(const std::vector<uint8>& rData)
{
  return Send(&rData[0], rData.size());
}

int nuiTCPClient::Send(const uint8* pData, int len)
{
#ifdef WIN32
  int res = send(mSocket, (const char*)pData, len, 0);
#else
  int res = send(mSocket, pData, len, 0);
#endif

  UpdateIdle();

  if (res < 0)
  {
    if (errno == EWOULDBLOCK && mNonBlocking)
      return res;

    DumpError(this, res, __FUNC__);
    mWriteConnected = false;
  }
  else
    mSent += res;
  return res;
}


int nuiTCPClient::ReceiveAvailable(std::vector<uint8>& rData)
{
  int PendingBytes = 0;
#ifdef WIN32
  int result = WSAIoctl(mSocket, FIONREAD, NULL, 0, &PendingBytes, sizeof(PendingBytes), NULL, NULL, NULL);
#else
  int result = ioctl(mSocket, FIONREAD, &PendingBytes);
#endif

  if (!PendingBytes || result != 0)
    return 0;

  rData.resize(PendingBytes);
#ifdef WIN32
  int res = recv(mSocket, (char*)&rData[0], rData.size(), MSG_WAITALL);
#else
  int res = recv(mSocket, &rData[0], rData.size(), MSG_WAITALL);
#endif

  mReadConnected = res != 0;
  UpdateIdle();

  if (res < 0)
  {
    if (errno == EWOULDBLOCK && mNonBlocking)
      return res;

    rData.clear();
    return res;
  }

  rData.resize(res);

  mReceived += res;
  return res;
}

int nuiTCPClient::Receive(uint8* pData, int32 len)
{
#ifdef WIN32
  int res = recv(mSocket, (char*)pData, len, MSG_WAITALL);
#else
  //int res = recv(mSocket, (char*)pData, len, MSG_WAITALL);
  int res = read(mSocket, pData, len);
  //printf("%p read returned %d\n", this, res);
#endif

  UpdateIdle();

  mReadConnected = res != 0;
  if (res > 0)
    mReceived += res;

  return res;
}

int nuiTCPClient::Receive(std::vector<uint8>& rData)
{
  int res = Receive(&rData[0], rData.size());

  if (res < 0)
  {
    // Error
    rData.clear();
    return res;
  }

  rData.resize(res);
  return res;
}


void nuiTCPClient::Close()
{
  nuiSocket::Close();
  mWriteConnected = false;
  mReadConnected = false;
}

bool nuiTCPClient::IsConnected() const
{
  return IsReadConnected() || IsWriteConnected();
}

bool nuiTCPClient::IsWriteConnected() const
{
  return mWriteConnected;
}

bool nuiTCPClient::IsReadConnected() const
{
  if (mReadConnected)
  {
//   bool retval = false;
//   int bytestoread = 0;
//   timeval timeout;
//   timeout.tv_sec = 0;
//   timeout.tv_usec = 0;
//   fd_set myfd;
//
//   FD_ZERO(&myfd);
//   FD_SET(mSocket, &myfd);
//   int sio = select(FD_SETSIZE, &myfd, (fd_set *)0, (fd_set *)0, &timeout);
//   //have to do select first for some reason
//   int dio = ioctl(mSocket, FIONREAD, &bytestoread);//should do error checking on return value of this
//   retval = ((bytestoread == 0) && (sio == 1));
//   
//   return retval;

    return true;
  }

  return false;
}

int32 nuiTCPClient::GetAvailable() const
{
  int PendingBytes = 0;
#ifdef WIN32
  int result = WSAIoctl(mSocket, FIONREAD, NULL, 0, &PendingBytes, sizeof(PendingBytes), NULL, NULL, NULL);
#else
  int result = ioctl(mSocket, FIONREAD, &PendingBytes);
#endif

  if (result != 0)
    PendingBytes = 0;

  return PendingBytes;
}

bool nuiTCPClient::CanWrite() const
{
  return IsWriteConnected();
}


// This is used by the client:
size_t nuiTCPClient::BufferedSend(const uint8* pBuffer, size_t size, bool BufferOnly)
{
  mOut.Write(pBuffer, size);
  if (!BufferOnly)
    SendWriteBuffer();
  return size;
}

size_t nuiTCPClient::BufferedSend(const nglString& rString, bool BufferOnly)
{
  return BufferedSend((uint8*)rString.GetChars(), rString.GetLength(), BufferOnly);
}

size_t nuiTCPClient::BufferedReceive(uint8* pBuffer, size_t size)
{
  return mIn.Read(pBuffer, size);
}

// This is only used by stream handlers
size_t nuiTCPClient::WriteToInputBuffer(const uint8* pBuffer, size_t size)
{
  return mIn.Write(pBuffer, size);
}

size_t nuiTCPClient::ReadFromOutputBuffer(uint8* pBuffer, size_t size)
{
  return mOut.Read(pBuffer, size);
}

void nuiTCPClient::SetAutoDelete(bool set)
{
  mAutoDelete = set;
}

void nuiTCPClient::SetAutoPool(nuiSocketPool* pPool)
{
  if (mpAutoPool != pPool)
    if (mpAutoPool)
      mpAutoPool->Del(this);

  if (mpPool != pPool && mpPool)
    mpPool->Del(this);

  mpAutoPool = pPool;
  if (mpAutoPool)
    mpAutoPool->Add(this, nuiSocketPool::eStateChange);
}


void nuiTCPClient::OnCanRead()
{
  if (mReadDelegate)
    mReadDelegate(*this);

  std::vector<uint8> Data;
  ReceiveAvailable(Data);

  WriteToInputBuffer(&Data[0], Data.size());
  //printf("%d write %d\n", GetSocket(), Data.size());
}

void nuiTCPClient::OnCanWrite()
{
  if (mWriteDelegate)
    mWriteDelegate(*this);

  SendWriteBuffer();
  if (mAutoDelete && mOut.GetSize() == 0)
    delete this;
}

void nuiTCPClient::SendWriteBuffer()
{
  const uint8* pBuffer = mOut.LockBuffer();
  size_t s = mOut.GetSize();

  if (!s)
  {
    mOut.UnlockBuffer();
    return;
  }

  int done = Send(pBuffer, s);
  if (done >= 0)
    mOut.Eat(done);

  //NGL_LOG("socket", NGL_LOG_INFO, "sent %d of %d bytes", done, s);

  mOut.UnlockBuffer();

  if (mpAutoPool)
  {
    if (mOut.GetSize() > 0)
    {
      if (!mpPool)
        mpAutoPool->Add(this, nuiSocketPool::eStateChange);
    }
    else
    {
      if (mpPool)
        mpAutoPool->Del(this);
    }
  }

  //  NGL_LOG("socket", NGL_LOG_INFO, "%p %d eat %d\n", this, GetSocket(), done);
}

void nuiTCPClient::OnReadClosed()
{
  NGL_LOG("socket", NGL_LOG_INFO, "%d read closed\n", GetSocket());
  nuiSocket::OnReadClosed();
  mReadConnected = false;
}

void nuiTCPClient::OnWriteClosed()
{
  NGL_LOG("socket", NGL_LOG_INFO, "%d write closed\n", GetSocket());
  nuiSocket::OnWriteClosed();
  mWriteConnected = false;
}


nglString nuiTCPClient::GetDesc() const
{
  nuiNetworkHost source(0, 0, nuiNetworkHost::eTCP);
  nuiNetworkHost dest(0, 0, nuiNetworkHost::eTCP);
  GetLocalHost(source);
  GetDistantHost(dest);
  uint32 S = source.GetIP();
  uint32 D = dest.GetIP();
  uint8* s = (uint8*)&S;
  uint8* d = (uint8*)&D;

  nglString str;
  str.CFormat("%5d: %s - from %d.%d.%d.%d:%d --> %d.%d.%d.%d:%d | i:%d / o:%d / a:%d / r:%lld / s:%lld| pool: %p%s [ %s ]",
              GetSocket(),
              IsNonBlocking() ? "NoBlock" : "Block  ",
              s[0], s[1], s[2], s[3], ntohs(source.GetPort()),
              d[0], d[1], d[2], d[3], ntohs(dest.GetPort()),
              (int)mIn.GetSize(), (int)mOut.GetSize(), GetAvailable(), mReceived, mSent,
              mpPool, (mpAutoPool?" (auto)":""),
              mName.GetChars());
  return str;
}




//////////////////////////
//class nuiPipe
nuiPipe::nuiPipe()
{

}

nuiPipe::~nuiPipe()
{

}

size_t nuiPipe::Write(const uint8* pBuffer, size_t size)
{
  nglCriticalSectionGuard guard(mCS);
  size_t p = mBuffer.size();
  mBuffer.resize(size + p);
  memcpy(&mBuffer[p], pBuffer, size);

  return size;
}

size_t nuiPipe::Write(const nglString& rString)
{
  return Write((const uint8*)rString.GetChars(), rString.GetLength());
}

size_t nuiPipe::Read(uint8* pBuffer, size_t size)
{
  nglCriticalSectionGuard guard(mCS);
  size_t p = mBuffer.size();
  size_t todo = MIN(size, p);
  size_t remain = p - todo;

  memcpy(pBuffer, &mBuffer[0], todo);
  memmove(&mBuffer[0], &mBuffer[todo], remain);
  mBuffer.resize(remain);
  return todo;
}

size_t nuiPipe::GetSize() const
{
  nglCriticalSectionGuard guard(mCS);
  return mBuffer.size();
}

const uint8* nuiPipe::LockBuffer()
{
  mCS.Lock();
  return &mBuffer[0];
}

void nuiPipe::UnlockBuffer()
{
  mCS.Unlock();
}


void nuiPipe::Eat(size_t size)
{
  nglCriticalSectionGuard guard(mCS);
  size_t p = mBuffer.size();
  size_t todo = MIN(size, p);
  size_t remain = p - todo;

  memmove(&mBuffer[0], &mBuffer[todo], remain);
  mBuffer.resize(remain);
}


void nuiPipe::Clear()
{
  nglCriticalSectionGuard guard(mCS);
  mBuffer.clear();
}

