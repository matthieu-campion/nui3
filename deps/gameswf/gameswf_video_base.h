// gameswf_video_base.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// base class for video


#ifndef GAMESWF_VIDEO_BASE_H
#define GAMESWF_VIDEO_BASE_H

#include "base/smart_ptr.h"
#include "gameswf.h"
#include "gameswf_mutex.h"

namespace gameswf
{
	struct video_handler : public ref_counted
	{
		// multithread !!!
		Uint8* m_data;
		int m_width;
		int m_height;
		const matrix* m;
		const rect* m_bounds;
		tu_mutex m_mutex;

		video_handler();
		~video_handler();

		virtual void display(const matrix* m, const rect* bounds, const rgba& cx) = 0;
		virtual void update_video(Uint8* data, int width, int height) = 0;
	};
}

#endif
