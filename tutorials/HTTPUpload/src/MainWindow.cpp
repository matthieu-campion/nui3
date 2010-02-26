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
#include "nuiHTTP.h"
#include "nglOMemory.h"


/*
 * MainWindow
 */

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
  //////////////////////////////////////
  nglString varname(_T("MyParam"));
  nglString value(_T("MyValue"));
  
  nglString fileref(_T("MyFile"));
  nglString filename(_T("prout.txt"));
  //  nglString filename(_T(""));

  nglString mimetype("plain/text");
  const uint8* data = (const uint8*)"YATTA!";
  uint32 datalen = strlen((const char*)data);
  //////////////////////////////////////
  
  nuiHTTPRequest request(_T("http://testupload:8888/"), _T("POST"));
  nglString boundary;
  boundary.Add(_T("NuiBoundary"));
  boundary.Add((uint32)nglTime(), 16);
  nglOMemory mem;
  mem.SetTextFormat(eTextDOS);
  mem.SetTextEncoding(eUTF8);
  nglString str;
  
  nglString start;
  start.CFormat(_T("--%ls"), boundary.GetChars());
  
  nglString end;
  end.Add(_T("\n"));
  end.Add(start);
  end.Add(_T("--\n"));
  start.Add(_T("\n"));


  //str.CFormat(_T("Content-type: multipart/form-data, boundary=%ls\n\n"), boundary.GetChars());
  str.CFormat(_T("multipart/form-data; boundary=%ls"), boundary.GetChars());
  request.AddHeader(_T("Content-Type"), str);
  //mem.WriteText(str);

  
  
  mem.WriteText(start);

  str.CFormat(_T("Content-Disposition: form-data; name=\"%ls\"\n\n"), varname.GetChars());
  mem.WriteText(str);
  mem.WriteText(value);
  mem.WriteText(_T("\n"));

  mem.WriteText(start);
  str.CFormat(_T("Content-Disposition: form-data; name=\"%ls\"; filename=\"%ls\"\n"), fileref.GetChars(), filename.GetChars());
  mem.WriteText(str);
  
  str.CFormat(_T("Content-Type: %ls\n\n"), mimetype.GetChars());
  mem.WriteText(str);
  mem.WriteUInt8(data, datalen);
  mem.WriteText(end);
  
  request.SetBody(mem.GetBufferData(), mem.GetSize());
  
  mem.WriteUInt8((const uint8*)"\0", 1); // Add final 0 for printf
  printf("Mime encoded:\n%s\n", mem.GetBufferData());

  nuiHTTPResponse* pRes = request.SendRequest();
  printf("Result:\n%ls\n", pRes->GetBodyStr().GetChars());
  
  delete pRes;
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
