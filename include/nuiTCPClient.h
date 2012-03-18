/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSocket.h"

class nuiTCPClient : public nuiSocket
{
public:
  nuiTCPClient();
  ~nuiTCPClient();
  
  bool Connect(const nuiNetworkHost& rHost);
  bool Connect(const nglString& rHost, int16 port);
  bool Connect(uint32 ipaddress, int16 port);
  
  int Send(const std::vector<uint8>& rData);
  int Send(const uint8* pData, int len);
  int Send(const nglString& rString);
  int ReceiveAvailable(std::vector<uint8>& rData); ///< Fill the rData vector with the data that is already available to read on the socket.
  int Receive(std::vector<uint8>& rData); ///< Fill the buffer up to its current size.
  int Receive(uint8* pData, int32 len); ///< Fill the buffer up to its current size.
  
  bool IsConnected() const;
  int32 GetAvailable() const;
  bool CanWrite() const;
  
  bool Close();
  
protected:
  friend class nuiTCPServer;
  nuiTCPClient(int sock);
  bool mConnected;
};

class nuiPipe
{
public:
  nuiPipe();
  virtual ~nuiPipe();
  
  size_t Write(const uint8* pBuffer, size_t size);
  size_t Write(const nglString& rString);
  size_t Read(uint8* pBuffer, size_t size);
  size_t GetSize() const;
  const uint8* GetBuffer() const;
  void Eat(size_t size);
  void Clear();
  
protected:
  std::vector<uint8> mBuffer;
};



class nuiBufferedTCPClient : public nuiTCPClient
{
public:
  nuiBufferedTCPClient();
  virtual ~nuiBufferedTCPClient();

  // This is used by the client:
  size_t BufferedWrite(const uint8* pBuffer, size_t size);
  size_t BufferedWrite(const nglString& rString);
  size_t BufferedRead(uint8* pBuffer, size_t size);
  
  // This is only used by stream handlers
  size_t WriteToInputBuffer(const uint8* pBuffer, size_t size);
  size_t ReadFromOutputBuffer(uint8* pBuffer, size_t size);
  
  void OnCanRead();
  void OnCanWrite();
  void OnReadClosed();
  void OnWriteClosed();
  
private:
  nuiPipe mIn;
  nuiPipe mOut;
};

