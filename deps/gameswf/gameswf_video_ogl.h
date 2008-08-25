// gameswf_video_ogl.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_VIDEO_OGL_H
#define GAMESWF_VIDEO_OGL_H

#include "base/tu_opengl_includes.h"
#include "gameswf_video_base.h"

struct video_ogl : public gameswf::video_handler
{
	GLuint m_texture;
	float m_scoord;
	float m_tcoord;
	int m_width2p;
	int m_height2p;

	video_ogl();
	~video_ogl();

	void update_video(Uint8* data, int width, int height);
	void display(const gameswf::matrix* mat, const gameswf::rect* bounds, const gameswf::rgba& color);

};

#endif
