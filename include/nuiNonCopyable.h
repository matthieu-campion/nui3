/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiNonCopyable
{
protected:
  nuiNonCopyable()
  {
  }
  virtual ~nuiNonCopyable()
  {
  }
private:
	nuiNonCopyable( const nuiNonCopyable& );
	const nuiNonCopyable& operator=( const nuiNonCopyable& );
};

