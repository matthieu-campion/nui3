/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"

#define NGL_CONSOLE_PROMPT _T("> ")


nglConsole::nglConsole(bool IsVisible)
{
  mIsVisible = IsVisible;
  if (mIsVisible) NGL_OUT (NGL_CONSOLE_PROMPT);
//  App->AddEvent (this);
}

nglConsole::~nglConsole()
{
}


/*
 * Public methods
 */

void nglConsole::Show (bool IsVisible)
{
  mIsVisible = IsVisible;
}

/*
 * Internals
 */

void nglConsole::OnOutput (const nglString& rText)
{
  printf (rText.GetChars());
  fflush (stdout);
}

#define BUFFER_MAX 4096

void nglConsole::OnEvent(int Flags)
{
//  char buffer[BUFFER_MAX+1];
//  int count;
}

#ifdef __MWERKS__

/* Metrowerks Standard Library
 * Copyright   1995-2001 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2008-04-14 15:58:33 $
 * $Revision: 1.4 $
 */
 
/*****************************************************************************/
/* Project...: Standard ANSI-C Library              */
/* Purpose...: Stubs for console.c               */
/*****************************************************************************/

#ifndef __CONSOLE__
#include <console.h>
#endif

/*
 * The following four functions provide the UI for the console package.
 * Users wishing to replace SIOUX with their own console package need
 * only provide the four functions below in a library.
 */

/*
 * extern short InstallConsole(short fd);
 *
 * Installs the Console package, this function will be called right
 * before any read or write to one of the standard streams.
 *
 * short fd:  The stream which we are reading/writing to/from.
 * returns short: 0 no error occurred, anything else error.
 */

short InstallConsole(short fd)
{
#pragma unused (fd)

 return 0;
}

/*
 * extern void RemoveConsole(void);
 *
 * Removes the console package.  It is called after all other streams
 * are closed and exit functions (installed by either atexit or _atexit)
 * have been called.  Since there is no way to recover from an error,
 * this function doesn't need to return any.
 */

void RemoveConsole(void)
{
}

/*
 * extern long WriteCharsToConsole(char *buffer, long n);
 *
 * Writes a stream of output to the Console window.  This function is
 * called by write.
 *
 * char *buffer: Pointer to the buffer to be written.
 * long n:   The length of the buffer to be written.
 * returns short: Actual number of characters written to the stream,
 *     -1 if an error occurred.
 */


long WriteCharsToConsole(char *buffer, long n)
{
#if 1
#pragma unused (buffer, n)
#else
 Ptr tPtr = NewPtr(n + 1);
 if (NULL != tPtr)
 {
  BlockMoveData(buffer,tPtr + 1,n);
  if (n < 256)
   tPtr[0] = n;
  else
   tPtr[0] = 255;
  DebugStr((UInt8*) tPtr);
  DisposePtr(tPtr);
  return n;
 } else
#endif
 return 0;
}

/*
 * extern long WriteCharsToErrorConsole(char *buffer, long n);
 *
 * Writes a stream of output to the Error Console window.  This function is
 * called by write.
 *
 * char *buffer: Pointer to the buffer to be written.
 * long n:   The length of the buffer to be written.
 * returns short: Actual number of characters written to the stream,
 *     -1 if an error occurred.
 */


long WriteCharsToErrorConsole(char *buffer, long n)
{
#if 1
#pragma unused (buffer, n)
#else
 Ptr tPtr = NewPtr(n + 1);
 if (NULL != tPtr)
 {
  BlockMoveData(buffer,tPtr + 1,n);
  if (n < 256)
   tPtr[0] = n;
  else
   tPtr[0] = 255;
  DebugStr((UInt8*) tPtr);
  DisposePtr(tPtr);
  return n;
 } else
#endif
 return 0;
}

/*
 * extern long ReadCharsFromConsole(char *buffer, long n);
 *
 * Reads from the Console into a buffer.  This function is called by
 * read.
 *
 * char *buffer: Pointer to the buffer which will recieve the input.
 * long n:   The maximum amount of characters to be read (size of
 *     buffer).
 * returns short: Actual number of characters read from the stream,
 *     -1 if an error occurred.
 */

long ReadCharsFromConsole(char *buffer, long n)
{
#pragma unused (buffer, n)

 return 0;
}

/*
 * extern char *__ttyname(long fildes);
 *
 * Return the name of the current terminal (only valid terminals are
 * the standard stream (ie stdin, stdout, stderr).
 *
 * long fildes: The stream to query.
 *
 * returns char*: A pointer to static global data which contains a C string
 *     or NULL if the stream is not valid.
 */

extern char *__ttyname(long fildes)
{
#pragma unused (fildes)
 /* all streams have the same name */
 static char *__devicename = "null device";

 if (fildes >= 0 && fildes <= 2)
  return (__devicename);

 return (0L);
}

/* Begin mm 981218 */
/*
*
*    int kbhit()
*
*    returns true if any keyboard key is pressed without retrieving the key
*    used for stopping a loop by pressing any key
*/
int kbhit(void)
{
      return 0; 
}

/*
*
*    int getch()
*
*    returns the keyboard character pressed when an ascii key is pressed  
*    used for console style menu selections for immediate actions.
*/
int getch(void)
{
      return 0; 
}

/*
*     void clrscr()
*
*     clears screen
*/
void clrscr()
{
 return;
}
/* End mm 981218 */

/* Change record:
 * mm  981218 Added stubs for kbhit(), getch(), and clrscr()
 */

#endif
