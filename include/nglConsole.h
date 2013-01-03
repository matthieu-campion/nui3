/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglConsole.h
\brief Interactive console
*/

#ifndef __nglConsole_h__
#define __nglConsole_h__

//#include "nui.h"
#include "nglEvent.h"
#include "nglString.h"
#include "nglCriticalSection.h"

#define NGL_DISABLE_CONSOLE 0


//! Interactive portable console
/*!
The console represents a simple terminal connnected to the current application,
allowing text ouput and user input. An application can have only one console
at once, or no console at all. To ensure there is only one instance at once,
the reference to the console object is held in the application instance. It means
that instantiating a new console object will automatically a) delete any
previous instance and b) attach the new console to the application. Exemple :

\code
new nglConsole();
nglConsole& con = App->GetConsole();
con.UseHistory();
con.UseCompletion();
con.Show();
\endcode

You can fetch the reference to the current console object with \a App->GetConsole().
NGL does not automatically instantiate a console object at application startup.
However, if the user did not provided a console and \a App->GetConsole() is called,
the default console (of type \e nglConsole) is immediatly built and used. It means
that the example above still works the same if the first line is removed.

The default console is implemented in different ways, depending on the platform :

- Windows user have a new option in each NGL window system menu (the icon on
the top left corner) to display the console. You can programmatically force
the display of the console (or hide it) with the Show() method.
The Window console is a 'handcrafted' facility.

- Unix-like terminal emulators (xterm, getty and co.) are automatically used as
the NGL console when launched through them. Applications not attached to a tty
(such as those run with the ending '&' shell directive, or by third party launchers
such as you favorite desktop browser) cannot have a 'true' console support. The
output will still be written to stdout, but no interactive input is possible
(even with pipes). From the programmer's point of view, everything works the same
but the input and completion callbacks will never be called.
The Show() method only controls the apparition of the prompt.


\par Using input

If you want to receive and interpret console input, you must create
your own console class from nglConsole and overload the OnInput() method. Example :

\code
class MyConsole : public nglConsole
{
public:
  MyConsole() : nglConsole (true) // Show on creation
  {
    UseHistory();
    UseCompletion();
  }

  virtual void OnInput (const nglString& rLine)
  {
    if (!rLine.Compare("exit")) Quit (0); else
      NGL_OUT ("sorry, unknown command\n");
  }
};

MyApplication::OnInit()
{
  new MyConsole(); // And voila !
}
\endcode

This code does the same job as the first example : it creates a console,
activates history and completion and shows it at startup. But here you can
overload the OnInput() and OnCompletion() callbacks and program an interactive
console. Since the application console is used as default output for the log
facility (see nglLog), it is a good idea to instantiate your console as soon as
possible (there is no sooner than the first line of application's \a OnInit() !).


\par Using Output

You might want to intercept the default console output, to display it in a
nglWindow with the help of nuiFont for instance. All you have to do is to
overload the OnOutput() callback and code a drawing routine.
Here is a little example showing how to code a GL console based on this principle :

\code
class GLConsole : public nglConsole
{
  nglString mBacklog;

public:
  GLConsole() : nglConsole (false) {} // Hide default console

  virtual void OnOutput (const nglString& rLine)
  {
    mBacklog += rLine;
    Draw();
  }

  void Draw()
  {
    // Here some GL code to display mBacklog content
  }
};
\endcode


\par History and completion

The NGL console can manage two major services for you : \e history and \e completion.

History is not automatically available, you must call UseHistory() first.
When enabled, a call to AddToHistory() will insert a new entry into history,
and the user will be able navigate through the last entries with the \e up and
\e down cursor keys. Usually AddToHistory() is called from the OnInput()
callback. The history buffer can be limited by line count and/or
total chars count, or not limited at all, see SetHistory().

You can also enable \e history \e expansion, which is a facility inspired from
GNU readline. The user can begin a line with the '!' (exclamation) character,
and type the first few letters of a recent command. If expansion is enabled,
the console will automatically search (from more recent to older entries) an
entry in history matching these first letters when the user strikes 'enter'.
Matching case sensitiveness is controled via SetHistoryCase().
The '!!' expression is interpreted as 'repeat last command'.
If completion is enabled, it is also possible to complete against history
(see \e completion below).

Completion is not automatically available, you must call UseCompletion() first.
When enabled, the user can trigger a completion request by hitting the \e Tab
key. The OnCompletion() callback is triggered, and you are required to give
a list of matching items from a context (current line and word to complete).
If only one item (one match) is returned, the current word is immediatly
replaced with this item. If several matches are returned, the current word is
replaced with the least common denominator of all matches : these are the first
similar characters of all matches, 'similarity' case sensitiveness being
controlled by SetCompletionCase(). When no further completion is possible,
hitting \e Tab twice display all possible matches.
See OnCompletion() documentation for more informations.

If both \e history \e expansion and \e completion are enabled, a completion
request on the first word (beginning of the line) \e and with a heading '!'
is automatically completed against history. The OnCompletion() callback
is not triggered in this case, since it is internally handled by NGL.
*/
class NGL_API nglConsole : public nglEvent
{
public:
  /** @name Life cycle */
  //@{
  nglConsole (bool IsVisible = false);
  /*!<
    Create a console.
    \param IsVisible show console immediatly, see Show()

    Set application console to this newly created instance, delete the previous
    one if any. A reference to this object is held for you in nglBaseApp and
    can be retrieved at any moment with a call to \a App->GetConsole().
  */
  virtual ~nglConsole();
  /*!<
    Destroy the console.

    An application can have no console at all, however all nglLog objects using the
    application console as their only output will be muted, and their text
    data will be sent into deep space.
  */
  //@}

  /** @name Output */
  //@{
  void Output  (const nglChar* pFormat, ...);
  void Outputv (const nglChar* pFormat, va_list Args);
  void Output  (const nglString& rText);
  void Show    (bool IsVisible = true);
  /*!<
    Display or hide the console.

    On Windows, display or hide the custom console window. On Unix and
    assimilated (BeOS, MacOSX, etc), simply show or hide the prompt.
  */
  //@}

  /** @name History */
  //@{
  std::list<nglString*>& GetHistory();
  /*!<
    Return all history entries
  */
  int               GetHistory (std::list<const nglString*>& rMatches, nglString& rFilter, bool IsCaseSensitive = false);
  /*!<
    Return all history entries whose first characters match a filter
    \param rMatches history items are appended to this list
    \param rFilter matching filter
    \param IsCaseSensitive wether to perform case sensitive matching
  */
  nglString         GetHistory (uint Line);                                         ///< Return the \a Line-nth entry (0 is the more recent one)
  void              UseHistory (bool Use = true, bool UseExpansion = true);         ///< Activate history
  void              SetHistory (uint LineMax = 100, uint CharMax = 0);
  /*!<
    Set history buffer limits.
    \param LineMax maximum number of lines, 0 means no limit
    \param CharMax maximum total number of chars, 0 means no limit
  */
  void              SetHistoryCase (bool IsCaseSensitive = false);                  ///< Case sensitiveness for history expansion
  void              AddToHistory (const nglString& rLine);                          ///< Add an entry into history
  void              ClearHistory ();                                                ///< Remove all entries from history
  //@}

  /** @name Completion */
  //@{
  void UseCompletion (bool Use = true);                   ///< Activate completion
  void SetCompletionCase (bool IsCaseSensitive = false);  ///< Case sensitiveness for least common denominator of multiple matches, see OnCompletion().
  //@}

  /** @name User callbacks */
  //@{
  virtual void OnOutput (const nglString& rText);
  virtual void OnInput  (const nglString& rLine);
  /*!<
    A line has been entered in the console (the user pressed \e Enter).
    \param rLine text entered

    The rLine string does not include the final carriage return (\\n).
  */
  virtual std::list<nglString>& OnCompletion (nglString& rLine, uint Start, uint End, std::list<nglString>& rAppendTo);
  /*!<
    Completion request.
    \param rLine current line text
    \param Start start of current word (char index in rLine)
    \param End position of cursor (char index in rLine)
    \param rAppendTo list to complete with possible matches (initially empty)

    When the user requests completion (by hitting the \a Tab key), this method is called
    with the current context. You should provide a list of matches based on the substring
    delimited by the \a Start position (first letter of the word to complete) and one
    step before the \a End position (cursor pos). Example :

\code
std::list<nglString>& MyConsole::OnCompletion (nglString& rLine,
                                               uint Start,
                                               uint End,
                                               std::list<nglString>& rAppendTo)
{
  char* commands[] = {"cd", "ls", "echo", "exit", NULL};
  char* args[] = {"toto.jpg", "README", "test.xml", NULL};
  int i;

  if (Start == 0) // first word, this is a command
  {
    // End == 0 : no text entered, propose all commands
    for (i = 0; commands[i]; i++)
      if ((End == 0) || (!rLine.Compare (commands[i], Start, End-Start)))
        rAppendTo.push_front (nglString (commands[i]));
  }
  else // this is a command argument
  {
    // Start == End : no text entered, propose all args
    for (i = 0; args[i]; i++)
      if ((Start == End) || (!rLine.Compare (args[i], Start, End-Start)))
        rAppendTo.push_front (nglString (args[i]));
  }
  return rAppendTo;
}
\endcode

    The current word is completed with the least common denominator of the returned
    list, if non void. The identification of the first common letters of the matches
    is controlled by the SetCompletionCase() method.
    The list is also sorted and displayed when the user hits the \e Tab key twice.

    When the line is empty, expect both \a Start and \a End to be zero. In any case,
    check the case where the word to complete is empty (End == Start) is properly
    handled.
  */
  //@}

private:
  bool                  mUseHistory;
  bool                  mUseHExpansion;
  bool                  mUseCompletion;
  bool                  mHistoryCase;
  bool                  mCompletionCase;
  uint                  mLineMax;
  uint                  mLineCnt;
  uint                  mCharMax;
  uint                  mCharCnt;
  std::list<nglString*> mHistory;
#if !NGL_DISABLE_CONSOLE
  nglString             mOutputBuffer;

  nglCriticalSection mCS;
#endif

  nglConsole(const nglConsole&)
  {
     // Undefined copy constructor
  }

  void                  Setup();
  void                  Input(nglString& rLine);
  std::list<nglString>& Completion(nglString& rLine, uint Start, uint End, std::list<nglString>& rAppendTo);


#if (defined _UNIX_ || defined _CARBON_ || defined _UIKIT_ || defined _COCOA_)
private:
  bool      mIsVisible;
  nglString mInputBuffer;

  void OnEvent(uint Flags);
#endif


#ifdef _WIN32_
public:
  unsigned int mInputHeight;

  void Clear();
  virtual bool OnClose();
  friend int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd);

private:
  int   mOutputPosition;
  int   mHistoryPosition;

  HWND    mHWnd;
  HWND    mOutputWnd;
  HWND    mInputWnd;
  WNDPROC mpOutputWndProc; // Overriden output proc
  WNDPROC mpInputWndProc;  // Overriden input proc

  void Append(const nglChar* pText);

  LRESULT WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
  LRESULT OutputProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);
  LRESULT InputProc (HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);
  bool    CheckAccel(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

  friend LRESULT CALLBACK ConsoleWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
  friend LRESULT CALLBACK ConsoleOutputProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);
  friend LRESULT CALLBACK ConsoleInputProc (HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

  // Win32 standard console code:
  HANDLE mHThread;
  HANDLE mHCIn;
  HANDLE mHCOut;
  unsigned long mThreadId;

  bool mHasfocus;
  bool mConsoleAllocated;

  nglString mLine;

  static DWORD WINAPI ThreadProc(LPVOID lpParameter);
#endif // _WIN32_
};

#endif // __nglConsole_h__
