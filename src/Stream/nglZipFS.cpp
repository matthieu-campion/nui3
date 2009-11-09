/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglZipFS.h"
#include "nglIFile.h"
#include "nglIZip.h"
#include "unzip.h"

nglZipPath::nglZipPath(const nglZipFS* pZipFS, const nglString& rPathInZip)
  : nglPath(rPathInZip)
{
  mpZipFS = pZipFS;
}

nglZipPath::nglZipPath(const nglZipPath& rPath)
  : nglPath(rPath)
{
  mpZipFS = rPath.mpZipFS;
}

nglZipPath::~nglZipPath()
{
}

bool nglZipPath::Delete(bool Recurse)
{
  return false;
}

bool nglZipPath::Create(bool Recurse)
{
  return false;
}

int  nglZipPath::GetChildren(std::list<nglZipPath>* pChildren) const
{
  return mpZipFS->GetChildren(*this, *pChildren);
}

int  nglZipPath::GetChildren(std::list<nglPath>* pChildren) const
{
  std::list<nglZipPath> Children;
  GetChildren(&Children);

  std::list<nglZipPath>::iterator it;
  std::list<nglZipPath>::iterator end = Children.end();

  for (it = Children.begin(); it != end; ++it)
    pChildren->push_back(*it);

  return Children.size();
}

bool nglZipPath::GetInfo (nglPathInfo& rInfo) const
{
  return mpZipFS->GetInfo(*this, rInfo);
}

bool nglZipPath::Exists() const
{
  nglPathInfo Info;
  GetInfo(Info);
  return Info.Exists;
}

bool nglZipPath::CanRead() const
{
  return mpZipFS->CanRead() && Exists();
}

bool nglZipPath::CanWrite() const
{
  return false;
}

bool nglZipPath::Decompose(const nglPath& rPath, std::list<nglPath>& rList)
{
  nglPath parent(rPath.GetParent());
  nglString tmp = rPath.GetNodeName();
  nglPath node(tmp.IsNull()? nglString::Empty : tmp);

  if (parent == rPath)
    return true;

  rList.push_front(node);
  return Decompose(parent, rList);

  return true;
}


// nglZipFS

// Low level file access stubs:
voidpf ZCALLBACK zOpen(voidpf opaque, const char* filename, int mode)
{
//  return ((nglZipFS*)opaque)->mpStream;
  return opaque;
}

uLong ZCALLBACK zRead(voidpf opaque, voidpf stream, void* buf, uLong size)
{
  uLong res;
  res = ((nglIStream*)stream)->Read(buf, size, 1);

  return res;
}

uLong ZCALLBACK zWrite(voidpf opaque, voidpf stream, const void* buf, uLong size)
{
//  return ((nglIStream*)stream)->Write(buf, size, 1);
  return 0;
}

long ZCALLBACK zTell(voidpf opaque, voidpf stream)
{
  return (long)((nglIStream*)stream)->GetPos();
}

long ZCALLBACK zSeek(voidpf opaque, voidpf stream, uLong offset, int origin)
{
  nglStreamWhence whence;

  switch (origin)
  {
  case ZLIB_FILEFUNC_SEEK_CUR:
    whence = eStreamForward;
    break;
  case ZLIB_FILEFUNC_SEEK_SET:
    whence = eStreamFromStart;
    break;
  case ZLIB_FILEFUNC_SEEK_END:
    whence = eStreamFromEnd;
    break;
  default:
    return -1;
  }

  ((nglIStream*)stream)->SetPos(offset, whence);
  return 0;
}

int ZCALLBACK zClose(voidpf opaque, voidpf stream)
{
  // Reset the stream as we can't close a stream, so the next time open is called it will still work as expected (hopefully)...
  ((nglIStream*)stream)->SetPos(0, eStreamFromStart);
  return 0;
}

int ZCALLBACK zError(voidpf opaque, voidpf stream)
{
  return ((nglIStream*)stream)->GetState();
}


class nglZipPrivate
{
public:
  nglZipPrivate()
  {
    mZip = NULL;
  }
  virtual ~nglZipPrivate()
  {
    if (mZip)
      unzClose(mZip);
  }

private:
  unzFile mZip;

  friend class nglZipPath;
  friend class nglZipFS;
};


static zlib_filefunc_def zlib_filefunc;


nglZipFS::nglZipFS(nglIStream* pStream, bool Own)
: mRoot(_T(""), 0, 0, 0, false)
{
  mpStream = pStream;
  mOwnStream = Own;
	mIsValid = mpStream != NULL;

	if (mpStream)
		mpStream->SetEndian(eEndianIntel);
	
  mpPrivate = new nglZipPrivate();
  NGL_ASSERT(mpPrivate);
}

nglZipFS::nglZipFS(const nglPath& rPath)
: mRoot(_T(""), 0, 0, 0, false)
{
  mpStream = rPath.OpenRead();
  mOwnStream = true;
	mIsValid = mpStream != NULL;

	if (mpStream)
		mpStream->SetEndian(eEndianIntel);
	
  mpPrivate = new nglZipPrivate();
  NGL_ASSERT(mpPrivate);
}

nglZipFS::~nglZipFS()
{
  if (mpPrivate)
    delete mpPrivate;
  if (mOwnStream && mpStream)
    delete mpStream;
}

bool nglZipFS::IsValid() const
{
	return mIsValid;
}

bool nglZipFS::Open()
{	
  zlib_filefunc.opaque = mpStream;
  zlib_filefunc.zopen_file  = &::zOpen;
  zlib_filefunc.zread_file  = &::zRead;
  zlib_filefunc.zwrite_file = &::zWrite;
  zlib_filefunc.ztell_file  = &::zTell;
  zlib_filefunc.zseek_file  = &::zSeek;
  zlib_filefunc.zclose_file = &::zClose;
  zlib_filefunc.zerror_file = &::zError;

	if (mpStream == NULL || mpPrivate == NULL || !mIsValid)
		return false;
	
  mpPrivate->mZip = unzOpen2("", &zlib_filefunc);

  if (mpPrivate->mZip == NULL)
    return false;

  return BuildIndex();
}

bool nglZipFS::BuildIndex()
{
  int res;
  unzFile Zip = mpPrivate->mZip;
  unz_global_info global_info;

  if (UNZ_OK != unzGetGlobalInfo(Zip, &global_info))
    return false;

  if (UNZ_OK != unzGoToFirstFile(Zip))
    return false;

  do 
  {
    unz_file_info file_info;
    unz_file_pos file_pos;
    char filename[4096];

    if (UNZ_OK != unzGetCurrentFileInfo(Zip, &file_info, filename, 4096, NULL, 0, NULL, 0))
      return false;

    if (UNZ_OK != unzGetFilePos(Zip, &file_pos))
      return false;

    uint len = strlen(filename);
    bool IsDir = (filename[len-1] == '\\') || (filename[len-1] == '/');
    std::list<nglPath> List;

    nglZipPath::Decompose(filename, List);

    std::list<nglPath>::iterator it;
    std::list<nglPath>::iterator end = List.end();

    Node* pPath = &mRoot;

    int i = List.size();
    for (it = List.begin(); it != end; ++it)
    {
      nglString name = (*it).GetPathName();
      Node* pChild = pPath->Find(name);
      if (!pChild)
      {
        pChild = new Node(name, file_info.uncompressed_size, file_pos.pos_in_zip_directory, file_pos.num_of_file, i == 1 && !IsDir);
        pPath->AddChild(pChild);
      }
      pPath = pChild;
      i--;
    }
  }
  while (UNZ_OK == (res = unzGoToNextFile(Zip)));

  if (res == UNZ_END_OF_LIST_OF_FILE)
    return true;
  return false;
}


bool nglZipFS::CanWrite() const
{
  return false;
}

bool nglZipFS::CanRead() const
{
  return mpStream->GetState() == eStreamReady;
}

bool nglZipFS::GetInfo (const nglZipPath& rPath, nglPathInfo& rInfo) const
{
  Node* pChild = mRoot.Find(rPath); 

  rInfo.Exists = pChild != NULL;
  rInfo.CanRead = CanRead() && rInfo.Exists;
  rInfo.CanWrite = CanWrite();
  rInfo.IsLeaf = (pChild != NULL) ? (pChild->mIsLeaf) : false;
  rInfo.Size = (pChild != NULL) ? (pChild->mSize) : 0;
  rInfo.Visible = true; ///< Always visible...

  return pChild != NULL;
}

int nglZipFS::GetChildren(const nglZipPath& rPath, std::list<nglZipPath>& rList) const
{
  Node* pNode = mRoot.Find(rPath);
  if (!pNode)
    return 0;

  std::list<nglZipFS::Node*>::iterator it;
  std::list<nglZipFS::Node*>::iterator end = pNode->mpChildren.end();
  for (it = pNode->mpChildren.begin(); it != end; ++it)
  {
    if (*it)
    {
      nglZipPath path(this, rPath.GetPathName().IsEmpty()? (*it)->mName : rPath.GetPathName() );
      if (!rPath.GetPathName().IsEmpty())
        path += nglPath((*it)->mName);
        
      rList.push_back(path);
    }
  }
  return rList.size();
}

nglIZip* nglZipFS::GetStream(const nglZipPath& rPath)
{
  Node* pNode = mRoot.Find(rPath);
  if (!pNode)
    return NULL;
  if (!pNode->mIsLeaf)
    return NULL;

  unz_file_pos file_pos;
  file_pos.num_of_file = pNode->mNumOfFile;
  file_pos.pos_in_zip_directory = pNode->mPosInZipDirectory;

  if (unzGoToFilePos(mpPrivate->mZip, &file_pos) != UNZ_OK)
    return NULL;

  if (unzOpenCurrentFile(mpPrivate->mZip) != UNZ_OK)
    return NULL;

  void* pUnzip = unzGetCurrentFile(mpPrivate->mZip);

  if (!pUnzip)
    return NULL;

  return new nglIZip(this, pUnzip, pNode->mSize, pNode->mNumOfFile, pNode->mPosInZipDirectory);
}

// Direct File Operations:
bool nglZipFS::Close(void* pUnzip) 
{
  if (pUnzip == NULL)
    return false;

  unzSetCurrentFile(mpPrivate->mZip, pUnzip);
  return UNZ_CRCERROR != unzCloseCurrentFile(mpPrivate->mZip);
}

nglStreamState nglZipFS::GetState(void* pUnzip) const
{
  if (pUnzip == NULL)
    return eStreamError;

  unzSetCurrentFile(mpPrivate->mZip, pUnzip);
  if (unzeof(mpPrivate->mZip))
    return eStreamEnd;
  
  return eStreamReady;
}

uint nglZipFS::GetError(void* pUnzip) const
{
  return GetState(pUnzip) == eStreamError ? 1 : 0;
}

nglFileOffset nglZipFS::GetPos(void* pUnzip) const
{
  unzSetCurrentFile(mpPrivate->mZip, pUnzip);
  return unztell(mpPrivate->mZip);
}

nglFileOffset nglZipFS::SetPos (void* pUnzip, nglFileOffset Where, nglIZip* pFile)
{
  unzSetCurrentFile(mpPrivate->mZip, pUnzip);
  char dummy[1024];
  nglFileOffset Pos = GetPos(pUnzip);

  if (Where < Pos)
  {
    unzCloseCurrentFile(mpPrivate->mZip);

    unz_file_pos file_pos;
    file_pos.num_of_file = pFile->mNumOfFile;
    file_pos.pos_in_zip_directory = pFile->mPosInZipDirectory;
    unzGoToFilePos(mpPrivate->mZip, &file_pos);

    if (unzOpenCurrentFile(mpPrivate->mZip) != UNZ_OK)
      return 0;

    pUnzip = unzGetCurrentFile(mpPrivate->mZip);

    if (!pUnzip)
      return 0;

    pFile->SetRef(pUnzip);
    Pos = 0;
  }

  Where -= Pos;
  while (Where > 0)
  {
    nglFileOffset size = MIN(Where, 1024);
    nglSize res = Read(pUnzip, dummy, (long)size, 1, eEndianNative);

    if (res <= 0) // An Error occured
      return GetPos(pUnzip);

    Where -= res;
  }

  return GetPos(pUnzip);
}


nglSize nglZipFS::Read (void* pUnzip, void* pData, nglSize WordCount, uint WordSize, nglEndian nglEndian)
{
  if (unzSetCurrentFile(mpPrivate->mZip, pUnzip) != UNZ_OK)
    return 0;
  nglSize done = unzReadCurrentFile(mpPrivate->mZip, pData, WordCount * WordSize);
  done /= WordSize;

  if ((done > 0) && (nglEndian != eEndianNative))
  {
    switch (WordSize)
    {
      case 2: bswap_16_s ((uint16*)pData, done); break;
      case 4: bswap_32_s ((uint32*)pData, done); break;
      case 8: bswap_64_s ((uint64*)pData, done); break;
    }
  }

  return done;
}


nglZipFS::Node::Node(const nglString& rPath, uint Size, uint Pos, uint Num, bool IsLeaf)
{
  mIsLeaf = IsLeaf;

  mSize = Size;
  mPosInZipDirectory = Pos;
  mNumOfFile = Num;

  mName = rPath;
}

nglZipFS::Node::~Node()
{
  std::list<nglZipFS::Node*>::iterator it;
  std::list<nglZipFS::Node*>::iterator end = mpChildren.end();
  for (it = mpChildren.begin(); it != end; ++it)
  {
    if (*it)
      delete *it;
  }
}

nglZipFS::Node* nglZipFS::Node::Find(const nglPath& rPath) const
{
  if (rPath.GetPathName() == _T("") || rPath.GetPathName() == _T(".")  || rPath.GetPathName() == _T("/"))
    return (nglZipFS::Node*)this;
  std::list<nglPath> elems;
  nglZipPath::Decompose(rPath, elems);
  std::list<nglPath>::iterator it;
  std::list<nglPath>::iterator end = elems.end();

  Node* pChild = NULL; 
  const Node* pRoot = this; 
  for (it = elems.begin(); it != end && pRoot != NULL; ++it)
  {
    pChild = pRoot->Find((*it).GetPathName());
    pRoot = pChild;
  }

  return pChild;
}

nglZipFS::Node* nglZipFS::Node::Find(const nglString& path) const
{
  if (path == _T("") || path == _T(".")  || path == _T("/"))
    return (nglZipFS::Node*)this;
  std::list<nglZipFS::Node*>::const_iterator it;
  std::list<nglZipFS::Node*>::const_iterator end = mpChildren.end();
  for (it = mpChildren.begin(); it != end; ++it)
    if ((*it)->mName == path)
      return *it;
  return NULL;
}

bool nglZipFS::Node::AddChild(nglZipFS::Node* pPath)
{
  mpChildren.push_back(pPath);
  return true;
}
