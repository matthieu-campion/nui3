// gameswf_value.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf.h"
#include "gameswf_value.h"
#include "gameswf_character.h"
#include "gameswf_sprite.h"
#include "gameswf_log.h"
#include "gameswf_function.h"
#include "gameswf_render.h"

#if TU_CONFIG_LINK_TO_LIB3DS == 1
	#include "extensions/lib3ds/gameswf_3ds_inst.h"
#endif

namespace gameswf
{
	// misc
	enum file_type
	{
		UNKNOWN,
		SWF,
		JPG,
		X3DS
	};

	// static
	file_type get_file_type(const char* url)
	{
		tu_string fn = url;
		if (fn.size() < 5)	// At least 5 symbols
		{
			return UNKNOWN;
		}

		tu_stringi fn_ext = fn.utf8_substring(fn.size() - 4, fn.size());

		if (fn_ext == ".swf")
		{
			return SWF;
		}
		else
		if (fn_ext == ".jpg")
		{
			return JPG;
		}
		else
		if (fn_ext == ".3ds")
		{
			return X3DS;
		}
		return UNKNOWN;
	}

	// static
	tu_string get_full_url(const tu_string& workdir, const char* url)
	{
		 tu_string fn;

		// is path relative ?
		if (url[1] == ':' || url[0] == '/')	// like c:\my.swf or /home/my.swf
		{
			fn = "";
		}
		else
		{
			fn = workdir;
		}
		fn += url;

		return fn;
	}

	// static
	const char*	next_slash_or_dot(const char* word)
	// Search for next '.' or '/' character in this word.  Return
	// a pointer to it, or to NULL if it wasn't found.
	{
		for (const char* p = word; *p; p++)
		{
			if (*p == '.' && p[1] == '.')
			{
				p++;
			}
			else if (*p == '.' || *p == '/')
			{
				return p;
			}
		}

		return NULL;
	}


	// url=="" means that the load_file() works as unloadMovie(target)
	character* as_environment::load_file(const char* url, const as_value& target_value)
	{
		character* target = cast_to<character>(find_target(target_value));
		if (target == NULL)
		{
			IF_VERBOSE_ACTION(log_msg("load_file: target %s is't found\n", target_value.to_string()));
			return NULL;
		}

		// is unloadMovie() ?
		if (strlen(url) == 0)
		{
			character* parent = target->get_parent();
			if (parent)
			{
				parent->remove_display_object(target);
			}
			else	// target is _root, unloadMovie(_root)
			{
				target->clear_display_objects();
			}
			return NULL;
		}

		// is path relative ?
		tu_string fn = get_full_url(get_player()->get_workdir(), url);
		switch (get_file_type(fn.c_str()))
		{
			default:
				break;

			case SWF:
			{
				movie_definition*	md = get_player()->create_movie(fn.c_str());
				if (md)
				{
					return target->replace_me(md);
				}
				break;
			}

			case X3DS:
			{
#if TU_CONFIG_LINK_TO_LIB3DS == 0
				log_error("gameswf is not linked to lib3ds -- can't load 3DS file\n");
#else
				x3ds_definition* x3ds = create_3ds_definition(get_player(), fn.c_str());
				if (x3ds)
				{
					if (x3ds->is_loaded())
					{
						rect bound;
						target->get_bound(&bound);
						x3ds->set_bound(bound);
						return target->replace_me(x3ds);
					}
				}
#endif
				break;
			}

			case JPG:
			{
#if TU_CONFIG_LINK_TO_JPEGLIB == 0
				log_error("gameswf is not linked to jpeglib -- can't load jpeg image data!\n");
#else
				image::rgb* im = image::read_jpeg(fn.c_str());
				if (im)
				{
					bitmap_info* bi = render::create_bitmap_info_rgb(im);
					delete im;

					bitmap_character*	jpeg = new bitmap_character(get_player(), bi);
					return target->replace_me(jpeg);
				}
#endif
				break;
			}
		}

		return NULL;

	}


	as_value	as_environment::get_variable(const tu_string& varname, const array<with_stack_entry>& with_stack) const
	// Return the value of the given var, if it's defined.
	{
		// Path lookup rigamarole.
		as_object*	target = get_target();
		tu_string	path;
		tu_string	var;
		if (parse_path(varname, &path, &var))
		{
			// @@ Use with_stack here too???  Need to test.
			target = find_target(path.c_str());
			if (target)
			{
				as_value	val;
				target->get_member(var, &val);
				return val;
			}
			else if(target=get_player()->get_global()->find_target(path.c_str()))
			{
				as_value	val;
				target->get_member(var, &val);
				return val;
			}
			else
			{
				IF_VERBOSE_ACTION(log_msg("find_target(\"%s\") failed\n", path.c_str()));
				return as_value();
			}
		}
		else
		{
			return get_variable_raw(varname, with_stack);
		}
	}


	as_value	as_environment::get_variable_raw(
		const tu_string& varname,
		const array<with_stack_entry>& with_stack) const
	// varname must be a plain variable name; no path parsing.
	{
		as_value	val;

		// First check the with-stack.
		for (int i = with_stack.size() - 1; i >= 0; i--)
		{
			as_object*	obj = with_stack[i].m_object.get_ptr();
			if (obj && obj->get_member(varname, &val))
			{
				// Found the var in this context.
				return val;
			}
		}

		// Then check locals.
		int	local_index = find_local(varname, true);
		if (local_index >= 0)
		{
			return m_local_frames[local_index].m_value;
		}

		// Check movie members.
		if (m_target != NULL && m_target->get_member(varname, &val))
		{
			return val;
		}

		// Check this, _global, _root
		as_standard_member	varname_id = get_standard_member(varname);
		switch (varname_id)
		{
			default:
				break;

			case M_GLOBAL:
				val.set_as_object(get_player()->get_global());
				return val;

			case MTHIS:
				val.set_as_object(get_target());
				return val;

			case M_ROOT:
			case M_LEVEL0:
				val.set_as_object(get_root()->get_root_movie());
				return val;
		}

		// check _global.member
		if (get_player()->get_global()->get_member(varname, &val))
		{
			return val;
		}

		// Fallback.
		IF_VERBOSE_ACTION(log_msg("get_variable_raw(\"%s\") failed, returning UNDEFINED.\n", varname.c_str()));
		return val;
	}


	character*	as_environment::get_target() const
	{
		return cast_to<character>(m_target.get_ptr());
	}

	void as_environment::set_target(character* target)
	{
		m_target = target;
	}

	void as_environment::set_target(as_value& target, character* original_target)
	{
		if (target.is_string())
		{
			tu_string path = target.to_tu_string();
			IF_VERBOSE_ACTION(log_msg("-------------- ActionSetTarget2: %s", path.c_str()));
			if (path.size() > 0)
			{
				character* tar = cast_to<character>(find_target(path.c_str()));
				if (tar)
				{
					set_target(tar);
					return;
				}
			}
			else
			{
				set_target(original_target);
				return;
			}
		}
		else
		if (target.is_object())
		{
			IF_VERBOSE_ACTION(log_msg("-------------- ActionSetTarget2: %s", target.to_string()));
			character* tar = cast_to<character>(find_target(target));
			if (tar)
			{
				set_target(tar);
				return;
			}
		}
		IF_VERBOSE_ACTION(log_msg("can't set target %s\n", target.to_string()));
	}

	void	as_environment::set_variable(
		const tu_string& varname,
		const as_value& val,
		const array<with_stack_entry>& with_stack)
	// Given a path to variable, set its value.
	{
		IF_VERBOSE_ACTION(log_msg("-------------- %s = %s\n", varname.c_str(), val.to_string()));//xxxxxxxxxx

		// Path lookup rigamarole.
		character*	target = get_target();
		tu_string	path;
		tu_string	var;
		if (parse_path(varname, &path, &var))
		{
			target = cast_to<character>(find_target(path.c_str()));
			if (target)
			{
				target->set_member(var, val);
			}
		}
		else
		{
			set_variable_raw(varname, val, with_stack);
		}
	}


	void	as_environment::set_variable_raw(
		const tu_string& varname,
		const as_value& val,
		const array<with_stack_entry>& with_stack)
	// No path rigamarole.
	{
		// Check the with-stack.
		for (int i = with_stack.size() - 1; i >= 0; i--)
		{
			as_object*	obj = with_stack[i].m_object.get_ptr();
			as_value unused;
			if (obj && obj->get_member(varname, &unused))
			{
				// This object has the member; so set it here.
				obj->set_member(varname, val);
				return;
			}
		}

		// Check locals.
		int	local_index = find_local(varname, true);
		if (local_index >= 0)
		{
			// Set local var.
			m_local_frames[local_index].m_value = val;
			return;
		}

		if (m_target != NULL)
		{
			m_target->set_member(varname, val);
		}
		else
		{
			// assume local var
			// This case happens for example so
			// class myclass
			// {
			//		function myfunc()
			//		{
			//			for (i=0;...)		should be for (var i=0; ...)
			//			{
			//			}
			//		}
			//	}
			add_local(varname, val);
			IF_VERBOSE_ACTION(log_error("can't set_variable_raw %s=%s, target is NULL, it's assumed as local\n",
				varname.c_str(), val.to_string()));
			IF_VERBOSE_ACTION(log_error("probably you forgot to declare variable '%s'\n", varname.c_str()));

		}
	}


	void	as_environment::set_local(const tu_string& varname, const as_value& val)
	// Set/initialize the value of the local variable.
	{
		// Is it in the current frame already?
		int	index = find_local(varname, false);
		if (index < 0)
		{
			// Not in frame; create a new local var.
			add_local(varname, val);
		}
		else
		{
			// In frame already; modify existing var.
			m_local_frames[index].m_value = val;
		}
	}

	
	void	as_environment::add_local(const tu_string& varname, const as_value& val)
	// Add a local var with the given name and value to our
	// current local frame.  Use this when you know the var
	// doesn't exist yet, since it's faster than set_local();
	// e.g. when setting up args for a function.
	{
		assert(varname.length() > 0);
		m_local_frames.push_back(frame_slot(varname, val));
	}


	void	as_environment::declare_local(const tu_string& varname)
	// Create the specified local var if it doesn't exist already.
	{
		// Is it in the current frame already?
		int	index = find_local(varname, false);
		if (index < 0)
		{
			// Not in frame; create a new local var.
			add_local(varname, as_value());
		}
		else
		{
			// In frame already; don't mess with it.
		}
	}

	as_value* as_environment::get_register(int reg)
	{
		as_value* val = local_register_ptr(reg);
		IF_VERBOSE_ACTION(log_msg("-------------- get_register(%d): %s at 0x%X\n", 
			reg, val->to_string(), val->to_object()));
		return val;
	}

	void as_environment::set_register(int reg, const as_value& val)
	{
		IF_VERBOSE_ACTION(log_msg("-------------- set_register(%d): %s at 0x%X\n",
			reg, val.to_string(), val.to_object()));
		*local_register_ptr(reg) = val;
	}

	as_value*	as_environment::local_register_ptr(int reg)
	// Return a pointer to the specified local register.
	// Local registers are numbered starting with 1.
	//
	// Return value will never be NULL.  If reg is out of bounds,
	// we log an error, but still return a valid pointer (to
	// global reg[0]).  So the behavior is a bit undefined, but
	// not dangerous.
	{
		// We index the registers from the end of the register
		// array, so we don't have to keep base/frame
		// pointers.

		assert(reg >=0 && reg <= m_local_register.size());

		// Flash 8 can have zero register (-1 for zero)
		return &m_local_register[m_local_register.size() - reg - 1];
	}


	int	as_environment::find_local(const tu_string& varname, bool ignore_barrier) const
	// Search the active frame for the named var; return its index
	// in the m_local_frames stack if found.
	// 
	// Otherwise return -1.
	// set_local should use "ignore_barrier=false"
	// get_variable should use "ignore_barrier=true"
	{
		// Linear search sucks, but is probably fine for
		// typical use of local vars in script.  There could
		// be pathological breakdowns if a function has tons
		// of locals though.  The ActionScript bytecode does
		// not help us much by using strings to index locals.

		for (int i = m_local_frames.size() - 1; i >= 0; i--)
		{
			const frame_slot&	slot = m_local_frames[i];
			if (slot.m_name.length() == 0 && ignore_barrier == false)
			{
				// End of local frame; stop looking.
				return -1;
			}
			else
			if (slot.m_name == varname)
			{
				// Found it.
				return i;
			}
		}
		return -1;
	}


	// Should be highly optimized !!!
	bool	as_environment::parse_path(const tu_string& var_path, tu_string* path, tu_string* var)
	// See if the given variable name is actually a sprite path
	// followed by a variable name.  These come in the format:
	//
	// 	/path/to/some/sprite/:varname
	//
	// (or same thing, without the last '/')
	//
	// or
	//	path.to.some.var
	//
	// If that's the format, puts the path part (no colon or
	// trailing slash) in *path, and the varname part (no color)
	// in *var and returns true.
	//
	// If no colon, returns false and leaves *path & *var alone.
	{
		// Search for colon.
		const char* colon = strrchr(var_path.c_str(), ':');
		if (colon)
		{
			// Make the subparts.
			*var = colon + 1;

			// delete prev '/' if it is not first character
			if (colon > var_path.c_str() + 1 && *(colon - 1) == '/')
			{
				colon--;
			}
			*path = var_path;
			path->resize(int(colon - var_path.c_str()));
			return true;
		}
		else
		{
			// Is there a dot?  Find the last one, if any.
			colon = strrchr(var_path.c_str(), '.');
			if (colon)
			{
				// Make the subparts.
				*var = colon + 1;
				*path = var_path;
				path->resize(int(colon - var_path.c_str()));
				return true;
			}
		}
		return false;
	}

	as_object*	as_environment::find_target(const as_value& target) const
	{
		if (m_target != NULL)
		{
			return m_target->find_target(target);
		}
		return NULL;
	}

	bool	as_environment::set_member(const tu_stringi& name, const as_value& val)
	{
		if (m_target != NULL)
		{
			return m_target->set_member(name, val);
		}
		return false;
	}

	bool	as_environment::get_member(const tu_stringi& name, as_value* val)
	{
		if (m_target != NULL)
		{
			return m_target->get_member(name, val);
		}
		return false;
	}

	void as_environment::clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr)
	{
		// target
		if (m_target == this_ptr)
		{
			m_target = NULL;
		}

		// local vars
		for (int i = 0, n = m_local_frames.size(); i < n; i++)
		{
			as_object* obj = m_local_frames[i].m_value.to_object();
			if (obj)
			{
				if (obj == this_ptr)
				{
					m_local_frames[i].m_value.set_undefined();
				}
				else
				{
					obj->clear_refs(visited_objects, this_ptr);
				}
			}
		}

		// stack
		for (int i = 0, n = m_stack.size(); i < n; i++)
		{
			as_object* obj = m_stack[i].to_object();
			if (obj)
			{
				if (obj == this_ptr)
				{
					m_stack[i].set_undefined();
				}
				else
				{
					obj->clear_refs(visited_objects, this_ptr);
				}
			}
		}

		// global register
		for (int i = 0, n = GLOBAL_REGISTER_COUNT; i < n; i++)
		{
			as_object* obj = m_global_register[i].to_object();
			if (obj)
			{
				if (obj == this_ptr)
				{
					m_global_register[i].set_undefined();
				}
				else
				{
					obj->clear_refs(visited_objects, this_ptr);
				}
			}
		}

		// local register
		for (int i = 0, n = m_local_register.size(); i < n; i++)
		{
			as_object* obj = m_local_register[i].to_object();
			if (obj)
			{
				if (obj == this_ptr)
				{
					m_local_register[i].set_undefined();
				}
				else
				{
					obj->clear_refs(visited_objects, this_ptr);
				}
			}
		}
	}

	player* as_environment::get_player() const
	{
		return m_player.get_ptr();
	}

	root* as_environment::get_root() const
	{
		return m_player->get_root();
	}

	//  fn_call 

	player* fn_call::get_player() const
	{
		assert(env);
		return env->get_player();
	}

	root* fn_call::get_root() const
	{
		assert(env);
		return env->get_player()->get_root();
	}

}
