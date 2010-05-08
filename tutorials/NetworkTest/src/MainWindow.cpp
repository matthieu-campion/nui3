/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"

#include "nuiNetworkHost.h"
#include "nuiSocket.h"
#include "nuiTCPClient.h"

// Network classes:

class nuiTCPServer : public nuiSocket
{
public:
  nuiTCPServer();
  ~nuiTCPServer();
  
  bool Bind(const nglString& rHost, int16 port);
  bool Bind(uint32 ipaddress, int16 port);
  bool Bind(const nuiNetworkHost& rHost);
  
  bool Listen(int backlog = 10);
  
  nuiTCPClient* Accept();
  
  bool Close();
};

class nuiUDPClient : public nuiSocket
{
public:
  nuiUDPClient();
  nuiUDPClient(const nglString& rHost, int16 port);
  nuiUDPClient(uint32 ipaddress, int16 port);
  nuiUDPClient(const nuiNetworkHost& rHost);

  ~nuiUDPClient();

  bool Send(const std::vector<uint8>& rData);
  
  bool SendTo(const nglString& rHost, int16 port, const std::vector<uint8>& rData);
  bool SendTo(uint32 ipaddress, int16 port, const std::vector<uint8>& rData);
  bool SendTo(const nuiNetworkHost& rHost, const std::vector<uint8>& rData);
  
};

class nuiUDPServer : public nuiSocket
{
public:
  nuiUDPServer();
  ~nuiUDPServer();
  
  bool Bind(const nglString& rHost, int16 port);
  bool Bind(uint32 ipaddress, int16 port);
  bool Bind(const nuiNetworkHost& rHost);
  
  bool ReceiveFrom(nglString& rHost, std::vector<uint8>& rData);
  
  bool Close();
};

class nuiTCPSocketPool
{
public:
  nuiTCPSocketPool();

  void AddServer(nuiTCPServer* pServer);
  void DelServer(nuiTCPServer* pServer);
  
  void WaitForEvent(double waittime = -1);
  
//  virtual bool OnConnectionRequest(nuiTCPServer* pServer, nuiTCPClient* pClient);
//  virtual bool OnDataAvailable(nuiTCPClient* pClient);
//  virtual bool OnDataAvailable(nuiUDPClient* pClient);
//  virtual bool OnDataNeeded(nuiTCPClient* pClient);
//  virtual bool OnError(nuiTCPServer* pClient);
//  virtual bool OnError(nuiTCPClient* pClient);
//  virtual bool OnError(nuiUDPServer* pClient);
//  virtual bool OnError(nuiUDPClient* pClient);
};

// MainWindow:

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif

  LoadCSS(_T("rsrc:/css/main.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiNetworkHost host(_T("www.google.com"), 80, nuiNetworkHost::eTCP);
  std::vector<nuiNetworkHost> hosts;
  host.Resolve(hosts, _T("http"));
  
  for (uint32 i = 0; i < hosts.size(); i++)
  {
    uint32 _ip = hosts[i].GetIP();
    uint8* ip = (uint8*)&_ip;
    nglString proto;
    switch (hosts[i].GetProtocol())
    {
      case nuiNetworkHost::eTCP:
        proto = "tcp";
        break;
      case nuiNetworkHost::eUDP:
        proto = "udp";
        break;
      default:
        proto = "any";
        break;
    }
    
    NGL_OUT(_T("%d: %ls (%d/%ls) [%d.%d.%d.%d]\n"), i, hosts[i].GetName().GetChars(), hosts[i].GetPort(), proto.GetChars(), ip[0], ip[1], ip[2], ip[3]);
  }
  
  nuiTCPClient client;
  
  if (client.Connect(hosts[0]))
  {
    const char* msg = "GET /\n\n";
    client.Send((uint8*)msg, strlen(msg));
    
    std::vector<uint8> buffer;
    client.Receive(buffer);
    
    nglString s((const char*)&buffer[0], buffer.size());
    
    NGL_OUT(_T("Result:\n%ls\n\n"), s.GetChars());
  }
}



bool MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
  
  return true; // means the event is caught and not broadcasted
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;

  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }

  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;

  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }

  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}
