/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiNetworkHost
{
public:
  enum Protocol
  {
    eAny,
    eTCP,
    eUDP
  };
  
  nuiNetworkHost(int port = 0, Protocol protocol = eAny); ///< Init the object to local host with the given port
  nuiNetworkHost(const nglString& rHost, int port = 0, Protocol = eAny);
  nuiNetworkHost(uint32 IPAddress, int port = 0, Protocol = eAny);
  nuiNetworkHost(const nuiNetworkHost& rHost);

  ~nuiNetworkHost();
  
  uint32 GetIP() const;
  const nglString& GetName() const;
  int GetPort() const;
  Protocol GetProtocol() const;

  bool Resolve(std::vector<nuiNetworkHost>& rHosts, const nglString& rService = nglString::Null);
  nuiNetworkHost Resolve(const nglString& rService = nglString::Null);

protected:
  bool mIPSet : 1;
  bool mNameSet : 1;
  bool mResolved : 1;
  int mPort;
  uint32 mIP;
  nglString mName;
  Protocol mProtocol;
  
};
