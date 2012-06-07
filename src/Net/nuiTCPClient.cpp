/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiTCPClient.h"
#include "nuiNetworkHost.h"

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#undef GetAddrInfo
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif


nuiTCPClient::nuiTCPClient()
{
  mReadConnected = false;
  mWriteConnected = false;
}

nuiTCPClient::nuiTCPClient(int sock)
: nuiSocket(sock)
{
  mReadConnected = true;
  mWriteConnected = true;
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
    DumpError(errno);

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

  if (res < 0)
  {
    if (errno == EWOULDBLOCK && mNonBlocking)
      return res;

    DumpError(errno);
    mWriteConnected = false;
  }

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

  //#FIXME testing twice for the same condition... which one is right???
  if (res < 0)
  {
    if (errno == EWOULDBLOCK && mNonBlocking)
      return res;

    mReadConnected = false;
    rData.clear();
    return 0;
  }
  else if (res < 0)
  {
    // Error
    rData.clear();
    return 0;
  }

  rData.resize(res);

  return res != 0;
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


  if (res == 0)
  {
    mReadConnected = false;
  }
  else if (res < 0)
  {
    if (errno == EWOULDBLOCK && mNonBlocking)
      return res;
    // Error
    return res;
  }

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


bool nuiTCPClient::Close()
{
#ifdef WIN32
  //DisconnectEx(mSocket, NULL, 0, 0);
  closesocket(mSocket);
#else
  close(mSocket);
#endif

  mSocket = -1;
  mWriteConnected = false;
  mReadConnected = false;
  return true;
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
  bool retval = false;
  int bytestoread = 0;
  timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  fd_set myfd;

  FD_ZERO(&myfd);
  FD_SET(mSocket, &myfd);
  int sio = select(FD_SETSIZE, &myfd, (fd_set *)0, (fd_set *)0, &timeout);
  //have to do select first for some reason
  int dio = ioctl(mSocket, FIONREAD, &bytestoread);//should do error checking on return value of this
  retval = ((bytestoread == 0) && (sio == 1));

  return retval;
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
size_t nuiTCPClient::BufferedSend(const uint8* pBuffer, size_t size)
{
  mOut.Write(pBuffer, size);
  SendWriteBuffer();
  return size;
}

size_t nuiTCPClient::BufferedSend(const nglString& rString)
{
  return BufferedSend((uint8*)rString.GetChars(), rString.GetLength());
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
}

void nuiTCPClient::SendWriteBuffer()
{
  size_t s = mOut.GetSize();

  if (!s)
    return;

  const uint8* pBuffer = mOut.GetBuffer();

  int done = Send(pBuffer, s);
  if (done >= 0)
    mOut.Eat(done);
  //NGL_OUT("%p %d eat %d\n", this, GetSocket(), done);
}

void nuiTCPClient::OnReadClosed()
{
  printf("%d read closed\n", GetSocket());
  nuiSocket::OnReadClosed();
  mReadConnected = false;

}

void nuiTCPClient::OnWriteClosed()
{
  NGL_OUT("%d write closed\n", GetSocket());
  nuiSocket::OnWriteClosed();
  mWriteConnected = false;
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
  return mBuffer.size();
}

const uint8* nuiPipe::GetBuffer() const
{
  return &mBuffer[0];
}

void nuiPipe::Eat(size_t size)
{
  size_t p = mBuffer.size();
  size_t todo = MIN(size, p);
  size_t remain = p - todo;

  memmove(&mBuffer[0], &mBuffer[todo], remain);
  mBuffer.resize(remain);
}


void nuiPipe::Clear()
{
  mBuffer.clear();
}



