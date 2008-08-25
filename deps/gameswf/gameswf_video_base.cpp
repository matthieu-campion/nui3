// gameswf_video_base.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// base class for video


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <memory.h>
#include "gameswf.h"
#include "base/tu_types.h"
#include "gameswf_video_base.h"

namespace gameswf
{

	video_handler::video_handler():
		m_data(NULL),
		m_width(0),
		m_height(0)
	{
	}

	video_handler::~video_handler()
	{
		tu_autolock locker(m_mutex);
		if (m_data)
		{
			delete m_data;
			m_data = NULL;
		}
	}

}  // namespace gameswf
