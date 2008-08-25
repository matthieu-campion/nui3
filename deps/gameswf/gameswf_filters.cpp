// gameswf_filters.cpp	-- Julien Hamaide <julien.hamaide@gmail.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Filters

#include "gameswf.h"
#include "gameswf_types.h"
#include "gameswf_log.h"
#include "gameswf_stream.h"

namespace gameswf
{

	void read_filter_list( stream* in )
	{
		// reads FILTERLIST
		int filters = in->read_u8();
		for (int i = 0; i < filters; i++)
		{
			int filter_id = in->read_u8();
			switch (filter_id)
			{
			case 0 : // DropShadowFilter
				{
					rgba drop_shadow_color;
					drop_shadow_color.read_rgba(in);	// RGBA Color of the shadow
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(5); // passes
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = DropShadowFilter\n" ));
					break;
				}

			case 1 : // BlurFilter
				{
					float blur_x = in->read_fixed(); // Horizontal blur amount
					float blur_y = in->read_fixed(); // Vertical blur amount
					int passes = in->read_uint(5);	// Number of blur passes
					in->read_uint(3);	// Reserved UB[3] Must be 0
					IF_VERBOSE_PARSE(log_msg("  filter = BlurFilter\n" ));
					break;
				}

			case 2 : // GlowFilter
				{
					rgba glow_color;
					glow_color.read_rgba(in);	//RGBA Color of the shadow
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_glow = in->read_bool();	// Inner glow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					in->read_uint(5); // passes
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = GlowFilter\n" ));
					break;
				}

			case 3 : // BevelFilter
				{
					rgba shadow_color;
					shadow_color.read_rgba(in);	//RGBA Color of the shadow
					rgba highlight_color;
					highlight_color.read_rgba(in);	//RGBA Color of the highlight
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					bool on_top = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(4); // passes
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = BevelFilter\n" ));
					break;
				}

			case 4 : // GradientGlowFilter
				{
					int num_colors = in->read_u8();		// Number of colors in the gradient
					for (int i = 0; i < num_colors; i++)
					{
						rgba gradient_colors;
						gradient_colors.read_rgba(in);	// RGBA[NumColors] Gradient colors
					}
					for (int i = 0; i < num_colors; i++)
					{
						int gradient_ratio = in->read_u8();	// UI8[NumColors] Gradient ratios
					}
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					bool on_top = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(4); // passes
					IF_VERBOSE_PARSE(log_msg("  filter = GradientGlowFilter\n" ));
					break;
				}

			case 5 : // ConvolutionFilter
				{
					int matrix_x = in->read_u8();	// Horizontal matrix size
					int matrix_y = in->read_u8();	// Vertical matrix size
					float divisor = in->read_float();	// Divisor applied to the matrix values
					float bias = in->read_float();	// Bias applied to the matrix values
					for (int k = 0; k < matrix_x * matrix_y; k++)
					{
						in->read_float();	// Matrix values
					}
					rgba default_color;
					default_color.read_rgba(in);	// RGBA Default color for pixels outside the image
					in->read_uint(6);		// Reserved UB[6] Must be 0
					bool clamp = in->read_bool();	// UB[1] Clamp mode
					bool preserve_alpha = in->read_bool();	// UB[1]
					IF_VERBOSE_PARSE(log_msg("  filter = ConvolutionFilter\n" ));
					break;
				}

			case 6 : // ColorMatrixFilter
				// matrix is float[20]
				for (int k = 0; k < 20; k++)
				{
					in->read_float();
				}
				IF_VERBOSE_PARSE(log_msg("  filter = ColorMatrixFilter\n" ));
				break;

			case 7 : // GradientBevelFilter
				{
					int num_colors = in->read_u8();		// Number of colors in the gradient
					for (int i = 0; i < num_colors; i++)
					{
						rgba gradient_colors;
						gradient_colors.read_rgba(in);	// RGBA[NumColors] Gradient colors
					}
					for (int i = 0; i < num_colors; i++)
					{
						int gradient_ratio = in->read_u8();	// UI8[NumColors] Gradient ratios
					}
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					bool on_top = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(4); // passes
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = GradientBevelFilter\n" ));
					break;
				}

			default:
				assert(0);	// invalid input
			}
		}
	}
}

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
