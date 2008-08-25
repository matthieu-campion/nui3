// gameswf_environment.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript "environment", essentially VM state?

#ifndef GAMESWF_ENVIRONMENT_H
#define GAMESWF_ENVIRONMENT_H

#include "gameswf.h"
#include "gameswf_value.h"

namespace gameswf
{

	#define GLOBAL_REGISTER_COUNT 4

	struct character;
	struct sprite_instance;
	struct as_object;

	tu_string get_full_url(const tu_string& workdir, const char* url);

	//
	// with_stack_entry
	//
	// The "with" stack is for Pascal-like with-scoping.

	struct with_stack_entry
	{
		smart_ptr<as_object>	m_object;
		int	m_block_end_pc;
		
		with_stack_entry()
			:
			m_object(NULL),
			m_block_end_pc(0)
		{
		}

		with_stack_entry(as_object* obj, int end)
			:
			m_object(obj),
			m_block_end_pc(end)
		{
		}
	};

	struct as_environment
	{
		array<as_value>	m_stack;
		as_value	m_global_register[GLOBAL_REGISTER_COUNT];
		array<as_value>	m_local_register;	// function2 uses this
		smart_ptr<as_object>	m_target;

		// For local vars.  Use empty names to separate frames.
		struct frame_slot
		{
			tu_string	m_name;
			as_value	m_value;

			frame_slot() {}
			frame_slot(const tu_string& name, const as_value& val) : m_name(name), m_value(val) {}
		};
		array<frame_slot>	m_local_frames;

		weak_ptr<player> m_player;

		as_environment(player* player) :
			m_player(player)
		{
		}

//		~as_environment() {}
		bool	set_member(const tu_stringi& name, const as_value& val);
		bool	get_member(const tu_stringi& name, as_value* val);


		character*	get_target() const;
		void set_target(character* target);
		void set_target(as_value& target, character* original_target);

		// stack access/manipulation
		// @@ TODO do more checking on these

		// changes size of stack
		template<class T>
		void	push(T val) 
		{
			m_stack.push_back(as_value(val)); 
//			printf("push: size of stack = %d\n", m_stack.size());
		}

		// changes size of stack
		as_value	pop()
		{
			as_value result = m_stack.back();
			m_stack.pop_back(); 
//			printf("pop: size of stack = %d\n", m_stack.size());
			return result; 
		}

		// changes size of stack
		void	drop(int count)
		{
			m_stack.resize(m_stack.size() - count); 
//			printf("pop: size of stack = %d\n", m_stack.size());
		}

		as_value&	top(int dist) { return m_stack[m_stack.size() - 1 - dist]; }
		as_value&	bottom(int index) { return m_stack[index]; }

		int	get_top_index() const { return m_stack.size() - 1; }

		as_value	get_variable(const tu_string& varname, const array<with_stack_entry>& with_stack) const;
		// no path stuff:
		as_value	get_variable_raw(const tu_string& varname, const array<with_stack_entry>& with_stack) const;

		void	set_variable(const tu_string& path, const as_value& val, const array<with_stack_entry>& with_stack);
		// no path stuff:
		void	set_variable_raw(const tu_string& path, const as_value& val, const array<with_stack_entry>& with_stack);

		void	set_local(const tu_string& varname, const as_value& val);
		void	add_local(const tu_string& varname, const as_value& val);	// when you know it doesn't exist.
		void	declare_local(const tu_string& varname);	// Declare varname; undefined unless it already exists.

		// Parameter/local stack frame management.
		int	get_local_frame_top() const { return m_local_frames.size(); }
		void	set_local_frame_top(int t) { assert(t <= m_local_frames.size()); m_local_frames.resize(t); }
		void	add_frame_barrier() { m_local_frames.push_back(frame_slot()); }

		// Local registers.
		void	add_local_registers(int register_count)
		{
			// Flash 8 can have zero register (+1 for zero)
			m_local_register.resize(m_local_register.size() + register_count + 1);
		}
		void	drop_local_registers(int register_count)
		{
			// Flash 8 can have zero register (-1 for zero)
			m_local_register.resize(m_local_register.size() - register_count - 1);
		}

		as_value* get_register(int reg);
		void set_register(int reg, const as_value& val);

		// may be used in outside of class instance
		static bool	parse_path(const tu_string& var_path, tu_string* path, tu_string* var);

		// Internal.
		int	find_local(const tu_string& varname, bool ignore_barrier) const;
		character* load_file(const char* url, const as_value& target);
		as_object*	find_target(const as_value& target) const;
		void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr);
		player* get_player() const;
		root* get_root() const;

		private:

		as_value*	local_register_ptr(int reg);

	};


	// Parameters/environment for C functions callable from ActionScript.
	struct fn_call
	{
		as_value* result;
		as_object* this_ptr;
		const as_value& this_value;	// Number or String or Boolean value
		as_environment* env;
		int nargs;
		int first_arg_bottom_index;

		fn_call(as_value* res_in, const as_value& this_in, as_environment* env_in, int nargs_in, int first_in) :
			result(res_in),
			this_value(this_in),
			env(env_in),
			nargs(nargs_in),
			first_arg_bottom_index(first_in)
		{
			this_ptr = this_in.to_object();
		}

		as_value& arg(int n) const
		// Access a particular argument.
		{
			assert(n < nargs);
			return env->bottom(first_arg_bottom_index - n);
		}

		exported_module player* get_player() const;
		exported_module root* get_root() const;
	};

}

#endif
