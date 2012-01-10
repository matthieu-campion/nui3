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
  mConnected = false;
}

nuiTCPClient::nuiTCPClient(int sock)
: nuiSocket(sock)
{
  mConnected = true;
}

nuiTCPClient::~nuiTCPClient()
{
  Close();
}

bool nuiTCPClient::Connect(const nuiNetworkHost& rHost)
{
  if (IsConnected())
    return false;

  if (!Init(AF_INET, SOCK_STREAM, 0))
    return false;

  struct addrinfo* addr = nuiSocket::GetAddrInfo(rHost);
  int res = connect(mSocket, addr->ai_addr, addr->ai_addrlen);
  if (res)
    DumpError(errno);
  
  freeaddrinfo(addr);

  mConnected = res == 0;
  
  return mConnected;
}

bool nuiTCPClient::Connect(const nglString& rHost, int16 port)
{
  return Connect(nuiNetworkHost(rHost, port, nuiNetworkHost::eTCP));
}

bool nuiTCPClient::Connect(uint32 ipaddress, int16 port)
{
  return Connect(nuiNetworkHost(ipaddress, port, nuiNetworkHost::eTCP));
}


bool nuiTCPClient::Send(const std::vector<uint8>& rData)
{
  return Send(&rData[0], rData.size());
}

bool nuiTCPClient::Send(const uint8* pData, int len)
{
  if (!IsConnected())
    return false;
  
#ifdef WIN32
  send(mSocket, (const char*)pData, len, 0);
#else
  send(mSocket, pData, len, 0);
#endif
  return false;
}


bool nuiTCPClient::ReceiveAvailable(std::vector<uint8>& rData)
{
  if (!IsConnected())
    return false;

  int PendingBytes = 0;
#ifdef WIN32
  int result = WSAIoctl(mSocket, FIONREAD, NULL, 0, &PendingBytes, sizeof(PendingBytes), NULL, NULL, NULL);
#else
  int result = ioctl(mSocket, FIONREAD, &PendingBytes);
#endif

  if (!PendingBytes || result != 0)
    return false;
  
  rData.resize(PendingBytes);
#ifdef WIN32
  int res = recv(mSocket, (char*)&rData[0], rData.size(), MSG_WAITALL);
#else
  int res = recv(mSocket, &rData[0], rData.size(), MSG_WAITALL);
#endif

  if (res < 0)
  {
    mConnected = false;
    rData.clear();
    return false;
  }
  else if (res < 0)
  {
    // Error
    rData.clear();
    return false;
  }

  rData.resize(res);

  return res != 0;
}

bool nuiTCPClient::Receive(std::vector<uint8>& rData)
{
  if (!IsConnected())
    return false;
  
#ifdef WIN32
  int res = recv(mSocket, (char*)&rData[0], rData.size(), MSG_WAITALL);
#else
  int res = recv(mSocket, &rData[0], rData.size(), MSG_WAITALL);
#endif

  if (res == 0)
  {
    mConnected = false;
  }
  else if (res < 0)
  {
    // Error
    return false;
  }

  rData.resize(res);
  
  return res != 0;
}


bool nuiTCPClient::Close()
{
  if (!IsConnected())
    return false;

  
#ifdef WIN32
  //DisconnectEx(mSocket, NULL, 0, 0);
  closesocket(mSocket);
#else
  close(mSocket);
#endif
  
  mSocket = -1;
  return true;
}

bool nuiTCPClient::IsConnected() const
{
  return mConnected;
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
  return IsConnected();
}


