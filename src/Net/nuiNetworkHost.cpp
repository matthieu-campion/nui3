/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#ifdef WIN32
#include <Ws2tcpip.h>
#undef GetAddrInfo
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif


//class nuiNetworkHost
nuiNetworkHost::nuiNetworkHost(int port, Protocol protocol)
: mIPSet(false),
  mNameSet(false),
  mResolved(false),
  mPort(port),
  mIP(0),
  mProtocol(protocol)
{
}

nuiNetworkHost::nuiNetworkHost(const nglString& rHost, int port, Protocol protocol)
: mIPSet(false),
  mNameSet(true),
  mResolved(false),
  mPort(port),
  mIP(0),
  mName(rHost),
  mProtocol(protocol)
{
}

nuiNetworkHost::nuiNetworkHost(uint32 IPAddress, int port, Protocol protocol)
: mIPSet(true),
  mNameSet(false),
  mPort(port),
  mIP(htonl(IPAddress)),
  mProtocol(protocol)
{
}

nuiNetworkHost::nuiNetworkHost(const nuiNetworkHost& rHost)
: mIPSet(rHost.mIPSet),
  mNameSet(rHost.mNameSet),
  mResolved(false),
  mPort(rHost.mPort),
  mIP(rHost.mIP),
  mName(rHost.mName),
  mProtocol(rHost.mProtocol)
{
}

nuiNetworkHost::~nuiNetworkHost()
{
}

uint32 nuiNetworkHost::GetIP() const
{
  return mIP;
}

const nglString& nuiNetworkHost::GetName() const
{
  return mName;
}

int nuiNetworkHost::GetPort() const
{
  return mPort;
}

nuiNetworkHost::Protocol nuiNetworkHost::GetProtocol() const
{
  return mProtocol;
}

bool nuiNetworkHost::Resolve(std::vector<nuiNetworkHost>& rHosts, const nglString& rService)
{
  struct addrinfo* infos = GetAddrInfo(rService);

  while (infos)
  {
    sockaddr_in* addr = (struct sockaddr_in*)infos->ai_addr;
    Protocol proto = eAny;
    if (infos->ai_socktype == SOCK_STREAM)
      proto = eTCP;
    else if (infos->ai_socktype == SOCK_DGRAM)
      proto = eUDP;

    nuiNetworkHost h(nglString(infos->ai_canonname), ntohs(addr->sin_port), proto);
    h.mIP = addr->sin_addr.s_addr;
    rHosts.push_back(h);

    infos = infos->ai_next;
  }

  freeaddrinfo(infos);

  return true;
}

nuiNetworkHost nuiNetworkHost::Resolve(const nglString& rService)
{
  std::vector<nuiNetworkHost> hosts;
  if (!Resolve(hosts, rService) || hosts.empty())
    return nuiNetworkHost(0, eAny);
  return hosts[0];
}


struct addrinfo* nuiNetworkHost::GetAddrInfo(const nglString& rService) const
{
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_CANONNAME;     /* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
  hints.ai_family = PF_INET;    /* PF_xxx */
  hints.ai_socktype = 0;  /* SOCK_xxx */
  hints.ai_protocol = 0;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */

  if (mProtocol == eTCP)
  {
    hints.ai_socktype = SOCK_STREAM;  /* SOCK_xxx */
    hints.ai_protocol = IPPROTO_TCP;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
  }
  else if (mProtocol == eUDP)
  {
    hints.ai_socktype = SOCK_DGRAM;  /* SOCK_xxx */
    hints.ai_protocol = IPPROTO_UDP;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
  }

  struct addrinfo* infos = NULL;

  nglString h;
  if (mNameSet)
    h = mName;
  else if (mIPSet)
  {
    uint8* ip = (uint8*)&mIP;
    h.CFormat(_T("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
  }

  char* hh = NULL;
  char* ss = NULL;
  if (!h.IsEmpty())
    hh = h.Export();
  if (!rService.IsEmpty())
    ss = rService.Export();
  else
  {
    nglString s;
    s.Add(mPort);
    ss = s.Export();
  }

  int res = getaddrinfo(hh, ss, &hints, &infos);

  free(hh);
  free(ss);


  if (res != 0)
  {
    nglString err;
    switch (res)
    {
      case EAI_AGAIN:
        err = "temporary failure in name resolution EAI_BADFLAGS invalid value for ai_flags EAI_BADHINTS invalid value for hints";
        break;
      case EAI_FAIL:
        err = "non-recoverable failure in name resolution";
        break;
      case EAI_FAMILY:
        err = "ai_family not supported";
        break;
      case EAI_MEMORY:
        err = "memory allocation failure";
        break;
      case EAI_NONAME:
        err = "hostname or servname not provided, or not known EAI_OVERFLOW argument buffer overflow EAI_PROTOCOL resolved protocol is unknown";
        break;
      case EAI_SERVICE:
        err = "servname not supported for ai_socktype EAI_SOCKTYPE ai_socktype not supported";
        break;
#ifndef WIN32
      case EAI_SYSTEM:
        err = "system error returned in errno";
        break;
#endif
    }

    NGL_LOG(_T("network"), 0, _T("nuiNetworkHost::Resolve error: %s\n"), err.GetChars());
  }


  return infos;
}

