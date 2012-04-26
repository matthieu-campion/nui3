/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTTPServer.h"
#include "nuiNetworkHost.h"


//class nuiURLHandler
nuiURLHandler::nuiURLHandler(const nglString& rRegExp, const HandlerDelegate& rDelegate)
: mRegExp(rRegExp), mDelegate(rDelegate)
{
}

nuiURLHandler::~nuiURLHandler()
{
}

const nglString& nuiURLHandler::GetRegExp() const
{
  return mRegExp.GetExpression();
}

nuiHTTPHandler* nuiURLHandler::Handle(const nglString& rURL)
{
  if (mRegExp.Match(rURL))
  {
    std::vector<nglString> args;
    for (uint32 i = 0; i < mRegExp.SubStrings(); i++)
      args.push_back(rURL.Extract(mRegExp.SubStart(i), mRegExp.SubLength(i)));
    return mDelegate(rURL, args);
  }
}
  

//class nuiURLDispatcher
nuiURLDispatcher::nuiURLDispatcher()
{
}

nuiURLDispatcher::~nuiURLDispatcher()
{
  for (int i = 0; i < mpHandlers.size(); i++)
    delete mpHandlers[i];
}

void nuiURLDispatcher::AddHandler(nuiURLHandler* pHandler)
{
  mpHandlers.push_back(pHandler);
}

void nuiURLDispatcher::AddHandler(const nglString& rRegExp, const nuiURLHandler::HandlerDelegate& rDelegate)
{
  mpHandlers.push_back(new nuiURLHandler(rRegExp, rDelegate));
}

nuiHTTPHandler* nuiURLDispatcher::Dispatch(const nglString& rURL)
{
  for (int i = 0; i < mpHandlers.size(); i++)
  {
     nuiHTTPHandler* pHandler = mpHandlers[i]->Handle(rURL);
    if (pHandler)
      return pHandler;
  }

  return NULL;
}


//class nuiHTTPHandler
nuiHTTPHandler::nuiHTTPHandler(nuiTCPClient* pClient)
: mpClient(pClient)
{
}

nuiHTTPHandler::~nuiHTTPHandler()
{
  delete mpClient;
}

void nuiHTTPHandler::Parse()
{
  std::vector<uint8> data;
  nglChar cur = 0;
  State state = Request;
  data.resize(1024);
  while (mpClient->Receive(data))
  {
    size_t index = 0;
    while (index < data.size())
    {
      cur = data[index];
      if (state == Body)
      {
        std::vector<uint8> d(data.begin() + index, data.end());
        //NGL_OUT("...Body data... (%d)\n", d.size());
        OnBodyData(d);
        index = data.size();
      }
      else
      {
        index++;

        if (cur == 10)
        {
          // skip...
        }
        else if (cur == 13)
        {
          // found a line:
          switch (state)
          {
            case Request:
            {
              mCurrentLine.Trim();
              int pos = mCurrentLine.Find(' ');
              if (pos < 0)
              {
                // Error!
                return;
              }

              mMethod = mCurrentLine.GetLeft(pos);
              //NGL_OUT("Method: %s\n", mMethod.GetChars());
              if (!OnMethod(mMethod))
                return;

              while (mCurrentLine[pos] == ' ')
                pos++;
              int pos2 = pos;
              while (mCurrentLine[pos2] != ' ')
                pos2++;
              mURL = mCurrentLine.Extract(pos, pos2 - pos);
              //NGL_OUT("URL: %s\n", mURL.GetChars());
              if (!OnURL(mURL))
                return;

              pos = pos2;
              while (mCurrentLine[pos] == ' ')
                pos++;
              pos2 = pos;
              while (mCurrentLine[pos2] != '/')
                pos2++;

              mProtocol = mCurrentLine.Extract(pos, pos2 - pos);
              mVersion = mCurrentLine.Extract(pos2 + 1);
              mVersion.Trim();
              //NGL_OUT("Protocol: %s\n", mProtocol.GetChars());
              //NGL_OUT("Version: %s\n", mVersion.GetChars());
              if (!OnProtocol(mProtocol, mVersion))
                return;

              state = Header;

              mCurrentLine.Wipe();
            }
              break;

            case Header:
            {
              if (mCurrentLine.IsEmpty())
              {
                //NGL_OUT("Start body...\n");
                if (!OnBodyStart())
                  return;
                state = Body;
              }
              else
              {
                mCurrentLine.Trim();
                int pos = mCurrentLine.Find(':');
                if (pos < 0)
                {
                  // Error!
                  return;
                }

                nglString key = mCurrentLine.GetLeft(pos);
                nglString value = mCurrentLine.Extract(pos + 1);

                key.Trim();
                value.Trim();

                mHeaders[key] = value;

                //NGL_OUT("[%s]: '%s'\n", key.GetChars(), value.GetChars());

                if (!OnHeader(key, value))
                  return;

                state = Header;
                mCurrentLine.Wipe();
              }
            }
              break;
            default:
              NGL_ASSERT(0);
              break;
          }
        }
        else
        {
          mCurrentLine.Append(cur);
        }
      }

    }
  }
  //NGL_OUT("End body\n");
  OnBodyEnd();
}

bool nuiHTTPHandler::OnMethod(const nglString& rValue)
{
  return true;
}

bool nuiHTTPHandler::OnURL(const nglString& rValue)
{
  return true;
}

bool nuiHTTPHandler::OnProtocol(const nglString& rValue, const nglString rVersion)
{
  return true;
}

bool nuiHTTPHandler::OnHeader(const nglString& rKey, const nglString& rValue)
{
  return true;
}

bool nuiHTTPHandler::OnBodyStart()
{
  return true;
}

bool nuiHTTPHandler::OnBodyData(const std::vector<uint8>& rData)
{
  return true;
}

void nuiHTTPHandler::OnBodyEnd()
{
}

bool nuiHTTPHandler::ReplyLine(const nglString& rString)
{
  bool res = mpClient->Send(rString);
  res &= mpClient->Send("\r\n");
  return res;
}

bool nuiHTTPHandler::ReplyHeader(const nglString& rKey, const nglString& rValue)
{
  nglString str;
  str.Add(rKey).Add(": ").Add(rValue);
  return ReplyLine(str);
}

bool nuiHTTPHandler::ReplyError(int32 code, const nglString& rErrorStr)
{
  nglString errstr;
  errstr.Add("HTTP/1.1 ").Add(code).Add(" ").Add(rErrorStr);
  ReplyLine(errstr);
  ReplyHeader("Content-Type", "text/plain");
  ReplyHeader("Server", "Yastream 1.0.0");
  ReplyLine("");
  ReplyLine(rErrorStr);

  // Log the error:
  Log(code);
}

bool nuiHTTPHandler::Log(int32 code)
{
  nuiNetworkHost client(0, 0, nuiNetworkHost::eTCP);
  bool res = mpClient->GetDistantHost(client);
  if (!res)
    return false;

  uint32 ip = client.GetIP();
  uint8* pIp = (uint8*)&ip;
  nglString t = nglTime().GetLocalTimeStr("%a, %d %b %Y %H:%M:%S %z");
  NGL_OUT("%d.%d.%d.%d \"%s %s\" %d %s\n", pIp[0], pIp[1], pIp[2], pIp[3], mMethod.GetChars(), mURL.GetChars(), code, t.GetChars());
}


//class nuiHTTPServerThread : public nglThread
nuiHTTPServerThread::nuiHTTPServerThread(nuiHTTPHandler* pHandler, size_t StackSize)
: nglThread(nglThread::Normal, StackSize), mpHandler(pHandler)
{
  SetAutoDelete(true);
}

nuiHTTPServerThread::~nuiHTTPServerThread()
{
  delete mpHandler;
}

void nuiHTTPServerThread::OnStart()
{
  mpHandler->Parse();
}



//class nuiHTTPServer : public nuiTCPServer
nuiHTTPServer::nuiHTTPServer()
{
  mClientStackSize = 0;
  SetHandlerDelegate(nuiMakeDelegate(this, &nuiHTTPServer::DefaultHandler));
}

nuiHTTPServer::~nuiHTTPServer()
{
}

void nuiHTTPServer::AcceptConnections()
{
  nuiTCPClient* pClient = NULL;
  Listen();
  while ((pClient = Accept()))
  {
    OnNewClient(pClient);
    //Listen();
  }

}

void nuiHTTPServer::OnNewClient(nuiTCPClient* pClient)
{
  //NGL_OUT("Received new connection...\n");
  nuiHTTPServerThread* pThread = new nuiHTTPServerThread(mDelegate(pClient), mClientStackSize);
  pThread->Start();
}

void nuiHTTPServer::SetHandlerDelegate(const nuiFastDelegate1<nuiTCPClient*, nuiHTTPHandler*>& rDelegate)
{
  mDelegate = rDelegate;
}

nuiHTTPHandler* nuiHTTPServer::DefaultHandler(nuiTCPClient* pClient)
{
  return new nuiHTTPHandler(pClient);
}


void nuiHTTPServer::SetClientStackSize(size_t StackSize)
{
  mClientStackSize = StackSize;
}

size_t nuiHTTPServer::GetClientStackSize() const
{
  return mClientStackSize;
}

