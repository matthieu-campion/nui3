/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglLog.h
\brief Log and debug facilities
*/

#ifndef __nglLog_h__
#define __nglLog_h__

//#include "nui.h"
#include "nglString.h"
#include "nglReaderWriterLock.h"
#include "nglCriticalSection.h"
class nglOStream;

/* Verbosity levels
 */
#define NGL_LOG_ALWAYS  (0)
#define NGL_LOG_ERROR   (1)
#define NGL_LOG_WARNING (2)
#define NGL_LOG_INFO    (3)
#define NGL_LOG_DEBUG   (8)


//! Logging facilities
/*!
The nglLog class is a simple facility to manage event logging or simple debugging.
A nglLog object can have :

- multiple outputs, such as console, files or other output streams
- many domains, each with their own verbose level
- configurable stamping of events
- uniform output in any case, easily parsable for diagnosis

Domains let you easily classify the events in your application : if you follow
some simple guidelines, you will be able to tune very precisely which messages
you want to log and their given verbose level.

NGL programs have an <em>application log</em> which is always created at
startup and used internally by NGL to report errors and other useful
informations. Have a look at nglApplication::GetLog() and
nglApplication::ParseDefaultArgs() to see how this facility can help you.
*/
class NGL_API nglLog
{
public:
  typedef uint StampFlags;

  static const StampFlags NoStamp;      ///< Do not stamp log lines (raw output)
  static const StampFlags TimeStamp;    ///< Add a time stamp (HH:MM:SS format)
  static const StampFlags DateStamp;    ///< Add a date stamp (YY/MM/DD format)
  static const StampFlags DomainStamp;  ///< Add a domain stamp (domain:)

  /** @name Life cycle */
  //@{
  nglLog(bool UseConsole = true);
  /*!<
    \param UseConsole output events to the console

    You can add outputs later. Set \a UseConsole explicitly to false if you
    want to use only alternate methods (file, network). See the AddOutput()
    and UseConsole() methods.
  */
  virtual ~nglLog();
  /*!< Flush all pending events and clos all current outputs. */
  //@}

  /** @name Formatting flags */
  //@{
  void       SetFlags (StampFlags Flags);  ///< Set output flags
  StampFlags GetFlags ();                  ///< Get current output flags
  //@}

  /** @name Output selection */
  //@{
  void       UseConsole(bool Use);  ///< Send log output to the application's console
  bool       AddOutput (nglOStream* pStream);
  /*!<
    Add an output stream
    \param pStream new output stream
    \return true if the stream was succesfully added
  */
  bool       DelOutput (nglOStream* pStream);
  /*!<
    Remove an output stream
    \param pStream output stream to remove
    \return true if the domain was found and succesfuly removed
  */
  //@}

  /** @name Domain management */
  //@{
  uint       GetLevel (const nglChar* pDomain);
  /*!<
    Get the verbose level from a domain name
    \param pDomain domain name
    \return verbose level. This is set to zero if the domain is unknown
  */
  void       SetLevel (const nglChar* pDomain, uint Level);
  /*!<
    Set the verbose level of a domain
    \param pDomain domain name
    \param Level new verbose level

    If \p pDomain is set to "all", \p Level will be applied to all known domains
    and used as the default verbose level for yet unknown domains.
  */
  //@}

  /** @name Output methods */
  //@{
  void       Log (const nglChar* pDomain, uint Level, const nglChar* pText, ...);
  /*!<
    Log an event
    \param pDomain domain name
    \param Level verbose level
    \param pText format string
    \param ... arguments

    The domain name is case sensitive, use them consistently in your programs.
    The "" empty string is a valid domain; however note that the ':' separator will still be
    displayed if you activated domain stamping, in order to keep log parsing consistent.

    The message \p Level is compared against the domain's current verbose level : if the first
    is lower or equal to the latter, the event is logged. As a consequence, level 0 events are
    unmaskable, since a domain verbose level is at least 0.

    Trailing newlines are automaticaly removed, and multiples lines message have each of their
    lines properly stamped.
  */
  void       Logv (const nglChar* pDomain, uint Level, const nglChar* pText, va_list Args);
  /*!<
    Log an event
    \param pDomain domain name
    \param Level verbose level
    \param pText format string
    \param Args arguments

    Vararg variant of the Log() method.
  */
  void       Dump (uint Level = 0) const;  ///< Dumps domain usage statistics using \p Level verbosity
  //@}

private:
  class Domain
  {
  public:
    nglString Name;
    nglAtomic Level;
    nglAtomic Count;

    Domain(const nglChar* pName, uint LogLevel) : Name(pName), Level(LogLevel), Count(0) {}
  };
  typedef std::list<nglOStream*> OutputList;
  typedef std::vector<Domain>    DomainList;

  uint       mDefaultLevel;
  bool       mUseConsole;
  StampFlags mStampFlags;
  nglString  mOutputBuffer;
  nglString  mPrefix;
  nglString  mBody;
  nglString  mDomainFormat;
  uint       mDomainFormatLen;
  OutputList mOutputList;
  DomainList mDomainList;

  nglLog(const nglLog&) {} // Undefined copy constructor

  Domain* LookupDomain (const nglChar* pName);
  void    Output (const nglString& rText) const;

  mutable nglReaderWriterLock mLock;
  nglCriticalSection mCS;
};

#endif // __nglLog_h__
