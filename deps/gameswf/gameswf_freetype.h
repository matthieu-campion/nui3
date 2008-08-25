// gameswf_freetype.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Truetype font rasterizer based on freetype library

#ifndef GAMESWF_FREETYPE_H
#define GAMESWF_FREETYPE_H

#include "base/tu_config.h"

#include "gameswf.h"
#include "gameswf_shape.h"

#if TU_CONFIG_LINK_TO_FREETYPE == 1

#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#define FT_CONST 

namespace gameswf
{

	// helper

	struct glyph_entity
	{
		smart_ptr<bitmap_info> m_bi;
		float m_advance;
		rect m_bounds;
	};

	struct face_entity : public ref_counted
	{
		FT_Face m_face;
		hash<int, glyph_entity*> m_ge;	// <code, glyph_entity>

		face_entity(FT_Face face) :
			m_face(face)
		{
			assert(face);
		}

		~face_entity()
		{
			FT_Done_Face(m_face);
			for (hash<int, glyph_entity*>::iterator it = m_ge.begin(); it != m_ge.end(); ++it)
			{
				delete it->second;
			}
		}

	};

	struct glyph_provider
	{
		glyph_provider(FT_Library	lib);
		~glyph_provider();
		
		face_entity* get_face_entity(const tu_string& fontname,
			bool is_bold, bool is_italic);

		// callbacks
		static int move_to_callback(FT_CONST FT_Vector* vec, void* ptr);
		static int line_to_callback(FT_CONST FT_Vector* vec, void* ptr);
		static int conic_to_callback(FT_CONST FT_Vector* ctrl, FT_CONST FT_Vector* vec, 
			void* ptr);
		static int cubic_to_callback(FT_CONST FT_Vector* ctrl1, FT_CONST FT_Vector* ctrl2,
			FT_CONST FT_Vector* vec, void* ptr);

		image::alpha* draw_bitmap(const FT_Bitmap& bitmap);
		bitmap_info* get_char_image(Uint16 code, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance);
		shape_character_def* get_char_def(Uint16 code,
			const char* fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance);

	private:
		
		FT_Library	m_lib;
		float m_scale;
		smart_ptr<canvas> m_canvas;
		string_hash< smart_ptr<face_entity> > m_face_entity;
	};

}

#else	// TU_CONFIG_LINK_TO_FREETYPE == 0

namespace gameswf
{

	struct glyph_provider
	{
		glyph_provider(void*	lib) {}
		~glyph_provider() {}
		
		bitmap_info* get_char_image(Uint16 code, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance)
		{
			return NULL;
		}

	};
}

#endif		// TU_CONFIG_LINK_TO_FREETYPE

namespace gameswf
{
	exported_module glyph_provider* get_glyph_provider();
	exported_module void create_glyph_provider();
	exported_module void close_glyph_provider();
}

#endif	// GAMESWF_FREETYPE_H
