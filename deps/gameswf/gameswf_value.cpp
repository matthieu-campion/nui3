// gameswf_value.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript value type.

#include "gameswf_value.h"
#include "gameswf.h"
#include "gameswf_root.h"
#include "gameswf_action.h"
#include "gameswf_character.h"
#include "gameswf_function.h"
#include "gameswf_movie_def.h"
#include "gameswf_as_classes/as_number.h"
#include "gameswf_as_classes/as_boolean.h"
#include "gameswf_as_classes/as_string.h"
#include <float.h>

namespace gameswf
{

	as_value::as_value(as_object* obj) :
		m_type(OBJECT),
		m_object(obj),
		m_flags(0)
	{
		if (m_object)
		{
			m_object->add_ref();
		}
	}


	as_value::as_value(as_s_function* func)	:
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_as_object(func);
	}

	as_value::as_value(const as_value& getter, const as_value& setter) :
		m_property_target(NULL),
		m_type(PROPERTY),
		m_flags(0)
	{
		m_property = new as_property(getter, setter);
		m_property->add_ref();
	}

	const char*	as_value::to_string() const
	// Conversion to string.
	{
		return to_tu_string().c_str();
	}

	const char*	as_value::to_xstring() const
	// Conversion to string, for debuging
	{
		static char buf[16];
		if (m_type == OBJECT)
		{
			snprintf(buf, 16, "0x%p", m_object);
			return buf;
		}
		return to_tu_string().c_str();
	}


	const tu_stringi&	as_value::to_tu_stringi() const
	{
		return reinterpret_cast<const tu_stringi&>(to_tu_string());
	}


	const tu_string&	as_value::to_tu_string() const
	// Conversion to const tu_string&.
	{
		switch (m_type)
		{
			case STRING:
				// don't need to do anything
				break;

			case UNDEFINED:
			{
				// Behavior depends on file version.  In
				// version 7+, it's "undefined", in versions
				// 6-, it's "".
				int version = 8; //vv fixme: get_root()->get_movie_version();
				if (version <= 6)
				{
					m_string = "";
				}
				else
				{
					m_string = "undefined";
				}
				break;
			}

			case BOOLEAN:
				m_string = m_bool ? "true" : "false";
				break;

			case NUMBER:
				// @@ Moock says if value is a NAN, then result is "NaN"
				// INF goes to "Infinity"
				// -INF goes to "-Infinity"
				if (isnan(m_number))
				{
					m_string = "NaN";
				} 
				else
				{
					char buffer[50];
					snprintf(buffer, 50, "%.14g", m_number);
					m_string = buffer;
				}
				break;

			case OBJECT:
				// Moock says, "the value that results from
				// calling toString() on the object".
				//
				// The default toString() returns "[object
				// Object]" but may be customized.
				if (m_object == NULL)
				{
					m_string = "null";
				}
				else
				{
					m_string = m_object->to_string();
				}
				break;
	
			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				m_string = val.to_tu_string();
				break;
			}

			default:
				assert(0);
		}
		return m_string;
	}

	double	as_value::to_number() const
	// Conversion to double.
	{
		switch (m_type)
		{
			case STRING:
			{
				// @@ Moock says the rule here is: if the
				// string is a valid float literal, then it
				// gets converted; otherwise it is set to NaN.
				//
				// Also, "Infinity", "-Infinity", and "NaN"
				// are recognized.
				double val;
				if (! string_to_number(&val, m_string.c_str()))
				{
					// Failed conversion to Number.
					val = 0.0;	// TODO should be NaN
				}
				return val;
			}

			case NUMBER:
				return m_number;

			case BOOLEAN:
				return m_bool ? 1 : 0;

			case OBJECT:
				if (m_object)
				{
					return m_object->to_number();
				}
	 			// Evan: from my tests
				return 0;
	
			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_number();
			}

			case UNDEFINED:
			{
				return get_nan();
			}

			default:
				return 0.0;
		}
	}


	bool	as_value::to_bool() const
	// Conversion to boolean.
	{
		switch (m_type)
		{
			case STRING:
				// From Moock
//vv fixme:				if (get_root()->get_movie_version() >= 7)
				{
					return m_string.size() > 0 ? true : false;
				}

				if (m_string == "false")
				{
					return false;
				}
				else
				if (m_string == "true")
				{
					return true;
				}
				else
				{
					// @@ Moock: "true if the string can
					// be converted to a valid nonzero
					// number".
					//
					// Empty string --> false
					return to_number() != 0.0;
				}

			case OBJECT:
				if (m_object)
				{
					return m_object->to_bool();
				}
				return false;

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_bool();
			}

			case NUMBER:
				return m_number != 0;

			case BOOLEAN:
				return m_bool;

			case UNDEFINED:
				return false;

			default:
				assert(0);
		}
		return false;
	}

	
	as_object*	as_value::to_object() const
	// Return value as an object.
	{
		if (m_type == OBJECT)
		{
			// OK.
			return m_object;
		}
		return NULL;
	}

	as_function*	as_value::to_function() const
	// Return value as an function.
	{
		if (m_type == OBJECT)
		{
			// OK.
			return cast_to<as_function>(m_object);
		}
		return NULL;
	}

	void	as_value::set_as_object(as_object* obj)
	{
		if (m_type != OBJECT || m_object != obj)
		{
			drop_refs();
			m_type = OBJECT;
			m_object = obj;
			if (obj)
			{
				m_object->add_ref();
			}
		}
	}

	void	as_value::operator=(const as_value& v)
	{
		m_flags = v.m_flags;
		switch (v.m_type)
		{
			case UNDEFINED:
				m_flags = 0;
				set_undefined();
				break;
			case NUMBER:
				set_double(v.m_number);
				break;
			case BOOLEAN:
				set_bool(v.m_bool);
				break;
			case STRING:
				set_tu_string(v.m_string);
				break;
			case OBJECT:
				set_as_object(v.m_object);
				break;

			case PROPERTY:
				drop_refs(); 
				
				// is binded property ?
				if (v.m_property_target == NULL)
				{
					m_type = PROPERTY;
					m_property = v.m_property;
					m_property->add_ref();
					m_property_target = NULL;
				}
				else
				{
					v.get_property(this);
				}

				break;

			default:
				assert(0);
		}
	}

	bool	as_value::operator==(const as_value& v) const
	// Return true if operands are equal.
	{
		// types don't match
		if (m_type != PROPERTY && v.m_type != PROPERTY && m_type != v.m_type)
		{
			return false;
		}

		switch (m_type)
		{
			case UNDEFINED:
				return v.m_type == UNDEFINED;

			case STRING:
				return m_string == v.to_tu_string();

			case NUMBER:
				return m_number == v.to_number();

			case BOOLEAN:
				return m_bool == v.to_bool();

			case OBJECT:
				return m_object == v.to_object();

			case PROPERTY:
			{
				as_value prop;
				get_property(&prop);
				return prop == v;
			}

			default:
				assert(0);
				return false;
		}
	}

	
	bool	as_value::operator!=(const as_value& v) const
	// Return true if operands are not equal.
	{
		return ! (*this == v);
	}

	void	as_value::drop_refs()
	// Drop any ref counts we have; this happens prior to changing our value.
	{
		if (m_type == OBJECT)
		{
			if (m_object)
			{
				m_object->drop_ref();
				m_object = 0;
			}
		}
		else
		if (m_type == PROPERTY)
		{
			if (m_property)
			{
				m_property->drop_ref();
				m_property = NULL;
			}
			if (m_property_target)
			{
				m_property_target->drop_ref();
				m_property_target = NULL;
			}
		}
	}

	void	as_value::set_property(const as_value& val)
	{
		assert(is_property());
		m_property->set(m_property_target, val);
	}

	// get property of primitive value, like Number
	void as_value::get_property(const as_value& primitive, as_value* val) const
	{
		assert(is_property());
		m_property->get(primitive, val);
	}

	void as_value::get_property(as_value* val) const
	{
		assert(is_property());
		m_property->get(m_property_target, val);
	}

	as_property* as_value::to_property() const
	{
		if (is_property())
		{
			return m_property;
		}
		return NULL;
	}

	const as_object* as_value::get_property_target() const
	{
		if (is_property())
		{
			return m_property_target;
		}
		return NULL;
	}

	void as_value::set_property_target(as_object* target)
	// Sets the target to the given object.
	{
		assert(is_property());
		if (m_property_target)
		{
			m_property_target->drop_ref();
		}
		m_property_target = target;
		m_property_target->add_ref();
	}

	as_value::as_value(float val) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_double(val);
	}

	as_value::as_value(int val) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_double(val);
	}

	as_value::as_value(double val) :
		m_type(NUMBER),
		m_number(val),
		m_flags(0)
	{
	}

	void	as_value::set_double(double val)
	{
		drop_refs(); m_type = NUMBER; m_number = val;
	}

	as_value::as_value(bool val) :
		m_type(BOOLEAN),
		m_bool(val),
		m_flags(0)
	{
	}

	void	as_value::set_bool(bool val)
	{
		drop_refs(); m_type = BOOLEAN; m_bool = val;
	}


	bool as_value::is_function() const
	{
		if (m_type == OBJECT)
		{
			return cast_to<as_function>(m_object) ? true : false;
		}
		return false;
	}

	as_value::as_value(as_c_function_ptr func) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_as_c_function(func);
	}

	void	as_value::set_as_c_function(as_c_function_ptr func)
	{
		//vv hack, c_function object has no pointer to player instance
		set_as_object(new as_c_function(NULL, func));
	}

		
	bool as_value::is_instance_of(const as_function* constructor) const
	{
		switch (m_type)
		{
			case UNDEFINED:
				break;

			case STRING:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_string_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case NUMBER:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_number_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case BOOLEAN:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_boolean_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case OBJECT:
				if (m_object)
				{
					return m_object->is_instance_of(constructor);
				}
				break;

			case PROPERTY:
			{
				//vv fixme:	
				break;
			}

			default:
				break;

		}
		return false;
	}

	const char*	as_value::_typeof() const
	{
		switch (m_type)
		{
			case UNDEFINED:
				return "undefined";

			case STRING:
				return "string";

			case NUMBER:
				return "number";

			case BOOLEAN:
				return "boolean";

			case OBJECT:
				if (m_object)
				{
					return m_object->_typeof();
				}
				return "null";

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val._typeof();
			}

			default:
				assert(0);

		}
		return 0;
	}


	bool as_value::get_member( const tu_string& name, as_value* val)
	{
		switch (m_type)
		{
			default:
				break;

			case STRING:
			{
				return get_builtin(BUILTIN_STRING_METHOD, name, val);
			}

			case NUMBER:
			{
				return get_builtin(BUILTIN_NUMBER_METHOD, name, val);
			}

			case BOOLEAN:
			{
				return get_builtin(BUILTIN_BOOLEAN_METHOD, name, val);
			}

			case OBJECT:
			{
				if (m_object)
				{
					return m_object->get_member(name, val);
				}
			}
		}
		return false;
	}

	void	as_value::set_tu_string(const tu_string& str)
	{
		drop_refs(); m_type = STRING; m_string = str; 
	}
	
	void	as_value::set_string(const char* str)
	{
		drop_refs(); m_type = STRING; m_string = str; 
	}
	
	as_value::as_value(const char* str) :
		m_type(STRING),
		m_string(str),
		m_flags(0)
	{
	}

	as_value::as_value(const wchar_t* wstr)	:
		m_type(STRING),
		m_flags(0)
	{
		// Encode the string value as UTF-8.
		//
		// Is this dumb?  Alternatives:
		//
		// 1. store a tu_wstring instead of tu_string?
		// Bloats typical ASCII strings, needs a
		// tu_wstring type, and conversion back the
		// other way to interface with char[].
		// 
		// 2. store a tu_wstring as a union with
		// tu_string?  Extra complexity.
		//
		// 3. ??
		//
		// Storing UTF-8 seems like a pretty decent
		// way to do it.  Everything else just
		// continues to work.

    if (sizeof(wchar_t) == 2)
		  tu_string::encode_utf8_from_wchar(&m_string, (const uint16 *)wstr);
    else if (sizeof(wchar_t) == 4)
			tu_string::encode_utf8_from_wchar(&m_string, (const uint32 *)wstr);
	}

	as_value::as_value() :
		m_type(UNDEFINED),
		m_flags(0)
	{
	}

	as_value::as_value(const as_value& v) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		*this = v;
	}


	//
	//	as_property
	//

	as_property::as_property(const as_value& getter,	const as_value& setter)
	{
		m_getter = cast_to<as_function>(getter.to_object());
		m_setter = cast_to<as_function>(setter.to_object());
	}

	as_property::~as_property()
	{
	}

	void	as_property::set(as_object* target, const as_value& val)
	{
		assert(target);

		as_environment env(target->get_player());
		env.push(val);
		if (m_setter != NULL)
		{
			smart_ptr<as_object> tar = target;
			(*m_setter.get_ptr())(fn_call(NULL, tar.get_ptr(),	&env, 1, env.get_top_index()));
		}
	}

	void as_property::get(as_object* target, as_value* val) const
	{
		assert(target);

		// env is used when m_getter->m_env is NULL
		as_environment env(target->get_player());
		if (m_getter != NULL)
		{
			smart_ptr<as_object> tar = target;
			(*m_getter.get_ptr())(fn_call(val, tar.get_ptr(), &env, 0,	0));
		}
	}

	// call static method
	void as_property::get(const as_value& primitive, as_value* val) const
	{
		if (m_getter != NULL)
		{
			(*m_getter.get_ptr())(fn_call(val, primitive, NULL, 0,	0));
		}
	}

}
