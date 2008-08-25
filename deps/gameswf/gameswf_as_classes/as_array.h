// array.h	-- Thatcher Ulrich <tu@tulrich.com> 2003, Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script Array implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_ARRAY_H
#define GAMESWF_AS_ARRAY_H

#include "gameswf_action.h"	// for as_object

namespace gameswf
{

	void	as_global_array_ctor(const fn_call& fn);

	struct as_array : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_ARRAY };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		exported_module as_array(player* player);
		exported_module virtual const char* to_string();

		exported_module void push(const as_value& val);
		exported_module void pop(as_value* val);
		exported_module void erase(const tu_stringi& index);
		exported_module int size();

		tu_string m_string_value;
	};

}	// end namespace gameswf


#endif // GAMESWF_AS_ARRAY_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
