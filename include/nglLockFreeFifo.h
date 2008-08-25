/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		LockFreeFifo
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class nglLockFreeFifo
{
public:
	nglLockFreeFifo (uint32 bufsz) : m_readidx(0), m_writeidx(0), m_buffer(bufsz)
	{
  }

	T Get(void)
	{
		if (mReadIdx == mWriteIdx)
			throw runtime_error ("lock free fifo underrun");
		
		T result = mBuffer[mReadIdx];
		
		if ((mReadIdx + 1) >= mBuffer.size())
			mReadIdx = 0;
		else
			mReadIdx = mReadIdx + 1;

		return result;
	}

	void Put(T datum)
	{
		unsigned newIdx;
		
		if ((mWriteIdx + 1) >= mBuffer.size())
			newIdx = 0;
		else
			newIdx = mWriteIdx + 1;

		if (newIdx == mReadIdx)
			throw runtime_error ("lock free fifo overrun");
		
		mBuffer[mWriteIdx] = datum;
		
		mWriteIdx = newIdx;
	}
	
private:

	volatile unsigned  mReadIdx, mWriteIdx;
  std::vector<T> mBuffer;
};

