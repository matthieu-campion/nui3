/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include <stdio.h>

using namespace std;

//#define USE_STANDARD_WIN32_CONSOLE 1 ///< Define this macro if you want to have a standard Win32 console (0 not to have it).

#define CONSOLE_CLASS   _T("nglConsole")
#define MENU_QUITAPP_ID 42
#define BACKLOG_SIZE    (60*1024)


/*
 * Life cycle
 */

nglConsole::nglConsole(bool IsVisible)
{
  Setup();

  mOutputPosition = 0;
  mHistoryPosition = -1;

  mOutputWnd = 0;
  mInputWnd = 0;
  mpInputWndProc = 0;

  /* Console window setup
   */
  WNDCLASSEX wcls;
  nglChar* sName;
  int xpos,ypos,width,height;

  sName  = _T("NGL Console");
  xpos   = (int)CW_USEDEFAULT;
  ypos   = (int)CW_USEDEFAULT;
  width  = 640;
  height = 480;
  mInputHeight = 20; // arbitrary input window height

  wcls.cbSize        = sizeof(WNDCLASSEX);
  wcls.style         = 0;
  wcls.lpfnWndProc   = ConsoleWindowProc;
  wcls.cbClsExtra    = 0;
  wcls.cbWndExtra    = 0;
  wcls.hInstance     = App->GetHInstance();
  wcls.hIcon         = LoadIcon(App->GetHInstance(), _T("0"));              
  wcls.hCursor       = LoadCursor(NULL,IDC_ARROW);
  wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcls.lpszMenuName  = NULL;
  wcls.lpszClassName = CONSOLE_CLASS;
  wcls.hIconSm       = NULL;

  RegisterClassEx(&wcls);
  mHWnd = CreateWindow(
    CONSOLE_CLASS,
    sName,
    WS_OVERLAPPEDWINDOW,
    xpos, ypos, width, height,
    NULL,
    NULL,
    App->GetHInstance(),
    this);
//  mHWnd=CreateWindowEx(WS_EX_CONTROLPARENT,"nglConsole",sName,WS_OVERLAPPEDWINDOW,xpos,ypos,width,height,NULL,NULL,app->GetHInstance(),this);
  //NGL_OUT(_T("Create console window hwnd = 0x%x"), mHWnd);
  DWORD err = GetLastError();

  HMENU hmenu = GetSystemMenu(mHWnd,FALSE);
  if (hmenu)
  {
    AppendMenu(hmenu,MF_SEPARATOR,0,NULL);
    AppendMenu(hmenu,MF_STRING,MENU_QUITAPP_ID, _T("&Quit application  \tCtrl Break"));
  }

  UpdateWindow(mHWnd);
  ShowWindow(mHWnd, IsVisible ? SW_SHOW : SW_HIDE);

#ifdef USE_STANDARD_WIN32_CONSOLE
  mHThread = NULL;
  mThreadId = 0;
  mHCOut = NULL;
  mHCIn = NULL;
  mConsoleAllocated = false;

  if (AllocConsole())
  {
    mConsoleAllocated = true;
    mHCOut = GetStdHandle(STD_OUTPUT_HANDLE);
    mHCIn = GetStdHandle(STD_INPUT_HANDLE);
    if (mHCOut == INVALID_HANDLE_VALUE || mHCIn == INVALID_HANDLE_VALUE) 
      return;
  }
  else
    return;
  mHThread = CreateThread(NULL,0,ThreadProc,this,0,&mThreadId);
#endif // USE_STANDARD_WIN32_CONSOLE
}

nglConsole::~nglConsole()
{

  DestroyWindow(mHWnd);
  UnregisterClass(CONSOLE_CLASS, App->GetHInstance());

  ClearHistory();

#ifdef USE_STANDARD_WIN32_CONSOLE
  if(mHThread)
  {
    TerminateThread(mHThread,0);
    mHThread = NULL;
  }
  if (mConsoleAllocated)
  {
    if (mHCIn)
      CloseHandle(mHCIn);
    if (mHCOut) 
      CloseHandle(mHCOut);
    FreeConsole();
  }
#endif // USE_STANDARD_WIN32_CONSOLE
}


/*
 * Public interface
 */

void nglConsole::OnOutput(const nglString& rText)
{
  // 'char' mode : string buffer is considered to use the locale's encoding
  Append(rText.GetChars());

  // Write to the standard win32 console:
#ifdef USE_STANDARD_WIN32_CONSOLE
  unsigned long res = 0;
  WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),rText.GetChars(),rText.GetLength(),&res,NULL);
#endif

  // I don't care if this is slow, its really mandatory in win32.
  vector<nglString> vec;
  uint ret=rText.Tokenize(vec, _T('\n'));
  for (uint i=0; i<ret; i++)
  {
    OutputDebugString(vec[i].GetChars()); // fixme! to be removed ?
    OutputDebugString(_T("\n"));
  }
}

void nglConsole::Show(bool val)
{
/*
FIXME
  // to scroll to bottom when showing console
  SendMessage(mOutputWnd, EM_SETSEL , mBackLogSize, mBackLogSize);
  // to give focus to input command line
  SendMessage(mOutputWnd, EM_SCROLLCARET , 0,0);
*/

  ShowWindow(mHWnd, val ? SW_SHOWNORMAL : SW_HIDE);
}

bool nglConsole::OnClose()
{
  Show(false);

  return false;
}


/*
 * Private interface
 */

void nglConsole::Append(const nglChar* pText)
{
  // Count EOL ('\n') occcurrences
  int eol_cnt = 0;
  const nglChar* eol = pText;

  while ((eol = wcschr(eol, '\n')))
  {
    eol++;
    eol_cnt++;
  }

  /* So we need that much bytes to output :
   *   strlen(pText)
   *   + eol_cnt (\n's become \r\n)
   *   + ending zero
   */
  int out_bytes = wcslen(pText) + eol_cnt + 1;
  nglChar* buffer  = (nglChar*)alloca(out_bytes * sizeof(nglChar));

  const nglChar* src = pText;
  nglChar* dst = buffer;
  nglChar c;
  int pos = mOutputPosition;

  do
  {
    c = *src++;
    if (c == '\n')
    {
      *dst++ = '\r';
      mOutputPosition++;
    }
    *dst++ = c;
    mOutputPosition++;
  }
  while (c);
  mOutputPosition--; // Don't count the ending zero

/*
  // EM_GETTHUMB desesperately returns a steady zero (Win2K SP2)
  int x = (int)SendMessage(mOutputWnd, EM_GETTHUMB, 0, 0);
  sprintf(buffer, "thumb: %d\r\n", x);
  pos = -1;
*/
  if (mOutputWnd)
  {
    bool need_delete = false;
    if (mOutputPosition >= BACKLOG_SIZE - 1)
    {
      const nglChar* nothing = _T("");
      int to_delete = mOutputPosition - (BACKLOG_SIZE - 1) + 1;

      need_delete = true;
      PostMessage(mOutputWnd, WM_SETREDRAW, (WPARAM)FALSE, 0);
      PostMessage(mOutputWnd, EM_SETSEL,     (WPARAM)0,     (LPARAM)to_delete);
      PostMessage(mOutputWnd, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)nothing);
      mOutputPosition -= to_delete;
      pos -= to_delete;
    }
    PostMessage(mOutputWnd, EM_SETSEL,     (WPARAM)pos,    (LPARAM)pos);
    PostMessage(mOutputWnd, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)buffer);
    if (need_delete)
    {
      PostMessage(mOutputWnd, WM_SETREDRAW,  (WPARAM)TRUE, 0);
      PostMessage(mOutputWnd, EM_SCROLLCARET, 0, 0);
    }
  }
}

void nglConsole::Clear()
{
  const static nglChar* nothing = _T("");

  mOutputPosition = 1;
  if (mOutputWnd  )
    PostMessage(mOutputWnd, WM_SETTEXT, 0, (long)nothing);
}


/*
 * Window proc
 */

LRESULT CALLBACK ConsoleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  nglConsole*win = NULL;
  if (uMsg == WM_CREATE)
  {
    CREATESTRUCT *cs = (CREATESTRUCT*) lParam;
    win = (nglConsole*)cs->lpCreateParams;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (DWORD_PTR)win);
  }
  else
  {
    win = (nglConsole*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  }
  return win ?
    win->WindowProc(hwnd, uMsg, wParam, lParam) :
    DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT nglConsole::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  RECT rect;
  switch(uMsg)
  {
  case WM_CREATE:
    {
      GetClientRect(hwnd,&rect);
      mOutputWnd =
        CreateWindow(
          _T("EDIT"),
          NULL,
          WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | ES_WANTRETURN,
          0, 0, rect.right, rect.bottom - mInputHeight-1,
          hwnd,
          NULL,
          App->GetHInstance(),
          this);
      //NGL_OUT(_T("Create console output window hwnd = 0x%x"), mOutputWnd);

      PostMessage(mOutputWnd, WM_SETFONT,  (WPARAM)App->GetSystemFont(), 0);
      PostMessage(mOutputWnd, EM_LIMITTEXT,(WPARAM)BACKLOG_SIZE, 0);
      // Override default proc
      mpOutputWndProc = (WNDPROC)GetWindowLongPtr(mOutputWnd, GWLP_WNDPROC);
      SetWindowLongPtr(mOutputWnd, GWLP_WNDPROC, (DWORD_PTR)ConsoleOutputProc);
      SetWindowLongPtr(mOutputWnd, GWLP_USERDATA, (DWORD_PTR)this);

      // input accept files, todo : for scripts
      mInputWnd = CreateWindowEx(
        /*WS_EX_ACCEPTFILES+*/WS_EX_STATICEDGE,
        _T("EDIT"),
        NULL,
        WS_CHILD+WS_VISIBLE+ES_AUTOHSCROLL,
        0, rect.bottom - mInputHeight, rect.right, mInputHeight,
        hwnd,
        NULL,
        App->GetHInstance(),
        this);
      //NGL_OUT(_T("Create console input window hwnd = 0x%x"), mInputWnd);

      PostMessage(mInputWnd ,WM_SETFONT,(WORD)App->GetSystemFont(),0);
      // Override default proc
      mpInputWndProc = (WNDPROC)GetWindowLongPtr(mInputWnd, GWLP_WNDPROC);
      SetWindowLongPtr(mInputWnd, GWLP_WNDPROC, (DWORD_PTR)ConsoleInputProc);
      SetWindowLongPtr(mInputWnd, GWLP_USERDATA, (DWORD_PTR)this);
      return 0;
    }

  case WM_CLOSE:
    {
      if (OnClose())
        DestroyWindow(hwnd);
      return 0;
    }
  
  case WM_SIZE:
    {
      if (mOutputWnd)
        MoveWindow (mOutputWnd, 0, 0, LOWORD(lParam), HIWORD(lParam)-mInputHeight, TRUE);
      if (mInputWnd)
        MoveWindow (mInputWnd, 0, HIWORD(lParam)-mInputHeight, LOWORD(lParam), mInputHeight, TRUE);
      return 0;
    }
  
  case WM_ACTIVATE:
    {
      if (mInputWnd && LOWORD(wParam) != WA_INACTIVE)
        PostMessage(mInputWnd, WM_ACTIVATE, wParam, (LPARAM)mInputWnd);
      return 0;
    }

  case WM_SETFOCUS:
    {
      if (mInputWnd)
        SetFocus(mInputWnd);
      return 0;
    }
  
  case WM_SYSCOMMAND:
    {
      if (wParam==MENU_QUITAPP_ID)
      {
        PostQuitMessage(0);
        //App->Quit(0);
        return 0;
      }
    }
    break;
  
  case WM_KEYDOWN:
    {
      switch (wParam)
      {
      case VK_TAB:
        if (mInputWnd)
          SetFocus(mInputWnd);
        return 0;
      }
    }
  }

  return DefWindowProc(hwnd,uMsg,wParam,lParam);
}


/*
 * Output control proc
 */

LRESULT CALLBACK ConsoleOutputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  nglConsole*win = NULL;
  win = (nglConsole*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  return win ?
    win->OutputProc(hwnd, uMsg, wParam, lParam) :
    DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT nglConsole::OutputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
/*
  if (uMsg == WM_ERASEBKGND)
    return 1;
*/
  if (CheckAccel(hwnd, uMsg, wParam, lParam))
    return 0; // This is an accelerator, intercept

  if (mpOutputWndProc)
    return CallWindowProc(mpOutputWndProc, mOutputWnd, uMsg, wParam, lParam);
  return DefWindowProc(mOutputWnd, uMsg, wParam, lParam);
}    


/*
 * Input control proc
 */

LRESULT CALLBACK ConsoleInputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  nglConsole*win = NULL;
  win = (nglConsole*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  return win ?
    win->InputProc(hwnd, uMsg, wParam, lParam) :
    DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT nglConsole::InputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (CheckAccel(hwnd, uMsg, wParam, lParam))
    return 0; // This is an accelerator, intercept

  switch (uMsg)
  {
  case WM_KEYDOWN:
    switch (wParam)
    {
    case VK_RETURN:
      {
        UINT cCommand = SendMessage(hwnd,EM_LINELENGTH,(WPARAM) 0,(LPARAM) 0);
        if (cCommand>0)
        {
          nglString line;
          int result;
          char *szCommand=(char*)malloc(cCommand+1);
          if (!szCommand) return 0; // fixme!
          *((WORD*)szCommand) = (cCommand+1); // The message EM_GETLINE take the first word of the buffer as the size of the buffer (shitty win32 trick!)
          result = SendMessage(hwnd,EM_GETLINE,(WPARAM) 0,(LPARAM)(LPCSTR) szCommand);
          if (result)
          {
            szCommand[cCommand] = 0;
            line = szCommand;

            line.Trim();
            OnInput(line);
            free(szCommand);

            // reset input command line
            PostMessage(hwnd,WM_SETTEXT,0,(long) "");

            // reset history position
            mHistoryPosition = -1;
          }
        }
        return 0;
      }

    case VK_TAB: // completion
      // fixme! 'ding' sound on tab press !?! how comes ?
      {
        list<nglString> templist;
        int word = 0;
        int cursor = 0;
        nglString line;
        UINT cCommand = SendMessage(hwnd,EM_LINELENGTH,(WPARAM) 0,(LPARAM) 0);
        if (cCommand>0)
        {
          if (cCommand < 2) cCommand=2;
          char *szCommand=(char*)malloc(cCommand+1);
          if (!szCommand) return 0; // fixme!

          *(short *)szCommand = cCommand;
          cCommand = SendMessage(hwnd,EM_GETLINE,(WPARAM) 0,(LPARAM)(LPCSTR) szCommand);
          szCommand[cCommand] = 0;
          line = szCommand;
          free(szCommand);

          cursor = SendMessage(hwnd,EM_GETSEL,0,0);
          cursor = LOWORD(cursor);
        
          word=max(0, line.FindLast(_T(' '),line.GetLength()-cursor));
        }

        Completion(line,word,cursor,templist);

        switch (templist.size())
        {
        case 0:
//          PlaySound("SystemExclamation", NULL, SND_SYNC);
          break;
        case 1:
          line.Delete(word);
          line.Append(*templist.begin());
          line.Append(_T(' '));
          PostMessage(hwnd,WM_SETTEXT,(WPARAM) 0,(long) line.GetChars());
          cursor = SendMessage(hwnd,EM_SETSEL,line.GetLength(),line.GetLength());
          break;
        default:
          {
            list<nglString>::iterator it;
            list<nglString>::iterator end=templist.end();
//            PlaySound("SystemExclamation", NULL, SND_SYNC);
            Output(_T("\n"));
            for (it=templist.begin(); it!=end; ++it)
              Output(_T("%ls\t"), (*it).GetChars());

            Output(_T("\n"));
          }
          break;
        }
        return 0;
      }

    case VK_UP: // history up
      {
        mHistoryPosition++;
        if (mHistoryPosition >= (int)mHistory.size())
          mHistoryPosition = mHistory.size() - 1;

        if (mHistoryPosition != -1)
        {
          nglString& text = GetHistory(mHistoryPosition);
          int text_size = text.GetLength();
          PostMessage(hwnd,WM_SETTEXT,(WPARAM) 0,(LPARAM) text.GetChars());
          // put cursor at end of command line
          PostMessage(hwnd,EM_SETSEL,(WPARAM) text_size,(LPARAM) text_size);
        }

        return 0;
      }

    case VK_DOWN: // history down
      {
        mHistoryPosition--;
        if (mHistoryPosition >= 0)
        {
          nglString& text = GetHistory(mHistoryPosition);
          int text_size = text.GetLength();
          PostMessage(hwnd,WM_SETTEXT, 0, (long)text.GetChars());
          PostMessage(hwnd,EM_SETSEL,text_size,text_size);
        }
        else if (mHistoryPosition == -1) // if not already at history bottom
          // fixme! dont wreck current command line when browsing history, and put it back here !
          PostMessage(hwnd,WM_SETTEXT, 0, (long)_T(""));
        else
          mHistoryPosition = -1;

        return 0;
      }
    }
    break;
  }

  if (mpInputWndProc)
    return CallWindowProc(mpInputWndProc, mInputWnd, uMsg, wParam, lParam);
  return DefWindowProc(mInputWnd, uMsg, wParam, lParam);
}


/*
 * Keyboard accelerators
 */

bool nglConsole::CheckAccel(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg != WM_KEYDOWN)
    return false;

  int scancode = (lParam & 0xFF0000) >> 16;
  bool is_ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x80000000) != 0;

  // Check Ctrl+² (inverse behaviour of nglWindow accel)
  if (scancode == 0x29 && is_ctrl)
  {
    App->GetConsole().Show(false);
    return true;
  }

  // Check Ctrl+Q or Ctrl+Break
  if ((wParam == 0x51 /*VK_Q*/ && is_ctrl) ||
       wParam == VK_CANCEL)
  {
    PostQuitMessage(0);
    //App->Quit(0);
    return true;
  }

  if ( mInputWnd && (wParam == VK_TAB) )
  {
    SetFocus(mInputWnd);
    return true;
  }

  
  return false;
}


/*
 * Win32 standard console (polling)
 */

DWORD WINAPI nglConsole::ThreadProc(LPVOID pParameter)
{
  INPUT_RECORD in;
  unsigned long num;

  nglConsole* pThis = (nglConsole*)pParameter;
  if(!pThis || pThis->mHCIn == NULL) 
    return 0;

  nglString buf;

  while(PeekConsoleInput(pThis->mHCIn,&in,1,&num))
  {
    ReadConsoleInput(pThis->mHCIn,&in,1,&num);
    switch(in.EventType)
    {
      case FOCUS_EVENT:
      pThis->mHasfocus = (in.Event.FocusEvent.bSetFocus == TRUE);
      case KEY_EVENT:
      if(pThis->mHasfocus && in.Event.KeyEvent.bKeyDown == false)
      {
        switch(in.Event.KeyEvent.wVirtualKeyCode)
        {
          case VK_RETURN:
            NGL_LOG(_T("nglConsole"), NGL_LOG_DEBUG, _T("%s"),pThis->mLine.NewLine);
            pThis->OnInput(pThis->mLine);
            pThis->mLine.Wipe();
            break;
          default:
            if (in.Event.KeyEvent.uChar.AsciiChar >= 32 && in.Event.KeyEvent.uChar.AsciiChar < 128)
            {// is printable
              NGL_LOG(_T("nglConsole"), NGL_LOG_DEBUG, _T("%c"),in.Event.KeyEvent.uChar.AsciiChar);
              pThis->mLine.Append(in.Event.KeyEvent.uChar.AsciiChar);
            }
            break;
        }
      }
    }
  }

  return 1;
}
