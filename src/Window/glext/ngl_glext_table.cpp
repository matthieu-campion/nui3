/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* This file was generated with glext_gen.pl, please do not edit.  */

#include "nui.h"
#include "ngl_glext_table.h"

#if ((defined GL_OES_VERSION_1_1) || (defined _ANDROID_))

// No GL ES extentions in nglContext
GLExtNameInfo gpGLExtensionNameTable[] = { NULL, 0 };
GLExtFuncInfo gpGLExtensionFuncTable[] = { NULL, (GLExtFuncPtr)NULL };

#else

GLExtNameInfo gpGLExtensionNameTable[] =
{
  { "GL_VERSION_1_2",                 0   },
  { "GL_VERSION_1_2_DEPRECATED",      6   },
  { "GL_VERSION_1_3",                 38  },
  { "GL_VERSION_1_3_DEPRECATED",      47  },
  { "GL_VERSION_1_4",                 84  },
  { "GL_VERSION_1_4_DEPRECATED",      91  },
  { "GL_VERSION_1_5",                 129 },
  { "GL_VERSION_2_0",                 148 },
  { "GL_VERSION_2_1",                 241 },
  { "GL_VERSION_3_0",                 247 },
  { "GL_VERSION_3_1",                 305 },
  { "GL_VERSION_3_2",                 309 },
  { "GL_VERSION_3_3",                 312 },
  { "GL_VERSION_4_0",                 313 },
  { "GL_ARB_multitexture",            318 },
  { "GL_ARB_transpose_matrix",        352 },
  { "GL_ARB_multisample",             356 },
  { "GL_ARB_texture_compression",     357 },
  { "GL_ARB_point_parameters",        364 },
  { "GL_ARB_vertex_blend",            366 },
  { "GL_ARB_matrix_palette",          376 },
  { "GL_ARB_window_pos",              381 },
  { "GL_ARB_vertex_program",          397 },
  { "GL_ARB_vertex_buffer_object",    459 },
  { "GL_ARB_occlusion_query",         470 },
  { "GL_ARB_shader_objects",          478 },
  { "GL_ARB_vertex_shader",           517 },
  { "GL_ARB_draw_buffers",            520 },
  { "GL_ARB_color_buffer_float",      521 },
  { "GL_ARB_draw_instanced",          522 },
  { "GL_ARB_framebuffer_object",      524 },
  { "GL_ARB_geometry_shader4",        544 },
  { "GL_ARB_instanced_arrays",        548 },
  { "GL_ARB_map_buffer_range",        549 },
  { "GL_ARB_texture_buffer_object",   551 },
  { "GL_ARB_vertex_array_object",     552 },
  { "GL_ARB_uniform_buffer_object",   556 },
  { "GL_ARB_copy_buffer",             563 },
  { "GL_ARB_draw_elements_base_vertex", 564 },
  { "GL_ARB_provoking_vertex",        568 },
  { "GL_ARB_sync",                    569 },
  { "GL_ARB_texture_multisample",     576 },
  { "GL_ARB_draw_buffers_blend",      580 },
  { "GL_ARB_sample_shading",          584 },
  { "GL_ARB_shading_language_include", 585 },
  { "GL_ARB_blend_func_extended",     591 },
  { "GL_ARB_sampler_objects",         593 },
  { "GL_ARB_timer_query",             607 },
  { "GL_ARB_vertex_type_2_10_10_10_rev", 610 },
  { "GL_ARB_draw_indirect",           648 },
  { "GL_ARB_gpu_shader_fp64",         650 },
  { "GL_ARB_shader_subroutine",       668 },
  { "GL_ARB_tessellation_shader",     676 },
  { "GL_ARB_transform_feedback2",     678 },
  { "GL_ARB_transform_feedback3",     685 },
  { "GL_ARB_ES2_compatibility",       689 },
  { "GL_ARB_get_program_binary",      694 },
  { "GL_ARB_separate_shader_objects", 697 },
  { "GL_ARB_vertex_attrib_64bit",     757 },
  { "GL_ARB_viewport_array",          767 },
  { "GL_ARB_cl_event",                777 },
  { "GL_ARB_debug_output",            778 },
  { "GL_ARB_robustness",              782 },
  { "GL_EXT_blend_color",             802 },
  { "GL_EXT_polygon_offset",          803 },
  { "GL_EXT_texture3D",               804 },
  { "GL_SGIS_texture_filter4",        806 },
  { "GL_EXT_subtexture",              808 },
  { "GL_EXT_copy_texture",            810 },
  { "GL_EXT_histogram",               815 },
  { "GL_EXT_convolution",             825 },
  { "GL_SGI_color_table",             838 },
  { "GL_SGIX_pixel_texture",          845 },
  { "GL_SGIS_pixel_texture",          846 },
  { "GL_SGIS_texture4D",              852 },
  { "GL_EXT_texture_object",          854 },
  { "GL_SGIS_detail_texture",         860 },
  { "GL_SGIS_sharpen_texture",        862 },
  { "GL_SGIS_multisample",            864 },
  { "GL_EXT_vertex_array",            866 },
  { "GL_EXT_blend_minmax",            875 },
  { "GL_SGIX_sprite",                 876 },
  { "GL_EXT_point_parameters",        880 },
  { "GL_SGIS_point_parameters",       882 },
  { "GL_SGIX_instruments",            884 },
  { "GL_SGIX_framezoom",              890 },
  { "GL_SGIX_tag_sample_buffer",      891 },
  { "GL_SGIX_polynomial_ffd",         892 },
  { "GL_SGIX_reference_plane",        896 },
  { "GL_SGIX_flush_raster",           897 },
  { "GL_SGIS_fog_function",           898 },
  { "GL_HP_image_transform",          900 },
  { "GL_EXT_color_subtable",          906 },
  { "GL_PGI_misc_hints",              908 },
  { "GL_EXT_paletted_texture",        909 },
  { "GL_SGIX_list_priority",          913 },
  { "GL_EXT_index_material",          919 },
  { "GL_EXT_index_func",              920 },
  { "GL_EXT_compiled_vertex_array",   921 },
  { "GL_EXT_cull_vertex",             923 },
  { "GL_SGIX_fragment_lighting",      925 },
  { "GL_EXT_draw_range_elements",     943 },
  { "GL_EXT_light_texture",           944 },
  { "GL_SGIX_async",                  947 },
  { "GL_INTEL_parallel_arrays",       953 },
  { "GL_EXT_pixel_transform",         957 },
  { "GL_EXT_secondary_color",         961 },
  { "GL_EXT_texture_perturb_normal",  978 },
  { "GL_EXT_multi_draw_arrays",       979 },
  { "GL_EXT_fog_coord",               981 },
  { "GL_EXT_coordinate_frame",        986 },
  { "GL_SUNX_constant_data",          1008 },
  { "GL_SUN_global_alpha",            1009 },
  { "GL_SUN_triangle_list",           1017 },
  { "GL_SUN_vertex",                  1024 },
  { "GL_EXT_blend_func_separate",     1064 },
  { "GL_INGR_blend_func_separate",    1065 },
  { "GL_EXT_vertex_weighting",        1066 },
  { "GL_NV_vertex_array_range",       1069 },
  { "GL_NV_register_combiners",       1071 },
  { "GL_MESA_resize_buffers",         1084 },
  { "GL_MESA_window_pos",             1085 },
  { "GL_IBM_multimode_draw_arrays",   1109 },
  { "GL_IBM_vertex_array_lists",      1111 },
  { "GL_3DFX_tbuffer",                1119 },
  { "GL_EXT_multisample",             1120 },
  { "GL_SGIS_texture_color_mask",     1122 },
  { "GL_SGIX_igloo_interface",        1123 },
  { "GL_NV_fence",                    1124 },
  { "GL_NV_evaluators",               1131 },
  { "GL_NV_register_combiners2",      1140 },
  { "GL_NV_vertex_program",           1142 },
  { "GL_ATI_envmap_bumpmap",          1206 },
  { "GL_ATI_fragment_shader",         1210 },
  { "GL_ATI_pn_triangles",            1224 },
  { "GL_ATI_vertex_array_object",     1226 },
  { "GL_EXT_vertex_shader",           1238 },
  { "GL_ATI_vertex_streams",          1280 },
  { "GL_ATI_element_array",           1325 },
  { "GL_SUN_mesh_array",              1328 },
  { "GL_NV_occlusion_query",          1329 },
  { "GL_NV_point_sprite",             1336 },
  { "GL_EXT_stencil_two_side",        1338 },
  { "GL_APPLE_element_array",         1339 },
  { "GL_APPLE_fence",                 1344 },
  { "GL_APPLE_vertex_array_object",   1352 },
  { "GL_APPLE_vertex_array_range",    1356 },
  { "GL_ATI_draw_buffers",            1359 },
  { "GL_NV_fragment_program",         1360 },
  { "GL_NV_half_float",               1366 },
  { "GL_NV_pixel_data_range",         1412 },
  { "GL_NV_primitive_restart",        1414 },
  { "GL_ATI_map_object_buffer",       1416 },
  { "GL_ATI_separate_stencil",        1418 },
  { "GL_ATI_vertex_attrib_array_object", 1420 },
  { "GL_EXT_depth_bounds_test",       1423 },
  { "GL_EXT_blend_equation_separate", 1424 },
  { "GL_EXT_framebuffer_object",      1425 },
  { "GL_GREMEDY_string_marker",       1442 },
  { "GL_EXT_stencil_clear_tag",       1443 },
  { "GL_EXT_framebuffer_blit",        1444 },
  { "GL_EXT_framebuffer_multisample", 1445 },
  { "GL_EXT_timer_query",             1446 },
  { "GL_EXT_gpu_program_parameters",  1448 },
  { "GL_APPLE_flush_buffer_range",    1450 },
  { "GL_NV_gpu_program4",             1452 },
  { "GL_NV_geometry_program4",        1468 },
  { "GL_EXT_geometry_shader4",        1472 },
  { "GL_NV_vertex_program4",          1473 },
  { "GL_EXT_gpu_shader4",             1496 },
  { "GL_EXT_draw_instanced",          1507 },
  { "GL_EXT_texture_buffer_object",   1509 },
  { "GL_NV_depth_buffer_float",       1510 },
  { "GL_NV_framebuffer_multisample_coverage", 1513 },
  { "GL_NV_parameter_buffer_object",  1514 },
  { "GL_EXT_draw_buffers2",           1517 },
  { "GL_NV_transform_feedback",       1523 },
  { "GL_EXT_bindable_uniform",        1535 },
  { "GL_EXT_texture_integer",         1538 },
  { "GL_GREMEDY_frame_terminator",    1544 },
  { "GL_NV_conditional_render",       1545 },
  { "GL_NV_present_video",            1547 },
  { "GL_EXT_transform_feedback",      1553 },
  { "GL_EXT_direct_state_access",     1560 },
  { "GL_NV_explicit_multisample",     1766 },
  { "GL_NV_transform_feedback2",      1769 },
  { "GL_AMD_performance_monitor",     1776 },
  { "GL_AMD_vertex_shader_tesselator", 1787 },
  { "GL_EXT_provoking_vertex",        1789 },
  { "GL_AMD_draw_buffers_blend",      1790 },
  { "GL_APPLE_texture_range",         1794 },
  { "GL_APPLE_vertex_program_evaluators", 1796 },
  { "GL_APPLE_object_purgeable",      1803 },
  { "GL_NV_video_capture",            1806 },
  { "GL_NV_copy_image",               1818 },
  { "GL_EXT_separate_shader_objects", 1819 },
  { "GL_NV_shader_buffer_load",       1822 },
  { "GL_NV_vertex_buffer_unified_memory", 1836 },
  { "GL_NV_texture_barrier",          1848 },
  { "GL_EXT_shader_image_load_store", 1849 },
  { "GL_EXT_vertex_attrib_64bit",     1851 },
  { "GL_NV_gpu_program5",             1862 },
  { "GL_NV_gpu_shader5",              1864 },
  { "GL_NV_vertex_attrib_integer_64bit", 1897 },
  { "GL_AMD_name_gen_delete",         1916 },
  { "GL_AMD_debug_output",            1919 },
  { "GL_NV_vdpau_interop",            1923 },
  { "GL_NV_texture_multisample",      1933 },
  { "GL_AMD_sample_positions",        1939 },
  { "GL_EXT_x11_sync_object",         1940 },
  { "GL_AMD_multi_draw_indirect",     1941 },

  // End of list
  { NULL, 1943 }
};


GLExtFuncInfo gpGLExtensionFuncTable[] =
{
/* GL_VERSION_1_2
 */
/* 0   */ { "glBlendColor", (GLExtFuncPtr)&nglContext::glBlendColor },
/* 1   */ { "glBlendEquation", (GLExtFuncPtr)&nglContext::glBlendEquation },
/* 2   */ { "glDrawRangeElements", (GLExtFuncPtr)&nglContext::glDrawRangeElements },
/* 3   */ { "glTexImage3D", (GLExtFuncPtr)&nglContext::glTexImage3D },
/* 4   */ { "glTexSubImage3D", (GLExtFuncPtr)&nglContext::glTexSubImage3D },
/* 5   */ { "glCopyTexSubImage3D", (GLExtFuncPtr)&nglContext::glCopyTexSubImage3D },

/* GL_VERSION_1_2_DEPRECATED
 */
/* 6   */ { "glColorTable", (GLExtFuncPtr)&nglContext::glColorTable },
/* 7   */ { "glColorTableParameterfv", (GLExtFuncPtr)&nglContext::glColorTableParameterfv },
/* 8   */ { "glColorTableParameteriv", (GLExtFuncPtr)&nglContext::glColorTableParameteriv },
/* 9   */ { "glCopyColorTable", (GLExtFuncPtr)&nglContext::glCopyColorTable },
/* 10  */ { "glGetColorTable", (GLExtFuncPtr)&nglContext::glGetColorTable },
/* 11  */ { "glGetColorTableParameterfv", (GLExtFuncPtr)&nglContext::glGetColorTableParameterfv },
/* 12  */ { "glGetColorTableParameteriv", (GLExtFuncPtr)&nglContext::glGetColorTableParameteriv },
/* 13  */ { "glColorSubTable", (GLExtFuncPtr)&nglContext::glColorSubTable },
/* 14  */ { "glCopyColorSubTable", (GLExtFuncPtr)&nglContext::glCopyColorSubTable },
/* 15  */ { "glConvolutionFilter1D", (GLExtFuncPtr)&nglContext::glConvolutionFilter1D },
/* 16  */ { "glConvolutionFilter2D", (GLExtFuncPtr)&nglContext::glConvolutionFilter2D },
/* 17  */ { "glConvolutionParameterf", (GLExtFuncPtr)&nglContext::glConvolutionParameterf },
/* 18  */ { "glConvolutionParameterfv", (GLExtFuncPtr)&nglContext::glConvolutionParameterfv },
/* 19  */ { "glConvolutionParameteri", (GLExtFuncPtr)&nglContext::glConvolutionParameteri },
/* 20  */ { "glConvolutionParameteriv", (GLExtFuncPtr)&nglContext::glConvolutionParameteriv },
/* 21  */ { "glCopyConvolutionFilter1D", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter1D },
/* 22  */ { "glCopyConvolutionFilter2D", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter2D },
/* 23  */ { "glGetConvolutionFilter", (GLExtFuncPtr)&nglContext::glGetConvolutionFilter },
/* 24  */ { "glGetConvolutionParameterfv", (GLExtFuncPtr)&nglContext::glGetConvolutionParameterfv },
/* 25  */ { "glGetConvolutionParameteriv", (GLExtFuncPtr)&nglContext::glGetConvolutionParameteriv },
/* 26  */ { "glGetSeparableFilter", (GLExtFuncPtr)&nglContext::glGetSeparableFilter },
/* 27  */ { "glSeparableFilter2D", (GLExtFuncPtr)&nglContext::glSeparableFilter2D },
/* 28  */ { "glGetHistogram", (GLExtFuncPtr)&nglContext::glGetHistogram },
/* 29  */ { "glGetHistogramParameterfv", (GLExtFuncPtr)&nglContext::glGetHistogramParameterfv },
/* 30  */ { "glGetHistogramParameteriv", (GLExtFuncPtr)&nglContext::glGetHistogramParameteriv },
/* 31  */ { "glGetMinmax", (GLExtFuncPtr)&nglContext::glGetMinmax },
/* 32  */ { "glGetMinmaxParameterfv", (GLExtFuncPtr)&nglContext::glGetMinmaxParameterfv },
/* 33  */ { "glGetMinmaxParameteriv", (GLExtFuncPtr)&nglContext::glGetMinmaxParameteriv },
/* 34  */ { "glHistogram", (GLExtFuncPtr)&nglContext::glHistogram },
/* 35  */ { "glMinmax", (GLExtFuncPtr)&nglContext::glMinmax },
/* 36  */ { "glResetHistogram", (GLExtFuncPtr)&nglContext::glResetHistogram },
/* 37  */ { "glResetMinmax", (GLExtFuncPtr)&nglContext::glResetMinmax },

/* GL_VERSION_1_3
 */
/* 38  */ { "glActiveTexture", (GLExtFuncPtr)&nglContext::glActiveTexture },
/* 39  */ { "glSampleCoverage", (GLExtFuncPtr)&nglContext::glSampleCoverage },
/* 40  */ { "glCompressedTexImage3D", (GLExtFuncPtr)&nglContext::glCompressedTexImage3D },
/* 41  */ { "glCompressedTexImage2D", (GLExtFuncPtr)&nglContext::glCompressedTexImage2D },
/* 42  */ { "glCompressedTexImage1D", (GLExtFuncPtr)&nglContext::glCompressedTexImage1D },
/* 43  */ { "glCompressedTexSubImage3D", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage3D },
/* 44  */ { "glCompressedTexSubImage2D", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage2D },
/* 45  */ { "glCompressedTexSubImage1D", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage1D },
/* 46  */ { "glGetCompressedTexImage", (GLExtFuncPtr)&nglContext::glGetCompressedTexImage },

/* GL_VERSION_1_3_DEPRECATED
 */
/* 47  */ { "glClientActiveTexture", (GLExtFuncPtr)&nglContext::glClientActiveTexture },
/* 48  */ { "glMultiTexCoord1d", (GLExtFuncPtr)&nglContext::glMultiTexCoord1d },
/* 49  */ { "glMultiTexCoord1dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1dv },
/* 50  */ { "glMultiTexCoord1f", (GLExtFuncPtr)&nglContext::glMultiTexCoord1f },
/* 51  */ { "glMultiTexCoord1fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1fv },
/* 52  */ { "glMultiTexCoord1i", (GLExtFuncPtr)&nglContext::glMultiTexCoord1i },
/* 53  */ { "glMultiTexCoord1iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1iv },
/* 54  */ { "glMultiTexCoord1s", (GLExtFuncPtr)&nglContext::glMultiTexCoord1s },
/* 55  */ { "glMultiTexCoord1sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1sv },
/* 56  */ { "glMultiTexCoord2d", (GLExtFuncPtr)&nglContext::glMultiTexCoord2d },
/* 57  */ { "glMultiTexCoord2dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2dv },
/* 58  */ { "glMultiTexCoord2f", (GLExtFuncPtr)&nglContext::glMultiTexCoord2f },
/* 59  */ { "glMultiTexCoord2fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2fv },
/* 60  */ { "glMultiTexCoord2i", (GLExtFuncPtr)&nglContext::glMultiTexCoord2i },
/* 61  */ { "glMultiTexCoord2iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2iv },
/* 62  */ { "glMultiTexCoord2s", (GLExtFuncPtr)&nglContext::glMultiTexCoord2s },
/* 63  */ { "glMultiTexCoord2sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2sv },
/* 64  */ { "glMultiTexCoord3d", (GLExtFuncPtr)&nglContext::glMultiTexCoord3d },
/* 65  */ { "glMultiTexCoord3dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3dv },
/* 66  */ { "glMultiTexCoord3f", (GLExtFuncPtr)&nglContext::glMultiTexCoord3f },
/* 67  */ { "glMultiTexCoord3fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3fv },
/* 68  */ { "glMultiTexCoord3i", (GLExtFuncPtr)&nglContext::glMultiTexCoord3i },
/* 69  */ { "glMultiTexCoord3iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3iv },
/* 70  */ { "glMultiTexCoord3s", (GLExtFuncPtr)&nglContext::glMultiTexCoord3s },
/* 71  */ { "glMultiTexCoord3sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3sv },
/* 72  */ { "glMultiTexCoord4d", (GLExtFuncPtr)&nglContext::glMultiTexCoord4d },
/* 73  */ { "glMultiTexCoord4dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4dv },
/* 74  */ { "glMultiTexCoord4f", (GLExtFuncPtr)&nglContext::glMultiTexCoord4f },
/* 75  */ { "glMultiTexCoord4fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4fv },
/* 76  */ { "glMultiTexCoord4i", (GLExtFuncPtr)&nglContext::glMultiTexCoord4i },
/* 77  */ { "glMultiTexCoord4iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4iv },
/* 78  */ { "glMultiTexCoord4s", (GLExtFuncPtr)&nglContext::glMultiTexCoord4s },
/* 79  */ { "glMultiTexCoord4sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4sv },
/* 80  */ { "glLoadTransposeMatrixf", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixf },
/* 81  */ { "glLoadTransposeMatrixd", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixd },
/* 82  */ { "glMultTransposeMatrixf", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixf },
/* 83  */ { "glMultTransposeMatrixd", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixd },

/* GL_VERSION_1_4
 */
/* 84  */ { "glBlendFuncSeparate", (GLExtFuncPtr)&nglContext::glBlendFuncSeparate },
/* 85  */ { "glMultiDrawArrays", (GLExtFuncPtr)&nglContext::glMultiDrawArrays },
/* 86  */ { "glMultiDrawElements", (GLExtFuncPtr)&nglContext::glMultiDrawElements },
/* 87  */ { "glPointParameterf", (GLExtFuncPtr)&nglContext::glPointParameterf },
/* 88  */ { "glPointParameterfv", (GLExtFuncPtr)&nglContext::glPointParameterfv },
/* 89  */ { "glPointParameteri", (GLExtFuncPtr)&nglContext::glPointParameteri },
/* 90  */ { "glPointParameteriv", (GLExtFuncPtr)&nglContext::glPointParameteriv },

/* GL_VERSION_1_4_DEPRECATED
 */
/* 91  */ { "glFogCoordf", (GLExtFuncPtr)&nglContext::glFogCoordf },
/* 92  */ { "glFogCoordfv", (GLExtFuncPtr)&nglContext::glFogCoordfv },
/* 93  */ { "glFogCoordd", (GLExtFuncPtr)&nglContext::glFogCoordd },
/* 94  */ { "glFogCoorddv", (GLExtFuncPtr)&nglContext::glFogCoorddv },
/* 95  */ { "glFogCoordPointer", (GLExtFuncPtr)&nglContext::glFogCoordPointer },
/* 96  */ { "glSecondaryColor3b", (GLExtFuncPtr)&nglContext::glSecondaryColor3b },
/* 97  */ { "glSecondaryColor3bv", (GLExtFuncPtr)&nglContext::glSecondaryColor3bv },
/* 98  */ { "glSecondaryColor3d", (GLExtFuncPtr)&nglContext::glSecondaryColor3d },
/* 99  */ { "glSecondaryColor3dv", (GLExtFuncPtr)&nglContext::glSecondaryColor3dv },
/* 100 */ { "glSecondaryColor3f", (GLExtFuncPtr)&nglContext::glSecondaryColor3f },
/* 101 */ { "glSecondaryColor3fv", (GLExtFuncPtr)&nglContext::glSecondaryColor3fv },
/* 102 */ { "glSecondaryColor3i", (GLExtFuncPtr)&nglContext::glSecondaryColor3i },
/* 103 */ { "glSecondaryColor3iv", (GLExtFuncPtr)&nglContext::glSecondaryColor3iv },
/* 104 */ { "glSecondaryColor3s", (GLExtFuncPtr)&nglContext::glSecondaryColor3s },
/* 105 */ { "glSecondaryColor3sv", (GLExtFuncPtr)&nglContext::glSecondaryColor3sv },
/* 106 */ { "glSecondaryColor3ub", (GLExtFuncPtr)&nglContext::glSecondaryColor3ub },
/* 107 */ { "glSecondaryColor3ubv", (GLExtFuncPtr)&nglContext::glSecondaryColor3ubv },
/* 108 */ { "glSecondaryColor3ui", (GLExtFuncPtr)&nglContext::glSecondaryColor3ui },
/* 109 */ { "glSecondaryColor3uiv", (GLExtFuncPtr)&nglContext::glSecondaryColor3uiv },
/* 110 */ { "glSecondaryColor3us", (GLExtFuncPtr)&nglContext::glSecondaryColor3us },
/* 111 */ { "glSecondaryColor3usv", (GLExtFuncPtr)&nglContext::glSecondaryColor3usv },
/* 112 */ { "glSecondaryColorPointer", (GLExtFuncPtr)&nglContext::glSecondaryColorPointer },
/* 113 */ { "glWindowPos2d", (GLExtFuncPtr)&nglContext::glWindowPos2d },
/* 114 */ { "glWindowPos2dv", (GLExtFuncPtr)&nglContext::glWindowPos2dv },
/* 115 */ { "glWindowPos2f", (GLExtFuncPtr)&nglContext::glWindowPos2f },
/* 116 */ { "glWindowPos2fv", (GLExtFuncPtr)&nglContext::glWindowPos2fv },
/* 117 */ { "glWindowPos2i", (GLExtFuncPtr)&nglContext::glWindowPos2i },
/* 118 */ { "glWindowPos2iv", (GLExtFuncPtr)&nglContext::glWindowPos2iv },
/* 119 */ { "glWindowPos2s", (GLExtFuncPtr)&nglContext::glWindowPos2s },
/* 120 */ { "glWindowPos2sv", (GLExtFuncPtr)&nglContext::glWindowPos2sv },
/* 121 */ { "glWindowPos3d", (GLExtFuncPtr)&nglContext::glWindowPos3d },
/* 122 */ { "glWindowPos3dv", (GLExtFuncPtr)&nglContext::glWindowPos3dv },
/* 123 */ { "glWindowPos3f", (GLExtFuncPtr)&nglContext::glWindowPos3f },
/* 124 */ { "glWindowPos3fv", (GLExtFuncPtr)&nglContext::glWindowPos3fv },
/* 125 */ { "glWindowPos3i", (GLExtFuncPtr)&nglContext::glWindowPos3i },
/* 126 */ { "glWindowPos3iv", (GLExtFuncPtr)&nglContext::glWindowPos3iv },
/* 127 */ { "glWindowPos3s", (GLExtFuncPtr)&nglContext::glWindowPos3s },
/* 128 */ { "glWindowPos3sv", (GLExtFuncPtr)&nglContext::glWindowPos3sv },

/* GL_VERSION_1_5
 */
/* 129 */ { "glGenQueries", (GLExtFuncPtr)&nglContext::glGenQueries },
/* 130 */ { "glDeleteQueries", (GLExtFuncPtr)&nglContext::glDeleteQueries },
/* 131 */ { "glIsQuery", (GLExtFuncPtr)&nglContext::glIsQuery },
/* 132 */ { "glBeginQuery", (GLExtFuncPtr)&nglContext::glBeginQuery },
/* 133 */ { "glEndQuery", (GLExtFuncPtr)&nglContext::glEndQuery },
/* 134 */ { "glGetQueryiv", (GLExtFuncPtr)&nglContext::glGetQueryiv },
/* 135 */ { "glGetQueryObjectiv", (GLExtFuncPtr)&nglContext::glGetQueryObjectiv },
/* 136 */ { "glGetQueryObjectuiv", (GLExtFuncPtr)&nglContext::glGetQueryObjectuiv },
/* 137 */ { "glBindBuffer", (GLExtFuncPtr)&nglContext::glBindBuffer },
/* 138 */ { "glDeleteBuffers", (GLExtFuncPtr)&nglContext::glDeleteBuffers },
/* 139 */ { "glGenBuffers", (GLExtFuncPtr)&nglContext::glGenBuffers },
/* 140 */ { "glIsBuffer", (GLExtFuncPtr)&nglContext::glIsBuffer },
/* 141 */ { "glBufferData", (GLExtFuncPtr)&nglContext::glBufferData },
/* 142 */ { "glBufferSubData", (GLExtFuncPtr)&nglContext::glBufferSubData },
/* 143 */ { "glGetBufferSubData", (GLExtFuncPtr)&nglContext::glGetBufferSubData },
/* 144 */ { "glMapBuffer", (GLExtFuncPtr)&nglContext::glMapBuffer },
/* 145 */ { "glUnmapBuffer", (GLExtFuncPtr)&nglContext::glUnmapBuffer },
/* 146 */ { "glGetBufferParameteriv", (GLExtFuncPtr)&nglContext::glGetBufferParameteriv },
/* 147 */ { "glGetBufferPointerv", (GLExtFuncPtr)&nglContext::glGetBufferPointerv },

/* GL_VERSION_2_0
 */
/* 148 */ { "glBlendEquationSeparate", (GLExtFuncPtr)&nglContext::glBlendEquationSeparate },
/* 149 */ { "glDrawBuffers", (GLExtFuncPtr)&nglContext::glDrawBuffers },
/* 150 */ { "glStencilOpSeparate", (GLExtFuncPtr)&nglContext::glStencilOpSeparate },
/* 151 */ { "glStencilFuncSeparate", (GLExtFuncPtr)&nglContext::glStencilFuncSeparate },
/* 152 */ { "glStencilMaskSeparate", (GLExtFuncPtr)&nglContext::glStencilMaskSeparate },
/* 153 */ { "glAttachShader", (GLExtFuncPtr)&nglContext::glAttachShader },
/* 154 */ { "glBindAttribLocation", (GLExtFuncPtr)&nglContext::glBindAttribLocation },
/* 155 */ { "glCompileShader", (GLExtFuncPtr)&nglContext::glCompileShader },
/* 156 */ { "glCreateProgram", (GLExtFuncPtr)&nglContext::glCreateProgram },
/* 157 */ { "glCreateShader", (GLExtFuncPtr)&nglContext::glCreateShader },
/* 158 */ { "glDeleteProgram", (GLExtFuncPtr)&nglContext::glDeleteProgram },
/* 159 */ { "glDeleteShader", (GLExtFuncPtr)&nglContext::glDeleteShader },
/* 160 */ { "glDetachShader", (GLExtFuncPtr)&nglContext::glDetachShader },
/* 161 */ { "glDisableVertexAttribArray", (GLExtFuncPtr)&nglContext::glDisableVertexAttribArray },
/* 162 */ { "glEnableVertexAttribArray", (GLExtFuncPtr)&nglContext::glEnableVertexAttribArray },
/* 163 */ { "glGetActiveAttrib", (GLExtFuncPtr)&nglContext::glGetActiveAttrib },
/* 164 */ { "glGetActiveUniform", (GLExtFuncPtr)&nglContext::glGetActiveUniform },
/* 165 */ { "glGetAttachedShaders", (GLExtFuncPtr)&nglContext::glGetAttachedShaders },
/* 166 */ { "glGetAttribLocation", (GLExtFuncPtr)&nglContext::glGetAttribLocation },
/* 167 */ { "glGetProgramiv", (GLExtFuncPtr)&nglContext::glGetProgramiv },
/* 168 */ { "glGetProgramInfoLog", (GLExtFuncPtr)&nglContext::glGetProgramInfoLog },
/* 169 */ { "glGetShaderiv", (GLExtFuncPtr)&nglContext::glGetShaderiv },
/* 170 */ { "glGetShaderInfoLog", (GLExtFuncPtr)&nglContext::glGetShaderInfoLog },
/* 171 */ { "glGetShaderSource", (GLExtFuncPtr)&nglContext::glGetShaderSource },
/* 172 */ { "glGetUniformLocation", (GLExtFuncPtr)&nglContext::glGetUniformLocation },
/* 173 */ { "glGetUniformfv", (GLExtFuncPtr)&nglContext::glGetUniformfv },
/* 174 */ { "glGetUniformiv", (GLExtFuncPtr)&nglContext::glGetUniformiv },
/* 175 */ { "glGetVertexAttribdv", (GLExtFuncPtr)&nglContext::glGetVertexAttribdv },
/* 176 */ { "glGetVertexAttribfv", (GLExtFuncPtr)&nglContext::glGetVertexAttribfv },
/* 177 */ { "glGetVertexAttribiv", (GLExtFuncPtr)&nglContext::glGetVertexAttribiv },
/* 178 */ { "glGetVertexAttribPointerv", (GLExtFuncPtr)&nglContext::glGetVertexAttribPointerv },
/* 179 */ { "glIsProgram", (GLExtFuncPtr)&nglContext::glIsProgram },
/* 180 */ { "glIsShader", (GLExtFuncPtr)&nglContext::glIsShader },
/* 181 */ { "glLinkProgram", (GLExtFuncPtr)&nglContext::glLinkProgram },
/* 182 */ { "glShaderSource", (GLExtFuncPtr)&nglContext::glShaderSource },
/* 183 */ { "glUseProgram", (GLExtFuncPtr)&nglContext::glUseProgram },
/* 184 */ { "glUniform1f", (GLExtFuncPtr)&nglContext::glUniform1f },
/* 185 */ { "glUniform2f", (GLExtFuncPtr)&nglContext::glUniform2f },
/* 186 */ { "glUniform3f", (GLExtFuncPtr)&nglContext::glUniform3f },
/* 187 */ { "glUniform4f", (GLExtFuncPtr)&nglContext::glUniform4f },
/* 188 */ { "glUniform1i", (GLExtFuncPtr)&nglContext::glUniform1i },
/* 189 */ { "glUniform2i", (GLExtFuncPtr)&nglContext::glUniform2i },
/* 190 */ { "glUniform3i", (GLExtFuncPtr)&nglContext::glUniform3i },
/* 191 */ { "glUniform4i", (GLExtFuncPtr)&nglContext::glUniform4i },
/* 192 */ { "glUniform1fv", (GLExtFuncPtr)&nglContext::glUniform1fv },
/* 193 */ { "glUniform2fv", (GLExtFuncPtr)&nglContext::glUniform2fv },
/* 194 */ { "glUniform3fv", (GLExtFuncPtr)&nglContext::glUniform3fv },
/* 195 */ { "glUniform4fv", (GLExtFuncPtr)&nglContext::glUniform4fv },
/* 196 */ { "glUniform1iv", (GLExtFuncPtr)&nglContext::glUniform1iv },
/* 197 */ { "glUniform2iv", (GLExtFuncPtr)&nglContext::glUniform2iv },
/* 198 */ { "glUniform3iv", (GLExtFuncPtr)&nglContext::glUniform3iv },
/* 199 */ { "glUniform4iv", (GLExtFuncPtr)&nglContext::glUniform4iv },
/* 200 */ { "glUniformMatrix2fv", (GLExtFuncPtr)&nglContext::glUniformMatrix2fv },
/* 201 */ { "glUniformMatrix3fv", (GLExtFuncPtr)&nglContext::glUniformMatrix3fv },
/* 202 */ { "glUniformMatrix4fv", (GLExtFuncPtr)&nglContext::glUniformMatrix4fv },
/* 203 */ { "glValidateProgram", (GLExtFuncPtr)&nglContext::glValidateProgram },
/* 204 */ { "glVertexAttrib1d", (GLExtFuncPtr)&nglContext::glVertexAttrib1d },
/* 205 */ { "glVertexAttrib1dv", (GLExtFuncPtr)&nglContext::glVertexAttrib1dv },
/* 206 */ { "glVertexAttrib1f", (GLExtFuncPtr)&nglContext::glVertexAttrib1f },
/* 207 */ { "glVertexAttrib1fv", (GLExtFuncPtr)&nglContext::glVertexAttrib1fv },
/* 208 */ { "glVertexAttrib1s", (GLExtFuncPtr)&nglContext::glVertexAttrib1s },
/* 209 */ { "glVertexAttrib1sv", (GLExtFuncPtr)&nglContext::glVertexAttrib1sv },
/* 210 */ { "glVertexAttrib2d", (GLExtFuncPtr)&nglContext::glVertexAttrib2d },
/* 211 */ { "glVertexAttrib2dv", (GLExtFuncPtr)&nglContext::glVertexAttrib2dv },
/* 212 */ { "glVertexAttrib2f", (GLExtFuncPtr)&nglContext::glVertexAttrib2f },
/* 213 */ { "glVertexAttrib2fv", (GLExtFuncPtr)&nglContext::glVertexAttrib2fv },
/* 214 */ { "glVertexAttrib2s", (GLExtFuncPtr)&nglContext::glVertexAttrib2s },
/* 215 */ { "glVertexAttrib2sv", (GLExtFuncPtr)&nglContext::glVertexAttrib2sv },
/* 216 */ { "glVertexAttrib3d", (GLExtFuncPtr)&nglContext::glVertexAttrib3d },
/* 217 */ { "glVertexAttrib3dv", (GLExtFuncPtr)&nglContext::glVertexAttrib3dv },
/* 218 */ { "glVertexAttrib3f", (GLExtFuncPtr)&nglContext::glVertexAttrib3f },
/* 219 */ { "glVertexAttrib3fv", (GLExtFuncPtr)&nglContext::glVertexAttrib3fv },
/* 220 */ { "glVertexAttrib3s", (GLExtFuncPtr)&nglContext::glVertexAttrib3s },
/* 221 */ { "glVertexAttrib3sv", (GLExtFuncPtr)&nglContext::glVertexAttrib3sv },
/* 222 */ { "glVertexAttrib4Nbv", (GLExtFuncPtr)&nglContext::glVertexAttrib4Nbv },
/* 223 */ { "glVertexAttrib4Niv", (GLExtFuncPtr)&nglContext::glVertexAttrib4Niv },
/* 224 */ { "glVertexAttrib4Nsv", (GLExtFuncPtr)&nglContext::glVertexAttrib4Nsv },
/* 225 */ { "glVertexAttrib4Nub", (GLExtFuncPtr)&nglContext::glVertexAttrib4Nub },
/* 226 */ { "glVertexAttrib4Nubv", (GLExtFuncPtr)&nglContext::glVertexAttrib4Nubv },
/* 227 */ { "glVertexAttrib4Nuiv", (GLExtFuncPtr)&nglContext::glVertexAttrib4Nuiv },
/* 228 */ { "glVertexAttrib4Nusv", (GLExtFuncPtr)&nglContext::glVertexAttrib4Nusv },
/* 229 */ { "glVertexAttrib4bv", (GLExtFuncPtr)&nglContext::glVertexAttrib4bv },
/* 230 */ { "glVertexAttrib4d", (GLExtFuncPtr)&nglContext::glVertexAttrib4d },
/* 231 */ { "glVertexAttrib4dv", (GLExtFuncPtr)&nglContext::glVertexAttrib4dv },
/* 232 */ { "glVertexAttrib4f", (GLExtFuncPtr)&nglContext::glVertexAttrib4f },
/* 233 */ { "glVertexAttrib4fv", (GLExtFuncPtr)&nglContext::glVertexAttrib4fv },
/* 234 */ { "glVertexAttrib4iv", (GLExtFuncPtr)&nglContext::glVertexAttrib4iv },
/* 235 */ { "glVertexAttrib4s", (GLExtFuncPtr)&nglContext::glVertexAttrib4s },
/* 236 */ { "glVertexAttrib4sv", (GLExtFuncPtr)&nglContext::glVertexAttrib4sv },
/* 237 */ { "glVertexAttrib4ubv", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubv },
/* 238 */ { "glVertexAttrib4uiv", (GLExtFuncPtr)&nglContext::glVertexAttrib4uiv },
/* 239 */ { "glVertexAttrib4usv", (GLExtFuncPtr)&nglContext::glVertexAttrib4usv },
/* 240 */ { "glVertexAttribPointer", (GLExtFuncPtr)&nglContext::glVertexAttribPointer },

/* GL_VERSION_2_1
 */
/* 241 */ { "glUniformMatrix2x3fv", (GLExtFuncPtr)&nglContext::glUniformMatrix2x3fv },
/* 242 */ { "glUniformMatrix3x2fv", (GLExtFuncPtr)&nglContext::glUniformMatrix3x2fv },
/* 243 */ { "glUniformMatrix2x4fv", (GLExtFuncPtr)&nglContext::glUniformMatrix2x4fv },
/* 244 */ { "glUniformMatrix4x2fv", (GLExtFuncPtr)&nglContext::glUniformMatrix4x2fv },
/* 245 */ { "glUniformMatrix3x4fv", (GLExtFuncPtr)&nglContext::glUniformMatrix3x4fv },
/* 246 */ { "glUniformMatrix4x3fv", (GLExtFuncPtr)&nglContext::glUniformMatrix4x3fv },

/* GL_VERSION_3_0
 */
/* 247 */ { "glColorMaski", (GLExtFuncPtr)&nglContext::glColorMaski },
/* 248 */ { "glGetBooleani_v", (GLExtFuncPtr)&nglContext::glGetBooleani_v },
/* 249 */ { "glGetIntegeri_v", (GLExtFuncPtr)&nglContext::glGetIntegeri_v },
/* 250 */ { "glEnablei", (GLExtFuncPtr)&nglContext::glEnablei },
/* 251 */ { "glDisablei", (GLExtFuncPtr)&nglContext::glDisablei },
/* 252 */ { "glIsEnabledi", (GLExtFuncPtr)&nglContext::glIsEnabledi },
/* 253 */ { "glBeginTransformFeedback", (GLExtFuncPtr)&nglContext::glBeginTransformFeedback },
/* 254 */ { "glEndTransformFeedback", (GLExtFuncPtr)&nglContext::glEndTransformFeedback },
/* 255 */ { "glBindBufferRange", (GLExtFuncPtr)&nglContext::glBindBufferRange },
/* 256 */ { "glBindBufferBase", (GLExtFuncPtr)&nglContext::glBindBufferBase },
/* 257 */ { "glTransformFeedbackVaryings", (GLExtFuncPtr)&nglContext::glTransformFeedbackVaryings },
/* 258 */ { "glGetTransformFeedbackVarying", (GLExtFuncPtr)&nglContext::glGetTransformFeedbackVarying },
/* 259 */ { "glClampColor", (GLExtFuncPtr)&nglContext::glClampColor },
/* 260 */ { "glBeginConditionalRender", (GLExtFuncPtr)&nglContext::glBeginConditionalRender },
/* 261 */ { "glEndConditionalRender", (GLExtFuncPtr)&nglContext::glEndConditionalRender },
/* 262 */ { "glVertexAttribIPointer", (GLExtFuncPtr)&nglContext::glVertexAttribIPointer },
/* 263 */ { "glGetVertexAttribIiv", (GLExtFuncPtr)&nglContext::glGetVertexAttribIiv },
/* 264 */ { "glGetVertexAttribIuiv", (GLExtFuncPtr)&nglContext::glGetVertexAttribIuiv },
/* 265 */ { "glVertexAttribI1i", (GLExtFuncPtr)&nglContext::glVertexAttribI1i },
/* 266 */ { "glVertexAttribI2i", (GLExtFuncPtr)&nglContext::glVertexAttribI2i },
/* 267 */ { "glVertexAttribI3i", (GLExtFuncPtr)&nglContext::glVertexAttribI3i },
/* 268 */ { "glVertexAttribI4i", (GLExtFuncPtr)&nglContext::glVertexAttribI4i },
/* 269 */ { "glVertexAttribI1ui", (GLExtFuncPtr)&nglContext::glVertexAttribI1ui },
/* 270 */ { "glVertexAttribI2ui", (GLExtFuncPtr)&nglContext::glVertexAttribI2ui },
/* 271 */ { "glVertexAttribI3ui", (GLExtFuncPtr)&nglContext::glVertexAttribI3ui },
/* 272 */ { "glVertexAttribI4ui", (GLExtFuncPtr)&nglContext::glVertexAttribI4ui },
/* 273 */ { "glVertexAttribI1iv", (GLExtFuncPtr)&nglContext::glVertexAttribI1iv },
/* 274 */ { "glVertexAttribI2iv", (GLExtFuncPtr)&nglContext::glVertexAttribI2iv },
/* 275 */ { "glVertexAttribI3iv", (GLExtFuncPtr)&nglContext::glVertexAttribI3iv },
/* 276 */ { "glVertexAttribI4iv", (GLExtFuncPtr)&nglContext::glVertexAttribI4iv },
/* 277 */ { "glVertexAttribI1uiv", (GLExtFuncPtr)&nglContext::glVertexAttribI1uiv },
/* 278 */ { "glVertexAttribI2uiv", (GLExtFuncPtr)&nglContext::glVertexAttribI2uiv },
/* 279 */ { "glVertexAttribI3uiv", (GLExtFuncPtr)&nglContext::glVertexAttribI3uiv },
/* 280 */ { "glVertexAttribI4uiv", (GLExtFuncPtr)&nglContext::glVertexAttribI4uiv },
/* 281 */ { "glVertexAttribI4bv", (GLExtFuncPtr)&nglContext::glVertexAttribI4bv },
/* 282 */ { "glVertexAttribI4sv", (GLExtFuncPtr)&nglContext::glVertexAttribI4sv },
/* 283 */ { "glVertexAttribI4ubv", (GLExtFuncPtr)&nglContext::glVertexAttribI4ubv },
/* 284 */ { "glVertexAttribI4usv", (GLExtFuncPtr)&nglContext::glVertexAttribI4usv },
/* 285 */ { "glGetUniformuiv", (GLExtFuncPtr)&nglContext::glGetUniformuiv },
/* 286 */ { "glBindFragDataLocation", (GLExtFuncPtr)&nglContext::glBindFragDataLocation },
/* 287 */ { "glGetFragDataLocation", (GLExtFuncPtr)&nglContext::glGetFragDataLocation },
/* 288 */ { "glUniform1ui", (GLExtFuncPtr)&nglContext::glUniform1ui },
/* 289 */ { "glUniform2ui", (GLExtFuncPtr)&nglContext::glUniform2ui },
/* 290 */ { "glUniform3ui", (GLExtFuncPtr)&nglContext::glUniform3ui },
/* 291 */ { "glUniform4ui", (GLExtFuncPtr)&nglContext::glUniform4ui },
/* 292 */ { "glUniform1uiv", (GLExtFuncPtr)&nglContext::glUniform1uiv },
/* 293 */ { "glUniform2uiv", (GLExtFuncPtr)&nglContext::glUniform2uiv },
/* 294 */ { "glUniform3uiv", (GLExtFuncPtr)&nglContext::glUniform3uiv },
/* 295 */ { "glUniform4uiv", (GLExtFuncPtr)&nglContext::glUniform4uiv },
/* 296 */ { "glTexParameterIiv", (GLExtFuncPtr)&nglContext::glTexParameterIiv },
/* 297 */ { "glTexParameterIuiv", (GLExtFuncPtr)&nglContext::glTexParameterIuiv },
/* 298 */ { "glGetTexParameterIiv", (GLExtFuncPtr)&nglContext::glGetTexParameterIiv },
/* 299 */ { "glGetTexParameterIuiv", (GLExtFuncPtr)&nglContext::glGetTexParameterIuiv },
/* 300 */ { "glClearBufferiv", (GLExtFuncPtr)&nglContext::glClearBufferiv },
/* 301 */ { "glClearBufferuiv", (GLExtFuncPtr)&nglContext::glClearBufferuiv },
/* 302 */ { "glClearBufferfv", (GLExtFuncPtr)&nglContext::glClearBufferfv },
/* 303 */ { "glClearBufferfi", (GLExtFuncPtr)&nglContext::glClearBufferfi },
/* 304 */ { "glGetStringi", (GLExtFuncPtr)&nglContext::glGetStringi },


/* GL_VERSION_3_1
 */
/* 305 */ { "glDrawArraysInstanced", (GLExtFuncPtr)&nglContext::glDrawArraysInstanced },
/* 306 */ { "glDrawElementsInstanced", (GLExtFuncPtr)&nglContext::glDrawElementsInstanced },
/* 307 */ { "glTexBuffer", (GLExtFuncPtr)&nglContext::glTexBuffer },
/* 308 */ { "glPrimitiveRestartIndex", (GLExtFuncPtr)&nglContext::glPrimitiveRestartIndex },

/* GL_VERSION_3_2
 */
/* 309 */ { "glGetInteger64i_v", (GLExtFuncPtr)&nglContext::glGetInteger64i_v },
/* 310 */ { "glGetBufferParameteri64v", (GLExtFuncPtr)&nglContext::glGetBufferParameteri64v },
/* 311 */ { "glFramebufferTexture", (GLExtFuncPtr)&nglContext::glFramebufferTexture },

/* GL_VERSION_3_3
 */
/* 312 */ { "glVertexAttribDivisor", (GLExtFuncPtr)&nglContext::glVertexAttribDivisor },

/* GL_VERSION_4_0
 */
/* 313 */ { "glMinSampleShading", (GLExtFuncPtr)&nglContext::glMinSampleShading },
/* 314 */ { "glBlendEquationi", (GLExtFuncPtr)&nglContext::glBlendEquationi },
/* 315 */ { "glBlendEquationSeparatei", (GLExtFuncPtr)&nglContext::glBlendEquationSeparatei },
/* 316 */ { "glBlendFunci", (GLExtFuncPtr)&nglContext::glBlendFunci },
/* 317 */ { "glBlendFuncSeparatei", (GLExtFuncPtr)&nglContext::glBlendFuncSeparatei },

/* GL_ARB_multitexture
 */
/* 318 */ { "glActiveTextureARB", (GLExtFuncPtr)&nglContext::glActiveTextureARB },
/* 319 */ { "glClientActiveTextureARB", (GLExtFuncPtr)&nglContext::glClientActiveTextureARB },
/* 320 */ { "glMultiTexCoord1dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1dARB },
/* 321 */ { "glMultiTexCoord1dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1dvARB },
/* 322 */ { "glMultiTexCoord1fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1fARB },
/* 323 */ { "glMultiTexCoord1fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1fvARB },
/* 324 */ { "glMultiTexCoord1iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1iARB },
/* 325 */ { "glMultiTexCoord1ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1ivARB },
/* 326 */ { "glMultiTexCoord1sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1sARB },
/* 327 */ { "glMultiTexCoord1svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1svARB },
/* 328 */ { "glMultiTexCoord2dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2dARB },
/* 329 */ { "glMultiTexCoord2dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2dvARB },
/* 330 */ { "glMultiTexCoord2fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2fARB },
/* 331 */ { "glMultiTexCoord2fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2fvARB },
/* 332 */ { "glMultiTexCoord2iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2iARB },
/* 333 */ { "glMultiTexCoord2ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2ivARB },
/* 334 */ { "glMultiTexCoord2sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2sARB },
/* 335 */ { "glMultiTexCoord2svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2svARB },
/* 336 */ { "glMultiTexCoord3dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3dARB },
/* 337 */ { "glMultiTexCoord3dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3dvARB },
/* 338 */ { "glMultiTexCoord3fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3fARB },
/* 339 */ { "glMultiTexCoord3fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3fvARB },
/* 340 */ { "glMultiTexCoord3iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3iARB },
/* 341 */ { "glMultiTexCoord3ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3ivARB },
/* 342 */ { "glMultiTexCoord3sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3sARB },
/* 343 */ { "glMultiTexCoord3svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3svARB },
/* 344 */ { "glMultiTexCoord4dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4dARB },
/* 345 */ { "glMultiTexCoord4dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4dvARB },
/* 346 */ { "glMultiTexCoord4fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4fARB },
/* 347 */ { "glMultiTexCoord4fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4fvARB },
/* 348 */ { "glMultiTexCoord4iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4iARB },
/* 349 */ { "glMultiTexCoord4ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4ivARB },
/* 350 */ { "glMultiTexCoord4sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4sARB },
/* 351 */ { "glMultiTexCoord4svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4svARB },

/* GL_ARB_transpose_matrix
 */
/* 352 */ { "glLoadTransposeMatrixfARB", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixfARB },
/* 353 */ { "glLoadTransposeMatrixdARB", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixdARB },
/* 354 */ { "glMultTransposeMatrixfARB", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixfARB },
/* 355 */ { "glMultTransposeMatrixdARB", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixdARB },

/* GL_ARB_multisample
 */
/* 356 */ { "glSampleCoverageARB", (GLExtFuncPtr)&nglContext::glSampleCoverageARB },

/* GL_ARB_texture_compression
 */
/* 357 */ { "glCompressedTexImage3DARB", (GLExtFuncPtr)&nglContext::glCompressedTexImage3DARB },
/* 358 */ { "glCompressedTexImage2DARB", (GLExtFuncPtr)&nglContext::glCompressedTexImage2DARB },
/* 359 */ { "glCompressedTexImage1DARB", (GLExtFuncPtr)&nglContext::glCompressedTexImage1DARB },
/* 360 */ { "glCompressedTexSubImage3DARB", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage3DARB },
/* 361 */ { "glCompressedTexSubImage2DARB", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage2DARB },
/* 362 */ { "glCompressedTexSubImage1DARB", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage1DARB },
/* 363 */ { "glGetCompressedTexImageARB", (GLExtFuncPtr)&nglContext::glGetCompressedTexImageARB },

/* GL_ARB_point_parameters
 */
/* 364 */ { "glPointParameterfARB", (GLExtFuncPtr)&nglContext::glPointParameterfARB },
/* 365 */ { "glPointParameterfvARB", (GLExtFuncPtr)&nglContext::glPointParameterfvARB },

/* GL_ARB_vertex_blend
 */
/* 366 */ { "glWeightbvARB", (GLExtFuncPtr)&nglContext::glWeightbvARB },
/* 367 */ { "glWeightsvARB", (GLExtFuncPtr)&nglContext::glWeightsvARB },
/* 368 */ { "glWeightivARB", (GLExtFuncPtr)&nglContext::glWeightivARB },
/* 369 */ { "glWeightfvARB", (GLExtFuncPtr)&nglContext::glWeightfvARB },
/* 370 */ { "glWeightdvARB", (GLExtFuncPtr)&nglContext::glWeightdvARB },
/* 371 */ { "glWeightubvARB", (GLExtFuncPtr)&nglContext::glWeightubvARB },
/* 372 */ { "glWeightusvARB", (GLExtFuncPtr)&nglContext::glWeightusvARB },
/* 373 */ { "glWeightuivARB", (GLExtFuncPtr)&nglContext::glWeightuivARB },
/* 374 */ { "glWeightPointerARB", (GLExtFuncPtr)&nglContext::glWeightPointerARB },
/* 375 */ { "glVertexBlendARB", (GLExtFuncPtr)&nglContext::glVertexBlendARB },

/* GL_ARB_matrix_palette
 */
/* 376 */ { "glCurrentPaletteMatrixARB", (GLExtFuncPtr)&nglContext::glCurrentPaletteMatrixARB },
/* 377 */ { "glMatrixIndexubvARB", (GLExtFuncPtr)&nglContext::glMatrixIndexubvARB },
/* 378 */ { "glMatrixIndexusvARB", (GLExtFuncPtr)&nglContext::glMatrixIndexusvARB },
/* 379 */ { "glMatrixIndexuivARB", (GLExtFuncPtr)&nglContext::glMatrixIndexuivARB },
/* 380 */ { "glMatrixIndexPointerARB", (GLExtFuncPtr)&nglContext::glMatrixIndexPointerARB },

/* GL_ARB_window_pos
 */
/* 381 */ { "glWindowPos2dARB", (GLExtFuncPtr)&nglContext::glWindowPos2dARB },
/* 382 */ { "glWindowPos2dvARB", (GLExtFuncPtr)&nglContext::glWindowPos2dvARB },
/* 383 */ { "glWindowPos2fARB", (GLExtFuncPtr)&nglContext::glWindowPos2fARB },
/* 384 */ { "glWindowPos2fvARB", (GLExtFuncPtr)&nglContext::glWindowPos2fvARB },
/* 385 */ { "glWindowPos2iARB", (GLExtFuncPtr)&nglContext::glWindowPos2iARB },
/* 386 */ { "glWindowPos2ivARB", (GLExtFuncPtr)&nglContext::glWindowPos2ivARB },
/* 387 */ { "glWindowPos2sARB", (GLExtFuncPtr)&nglContext::glWindowPos2sARB },
/* 388 */ { "glWindowPos2svARB", (GLExtFuncPtr)&nglContext::glWindowPos2svARB },
/* 389 */ { "glWindowPos3dARB", (GLExtFuncPtr)&nglContext::glWindowPos3dARB },
/* 390 */ { "glWindowPos3dvARB", (GLExtFuncPtr)&nglContext::glWindowPos3dvARB },
/* 391 */ { "glWindowPos3fARB", (GLExtFuncPtr)&nglContext::glWindowPos3fARB },
/* 392 */ { "glWindowPos3fvARB", (GLExtFuncPtr)&nglContext::glWindowPos3fvARB },
/* 393 */ { "glWindowPos3iARB", (GLExtFuncPtr)&nglContext::glWindowPos3iARB },
/* 394 */ { "glWindowPos3ivARB", (GLExtFuncPtr)&nglContext::glWindowPos3ivARB },
/* 395 */ { "glWindowPos3sARB", (GLExtFuncPtr)&nglContext::glWindowPos3sARB },
/* 396 */ { "glWindowPos3svARB", (GLExtFuncPtr)&nglContext::glWindowPos3svARB },

/* GL_ARB_vertex_program
 */
/* 397 */ { "glVertexAttrib1dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1dARB },
/* 398 */ { "glVertexAttrib1dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1dvARB },
/* 399 */ { "glVertexAttrib1fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1fARB },
/* 400 */ { "glVertexAttrib1fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1fvARB },
/* 401 */ { "glVertexAttrib1sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1sARB },
/* 402 */ { "glVertexAttrib1svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1svARB },
/* 403 */ { "glVertexAttrib2dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2dARB },
/* 404 */ { "glVertexAttrib2dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2dvARB },
/* 405 */ { "glVertexAttrib2fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2fARB },
/* 406 */ { "glVertexAttrib2fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2fvARB },
/* 407 */ { "glVertexAttrib2sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2sARB },
/* 408 */ { "glVertexAttrib2svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2svARB },
/* 409 */ { "glVertexAttrib3dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3dARB },
/* 410 */ { "glVertexAttrib3dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3dvARB },
/* 411 */ { "glVertexAttrib3fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3fARB },
/* 412 */ { "glVertexAttrib3fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3fvARB },
/* 413 */ { "glVertexAttrib3sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3sARB },
/* 414 */ { "glVertexAttrib3svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3svARB },
/* 415 */ { "glVertexAttrib4NbvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NbvARB },
/* 416 */ { "glVertexAttrib4NivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NivARB },
/* 417 */ { "glVertexAttrib4NsvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NsvARB },
/* 418 */ { "glVertexAttrib4NubARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NubARB },
/* 419 */ { "glVertexAttrib4NubvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NubvARB },
/* 420 */ { "glVertexAttrib4NuivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NuivARB },
/* 421 */ { "glVertexAttrib4NusvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NusvARB },
/* 422 */ { "glVertexAttrib4bvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4bvARB },
/* 423 */ { "glVertexAttrib4dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4dARB },
/* 424 */ { "glVertexAttrib4dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4dvARB },
/* 425 */ { "glVertexAttrib4fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4fARB },
/* 426 */ { "glVertexAttrib4fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4fvARB },
/* 427 */ { "glVertexAttrib4ivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4ivARB },
/* 428 */ { "glVertexAttrib4sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4sARB },
/* 429 */ { "glVertexAttrib4svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4svARB },
/* 430 */ { "glVertexAttrib4ubvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubvARB },
/* 431 */ { "glVertexAttrib4uivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4uivARB },
/* 432 */ { "glVertexAttrib4usvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4usvARB },
/* 433 */ { "glVertexAttribPointerARB", (GLExtFuncPtr)&nglContext::glVertexAttribPointerARB },
/* 434 */ { "glEnableVertexAttribArrayARB", (GLExtFuncPtr)&nglContext::glEnableVertexAttribArrayARB },
/* 435 */ { "glDisableVertexAttribArrayARB", (GLExtFuncPtr)&nglContext::glDisableVertexAttribArrayARB },
/* 436 */ { "glProgramStringARB", (GLExtFuncPtr)&nglContext::glProgramStringARB },
/* 437 */ { "glBindProgramARB", (GLExtFuncPtr)&nglContext::glBindProgramARB },
/* 438 */ { "glDeleteProgramsARB", (GLExtFuncPtr)&nglContext::glDeleteProgramsARB },
/* 439 */ { "glGenProgramsARB", (GLExtFuncPtr)&nglContext::glGenProgramsARB },
/* 440 */ { "glProgramEnvParameter4dARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4dARB },
/* 441 */ { "glProgramEnvParameter4dvARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4dvARB },
/* 442 */ { "glProgramEnvParameter4fARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4fARB },
/* 443 */ { "glProgramEnvParameter4fvARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4fvARB },
/* 444 */ { "glProgramLocalParameter4dARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4dARB },
/* 445 */ { "glProgramLocalParameter4dvARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4dvARB },
/* 446 */ { "glProgramLocalParameter4fARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4fARB },
/* 447 */ { "glProgramLocalParameter4fvARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4fvARB },
/* 448 */ { "glGetProgramEnvParameterdvARB", (GLExtFuncPtr)&nglContext::glGetProgramEnvParameterdvARB },
/* 449 */ { "glGetProgramEnvParameterfvARB", (GLExtFuncPtr)&nglContext::glGetProgramEnvParameterfvARB },
/* 450 */ { "glGetProgramLocalParameterdvARB", (GLExtFuncPtr)&nglContext::glGetProgramLocalParameterdvARB },
/* 451 */ { "glGetProgramLocalParameterfvARB", (GLExtFuncPtr)&nglContext::glGetProgramLocalParameterfvARB },
/* 452 */ { "glGetProgramivARB", (GLExtFuncPtr)&nglContext::glGetProgramivARB },
/* 453 */ { "glGetProgramStringARB", (GLExtFuncPtr)&nglContext::glGetProgramStringARB },
/* 454 */ { "glGetVertexAttribdvARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribdvARB },
/* 455 */ { "glGetVertexAttribfvARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribfvARB },
/* 456 */ { "glGetVertexAttribivARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribivARB },
/* 457 */ { "glGetVertexAttribPointervARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribPointervARB },
/* 458 */ { "glIsProgramARB", (GLExtFuncPtr)&nglContext::glIsProgramARB },

/* GL_ARB_vertex_buffer_object
 */
/* 459 */ { "glBindBufferARB", (GLExtFuncPtr)&nglContext::glBindBufferARB },
/* 460 */ { "glDeleteBuffersARB", (GLExtFuncPtr)&nglContext::glDeleteBuffersARB },
/* 461 */ { "glGenBuffersARB", (GLExtFuncPtr)&nglContext::glGenBuffersARB },
/* 462 */ { "glIsBufferARB", (GLExtFuncPtr)&nglContext::glIsBufferARB },
/* 463 */ { "glBufferDataARB", (GLExtFuncPtr)&nglContext::glBufferDataARB },
/* 464 */ { "glBufferSubDataARB", (GLExtFuncPtr)&nglContext::glBufferSubDataARB },
/* 465 */ { "glGetBufferSubDataARB", (GLExtFuncPtr)&nglContext::glGetBufferSubDataARB },
/* 466 */ { "glMapBufferARB", (GLExtFuncPtr)&nglContext::glMapBufferARB },
/* 467 */ { "glUnmapBufferARB", (GLExtFuncPtr)&nglContext::glUnmapBufferARB },
/* 468 */ { "glGetBufferParameterivARB", (GLExtFuncPtr)&nglContext::glGetBufferParameterivARB },
/* 469 */ { "glGetBufferPointervARB", (GLExtFuncPtr)&nglContext::glGetBufferPointervARB },

/* GL_ARB_occlusion_query
 */
/* 470 */ { "glGenQueriesARB", (GLExtFuncPtr)&nglContext::glGenQueriesARB },
/* 471 */ { "glDeleteQueriesARB", (GLExtFuncPtr)&nglContext::glDeleteQueriesARB },
/* 472 */ { "glIsQueryARB", (GLExtFuncPtr)&nglContext::glIsQueryARB },
/* 473 */ { "glBeginQueryARB", (GLExtFuncPtr)&nglContext::glBeginQueryARB },
/* 474 */ { "glEndQueryARB", (GLExtFuncPtr)&nglContext::glEndQueryARB },
/* 475 */ { "glGetQueryivARB", (GLExtFuncPtr)&nglContext::glGetQueryivARB },
/* 476 */ { "glGetQueryObjectivARB", (GLExtFuncPtr)&nglContext::glGetQueryObjectivARB },
/* 477 */ { "glGetQueryObjectuivARB", (GLExtFuncPtr)&nglContext::glGetQueryObjectuivARB },

/* GL_ARB_shader_objects
 */
/* 478 */ { "glDeleteObjectARB", (GLExtFuncPtr)&nglContext::glDeleteObjectARB },
/* 479 */ { "glGetHandleARB", (GLExtFuncPtr)&nglContext::glGetHandleARB },
/* 480 */ { "glDetachObjectARB", (GLExtFuncPtr)&nglContext::glDetachObjectARB },
/* 481 */ { "glCreateShaderObjectARB", (GLExtFuncPtr)&nglContext::glCreateShaderObjectARB },
/* 482 */ { "glShaderSourceARB", (GLExtFuncPtr)&nglContext::glShaderSourceARB },
/* 483 */ { "glCompileShaderARB", (GLExtFuncPtr)&nglContext::glCompileShaderARB },
/* 484 */ { "glCreateProgramObjectARB", (GLExtFuncPtr)&nglContext::glCreateProgramObjectARB },
/* 485 */ { "glAttachObjectARB", (GLExtFuncPtr)&nglContext::glAttachObjectARB },
/* 486 */ { "glLinkProgramARB", (GLExtFuncPtr)&nglContext::glLinkProgramARB },
/* 487 */ { "glUseProgramObjectARB", (GLExtFuncPtr)&nglContext::glUseProgramObjectARB },
/* 488 */ { "glValidateProgramARB", (GLExtFuncPtr)&nglContext::glValidateProgramARB },
/* 489 */ { "glUniform1fARB", (GLExtFuncPtr)&nglContext::glUniform1fARB },
/* 490 */ { "glUniform2fARB", (GLExtFuncPtr)&nglContext::glUniform2fARB },
/* 491 */ { "glUniform3fARB", (GLExtFuncPtr)&nglContext::glUniform3fARB },
/* 492 */ { "glUniform4fARB", (GLExtFuncPtr)&nglContext::glUniform4fARB },
/* 493 */ { "glUniform1iARB", (GLExtFuncPtr)&nglContext::glUniform1iARB },
/* 494 */ { "glUniform2iARB", (GLExtFuncPtr)&nglContext::glUniform2iARB },
/* 495 */ { "glUniform3iARB", (GLExtFuncPtr)&nglContext::glUniform3iARB },
/* 496 */ { "glUniform4iARB", (GLExtFuncPtr)&nglContext::glUniform4iARB },
/* 497 */ { "glUniform1fvARB", (GLExtFuncPtr)&nglContext::glUniform1fvARB },
/* 498 */ { "glUniform2fvARB", (GLExtFuncPtr)&nglContext::glUniform2fvARB },
/* 499 */ { "glUniform3fvARB", (GLExtFuncPtr)&nglContext::glUniform3fvARB },
/* 500 */ { "glUniform4fvARB", (GLExtFuncPtr)&nglContext::glUniform4fvARB },
/* 501 */ { "glUniform1ivARB", (GLExtFuncPtr)&nglContext::glUniform1ivARB },
/* 502 */ { "glUniform2ivARB", (GLExtFuncPtr)&nglContext::glUniform2ivARB },
/* 503 */ { "glUniform3ivARB", (GLExtFuncPtr)&nglContext::glUniform3ivARB },
/* 504 */ { "glUniform4ivARB", (GLExtFuncPtr)&nglContext::glUniform4ivARB },
/* 505 */ { "glUniformMatrix2fvARB", (GLExtFuncPtr)&nglContext::glUniformMatrix2fvARB },
/* 506 */ { "glUniformMatrix3fvARB", (GLExtFuncPtr)&nglContext::glUniformMatrix3fvARB },
/* 507 */ { "glUniformMatrix4fvARB", (GLExtFuncPtr)&nglContext::glUniformMatrix4fvARB },
/* 508 */ { "glGetObjectParameterfvARB", (GLExtFuncPtr)&nglContext::glGetObjectParameterfvARB },
/* 509 */ { "glGetObjectParameterivARB", (GLExtFuncPtr)&nglContext::glGetObjectParameterivARB },
/* 510 */ { "glGetInfoLogARB", (GLExtFuncPtr)&nglContext::glGetInfoLogARB },
/* 511 */ { "glGetAttachedObjectsARB", (GLExtFuncPtr)&nglContext::glGetAttachedObjectsARB },
/* 512 */ { "glGetUniformLocationARB", (GLExtFuncPtr)&nglContext::glGetUniformLocationARB },
/* 513 */ { "glGetActiveUniformARB", (GLExtFuncPtr)&nglContext::glGetActiveUniformARB },
/* 514 */ { "glGetUniformfvARB", (GLExtFuncPtr)&nglContext::glGetUniformfvARB },
/* 515 */ { "glGetUniformivARB", (GLExtFuncPtr)&nglContext::glGetUniformivARB },
/* 516 */ { "glGetShaderSourceARB", (GLExtFuncPtr)&nglContext::glGetShaderSourceARB },

/* GL_ARB_vertex_shader
 */
/* 517 */ { "glBindAttribLocationARB", (GLExtFuncPtr)&nglContext::glBindAttribLocationARB },
/* 518 */ { "glGetActiveAttribARB", (GLExtFuncPtr)&nglContext::glGetActiveAttribARB },
/* 519 */ { "glGetAttribLocationARB", (GLExtFuncPtr)&nglContext::glGetAttribLocationARB },

/* GL_ARB_draw_buffers
 */
/* 520 */ { "glDrawBuffersARB", (GLExtFuncPtr)&nglContext::glDrawBuffersARB },

/* GL_ARB_color_buffer_float
 */
/* 521 */ { "glClampColorARB", (GLExtFuncPtr)&nglContext::glClampColorARB },

/* GL_ARB_draw_instanced
 */
/* 522 */ { "glDrawArraysInstancedARB", (GLExtFuncPtr)&nglContext::glDrawArraysInstancedARB },
/* 523 */ { "glDrawElementsInstancedARB", (GLExtFuncPtr)&nglContext::glDrawElementsInstancedARB },

/* GL_ARB_framebuffer_object
 */
/* 524 */ { "glIsRenderbuffer", (GLExtFuncPtr)&nglContext::glIsRenderbuffer },
/* 525 */ { "glBindRenderbuffer", (GLExtFuncPtr)&nglContext::glBindRenderbuffer },
/* 526 */ { "glDeleteRenderbuffers", (GLExtFuncPtr)&nglContext::glDeleteRenderbuffers },
/* 527 */ { "glGenRenderbuffers", (GLExtFuncPtr)&nglContext::glGenRenderbuffers },
/* 528 */ { "glRenderbufferStorage", (GLExtFuncPtr)&nglContext::glRenderbufferStorage },
/* 529 */ { "glGetRenderbufferParameteriv", (GLExtFuncPtr)&nglContext::glGetRenderbufferParameteriv },
/* 530 */ { "glIsFramebuffer", (GLExtFuncPtr)&nglContext::glIsFramebuffer },
/* 531 */ { "glBindFramebuffer", (GLExtFuncPtr)&nglContext::glBindFramebuffer },
/* 532 */ { "glDeleteFramebuffers", (GLExtFuncPtr)&nglContext::glDeleteFramebuffers },
/* 533 */ { "glGenFramebuffers", (GLExtFuncPtr)&nglContext::glGenFramebuffers },
/* 534 */ { "glCheckFramebufferStatus", (GLExtFuncPtr)&nglContext::glCheckFramebufferStatus },
/* 535 */ { "glFramebufferTexture1D", (GLExtFuncPtr)&nglContext::glFramebufferTexture1D },
/* 536 */ { "glFramebufferTexture2D", (GLExtFuncPtr)&nglContext::glFramebufferTexture2D },
/* 537 */ { "glFramebufferTexture3D", (GLExtFuncPtr)&nglContext::glFramebufferTexture3D },
/* 538 */ { "glFramebufferRenderbuffer", (GLExtFuncPtr)&nglContext::glFramebufferRenderbuffer },
/* 539 */ { "glGetFramebufferAttachmentParameteriv", (GLExtFuncPtr)&nglContext::glGetFramebufferAttachmentParameteriv },
/* 540 */ { "glGenerateMipmap", (GLExtFuncPtr)&nglContext::glGenerateMipmap },
/* 541 */ { "glBlitFramebuffer", (GLExtFuncPtr)&nglContext::glBlitFramebuffer },
/* 542 */ { "glRenderbufferStorageMultisample", (GLExtFuncPtr)&nglContext::glRenderbufferStorageMultisample },
/* 543 */ { "glFramebufferTextureLayer", (GLExtFuncPtr)&nglContext::glFramebufferTextureLayer },

/* GL_ARB_geometry_shader4
 */
/* 544 */ { "glProgramParameteriARB", (GLExtFuncPtr)&nglContext::glProgramParameteriARB },
/* 545 */ { "glFramebufferTextureARB", (GLExtFuncPtr)&nglContext::glFramebufferTextureARB },
/* 546 */ { "glFramebufferTextureLayerARB", (GLExtFuncPtr)&nglContext::glFramebufferTextureLayerARB },
/* 547 */ { "glFramebufferTextureFaceARB", (GLExtFuncPtr)&nglContext::glFramebufferTextureFaceARB },

/* GL_ARB_instanced_arrays
 */
/* 548 */ { "glVertexAttribDivisorARB", (GLExtFuncPtr)&nglContext::glVertexAttribDivisorARB },

/* GL_ARB_map_buffer_range
 */
/* 549 */ { "glMapBufferRange", (GLExtFuncPtr)&nglContext::glMapBufferRange },
/* 550 */ { "glFlushMappedBufferRange", (GLExtFuncPtr)&nglContext::glFlushMappedBufferRange },

/* GL_ARB_texture_buffer_object
 */
/* 551 */ { "glTexBufferARB", (GLExtFuncPtr)&nglContext::glTexBufferARB },

/* GL_ARB_vertex_array_object
 */
/* 552 */ { "glBindVertexArray", (GLExtFuncPtr)&nglContext::glBindVertexArray },
/* 553 */ { "glDeleteVertexArrays", (GLExtFuncPtr)&nglContext::glDeleteVertexArrays },
/* 554 */ { "glGenVertexArrays", (GLExtFuncPtr)&nglContext::glGenVertexArrays },
/* 555 */ { "glIsVertexArray", (GLExtFuncPtr)&nglContext::glIsVertexArray },

/* GL_ARB_uniform_buffer_object
 */
/* 556 */ { "glGetUniformIndices", (GLExtFuncPtr)&nglContext::glGetUniformIndices },
/* 557 */ { "glGetActiveUniformsiv", (GLExtFuncPtr)&nglContext::glGetActiveUniformsiv },
/* 558 */ { "glGetActiveUniformName", (GLExtFuncPtr)&nglContext::glGetActiveUniformName },
/* 559 */ { "glGetUniformBlockIndex", (GLExtFuncPtr)&nglContext::glGetUniformBlockIndex },
/* 560 */ { "glGetActiveUniformBlockiv", (GLExtFuncPtr)&nglContext::glGetActiveUniformBlockiv },
/* 561 */ { "glGetActiveUniformBlockName", (GLExtFuncPtr)&nglContext::glGetActiveUniformBlockName },
/* 562 */ { "glUniformBlockBinding", (GLExtFuncPtr)&nglContext::glUniformBlockBinding },

/* GL_ARB_copy_buffer
 */
/* 563 */ { "glCopyBufferSubData", (GLExtFuncPtr)&nglContext::glCopyBufferSubData },

/* GL_ARB_draw_elements_base_vertex
 */
/* 564 */ { "glDrawElementsBaseVertex", (GLExtFuncPtr)&nglContext::glDrawElementsBaseVertex },
/* 565 */ { "glDrawRangeElementsBaseVertex", (GLExtFuncPtr)&nglContext::glDrawRangeElementsBaseVertex },
/* 566 */ { "glDrawElementsInstancedBaseVertex", (GLExtFuncPtr)&nglContext::glDrawElementsInstancedBaseVertex },
/* 567 */ { "glMultiDrawElementsBaseVertex", (GLExtFuncPtr)&nglContext::glMultiDrawElementsBaseVertex },

/* GL_ARB_provoking_vertex
 */
/* 568 */ { "glProvokingVertex", (GLExtFuncPtr)&nglContext::glProvokingVertex },

/* GL_ARB_sync
 */
/* 569 */ { "glFenceSync", (GLExtFuncPtr)&nglContext::glFenceSync },
/* 570 */ { "glIsSync", (GLExtFuncPtr)&nglContext::glIsSync },
/* 571 */ { "glDeleteSync", (GLExtFuncPtr)&nglContext::glDeleteSync },
/* 572 */ { "glClientWaitSync", (GLExtFuncPtr)&nglContext::glClientWaitSync },
/* 573 */ { "glWaitSync", (GLExtFuncPtr)&nglContext::glWaitSync },
/* 574 */ { "glGetInteger64v", (GLExtFuncPtr)&nglContext::glGetInteger64v },
/* 575 */ { "glGetSynciv", (GLExtFuncPtr)&nglContext::glGetSynciv },

/* GL_ARB_texture_multisample
 */
/* 576 */ { "glTexImage2DMultisample", (GLExtFuncPtr)&nglContext::glTexImage2DMultisample },
/* 577 */ { "glTexImage3DMultisample", (GLExtFuncPtr)&nglContext::glTexImage3DMultisample },
/* 578 */ { "glGetMultisamplefv", (GLExtFuncPtr)&nglContext::glGetMultisamplefv },
/* 579 */ { "glSampleMaski", (GLExtFuncPtr)&nglContext::glSampleMaski },

/* GL_ARB_draw_buffers_blend
 */
/* 580 */ { "glBlendEquationiARB", (GLExtFuncPtr)&nglContext::glBlendEquationiARB },
/* 581 */ { "glBlendEquationSeparateiARB", (GLExtFuncPtr)&nglContext::glBlendEquationSeparateiARB },
/* 582 */ { "glBlendFunciARB", (GLExtFuncPtr)&nglContext::glBlendFunciARB },
/* 583 */ { "glBlendFuncSeparateiARB", (GLExtFuncPtr)&nglContext::glBlendFuncSeparateiARB },

/* GL_ARB_sample_shading
 */
/* 584 */ { "glMinSampleShadingARB", (GLExtFuncPtr)&nglContext::glMinSampleShadingARB },

/* GL_ARB_shading_language_include
 */
/* 585 */ { "glNamedStringARB", (GLExtFuncPtr)&nglContext::glNamedStringARB },
/* 586 */ { "glDeleteNamedStringARB", (GLExtFuncPtr)&nglContext::glDeleteNamedStringARB },
/* 587 */ { "glCompileShaderIncludeARB", (GLExtFuncPtr)&nglContext::glCompileShaderIncludeARB },
/* 588 */ { "glIsNamedStringARB", (GLExtFuncPtr)&nglContext::glIsNamedStringARB },
/* 589 */ { "glGetNamedStringARB", (GLExtFuncPtr)&nglContext::glGetNamedStringARB },
/* 590 */ { "glGetNamedStringivARB", (GLExtFuncPtr)&nglContext::glGetNamedStringivARB },

/* GL_ARB_blend_func_extended
 */
/* 591 */ { "glBindFragDataLocationIndexed", (GLExtFuncPtr)&nglContext::glBindFragDataLocationIndexed },
/* 592 */ { "glGetFragDataIndex", (GLExtFuncPtr)&nglContext::glGetFragDataIndex },

/* GL_ARB_sampler_objects
 */
/* 593 */ { "glGenSamplers", (GLExtFuncPtr)&nglContext::glGenSamplers },
/* 594 */ { "glDeleteSamplers", (GLExtFuncPtr)&nglContext::glDeleteSamplers },
/* 595 */ { "glIsSampler", (GLExtFuncPtr)&nglContext::glIsSampler },
/* 596 */ { "glBindSampler", (GLExtFuncPtr)&nglContext::glBindSampler },
/* 597 */ { "glSamplerParameteri", (GLExtFuncPtr)&nglContext::glSamplerParameteri },
/* 598 */ { "glSamplerParameteriv", (GLExtFuncPtr)&nglContext::glSamplerParameteriv },
/* 599 */ { "glSamplerParameterf", (GLExtFuncPtr)&nglContext::glSamplerParameterf },
/* 600 */ { "glSamplerParameterfv", (GLExtFuncPtr)&nglContext::glSamplerParameterfv },
/* 601 */ { "glSamplerParameterIiv", (GLExtFuncPtr)&nglContext::glSamplerParameterIiv },
/* 602 */ { "glSamplerParameterIuiv", (GLExtFuncPtr)&nglContext::glSamplerParameterIuiv },
/* 603 */ { "glGetSamplerParameteriv", (GLExtFuncPtr)&nglContext::glGetSamplerParameteriv },
/* 604 */ { "glGetSamplerParameterIiv", (GLExtFuncPtr)&nglContext::glGetSamplerParameterIiv },
/* 605 */ { "glGetSamplerParameterfv", (GLExtFuncPtr)&nglContext::glGetSamplerParameterfv },
/* 606 */ { "glGetSamplerParameterIuiv", (GLExtFuncPtr)&nglContext::glGetSamplerParameterIuiv },

/* GL_ARB_timer_query
 */
/* 607 */ { "glQueryCounter", (GLExtFuncPtr)&nglContext::glQueryCounter },
/* 608 */ { "glGetQueryObjecti64v", (GLExtFuncPtr)&nglContext::glGetQueryObjecti64v },
/* 609 */ { "glGetQueryObjectui64v", (GLExtFuncPtr)&nglContext::glGetQueryObjectui64v },

/* GL_ARB_vertex_type_2_10_10_10_rev
 */
/* 610 */ { "glVertexP2ui", (GLExtFuncPtr)&nglContext::glVertexP2ui },
/* 611 */ { "glVertexP2uiv", (GLExtFuncPtr)&nglContext::glVertexP2uiv },
/* 612 */ { "glVertexP3ui", (GLExtFuncPtr)&nglContext::glVertexP3ui },
/* 613 */ { "glVertexP3uiv", (GLExtFuncPtr)&nglContext::glVertexP3uiv },
/* 614 */ { "glVertexP4ui", (GLExtFuncPtr)&nglContext::glVertexP4ui },
/* 615 */ { "glVertexP4uiv", (GLExtFuncPtr)&nglContext::glVertexP4uiv },
/* 616 */ { "glTexCoordP1ui", (GLExtFuncPtr)&nglContext::glTexCoordP1ui },
/* 617 */ { "glTexCoordP1uiv", (GLExtFuncPtr)&nglContext::glTexCoordP1uiv },
/* 618 */ { "glTexCoordP2ui", (GLExtFuncPtr)&nglContext::glTexCoordP2ui },
/* 619 */ { "glTexCoordP2uiv", (GLExtFuncPtr)&nglContext::glTexCoordP2uiv },
/* 620 */ { "glTexCoordP3ui", (GLExtFuncPtr)&nglContext::glTexCoordP3ui },
/* 621 */ { "glTexCoordP3uiv", (GLExtFuncPtr)&nglContext::glTexCoordP3uiv },
/* 622 */ { "glTexCoordP4ui", (GLExtFuncPtr)&nglContext::glTexCoordP4ui },
/* 623 */ { "glTexCoordP4uiv", (GLExtFuncPtr)&nglContext::glTexCoordP4uiv },
/* 624 */ { "glMultiTexCoordP1ui", (GLExtFuncPtr)&nglContext::glMultiTexCoordP1ui },
/* 625 */ { "glMultiTexCoordP1uiv", (GLExtFuncPtr)&nglContext::glMultiTexCoordP1uiv },
/* 626 */ { "glMultiTexCoordP2ui", (GLExtFuncPtr)&nglContext::glMultiTexCoordP2ui },
/* 627 */ { "glMultiTexCoordP2uiv", (GLExtFuncPtr)&nglContext::glMultiTexCoordP2uiv },
/* 628 */ { "glMultiTexCoordP3ui", (GLExtFuncPtr)&nglContext::glMultiTexCoordP3ui },
/* 629 */ { "glMultiTexCoordP3uiv", (GLExtFuncPtr)&nglContext::glMultiTexCoordP3uiv },
/* 630 */ { "glMultiTexCoordP4ui", (GLExtFuncPtr)&nglContext::glMultiTexCoordP4ui },
/* 631 */ { "glMultiTexCoordP4uiv", (GLExtFuncPtr)&nglContext::glMultiTexCoordP4uiv },
/* 632 */ { "glNormalP3ui", (GLExtFuncPtr)&nglContext::glNormalP3ui },
/* 633 */ { "glNormalP3uiv", (GLExtFuncPtr)&nglContext::glNormalP3uiv },
/* 634 */ { "glColorP3ui", (GLExtFuncPtr)&nglContext::glColorP3ui },
/* 635 */ { "glColorP3uiv", (GLExtFuncPtr)&nglContext::glColorP3uiv },
/* 636 */ { "glColorP4ui", (GLExtFuncPtr)&nglContext::glColorP4ui },
/* 637 */ { "glColorP4uiv", (GLExtFuncPtr)&nglContext::glColorP4uiv },
/* 638 */ { "glSecondaryColorP3ui", (GLExtFuncPtr)&nglContext::glSecondaryColorP3ui },
/* 639 */ { "glSecondaryColorP3uiv", (GLExtFuncPtr)&nglContext::glSecondaryColorP3uiv },
/* 640 */ { "glVertexAttribP1ui", (GLExtFuncPtr)&nglContext::glVertexAttribP1ui },
/* 641 */ { "glVertexAttribP1uiv", (GLExtFuncPtr)&nglContext::glVertexAttribP1uiv },
/* 642 */ { "glVertexAttribP2ui", (GLExtFuncPtr)&nglContext::glVertexAttribP2ui },
/* 643 */ { "glVertexAttribP2uiv", (GLExtFuncPtr)&nglContext::glVertexAttribP2uiv },
/* 644 */ { "glVertexAttribP3ui", (GLExtFuncPtr)&nglContext::glVertexAttribP3ui },
/* 645 */ { "glVertexAttribP3uiv", (GLExtFuncPtr)&nglContext::glVertexAttribP3uiv },
/* 646 */ { "glVertexAttribP4ui", (GLExtFuncPtr)&nglContext::glVertexAttribP4ui },
/* 647 */ { "glVertexAttribP4uiv", (GLExtFuncPtr)&nglContext::glVertexAttribP4uiv },

/* GL_ARB_draw_indirect
 */
/* 648 */ { "glDrawArraysIndirect", (GLExtFuncPtr)&nglContext::glDrawArraysIndirect },
/* 649 */ { "glDrawElementsIndirect", (GLExtFuncPtr)&nglContext::glDrawElementsIndirect },

/* GL_ARB_gpu_shader_fp64
 */
/* 650 */ { "glUniform1d", (GLExtFuncPtr)&nglContext::glUniform1d },
/* 651 */ { "glUniform2d", (GLExtFuncPtr)&nglContext::glUniform2d },
/* 652 */ { "glUniform3d", (GLExtFuncPtr)&nglContext::glUniform3d },
/* 653 */ { "glUniform4d", (GLExtFuncPtr)&nglContext::glUniform4d },
/* 654 */ { "glUniform1dv", (GLExtFuncPtr)&nglContext::glUniform1dv },
/* 655 */ { "glUniform2dv", (GLExtFuncPtr)&nglContext::glUniform2dv },
/* 656 */ { "glUniform3dv", (GLExtFuncPtr)&nglContext::glUniform3dv },
/* 657 */ { "glUniform4dv", (GLExtFuncPtr)&nglContext::glUniform4dv },
/* 658 */ { "glUniformMatrix2dv", (GLExtFuncPtr)&nglContext::glUniformMatrix2dv },
/* 659 */ { "glUniformMatrix3dv", (GLExtFuncPtr)&nglContext::glUniformMatrix3dv },
/* 660 */ { "glUniformMatrix4dv", (GLExtFuncPtr)&nglContext::glUniformMatrix4dv },
/* 661 */ { "glUniformMatrix2x3dv", (GLExtFuncPtr)&nglContext::glUniformMatrix2x3dv },
/* 662 */ { "glUniformMatrix2x4dv", (GLExtFuncPtr)&nglContext::glUniformMatrix2x4dv },
/* 663 */ { "glUniformMatrix3x2dv", (GLExtFuncPtr)&nglContext::glUniformMatrix3x2dv },
/* 664 */ { "glUniformMatrix3x4dv", (GLExtFuncPtr)&nglContext::glUniformMatrix3x4dv },
/* 665 */ { "glUniformMatrix4x2dv", (GLExtFuncPtr)&nglContext::glUniformMatrix4x2dv },
/* 666 */ { "glUniformMatrix4x3dv", (GLExtFuncPtr)&nglContext::glUniformMatrix4x3dv },
/* 667 */ { "glGetUniformdv", (GLExtFuncPtr)&nglContext::glGetUniformdv },

/* GL_ARB_shader_subroutine
 */
/* 668 */ { "glGetSubroutineUniformLocation", (GLExtFuncPtr)&nglContext::glGetSubroutineUniformLocation },
/* 669 */ { "glGetSubroutineIndex", (GLExtFuncPtr)&nglContext::glGetSubroutineIndex },
/* 670 */ { "glGetActiveSubroutineUniformiv", (GLExtFuncPtr)&nglContext::glGetActiveSubroutineUniformiv },
/* 671 */ { "glGetActiveSubroutineUniformName", (GLExtFuncPtr)&nglContext::glGetActiveSubroutineUniformName },
/* 672 */ { "glGetActiveSubroutineName", (GLExtFuncPtr)&nglContext::glGetActiveSubroutineName },
/* 673 */ { "glUniformSubroutinesuiv", (GLExtFuncPtr)&nglContext::glUniformSubroutinesuiv },
/* 674 */ { "glGetUniformSubroutineuiv", (GLExtFuncPtr)&nglContext::glGetUniformSubroutineuiv },
/* 675 */ { "glGetProgramStageiv", (GLExtFuncPtr)&nglContext::glGetProgramStageiv },

/* GL_ARB_tessellation_shader
 */
/* 676 */ { "glPatchParameteri", (GLExtFuncPtr)&nglContext::glPatchParameteri },
/* 677 */ { "glPatchParameterfv", (GLExtFuncPtr)&nglContext::glPatchParameterfv },

/* GL_ARB_transform_feedback2
 */
/* 678 */ { "glBindTransformFeedback", (GLExtFuncPtr)&nglContext::glBindTransformFeedback },
/* 679 */ { "glDeleteTransformFeedbacks", (GLExtFuncPtr)&nglContext::glDeleteTransformFeedbacks },
/* 680 */ { "glGenTransformFeedbacks", (GLExtFuncPtr)&nglContext::glGenTransformFeedbacks },
/* 681 */ { "glIsTransformFeedback", (GLExtFuncPtr)&nglContext::glIsTransformFeedback },
/* 682 */ { "glPauseTransformFeedback", (GLExtFuncPtr)&nglContext::glPauseTransformFeedback },
/* 683 */ { "glResumeTransformFeedback", (GLExtFuncPtr)&nglContext::glResumeTransformFeedback },
/* 684 */ { "glDrawTransformFeedback", (GLExtFuncPtr)&nglContext::glDrawTransformFeedback },

/* GL_ARB_transform_feedback3
 */
/* 685 */ { "glDrawTransformFeedbackStream", (GLExtFuncPtr)&nglContext::glDrawTransformFeedbackStream },
/* 686 */ { "glBeginQueryIndexed", (GLExtFuncPtr)&nglContext::glBeginQueryIndexed },
/* 687 */ { "glEndQueryIndexed", (GLExtFuncPtr)&nglContext::glEndQueryIndexed },
/* 688 */ { "glGetQueryIndexediv", (GLExtFuncPtr)&nglContext::glGetQueryIndexediv },

/* GL_ARB_ES2_compatibility
 */
/* 689 */ { "glReleaseShaderCompiler", (GLExtFuncPtr)&nglContext::glReleaseShaderCompiler },
/* 690 */ { "glShaderBinary", (GLExtFuncPtr)&nglContext::glShaderBinary },
/* 691 */ { "glGetShaderPrecisionFormat", (GLExtFuncPtr)&nglContext::glGetShaderPrecisionFormat },
/* 692 */ { "glDepthRangef", (GLExtFuncPtr)&nglContext::glDepthRangef },
/* 693 */ { "glClearDepthf", (GLExtFuncPtr)&nglContext::glClearDepthf },

/* GL_ARB_get_program_binary
 */
/* 694 */ { "glGetProgramBinary", (GLExtFuncPtr)&nglContext::glGetProgramBinary },
/* 695 */ { "glProgramBinary", (GLExtFuncPtr)&nglContext::glProgramBinary },
/* 696 */ { "glProgramParameteri", (GLExtFuncPtr)&nglContext::glProgramParameteri },

/* GL_ARB_separate_shader_objects
 */
/* 697 */ { "glUseProgramStages", (GLExtFuncPtr)&nglContext::glUseProgramStages },
/* 698 */ { "glActiveShaderProgram", (GLExtFuncPtr)&nglContext::glActiveShaderProgram },
/* 699 */ { "glCreateShaderProgramv", (GLExtFuncPtr)&nglContext::glCreateShaderProgramv },
/* 700 */ { "glBindProgramPipeline", (GLExtFuncPtr)&nglContext::glBindProgramPipeline },
/* 701 */ { "glDeleteProgramPipelines", (GLExtFuncPtr)&nglContext::glDeleteProgramPipelines },
/* 702 */ { "glGenProgramPipelines", (GLExtFuncPtr)&nglContext::glGenProgramPipelines },
/* 703 */ { "glIsProgramPipeline", (GLExtFuncPtr)&nglContext::glIsProgramPipeline },
/* 704 */ { "glGetProgramPipelineiv", (GLExtFuncPtr)&nglContext::glGetProgramPipelineiv },
/* 705 */ { "glProgramUniform1i", (GLExtFuncPtr)&nglContext::glProgramUniform1i },
/* 706 */ { "glProgramUniform1iv", (GLExtFuncPtr)&nglContext::glProgramUniform1iv },
/* 707 */ { "glProgramUniform1f", (GLExtFuncPtr)&nglContext::glProgramUniform1f },
/* 708 */ { "glProgramUniform1fv", (GLExtFuncPtr)&nglContext::glProgramUniform1fv },
/* 709 */ { "glProgramUniform1d", (GLExtFuncPtr)&nglContext::glProgramUniform1d },
/* 710 */ { "glProgramUniform1dv", (GLExtFuncPtr)&nglContext::glProgramUniform1dv },
/* 711 */ { "glProgramUniform1ui", (GLExtFuncPtr)&nglContext::glProgramUniform1ui },
/* 712 */ { "glProgramUniform1uiv", (GLExtFuncPtr)&nglContext::glProgramUniform1uiv },
/* 713 */ { "glProgramUniform2i", (GLExtFuncPtr)&nglContext::glProgramUniform2i },
/* 714 */ { "glProgramUniform2iv", (GLExtFuncPtr)&nglContext::glProgramUniform2iv },
/* 715 */ { "glProgramUniform2f", (GLExtFuncPtr)&nglContext::glProgramUniform2f },
/* 716 */ { "glProgramUniform2fv", (GLExtFuncPtr)&nglContext::glProgramUniform2fv },
/* 717 */ { "glProgramUniform2d", (GLExtFuncPtr)&nglContext::glProgramUniform2d },
/* 718 */ { "glProgramUniform2dv", (GLExtFuncPtr)&nglContext::glProgramUniform2dv },
/* 719 */ { "glProgramUniform2ui", (GLExtFuncPtr)&nglContext::glProgramUniform2ui },
/* 720 */ { "glProgramUniform2uiv", (GLExtFuncPtr)&nglContext::glProgramUniform2uiv },
/* 721 */ { "glProgramUniform3i", (GLExtFuncPtr)&nglContext::glProgramUniform3i },
/* 722 */ { "glProgramUniform3iv", (GLExtFuncPtr)&nglContext::glProgramUniform3iv },
/* 723 */ { "glProgramUniform3f", (GLExtFuncPtr)&nglContext::glProgramUniform3f },
/* 724 */ { "glProgramUniform3fv", (GLExtFuncPtr)&nglContext::glProgramUniform3fv },
/* 725 */ { "glProgramUniform3d", (GLExtFuncPtr)&nglContext::glProgramUniform3d },
/* 726 */ { "glProgramUniform3dv", (GLExtFuncPtr)&nglContext::glProgramUniform3dv },
/* 727 */ { "glProgramUniform3ui", (GLExtFuncPtr)&nglContext::glProgramUniform3ui },
/* 728 */ { "glProgramUniform3uiv", (GLExtFuncPtr)&nglContext::glProgramUniform3uiv },
/* 729 */ { "glProgramUniform4i", (GLExtFuncPtr)&nglContext::glProgramUniform4i },
/* 730 */ { "glProgramUniform4iv", (GLExtFuncPtr)&nglContext::glProgramUniform4iv },
/* 731 */ { "glProgramUniform4f", (GLExtFuncPtr)&nglContext::glProgramUniform4f },
/* 732 */ { "glProgramUniform4fv", (GLExtFuncPtr)&nglContext::glProgramUniform4fv },
/* 733 */ { "glProgramUniform4d", (GLExtFuncPtr)&nglContext::glProgramUniform4d },
/* 734 */ { "glProgramUniform4dv", (GLExtFuncPtr)&nglContext::glProgramUniform4dv },
/* 735 */ { "glProgramUniform4ui", (GLExtFuncPtr)&nglContext::glProgramUniform4ui },
/* 736 */ { "glProgramUniform4uiv", (GLExtFuncPtr)&nglContext::glProgramUniform4uiv },
/* 737 */ { "glProgramUniformMatrix2fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2fv },
/* 738 */ { "glProgramUniformMatrix3fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3fv },
/* 739 */ { "glProgramUniformMatrix4fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4fv },
/* 740 */ { "glProgramUniformMatrix2dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2dv },
/* 741 */ { "glProgramUniformMatrix3dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3dv },
/* 742 */ { "glProgramUniformMatrix4dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4dv },
/* 743 */ { "glProgramUniformMatrix2x3fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x3fv },
/* 744 */ { "glProgramUniformMatrix3x2fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x2fv },
/* 745 */ { "glProgramUniformMatrix2x4fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x4fv },
/* 746 */ { "glProgramUniformMatrix4x2fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x2fv },
/* 747 */ { "glProgramUniformMatrix3x4fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x4fv },
/* 748 */ { "glProgramUniformMatrix4x3fv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x3fv },
/* 749 */ { "glProgramUniformMatrix2x3dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x3dv },
/* 750 */ { "glProgramUniformMatrix3x2dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x2dv },
/* 751 */ { "glProgramUniformMatrix2x4dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x4dv },
/* 752 */ { "glProgramUniformMatrix4x2dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x2dv },
/* 753 */ { "glProgramUniformMatrix3x4dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x4dv },
/* 754 */ { "glProgramUniformMatrix4x3dv", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x3dv },
/* 755 */ { "glValidateProgramPipeline", (GLExtFuncPtr)&nglContext::glValidateProgramPipeline },
/* 756 */ { "glGetProgramPipelineInfoLog", (GLExtFuncPtr)&nglContext::glGetProgramPipelineInfoLog },

/* GL_ARB_vertex_attrib_64bit
 */
/* 757 */ { "glVertexAttribL1d", (GLExtFuncPtr)&nglContext::glVertexAttribL1d },
/* 758 */ { "glVertexAttribL2d", (GLExtFuncPtr)&nglContext::glVertexAttribL2d },
/* 759 */ { "glVertexAttribL3d", (GLExtFuncPtr)&nglContext::glVertexAttribL3d },
/* 760 */ { "glVertexAttribL4d", (GLExtFuncPtr)&nglContext::glVertexAttribL4d },
/* 761 */ { "glVertexAttribL1dv", (GLExtFuncPtr)&nglContext::glVertexAttribL1dv },
/* 762 */ { "glVertexAttribL2dv", (GLExtFuncPtr)&nglContext::glVertexAttribL2dv },
/* 763 */ { "glVertexAttribL3dv", (GLExtFuncPtr)&nglContext::glVertexAttribL3dv },
/* 764 */ { "glVertexAttribL4dv", (GLExtFuncPtr)&nglContext::glVertexAttribL4dv },
/* 765 */ { "glVertexAttribLPointer", (GLExtFuncPtr)&nglContext::glVertexAttribLPointer },
/* 766 */ { "glGetVertexAttribLdv", (GLExtFuncPtr)&nglContext::glGetVertexAttribLdv },

/* GL_ARB_viewport_array
 */
/* 767 */ { "glViewportArrayv", (GLExtFuncPtr)&nglContext::glViewportArrayv },
/* 768 */ { "glViewportIndexedf", (GLExtFuncPtr)&nglContext::glViewportIndexedf },
/* 769 */ { "glViewportIndexedfv", (GLExtFuncPtr)&nglContext::glViewportIndexedfv },
/* 770 */ { "glScissorArrayv", (GLExtFuncPtr)&nglContext::glScissorArrayv },
/* 771 */ { "glScissorIndexed", (GLExtFuncPtr)&nglContext::glScissorIndexed },
/* 772 */ { "glScissorIndexedv", (GLExtFuncPtr)&nglContext::glScissorIndexedv },
/* 773 */ { "glDepthRangeArrayv", (GLExtFuncPtr)&nglContext::glDepthRangeArrayv },
/* 774 */ { "glDepthRangeIndexed", (GLExtFuncPtr)&nglContext::glDepthRangeIndexed },
/* 775 */ { "glGetFloati_v", (GLExtFuncPtr)&nglContext::glGetFloati_v },
/* 776 */ { "glGetDoublei_v", (GLExtFuncPtr)&nglContext::glGetDoublei_v },

/* GL_ARB_cl_event
 */
/* 777 */ { "glCreateSyncFromCLeventARB", (GLExtFuncPtr)&nglContext::glCreateSyncFromCLeventARB },

/* GL_ARB_debug_output
 */
/* 778 */ { "glDebugMessageControlARB", (GLExtFuncPtr)&nglContext::glDebugMessageControlARB },
/* 779 */ { "glDebugMessageInsertARB", (GLExtFuncPtr)&nglContext::glDebugMessageInsertARB },
/* 780 */ { "glDebugMessageCallbackARB", (GLExtFuncPtr)&nglContext::glDebugMessageCallbackARB },
/* 781 */ { "glGetDebugMessageLogARB", (GLExtFuncPtr)&nglContext::glGetDebugMessageLogARB },

/* GL_ARB_robustness
 */
/* 782 */ { "glGetGraphicsResetStatusARB", (GLExtFuncPtr)&nglContext::glGetGraphicsResetStatusARB },
/* 783 */ { "glGetnMapdvARB", (GLExtFuncPtr)&nglContext::glGetnMapdvARB },
/* 784 */ { "glGetnMapfvARB", (GLExtFuncPtr)&nglContext::glGetnMapfvARB },
/* 785 */ { "glGetnMapivARB", (GLExtFuncPtr)&nglContext::glGetnMapivARB },
/* 786 */ { "glGetnPixelMapfvARB", (GLExtFuncPtr)&nglContext::glGetnPixelMapfvARB },
/* 787 */ { "glGetnPixelMapuivARB", (GLExtFuncPtr)&nglContext::glGetnPixelMapuivARB },
/* 788 */ { "glGetnPixelMapusvARB", (GLExtFuncPtr)&nglContext::glGetnPixelMapusvARB },
/* 789 */ { "glGetnPolygonStippleARB", (GLExtFuncPtr)&nglContext::glGetnPolygonStippleARB },
/* 790 */ { "glGetnColorTableARB", (GLExtFuncPtr)&nglContext::glGetnColorTableARB },
/* 791 */ { "glGetnConvolutionFilterARB", (GLExtFuncPtr)&nglContext::glGetnConvolutionFilterARB },
/* 792 */ { "glGetnSeparableFilterARB", (GLExtFuncPtr)&nglContext::glGetnSeparableFilterARB },
/* 793 */ { "glGetnHistogramARB", (GLExtFuncPtr)&nglContext::glGetnHistogramARB },
/* 794 */ { "glGetnMinmaxARB", (GLExtFuncPtr)&nglContext::glGetnMinmaxARB },
/* 795 */ { "glGetnTexImageARB", (GLExtFuncPtr)&nglContext::glGetnTexImageARB },
/* 796 */ { "glReadnPixelsARB", (GLExtFuncPtr)&nglContext::glReadnPixelsARB },
/* 797 */ { "glGetnCompressedTexImageARB", (GLExtFuncPtr)&nglContext::glGetnCompressedTexImageARB },
/* 798 */ { "glGetnUniformfvARB", (GLExtFuncPtr)&nglContext::glGetnUniformfvARB },
/* 799 */ { "glGetnUniformivARB", (GLExtFuncPtr)&nglContext::glGetnUniformivARB },
/* 800 */ { "glGetnUniformuivARB", (GLExtFuncPtr)&nglContext::glGetnUniformuivARB },
/* 801 */ { "glGetnUniformdvARB", (GLExtFuncPtr)&nglContext::glGetnUniformdvARB },

/* GL_EXT_blend_color
 */
/* 802 */ { "glBlendColorEXT", (GLExtFuncPtr)&nglContext::glBlendColorEXT },

/* GL_EXT_polygon_offset
 */
/* 803 */ { "glPolygonOffsetEXT", (GLExtFuncPtr)&nglContext::glPolygonOffsetEXT },

/* GL_EXT_texture3D
 */
/* 804 */ { "glTexImage3DEXT", (GLExtFuncPtr)&nglContext::glTexImage3DEXT },
/* 805 */ { "glTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glTexSubImage3DEXT },

/* GL_SGIS_texture_filter4
 */
/* 806 */ { "glGetTexFilterFuncSGIS", (GLExtFuncPtr)&nglContext::glGetTexFilterFuncSGIS },
/* 807 */ { "glTexFilterFuncSGIS", (GLExtFuncPtr)&nglContext::glTexFilterFuncSGIS },

/* GL_EXT_subtexture
 */
/* 808 */ { "glTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glTexSubImage1DEXT },
/* 809 */ { "glTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glTexSubImage2DEXT },

/* GL_EXT_copy_texture
 */
/* 810 */ { "glCopyTexImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyTexImage1DEXT },
/* 811 */ { "glCopyTexImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyTexImage2DEXT },
/* 812 */ { "glCopyTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyTexSubImage1DEXT },
/* 813 */ { "glCopyTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyTexSubImage2DEXT },
/* 814 */ { "glCopyTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glCopyTexSubImage3DEXT },

/* GL_EXT_histogram
 */
/* 815 */ { "glGetHistogramEXT", (GLExtFuncPtr)&nglContext::glGetHistogramEXT },
/* 816 */ { "glGetHistogramParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetHistogramParameterfvEXT },
/* 817 */ { "glGetHistogramParameterivEXT", (GLExtFuncPtr)&nglContext::glGetHistogramParameterivEXT },
/* 818 */ { "glGetMinmaxEXT", (GLExtFuncPtr)&nglContext::glGetMinmaxEXT },
/* 819 */ { "glGetMinmaxParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetMinmaxParameterfvEXT },
/* 820 */ { "glGetMinmaxParameterivEXT", (GLExtFuncPtr)&nglContext::glGetMinmaxParameterivEXT },
/* 821 */ { "glHistogramEXT", (GLExtFuncPtr)&nglContext::glHistogramEXT },
/* 822 */ { "glMinmaxEXT", (GLExtFuncPtr)&nglContext::glMinmaxEXT },
/* 823 */ { "glResetHistogramEXT", (GLExtFuncPtr)&nglContext::glResetHistogramEXT },
/* 824 */ { "glResetMinmaxEXT", (GLExtFuncPtr)&nglContext::glResetMinmaxEXT },

/* GL_EXT_convolution
 */
/* 825 */ { "glConvolutionFilter1DEXT", (GLExtFuncPtr)&nglContext::glConvolutionFilter1DEXT },
/* 826 */ { "glConvolutionFilter2DEXT", (GLExtFuncPtr)&nglContext::glConvolutionFilter2DEXT },
/* 827 */ { "glConvolutionParameterfEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameterfEXT },
/* 828 */ { "glConvolutionParameterfvEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameterfvEXT },
/* 829 */ { "glConvolutionParameteriEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameteriEXT },
/* 830 */ { "glConvolutionParameterivEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameterivEXT },
/* 831 */ { "glCopyConvolutionFilter1DEXT", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter1DEXT },
/* 832 */ { "glCopyConvolutionFilter2DEXT", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter2DEXT },
/* 833 */ { "glGetConvolutionFilterEXT", (GLExtFuncPtr)&nglContext::glGetConvolutionFilterEXT },
/* 834 */ { "glGetConvolutionParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetConvolutionParameterfvEXT },
/* 835 */ { "glGetConvolutionParameterivEXT", (GLExtFuncPtr)&nglContext::glGetConvolutionParameterivEXT },
/* 836 */ { "glGetSeparableFilterEXT", (GLExtFuncPtr)&nglContext::glGetSeparableFilterEXT },
/* 837 */ { "glSeparableFilter2DEXT", (GLExtFuncPtr)&nglContext::glSeparableFilter2DEXT },

/* GL_SGI_color_table
 */
/* 838 */ { "glColorTableSGI", (GLExtFuncPtr)&nglContext::glColorTableSGI },
/* 839 */ { "glColorTableParameterfvSGI", (GLExtFuncPtr)&nglContext::glColorTableParameterfvSGI },
/* 840 */ { "glColorTableParameterivSGI", (GLExtFuncPtr)&nglContext::glColorTableParameterivSGI },
/* 841 */ { "glCopyColorTableSGI", (GLExtFuncPtr)&nglContext::glCopyColorTableSGI },
/* 842 */ { "glGetColorTableSGI", (GLExtFuncPtr)&nglContext::glGetColorTableSGI },
/* 843 */ { "glGetColorTableParameterfvSGI", (GLExtFuncPtr)&nglContext::glGetColorTableParameterfvSGI },
/* 844 */ { "glGetColorTableParameterivSGI", (GLExtFuncPtr)&nglContext::glGetColorTableParameterivSGI },

/* GL_SGIX_pixel_texture
 */
/* 845 */ { "glPixelTexGenSGIX", (GLExtFuncPtr)&nglContext::glPixelTexGenSGIX },

/* GL_SGIS_pixel_texture
 */
/* 846 */ { "glPixelTexGenParameteriSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameteriSGIS },
/* 847 */ { "glPixelTexGenParameterivSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameterivSGIS },
/* 848 */ { "glPixelTexGenParameterfSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameterfSGIS },
/* 849 */ { "glPixelTexGenParameterfvSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameterfvSGIS },
/* 850 */ { "glGetPixelTexGenParameterivSGIS", (GLExtFuncPtr)&nglContext::glGetPixelTexGenParameterivSGIS },
/* 851 */ { "glGetPixelTexGenParameterfvSGIS", (GLExtFuncPtr)&nglContext::glGetPixelTexGenParameterfvSGIS },

/* GL_SGIS_texture4D
 */
/* 852 */ { "glTexImage4DSGIS", (GLExtFuncPtr)&nglContext::glTexImage4DSGIS },
/* 853 */ { "glTexSubImage4DSGIS", (GLExtFuncPtr)&nglContext::glTexSubImage4DSGIS },

/* GL_EXT_texture_object
 */
/* 854 */ { "glAreTexturesResidentEXT", (GLExtFuncPtr)&nglContext::glAreTexturesResidentEXT },
/* 855 */ { "glBindTextureEXT", (GLExtFuncPtr)&nglContext::glBindTextureEXT },
/* 856 */ { "glDeleteTexturesEXT", (GLExtFuncPtr)&nglContext::glDeleteTexturesEXT },
/* 857 */ { "glGenTexturesEXT", (GLExtFuncPtr)&nglContext::glGenTexturesEXT },
/* 858 */ { "glIsTextureEXT", (GLExtFuncPtr)&nglContext::glIsTextureEXT },
/* 859 */ { "glPrioritizeTexturesEXT", (GLExtFuncPtr)&nglContext::glPrioritizeTexturesEXT },

/* GL_SGIS_detail_texture
 */
/* 860 */ { "glDetailTexFuncSGIS", (GLExtFuncPtr)&nglContext::glDetailTexFuncSGIS },
/* 861 */ { "glGetDetailTexFuncSGIS", (GLExtFuncPtr)&nglContext::glGetDetailTexFuncSGIS },

/* GL_SGIS_sharpen_texture
 */
/* 862 */ { "glSharpenTexFuncSGIS", (GLExtFuncPtr)&nglContext::glSharpenTexFuncSGIS },
/* 863 */ { "glGetSharpenTexFuncSGIS", (GLExtFuncPtr)&nglContext::glGetSharpenTexFuncSGIS },

/* GL_SGIS_multisample
 */
/* 864 */ { "glSampleMaskSGIS", (GLExtFuncPtr)&nglContext::glSampleMaskSGIS },
/* 865 */ { "glSamplePatternSGIS", (GLExtFuncPtr)&nglContext::glSamplePatternSGIS },

/* GL_EXT_vertex_array
 */
/* 866 */ { "glArrayElementEXT", (GLExtFuncPtr)&nglContext::glArrayElementEXT },
/* 867 */ { "glColorPointerEXT", (GLExtFuncPtr)&nglContext::glColorPointerEXT },
/* 868 */ { "glDrawArraysEXT", (GLExtFuncPtr)&nglContext::glDrawArraysEXT },
/* 869 */ { "glEdgeFlagPointerEXT", (GLExtFuncPtr)&nglContext::glEdgeFlagPointerEXT },
/* 870 */ { "glGetPointervEXT", (GLExtFuncPtr)&nglContext::glGetPointervEXT },
/* 871 */ { "glIndexPointerEXT", (GLExtFuncPtr)&nglContext::glIndexPointerEXT },
/* 872 */ { "glNormalPointerEXT", (GLExtFuncPtr)&nglContext::glNormalPointerEXT },
/* 873 */ { "glTexCoordPointerEXT", (GLExtFuncPtr)&nglContext::glTexCoordPointerEXT },
/* 874 */ { "glVertexPointerEXT", (GLExtFuncPtr)&nglContext::glVertexPointerEXT },

/* GL_EXT_blend_minmax
 */
/* 875 */ { "glBlendEquationEXT", (GLExtFuncPtr)&nglContext::glBlendEquationEXT },

/* GL_SGIX_sprite
 */
/* 876 */ { "glSpriteParameterfSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameterfSGIX },
/* 877 */ { "glSpriteParameterfvSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameterfvSGIX },
/* 878 */ { "glSpriteParameteriSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameteriSGIX },
/* 879 */ { "glSpriteParameterivSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameterivSGIX },

/* GL_EXT_point_parameters
 */
/* 880 */ { "glPointParameterfEXT", (GLExtFuncPtr)&nglContext::glPointParameterfEXT },
/* 881 */ { "glPointParameterfvEXT", (GLExtFuncPtr)&nglContext::glPointParameterfvEXT },

/* GL_SGIS_point_parameters
 */
/* 882 */ { "glPointParameterfSGIS", (GLExtFuncPtr)&nglContext::glPointParameterfSGIS },
/* 883 */ { "glPointParameterfvSGIS", (GLExtFuncPtr)&nglContext::glPointParameterfvSGIS },

/* GL_SGIX_instruments
 */
/* 884 */ { "glGetInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glGetInstrumentsSGIX },
/* 885 */ { "glInstrumentsBufferSGIX", (GLExtFuncPtr)&nglContext::glInstrumentsBufferSGIX },
/* 886 */ { "glPollInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glPollInstrumentsSGIX },
/* 887 */ { "glReadInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glReadInstrumentsSGIX },
/* 888 */ { "glStartInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glStartInstrumentsSGIX },
/* 889 */ { "glStopInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glStopInstrumentsSGIX },

/* GL_SGIX_framezoom
 */
/* 890 */ { "glFrameZoomSGIX", (GLExtFuncPtr)&nglContext::glFrameZoomSGIX },

/* GL_SGIX_tag_sample_buffer
 */
/* 891 */ { "glTagSampleBufferSGIX", (GLExtFuncPtr)&nglContext::glTagSampleBufferSGIX },

/* GL_SGIX_polynomial_ffd
 */
/* 892 */ { "glDeformationMap3dSGIX", (GLExtFuncPtr)&nglContext::glDeformationMap3dSGIX },
/* 893 */ { "glDeformationMap3fSGIX", (GLExtFuncPtr)&nglContext::glDeformationMap3fSGIX },
/* 894 */ { "glDeformSGIX", (GLExtFuncPtr)&nglContext::glDeformSGIX },
/* 895 */ { "glLoadIdentityDeformationMapSGIX", (GLExtFuncPtr)&nglContext::glLoadIdentityDeformationMapSGIX },

/* GL_SGIX_reference_plane
 */
/* 896 */ { "glReferencePlaneSGIX", (GLExtFuncPtr)&nglContext::glReferencePlaneSGIX },

/* GL_SGIX_flush_raster
 */
/* 897 */ { "glFlushRasterSGIX", (GLExtFuncPtr)&nglContext::glFlushRasterSGIX },

/* GL_SGIS_fog_function
 */
/* 898 */ { "glFogFuncSGIS", (GLExtFuncPtr)&nglContext::glFogFuncSGIS },
/* 899 */ { "glGetFogFuncSGIS", (GLExtFuncPtr)&nglContext::glGetFogFuncSGIS },

/* GL_HP_image_transform
 */
/* 900 */ { "glImageTransformParameteriHP", (GLExtFuncPtr)&nglContext::glImageTransformParameteriHP },
/* 901 */ { "glImageTransformParameterfHP", (GLExtFuncPtr)&nglContext::glImageTransformParameterfHP },
/* 902 */ { "glImageTransformParameterivHP", (GLExtFuncPtr)&nglContext::glImageTransformParameterivHP },
/* 903 */ { "glImageTransformParameterfvHP", (GLExtFuncPtr)&nglContext::glImageTransformParameterfvHP },
/* 904 */ { "glGetImageTransformParameterivHP", (GLExtFuncPtr)&nglContext::glGetImageTransformParameterivHP },
/* 905 */ { "glGetImageTransformParameterfvHP", (GLExtFuncPtr)&nglContext::glGetImageTransformParameterfvHP },

/* GL_EXT_color_subtable
 */
/* 906 */ { "glColorSubTableEXT", (GLExtFuncPtr)&nglContext::glColorSubTableEXT },
/* 907 */ { "glCopyColorSubTableEXT", (GLExtFuncPtr)&nglContext::glCopyColorSubTableEXT },

/* GL_PGI_misc_hints
 */
/* 908 */ { "glHintPGI", (GLExtFuncPtr)&nglContext::glHintPGI },

/* GL_EXT_paletted_texture
 */
/* 909 */ { "glColorTableEXT", (GLExtFuncPtr)&nglContext::glColorTableEXT },
/* 910 */ { "glGetColorTableEXT", (GLExtFuncPtr)&nglContext::glGetColorTableEXT },
/* 911 */ { "glGetColorTableParameterivEXT", (GLExtFuncPtr)&nglContext::glGetColorTableParameterivEXT },
/* 912 */ { "glGetColorTableParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetColorTableParameterfvEXT },

/* GL_SGIX_list_priority
 */
/* 913 */ { "glGetListParameterfvSGIX", (GLExtFuncPtr)&nglContext::glGetListParameterfvSGIX },
/* 914 */ { "glGetListParameterivSGIX", (GLExtFuncPtr)&nglContext::glGetListParameterivSGIX },
/* 915 */ { "glListParameterfSGIX", (GLExtFuncPtr)&nglContext::glListParameterfSGIX },
/* 916 */ { "glListParameterfvSGIX", (GLExtFuncPtr)&nglContext::glListParameterfvSGIX },
/* 917 */ { "glListParameteriSGIX", (GLExtFuncPtr)&nglContext::glListParameteriSGIX },
/* 918 */ { "glListParameterivSGIX", (GLExtFuncPtr)&nglContext::glListParameterivSGIX },

/* GL_EXT_index_material
 */
/* 919 */ { "glIndexMaterialEXT", (GLExtFuncPtr)&nglContext::glIndexMaterialEXT },

/* GL_EXT_index_func
 */
/* 920 */ { "glIndexFuncEXT", (GLExtFuncPtr)&nglContext::glIndexFuncEXT },

/* GL_EXT_compiled_vertex_array
 */
/* 921 */ { "glLockArraysEXT", (GLExtFuncPtr)&nglContext::glLockArraysEXT },
/* 922 */ { "glUnlockArraysEXT", (GLExtFuncPtr)&nglContext::glUnlockArraysEXT },

/* GL_EXT_cull_vertex
 */
/* 923 */ { "glCullParameterdvEXT", (GLExtFuncPtr)&nglContext::glCullParameterdvEXT },
/* 924 */ { "glCullParameterfvEXT", (GLExtFuncPtr)&nglContext::glCullParameterfvEXT },

/* GL_SGIX_fragment_lighting
 */
/* 925 */ { "glFragmentColorMaterialSGIX", (GLExtFuncPtr)&nglContext::glFragmentColorMaterialSGIX },
/* 926 */ { "glFragmentLightfSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightfSGIX },
/* 927 */ { "glFragmentLightfvSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightfvSGIX },
/* 928 */ { "glFragmentLightiSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightiSGIX },
/* 929 */ { "glFragmentLightivSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightivSGIX },
/* 930 */ { "glFragmentLightModelfSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModelfSGIX },
/* 931 */ { "glFragmentLightModelfvSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModelfvSGIX },
/* 932 */ { "glFragmentLightModeliSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModeliSGIX },
/* 933 */ { "glFragmentLightModelivSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModelivSGIX },
/* 934 */ { "glFragmentMaterialfSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialfSGIX },
/* 935 */ { "glFragmentMaterialfvSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialfvSGIX },
/* 936 */ { "glFragmentMaterialiSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialiSGIX },
/* 937 */ { "glFragmentMaterialivSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialivSGIX },
/* 938 */ { "glGetFragmentLightfvSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentLightfvSGIX },
/* 939 */ { "glGetFragmentLightivSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentLightivSGIX },
/* 940 */ { "glGetFragmentMaterialfvSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentMaterialfvSGIX },
/* 941 */ { "glGetFragmentMaterialivSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentMaterialivSGIX },
/* 942 */ { "glLightEnviSGIX", (GLExtFuncPtr)&nglContext::glLightEnviSGIX },

/* GL_EXT_draw_range_elements
 */
/* 943 */ { "glDrawRangeElementsEXT", (GLExtFuncPtr)&nglContext::glDrawRangeElementsEXT },

/* GL_EXT_light_texture
 */
/* 944 */ { "glApplyTextureEXT", (GLExtFuncPtr)&nglContext::glApplyTextureEXT },
/* 945 */ { "glTextureLightEXT", (GLExtFuncPtr)&nglContext::glTextureLightEXT },
/* 946 */ { "glTextureMaterialEXT", (GLExtFuncPtr)&nglContext::glTextureMaterialEXT },

/* GL_SGIX_async
 */
/* 947 */ { "glAsyncMarkerSGIX", (GLExtFuncPtr)&nglContext::glAsyncMarkerSGIX },
/* 948 */ { "glFinishAsyncSGIX", (GLExtFuncPtr)&nglContext::glFinishAsyncSGIX },
/* 949 */ { "glPollAsyncSGIX", (GLExtFuncPtr)&nglContext::glPollAsyncSGIX },
/* 950 */ { "glGenAsyncMarkersSGIX", (GLExtFuncPtr)&nglContext::glGenAsyncMarkersSGIX },
/* 951 */ { "glDeleteAsyncMarkersSGIX", (GLExtFuncPtr)&nglContext::glDeleteAsyncMarkersSGIX },
/* 952 */ { "glIsAsyncMarkerSGIX", (GLExtFuncPtr)&nglContext::glIsAsyncMarkerSGIX },

/* GL_INTEL_parallel_arrays
 */
/* 953 */ { "glVertexPointervINTEL", (GLExtFuncPtr)&nglContext::glVertexPointervINTEL },
/* 954 */ { "glNormalPointervINTEL", (GLExtFuncPtr)&nglContext::glNormalPointervINTEL },
/* 955 */ { "glColorPointervINTEL", (GLExtFuncPtr)&nglContext::glColorPointervINTEL },
/* 956 */ { "glTexCoordPointervINTEL", (GLExtFuncPtr)&nglContext::glTexCoordPointervINTEL },

/* GL_EXT_pixel_transform
 */
/* 957 */ { "glPixelTransformParameteriEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameteriEXT },
/* 958 */ { "glPixelTransformParameterfEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameterfEXT },
/* 959 */ { "glPixelTransformParameterivEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameterivEXT },
/* 960 */ { "glPixelTransformParameterfvEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameterfvEXT },

/* GL_EXT_secondary_color
 */
/* 961 */ { "glSecondaryColor3bEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3bEXT },
/* 962 */ { "glSecondaryColor3bvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3bvEXT },
/* 963 */ { "glSecondaryColor3dEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3dEXT },
/* 964 */ { "glSecondaryColor3dvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3dvEXT },
/* 965 */ { "glSecondaryColor3fEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3fEXT },
/* 966 */ { "glSecondaryColor3fvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3fvEXT },
/* 967 */ { "glSecondaryColor3iEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3iEXT },
/* 968 */ { "glSecondaryColor3ivEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3ivEXT },
/* 969 */ { "glSecondaryColor3sEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3sEXT },
/* 970 */ { "glSecondaryColor3svEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3svEXT },
/* 971 */ { "glSecondaryColor3ubEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3ubEXT },
/* 972 */ { "glSecondaryColor3ubvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3ubvEXT },
/* 973 */ { "glSecondaryColor3uiEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3uiEXT },
/* 974 */ { "glSecondaryColor3uivEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3uivEXT },
/* 975 */ { "glSecondaryColor3usEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3usEXT },
/* 976 */ { "glSecondaryColor3usvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3usvEXT },
/* 977 */ { "glSecondaryColorPointerEXT", (GLExtFuncPtr)&nglContext::glSecondaryColorPointerEXT },

/* GL_EXT_texture_perturb_normal
 */
/* 978 */ { "glTextureNormalEXT", (GLExtFuncPtr)&nglContext::glTextureNormalEXT },

/* GL_EXT_multi_draw_arrays
 */
/* 979 */ { "glMultiDrawArraysEXT", (GLExtFuncPtr)&nglContext::glMultiDrawArraysEXT },
/* 980 */ { "glMultiDrawElementsEXT", (GLExtFuncPtr)&nglContext::glMultiDrawElementsEXT },

/* GL_EXT_fog_coord
 */
/* 981 */ { "glFogCoordfEXT", (GLExtFuncPtr)&nglContext::glFogCoordfEXT },
/* 982 */ { "glFogCoordfvEXT", (GLExtFuncPtr)&nglContext::glFogCoordfvEXT },
/* 983 */ { "glFogCoorddEXT", (GLExtFuncPtr)&nglContext::glFogCoorddEXT },
/* 984 */ { "glFogCoorddvEXT", (GLExtFuncPtr)&nglContext::glFogCoorddvEXT },
/* 985 */ { "glFogCoordPointerEXT", (GLExtFuncPtr)&nglContext::glFogCoordPointerEXT },

/* GL_EXT_coordinate_frame
 */
/* 986 */ { "glTangent3bEXT", (GLExtFuncPtr)&nglContext::glTangent3bEXT },
/* 987 */ { "glTangent3bvEXT", (GLExtFuncPtr)&nglContext::glTangent3bvEXT },
/* 988 */ { "glTangent3dEXT", (GLExtFuncPtr)&nglContext::glTangent3dEXT },
/* 989 */ { "glTangent3dvEXT", (GLExtFuncPtr)&nglContext::glTangent3dvEXT },
/* 990 */ { "glTangent3fEXT", (GLExtFuncPtr)&nglContext::glTangent3fEXT },
/* 991 */ { "glTangent3fvEXT", (GLExtFuncPtr)&nglContext::glTangent3fvEXT },
/* 992 */ { "glTangent3iEXT", (GLExtFuncPtr)&nglContext::glTangent3iEXT },
/* 993 */ { "glTangent3ivEXT", (GLExtFuncPtr)&nglContext::glTangent3ivEXT },
/* 994 */ { "glTangent3sEXT", (GLExtFuncPtr)&nglContext::glTangent3sEXT },
/* 995 */ { "glTangent3svEXT", (GLExtFuncPtr)&nglContext::glTangent3svEXT },
/* 996 */ { "glBinormal3bEXT", (GLExtFuncPtr)&nglContext::glBinormal3bEXT },
/* 997 */ { "glBinormal3bvEXT", (GLExtFuncPtr)&nglContext::glBinormal3bvEXT },
/* 998 */ { "glBinormal3dEXT", (GLExtFuncPtr)&nglContext::glBinormal3dEXT },
/* 999 */ { "glBinormal3dvEXT", (GLExtFuncPtr)&nglContext::glBinormal3dvEXT },
/* 1000 */ { "glBinormal3fEXT", (GLExtFuncPtr)&nglContext::glBinormal3fEXT },
/* 1001 */ { "glBinormal3fvEXT", (GLExtFuncPtr)&nglContext::glBinormal3fvEXT },
/* 1002 */ { "glBinormal3iEXT", (GLExtFuncPtr)&nglContext::glBinormal3iEXT },
/* 1003 */ { "glBinormal3ivEXT", (GLExtFuncPtr)&nglContext::glBinormal3ivEXT },
/* 1004 */ { "glBinormal3sEXT", (GLExtFuncPtr)&nglContext::glBinormal3sEXT },
/* 1005 */ { "glBinormal3svEXT", (GLExtFuncPtr)&nglContext::glBinormal3svEXT },
/* 1006 */ { "glTangentPointerEXT", (GLExtFuncPtr)&nglContext::glTangentPointerEXT },
/* 1007 */ { "glBinormalPointerEXT", (GLExtFuncPtr)&nglContext::glBinormalPointerEXT },

/* GL_SUNX_constant_data
 */
/* 1008 */ { "glFinishTextureSUNX", (GLExtFuncPtr)&nglContext::glFinishTextureSUNX },

/* GL_SUN_global_alpha
 */
/* 1009 */ { "glGlobalAlphaFactorbSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorbSUN },
/* 1010 */ { "glGlobalAlphaFactorsSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorsSUN },
/* 1011 */ { "glGlobalAlphaFactoriSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactoriSUN },
/* 1012 */ { "glGlobalAlphaFactorfSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorfSUN },
/* 1013 */ { "glGlobalAlphaFactordSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactordSUN },
/* 1014 */ { "glGlobalAlphaFactorubSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorubSUN },
/* 1015 */ { "glGlobalAlphaFactorusSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorusSUN },
/* 1016 */ { "glGlobalAlphaFactoruiSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactoruiSUN },

/* GL_SUN_triangle_list
 */
/* 1017 */ { "glReplacementCodeuiSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiSUN },
/* 1018 */ { "glReplacementCodeusSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeusSUN },
/* 1019 */ { "glReplacementCodeubSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeubSUN },
/* 1020 */ { "glReplacementCodeuivSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuivSUN },
/* 1021 */ { "glReplacementCodeusvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeusvSUN },
/* 1022 */ { "glReplacementCodeubvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeubvSUN },
/* 1023 */ { "glReplacementCodePointerSUN", (GLExtFuncPtr)&nglContext::glReplacementCodePointerSUN },

/* GL_SUN_vertex
 */
/* 1024 */ { "glColor4ubVertex2fSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex2fSUN },
/* 1025 */ { "glColor4ubVertex2fvSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex2fvSUN },
/* 1026 */ { "glColor4ubVertex3fSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex3fSUN },
/* 1027 */ { "glColor4ubVertex3fvSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex3fvSUN },
/* 1028 */ { "glColor3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glColor3fVertex3fSUN },
/* 1029 */ { "glColor3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glColor3fVertex3fvSUN },
/* 1030 */ { "glNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glNormal3fVertex3fSUN },
/* 1031 */ { "glNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glNormal3fVertex3fvSUN },
/* 1032 */ { "glColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glColor4fNormal3fVertex3fSUN },
/* 1033 */ { "glColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glColor4fNormal3fVertex3fvSUN },
/* 1034 */ { "glTexCoord2fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fVertex3fSUN },
/* 1035 */ { "glTexCoord2fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fVertex3fvSUN },
/* 1036 */ { "glTexCoord4fVertex4fSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fVertex4fSUN },
/* 1037 */ { "glTexCoord4fVertex4fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fVertex4fvSUN },
/* 1038 */ { "glTexCoord2fColor4ubVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4ubVertex3fSUN },
/* 1039 */ { "glTexCoord2fColor4ubVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4ubVertex3fvSUN },
/* 1040 */ { "glTexCoord2fColor3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor3fVertex3fSUN },
/* 1041 */ { "glTexCoord2fColor3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor3fVertex3fvSUN },
/* 1042 */ { "glTexCoord2fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fNormal3fVertex3fSUN },
/* 1043 */ { "glTexCoord2fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fNormal3fVertex3fvSUN },
/* 1044 */ { "glTexCoord2fColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4fNormal3fVertex3fSUN },
/* 1045 */ { "glTexCoord2fColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4fNormal3fVertex3fvSUN },
/* 1046 */ { "glTexCoord4fColor4fNormal3fVertex4fSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fColor4fNormal3fVertex4fSUN },
/* 1047 */ { "glTexCoord4fColor4fNormal3fVertex4fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fColor4fNormal3fVertex4fvSUN },
/* 1048 */ { "glReplacementCodeuiVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiVertex3fSUN },
/* 1049 */ { "glReplacementCodeuiVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiVertex3fvSUN },
/* 1050 */ { "glReplacementCodeuiColor4ubVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4ubVertex3fSUN },
/* 1051 */ { "glReplacementCodeuiColor4ubVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4ubVertex3fvSUN },
/* 1052 */ { "glReplacementCodeuiColor3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor3fVertex3fSUN },
/* 1053 */ { "glReplacementCodeuiColor3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor3fVertex3fvSUN },
/* 1054 */ { "glReplacementCodeuiNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiNormal3fVertex3fSUN },
/* 1055 */ { "glReplacementCodeuiNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiNormal3fVertex3fvSUN },
/* 1056 */ { "glReplacementCodeuiColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4fNormal3fVertex3fSUN },
/* 1057 */ { "glReplacementCodeuiColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4fNormal3fVertex3fvSUN },
/* 1058 */ { "glReplacementCodeuiTexCoord2fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fVertex3fSUN },
/* 1059 */ { "glReplacementCodeuiTexCoord2fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fVertex3fvSUN },
/* 1060 */ { "glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN },
/* 1061 */ { "glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN },
/* 1062 */ { "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN },
/* 1063 */ { "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN },

/* GL_EXT_blend_func_separate
 */
/* 1064 */ { "glBlendFuncSeparateEXT", (GLExtFuncPtr)&nglContext::glBlendFuncSeparateEXT },

/* GL_INGR_blend_func_separate
 */
/* 1065 */ { "glBlendFuncSeparateINGR", (GLExtFuncPtr)&nglContext::glBlendFuncSeparateINGR },

/* GL_EXT_vertex_weighting
 */
/* 1066 */ { "glVertexWeightfEXT", (GLExtFuncPtr)&nglContext::glVertexWeightfEXT },
/* 1067 */ { "glVertexWeightfvEXT", (GLExtFuncPtr)&nglContext::glVertexWeightfvEXT },
/* 1068 */ { "glVertexWeightPointerEXT", (GLExtFuncPtr)&nglContext::glVertexWeightPointerEXT },

/* GL_NV_vertex_array_range
 */
/* 1069 */ { "glFlushVertexArrayRangeNV", (GLExtFuncPtr)&nglContext::glFlushVertexArrayRangeNV },
/* 1070 */ { "glVertexArrayRangeNV", (GLExtFuncPtr)&nglContext::glVertexArrayRangeNV },

/* GL_NV_register_combiners
 */
/* 1071 */ { "glCombinerParameterfvNV", (GLExtFuncPtr)&nglContext::glCombinerParameterfvNV },
/* 1072 */ { "glCombinerParameterfNV", (GLExtFuncPtr)&nglContext::glCombinerParameterfNV },
/* 1073 */ { "glCombinerParameterivNV", (GLExtFuncPtr)&nglContext::glCombinerParameterivNV },
/* 1074 */ { "glCombinerParameteriNV", (GLExtFuncPtr)&nglContext::glCombinerParameteriNV },
/* 1075 */ { "glCombinerInputNV", (GLExtFuncPtr)&nglContext::glCombinerInputNV },
/* 1076 */ { "glCombinerOutputNV", (GLExtFuncPtr)&nglContext::glCombinerOutputNV },
/* 1077 */ { "glFinalCombinerInputNV", (GLExtFuncPtr)&nglContext::glFinalCombinerInputNV },
/* 1078 */ { "glGetCombinerInputParameterfvNV", (GLExtFuncPtr)&nglContext::glGetCombinerInputParameterfvNV },
/* 1079 */ { "glGetCombinerInputParameterivNV", (GLExtFuncPtr)&nglContext::glGetCombinerInputParameterivNV },
/* 1080 */ { "glGetCombinerOutputParameterfvNV", (GLExtFuncPtr)&nglContext::glGetCombinerOutputParameterfvNV },
/* 1081 */ { "glGetCombinerOutputParameterivNV", (GLExtFuncPtr)&nglContext::glGetCombinerOutputParameterivNV },
/* 1082 */ { "glGetFinalCombinerInputParameterfvNV", (GLExtFuncPtr)&nglContext::glGetFinalCombinerInputParameterfvNV },
/* 1083 */ { "glGetFinalCombinerInputParameterivNV", (GLExtFuncPtr)&nglContext::glGetFinalCombinerInputParameterivNV },

/* GL_MESA_resize_buffers
 */
/* 1084 */ { "glResizeBuffersMESA", (GLExtFuncPtr)&nglContext::glResizeBuffersMESA },

/* GL_MESA_window_pos
 */
/* 1085 */ { "glWindowPos2dMESA", (GLExtFuncPtr)&nglContext::glWindowPos2dMESA },
/* 1086 */ { "glWindowPos2dvMESA", (GLExtFuncPtr)&nglContext::glWindowPos2dvMESA },
/* 1087 */ { "glWindowPos2fMESA", (GLExtFuncPtr)&nglContext::glWindowPos2fMESA },
/* 1088 */ { "glWindowPos2fvMESA", (GLExtFuncPtr)&nglContext::glWindowPos2fvMESA },
/* 1089 */ { "glWindowPos2iMESA", (GLExtFuncPtr)&nglContext::glWindowPos2iMESA },
/* 1090 */ { "glWindowPos2ivMESA", (GLExtFuncPtr)&nglContext::glWindowPos2ivMESA },
/* 1091 */ { "glWindowPos2sMESA", (GLExtFuncPtr)&nglContext::glWindowPos2sMESA },
/* 1092 */ { "glWindowPos2svMESA", (GLExtFuncPtr)&nglContext::glWindowPos2svMESA },
/* 1093 */ { "glWindowPos3dMESA", (GLExtFuncPtr)&nglContext::glWindowPos3dMESA },
/* 1094 */ { "glWindowPos3dvMESA", (GLExtFuncPtr)&nglContext::glWindowPos3dvMESA },
/* 1095 */ { "glWindowPos3fMESA", (GLExtFuncPtr)&nglContext::glWindowPos3fMESA },
/* 1096 */ { "glWindowPos3fvMESA", (GLExtFuncPtr)&nglContext::glWindowPos3fvMESA },
/* 1097 */ { "glWindowPos3iMESA", (GLExtFuncPtr)&nglContext::glWindowPos3iMESA },
/* 1098 */ { "glWindowPos3ivMESA", (GLExtFuncPtr)&nglContext::glWindowPos3ivMESA },
/* 1099 */ { "glWindowPos3sMESA", (GLExtFuncPtr)&nglContext::glWindowPos3sMESA },
/* 1100 */ { "glWindowPos3svMESA", (GLExtFuncPtr)&nglContext::glWindowPos3svMESA },
/* 1101 */ { "glWindowPos4dMESA", (GLExtFuncPtr)&nglContext::glWindowPos4dMESA },
/* 1102 */ { "glWindowPos4dvMESA", (GLExtFuncPtr)&nglContext::glWindowPos4dvMESA },
/* 1103 */ { "glWindowPos4fMESA", (GLExtFuncPtr)&nglContext::glWindowPos4fMESA },
/* 1104 */ { "glWindowPos4fvMESA", (GLExtFuncPtr)&nglContext::glWindowPos4fvMESA },
/* 1105 */ { "glWindowPos4iMESA", (GLExtFuncPtr)&nglContext::glWindowPos4iMESA },
/* 1106 */ { "glWindowPos4ivMESA", (GLExtFuncPtr)&nglContext::glWindowPos4ivMESA },
/* 1107 */ { "glWindowPos4sMESA", (GLExtFuncPtr)&nglContext::glWindowPos4sMESA },
/* 1108 */ { "glWindowPos4svMESA", (GLExtFuncPtr)&nglContext::glWindowPos4svMESA },

/* GL_IBM_multimode_draw_arrays
 */
/* 1109 */ { "glMultiModeDrawArraysIBM", (GLExtFuncPtr)&nglContext::glMultiModeDrawArraysIBM },
/* 1110 */ { "glMultiModeDrawElementsIBM", (GLExtFuncPtr)&nglContext::glMultiModeDrawElementsIBM },

/* GL_IBM_vertex_array_lists
 */
/* 1111 */ { "glColorPointerListIBM", (GLExtFuncPtr)&nglContext::glColorPointerListIBM },
/* 1112 */ { "glSecondaryColorPointerListIBM", (GLExtFuncPtr)&nglContext::glSecondaryColorPointerListIBM },
/* 1113 */ { "glEdgeFlagPointerListIBM", (GLExtFuncPtr)&nglContext::glEdgeFlagPointerListIBM },
/* 1114 */ { "glFogCoordPointerListIBM", (GLExtFuncPtr)&nglContext::glFogCoordPointerListIBM },
/* 1115 */ { "glIndexPointerListIBM", (GLExtFuncPtr)&nglContext::glIndexPointerListIBM },
/* 1116 */ { "glNormalPointerListIBM", (GLExtFuncPtr)&nglContext::glNormalPointerListIBM },
/* 1117 */ { "glTexCoordPointerListIBM", (GLExtFuncPtr)&nglContext::glTexCoordPointerListIBM },
/* 1118 */ { "glVertexPointerListIBM", (GLExtFuncPtr)&nglContext::glVertexPointerListIBM },

/* GL_3DFX_tbuffer
 */
/* 1119 */ { "glTbufferMask3DFX", (GLExtFuncPtr)&nglContext::glTbufferMask3DFX },

/* GL_EXT_multisample
 */
/* 1120 */ { "glSampleMaskEXT", (GLExtFuncPtr)&nglContext::glSampleMaskEXT },
/* 1121 */ { "glSamplePatternEXT", (GLExtFuncPtr)&nglContext::glSamplePatternEXT },

/* GL_SGIS_texture_color_mask
 */
/* 1122 */ { "glTextureColorMaskSGIS", (GLExtFuncPtr)&nglContext::glTextureColorMaskSGIS },

/* GL_SGIX_igloo_interface
 */
/* 1123 */ { "glIglooInterfaceSGIX", (GLExtFuncPtr)&nglContext::glIglooInterfaceSGIX },

/* GL_NV_fence
 */
/* 1124 */ { "glDeleteFencesNV", (GLExtFuncPtr)&nglContext::glDeleteFencesNV },
/* 1125 */ { "glGenFencesNV", (GLExtFuncPtr)&nglContext::glGenFencesNV },
/* 1126 */ { "glIsFenceNV", (GLExtFuncPtr)&nglContext::glIsFenceNV },
/* 1127 */ { "glTestFenceNV", (GLExtFuncPtr)&nglContext::glTestFenceNV },
/* 1128 */ { "glGetFenceivNV", (GLExtFuncPtr)&nglContext::glGetFenceivNV },
/* 1129 */ { "glFinishFenceNV", (GLExtFuncPtr)&nglContext::glFinishFenceNV },
/* 1130 */ { "glSetFenceNV", (GLExtFuncPtr)&nglContext::glSetFenceNV },

/* GL_NV_evaluators
 */
/* 1131 */ { "glMapControlPointsNV", (GLExtFuncPtr)&nglContext::glMapControlPointsNV },
/* 1132 */ { "glMapParameterivNV", (GLExtFuncPtr)&nglContext::glMapParameterivNV },
/* 1133 */ { "glMapParameterfvNV", (GLExtFuncPtr)&nglContext::glMapParameterfvNV },
/* 1134 */ { "glGetMapControlPointsNV", (GLExtFuncPtr)&nglContext::glGetMapControlPointsNV },
/* 1135 */ { "glGetMapParameterivNV", (GLExtFuncPtr)&nglContext::glGetMapParameterivNV },
/* 1136 */ { "glGetMapParameterfvNV", (GLExtFuncPtr)&nglContext::glGetMapParameterfvNV },
/* 1137 */ { "glGetMapAttribParameterivNV", (GLExtFuncPtr)&nglContext::glGetMapAttribParameterivNV },
/* 1138 */ { "glGetMapAttribParameterfvNV", (GLExtFuncPtr)&nglContext::glGetMapAttribParameterfvNV },
/* 1139 */ { "glEvalMapsNV", (GLExtFuncPtr)&nglContext::glEvalMapsNV },

/* GL_NV_register_combiners2
 */
/* 1140 */ { "glCombinerStageParameterfvNV", (GLExtFuncPtr)&nglContext::glCombinerStageParameterfvNV },
/* 1141 */ { "glGetCombinerStageParameterfvNV", (GLExtFuncPtr)&nglContext::glGetCombinerStageParameterfvNV },

/* GL_NV_vertex_program
 */
/* 1142 */ { "glAreProgramsResidentNV", (GLExtFuncPtr)&nglContext::glAreProgramsResidentNV },
/* 1143 */ { "glBindProgramNV", (GLExtFuncPtr)&nglContext::glBindProgramNV },
/* 1144 */ { "glDeleteProgramsNV", (GLExtFuncPtr)&nglContext::glDeleteProgramsNV },
/* 1145 */ { "glExecuteProgramNV", (GLExtFuncPtr)&nglContext::glExecuteProgramNV },
/* 1146 */ { "glGenProgramsNV", (GLExtFuncPtr)&nglContext::glGenProgramsNV },
/* 1147 */ { "glGetProgramParameterdvNV", (GLExtFuncPtr)&nglContext::glGetProgramParameterdvNV },
/* 1148 */ { "glGetProgramParameterfvNV", (GLExtFuncPtr)&nglContext::glGetProgramParameterfvNV },
/* 1149 */ { "glGetProgramivNV", (GLExtFuncPtr)&nglContext::glGetProgramivNV },
/* 1150 */ { "glGetProgramStringNV", (GLExtFuncPtr)&nglContext::glGetProgramStringNV },
/* 1151 */ { "glGetTrackMatrixivNV", (GLExtFuncPtr)&nglContext::glGetTrackMatrixivNV },
/* 1152 */ { "glGetVertexAttribdvNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribdvNV },
/* 1153 */ { "glGetVertexAttribfvNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribfvNV },
/* 1154 */ { "glGetVertexAttribivNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribivNV },
/* 1155 */ { "glGetVertexAttribPointervNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribPointervNV },
/* 1156 */ { "glIsProgramNV", (GLExtFuncPtr)&nglContext::glIsProgramNV },
/* 1157 */ { "glLoadProgramNV", (GLExtFuncPtr)&nglContext::glLoadProgramNV },
/* 1158 */ { "glProgramParameter4dNV", (GLExtFuncPtr)&nglContext::glProgramParameter4dNV },
/* 1159 */ { "glProgramParameter4dvNV", (GLExtFuncPtr)&nglContext::glProgramParameter4dvNV },
/* 1160 */ { "glProgramParameter4fNV", (GLExtFuncPtr)&nglContext::glProgramParameter4fNV },
/* 1161 */ { "glProgramParameter4fvNV", (GLExtFuncPtr)&nglContext::glProgramParameter4fvNV },
/* 1162 */ { "glProgramParameters4dvNV", (GLExtFuncPtr)&nglContext::glProgramParameters4dvNV },
/* 1163 */ { "glProgramParameters4fvNV", (GLExtFuncPtr)&nglContext::glProgramParameters4fvNV },
/* 1164 */ { "glRequestResidentProgramsNV", (GLExtFuncPtr)&nglContext::glRequestResidentProgramsNV },
/* 1165 */ { "glTrackMatrixNV", (GLExtFuncPtr)&nglContext::glTrackMatrixNV },
/* 1166 */ { "glVertexAttribPointerNV", (GLExtFuncPtr)&nglContext::glVertexAttribPointerNV },
/* 1167 */ { "glVertexAttrib1dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1dNV },
/* 1168 */ { "glVertexAttrib1dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1dvNV },
/* 1169 */ { "glVertexAttrib1fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1fNV },
/* 1170 */ { "glVertexAttrib1fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1fvNV },
/* 1171 */ { "glVertexAttrib1sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1sNV },
/* 1172 */ { "glVertexAttrib1svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1svNV },
/* 1173 */ { "glVertexAttrib2dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2dNV },
/* 1174 */ { "glVertexAttrib2dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2dvNV },
/* 1175 */ { "glVertexAttrib2fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2fNV },
/* 1176 */ { "glVertexAttrib2fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2fvNV },
/* 1177 */ { "glVertexAttrib2sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2sNV },
/* 1178 */ { "glVertexAttrib2svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2svNV },
/* 1179 */ { "glVertexAttrib3dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3dNV },
/* 1180 */ { "glVertexAttrib3dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3dvNV },
/* 1181 */ { "glVertexAttrib3fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3fNV },
/* 1182 */ { "glVertexAttrib3fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3fvNV },
/* 1183 */ { "glVertexAttrib3sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3sNV },
/* 1184 */ { "glVertexAttrib3svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3svNV },
/* 1185 */ { "glVertexAttrib4dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4dNV },
/* 1186 */ { "glVertexAttrib4dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4dvNV },
/* 1187 */ { "glVertexAttrib4fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4fNV },
/* 1188 */ { "glVertexAttrib4fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4fvNV },
/* 1189 */ { "glVertexAttrib4sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4sNV },
/* 1190 */ { "glVertexAttrib4svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4svNV },
/* 1191 */ { "glVertexAttrib4ubNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubNV },
/* 1192 */ { "glVertexAttrib4ubvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubvNV },
/* 1193 */ { "glVertexAttribs1dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1dvNV },
/* 1194 */ { "glVertexAttribs1fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1fvNV },
/* 1195 */ { "glVertexAttribs1svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1svNV },
/* 1196 */ { "glVertexAttribs2dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2dvNV },
/* 1197 */ { "glVertexAttribs2fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2fvNV },
/* 1198 */ { "glVertexAttribs2svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2svNV },
/* 1199 */ { "glVertexAttribs3dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3dvNV },
/* 1200 */ { "glVertexAttribs3fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3fvNV },
/* 1201 */ { "glVertexAttribs3svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3svNV },
/* 1202 */ { "glVertexAttribs4dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4dvNV },
/* 1203 */ { "glVertexAttribs4fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4fvNV },
/* 1204 */ { "glVertexAttribs4svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4svNV },
/* 1205 */ { "glVertexAttribs4ubvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4ubvNV },

/* GL_ATI_envmap_bumpmap
 */
/* 1206 */ { "glTexBumpParameterivATI", (GLExtFuncPtr)&nglContext::glTexBumpParameterivATI },
/* 1207 */ { "glTexBumpParameterfvATI", (GLExtFuncPtr)&nglContext::glTexBumpParameterfvATI },
/* 1208 */ { "glGetTexBumpParameterivATI", (GLExtFuncPtr)&nglContext::glGetTexBumpParameterivATI },
/* 1209 */ { "glGetTexBumpParameterfvATI", (GLExtFuncPtr)&nglContext::glGetTexBumpParameterfvATI },

/* GL_ATI_fragment_shader
 */
/* 1210 */ { "glGenFragmentShadersATI", (GLExtFuncPtr)&nglContext::glGenFragmentShadersATI },
/* 1211 */ { "glBindFragmentShaderATI", (GLExtFuncPtr)&nglContext::glBindFragmentShaderATI },
/* 1212 */ { "glDeleteFragmentShaderATI", (GLExtFuncPtr)&nglContext::glDeleteFragmentShaderATI },
/* 1213 */ { "glBeginFragmentShaderATI", (GLExtFuncPtr)&nglContext::glBeginFragmentShaderATI },
/* 1214 */ { "glEndFragmentShaderATI", (GLExtFuncPtr)&nglContext::glEndFragmentShaderATI },
/* 1215 */ { "glPassTexCoordATI", (GLExtFuncPtr)&nglContext::glPassTexCoordATI },
/* 1216 */ { "glSampleMapATI", (GLExtFuncPtr)&nglContext::glSampleMapATI },
/* 1217 */ { "glColorFragmentOp1ATI", (GLExtFuncPtr)&nglContext::glColorFragmentOp1ATI },
/* 1218 */ { "glColorFragmentOp2ATI", (GLExtFuncPtr)&nglContext::glColorFragmentOp2ATI },
/* 1219 */ { "glColorFragmentOp3ATI", (GLExtFuncPtr)&nglContext::glColorFragmentOp3ATI },
/* 1220 */ { "glAlphaFragmentOp1ATI", (GLExtFuncPtr)&nglContext::glAlphaFragmentOp1ATI },
/* 1221 */ { "glAlphaFragmentOp2ATI", (GLExtFuncPtr)&nglContext::glAlphaFragmentOp2ATI },
/* 1222 */ { "glAlphaFragmentOp3ATI", (GLExtFuncPtr)&nglContext::glAlphaFragmentOp3ATI },
/* 1223 */ { "glSetFragmentShaderConstantATI", (GLExtFuncPtr)&nglContext::glSetFragmentShaderConstantATI },

/* GL_ATI_pn_triangles
 */
/* 1224 */ { "glPNTrianglesiATI", (GLExtFuncPtr)&nglContext::glPNTrianglesiATI },
/* 1225 */ { "glPNTrianglesfATI", (GLExtFuncPtr)&nglContext::glPNTrianglesfATI },

/* GL_ATI_vertex_array_object
 */
/* 1226 */ { "glNewObjectBufferATI", (GLExtFuncPtr)&nglContext::glNewObjectBufferATI },
/* 1227 */ { "glIsObjectBufferATI", (GLExtFuncPtr)&nglContext::glIsObjectBufferATI },
/* 1228 */ { "glUpdateObjectBufferATI", (GLExtFuncPtr)&nglContext::glUpdateObjectBufferATI },
/* 1229 */ { "glGetObjectBufferfvATI", (GLExtFuncPtr)&nglContext::glGetObjectBufferfvATI },
/* 1230 */ { "glGetObjectBufferivATI", (GLExtFuncPtr)&nglContext::glGetObjectBufferivATI },
/* 1231 */ { "glFreeObjectBufferATI", (GLExtFuncPtr)&nglContext::glFreeObjectBufferATI },
/* 1232 */ { "glArrayObjectATI", (GLExtFuncPtr)&nglContext::glArrayObjectATI },
/* 1233 */ { "glGetArrayObjectfvATI", (GLExtFuncPtr)&nglContext::glGetArrayObjectfvATI },
/* 1234 */ { "glGetArrayObjectivATI", (GLExtFuncPtr)&nglContext::glGetArrayObjectivATI },
/* 1235 */ { "glVariantArrayObjectATI", (GLExtFuncPtr)&nglContext::glVariantArrayObjectATI },
/* 1236 */ { "glGetVariantArrayObjectfvATI", (GLExtFuncPtr)&nglContext::glGetVariantArrayObjectfvATI },
/* 1237 */ { "glGetVariantArrayObjectivATI", (GLExtFuncPtr)&nglContext::glGetVariantArrayObjectivATI },

/* GL_EXT_vertex_shader
 */
/* 1238 */ { "glBeginVertexShaderEXT", (GLExtFuncPtr)&nglContext::glBeginVertexShaderEXT },
/* 1239 */ { "glEndVertexShaderEXT", (GLExtFuncPtr)&nglContext::glEndVertexShaderEXT },
/* 1240 */ { "glBindVertexShaderEXT", (GLExtFuncPtr)&nglContext::glBindVertexShaderEXT },
/* 1241 */ { "glGenVertexShadersEXT", (GLExtFuncPtr)&nglContext::glGenVertexShadersEXT },
/* 1242 */ { "glDeleteVertexShaderEXT", (GLExtFuncPtr)&nglContext::glDeleteVertexShaderEXT },
/* 1243 */ { "glShaderOp1EXT", (GLExtFuncPtr)&nglContext::glShaderOp1EXT },
/* 1244 */ { "glShaderOp2EXT", (GLExtFuncPtr)&nglContext::glShaderOp2EXT },
/* 1245 */ { "glShaderOp3EXT", (GLExtFuncPtr)&nglContext::glShaderOp3EXT },
/* 1246 */ { "glSwizzleEXT", (GLExtFuncPtr)&nglContext::glSwizzleEXT },
/* 1247 */ { "glWriteMaskEXT", (GLExtFuncPtr)&nglContext::glWriteMaskEXT },
/* 1248 */ { "glInsertComponentEXT", (GLExtFuncPtr)&nglContext::glInsertComponentEXT },
/* 1249 */ { "glExtractComponentEXT", (GLExtFuncPtr)&nglContext::glExtractComponentEXT },
/* 1250 */ { "glGenSymbolsEXT", (GLExtFuncPtr)&nglContext::glGenSymbolsEXT },
/* 1251 */ { "glSetInvariantEXT", (GLExtFuncPtr)&nglContext::glSetInvariantEXT },
/* 1252 */ { "glSetLocalConstantEXT", (GLExtFuncPtr)&nglContext::glSetLocalConstantEXT },
/* 1253 */ { "glVariantbvEXT", (GLExtFuncPtr)&nglContext::glVariantbvEXT },
/* 1254 */ { "glVariantsvEXT", (GLExtFuncPtr)&nglContext::glVariantsvEXT },
/* 1255 */ { "glVariantivEXT", (GLExtFuncPtr)&nglContext::glVariantivEXT },
/* 1256 */ { "glVariantfvEXT", (GLExtFuncPtr)&nglContext::glVariantfvEXT },
/* 1257 */ { "glVariantdvEXT", (GLExtFuncPtr)&nglContext::glVariantdvEXT },
/* 1258 */ { "glVariantubvEXT", (GLExtFuncPtr)&nglContext::glVariantubvEXT },
/* 1259 */ { "glVariantusvEXT", (GLExtFuncPtr)&nglContext::glVariantusvEXT },
/* 1260 */ { "glVariantuivEXT", (GLExtFuncPtr)&nglContext::glVariantuivEXT },
/* 1261 */ { "glVariantPointerEXT", (GLExtFuncPtr)&nglContext::glVariantPointerEXT },
/* 1262 */ { "glEnableVariantClientStateEXT", (GLExtFuncPtr)&nglContext::glEnableVariantClientStateEXT },
/* 1263 */ { "glDisableVariantClientStateEXT", (GLExtFuncPtr)&nglContext::glDisableVariantClientStateEXT },
/* 1264 */ { "glBindLightParameterEXT", (GLExtFuncPtr)&nglContext::glBindLightParameterEXT },
/* 1265 */ { "glBindMaterialParameterEXT", (GLExtFuncPtr)&nglContext::glBindMaterialParameterEXT },
/* 1266 */ { "glBindTexGenParameterEXT", (GLExtFuncPtr)&nglContext::glBindTexGenParameterEXT },
/* 1267 */ { "glBindTextureUnitParameterEXT", (GLExtFuncPtr)&nglContext::glBindTextureUnitParameterEXT },
/* 1268 */ { "glBindParameterEXT", (GLExtFuncPtr)&nglContext::glBindParameterEXT },
/* 1269 */ { "glIsVariantEnabledEXT", (GLExtFuncPtr)&nglContext::glIsVariantEnabledEXT },
/* 1270 */ { "glGetVariantBooleanvEXT", (GLExtFuncPtr)&nglContext::glGetVariantBooleanvEXT },
/* 1271 */ { "glGetVariantIntegervEXT", (GLExtFuncPtr)&nglContext::glGetVariantIntegervEXT },
/* 1272 */ { "glGetVariantFloatvEXT", (GLExtFuncPtr)&nglContext::glGetVariantFloatvEXT },
/* 1273 */ { "glGetVariantPointervEXT", (GLExtFuncPtr)&nglContext::glGetVariantPointervEXT },
/* 1274 */ { "glGetInvariantBooleanvEXT", (GLExtFuncPtr)&nglContext::glGetInvariantBooleanvEXT },
/* 1275 */ { "glGetInvariantIntegervEXT", (GLExtFuncPtr)&nglContext::glGetInvariantIntegervEXT },
/* 1276 */ { "glGetInvariantFloatvEXT", (GLExtFuncPtr)&nglContext::glGetInvariantFloatvEXT },
/* 1277 */ { "glGetLocalConstantBooleanvEXT", (GLExtFuncPtr)&nglContext::glGetLocalConstantBooleanvEXT },
/* 1278 */ { "glGetLocalConstantIntegervEXT", (GLExtFuncPtr)&nglContext::glGetLocalConstantIntegervEXT },
/* 1279 */ { "glGetLocalConstantFloatvEXT", (GLExtFuncPtr)&nglContext::glGetLocalConstantFloatvEXT },

/* GL_ATI_vertex_streams
 */
/* 1280 */ { "glVertexStream1sATI", (GLExtFuncPtr)&nglContext::glVertexStream1sATI },
/* 1281 */ { "glVertexStream1svATI", (GLExtFuncPtr)&nglContext::glVertexStream1svATI },
/* 1282 */ { "glVertexStream1iATI", (GLExtFuncPtr)&nglContext::glVertexStream1iATI },
/* 1283 */ { "glVertexStream1ivATI", (GLExtFuncPtr)&nglContext::glVertexStream1ivATI },
/* 1284 */ { "glVertexStream1fATI", (GLExtFuncPtr)&nglContext::glVertexStream1fATI },
/* 1285 */ { "glVertexStream1fvATI", (GLExtFuncPtr)&nglContext::glVertexStream1fvATI },
/* 1286 */ { "glVertexStream1dATI", (GLExtFuncPtr)&nglContext::glVertexStream1dATI },
/* 1287 */ { "glVertexStream1dvATI", (GLExtFuncPtr)&nglContext::glVertexStream1dvATI },
/* 1288 */ { "glVertexStream2sATI", (GLExtFuncPtr)&nglContext::glVertexStream2sATI },
/* 1289 */ { "glVertexStream2svATI", (GLExtFuncPtr)&nglContext::glVertexStream2svATI },
/* 1290 */ { "glVertexStream2iATI", (GLExtFuncPtr)&nglContext::glVertexStream2iATI },
/* 1291 */ { "glVertexStream2ivATI", (GLExtFuncPtr)&nglContext::glVertexStream2ivATI },
/* 1292 */ { "glVertexStream2fATI", (GLExtFuncPtr)&nglContext::glVertexStream2fATI },
/* 1293 */ { "glVertexStream2fvATI", (GLExtFuncPtr)&nglContext::glVertexStream2fvATI },
/* 1294 */ { "glVertexStream2dATI", (GLExtFuncPtr)&nglContext::glVertexStream2dATI },
/* 1295 */ { "glVertexStream2dvATI", (GLExtFuncPtr)&nglContext::glVertexStream2dvATI },
/* 1296 */ { "glVertexStream3sATI", (GLExtFuncPtr)&nglContext::glVertexStream3sATI },
/* 1297 */ { "glVertexStream3svATI", (GLExtFuncPtr)&nglContext::glVertexStream3svATI },
/* 1298 */ { "glVertexStream3iATI", (GLExtFuncPtr)&nglContext::glVertexStream3iATI },
/* 1299 */ { "glVertexStream3ivATI", (GLExtFuncPtr)&nglContext::glVertexStream3ivATI },
/* 1300 */ { "glVertexStream3fATI", (GLExtFuncPtr)&nglContext::glVertexStream3fATI },
/* 1301 */ { "glVertexStream3fvATI", (GLExtFuncPtr)&nglContext::glVertexStream3fvATI },
/* 1302 */ { "glVertexStream3dATI", (GLExtFuncPtr)&nglContext::glVertexStream3dATI },
/* 1303 */ { "glVertexStream3dvATI", (GLExtFuncPtr)&nglContext::glVertexStream3dvATI },
/* 1304 */ { "glVertexStream4sATI", (GLExtFuncPtr)&nglContext::glVertexStream4sATI },
/* 1305 */ { "glVertexStream4svATI", (GLExtFuncPtr)&nglContext::glVertexStream4svATI },
/* 1306 */ { "glVertexStream4iATI", (GLExtFuncPtr)&nglContext::glVertexStream4iATI },
/* 1307 */ { "glVertexStream4ivATI", (GLExtFuncPtr)&nglContext::glVertexStream4ivATI },
/* 1308 */ { "glVertexStream4fATI", (GLExtFuncPtr)&nglContext::glVertexStream4fATI },
/* 1309 */ { "glVertexStream4fvATI", (GLExtFuncPtr)&nglContext::glVertexStream4fvATI },
/* 1310 */ { "glVertexStream4dATI", (GLExtFuncPtr)&nglContext::glVertexStream4dATI },
/* 1311 */ { "glVertexStream4dvATI", (GLExtFuncPtr)&nglContext::glVertexStream4dvATI },
/* 1312 */ { "glNormalStream3bATI", (GLExtFuncPtr)&nglContext::glNormalStream3bATI },
/* 1313 */ { "glNormalStream3bvATI", (GLExtFuncPtr)&nglContext::glNormalStream3bvATI },
/* 1314 */ { "glNormalStream3sATI", (GLExtFuncPtr)&nglContext::glNormalStream3sATI },
/* 1315 */ { "glNormalStream3svATI", (GLExtFuncPtr)&nglContext::glNormalStream3svATI },
/* 1316 */ { "glNormalStream3iATI", (GLExtFuncPtr)&nglContext::glNormalStream3iATI },
/* 1317 */ { "glNormalStream3ivATI", (GLExtFuncPtr)&nglContext::glNormalStream3ivATI },
/* 1318 */ { "glNormalStream3fATI", (GLExtFuncPtr)&nglContext::glNormalStream3fATI },
/* 1319 */ { "glNormalStream3fvATI", (GLExtFuncPtr)&nglContext::glNormalStream3fvATI },
/* 1320 */ { "glNormalStream3dATI", (GLExtFuncPtr)&nglContext::glNormalStream3dATI },
/* 1321 */ { "glNormalStream3dvATI", (GLExtFuncPtr)&nglContext::glNormalStream3dvATI },
/* 1322 */ { "glClientActiveVertexStreamATI", (GLExtFuncPtr)&nglContext::glClientActiveVertexStreamATI },
/* 1323 */ { "glVertexBlendEnviATI", (GLExtFuncPtr)&nglContext::glVertexBlendEnviATI },
/* 1324 */ { "glVertexBlendEnvfATI", (GLExtFuncPtr)&nglContext::glVertexBlendEnvfATI },

/* GL_ATI_element_array
 */
/* 1325 */ { "glElementPointerATI", (GLExtFuncPtr)&nglContext::glElementPointerATI },
/* 1326 */ { "glDrawElementArrayATI", (GLExtFuncPtr)&nglContext::glDrawElementArrayATI },
/* 1327 */ { "glDrawRangeElementArrayATI", (GLExtFuncPtr)&nglContext::glDrawRangeElementArrayATI },

/* GL_SUN_mesh_array
 */
/* 1328 */ { "glDrawMeshArraysSUN", (GLExtFuncPtr)&nglContext::glDrawMeshArraysSUN },

/* GL_NV_occlusion_query
 */
/* 1329 */ { "glGenOcclusionQueriesNV", (GLExtFuncPtr)&nglContext::glGenOcclusionQueriesNV },
/* 1330 */ { "glDeleteOcclusionQueriesNV", (GLExtFuncPtr)&nglContext::glDeleteOcclusionQueriesNV },
/* 1331 */ { "glIsOcclusionQueryNV", (GLExtFuncPtr)&nglContext::glIsOcclusionQueryNV },
/* 1332 */ { "glBeginOcclusionQueryNV", (GLExtFuncPtr)&nglContext::glBeginOcclusionQueryNV },
/* 1333 */ { "glEndOcclusionQueryNV", (GLExtFuncPtr)&nglContext::glEndOcclusionQueryNV },
/* 1334 */ { "glGetOcclusionQueryivNV", (GLExtFuncPtr)&nglContext::glGetOcclusionQueryivNV },
/* 1335 */ { "glGetOcclusionQueryuivNV", (GLExtFuncPtr)&nglContext::glGetOcclusionQueryuivNV },

/* GL_NV_point_sprite
 */
/* 1336 */ { "glPointParameteriNV", (GLExtFuncPtr)&nglContext::glPointParameteriNV },
/* 1337 */ { "glPointParameterivNV", (GLExtFuncPtr)&nglContext::glPointParameterivNV },

/* GL_EXT_stencil_two_side
 */
/* 1338 */ { "glActiveStencilFaceEXT", (GLExtFuncPtr)&nglContext::glActiveStencilFaceEXT },

/* GL_APPLE_element_array
 */
/* 1339 */ { "glElementPointerAPPLE", (GLExtFuncPtr)&nglContext::glElementPointerAPPLE },
/* 1340 */ { "glDrawElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glDrawElementArrayAPPLE },
/* 1341 */ { "glDrawRangeElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glDrawRangeElementArrayAPPLE },
/* 1342 */ { "glMultiDrawElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glMultiDrawElementArrayAPPLE },
/* 1343 */ { "glMultiDrawRangeElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glMultiDrawRangeElementArrayAPPLE },

/* GL_APPLE_fence
 */
/* 1344 */ { "glGenFencesAPPLE", (GLExtFuncPtr)&nglContext::glGenFencesAPPLE },
/* 1345 */ { "glDeleteFencesAPPLE", (GLExtFuncPtr)&nglContext::glDeleteFencesAPPLE },
/* 1346 */ { "glSetFenceAPPLE", (GLExtFuncPtr)&nglContext::glSetFenceAPPLE },
/* 1347 */ { "glIsFenceAPPLE", (GLExtFuncPtr)&nglContext::glIsFenceAPPLE },
/* 1348 */ { "glTestFenceAPPLE", (GLExtFuncPtr)&nglContext::glTestFenceAPPLE },
/* 1349 */ { "glFinishFenceAPPLE", (GLExtFuncPtr)&nglContext::glFinishFenceAPPLE },
/* 1350 */ { "glTestObjectAPPLE", (GLExtFuncPtr)&nglContext::glTestObjectAPPLE },
/* 1351 */ { "glFinishObjectAPPLE", (GLExtFuncPtr)&nglContext::glFinishObjectAPPLE },

/* GL_APPLE_vertex_array_object
 */
/* 1352 */ { "glBindVertexArrayAPPLE", (GLExtFuncPtr)&nglContext::glBindVertexArrayAPPLE },
/* 1353 */ { "glDeleteVertexArraysAPPLE", (GLExtFuncPtr)&nglContext::glDeleteVertexArraysAPPLE },
/* 1354 */ { "glGenVertexArraysAPPLE", (GLExtFuncPtr)&nglContext::glGenVertexArraysAPPLE },
/* 1355 */ { "glIsVertexArrayAPPLE", (GLExtFuncPtr)&nglContext::glIsVertexArrayAPPLE },

/* GL_APPLE_vertex_array_range
 */
/* 1356 */ { "glVertexArrayRangeAPPLE", (GLExtFuncPtr)&nglContext::glVertexArrayRangeAPPLE },
/* 1357 */ { "glFlushVertexArrayRangeAPPLE", (GLExtFuncPtr)&nglContext::glFlushVertexArrayRangeAPPLE },
/* 1358 */ { "glVertexArrayParameteriAPPLE", (GLExtFuncPtr)&nglContext::glVertexArrayParameteriAPPLE },

/* GL_ATI_draw_buffers
 */
/* 1359 */ { "glDrawBuffersATI", (GLExtFuncPtr)&nglContext::glDrawBuffersATI },

/* GL_NV_fragment_program
 */
/* 1360 */ { "glProgramNamedParameter4fNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4fNV },
/* 1361 */ { "glProgramNamedParameter4dNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4dNV },
/* 1362 */ { "glProgramNamedParameter4fvNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4fvNV },
/* 1363 */ { "glProgramNamedParameter4dvNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4dvNV },
/* 1364 */ { "glGetProgramNamedParameterfvNV", (GLExtFuncPtr)&nglContext::glGetProgramNamedParameterfvNV },
/* 1365 */ { "glGetProgramNamedParameterdvNV", (GLExtFuncPtr)&nglContext::glGetProgramNamedParameterdvNV },

/* GL_NV_half_float
 */
/* 1366 */ { "glVertex2hNV", (GLExtFuncPtr)&nglContext::glVertex2hNV },
/* 1367 */ { "glVertex2hvNV", (GLExtFuncPtr)&nglContext::glVertex2hvNV },
/* 1368 */ { "glVertex3hNV", (GLExtFuncPtr)&nglContext::glVertex3hNV },
/* 1369 */ { "glVertex3hvNV", (GLExtFuncPtr)&nglContext::glVertex3hvNV },
/* 1370 */ { "glVertex4hNV", (GLExtFuncPtr)&nglContext::glVertex4hNV },
/* 1371 */ { "glVertex4hvNV", (GLExtFuncPtr)&nglContext::glVertex4hvNV },
/* 1372 */ { "glNormal3hNV", (GLExtFuncPtr)&nglContext::glNormal3hNV },
/* 1373 */ { "glNormal3hvNV", (GLExtFuncPtr)&nglContext::glNormal3hvNV },
/* 1374 */ { "glColor3hNV", (GLExtFuncPtr)&nglContext::glColor3hNV },
/* 1375 */ { "glColor3hvNV", (GLExtFuncPtr)&nglContext::glColor3hvNV },
/* 1376 */ { "glColor4hNV", (GLExtFuncPtr)&nglContext::glColor4hNV },
/* 1377 */ { "glColor4hvNV", (GLExtFuncPtr)&nglContext::glColor4hvNV },
/* 1378 */ { "glTexCoord1hNV", (GLExtFuncPtr)&nglContext::glTexCoord1hNV },
/* 1379 */ { "glTexCoord1hvNV", (GLExtFuncPtr)&nglContext::glTexCoord1hvNV },
/* 1380 */ { "glTexCoord2hNV", (GLExtFuncPtr)&nglContext::glTexCoord2hNV },
/* 1381 */ { "glTexCoord2hvNV", (GLExtFuncPtr)&nglContext::glTexCoord2hvNV },
/* 1382 */ { "glTexCoord3hNV", (GLExtFuncPtr)&nglContext::glTexCoord3hNV },
/* 1383 */ { "glTexCoord3hvNV", (GLExtFuncPtr)&nglContext::glTexCoord3hvNV },
/* 1384 */ { "glTexCoord4hNV", (GLExtFuncPtr)&nglContext::glTexCoord4hNV },
/* 1385 */ { "glTexCoord4hvNV", (GLExtFuncPtr)&nglContext::glTexCoord4hvNV },
/* 1386 */ { "glMultiTexCoord1hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord1hNV },
/* 1387 */ { "glMultiTexCoord1hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord1hvNV },
/* 1388 */ { "glMultiTexCoord2hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord2hNV },
/* 1389 */ { "glMultiTexCoord2hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord2hvNV },
/* 1390 */ { "glMultiTexCoord3hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord3hNV },
/* 1391 */ { "glMultiTexCoord3hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord3hvNV },
/* 1392 */ { "glMultiTexCoord4hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord4hNV },
/* 1393 */ { "glMultiTexCoord4hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord4hvNV },
/* 1394 */ { "glFogCoordhNV", (GLExtFuncPtr)&nglContext::glFogCoordhNV },
/* 1395 */ { "glFogCoordhvNV", (GLExtFuncPtr)&nglContext::glFogCoordhvNV },
/* 1396 */ { "glSecondaryColor3hNV", (GLExtFuncPtr)&nglContext::glSecondaryColor3hNV },
/* 1397 */ { "glSecondaryColor3hvNV", (GLExtFuncPtr)&nglContext::glSecondaryColor3hvNV },
/* 1398 */ { "glVertexWeighthNV", (GLExtFuncPtr)&nglContext::glVertexWeighthNV },
/* 1399 */ { "glVertexWeighthvNV", (GLExtFuncPtr)&nglContext::glVertexWeighthvNV },
/* 1400 */ { "glVertexAttrib1hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1hNV },
/* 1401 */ { "glVertexAttrib1hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1hvNV },
/* 1402 */ { "glVertexAttrib2hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2hNV },
/* 1403 */ { "glVertexAttrib2hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2hvNV },
/* 1404 */ { "glVertexAttrib3hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3hNV },
/* 1405 */ { "glVertexAttrib3hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3hvNV },
/* 1406 */ { "glVertexAttrib4hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4hNV },
/* 1407 */ { "glVertexAttrib4hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4hvNV },
/* 1408 */ { "glVertexAttribs1hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1hvNV },
/* 1409 */ { "glVertexAttribs2hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2hvNV },
/* 1410 */ { "glVertexAttribs3hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3hvNV },
/* 1411 */ { "glVertexAttribs4hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4hvNV },

/* GL_NV_pixel_data_range
 */
/* 1412 */ { "glPixelDataRangeNV", (GLExtFuncPtr)&nglContext::glPixelDataRangeNV },
/* 1413 */ { "glFlushPixelDataRangeNV", (GLExtFuncPtr)&nglContext::glFlushPixelDataRangeNV },

/* GL_NV_primitive_restart
 */
/* 1414 */ { "glPrimitiveRestartNV", (GLExtFuncPtr)&nglContext::glPrimitiveRestartNV },
/* 1415 */ { "glPrimitiveRestartIndexNV", (GLExtFuncPtr)&nglContext::glPrimitiveRestartIndexNV },

/* GL_ATI_map_object_buffer
 */
/* 1416 */ { "glMapObjectBufferATI", (GLExtFuncPtr)&nglContext::glMapObjectBufferATI },
/* 1417 */ { "glUnmapObjectBufferATI", (GLExtFuncPtr)&nglContext::glUnmapObjectBufferATI },

/* GL_ATI_separate_stencil
 */
/* 1418 */ { "glStencilOpSeparateATI", (GLExtFuncPtr)&nglContext::glStencilOpSeparateATI },
/* 1419 */ { "glStencilFuncSeparateATI", (GLExtFuncPtr)&nglContext::glStencilFuncSeparateATI },

/* GL_ATI_vertex_attrib_array_object
 */
/* 1420 */ { "glVertexAttribArrayObjectATI", (GLExtFuncPtr)&nglContext::glVertexAttribArrayObjectATI },
/* 1421 */ { "glGetVertexAttribArrayObjectfvATI", (GLExtFuncPtr)&nglContext::glGetVertexAttribArrayObjectfvATI },
/* 1422 */ { "glGetVertexAttribArrayObjectivATI", (GLExtFuncPtr)&nglContext::glGetVertexAttribArrayObjectivATI },

/* GL_EXT_depth_bounds_test
 */
/* 1423 */ { "glDepthBoundsEXT", (GLExtFuncPtr)&nglContext::glDepthBoundsEXT },

/* GL_EXT_blend_equation_separate
 */
/* 1424 */ { "glBlendEquationSeparateEXT", (GLExtFuncPtr)&nglContext::glBlendEquationSeparateEXT },

/* GL_EXT_framebuffer_object
 */
/* 1425 */ { "glIsRenderbufferEXT", (GLExtFuncPtr)&nglContext::glIsRenderbufferEXT },
/* 1426 */ { "glBindRenderbufferEXT", (GLExtFuncPtr)&nglContext::glBindRenderbufferEXT },
/* 1427 */ { "glDeleteRenderbuffersEXT", (GLExtFuncPtr)&nglContext::glDeleteRenderbuffersEXT },
/* 1428 */ { "glGenRenderbuffersEXT", (GLExtFuncPtr)&nglContext::glGenRenderbuffersEXT },
/* 1429 */ { "glRenderbufferStorageEXT", (GLExtFuncPtr)&nglContext::glRenderbufferStorageEXT },
/* 1430 */ { "glGetRenderbufferParameterivEXT", (GLExtFuncPtr)&nglContext::glGetRenderbufferParameterivEXT },
/* 1431 */ { "glIsFramebufferEXT", (GLExtFuncPtr)&nglContext::glIsFramebufferEXT },
/* 1432 */ { "glBindFramebufferEXT", (GLExtFuncPtr)&nglContext::glBindFramebufferEXT },
/* 1433 */ { "glDeleteFramebuffersEXT", (GLExtFuncPtr)&nglContext::glDeleteFramebuffersEXT },
/* 1434 */ { "glGenFramebuffersEXT", (GLExtFuncPtr)&nglContext::glGenFramebuffersEXT },
/* 1435 */ { "glCheckFramebufferStatusEXT", (GLExtFuncPtr)&nglContext::glCheckFramebufferStatusEXT },
/* 1436 */ { "glFramebufferTexture1DEXT", (GLExtFuncPtr)&nglContext::glFramebufferTexture1DEXT },
/* 1437 */ { "glFramebufferTexture2DEXT", (GLExtFuncPtr)&nglContext::glFramebufferTexture2DEXT },
/* 1438 */ { "glFramebufferTexture3DEXT", (GLExtFuncPtr)&nglContext::glFramebufferTexture3DEXT },
/* 1439 */ { "glFramebufferRenderbufferEXT", (GLExtFuncPtr)&nglContext::glFramebufferRenderbufferEXT },
/* 1440 */ { "glGetFramebufferAttachmentParameterivEXT", (GLExtFuncPtr)&nglContext::glGetFramebufferAttachmentParameterivEXT },
/* 1441 */ { "glGenerateMipmapEXT", (GLExtFuncPtr)&nglContext::glGenerateMipmapEXT },

/* GL_GREMEDY_string_marker
 */
/* 1442 */ { "glStringMarkerGREMEDY", (GLExtFuncPtr)&nglContext::glStringMarkerGREMEDY },

/* GL_EXT_stencil_clear_tag
 */
/* 1443 */ { "glStencilClearTagEXT", (GLExtFuncPtr)&nglContext::glStencilClearTagEXT },

/* GL_EXT_framebuffer_blit
 */
/* 1444 */ { "glBlitFramebufferEXT", (GLExtFuncPtr)&nglContext::glBlitFramebufferEXT },

/* GL_EXT_framebuffer_multisample
 */
/* 1445 */ { "glRenderbufferStorageMultisampleEXT", (GLExtFuncPtr)&nglContext::glRenderbufferStorageMultisampleEXT },

/* GL_EXT_timer_query
 */
/* 1446 */ { "glGetQueryObjecti64vEXT", (GLExtFuncPtr)&nglContext::glGetQueryObjecti64vEXT },
/* 1447 */ { "glGetQueryObjectui64vEXT", (GLExtFuncPtr)&nglContext::glGetQueryObjectui64vEXT },

/* GL_EXT_gpu_program_parameters
 */
/* 1448 */ { "glProgramEnvParameters4fvEXT", (GLExtFuncPtr)&nglContext::glProgramEnvParameters4fvEXT },
/* 1449 */ { "glProgramLocalParameters4fvEXT", (GLExtFuncPtr)&nglContext::glProgramLocalParameters4fvEXT },

/* GL_APPLE_flush_buffer_range
 */
/* 1450 */ { "glBufferParameteriAPPLE", (GLExtFuncPtr)&nglContext::glBufferParameteriAPPLE },
/* 1451 */ { "glFlushMappedBufferRangeAPPLE", (GLExtFuncPtr)&nglContext::glFlushMappedBufferRangeAPPLE },

/* GL_NV_gpu_program4
 */
/* 1452 */ { "glProgramLocalParameterI4iNV", (GLExtFuncPtr)&nglContext::glProgramLocalParameterI4iNV },
/* 1453 */ { "glProgramLocalParameterI4ivNV", (GLExtFuncPtr)&nglContext::glProgramLocalParameterI4ivNV },
/* 1454 */ { "glProgramLocalParametersI4ivNV", (GLExtFuncPtr)&nglContext::glProgramLocalParametersI4ivNV },
/* 1455 */ { "glProgramLocalParameterI4uiNV", (GLExtFuncPtr)&nglContext::glProgramLocalParameterI4uiNV },
/* 1456 */ { "glProgramLocalParameterI4uivNV", (GLExtFuncPtr)&nglContext::glProgramLocalParameterI4uivNV },
/* 1457 */ { "glProgramLocalParametersI4uivNV", (GLExtFuncPtr)&nglContext::glProgramLocalParametersI4uivNV },
/* 1458 */ { "glProgramEnvParameterI4iNV", (GLExtFuncPtr)&nglContext::glProgramEnvParameterI4iNV },
/* 1459 */ { "glProgramEnvParameterI4ivNV", (GLExtFuncPtr)&nglContext::glProgramEnvParameterI4ivNV },
/* 1460 */ { "glProgramEnvParametersI4ivNV", (GLExtFuncPtr)&nglContext::glProgramEnvParametersI4ivNV },
/* 1461 */ { "glProgramEnvParameterI4uiNV", (GLExtFuncPtr)&nglContext::glProgramEnvParameterI4uiNV },
/* 1462 */ { "glProgramEnvParameterI4uivNV", (GLExtFuncPtr)&nglContext::glProgramEnvParameterI4uivNV },
/* 1463 */ { "glProgramEnvParametersI4uivNV", (GLExtFuncPtr)&nglContext::glProgramEnvParametersI4uivNV },
/* 1464 */ { "glGetProgramLocalParameterIivNV", (GLExtFuncPtr)&nglContext::glGetProgramLocalParameterIivNV },
/* 1465 */ { "glGetProgramLocalParameterIuivNV", (GLExtFuncPtr)&nglContext::glGetProgramLocalParameterIuivNV },
/* 1466 */ { "glGetProgramEnvParameterIivNV", (GLExtFuncPtr)&nglContext::glGetProgramEnvParameterIivNV },
/* 1467 */ { "glGetProgramEnvParameterIuivNV", (GLExtFuncPtr)&nglContext::glGetProgramEnvParameterIuivNV },

/* GL_NV_geometry_program4
 */
/* 1468 */ { "glProgramVertexLimitNV", (GLExtFuncPtr)&nglContext::glProgramVertexLimitNV },
/* 1469 */ { "glFramebufferTextureEXT", (GLExtFuncPtr)&nglContext::glFramebufferTextureEXT },
/* 1470 */ { "glFramebufferTextureLayerEXT", (GLExtFuncPtr)&nglContext::glFramebufferTextureLayerEXT },
/* 1471 */ { "glFramebufferTextureFaceEXT", (GLExtFuncPtr)&nglContext::glFramebufferTextureFaceEXT },

/* GL_EXT_geometry_shader4
 */
/* 1472 */ { "glProgramParameteriEXT", (GLExtFuncPtr)&nglContext::glProgramParameteriEXT },

/* GL_NV_vertex_program4
 */
/* 1473 */ { "glVertexAttribI1iEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI1iEXT },
/* 1474 */ { "glVertexAttribI2iEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI2iEXT },
/* 1475 */ { "glVertexAttribI3iEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI3iEXT },
/* 1476 */ { "glVertexAttribI4iEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4iEXT },
/* 1477 */ { "glVertexAttribI1uiEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI1uiEXT },
/* 1478 */ { "glVertexAttribI2uiEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI2uiEXT },
/* 1479 */ { "glVertexAttribI3uiEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI3uiEXT },
/* 1480 */ { "glVertexAttribI4uiEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4uiEXT },
/* 1481 */ { "glVertexAttribI1ivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI1ivEXT },
/* 1482 */ { "glVertexAttribI2ivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI2ivEXT },
/* 1483 */ { "glVertexAttribI3ivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI3ivEXT },
/* 1484 */ { "glVertexAttribI4ivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4ivEXT },
/* 1485 */ { "glVertexAttribI1uivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI1uivEXT },
/* 1486 */ { "glVertexAttribI2uivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI2uivEXT },
/* 1487 */ { "glVertexAttribI3uivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI3uivEXT },
/* 1488 */ { "glVertexAttribI4uivEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4uivEXT },
/* 1489 */ { "glVertexAttribI4bvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4bvEXT },
/* 1490 */ { "glVertexAttribI4svEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4svEXT },
/* 1491 */ { "glVertexAttribI4ubvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4ubvEXT },
/* 1492 */ { "glVertexAttribI4usvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribI4usvEXT },
/* 1493 */ { "glVertexAttribIPointerEXT", (GLExtFuncPtr)&nglContext::glVertexAttribIPointerEXT },
/* 1494 */ { "glGetVertexAttribIivEXT", (GLExtFuncPtr)&nglContext::glGetVertexAttribIivEXT },
/* 1495 */ { "glGetVertexAttribIuivEXT", (GLExtFuncPtr)&nglContext::glGetVertexAttribIuivEXT },

/* GL_EXT_gpu_shader4
 */
/* 1496 */ { "glGetUniformuivEXT", (GLExtFuncPtr)&nglContext::glGetUniformuivEXT },
/* 1497 */ { "glBindFragDataLocationEXT", (GLExtFuncPtr)&nglContext::glBindFragDataLocationEXT },
/* 1498 */ { "glGetFragDataLocationEXT", (GLExtFuncPtr)&nglContext::glGetFragDataLocationEXT },
/* 1499 */ { "glUniform1uiEXT", (GLExtFuncPtr)&nglContext::glUniform1uiEXT },
/* 1500 */ { "glUniform2uiEXT", (GLExtFuncPtr)&nglContext::glUniform2uiEXT },
/* 1501 */ { "glUniform3uiEXT", (GLExtFuncPtr)&nglContext::glUniform3uiEXT },
/* 1502 */ { "glUniform4uiEXT", (GLExtFuncPtr)&nglContext::glUniform4uiEXT },
/* 1503 */ { "glUniform1uivEXT", (GLExtFuncPtr)&nglContext::glUniform1uivEXT },
/* 1504 */ { "glUniform2uivEXT", (GLExtFuncPtr)&nglContext::glUniform2uivEXT },
/* 1505 */ { "glUniform3uivEXT", (GLExtFuncPtr)&nglContext::glUniform3uivEXT },
/* 1506 */ { "glUniform4uivEXT", (GLExtFuncPtr)&nglContext::glUniform4uivEXT },

/* GL_EXT_draw_instanced
 */
/* 1507 */ { "glDrawArraysInstancedEXT", (GLExtFuncPtr)&nglContext::glDrawArraysInstancedEXT },
/* 1508 */ { "glDrawElementsInstancedEXT", (GLExtFuncPtr)&nglContext::glDrawElementsInstancedEXT },

/* GL_EXT_texture_buffer_object
 */
/* 1509 */ { "glTexBufferEXT", (GLExtFuncPtr)&nglContext::glTexBufferEXT },

/* GL_NV_depth_buffer_float
 */
/* 1510 */ { "glDepthRangedNV", (GLExtFuncPtr)&nglContext::glDepthRangedNV },
/* 1511 */ { "glClearDepthdNV", (GLExtFuncPtr)&nglContext::glClearDepthdNV },
/* 1512 */ { "glDepthBoundsdNV", (GLExtFuncPtr)&nglContext::glDepthBoundsdNV },

/* GL_NV_framebuffer_multisample_coverage
 */
/* 1513 */ { "glRenderbufferStorageMultisampleCoverageNV", (GLExtFuncPtr)&nglContext::glRenderbufferStorageMultisampleCoverageNV },

/* GL_NV_parameter_buffer_object
 */
/* 1514 */ { "glProgramBufferParametersfvNV", (GLExtFuncPtr)&nglContext::glProgramBufferParametersfvNV },
/* 1515 */ { "glProgramBufferParametersIivNV", (GLExtFuncPtr)&nglContext::glProgramBufferParametersIivNV },
/* 1516 */ { "glProgramBufferParametersIuivNV", (GLExtFuncPtr)&nglContext::glProgramBufferParametersIuivNV },

/* GL_EXT_draw_buffers2
 */
/* 1517 */ { "glColorMaskIndexedEXT", (GLExtFuncPtr)&nglContext::glColorMaskIndexedEXT },
/* 1518 */ { "glGetBooleanIndexedvEXT", (GLExtFuncPtr)&nglContext::glGetBooleanIndexedvEXT },
/* 1519 */ { "glGetIntegerIndexedvEXT", (GLExtFuncPtr)&nglContext::glGetIntegerIndexedvEXT },
/* 1520 */ { "glEnableIndexedEXT", (GLExtFuncPtr)&nglContext::glEnableIndexedEXT },
/* 1521 */ { "glDisableIndexedEXT", (GLExtFuncPtr)&nglContext::glDisableIndexedEXT },
/* 1522 */ { "glIsEnabledIndexedEXT", (GLExtFuncPtr)&nglContext::glIsEnabledIndexedEXT },

/* GL_NV_transform_feedback
 */
/* 1523 */ { "glBeginTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glBeginTransformFeedbackNV },
/* 1524 */ { "glEndTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glEndTransformFeedbackNV },
/* 1525 */ { "glTransformFeedbackAttribsNV", (GLExtFuncPtr)&nglContext::glTransformFeedbackAttribsNV },
/* 1526 */ { "glBindBufferRangeNV", (GLExtFuncPtr)&nglContext::glBindBufferRangeNV },
/* 1527 */ { "glBindBufferOffsetNV", (GLExtFuncPtr)&nglContext::glBindBufferOffsetNV },
/* 1528 */ { "glBindBufferBaseNV", (GLExtFuncPtr)&nglContext::glBindBufferBaseNV },
/* 1529 */ { "glTransformFeedbackVaryingsNV", (GLExtFuncPtr)&nglContext::glTransformFeedbackVaryingsNV },
/* 1530 */ { "glActiveVaryingNV", (GLExtFuncPtr)&nglContext::glActiveVaryingNV },
/* 1531 */ { "glGetVaryingLocationNV", (GLExtFuncPtr)&nglContext::glGetVaryingLocationNV },
/* 1532 */ { "glGetActiveVaryingNV", (GLExtFuncPtr)&nglContext::glGetActiveVaryingNV },
/* 1533 */ { "glGetTransformFeedbackVaryingNV", (GLExtFuncPtr)&nglContext::glGetTransformFeedbackVaryingNV },
/* 1534 */ { "glTransformFeedbackStreamAttribsNV", (GLExtFuncPtr)&nglContext::glTransformFeedbackStreamAttribsNV },

/* GL_EXT_bindable_uniform
 */
/* 1535 */ { "glUniformBufferEXT", (GLExtFuncPtr)&nglContext::glUniformBufferEXT },
/* 1536 */ { "glGetUniformBufferSizeEXT", (GLExtFuncPtr)&nglContext::glGetUniformBufferSizeEXT },
/* 1537 */ { "glGetUniformOffsetEXT", (GLExtFuncPtr)&nglContext::glGetUniformOffsetEXT },

/* GL_EXT_texture_integer
 */
/* 1538 */ { "glTexParameterIivEXT", (GLExtFuncPtr)&nglContext::glTexParameterIivEXT },
/* 1539 */ { "glTexParameterIuivEXT", (GLExtFuncPtr)&nglContext::glTexParameterIuivEXT },
/* 1540 */ { "glGetTexParameterIivEXT", (GLExtFuncPtr)&nglContext::glGetTexParameterIivEXT },
/* 1541 */ { "glGetTexParameterIuivEXT", (GLExtFuncPtr)&nglContext::glGetTexParameterIuivEXT },
/* 1542 */ { "glClearColorIiEXT", (GLExtFuncPtr)&nglContext::glClearColorIiEXT },
/* 1543 */ { "glClearColorIuiEXT", (GLExtFuncPtr)&nglContext::glClearColorIuiEXT },

/* GL_GREMEDY_frame_terminator
 */
/* 1544 */ { "glFrameTerminatorGREMEDY", (GLExtFuncPtr)&nglContext::glFrameTerminatorGREMEDY },

/* GL_NV_conditional_render
 */
/* 1545 */ { "glBeginConditionalRenderNV", (GLExtFuncPtr)&nglContext::glBeginConditionalRenderNV },
/* 1546 */ { "glEndConditionalRenderNV", (GLExtFuncPtr)&nglContext::glEndConditionalRenderNV },

/* GL_NV_present_video
 */
/* 1547 */ { "glPresentFrameKeyedNV", (GLExtFuncPtr)&nglContext::glPresentFrameKeyedNV },
/* 1548 */ { "glPresentFrameDualFillNV", (GLExtFuncPtr)&nglContext::glPresentFrameDualFillNV },
/* 1549 */ { "glGetVideoivNV", (GLExtFuncPtr)&nglContext::glGetVideoivNV },
/* 1550 */ { "glGetVideouivNV", (GLExtFuncPtr)&nglContext::glGetVideouivNV },
/* 1551 */ { "glGetVideoi64vNV", (GLExtFuncPtr)&nglContext::glGetVideoi64vNV },
/* 1552 */ { "glGetVideoui64vNV", (GLExtFuncPtr)&nglContext::glGetVideoui64vNV },

/* GL_EXT_transform_feedback
 */
/* 1553 */ { "glBeginTransformFeedbackEXT", (GLExtFuncPtr)&nglContext::glBeginTransformFeedbackEXT },
/* 1554 */ { "glEndTransformFeedbackEXT", (GLExtFuncPtr)&nglContext::glEndTransformFeedbackEXT },
/* 1555 */ { "glBindBufferRangeEXT", (GLExtFuncPtr)&nglContext::glBindBufferRangeEXT },
/* 1556 */ { "glBindBufferOffsetEXT", (GLExtFuncPtr)&nglContext::glBindBufferOffsetEXT },
/* 1557 */ { "glBindBufferBaseEXT", (GLExtFuncPtr)&nglContext::glBindBufferBaseEXT },
/* 1558 */ { "glTransformFeedbackVaryingsEXT", (GLExtFuncPtr)&nglContext::glTransformFeedbackVaryingsEXT },
/* 1559 */ { "glGetTransformFeedbackVaryingEXT", (GLExtFuncPtr)&nglContext::glGetTransformFeedbackVaryingEXT },

/* GL_EXT_direct_state_access
 */
/* 1560 */ { "glClientAttribDefaultEXT", (GLExtFuncPtr)&nglContext::glClientAttribDefaultEXT },
/* 1561 */ { "glPushClientAttribDefaultEXT", (GLExtFuncPtr)&nglContext::glPushClientAttribDefaultEXT },
/* 1562 */ { "glMatrixLoadfEXT", (GLExtFuncPtr)&nglContext::glMatrixLoadfEXT },
/* 1563 */ { "glMatrixLoaddEXT", (GLExtFuncPtr)&nglContext::glMatrixLoaddEXT },
/* 1564 */ { "glMatrixMultfEXT", (GLExtFuncPtr)&nglContext::glMatrixMultfEXT },
/* 1565 */ { "glMatrixMultdEXT", (GLExtFuncPtr)&nglContext::glMatrixMultdEXT },
/* 1566 */ { "glMatrixLoadIdentityEXT", (GLExtFuncPtr)&nglContext::glMatrixLoadIdentityEXT },
/* 1567 */ { "glMatrixRotatefEXT", (GLExtFuncPtr)&nglContext::glMatrixRotatefEXT },
/* 1568 */ { "glMatrixRotatedEXT", (GLExtFuncPtr)&nglContext::glMatrixRotatedEXT },
/* 1569 */ { "glMatrixScalefEXT", (GLExtFuncPtr)&nglContext::glMatrixScalefEXT },
/* 1570 */ { "glMatrixScaledEXT", (GLExtFuncPtr)&nglContext::glMatrixScaledEXT },
/* 1571 */ { "glMatrixTranslatefEXT", (GLExtFuncPtr)&nglContext::glMatrixTranslatefEXT },
/* 1572 */ { "glMatrixTranslatedEXT", (GLExtFuncPtr)&nglContext::glMatrixTranslatedEXT },
/* 1573 */ { "glMatrixFrustumEXT", (GLExtFuncPtr)&nglContext::glMatrixFrustumEXT },
/* 1574 */ { "glMatrixOrthoEXT", (GLExtFuncPtr)&nglContext::glMatrixOrthoEXT },
/* 1575 */ { "glMatrixPopEXT", (GLExtFuncPtr)&nglContext::glMatrixPopEXT },
/* 1576 */ { "glMatrixPushEXT", (GLExtFuncPtr)&nglContext::glMatrixPushEXT },
/* 1577 */ { "glMatrixLoadTransposefEXT", (GLExtFuncPtr)&nglContext::glMatrixLoadTransposefEXT },
/* 1578 */ { "glMatrixLoadTransposedEXT", (GLExtFuncPtr)&nglContext::glMatrixLoadTransposedEXT },
/* 1579 */ { "glMatrixMultTransposefEXT", (GLExtFuncPtr)&nglContext::glMatrixMultTransposefEXT },
/* 1580 */ { "glMatrixMultTransposedEXT", (GLExtFuncPtr)&nglContext::glMatrixMultTransposedEXT },
/* 1581 */ { "glTextureParameterfEXT", (GLExtFuncPtr)&nglContext::glTextureParameterfEXT },
/* 1582 */ { "glTextureParameterfvEXT", (GLExtFuncPtr)&nglContext::glTextureParameterfvEXT },
/* 1583 */ { "glTextureParameteriEXT", (GLExtFuncPtr)&nglContext::glTextureParameteriEXT },
/* 1584 */ { "glTextureParameterivEXT", (GLExtFuncPtr)&nglContext::glTextureParameterivEXT },
/* 1585 */ { "glTextureImage1DEXT", (GLExtFuncPtr)&nglContext::glTextureImage1DEXT },
/* 1586 */ { "glTextureImage2DEXT", (GLExtFuncPtr)&nglContext::glTextureImage2DEXT },
/* 1587 */ { "glTextureSubImage1DEXT", (GLExtFuncPtr)&nglContext::glTextureSubImage1DEXT },
/* 1588 */ { "glTextureSubImage2DEXT", (GLExtFuncPtr)&nglContext::glTextureSubImage2DEXT },
/* 1589 */ { "glCopyTextureImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyTextureImage1DEXT },
/* 1590 */ { "glCopyTextureImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyTextureImage2DEXT },
/* 1591 */ { "glCopyTextureSubImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyTextureSubImage1DEXT },
/* 1592 */ { "glCopyTextureSubImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyTextureSubImage2DEXT },
/* 1593 */ { "glGetTextureImageEXT", (GLExtFuncPtr)&nglContext::glGetTextureImageEXT },
/* 1594 */ { "glGetTextureParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetTextureParameterfvEXT },
/* 1595 */ { "glGetTextureParameterivEXT", (GLExtFuncPtr)&nglContext::glGetTextureParameterivEXT },
/* 1596 */ { "glGetTextureLevelParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetTextureLevelParameterfvEXT },
/* 1597 */ { "glGetTextureLevelParameterivEXT", (GLExtFuncPtr)&nglContext::glGetTextureLevelParameterivEXT },
/* 1598 */ { "glTextureImage3DEXT", (GLExtFuncPtr)&nglContext::glTextureImage3DEXT },
/* 1599 */ { "glTextureSubImage3DEXT", (GLExtFuncPtr)&nglContext::glTextureSubImage3DEXT },
/* 1600 */ { "glCopyTextureSubImage3DEXT", (GLExtFuncPtr)&nglContext::glCopyTextureSubImage3DEXT },
/* 1601 */ { "glMultiTexParameterfEXT", (GLExtFuncPtr)&nglContext::glMultiTexParameterfEXT },
/* 1602 */ { "glMultiTexParameterfvEXT", (GLExtFuncPtr)&nglContext::glMultiTexParameterfvEXT },
/* 1603 */ { "glMultiTexParameteriEXT", (GLExtFuncPtr)&nglContext::glMultiTexParameteriEXT },
/* 1604 */ { "glMultiTexParameterivEXT", (GLExtFuncPtr)&nglContext::glMultiTexParameterivEXT },
/* 1605 */ { "glMultiTexImage1DEXT", (GLExtFuncPtr)&nglContext::glMultiTexImage1DEXT },
/* 1606 */ { "glMultiTexImage2DEXT", (GLExtFuncPtr)&nglContext::glMultiTexImage2DEXT },
/* 1607 */ { "glMultiTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glMultiTexSubImage1DEXT },
/* 1608 */ { "glMultiTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glMultiTexSubImage2DEXT },
/* 1609 */ { "glCopyMultiTexImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyMultiTexImage1DEXT },
/* 1610 */ { "glCopyMultiTexImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyMultiTexImage2DEXT },
/* 1611 */ { "glCopyMultiTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyMultiTexSubImage1DEXT },
/* 1612 */ { "glCopyMultiTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyMultiTexSubImage2DEXT },
/* 1613 */ { "glGetMultiTexImageEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexImageEXT },
/* 1614 */ { "glGetMultiTexParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexParameterfvEXT },
/* 1615 */ { "glGetMultiTexParameterivEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexParameterivEXT },
/* 1616 */ { "glGetMultiTexLevelParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexLevelParameterfvEXT },
/* 1617 */ { "glGetMultiTexLevelParameterivEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexLevelParameterivEXT },
/* 1618 */ { "glMultiTexImage3DEXT", (GLExtFuncPtr)&nglContext::glMultiTexImage3DEXT },
/* 1619 */ { "glMultiTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glMultiTexSubImage3DEXT },
/* 1620 */ { "glCopyMultiTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glCopyMultiTexSubImage3DEXT },
/* 1621 */ { "glBindMultiTextureEXT", (GLExtFuncPtr)&nglContext::glBindMultiTextureEXT },
/* 1622 */ { "glEnableClientStateIndexedEXT", (GLExtFuncPtr)&nglContext::glEnableClientStateIndexedEXT },
/* 1623 */ { "glDisableClientStateIndexedEXT", (GLExtFuncPtr)&nglContext::glDisableClientStateIndexedEXT },
/* 1624 */ { "glMultiTexCoordPointerEXT", (GLExtFuncPtr)&nglContext::glMultiTexCoordPointerEXT },
/* 1625 */ { "glMultiTexEnvfEXT", (GLExtFuncPtr)&nglContext::glMultiTexEnvfEXT },
/* 1626 */ { "glMultiTexEnvfvEXT", (GLExtFuncPtr)&nglContext::glMultiTexEnvfvEXT },
/* 1627 */ { "glMultiTexEnviEXT", (GLExtFuncPtr)&nglContext::glMultiTexEnviEXT },
/* 1628 */ { "glMultiTexEnvivEXT", (GLExtFuncPtr)&nglContext::glMultiTexEnvivEXT },
/* 1629 */ { "glMultiTexGendEXT", (GLExtFuncPtr)&nglContext::glMultiTexGendEXT },
/* 1630 */ { "glMultiTexGendvEXT", (GLExtFuncPtr)&nglContext::glMultiTexGendvEXT },
/* 1631 */ { "glMultiTexGenfEXT", (GLExtFuncPtr)&nglContext::glMultiTexGenfEXT },
/* 1632 */ { "glMultiTexGenfvEXT", (GLExtFuncPtr)&nglContext::glMultiTexGenfvEXT },
/* 1633 */ { "glMultiTexGeniEXT", (GLExtFuncPtr)&nglContext::glMultiTexGeniEXT },
/* 1634 */ { "glMultiTexGenivEXT", (GLExtFuncPtr)&nglContext::glMultiTexGenivEXT },
/* 1635 */ { "glGetMultiTexEnvfvEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexEnvfvEXT },
/* 1636 */ { "glGetMultiTexEnvivEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexEnvivEXT },
/* 1637 */ { "glGetMultiTexGendvEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexGendvEXT },
/* 1638 */ { "glGetMultiTexGenfvEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexGenfvEXT },
/* 1639 */ { "glGetMultiTexGenivEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexGenivEXT },
/* 1640 */ { "glGetFloatIndexedvEXT", (GLExtFuncPtr)&nglContext::glGetFloatIndexedvEXT },
/* 1641 */ { "glGetDoubleIndexedvEXT", (GLExtFuncPtr)&nglContext::glGetDoubleIndexedvEXT },
/* 1642 */ { "glGetPointerIndexedvEXT", (GLExtFuncPtr)&nglContext::glGetPointerIndexedvEXT },
/* 1643 */ { "glCompressedTextureImage3DEXT", (GLExtFuncPtr)&nglContext::glCompressedTextureImage3DEXT },
/* 1644 */ { "glCompressedTextureImage2DEXT", (GLExtFuncPtr)&nglContext::glCompressedTextureImage2DEXT },
/* 1645 */ { "glCompressedTextureImage1DEXT", (GLExtFuncPtr)&nglContext::glCompressedTextureImage1DEXT },
/* 1646 */ { "glCompressedTextureSubImage3DEXT", (GLExtFuncPtr)&nglContext::glCompressedTextureSubImage3DEXT },
/* 1647 */ { "glCompressedTextureSubImage2DEXT", (GLExtFuncPtr)&nglContext::glCompressedTextureSubImage2DEXT },
/* 1648 */ { "glCompressedTextureSubImage1DEXT", (GLExtFuncPtr)&nglContext::glCompressedTextureSubImage1DEXT },
/* 1649 */ { "glGetCompressedTextureImageEXT", (GLExtFuncPtr)&nglContext::glGetCompressedTextureImageEXT },
/* 1650 */ { "glCompressedMultiTexImage3DEXT", (GLExtFuncPtr)&nglContext::glCompressedMultiTexImage3DEXT },
/* 1651 */ { "glCompressedMultiTexImage2DEXT", (GLExtFuncPtr)&nglContext::glCompressedMultiTexImage2DEXT },
/* 1652 */ { "glCompressedMultiTexImage1DEXT", (GLExtFuncPtr)&nglContext::glCompressedMultiTexImage1DEXT },
/* 1653 */ { "glCompressedMultiTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glCompressedMultiTexSubImage3DEXT },
/* 1654 */ { "glCompressedMultiTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glCompressedMultiTexSubImage2DEXT },
/* 1655 */ { "glCompressedMultiTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glCompressedMultiTexSubImage1DEXT },
/* 1656 */ { "glGetCompressedMultiTexImageEXT", (GLExtFuncPtr)&nglContext::glGetCompressedMultiTexImageEXT },
/* 1657 */ { "glNamedProgramStringEXT", (GLExtFuncPtr)&nglContext::glNamedProgramStringEXT },
/* 1658 */ { "glNamedProgramLocalParameter4dEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameter4dEXT },
/* 1659 */ { "glNamedProgramLocalParameter4dvEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameter4dvEXT },
/* 1660 */ { "glNamedProgramLocalParameter4fEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameter4fEXT },
/* 1661 */ { "glNamedProgramLocalParameter4fvEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameter4fvEXT },
/* 1662 */ { "glGetNamedProgramLocalParameterdvEXT", (GLExtFuncPtr)&nglContext::glGetNamedProgramLocalParameterdvEXT },
/* 1663 */ { "glGetNamedProgramLocalParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetNamedProgramLocalParameterfvEXT },
/* 1664 */ { "glGetNamedProgramivEXT", (GLExtFuncPtr)&nglContext::glGetNamedProgramivEXT },
/* 1665 */ { "glGetNamedProgramStringEXT", (GLExtFuncPtr)&nglContext::glGetNamedProgramStringEXT },
/* 1666 */ { "glNamedProgramLocalParameters4fvEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameters4fvEXT },
/* 1667 */ { "glNamedProgramLocalParameterI4iEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameterI4iEXT },
/* 1668 */ { "glNamedProgramLocalParameterI4ivEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameterI4ivEXT },
/* 1669 */ { "glNamedProgramLocalParametersI4ivEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParametersI4ivEXT },
/* 1670 */ { "glNamedProgramLocalParameterI4uiEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameterI4uiEXT },
/* 1671 */ { "glNamedProgramLocalParameterI4uivEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParameterI4uivEXT },
/* 1672 */ { "glNamedProgramLocalParametersI4uivEXT", (GLExtFuncPtr)&nglContext::glNamedProgramLocalParametersI4uivEXT },
/* 1673 */ { "glGetNamedProgramLocalParameterIivEXT", (GLExtFuncPtr)&nglContext::glGetNamedProgramLocalParameterIivEXT },
/* 1674 */ { "glGetNamedProgramLocalParameterIuivEXT", (GLExtFuncPtr)&nglContext::glGetNamedProgramLocalParameterIuivEXT },
/* 1675 */ { "glTextureParameterIivEXT", (GLExtFuncPtr)&nglContext::glTextureParameterIivEXT },
/* 1676 */ { "glTextureParameterIuivEXT", (GLExtFuncPtr)&nglContext::glTextureParameterIuivEXT },
/* 1677 */ { "glGetTextureParameterIivEXT", (GLExtFuncPtr)&nglContext::glGetTextureParameterIivEXT },
/* 1678 */ { "glGetTextureParameterIuivEXT", (GLExtFuncPtr)&nglContext::glGetTextureParameterIuivEXT },
/* 1679 */ { "glMultiTexParameterIivEXT", (GLExtFuncPtr)&nglContext::glMultiTexParameterIivEXT },
/* 1680 */ { "glMultiTexParameterIuivEXT", (GLExtFuncPtr)&nglContext::glMultiTexParameterIuivEXT },
/* 1681 */ { "glGetMultiTexParameterIivEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexParameterIivEXT },
/* 1682 */ { "glGetMultiTexParameterIuivEXT", (GLExtFuncPtr)&nglContext::glGetMultiTexParameterIuivEXT },
/* 1683 */ { "glProgramUniform1fEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1fEXT },
/* 1684 */ { "glProgramUniform2fEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2fEXT },
/* 1685 */ { "glProgramUniform3fEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3fEXT },
/* 1686 */ { "glProgramUniform4fEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4fEXT },
/* 1687 */ { "glProgramUniform1iEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1iEXT },
/* 1688 */ { "glProgramUniform2iEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2iEXT },
/* 1689 */ { "glProgramUniform3iEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3iEXT },
/* 1690 */ { "glProgramUniform4iEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4iEXT },
/* 1691 */ { "glProgramUniform1fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1fvEXT },
/* 1692 */ { "glProgramUniform2fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2fvEXT },
/* 1693 */ { "glProgramUniform3fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3fvEXT },
/* 1694 */ { "glProgramUniform4fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4fvEXT },
/* 1695 */ { "glProgramUniform1ivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1ivEXT },
/* 1696 */ { "glProgramUniform2ivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2ivEXT },
/* 1697 */ { "glProgramUniform3ivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3ivEXT },
/* 1698 */ { "glProgramUniform4ivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4ivEXT },
/* 1699 */ { "glProgramUniformMatrix2fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2fvEXT },
/* 1700 */ { "glProgramUniformMatrix3fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3fvEXT },
/* 1701 */ { "glProgramUniformMatrix4fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4fvEXT },
/* 1702 */ { "glProgramUniformMatrix2x3fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x3fvEXT },
/* 1703 */ { "glProgramUniformMatrix3x2fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x2fvEXT },
/* 1704 */ { "glProgramUniformMatrix2x4fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x4fvEXT },
/* 1705 */ { "glProgramUniformMatrix4x2fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x2fvEXT },
/* 1706 */ { "glProgramUniformMatrix3x4fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x4fvEXT },
/* 1707 */ { "glProgramUniformMatrix4x3fvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x3fvEXT },
/* 1708 */ { "glProgramUniform1uiEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1uiEXT },
/* 1709 */ { "glProgramUniform2uiEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2uiEXT },
/* 1710 */ { "glProgramUniform3uiEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3uiEXT },
/* 1711 */ { "glProgramUniform4uiEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4uiEXT },
/* 1712 */ { "glProgramUniform1uivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1uivEXT },
/* 1713 */ { "glProgramUniform2uivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2uivEXT },
/* 1714 */ { "glProgramUniform3uivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3uivEXT },
/* 1715 */ { "glProgramUniform4uivEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4uivEXT },
/* 1716 */ { "glNamedBufferDataEXT", (GLExtFuncPtr)&nglContext::glNamedBufferDataEXT },
/* 1717 */ { "glNamedBufferSubDataEXT", (GLExtFuncPtr)&nglContext::glNamedBufferSubDataEXT },
/* 1718 */ { "glMapNamedBufferEXT", (GLExtFuncPtr)&nglContext::glMapNamedBufferEXT },
/* 1719 */ { "glUnmapNamedBufferEXT", (GLExtFuncPtr)&nglContext::glUnmapNamedBufferEXT },
/* 1720 */ { "glMapNamedBufferRangeEXT", (GLExtFuncPtr)&nglContext::glMapNamedBufferRangeEXT },
/* 1721 */ { "glFlushMappedNamedBufferRangeEXT", (GLExtFuncPtr)&nglContext::glFlushMappedNamedBufferRangeEXT },
/* 1722 */ { "glNamedCopyBufferSubDataEXT", (GLExtFuncPtr)&nglContext::glNamedCopyBufferSubDataEXT },
/* 1723 */ { "glGetNamedBufferParameterivEXT", (GLExtFuncPtr)&nglContext::glGetNamedBufferParameterivEXT },
/* 1724 */ { "glGetNamedBufferPointervEXT", (GLExtFuncPtr)&nglContext::glGetNamedBufferPointervEXT },
/* 1725 */ { "glGetNamedBufferSubDataEXT", (GLExtFuncPtr)&nglContext::glGetNamedBufferSubDataEXT },
/* 1726 */ { "glTextureBufferEXT", (GLExtFuncPtr)&nglContext::glTextureBufferEXT },
/* 1727 */ { "glMultiTexBufferEXT", (GLExtFuncPtr)&nglContext::glMultiTexBufferEXT },
/* 1728 */ { "glNamedRenderbufferStorageEXT", (GLExtFuncPtr)&nglContext::glNamedRenderbufferStorageEXT },
/* 1729 */ { "glGetNamedRenderbufferParameterivEXT", (GLExtFuncPtr)&nglContext::glGetNamedRenderbufferParameterivEXT },
/* 1730 */ { "glCheckNamedFramebufferStatusEXT", (GLExtFuncPtr)&nglContext::glCheckNamedFramebufferStatusEXT },
/* 1731 */ { "glNamedFramebufferTexture1DEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferTexture1DEXT },
/* 1732 */ { "glNamedFramebufferTexture2DEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferTexture2DEXT },
/* 1733 */ { "glNamedFramebufferTexture3DEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferTexture3DEXT },
/* 1734 */ { "glNamedFramebufferRenderbufferEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferRenderbufferEXT },
/* 1735 */ { "glGetNamedFramebufferAttachmentParameterivEXT", (GLExtFuncPtr)&nglContext::glGetNamedFramebufferAttachmentParameterivEXT },
/* 1736 */ { "glGenerateTextureMipmapEXT", (GLExtFuncPtr)&nglContext::glGenerateTextureMipmapEXT },
/* 1737 */ { "glGenerateMultiTexMipmapEXT", (GLExtFuncPtr)&nglContext::glGenerateMultiTexMipmapEXT },
/* 1738 */ { "glFramebufferDrawBufferEXT", (GLExtFuncPtr)&nglContext::glFramebufferDrawBufferEXT },
/* 1739 */ { "glFramebufferDrawBuffersEXT", (GLExtFuncPtr)&nglContext::glFramebufferDrawBuffersEXT },
/* 1740 */ { "glFramebufferReadBufferEXT", (GLExtFuncPtr)&nglContext::glFramebufferReadBufferEXT },
/* 1741 */ { "glGetFramebufferParameterivEXT", (GLExtFuncPtr)&nglContext::glGetFramebufferParameterivEXT },
/* 1742 */ { "glNamedRenderbufferStorageMultisampleEXT", (GLExtFuncPtr)&nglContext::glNamedRenderbufferStorageMultisampleEXT },
/* 1743 */ { "glNamedRenderbufferStorageMultisampleCoverageEXT", (GLExtFuncPtr)&nglContext::glNamedRenderbufferStorageMultisampleCoverageEXT },
/* 1744 */ { "glNamedFramebufferTextureEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferTextureEXT },
/* 1745 */ { "glNamedFramebufferTextureLayerEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferTextureLayerEXT },
/* 1746 */ { "glNamedFramebufferTextureFaceEXT", (GLExtFuncPtr)&nglContext::glNamedFramebufferTextureFaceEXT },
/* 1747 */ { "glTextureRenderbufferEXT", (GLExtFuncPtr)&nglContext::glTextureRenderbufferEXT },
/* 1748 */ { "glMultiTexRenderbufferEXT", (GLExtFuncPtr)&nglContext::glMultiTexRenderbufferEXT },
/* 1749 */ { "glProgramUniform1dEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1dEXT },
/* 1750 */ { "glProgramUniform2dEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2dEXT },
/* 1751 */ { "glProgramUniform3dEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3dEXT },
/* 1752 */ { "glProgramUniform4dEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4dEXT },
/* 1753 */ { "glProgramUniform1dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform1dvEXT },
/* 1754 */ { "glProgramUniform2dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform2dvEXT },
/* 1755 */ { "glProgramUniform3dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform3dvEXT },
/* 1756 */ { "glProgramUniform4dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniform4dvEXT },
/* 1757 */ { "glProgramUniformMatrix2dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2dvEXT },
/* 1758 */ { "glProgramUniformMatrix3dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3dvEXT },
/* 1759 */ { "glProgramUniformMatrix4dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4dvEXT },
/* 1760 */ { "glProgramUniformMatrix2x3dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x3dvEXT },
/* 1761 */ { "glProgramUniformMatrix2x4dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix2x4dvEXT },
/* 1762 */ { "glProgramUniformMatrix3x2dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x2dvEXT },
/* 1763 */ { "glProgramUniformMatrix3x4dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix3x4dvEXT },
/* 1764 */ { "glProgramUniformMatrix4x2dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x2dvEXT },
/* 1765 */ { "glProgramUniformMatrix4x3dvEXT", (GLExtFuncPtr)&nglContext::glProgramUniformMatrix4x3dvEXT },

/* GL_NV_explicit_multisample
 */
/* 1766 */ { "glGetMultisamplefvNV", (GLExtFuncPtr)&nglContext::glGetMultisamplefvNV },
/* 1767 */ { "glSampleMaskIndexedNV", (GLExtFuncPtr)&nglContext::glSampleMaskIndexedNV },
/* 1768 */ { "glTexRenderbufferNV", (GLExtFuncPtr)&nglContext::glTexRenderbufferNV },

/* GL_NV_transform_feedback2
 */
/* 1769 */ { "glBindTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glBindTransformFeedbackNV },
/* 1770 */ { "glDeleteTransformFeedbacksNV", (GLExtFuncPtr)&nglContext::glDeleteTransformFeedbacksNV },
/* 1771 */ { "glGenTransformFeedbacksNV", (GLExtFuncPtr)&nglContext::glGenTransformFeedbacksNV },
/* 1772 */ { "glIsTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glIsTransformFeedbackNV },
/* 1773 */ { "glPauseTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glPauseTransformFeedbackNV },
/* 1774 */ { "glResumeTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glResumeTransformFeedbackNV },
/* 1775 */ { "glDrawTransformFeedbackNV", (GLExtFuncPtr)&nglContext::glDrawTransformFeedbackNV },

/* GL_AMD_performance_monitor
 */
/* 1776 */ { "glGetPerfMonitorGroupsAMD", (GLExtFuncPtr)&nglContext::glGetPerfMonitorGroupsAMD },
/* 1777 */ { "glGetPerfMonitorCountersAMD", (GLExtFuncPtr)&nglContext::glGetPerfMonitorCountersAMD },
/* 1778 */ { "glGetPerfMonitorGroupStringAMD", (GLExtFuncPtr)&nglContext::glGetPerfMonitorGroupStringAMD },
/* 1779 */ { "glGetPerfMonitorCounterStringAMD", (GLExtFuncPtr)&nglContext::glGetPerfMonitorCounterStringAMD },
/* 1780 */ { "glGetPerfMonitorCounterInfoAMD", (GLExtFuncPtr)&nglContext::glGetPerfMonitorCounterInfoAMD },
/* 1781 */ { "glGenPerfMonitorsAMD", (GLExtFuncPtr)&nglContext::glGenPerfMonitorsAMD },
/* 1782 */ { "glDeletePerfMonitorsAMD", (GLExtFuncPtr)&nglContext::glDeletePerfMonitorsAMD },
/* 1783 */ { "glSelectPerfMonitorCountersAMD", (GLExtFuncPtr)&nglContext::glSelectPerfMonitorCountersAMD },
/* 1784 */ { "glBeginPerfMonitorAMD", (GLExtFuncPtr)&nglContext::glBeginPerfMonitorAMD },
/* 1785 */ { "glEndPerfMonitorAMD", (GLExtFuncPtr)&nglContext::glEndPerfMonitorAMD },
/* 1786 */ { "glGetPerfMonitorCounterDataAMD", (GLExtFuncPtr)&nglContext::glGetPerfMonitorCounterDataAMD },

/* GL_AMD_vertex_shader_tesselator
 */
/* 1787 */ { "glTessellationFactorAMD", (GLExtFuncPtr)&nglContext::glTessellationFactorAMD },
/* 1788 */ { "glTessellationModeAMD", (GLExtFuncPtr)&nglContext::glTessellationModeAMD },

/* GL_EXT_provoking_vertex
 */
/* 1789 */ { "glProvokingVertexEXT", (GLExtFuncPtr)&nglContext::glProvokingVertexEXT },

/* GL_AMD_draw_buffers_blend
 */
/* 1790 */ { "glBlendFuncIndexedAMD", (GLExtFuncPtr)&nglContext::glBlendFuncIndexedAMD },
/* 1791 */ { "glBlendFuncSeparateIndexedAMD", (GLExtFuncPtr)&nglContext::glBlendFuncSeparateIndexedAMD },
/* 1792 */ { "glBlendEquationIndexedAMD", (GLExtFuncPtr)&nglContext::glBlendEquationIndexedAMD },
/* 1793 */ { "glBlendEquationSeparateIndexedAMD", (GLExtFuncPtr)&nglContext::glBlendEquationSeparateIndexedAMD },

/* GL_APPLE_texture_range
 */
/* 1794 */ { "glTextureRangeAPPLE", (GLExtFuncPtr)&nglContext::glTextureRangeAPPLE },
/* 1795 */ { "glGetTexParameterPointervAPPLE", (GLExtFuncPtr)&nglContext::glGetTexParameterPointervAPPLE },

/* GL_APPLE_vertex_program_evaluators
 */
/* 1796 */ { "glEnableVertexAttribAPPLE", (GLExtFuncPtr)&nglContext::glEnableVertexAttribAPPLE },
/* 1797 */ { "glDisableVertexAttribAPPLE", (GLExtFuncPtr)&nglContext::glDisableVertexAttribAPPLE },
/* 1798 */ { "glIsVertexAttribEnabledAPPLE", (GLExtFuncPtr)&nglContext::glIsVertexAttribEnabledAPPLE },
/* 1799 */ { "glMapVertexAttrib1dAPPLE", (GLExtFuncPtr)&nglContext::glMapVertexAttrib1dAPPLE },
/* 1800 */ { "glMapVertexAttrib1fAPPLE", (GLExtFuncPtr)&nglContext::glMapVertexAttrib1fAPPLE },
/* 1801 */ { "glMapVertexAttrib2dAPPLE", (GLExtFuncPtr)&nglContext::glMapVertexAttrib2dAPPLE },
/* 1802 */ { "glMapVertexAttrib2fAPPLE", (GLExtFuncPtr)&nglContext::glMapVertexAttrib2fAPPLE },

/* GL_APPLE_object_purgeable
 */
/* 1803 */ { "glObjectPurgeableAPPLE", (GLExtFuncPtr)&nglContext::glObjectPurgeableAPPLE },
/* 1804 */ { "glObjectUnpurgeableAPPLE", (GLExtFuncPtr)&nglContext::glObjectUnpurgeableAPPLE },
/* 1805 */ { "glGetObjectParameterivAPPLE", (GLExtFuncPtr)&nglContext::glGetObjectParameterivAPPLE },

/* GL_NV_video_capture
 */
/* 1806 */ { "glBeginVideoCaptureNV", (GLExtFuncPtr)&nglContext::glBeginVideoCaptureNV },
/* 1807 */ { "glBindVideoCaptureStreamBufferNV", (GLExtFuncPtr)&nglContext::glBindVideoCaptureStreamBufferNV },
/* 1808 */ { "glBindVideoCaptureStreamTextureNV", (GLExtFuncPtr)&nglContext::glBindVideoCaptureStreamTextureNV },
/* 1809 */ { "glEndVideoCaptureNV", (GLExtFuncPtr)&nglContext::glEndVideoCaptureNV },
/* 1810 */ { "glGetVideoCaptureivNV", (GLExtFuncPtr)&nglContext::glGetVideoCaptureivNV },
/* 1811 */ { "glGetVideoCaptureStreamivNV", (GLExtFuncPtr)&nglContext::glGetVideoCaptureStreamivNV },
/* 1812 */ { "glGetVideoCaptureStreamfvNV", (GLExtFuncPtr)&nglContext::glGetVideoCaptureStreamfvNV },
/* 1813 */ { "glGetVideoCaptureStreamdvNV", (GLExtFuncPtr)&nglContext::glGetVideoCaptureStreamdvNV },
/* 1814 */ { "glVideoCaptureNV", (GLExtFuncPtr)&nglContext::glVideoCaptureNV },
/* 1815 */ { "glVideoCaptureStreamParameterivNV", (GLExtFuncPtr)&nglContext::glVideoCaptureStreamParameterivNV },
/* 1816 */ { "glVideoCaptureStreamParameterfvNV", (GLExtFuncPtr)&nglContext::glVideoCaptureStreamParameterfvNV },
/* 1817 */ { "glVideoCaptureStreamParameterdvNV", (GLExtFuncPtr)&nglContext::glVideoCaptureStreamParameterdvNV },

/* GL_NV_copy_image
 */
/* 1818 */ { "glCopyImageSubDataNV", (GLExtFuncPtr)&nglContext::glCopyImageSubDataNV },

/* GL_EXT_separate_shader_objects
 */
/* 1819 */ { "glUseShaderProgramEXT", (GLExtFuncPtr)&nglContext::glUseShaderProgramEXT },
/* 1820 */ { "glActiveProgramEXT", (GLExtFuncPtr)&nglContext::glActiveProgramEXT },
/* 1821 */ { "glCreateShaderProgramEXT", (GLExtFuncPtr)&nglContext::glCreateShaderProgramEXT },

/* GL_NV_shader_buffer_load
 */
/* 1822 */ { "glMakeBufferResidentNV", (GLExtFuncPtr)&nglContext::glMakeBufferResidentNV },
/* 1823 */ { "glMakeBufferNonResidentNV", (GLExtFuncPtr)&nglContext::glMakeBufferNonResidentNV },
/* 1824 */ { "glIsBufferResidentNV", (GLExtFuncPtr)&nglContext::glIsBufferResidentNV },
/* 1825 */ { "glMakeNamedBufferResidentNV", (GLExtFuncPtr)&nglContext::glMakeNamedBufferResidentNV },
/* 1826 */ { "glMakeNamedBufferNonResidentNV", (GLExtFuncPtr)&nglContext::glMakeNamedBufferNonResidentNV },
/* 1827 */ { "glIsNamedBufferResidentNV", (GLExtFuncPtr)&nglContext::glIsNamedBufferResidentNV },
/* 1828 */ { "glGetBufferParameterui64vNV", (GLExtFuncPtr)&nglContext::glGetBufferParameterui64vNV },
/* 1829 */ { "glGetNamedBufferParameterui64vNV", (GLExtFuncPtr)&nglContext::glGetNamedBufferParameterui64vNV },
/* 1830 */ { "glGetIntegerui64vNV", (GLExtFuncPtr)&nglContext::glGetIntegerui64vNV },
/* 1831 */ { "glUniformui64NV", (GLExtFuncPtr)&nglContext::glUniformui64NV },
/* 1832 */ { "glUniformui64vNV", (GLExtFuncPtr)&nglContext::glUniformui64vNV },
/* 1833 */ { "glGetUniformui64vNV", (GLExtFuncPtr)&nglContext::glGetUniformui64vNV },
/* 1834 */ { "glProgramUniformui64NV", (GLExtFuncPtr)&nglContext::glProgramUniformui64NV },
/* 1835 */ { "glProgramUniformui64vNV", (GLExtFuncPtr)&nglContext::glProgramUniformui64vNV },

/* GL_NV_vertex_buffer_unified_memory
 */
/* 1836 */ { "glBufferAddressRangeNV", (GLExtFuncPtr)&nglContext::glBufferAddressRangeNV },
/* 1837 */ { "glVertexFormatNV", (GLExtFuncPtr)&nglContext::glVertexFormatNV },
/* 1838 */ { "glNormalFormatNV", (GLExtFuncPtr)&nglContext::glNormalFormatNV },
/* 1839 */ { "glColorFormatNV", (GLExtFuncPtr)&nglContext::glColorFormatNV },
/* 1840 */ { "glIndexFormatNV", (GLExtFuncPtr)&nglContext::glIndexFormatNV },
/* 1841 */ { "glTexCoordFormatNV", (GLExtFuncPtr)&nglContext::glTexCoordFormatNV },
/* 1842 */ { "glEdgeFlagFormatNV", (GLExtFuncPtr)&nglContext::glEdgeFlagFormatNV },
/* 1843 */ { "glSecondaryColorFormatNV", (GLExtFuncPtr)&nglContext::glSecondaryColorFormatNV },
/* 1844 */ { "glFogCoordFormatNV", (GLExtFuncPtr)&nglContext::glFogCoordFormatNV },
/* 1845 */ { "glVertexAttribFormatNV", (GLExtFuncPtr)&nglContext::glVertexAttribFormatNV },
/* 1846 */ { "glVertexAttribIFormatNV", (GLExtFuncPtr)&nglContext::glVertexAttribIFormatNV },
/* 1847 */ { "glGetIntegerui64i_vNV", (GLExtFuncPtr)&nglContext::glGetIntegerui64i_vNV },

/* GL_NV_texture_barrier
 */
/* 1848 */ { "glTextureBarrierNV", (GLExtFuncPtr)&nglContext::glTextureBarrierNV },

/* GL_EXT_shader_image_load_store
 */
/* 1849 */ { "glBindImageTextureEXT", (GLExtFuncPtr)&nglContext::glBindImageTextureEXT },
/* 1850 */ { "glMemoryBarrierEXT", (GLExtFuncPtr)&nglContext::glMemoryBarrierEXT },

/* GL_EXT_vertex_attrib_64bit
 */
/* 1851 */ { "glVertexAttribL1dEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL1dEXT },
/* 1852 */ { "glVertexAttribL2dEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL2dEXT },
/* 1853 */ { "glVertexAttribL3dEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL3dEXT },
/* 1854 */ { "glVertexAttribL4dEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL4dEXT },
/* 1855 */ { "glVertexAttribL1dvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL1dvEXT },
/* 1856 */ { "glVertexAttribL2dvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL2dvEXT },
/* 1857 */ { "glVertexAttribL3dvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL3dvEXT },
/* 1858 */ { "glVertexAttribL4dvEXT", (GLExtFuncPtr)&nglContext::glVertexAttribL4dvEXT },
/* 1859 */ { "glVertexAttribLPointerEXT", (GLExtFuncPtr)&nglContext::glVertexAttribLPointerEXT },
/* 1860 */ { "glGetVertexAttribLdvEXT", (GLExtFuncPtr)&nglContext::glGetVertexAttribLdvEXT },
/* 1861 */ { "glVertexArrayVertexAttribLOffsetEXT", (GLExtFuncPtr)&nglContext::glVertexArrayVertexAttribLOffsetEXT },

/* GL_NV_gpu_program5
 */
/* 1862 */ { "glProgramSubroutineParametersuivNV", (GLExtFuncPtr)&nglContext::glProgramSubroutineParametersuivNV },
/* 1863 */ { "glGetProgramSubroutineParameteruivNV", (GLExtFuncPtr)&nglContext::glGetProgramSubroutineParameteruivNV },

/* GL_NV_gpu_shader5
 */
/* 1864 */ { "glUniform1i64NV", (GLExtFuncPtr)&nglContext::glUniform1i64NV },
/* 1865 */ { "glUniform2i64NV", (GLExtFuncPtr)&nglContext::glUniform2i64NV },
/* 1866 */ { "glUniform3i64NV", (GLExtFuncPtr)&nglContext::glUniform3i64NV },
/* 1867 */ { "glUniform4i64NV", (GLExtFuncPtr)&nglContext::glUniform4i64NV },
/* 1868 */ { "glUniform1i64vNV", (GLExtFuncPtr)&nglContext::glUniform1i64vNV },
/* 1869 */ { "glUniform2i64vNV", (GLExtFuncPtr)&nglContext::glUniform2i64vNV },
/* 1870 */ { "glUniform3i64vNV", (GLExtFuncPtr)&nglContext::glUniform3i64vNV },
/* 1871 */ { "glUniform4i64vNV", (GLExtFuncPtr)&nglContext::glUniform4i64vNV },
/* 1872 */ { "glUniform1ui64NV", (GLExtFuncPtr)&nglContext::glUniform1ui64NV },
/* 1873 */ { "glUniform2ui64NV", (GLExtFuncPtr)&nglContext::glUniform2ui64NV },
/* 1874 */ { "glUniform3ui64NV", (GLExtFuncPtr)&nglContext::glUniform3ui64NV },
/* 1875 */ { "glUniform4ui64NV", (GLExtFuncPtr)&nglContext::glUniform4ui64NV },
/* 1876 */ { "glUniform1ui64vNV", (GLExtFuncPtr)&nglContext::glUniform1ui64vNV },
/* 1877 */ { "glUniform2ui64vNV", (GLExtFuncPtr)&nglContext::glUniform2ui64vNV },
/* 1878 */ { "glUniform3ui64vNV", (GLExtFuncPtr)&nglContext::glUniform3ui64vNV },
/* 1879 */ { "glUniform4ui64vNV", (GLExtFuncPtr)&nglContext::glUniform4ui64vNV },
/* 1880 */ { "glGetUniformi64vNV", (GLExtFuncPtr)&nglContext::glGetUniformi64vNV },
/* 1881 */ { "glProgramUniform1i64NV", (GLExtFuncPtr)&nglContext::glProgramUniform1i64NV },
/* 1882 */ { "glProgramUniform2i64NV", (GLExtFuncPtr)&nglContext::glProgramUniform2i64NV },
/* 1883 */ { "glProgramUniform3i64NV", (GLExtFuncPtr)&nglContext::glProgramUniform3i64NV },
/* 1884 */ { "glProgramUniform4i64NV", (GLExtFuncPtr)&nglContext::glProgramUniform4i64NV },
/* 1885 */ { "glProgramUniform1i64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform1i64vNV },
/* 1886 */ { "glProgramUniform2i64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform2i64vNV },
/* 1887 */ { "glProgramUniform3i64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform3i64vNV },
/* 1888 */ { "glProgramUniform4i64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform4i64vNV },
/* 1889 */ { "glProgramUniform1ui64NV", (GLExtFuncPtr)&nglContext::glProgramUniform1ui64NV },
/* 1890 */ { "glProgramUniform2ui64NV", (GLExtFuncPtr)&nglContext::glProgramUniform2ui64NV },
/* 1891 */ { "glProgramUniform3ui64NV", (GLExtFuncPtr)&nglContext::glProgramUniform3ui64NV },
/* 1892 */ { "glProgramUniform4ui64NV", (GLExtFuncPtr)&nglContext::glProgramUniform4ui64NV },
/* 1893 */ { "glProgramUniform1ui64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform1ui64vNV },
/* 1894 */ { "glProgramUniform2ui64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform2ui64vNV },
/* 1895 */ { "glProgramUniform3ui64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform3ui64vNV },
/* 1896 */ { "glProgramUniform4ui64vNV", (GLExtFuncPtr)&nglContext::glProgramUniform4ui64vNV },

/* GL_NV_vertex_attrib_integer_64bit
 */
/* 1897 */ { "glVertexAttribL1i64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL1i64NV },
/* 1898 */ { "glVertexAttribL2i64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL2i64NV },
/* 1899 */ { "glVertexAttribL3i64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL3i64NV },
/* 1900 */ { "glVertexAttribL4i64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL4i64NV },
/* 1901 */ { "glVertexAttribL1i64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL1i64vNV },
/* 1902 */ { "glVertexAttribL2i64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL2i64vNV },
/* 1903 */ { "glVertexAttribL3i64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL3i64vNV },
/* 1904 */ { "glVertexAttribL4i64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL4i64vNV },
/* 1905 */ { "glVertexAttribL1ui64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL1ui64NV },
/* 1906 */ { "glVertexAttribL2ui64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL2ui64NV },
/* 1907 */ { "glVertexAttribL3ui64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL3ui64NV },
/* 1908 */ { "glVertexAttribL4ui64NV", (GLExtFuncPtr)&nglContext::glVertexAttribL4ui64NV },
/* 1909 */ { "glVertexAttribL1ui64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL1ui64vNV },
/* 1910 */ { "glVertexAttribL2ui64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL2ui64vNV },
/* 1911 */ { "glVertexAttribL3ui64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL3ui64vNV },
/* 1912 */ { "glVertexAttribL4ui64vNV", (GLExtFuncPtr)&nglContext::glVertexAttribL4ui64vNV },
/* 1913 */ { "glGetVertexAttribLi64vNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribLi64vNV },
/* 1914 */ { "glGetVertexAttribLui64vNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribLui64vNV },
/* 1915 */ { "glVertexAttribLFormatNV", (GLExtFuncPtr)&nglContext::glVertexAttribLFormatNV },

/* GL_AMD_name_gen_delete
 */
/* 1916 */ { "glGenNamesAMD", (GLExtFuncPtr)&nglContext::glGenNamesAMD },
/* 1917 */ { "glDeleteNamesAMD", (GLExtFuncPtr)&nglContext::glDeleteNamesAMD },
/* 1918 */ { "glIsNameAMD", (GLExtFuncPtr)&nglContext::glIsNameAMD },

/* GL_AMD_debug_output
 */
/* 1919 */ { "glDebugMessageEnableAMD", (GLExtFuncPtr)&nglContext::glDebugMessageEnableAMD },
/* 1920 */ { "glDebugMessageInsertAMD", (GLExtFuncPtr)&nglContext::glDebugMessageInsertAMD },
/* 1921 */ { "glDebugMessageCallbackAMD", (GLExtFuncPtr)&nglContext::glDebugMessageCallbackAMD },
/* 1922 */ { "glGetDebugMessageLogAMD", (GLExtFuncPtr)&nglContext::glGetDebugMessageLogAMD },

/* GL_NV_vdpau_interop
 */
/* 1923 */ { "glVDPAUInitNV", (GLExtFuncPtr)&nglContext::glVDPAUInitNV },
/* 1924 */ { "glVDPAUFiniNV", (GLExtFuncPtr)&nglContext::glVDPAUFiniNV },
/* 1925 */ { "glVDPAURegisterVideoSurfaceNV", (GLExtFuncPtr)&nglContext::glVDPAURegisterVideoSurfaceNV },
/* 1926 */ { "glVDPAURegisterOutputSurfaceNV", (GLExtFuncPtr)&nglContext::glVDPAURegisterOutputSurfaceNV },
/* 1927 */ { "glVDPAUIsSurfaceNV", (GLExtFuncPtr)&nglContext::glVDPAUIsSurfaceNV },
/* 1928 */ { "glVDPAUUnregisterSurfaceNV", (GLExtFuncPtr)&nglContext::glVDPAUUnregisterSurfaceNV },
/* 1929 */ { "glVDPAUGetSurfaceivNV", (GLExtFuncPtr)&nglContext::glVDPAUGetSurfaceivNV },
/* 1930 */ { "glVDPAUSurfaceAccessNV", (GLExtFuncPtr)&nglContext::glVDPAUSurfaceAccessNV },
/* 1931 */ { "glVDPAUMapSurfacesNV", (GLExtFuncPtr)&nglContext::glVDPAUMapSurfacesNV },
/* 1932 */ { "glVDPAUUnmapSurfacesNV", (GLExtFuncPtr)&nglContext::glVDPAUUnmapSurfacesNV },

/* GL_NV_texture_multisample
 */
/* 1933 */ { "glTexImage2DMultisampleCoverageNV", (GLExtFuncPtr)&nglContext::glTexImage2DMultisampleCoverageNV },
/* 1934 */ { "glTexImage3DMultisampleCoverageNV", (GLExtFuncPtr)&nglContext::glTexImage3DMultisampleCoverageNV },
/* 1935 */ { "glTextureImage2DMultisampleNV", (GLExtFuncPtr)&nglContext::glTextureImage2DMultisampleNV },
/* 1936 */ { "glTextureImage3DMultisampleNV", (GLExtFuncPtr)&nglContext::glTextureImage3DMultisampleNV },
/* 1937 */ { "glTextureImage2DMultisampleCoverageNV", (GLExtFuncPtr)&nglContext::glTextureImage2DMultisampleCoverageNV },
/* 1938 */ { "glTextureImage3DMultisampleCoverageNV", (GLExtFuncPtr)&nglContext::glTextureImage3DMultisampleCoverageNV },

/* GL_AMD_sample_positions
 */
/* 1939 */ { "glSetMultisamplefvAMD", (GLExtFuncPtr)&nglContext::glSetMultisamplefvAMD },

/* GL_EXT_x11_sync_object
 */
/* 1940 */ { "glImportSyncEXT", (GLExtFuncPtr)&nglContext::glImportSyncEXT },

/* GL_AMD_multi_draw_indirect
 */
/* 1941 */ { "glMultiDrawArraysIndirectAMD", (GLExtFuncPtr)&nglContext::glMultiDrawArraysIndirectAMD },
/* 1942 */ { "glMultiDrawElementsIndirectAMD", (GLExtFuncPtr)&nglContext::glMultiDrawElementsIndirectAMD },

  // End of list
  { NULL, (GLExtFuncPtr)NULL }
};


/* 211 extensions, 1943 total functions */
#endif
