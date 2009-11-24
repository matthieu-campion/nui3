/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\File  nglZipFS.h
\brief Zip as a virtual file system.
*/

#ifndef __nglZipFS_h__
#define __nglZipFS_h__

#include "nglStream.h"
#include "nglPath.h"
#include "nglVolume.h"

class nglIStream;


class nglIZip;
class nglZipFS;
class nglZipPath;
class nglZipPrivate;

class NGL_API nglZipPath : public nglPath
{
public:
  nglZipPath(const nglZipFS* pZipFS, const nglString& rPathInZip = _T("")); 
  nglZipPath(const nglZipPath& rPath);
  virtual ~nglZipPath();

  virtual bool Delete(bool Recurse = false);
  virtual bool Create(bool Recurse = true);
  virtual int  GetChildren(std::list<nglPath>* pChildren) const;
  virtual int  GetChildren(std::list<nglZipPath>* pChildren) const;
  virtual bool GetInfo (nglPathInfo& rInfo) const;
  virtual bool Exists() const;
  virtual bool CanRead() const;
  virtual bool CanWrite() const;

  static bool Decompose(const nglPath& rPath, std::list<nglPath>& rList);
  /*!< Create a list of recursive node path from the given complete path
       If you give /truc/machin/prout.zip you will get a list with three elements :
       truc, machin and prout.zip.
  */

private:
  const nglZipFS* mpZipFS;
};

class NGL_API nglZipFS : public nglVolume
{
public:
  nglZipFS(const nglString& rVolumeName, nglIStream* pStream, bool Own = true); ///< Create a Zip FS from a stream.
  nglZipFS(const nglPath& rPath);                 ///< Create a Zip FS from a file. 
  virtual ~nglZipFS();

  // From nglVolume:
  virtual bool GetPathInfo(const nglPath& rPath, nglPathInfo& rInfo);
  virtual bool MakeDirectory(const nglPath& rPath);
  virtual bool Delete(const nglPath& rPathToDelete);
  virtual bool Move(const nglPath& rSource, const nglPath& rPathTarget);
  virtual nglIStream* OpenRead(const nglPath& rPath);
  virtual nglIOStream* OpenWrite(const nglPath& rPath, bool OverWrite);
  virtual bool GetChildren(const nglPath& rPath, std::list<nglPath>& pChildren);

  bool Open();

  bool CanWrite() const;
  bool CanRead() const;
  bool GetInfo (const nglZipPath& rPath, nglPathInfo& rInfo) const;
  int  GetChildren(const nglZipPath& rPath, std::list<nglZipPath>& rList) const; ///< Populate the list with the children of the given path node in the zip.

  nglIZip* GetStream(const nglZipPath& rPath); ///< Return a stream that can read the file pointed to by rPath in the Zip FS.

private:
  // Needed to build an index of the zip:
  class Node
  {
  public:
    Node(const nglString& rPath, uint Size, uint Pos, uint Num, bool IsLeaf);
    virtual ~Node();
    Node* Find(const nglPath& rPath) const;
    Node* Find(const nglString& rNodeName) const;
    bool AddChild(Node* pPath);

  private:
    std::list<Node*> mpChildren;
    uint      mSize;
    uint      mPosInZipDirectory;  /* offset in zip file directory */
    uint      mNumOfFile;          /* # of file */
    bool      mIsLeaf;
    nglString mName;

    friend class nglZipFS;
  };

  nglIStream*    mpStream;
  bool           mOwnStream;
  nglZipPrivate* mpPrivate;
  Node           mRoot; ///< The zip file's root directory

  // Direct File Operations:
  bool           Close(void* pUnzip);
  nglStreamState GetState(void* pUnzip) const;
  uint           GetError(void* pUnzip) const;

  nglFileOffset GetPos(void* pUnzip) const;
  nglFileOffset SetPos (void* pUnzip, nglFileOffset Where, nglIZip* pFile);
  nglFileSize Available (void* pUnzip, uint WordSize = 1);

  nglSize Read (void* pUnzip, void* pData, nglSize WordCount, uint WordSize, nglEndian nglEndian);

  bool BuildIndex(); // Build the internal ZIP directory

  friend class nglZipPath;
  friend class nglIZip;
};        

#endif // __nglZipFS_h__
