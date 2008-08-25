// gameswf_as_sprite.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation for SWF player.

// Useful links:
//
// http://sswf.sourceforge.net/SWFalexref.html
// http://www.openswf.org

#include "gameswf_action.h"	// for as_object
#include "gameswf_sprite.h"
#include "gameswf_as_classes/as_number.h"

namespace gameswf
{

	//
	// sprite built-in ActionScript methods
	//

	void	as_global_movieclip_ctor(const fn_call& fn)
	// Constructor for ActionScript class XMLSocket
	{
		root* rm = fn.get_player()->get_root();
		sprite_definition* empty_sprite_def = new sprite_definition(fn.get_player(), NULL);
		character* ch = new sprite_instance(fn.get_player(), empty_sprite_def,
			rm, rm->get_root_movie(), 0);
		fn.result->set_as_object(ch);
	}

	sprite_instance* sprite_getptr(const fn_call& fn)
	{
		sprite_instance* sprite = cast_to<sprite_instance>(fn.this_ptr);
		if (sprite == NULL)
		{
			sprite = cast_to<sprite_instance>(fn.env->get_target());
		}
		assert(sprite);
		return sprite;
	}

	void	sprite_hit_test(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		fn.result->set_bool(false);
		if (fn.nargs == 1) 
		{
			// Evaluates the bounding boxes of the target and specified instance,
			// and returns true if they overlap or intersect at any point.
			character* ch = cast_to<character>(fn.env->find_target(fn.arg(0)));
			if (ch)
			{
				fn.result->set_bool(sprite->hit_test(ch));
				return;
			}
			log_error("hitTest: can't find target\n");
			return;
		}
		log_error("hitTest(x,y,flag) is't implemented yet\n");
	}

	void	sprite_start_drag(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		fn.get_root()->start_drag(sprite);
	}

	void	sprite_stop_drag(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);

		if (fn.get_root()->m_drag_state.m_character == sprite )
		{
			fn.get_root()->stop_drag();
		}
	}

	void	sprite_play(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		sprite->set_play_state(character::PLAY);
	}

	void	sprite_stop(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		sprite->set_play_state(character::STOP);
	}

	void	sprite_goto_and_play(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		if (fn.nargs < 1)
		{
			log_error("error: sprite_goto_and_play needs one arg\n");
			return;
		}

		// gotoAndPlay(NaN) will be ignored
		if (fn.arg(0).is_string() || fn.arg(0).is_number())
		{
			sprite->goto_frame(fn.arg(0).to_tu_string());
			sprite->set_play_state(character::PLAY);
		}
	}

	void	sprite_goto_and_stop(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		if (fn.nargs < 1)
		{
			log_error("error: sprite_goto_and_stop needs one arg\n");
			return;
		}

		// gotoAndStop(NaN) will be ignored
		if (fn.arg(0).is_string() || fn.arg(0).is_number())
		{
			sprite->goto_frame(fn.arg(0).to_tu_string());
			sprite->set_play_state(character::STOP);
		}
	}

	void	sprite_next_frame(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		int frame_count = sprite->get_frame_count();
		int current_frame = sprite->get_current_frame();
		if (current_frame < frame_count)
		{
			sprite->goto_frame(current_frame + 1);
		}
		sprite->set_play_state(character::STOP);
	}

	void	sprite_prev_frame(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		int current_frame = sprite->get_current_frame();
		if (current_frame > 0)
		{
			sprite->goto_frame(current_frame - 1);
		}
		sprite->set_play_state(character::STOP);
	}

	void	sprite_get_bytes_loaded(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		fn.result->set_int(sprite->get_loaded_bytes());
	}

	void	sprite_get_bytes_total(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		fn.result->set_int(sprite->get_file_bytes());
	}

	//swapDepths(target:Object) : Void
	//swapDepths(depth:Number) : Void
	void sprite_swap_depths(const fn_call& fn) 
	{ 
		sprite_instance* sprite = sprite_getptr(fn);
		if (fn.nargs != 1) 
		{ 
			log_error("swapDepths needs one arg\n"); 
			return; 
		} 

		sprite_instance* target = NULL;

		if (fn.arg(0).is_number())
		{ 
			int target_depth = fn.arg(0).to_int();
			target_depth += ADJUST_DEPTH_VALUE;
			sprite_instance* parent = cast_to<sprite_instance>(sprite->get_parent());
			if (parent == NULL)
			{
				log_error("can't do _root.swapDepths\n"); 
				return; 
			}

			character* ch = parent->m_display_list.get_character_at_depth(target_depth);
			if (ch)
			{
				target = cast_to<sprite_instance>(ch);
			}
			else
			{
				// no character at depth
				parent->m_display_list.change_character_depth(sprite, target_depth);
				return;
			}
		}
		else
		if (fn.arg(0).is_object()) 
		{ 
			target = cast_to<sprite_instance>(fn.arg(0).to_object());
		}
		else 
		{ 
			log_error("swapDepths has received invalid arg\n"); 
			return; 
		} 

		if (sprite == NULL || target == NULL) 
		{ 
			log_error("It is impossible to swap NULL character\n"); 
			return; 
		} 

		if (sprite->get_parent() == target->get_parent() && sprite->get_parent() != NULL) 
		{ 
			int target_depth = target->get_depth(); 
			target->set_depth(sprite->get_depth()); 
			sprite->set_depth(target_depth); 
			sprite_instance* parent = cast_to<sprite_instance>(sprite->get_parent());
			parent->m_display_list.swap_characters(sprite, target); 
		} 
		else 
		{ 
			log_error("MovieClips should have the same parent\n"); 
		} 
	} 

	//duplicateMovieClip(name:String, depth:Number, [initObject:Object]) : MovieClip 
	void sprite_duplicate_movieclip(const fn_call& fn) 
	{ 
		sprite_instance* sprite = sprite_getptr(fn);
		if (fn.nargs >= 2) 
		{ 
			character* ch = sprite->clone_display_object(
				fn.arg(0).to_tu_string(), 
				fn.arg(1).to_int() + ADJUST_DEPTH_VALUE);

			if (fn.nargs == 3) 
			{ 
				as_object* init_object = fn.arg(2).to_object();
				if (init_object)
				{
					init_object->copy_to(ch);
				}
			} 
			fn.result->set_as_object(ch); 
			return;
		} 
		log_error("duplicateMovieClip needs 2 or 3 args\n"); 
	} 

	void sprite_get_depth(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		fn.result->set_int(sprite->get_depth() - ADJUST_DEPTH_VALUE);
	}

	//createEmptyMovieClip(name:String, depth:Number) : MovieClip
	void sprite_create_empty_movieclip(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		if (fn.nargs != 2)
		{
			log_error("createEmptyMovieClip needs 2 args\n");
			return;
		}

		character* ch = sprite->add_empty_movieclip(fn.arg(0).to_string(),
			fn.arg(1).to_int() + ADJUST_DEPTH_VALUE);
		fn.result->set_as_object(ch);
	}

	// removeMovieClip() : Void 
	void sprite_remove_movieclip(const fn_call& fn) 
	{ 
		sprite_instance* sprite = sprite_getptr(fn);
		sprite_instance* parent = cast_to<sprite_instance>(sprite->get_parent());
		if (parent) 
		{ 
			parent->remove_display_object(sprite); 
		} 
	} 

	// loadMovie(url:String, [method:String]) : Void
	// TODO: implement [method:String]
	void sprite_loadmovie(const fn_call& fn) 
	{ 
		if (fn.nargs >= 1)
		{
			fn.env->load_file(fn.arg(0).to_string(), fn.this_ptr);
		}

	} 

	// public unloadMovie() : Void
	void sprite_unloadmovie(const fn_call& fn) 
	{ 
//		sprite_instance* sprite = sprite_getptr(fn);
//                UNUSED(sprite);
		fn.env->load_file("", fn.this_ptr);
	} 

	// getNextHighestDepth() : Number
	// Determines a depth value that you can pass to MovieClip.attachMovie(),
	// MovieClip.duplicateMovieClip(), or MovieClip.createEmptyMovieClip()
	// to ensure that Flash renders the movie clip in front of all other objects
	// on the same level and layer in the current movie clip.
	// The value returned is 0 or larger (that is, negative numbers are not returned).
	// Content created at design time (in the authoring tool) starts at depth -16383.
	void sprite_getnexthighestdepth(const fn_call& fn) 
	{ 
		int highest_depth;
		sprite_instance* sprite = sprite_getptr(fn);
		highest_depth = sprite->get_highest_depth() - ADJUST_DEPTH_VALUE;

		// highest_depth must be 0 or larger !!!
		//highest_depth = max( highest_depth, 0 );
		assert(highest_depth >= 0);
		fn.result->set_int(highest_depth);
	} 

	// public createTextField(instanceName:String, depth:Number,
	// x:Number, y:Number, width:Number, height:Number) : TextField
	void sprite_create_text_field(const fn_call& fn) 
	{ 
		sprite_instance* sprite = sprite_getptr(fn);
		fn.result->set_as_object(NULL);
		if (fn.nargs != 6)
		{
			log_error("createTextField: the number of arguments must be 6\n");
			return;
		}

		fn.result->set_as_object(sprite->create_text_field(
			fn.arg(0).to_string(),	// field name
			fn.arg(1).to_int() + ADJUST_DEPTH_VALUE,	// depth
			fn.arg(2).to_int(),	// x
			fn.arg(3).to_int(),	// y
			fn.arg(4).to_int(),	// width
			fn.arg(5).to_int()	// height
		));
	} 

	// public attachMovie(id:String, name:String, depth:Number, [initObject:Object]) : MovieClip
	// Takes a symbol from the library and attaches it to the movie clip.
	void sprite_attach_movie(const fn_call& fn) 
	{ 
		sprite_instance* sprite = sprite_getptr(fn);

		if (fn.nargs >= 3)
		{
			tu_string id = fn.arg(0).to_string();	// the exported name (sprite_definition)
			tu_string name = fn.arg(1).to_string();	// instance name
			int depth = fn.arg(2).to_int() + ADJUST_DEPTH_VALUE;
			sprite_instance* ch = sprite->attach_movie(id, name, depth);

			if (fn.nargs >= 4)
			{
				as_object* init_object = fn.arg(3).to_object();
				if (init_object)
				{
					init_object->copy_to(ch);
				}
			}
			fn.result->set_as_object(ch);
			return;
		}
		log_error("attachMovie needs 3 or 4 args\n"); 
	} 

	void sprite_set_fps(const fn_call& fn) 
	{ 
		sprite_instance* sprite = sprite_getptr(fn);

		if (fn.nargs == 1)
		{
			float fps = fn.arg(0).to_float();
			sprite->set_fps(fps);
		}
	} 


	// drawing API

	//	public beginFill(rgb:Number, [alpha:Number]) : Void
	void sprite_begin_fill(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		canvas* canva = sprite->get_canvas();
		assert(canva);

		rgba color(0, 0, 0, 255);
		if (fn.nargs > 0)
		{
			color.set(fn.arg(0).to_number());
		}
		canva->begin_fill(color);
	}

	//	public endFill() : Void
	void sprite_end_fill(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		canvas* canva = sprite->get_canvas();
		assert(canva);
		canva->end_fill();
	}

	//	public clear() : Void
	void sprite_clear(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		if (sprite->m_canvas != NULL)
		{
			sprite->remove_display_object(sprite->m_canvas.get_ptr());
			sprite->m_canvas = NULL;
		}
	}

	// public moveTo(x:Number, y:Number) : Void
	void sprite_move_to(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		canvas* canva = sprite->get_canvas();
		assert(canva);

		if (fn.nargs >= 2)
		{
			float x = PIXELS_TO_TWIPS(fn.arg(0).to_float());
			float y = PIXELS_TO_TWIPS(fn.arg(1).to_float());
			canva->move_to(x, y);
		}
	}

	//	public lineTo(x:Number, y:Number) : Void
	void sprite_line_to(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		canvas* canva = sprite->get_canvas();
		assert(canva);

		if (fn.nargs >= 2)
		{
			float x = PIXELS_TO_TWIPS(fn.arg(0).to_float());
			float y = PIXELS_TO_TWIPS(fn.arg(1).to_float());
			canva->line_to(x, y);
		}
	}

	//	public curveTo(controlX:Number, controlY:Number, anchorX:Number, anchorY:Number) : Void
	void sprite_curve_to(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		canvas* canva = sprite->get_canvas();
		assert(canva);

		if (fn.nargs >= 4)
		{
			float cx = PIXELS_TO_TWIPS(fn.arg(0).to_float());
			float cy = PIXELS_TO_TWIPS(fn.arg(1).to_float());
			float ax = PIXELS_TO_TWIPS(fn.arg(2).to_float());
			float ay = PIXELS_TO_TWIPS(fn.arg(3).to_float());
			canva->curve_to(cx, cy, ax, ay);
		}
	}

	// public lineStyle(thickness:Number, rgb:Number, alpha:Number, pixelHinting:Boolean,
	// noScale:String, capsStyle:String, jointStyle:String, miterLimit:Number) : Void
	void sprite_line_style(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		canvas* canva = sprite->get_canvas();
		assert(canva);

		// If a thickness is not specified, or if the parameter is undefined, a line is not drawn
		if (fn.nargs == 0)
		{
			canva->m_current_line = 0;
			canva->add_path(false);
			return;
		}

		Uint16 width = (Uint16) PIXELS_TO_TWIPS(fclamp(fn.arg(0).to_float(), 0, 255));
		rgba color(0, 0, 0, 255);

		if (fn.nargs >= 2)
		{
			color.set(fn.arg(1).to_float());
			if (fn.nargs >= 3)
			{
				float alpha = fclamp(fn.arg(2).to_float(), 0, 100);
				color.m_a = Uint8(255 * (alpha/100));

				// capsStyle:String - Added in Flash Player 8. 
				// A string that specifies the type of caps at the end of lines.
				// Valid values are: "round", "square", and "none". 
				// If a value is not indicated, Flash uses round caps. 
				if (fn.nargs >= 6)
				{
					//TODO
				}
			}
		}

		canva->set_line_style(width, color);
	}

	// flash9
	void sprite_add_script(const fn_call& fn)
	{
		sprite_instance* sprite = sprite_getptr(fn);
		if (fn.nargs == 2) 
		{
			// arg #1 - frame number, 0 based
			// arg #2 - function
			sprite->add_script(fn.arg(0).to_int(), fn.arg(1).to_function());
		}
	}

}
