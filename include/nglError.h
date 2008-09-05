/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



/*!
\file  nglError.h
\brief Simple general purpose error class
*/

#ifndef __nglError_h__
#define __nglError_h__

//#include "nui.h"
#include <vector>


/* nglString surclases nglError, so we only use a forward declaration here
 */
class nglString;


//! Simple error management
/*!
Methods which may fail for any reason should have a uniform way to report an
error and let the user retrieve a human readable message. Sometimes it is not
possible to report an error in a convenient way (ie. from a constructor or when
a method returns a reference).

A class which may generate complex and various errors from a constructor or
a method should heritate from the nglError class. It is sufficient to have
one nglError as ancestor, since the error table can be extended along the
successive derivations.

The principle is very simple : you provide an error string table which is
implicitely indexed from 0 (no error) to \e N, where \e N is the number
of possible errors.

\code
#define MYCLASS_ENONE     0
#define MYCLASS_EODD      1
#define MYCLASS_ESTRANGE  2
#define MYCLASS_EFUZZY    3
#define MYCLASS_ELAST     3

const nglChar* gpMyClassErrorStr[] =
{
  "No error",                 // 0
  "Something odd happened",   // 1
  "Strange bug encountered",  // 2
  "Fuzzy thingy",             // 3
  NULL
};

class MyClass : public nglError
{
public:
  Handle* OpenRessource();

protected:
  const nglChar* OnError(int& rError)
  {
    return FetchError(gpMyClassErrorStr, NULL, rError);
  }
};
\endcode

The previous example defines three errors (plus a default 'no error' status)
for your class \e MyClass. If you have a function which may generate errors,
either let it signal that an error occured (return false or NULL for instance)
or urge the user to call a GetError() right after its execution to check
errors :

\code
// If an error occurs, returns NULL and set a local error.
Handle* MyClass::OpenRessource()
{
  Handle* h;
  h = sys_openressource();
  if (h == FAILED)
  {
    SetError (MYCLASS_EODD);
    return NULL;
  }
  return h;
}
\endcode

The user can easily retrieve the error code and message with the GetError() and
GetErrorStr() methods. Note that in debug mode, NGL automatically displays error
messages via the default domain of the application log.

The macros are convenient to name errors in your program. Never use error codes
directly, or your code will be unmaintanable and unreadable. You must only
check carefully that the macros definitons are in sync with the error message
table.

For the derived classes, you can extend the error table :

\code
#define MYCLASSCHILD_EBASE    (MYCLASS_ELAST+1)
#define MYCLASSCHILD_EWIZZY   (MYCLASSCHILD_EBASE + 0)
#define MYCLASSCHILD_ECLASH   (MYCLASSCHILD_EBASE + 1)
#define MYCLASSCHILD_ELAST    (MYCLASSCHILD_EBASE + 1)

const nglChar* gpMyClassChildErrorStr[] =
{
  "Wizzy wazza",                               // 0
  "Complete system clash, run for your life",  // 1
  NULL
};

class MyClassChild : public MyClass
{
public:
  Handle* OpenMoreRessource();

protected:
  const nglChar* OnError(int& rError)
  {
    return FetchError(gpMyClassErrorStr, MyClass::OnError(rError), rError);
  }
};
\endcode

Derived classes inheritate their ancestor's errors. Conceptually they should
not set any of their errors but only interpret them. However it is correct to
simply pass the ancestor's error rather than giving an abstraction :

\code
// If an error occurs, returns NULL and set a local error.
Handle* MyClasschild::OpenMoreRessource()
{
  Handle* h;
  h = OpenRessource();
  // No test : if h is NULL, an error is already set by MyClass.
  return h;
}
\endcode
*/
class NGL_API nglError
{
public:
  /** @name Life cycle */
  //@{
  nglError();
  virtual ~nglError();
  //@}

  /** @name User services */
  //@{
  uint           GetError() const;               ///< Retrieve the current error code
  const nglChar* GetErrorStr() const;            ///< Retrieve the current error message
  const nglChar* GetErrorStr(uint Error) const;  ///< Retrieve error message by code
  //@}

protected:
  /** @name Programmer services */
  //@{
  bool SetError (uint Error) const;
  /*!<
    Set error code.
    \param Error error code

    The error code cannot be negative or greater than the error table last
    element index. If the code is out of range, this method returns false.
  */
  bool SetError (const nglChar* pLogDomain, uint Error) const;
  /*!<
    Set error code within a log domain.
    \param pLogDomain logging domain name
    \param Error error code

    The error code cannot be negative or greater than the error table last
    element index. If the code is out of range, this method returns false.

    The \a pLogDomain is only used for log purposes : if the application is
    built in debug mode, errors will automatically be displayed through the
    current application log object. This is an easy way to catch \b all
    errors occuring in your application. This parameter is ignored in release
    mode.
  */
  const nglChar* FetchError (const nglChar** pTable, const nglChar* pParent, uint& rError) const;
  /*!<
    \param pTable error string table, first message has offset 0
    \param pParent parent class error message
    \param rError error code
    \return error message, NULL if the \a rError offset is out of bounds

    This method scans an error string table and retrieves the error message at offset
    \a rError. If \a pParent is non-NULL, this message is returned : this is an helper
    for recursive scanning of parent class string tables. See OnError() for more info.
  */
  virtual const nglChar* OnError (uint& rError) const = 0;
  /*!<
    \param rError error code
    \return error message, or NULL if \a rError is out of bounds

     When the user invokes GetErrorStr() on an object, the OnError() callback is
     called with the given error code and should return the corresponding message.
     When several classes have different string error tables in the same hierarchy,
     the latest derivation's OnError() is called : it should then use a parent-first
     search. Example :

\code
const nglChar** ChildErrorTable = {_T("no error"), _T("Bad luck"), NULL};

const nglChar* Child::OnError (uint& rError) const
{
  return FetchError(ChildErrorTable, Parent::OnError(rError), rError);
}
\endcode
  */
  //@}

private:
  mutable uint mError;

  bool InternalSetError (const nglChar* pLogDomain, uint Error, bool UseDomain) const;
};


#endif // __nglError_h__
