// gameswf_avm2.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// AVM2 implementation

#include "gameswf_avm2.h"
#include "gameswf_stream.h"
#include "gameswf_log.h"
#include "gameswf_abc.h"
#include "gameswf_disasm.h"
#include "gameswf_character.h"
#include "gameswf_jit.h"

namespace gameswf
{

	as_3_function::as_3_function(abc_def* abc, int method, player* player) :
		as_function(player),
		m_method(method),
		m_abc(abc)
	{
		m_this_ptr = this;

		// any function MUST have prototype
		builtin_member("prototype", new as_object(player));
	}

	as_3_function::~as_3_function()
	{
	}

	void	as_3_function::operator()(const fn_call& fn)
	// dispatch
	{
		assert(fn.env);

		// try to use caller environment
		// if the caller object has own environment then we use its environment
		as_environment* env = fn.env;
		if (fn.this_ptr)
		{
			if (fn.this_ptr->get_environment())
			{
				env = fn.this_ptr->get_environment();
			}
		}

		// set 'this'
		as_object* this_ptr = env->get_target();
		if (fn.this_ptr)
		{
			this_ptr = fn.this_ptr;
			if (this_ptr->m_this_ptr != NULL)
			{
				this_ptr = this_ptr->m_this_ptr.get_ptr();
			}
		}

		// Create local registers.
		array<as_value>	local_register;
		local_register.resize(m_local_count + 1);

		//	Register 0 holds the “this” object. This value is never null.
		assert(this_ptr);
		local_register[0] = this_ptr;

		// Create stack.
		array<as_value>	stack;

		// Create scope stack.
		array<as_value>	scope;

		// push '_global' into scope
		scope.push_back(get_global());

		// get flash package
		as_value val;
		get_global()->get_member("flash", &val);
		as_object* flash = val.to_object();
		assert(flash);

		// push 'Events'  into scope
		flash->get_member("Events", &val);
		scope.push_back(val);

#ifdef __GAMESWF_ENABLE_JIT__

		if( !m_compiled_code.is_valid() )
		{
			compile();
			m_compiled_code.initialize();
		}

#endif

		if( m_compiled_code.is_valid() )
		{
			m_compiled_code.call< array<as_value>&, array<as_value>&, array<as_value>&, as_value* >
				(local_register, stack, scope, fn.result );
		}
		else
		{
			// Execute the actions.
			IF_VERBOSE_ACTION(log_msg("\nEX: call method #%d\n", m_method));
			execute(local_register, stack, scope, fn.result);
			IF_VERBOSE_ACTION(log_msg("EX: ended. stack_size=%d\n", stack.size()));
		}

	}

	// interperate action script bytecode
	void	as_3_function::execute(array<as_value>& lregister,
		array<as_value>& stack,
		array<as_value>& scope,
		as_value* result)
	{
		int ip = 0;
		do
		{
			Uint8 opcode = m_code[ip++];
			switch (opcode)
			{
				case 0x24:	// pushbyte
				{
					int byte_value;
					ip += read_vu30(byte_value, &m_code[ip]);
					stack.push_back(byte_value);

					IF_VERBOSE_ACTION(log_msg("EX: pushbyte\t %d\n", byte_value));

					break;
				}

				case 0x2D:	// pushint
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					int val = m_abc->get_integer(index);
					stack.push_back(val);

					IF_VERBOSE_ACTION(log_msg("EX: pushint\t %d\n", val));

					break;
				}

				case 0x2C:	// pushstring
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* val = m_abc->get_string(index);
					stack.push_back(val);

					IF_VERBOSE_ACTION(log_msg("EX: pushstring\t '%s'\n", val));

					break;
				}

				case 0x2F:	// pushdouble
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					double val = m_abc->get_double(index);
					stack.push_back(val);

					IF_VERBOSE_ACTION(log_msg("EX: pushdouble\t %f\n", val));

					break;
				}

				case 0x30:	// pushscope
				{
					as_value& val = stack.back();
					scope.push_back(val);

					IF_VERBOSE_ACTION(log_msg("EX: pushscope\t %s\n", val.to_xstring()));

					stack.resize(stack.size() - 1); 
					break;
				}

				case 0x47:	// returnvoid
				{
					IF_VERBOSE_ACTION(log_msg("EX: returnvoid\t\n"));
					result->set_undefined();
					break;
				}

				case 0x49:	// constructsuper
				{
					// stack: object, arg1, arg2, ..., argn
					int arg_count;
					ip += read_vu30(arg_count, &m_code[ip]);

					as_object* obj = stack.back().to_object();
					stack.resize(stack.size() - 1);
					for (int i = 0; i < arg_count; i++)
					{
						as_value& val = stack.back();
						stack.resize(stack.size() - 1);
					}

					//TODO: construct super of obj

					IF_VERBOSE_ACTION(log_msg("EX: constructsuper\t 0x%p(args:%d)\n", obj, arg_count));

					break;
				}

				case 0x4F:	// callpropvoid, Call a property, discarding the return value.
				// Stack: …, obj, [ns], [name], arg1,...,argn => …
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					int arg_count;
					ip += read_vu30(arg_count, &m_code[ip]);

					as_environment env(get_player());
					for (int i = 0; i < arg_count; i++)
					{
						as_value& val = stack[stack.size() - 1 - i];
						env.push(val);
					}
					stack.resize(stack.size() - arg_count);

					as_object* obj = stack.back().to_object();
					stack.resize(stack.size() - 1);

					as_value func;
					if (obj && obj->get_member(name, &func))
					{
						call_method(func, &env, obj,	arg_count, env.get_top_index());
					}

					IF_VERBOSE_ACTION(log_msg("EX: callpropvoid\t 0x%p.%s(args:%d)\n", obj, name, arg_count));

					break;
				}

				case 0x5D:	// findpropstrict
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					// search property in scope
					as_object* obj = NULL;
					for (int i = scope.size() - 1; i >= 0; i--)
					{
						as_value val;
						if (scope[i].get_member(name, &val))
						{
							obj = scope[i].to_object();
							break;
						}
					}

					IF_VERBOSE_ACTION(log_msg("EX: findpropstrict\t %s, obj=0x%p\n", name, obj));

					stack.push_back(obj);
					break;
				}

				case 0x5E:	// findproperty, Search the scope stack for a property
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					as_object* obj = NULL;
					for (int i = scope.size() - 1; i >= 0; i--)
					{
						as_value val;
						if (scope[i].get_member(name, &val))
						{
							obj = scope[i].to_object();
							break;
						}
					}

					IF_VERBOSE_ACTION(log_msg("EX: findproperty\t %s, obj=0x%p\n", name, obj));

					stack.push_back(obj);
					break;

				}

				case 0x60:	// getlex, Find and get a property.
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					// search property in scope
					as_value val;
					for (int i = scope.size() - 1; i >= 0; i--)
					{
						if (scope[i].get_member(name, &val))
						{
							break;
						}
					}

					IF_VERBOSE_ACTION(log_msg("EX: getlex\t %s, value=%s\n", name, val.to_xstring()));

					stack.push_back(val);
					break;
				}

				case 0x66:	// getproperty
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					as_object* obj = stack.back().to_object();
					if (obj)
					{
						obj->get_member(name, &stack.back());
					}
					else
					{
						stack.back().set_undefined();
					}

					IF_VERBOSE_ACTION(log_msg("EX: getproperty\t %s, value=%s\n", name, stack.back().to_xstring()));

					break;
				}

				case 0x68:	// initproperty, Initialize a property.
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					as_value& val = stack[stack.size() - 1];
					as_object* obj = stack[stack.size() - 2].to_object();
					if (obj)
					{
						obj->set_member(name, val);
					}

					IF_VERBOSE_ACTION(log_msg("EX: initproperty\t 0x%p.%s=%s\n", obj, name, val.to_xstring()));

					stack.resize(stack.size() - 2);
					break;
				}

				case 0xD0:	// getlocal_0
				case 0xD1:	// getlocal_1
				case 0xD2:	// getlocal_2
				case 0xD3:	// getlocal_3
				{
					as_value& val = lregister[opcode & 0x03];
					stack.push_back(val);
					IF_VERBOSE_ACTION(log_msg("EX: getlocal_%d\t %s\n", opcode & 0x03, val.to_xstring()));
					break;
				}

				default:
					log_msg("TODO opcode 0x%02X\n", opcode);
					return;
			}

		}
		while (ip < m_code.size());
	}

	template< typename T >
	void stack_push_back_value( array<as_value> & stack, T value )
	{
		stack.push_back( value );
	}

	template< typename T >
	void stack_push_back_ref( array<as_value> & stack, T & value )
	{
		stack.push_back( value );
	}

	typedef void ( *stack_int_function )( array<as_value> & stack, int value );
	typedef void ( *stack_charp_function )( array<as_value> & stack, char * value );
	typedef void (array<as_value>::*push_value_function )( as_value & );
	typedef as_value & (array<as_value>::*index_function)(int);

    
	void as_3_function::compile()
	{
#ifdef __GAMESWF_ENABLE_JIT__
		int ip = 0;
		jit_prologue( m_compiled_code );
		do
		{
			Uint8 opcode = m_code[ip++];
			switch (opcode)
			{
				case 0x24:	// pushbyte
				{
					int byte_value;
					ip += read_vu30(byte_value, &m_code[ip]);

					//stack.push_back(byte_value);
					jit_pushargi( m_compiled_code, byte_value );
					jit_load( m_compiled_code, jit_eax, jit_getarg( 1 ) );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_call( m_compiled_code, stack_int_function(stack_push_back_value) );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushbyte\t %d\n", byte_value));

					break;
				}

				case 0x2D:	// pushint
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					int val = m_abc->get_integer(index);

					//stack.push_back(val);
					jit_pushargi( m_compiled_code, val );
					jit_load( m_compiled_code, jit_eax, jit_getarg( 1 ) );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_call( m_compiled_code, stack_int_function(stack_push_back_value) );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushint\t %d\n", val));

					break;
				}

				case 0x2C:	// pushstring
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* val = m_abc->get_string(index);

					//stack.push_back(val);
					jit_pushargi( m_compiled_code, val );
					jit_load( m_compiled_code, jit_eax, jit_getarg( 1 ) );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_call( m_compiled_code, stack_charp_function( &stack_push_back_value ) );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushstring\t '%s'\n", val));

					break;
				}

				case 0x2F:	// pushdouble
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     double val = m_abc->get_double(index);
					//                     stack.push_back(val);
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: pushdouble\t %f\n", val));

					break;
				}

				case 0x30:	// pushscope
				{
					//as_value& val = stack.back();
					typedef as_value& ( array<as_value>::*back_function )();
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					jit_this_call( m_compiled_code, (back_function)&array<as_value>::back );

					//scope.push_back(val);
					jit_push( m_compiled_code, jit_result );
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 2 ) );
					jit_this_call( m_compiled_code, (push_value_function) &array<as_value>::push_back );
					jit_popargs( m_compiled_code, 1 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushscope\t %s\n", val.to_xstring()));
					//TODO: Ecx is caller-saved, copy it to esi or edi before calling to prevent reload from memory
					//stack.resize(stack.size() - 1); 
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					jit_this_call( m_compiled_code, &array<as_value>::size );
					jit_subi( m_compiled_code, jit_eax, 1 );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					jit_this_call( m_compiled_code,  &array<as_value>::resize );
					jit_popargs( m_compiled_code, 1 );
					break;
				}

				case 0x47:	// returnvoid
				{
					//IF_VERBOSE_ACTION(log_msg("EX: returnvoid\t\n"));

					//result->set_undefined();
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 3 ) );
					jit_this_call( m_compiled_code, &as_value::set_undefined );

					break;
				}

				case 0x49:	// constructsuper
				{
					// stack: object, arg1, arg2, ..., argn
					//                     int arg_count;
					//                     ip += read_vu30(arg_count, &m_code[ip]);
					// 
					//                     as_object* obj = stack.back().to_object();
					//                     stack.resize(stack.size() - 1);
					//                     for (int i = 0; i < arg_count; i++)
					//                     {
					//                         as_value& val = stack.back();
					//                         stack.resize(stack.size() - 1);
					//                     }
					// 
					//                     //TODO: construct super of obj
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: constructsuper\t 0x%p(args:%d)\n", obj, arg_count));

					break;
				}

				case 0x4F:	// callpropvoid, Call a property, discarding the return value.
				// Stack: …, obj, [ns], [name], arg1,...,argn => …
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     const char* name = m_abc->get_multiname(index);
					// 
					//                     int arg_count;
					//                     ip += read_vu30(arg_count, &m_code[ip]);
					// 
					//                     as_environment env(get_player());
					//                     for (int i = 0; i < arg_count; i++)
					//                     {
					//                         as_value& val = stack[stack.size() - 1 - i];
					//                         env.push(val);
					//                     }
					//                     stack.resize(stack.size() - arg_count);
					// 
					//                     as_object* obj = stack.back().to_object();
					//                     stack.resize(stack.size() - 1);
					// 
					//                     as_value func;
					//                     if (obj && obj->get_member(name, &func))
					//                     {
					//                         call_method(func, &env, obj,	arg_count, env.get_top_index());
					//                     }
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: callpropvoid\t 0x%p.%s(args:%d)\n", obj, name, arg_count));

					break;
				}

				case 0x5D:	// findpropstrict
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     const char* name = m_abc->get_multiname(index);
					// 
					//                     // search property in scope
					//                     as_object* obj = NULL;
					//                     for (int i = scope.size() - 1; i >= 0; i--)
					//                     {
					//                         as_value val;
					//                         if (scope[i].get_member(name, &val))
					//                         {
					//                             obj = scope[i].to_object();
					//                             break;
					//                         }
					//                     }
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: findpropstrict\t %s, obj=0x%p\n", name, obj));
					// 
					//                     stack.push_back(obj);
					break;
				}

				case 0x5E:	// findproperty, Search the scope stack for a property
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     const char* name = m_abc->get_multiname(index);
					// 
					//                     as_object* obj = NULL;
					//                     for (int i = scope.size() - 1; i >= 0; i--)
					//                     {
					//                         as_value val;
					//                         if (scope[i].get_member(name, &val))
					//                         {
					//                             obj = scope[i].to_object();
					//                             break;
					//                         }
					//                     }
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: findproperty\t %s, obj=0x%p\n", name, obj));
					// 
					//                     stack.push_back(obj);
					break;

				}

				case 0x60:	// getlex, Find and get a property.
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     const char* name = m_abc->get_multiname(index);
					// 
					//                     // search property in scope
					//                     as_value val;
					//                     for (int i = scope.size() - 1; i >= 0; i--)
					//                     {
					//                         if (scope[i].get_member(name, &val))
					//                         {
					//                             break;
					//                         }
					//                     }
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: getlex\t %s, value=%s\n", name, val.to_xstring()));
					// 
					//                     stack.push_back(val);
					break;
				}

				case 0x66:	// getproperty
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     const char* name = m_abc->get_multiname(index);
					// 
					//                     as_object* obj = stack.back().to_object();
					//                     if (obj)
					//                     {
					//                         obj->get_member(name, &stack.back());
					//                     }
					//                     else
					//                     {
					//                         stack.back().set_undefined();
					//                     }
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: getproperty\t %s, value=%s\n", name, stack.back().to_xstring()));
					// 
					break;
				}

				case 0x68:	// initproperty, Initialize a property.
				{
					//                     int index;
					//                     ip += read_vu30(index, &m_code[ip]);
					//                     const char* name = m_abc->get_multiname(index);
					// 
					//                     as_value& val = stack[stack.size() - 1];
					//                     as_object* obj = stack[stack.size() - 2].to_object();
					//                     if (obj)
					//                     {
					//                         obj->set_member(name, val);
					//                     }
					// 
					//                     IF_VERBOSE_ACTION(log_msg("EX: initproperty\t 0x%p.%s=%s\n", obj, name, val.to_xstring()));
					// 
					//                     stack.resize(stack.size() - 2);
					break;
				}

				case 0xD0:	// getlocal_0
				case 0xD1:	// getlocal_1
				case 0xD2:	// getlocal_2
				case 0xD3:	// getlocal_3
				{
					//as_value& val = lregister[opcode & 0x03];
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 0 ) );
					jit_pushi( m_compiled_code, opcode & 0x03 );
					jit_this_call( m_compiled_code, (index_function)&array<as_value>::operator[] );
					jit_popargs( m_compiled_code, 1 );

					//stack.push_back(val);
					jit_push( m_compiled_code, jit_result );
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					jit_this_call( m_compiled_code, (push_value_function) &array<as_value>::push_back );
					jit_popargs( m_compiled_code, 1 );

					//IF_VERBOSE_ACTION(log_msg("EX: getlocal_%d\t %s\n", opcode & 0x03, val.to_xstring()));
					break;
				}

				default:
					log_msg("TODO opcode 0x%02X\n", opcode);
					break;
			}

		}
		while (ip < m_code.size());

		jit_return( m_compiled_code );
		m_compiled_code.initialize();
#else
		assert( false );
#endif
	}

	void as_3_function::read(stream* in)
	// read method_info
	{
		int param_count = in->read_vu30();

		// The return_type field is an index into the multiname
		m_return_type = in->read_vu30();

		m_param_type.resize(param_count);
		for (int i = 0; i < param_count; i++)
		{
			m_param_type[i] = in->read_vu30();
		}

		m_name = in->read_vu30();
		m_flags = in->read_u8();

		if (m_flags & HAS_OPTIONAL)
		{
			int option_count = in->read_vu30();
			m_options.resize(option_count);

			for (int o = 0; o < option_count; ++o)
			{
				m_options[o].m_value = in->read_vu30();
				m_options[o].m_kind = in->read_u8();
			}
		}

		if (m_flags & HAS_PARAM_NAMES)
		{
			assert(0 && "todo");
			//		param_info param_names
		}

		IF_VERBOSE_PARSE(log_msg("method_info: name='%s', type='%s', params=%d\n",
			m_abc->get_string(m_name), m_abc->get_multiname(m_return_type), m_param_type.size()));
	}

	void as_3_function::read_body(stream* in)
	// read body_info
	{
		IF_VERBOSE_PARSE(log_msg("body_info[%d]\n", m_method));

		m_max_stack = in->read_vu30();
		m_local_count = in->read_vu30();
		m_init_scope_depth = in->read_vu30();
		m_max_scope_depth = in->read_vu30();

		int i, n;
		n = in->read_vu30();	// code_length
		m_code.resize(n);
		for (i = 0; i < n; i++)
		{
			m_code[i] = in->read_u8();
		}

		n = in->read_vu30();	// exception_count
		m_exception.resize(n);
		for (i = 0; i < n; i++)
		{
			except_info* e = new except_info();
			e->read(in, m_abc.get_ptr());
			m_exception[i] = e;
		}

		n = in->read_vu30();	// trait_count
		m_trait.resize(n);
		for (int i = 0; i < n; i++)
		{
			traits_info* trait = new traits_info();
			trait->read(in, m_abc.get_ptr());
			m_trait[i] = trait;
		}

		IF_VERBOSE_PARSE(log_msg("method	%i\n", m_method));
		IF_VERBOSE_PARSE(log_disasm_avm2(m_code, m_abc.get_ptr()));

	}

}
