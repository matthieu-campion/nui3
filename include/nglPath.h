/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nglPath_h__
#define __nglPath_h__

#include "nglTime.h"
#include "nglString.h"

class nglIStream;
class nglIOStream;

//! nglPath base
/*!
Used by nglPath(nglPathBase) to fetch a system dependent path.
*/
enum nglPathBase
{
	ePathCurrent,   ///< Use the current working directory as root (note: the cwd cannot be changed by the application)
	ePathUser,      ///< Use a user-related ('unix sense') location as root
	ePathTemp,      ///< Use a general purpose scratch pad location as root
	ePathApp,        ///< This application's binary file path, including the executable file name
	ePathUserAppSettings, ///< Use a user-related ('unix sense') location as root for application settings(may not be visible by default)
	ePathUserDocuments, ///< Use a user-related ('unix sense') location as root for application documents
  ePathUserPreferences, ///< Use a user-related ('unix sense') location as root for preferences
  ePathUserDesktop    ///< Use the desktop folder as root
};


//! File path information
/*!
A path can be either a leaf (a file, or what can be emulated as a file such as
pipes, device nodes and so on) or a non-leaf (a folder). All fields are only valid
if \a Exists is true. The \a Size value is always zero when the node is not a
leaf.
*/
class nglPathInfo
{
public:
	bool Exists;      ///< Node existence. All other fields are invalid if set to false.
	bool IsLeaf;      ///< Leaf (file or assimilable) or non-leaf (folder)
	bool CanRead;     ///< True if the file (leaf) can be read or the folder (non-leaf) can be traversed and its content listed
	bool CanWrite;    ///< True if the file (leaf) can be written to or a new node can be created in this folder (non-leaf)
  bool Visible;     ///< True if the file is visible, false if it is hidden
	nglTime LastAccess;  ///< nglTime stamp of last access (read or exec)
	nglTime LastMod;     ///< nglTime stamp of last modification (write)
	nglFileSize Size;      ///< If the node is a leaf, size in bytes. If non-leaf, always zero.
};


class nglVolume;
class nglPathVolume;

//! Portable file system access
/*!
Every OS has its own way to handle file systems and its own path name conventions.
The nglPath class tries to make this job more portable (rather than relying simply on
strings), and provide services to explore the host file system.

Some terminology : a nglPath object designates a \e node in the local file system.
The \e node can be either :
- a \e leaf (files and also pipes, sockets, devices, more generally nodes behaving like files)
- a \e non-leaf (folder)
- invalid (an incomplete path name)
*/
class NGL_API nglPath
{
public:
	//! Portable char set
	/*!
	All strings passed to nglPath are tested against a valid char set, for
	portability purposes.

	\code
	const char nglPath::ValidChars[] =
	"/.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_";
	\endcode
	*/
	static const nglChar PortableCharset[];

	/** @name Life cycle */
	//@{
	nglPath();
	/*!<
	nglPath default constructor
	*/
	nglPath(const nglChar* pPathName);
	/*!< nglPath constructor
	\param pPathName path name, using current locale's encoding

	For portability reasons, any '\\' (anti-slash) characters are replaced by '/' (slash).
	*/
	nglPath(const nglString& rPathName);
	/*!< nglPath constructor
	\param rPathName path name

	For portability reasons, any '\\' (anti-slash) characters are replaced by '/' (slash).
	*/
	nglPath(nglPathBase Base);
	/*!< nglPath constructor
	\param Base path base (see nglPathBase)

	Build a path from a system-dependent location, see nglPathBase.
	*/
	nglPath(const nglPath& rPath);
	/*!<
	nglPath copy constructor
	*/
	~nglPath();
	//@}

	/** @name Node operations */
	//@{
	bool Move(const nglPath& PathTarget);
	/*!< Move the current path
	\param PathTarget new path to use
	\return True if the operation completed successfully.
	*/
  //@{
	bool Copy(const nglPath& PathTarget) const;
	/*!< Copy the file
   \param PathTarget new path to use
   \return True if the operation completed successfully.
   */
	bool Delete(bool Recurse = false) const;
	/*!< Delete the current node, and optionally its children if any
	\param Recurse if the node is a folder, delete all its content (files and folders) recursively
	\return True if the operation completed successfully. A false return in recursive mode
	means that the operation might have partially succeeded (zero or more nodes were deleted).
	*/
	bool Create(bool Recurse = true) const;
	/*!< Interpret current path as a directory and create it
	\param Recurse create intermediate folders if necessary
	\return True if the operation completed successfully. A false return in recursive mode
	means that the operation might have partially succeeded (zero or more nodes created).
	*/
	nglPath Find(const nglString &FileName) const;
	/*!< Find a child from the current path
	\param FileName name of the child to find
	\return the absolute path of the child if found otherwise return an empty path.
	*/
	//@}

	/** @name nglPath information */
	//@{
	bool      Canonize();               ///< Interpret \e . and \e .. pseudo paths to give a unequivocal path name (does not flatten symlinks)
	const nglString& GetPathName() const;      ///< Returns the path name as a string (same information as GetChar())
	nglString GetNodeName() const;      ///< Returns the last path component (node name)
	nglString GetExtension() const;     ///< Returns the file extension (without the dot), or an empty string if the path is a folder or a file without extension
	nglString GetRemovedExtension() const;     ///< Returns the path without extension (and without the dot)
  void SetExtension(const nglString& rExtension); ///< Changes the extension of the file (or add one if needed).
  nglString GetParentName() const;  ///< Returns the parent path as a string (remove the last node)
	nglPath   GetParent() const;      ///< Returns the parent path (remove the last node)
  void Split(std::vector<nglString>& rElements); ///< Split the elements of this path into its elements (folders and eventual file))
	int32 GetChildren(std::list<nglPath>* pChildren) const; ///< deprecated
	int32 GetChildren(std::list<nglPath>& pChildren) const; ///< that's the proper api
	int32 GetChildrenTree(std::list<nglPath>& pChildren) const; ///< Get the children recursively to get a complete tree.
 
	/*!< Get node's children
	\param pChildren if non-null, children will be appended to this list
	\return Number of children. Zero usually means the node was a leaf, check errors with GetError()

	Do not expect any children ordering, you must explicitly apply an alpha sort if so
	you wish. Children path names contain their own parent's path.
	*/
	const nglChar* GetChars() const;    ///< Retrieve path name as a nglChar array (suitable to varargs methods)
	operator const nglChar*() const;    ///< Retrieve path name as a nglChar array (suitable to varargs methods)
	//@}

	/** @name Node information */
	//@{
	bool GetInfo (nglPathInfo& rInfo) const;  ///< Returns node info, see nglPathInfo
	bool Exists() const;                      ///< Check existence of the current node
	bool CanRead() const;                     ///< Check read/traverse access on current node
	bool CanWrite() const;                    ///< Check write access on current node
	bool      IsLeaf() const;                      ///< Test whether the node is a leaf (file and assimilated) or not (folder). Calls GetInfo() internally.
	nglTime      GetLastAccess() const;               ///< Returns last (read) access time stamp. Calls GetInfo() internally.
	nglTime      GetLastMod() const;                  ///< Returns last modification (write) time stamp. Calls GetInfo() internally.
	nglFileSize    GetSize() const;                     ///< Returns node size in bytes (folders always return zero). Calls GetInfo() internally.
	nglString    GetMimeType() const;
	/*!< Returns the standard MIME type description from file extension.
	If the path is not a file or the type is unknown, returns an empty string
	*/
	bool IsVisible() const;                    ///< Check visibility on current node
	bool IsBundle() const;                    ///< Returns true is the path is a MacOSX bundle/package
	//@}
	bool      ResolveLink(); ///< If this object is a link (Win32) or an alias (MacOS) it is transformed into the path it points too. Otherwise it isn't changed.
  bool      MakeRelativeTo(const nglPath& rOriginal); ///< Change this path so that it is relative to rOriginal. Return false if the operation fails.
  
  nglIStream* OpenRead() const;
  nglIOStream* OpenWrite(bool OverWrite = true) const;
  
	/** @name Special (OS dependent) paths */
	//@{
	bool IsAbsolute() const;       ///< Returns whether the path is absolute or relative
	nglPath     GetAbsolutePath() const;  ///< Returns the absolute path name (from file system or volume root)
	static uint64  GetVolumes(std::list<nglPathVolume>& rVolumes, uint64 Flags);
	/*!< List physical 'volumes' and locate them on the file system
	\param rVolumes volume descriptions will be appended to this list
	\param Flags filter : only list volumes that have at least the bits from \a Flags set.
	Use nglPathVolume::All to fetch all possible volumes.
	\return number of volumes found

	\e Volume is a term to specify special entry points in the host file system,
	like hard disk partition roots, CD/DVD-ROM mount points or drive letter, network shares
	and so on.
	*/
	//@}

	/** @name Operators */
	//@{
	const nglPath& operator=(const nglChar* pSource);       ///< Initialize a path from a string using locale's encoding
	const nglPath& operator=(const nglString& rSource);  ///< Initialize a path from a string
	const nglPath& operator=(const nglPath& rSource);    ///< Copy a path

	const nglPath& operator+=(const nglPath& rAppend);
	/*!< nglPath concatenation
	\param rAppend path to append

	See nglPath::operator+ for more information.
	*/
	const nglPath& operator+=(const nglString& rAppend);
	/*!< nglPath concatenation
	\param rAppend path to append

	Variant of the operator+=(const nglPath&),
	the right operand will be nglPath(rAppend).
	*/
  const nglPath& operator+=(const nglChar* pAppend);
	/*!< nglPath concatenation
   \param pAppend path string to append
   
   Variant of the operator+=(const nglPath&),
   the right operand will be nglPath(pAppend).
   */

  friend NGL_API nglPath operator+ (const nglPath& rPath, const nglPath& rAppend);
	/*!< nglPath concatenation
	\param rPath current object
	\param rAppend path to append

	The concatenation operator will always consider the left operand to be a folder.
	nglPath will take care of proper concatenation regarding the '/' separator.
	You cannot use this operator to add an extension to a file name, you must
	use the nglString domain (see GetPathName() and nglPath::nglPath).

	Here is what you must expect (see the examples/file test) :

	\code
	nglPath("/usr/bin")  + nglPath("perl")  // path name is '/usr/bin/perl'
	nglPath("/usr/bin/") + nglPath("perl")  //              '/usr/bin/perl'
	nglPath("/usr/bin")  + nglPath("/perl") //              '/usr/bin/perl'
	nglPath("/usr/bin/") + nglPath("/perl") //              '/usr/bin/perl'
	\endcode
	*/
	friend NGL_API nglPath operator+ (const nglPath& rPath, const nglString& rAppend);
	/*!< nglPath concatenation
	\param rPath current object
	\param rAppend path to append

	Variant of the operator+(const nglPath&, const nglString&),
	the right operand will be nglPath(rAppend).
	*/
	friend NGL_API bool operator==(const nglPath& rLeft, const nglPath& rRight);
	/*!< nglPath comparison
	The comparison is case sensitive, depending on the underlying file system and OS
	*/
	friend NGL_API bool operator==(const nglPath& rLeft, const nglString& rRight);
	/*!< nglPath comparison
	The comparison is case sensitive, depending on the underlying file system and OS
	*/
	friend NGL_API bool operator==(const nglString& rLeft, const nglPath& rRight);
	/*!< nglPath comparison
	The comparison is case sensitive, depending on the underlying file system and OS
	*/
	friend NGL_API bool operator!=(const nglPath& rLeft, const nglPath& rRight);
	/*!< nglPath comparison
	The comparison is case sensitive, depending on the underlying file system and OS
	*/
	friend NGL_API bool operator!=(const nglPath& rLeft, const nglString& rRight);
	/*!< nglPath comparison
	The comparison is case sensitive, depending on the underlying file system and OS
	*/
	friend NGL_API bool operator!=(const nglString& rLeft, const nglPath& rRight);
	/*!< nglPath comparison
	The comparison is case sensitive, depending on the underlying file system and OS
	*/
	//@}

  friend bool operator<(const nglPath& rLeft, const nglPath& rRight);
	/*!< nglPath comparison
   The comparison is case sensitive
   */
	//@}
  
  nglString GetVolumeName() const;
  nglString GetVolumeLessPath() const;  
protected:
	nglString   mPathName;

#if (defined _UNIX_) || (defined _CARBON_) || (defined _UIKIT_) || (defined _COCOA_)
	typedef std::map<nglString, nglString, nglString::LessFunctor> MimeMap;

	static nglTime mMimeTypeStamp;
	static MimeMap mMimeType;
#endif // _UNIX_ || _CARBON_ || _UIKIT_ || _COCOA_

	bool InternalSetPath (const nglChar* pPath);
	//  bool ValidateChars();
	int32 GetRootPart() const;
  
};

bool operator<(const nglPath& rLeft, const nglPath& rRight);

//! Volume description
/*!
Volume description, as returned by nglPath::GetVolumes().
*/
class NGL_API nglPathVolume
{
public:
	typedef uint64 VolumeFlags;
	//! Volume media type
	enum MediaType
	{
		eTypeUnknown,
		eTypeFloppy,
		eTypeHD,
		eTypeCD,
		eTypeDVD,
		eTypeZip,
		eTypeTape,
		eTypeNetwork
	};
  
	nglPathVolume(const nglPath& rPath = nglString::Null, const nglString& rComment = nglString::Null, nglPathVolume::VolumeFlags Flags = 0, nglPathVolume::MediaType Type = eTypeUnknown);
	nglPathVolume(const nglPathVolume& rPathVolume);
	~nglPathVolume();

	nglPathVolume& operator=(const nglPathVolume& rPathVolume);



	static const VolumeFlags All;       ///< All flags set
	static const VolumeFlags ReadOnly;  ///< Volume is read-only
	static const VolumeFlags Removable; ///< Volume is removable
	static const VolumeFlags Offline;   ///< Volume is not connected/inserted
	static const VolumeFlags System;    ///< System internal volume

	nglPath            mPath;      ///< nglPath to access this volume
	nglString          mComment;   ///< Comment (volume label, share comment, etc)
	VolumeFlags     mFlags;  ///< Various properties
	MediaType       mType;   ///< Volume media type

private:
#ifdef _CARBON_
	int mRefNum;
	friend class nglPath;
	friend class nglKernel;
	friend pascal OSStatus nglVolumeEventHandler (EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
	static void UpdateVolumes(std::list<nglPathVolume>& rVolumes);
	static nglPathVolume AddVolume(std::list<nglPathVolume>& rVolumes, int32 volnum);
	static nglPathVolume DelVolume(std::list<nglPathVolume>& rVolumes, int32 volnum);
#endif // _CARBON_
  
#ifdef _COCOA_
  friend class nglPath;
  static void UpdateVolumes(std::list<nglPathVolume>& rVolumes);
#endif // _COCOA_
};

bool nglComparePath(const nglPath& rLeft, const nglPath& rRight);
bool nglCompareNaturalPath(const nglPath& rLeft, const nglPath& rRight);


#endif // __nglPath_h__
