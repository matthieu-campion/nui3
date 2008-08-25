// gameswf_video_ogl.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// OpenGL video implementation

#include "gameswf_types.h"
#include "gameswf_video_ogl.h"
#include "gameswf_video_base.h"

video_ogl::video_ogl():
	m_texture(0),
	m_scoord(0),
	m_tcoord(0),
	m_width2p(0),
	m_height2p(0)
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// GL_NEAREST ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
};

video_ogl::~video_ogl()
{
	glDeleteTextures(1, &m_texture);
}

void video_ogl::update_video(Uint8* data, int width, int height)
{
	gameswf::tu_autolock locker(m_mutex);
	if (m_data)
	{
		delete m_data;
	}
	m_data = data;
	m_width = width;
	m_height = height;

	m_width2p = 1; while (m_width2p < m_width) { m_width2p <<= 1; }
	m_height2p = 1; while (m_height2p < m_height) { m_height2p <<= 1; }
	m_scoord = (float) m_width / m_width2p;
	m_tcoord = (float) m_height / m_height2p;
}

void video_ogl::display(const gameswf::matrix* mat, const gameswf::rect* bounds, const gameswf::rgba& color)
{
	if (m_width == 0 && m_height == 0)
	{
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	{
		// lock m_data
		gameswf::tu_autolock locker(m_mutex);

		if (m_data)
		{

			// don't use compressed texture for video
			// it slows down video
//			ogl::create_texture(GL_RGBA, m_width2p, m_height2p, NULL);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width2p, m_height2p, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
			delete m_data;
			m_data = NULL;
		}
	}

	m = mat;
	m_bounds = bounds;
	gameswf::point a, b, c, d;
	m->transform(&a, gameswf::point(m_bounds->m_x_min, m_bounds->m_y_min));
	m->transform(&b, gameswf::point(m_bounds->m_x_max, m_bounds->m_y_min));
	m->transform(&c, gameswf::point(m_bounds->m_x_min, m_bounds->m_y_max));
	d.m_x = b.m_x + c.m_x - a.m_x;
	d.m_y = b.m_y + c.m_y - a.m_y;

	glColor4ub(color.m_r, color.m_g, color.m_b, color.m_a);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glTexCoord2f(0, 0);
		glVertex2f(a.m_x, a.m_y);
		glTexCoord2f(m_scoord, 0);
		glVertex2f(b.m_x, b.m_y);
		glTexCoord2f(0, m_tcoord);
		glVertex2f(c.m_x, c.m_y);
		glTexCoord2f(m_scoord, m_tcoord);
		glVertex2f(d.m_x, d.m_y);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


