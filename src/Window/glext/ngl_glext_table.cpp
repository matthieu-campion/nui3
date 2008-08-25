/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/* This file was generated with glext_gen.pl, please do not edit.  */

#include "nui.h"
#include "ngl_glext_table.h"


GLExtNameInfo gpGLExtensionNameTable[] =
{
  { "GL_VERSION_1_2",                 0   },
  { "GL_VERSION_1_3",                 38  },
  { "GL_VERSION_1_4",                 84  },
  { "GL_VERSION_1_5",                 129 },
  { "GL_VERSION_2_0",                 148 },
  { "GL_ARB_multitexture",            241 },
  { "GL_ARB_transpose_matrix",        275 },
  { "GL_ARB_multisample",             279 },
  { "GL_ARB_texture_compression",     280 },
  { "GL_ARB_point_parameters",        287 },
  { "GL_ARB_vertex_blend",            289 },
  { "GL_ARB_matrix_palette",          299 },
  { "GL_ARB_window_pos",              304 },
  { "GL_ARB_vertex_program",          320 },
  { "GL_ARB_vertex_buffer_object",    382 },
  { "GL_ARB_occlusion_query",         393 },
  { "GL_ARB_shader_objects",          401 },
  { "GL_ARB_vertex_shader",           440 },
  { "GL_ARB_draw_buffers",            443 },
  { "GL_ARB_color_buffer_float",      444 },
  { "GL_EXT_blend_color",             445 },
  { "GL_EXT_polygon_offset",          446 },
  { "GL_EXT_texture3D",               447 },
  { "GL_SGIS_texture_filter4",        449 },
  { "GL_EXT_subtexture",              451 },
  { "GL_EXT_copy_texture",            453 },
  { "GL_EXT_histogram",               458 },
  { "GL_EXT_convolution",             468 },
  { "GL_SGI_color_table",             481 },
  { "GL_SGIX_pixel_texture",          488 },
  { "GL_SGIS_pixel_texture",          489 },
  { "GL_SGIS_texture4D",              495 },
  { "GL_EXT_texture_object",          497 },
  { "GL_SGIS_detail_texture",         503 },
  { "GL_SGIS_sharpen_texture",        505 },
  { "GL_SGIS_multisample",            507 },
  { "GL_EXT_vertex_array",            509 },
  { "GL_EXT_blend_minmax",            518 },
  { "GL_SGIX_sprite",                 519 },
  { "GL_EXT_point_parameters",        523 },
  { "GL_SGIS_point_parameters",       525 },
  { "GL_SGIX_instruments",            527 },
  { "GL_SGIX_framezoom",              533 },
  { "GL_SGIX_tag_sample_buffer",      534 },
  { "GL_SGIX_polynomial_ffd",         535 },
  { "GL_SGIX_reference_plane",        539 },
  { "GL_SGIX_flush_raster",           540 },
  { "GL_SGIS_fog_function",           541 },
  { "GL_HP_image_transform",          543 },
  { "GL_EXT_color_subtable",          549 },
  { "GL_PGI_misc_hints",              551 },
  { "GL_EXT_paletted_texture",        552 },
  { "GL_SGIX_list_priority",          556 },
  { "GL_EXT_index_material",          562 },
  { "GL_EXT_index_func",              563 },
  { "GL_EXT_compiled_vertex_array",   564 },
  { "GL_EXT_cull_vertex",             566 },
  { "GL_SGIX_fragment_lighting",      568 },
  { "GL_EXT_draw_range_elements",     586 },
  { "GL_EXT_light_texture",           587 },
  { "GL_SGIX_async",                  590 },
  { "GL_INTEL_parallel_arrays",       596 },
  { "GL_EXT_pixel_transform",         600 },
  { "GL_EXT_secondary_color",         604 },
  { "GL_EXT_texture_perturb_normal",  621 },
  { "GL_EXT_multi_draw_arrays",       622 },
  { "GL_EXT_fog_coord",               624 },
  { "GL_EXT_coordinate_frame",        629 },
  { "GL_SUNX_constant_data",          651 },
  { "GL_SUN_global_alpha",            652 },
  { "GL_SUN_triangle_list",           660 },
  { "GL_SUN_vertex",                  667 },
  { "GL_EXT_blend_func_separate",     707 },
  { "GL_INGR_blend_func_separate",    708 },
  { "GL_EXT_vertex_weighting",        709 },
  { "GL_NV_vertex_array_range",       712 },
  { "GL_NV_register_combiners",       714 },
  { "GL_MESA_resize_buffers",         727 },
  { "GL_MESA_window_pos",             728 },
  { "GL_IBM_multimode_draw_arrays",   752 },
  { "GL_IBM_vertex_array_lists",      754 },
  { "GL_3DFX_tbuffer",                762 },
  { "GL_EXT_multisample",             763 },
  { "GL_SGIS_texture_color_mask",     765 },
  { "GL_SGIX_igloo_interface",        766 },
  { "GL_NV_fence",                    767 },
  { "GL_NV_evaluators",               774 },
  { "GL_NV_register_combiners2",      783 },
  { "GL_NV_vertex_program",           785 },
  { "GL_ATI_envmap_bumpmap",          849 },
  { "GL_ATI_fragment_shader",         853 },
  { "GL_ATI_pn_triangles",            867 },
  { "GL_ATI_vertex_array_object",     869 },
  { "GL_EXT_vertex_shader",           881 },
  { "GL_ATI_vertex_streams",          923 },
  { "GL_ATI_element_array",           968 },
  { "GL_SUN_mesh_array",              971 },
  { "GL_NV_occlusion_query",          972 },
  { "GL_NV_point_sprite",             979 },
  { "GL_EXT_stencil_two_side",        981 },
  { "GL_APPLE_element_array",         982 },
  { "GL_APPLE_fence",                 987 },
  { "GL_APPLE_vertex_array_object",   995 },
  { "GL_APPLE_vertex_array_range",    999 },
  { "GL_ATI_draw_buffers",            1002 },
  { "GL_NV_fragment_program",         1003 },
  { "GL_NV_half_float",               1009 },
  { "GL_NV_pixel_data_range",         1055 },
  { "GL_NV_primitive_restart",        1057 },
  { "GL_ATI_map_object_buffer",       1059 },
  { "GL_ATI_separate_stencil",        1061 },
  { "GL_ATI_vertex_attrib_array_object", 1063 },
  { "GL_EXT_depth_bounds_test",       1066 },
  { "GL_EXT_blend_equation_separate", 1067 },
  { "GL_EXT_framebuffer_object",      1068 },
  { "GL_GREMEDY_string_marker",       1085 },

  // End of list
  { NULL, 1086 }
};


GLExtFuncInfo gpGLExtensionFuncTable[] =
{
/* GL_VERSION_1_2
 */
/* 0   */ { "glBlendColor", (GLExtFuncPtr)&nglContext::glBlendColor },
/* 1   */ { "glBlendEquation", (GLExtFuncPtr)&nglContext::glBlendEquation },
/* 2   */ { "glDrawRangeElements", (GLExtFuncPtr)&nglContext::glDrawRangeElements },
/* 3   */ { "glColorTable", (GLExtFuncPtr)&nglContext::glColorTable },
/* 4   */ { "glColorTableParameterfv", (GLExtFuncPtr)&nglContext::glColorTableParameterfv },
/* 5   */ { "glColorTableParameteriv", (GLExtFuncPtr)&nglContext::glColorTableParameteriv },
/* 6   */ { "glCopyColorTable", (GLExtFuncPtr)&nglContext::glCopyColorTable },
/* 7   */ { "glGetColorTable", (GLExtFuncPtr)&nglContext::glGetColorTable },
/* 8   */ { "glGetColorTableParameterfv", (GLExtFuncPtr)&nglContext::glGetColorTableParameterfv },
/* 9   */ { "glGetColorTableParameteriv", (GLExtFuncPtr)&nglContext::glGetColorTableParameteriv },
/* 10  */ { "glColorSubTable", (GLExtFuncPtr)&nglContext::glColorSubTable },
/* 11  */ { "glCopyColorSubTable", (GLExtFuncPtr)&nglContext::glCopyColorSubTable },
/* 12  */ { "glConvolutionFilter1D", (GLExtFuncPtr)&nglContext::glConvolutionFilter1D },
/* 13  */ { "glConvolutionFilter2D", (GLExtFuncPtr)&nglContext::glConvolutionFilter2D },
/* 14  */ { "glConvolutionParameterf", (GLExtFuncPtr)&nglContext::glConvolutionParameterf },
/* 15  */ { "glConvolutionParameterfv", (GLExtFuncPtr)&nglContext::glConvolutionParameterfv },
/* 16  */ { "glConvolutionParameteri", (GLExtFuncPtr)&nglContext::glConvolutionParameteri },
/* 17  */ { "glConvolutionParameteriv", (GLExtFuncPtr)&nglContext::glConvolutionParameteriv },
/* 18  */ { "glCopyConvolutionFilter1D", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter1D },
/* 19  */ { "glCopyConvolutionFilter2D", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter2D },
/* 20  */ { "glGetConvolutionFilter", (GLExtFuncPtr)&nglContext::glGetConvolutionFilter },
/* 21  */ { "glGetConvolutionParameterfv", (GLExtFuncPtr)&nglContext::glGetConvolutionParameterfv },
/* 22  */ { "glGetConvolutionParameteriv", (GLExtFuncPtr)&nglContext::glGetConvolutionParameteriv },
/* 23  */ { "glGetSeparableFilter", (GLExtFuncPtr)&nglContext::glGetSeparableFilter },
/* 24  */ { "glSeparableFilter2D", (GLExtFuncPtr)&nglContext::glSeparableFilter2D },
/* 25  */ { "glGetHistogram", (GLExtFuncPtr)&nglContext::glGetHistogram },
/* 26  */ { "glGetHistogramParameterfv", (GLExtFuncPtr)&nglContext::glGetHistogramParameterfv },
/* 27  */ { "glGetHistogramParameteriv", (GLExtFuncPtr)&nglContext::glGetHistogramParameteriv },
/* 28  */ { "glGetMinmax", (GLExtFuncPtr)&nglContext::glGetMinmax },
/* 29  */ { "glGetMinmaxParameterfv", (GLExtFuncPtr)&nglContext::glGetMinmaxParameterfv },
/* 30  */ { "glGetMinmaxParameteriv", (GLExtFuncPtr)&nglContext::glGetMinmaxParameteriv },
/* 31  */ { "glHistogram", (GLExtFuncPtr)&nglContext::glHistogram },
/* 32  */ { "glMinmax", (GLExtFuncPtr)&nglContext::glMinmax },
/* 33  */ { "glResetHistogram", (GLExtFuncPtr)&nglContext::glResetHistogram },
/* 34  */ { "glResetMinmax", (GLExtFuncPtr)&nglContext::glResetMinmax },
/* 35  */ { "glTexImage3D", (GLExtFuncPtr)&nglContext::glTexImage3D },
/* 36  */ { "glTexSubImage3D", (GLExtFuncPtr)&nglContext::glTexSubImage3D },
/* 37  */ { "glCopyTexSubImage3D", (GLExtFuncPtr)&nglContext::glCopyTexSubImage3D },

/* GL_VERSION_1_3
 */
/* 38  */ { "glActiveTexture", (GLExtFuncPtr)&nglContext::glActiveTexture },
/* 39  */ { "glClientActiveTexture", (GLExtFuncPtr)&nglContext::glClientActiveTexture },
/* 40  */ { "glMultiTexCoord1d", (GLExtFuncPtr)&nglContext::glMultiTexCoord1d },
/* 41  */ { "glMultiTexCoord1dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1dv },
/* 42  */ { "glMultiTexCoord1f", (GLExtFuncPtr)&nglContext::glMultiTexCoord1f },
/* 43  */ { "glMultiTexCoord1fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1fv },
/* 44  */ { "glMultiTexCoord1i", (GLExtFuncPtr)&nglContext::glMultiTexCoord1i },
/* 45  */ { "glMultiTexCoord1iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1iv },
/* 46  */ { "glMultiTexCoord1s", (GLExtFuncPtr)&nglContext::glMultiTexCoord1s },
/* 47  */ { "glMultiTexCoord1sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord1sv },
/* 48  */ { "glMultiTexCoord2d", (GLExtFuncPtr)&nglContext::glMultiTexCoord2d },
/* 49  */ { "glMultiTexCoord2dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2dv },
/* 50  */ { "glMultiTexCoord2f", (GLExtFuncPtr)&nglContext::glMultiTexCoord2f },
/* 51  */ { "glMultiTexCoord2fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2fv },
/* 52  */ { "glMultiTexCoord2i", (GLExtFuncPtr)&nglContext::glMultiTexCoord2i },
/* 53  */ { "glMultiTexCoord2iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2iv },
/* 54  */ { "glMultiTexCoord2s", (GLExtFuncPtr)&nglContext::glMultiTexCoord2s },
/* 55  */ { "glMultiTexCoord2sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord2sv },
/* 56  */ { "glMultiTexCoord3d", (GLExtFuncPtr)&nglContext::glMultiTexCoord3d },
/* 57  */ { "glMultiTexCoord3dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3dv },
/* 58  */ { "glMultiTexCoord3f", (GLExtFuncPtr)&nglContext::glMultiTexCoord3f },
/* 59  */ { "glMultiTexCoord3fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3fv },
/* 60  */ { "glMultiTexCoord3i", (GLExtFuncPtr)&nglContext::glMultiTexCoord3i },
/* 61  */ { "glMultiTexCoord3iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3iv },
/* 62  */ { "glMultiTexCoord3s", (GLExtFuncPtr)&nglContext::glMultiTexCoord3s },
/* 63  */ { "glMultiTexCoord3sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord3sv },
/* 64  */ { "glMultiTexCoord4d", (GLExtFuncPtr)&nglContext::glMultiTexCoord4d },
/* 65  */ { "glMultiTexCoord4dv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4dv },
/* 66  */ { "glMultiTexCoord4f", (GLExtFuncPtr)&nglContext::glMultiTexCoord4f },
/* 67  */ { "glMultiTexCoord4fv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4fv },
/* 68  */ { "glMultiTexCoord4i", (GLExtFuncPtr)&nglContext::glMultiTexCoord4i },
/* 69  */ { "glMultiTexCoord4iv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4iv },
/* 70  */ { "glMultiTexCoord4s", (GLExtFuncPtr)&nglContext::glMultiTexCoord4s },
/* 71  */ { "glMultiTexCoord4sv", (GLExtFuncPtr)&nglContext::glMultiTexCoord4sv },
/* 72  */ { "glLoadTransposeMatrixf", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixf },
/* 73  */ { "glLoadTransposeMatrixd", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixd },
/* 74  */ { "glMultTransposeMatrixf", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixf },
/* 75  */ { "glMultTransposeMatrixd", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixd },
/* 76  */ { "glSampleCoverage", (GLExtFuncPtr)&nglContext::glSampleCoverage },
/* 77  */ { "glCompressedTexImage3D", (GLExtFuncPtr)&nglContext::glCompressedTexImage3D },
/* 78  */ { "glCompressedTexImage2D", (GLExtFuncPtr)&nglContext::glCompressedTexImage2D },
/* 79  */ { "glCompressedTexImage1D", (GLExtFuncPtr)&nglContext::glCompressedTexImage1D },
/* 80  */ { "glCompressedTexSubImage3D", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage3D },
/* 81  */ { "glCompressedTexSubImage2D", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage2D },
/* 82  */ { "glCompressedTexSubImage1D", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage1D },
/* 83  */ { "glGetCompressedTexImage", (GLExtFuncPtr)&nglContext::glGetCompressedTexImage },

/* GL_VERSION_1_4
 */
/* 84  */ { "glBlendFuncSeparate", (GLExtFuncPtr)&nglContext::glBlendFuncSeparate },
/* 85  */ { "glFogCoordf", (GLExtFuncPtr)&nglContext::glFogCoordf },
/* 86  */ { "glFogCoordfv", (GLExtFuncPtr)&nglContext::glFogCoordfv },
/* 87  */ { "glFogCoordd", (GLExtFuncPtr)&nglContext::glFogCoordd },
/* 88  */ { "glFogCoorddv", (GLExtFuncPtr)&nglContext::glFogCoorddv },
/* 89  */ { "glFogCoordPointer", (GLExtFuncPtr)&nglContext::glFogCoordPointer },
/* 90  */ { "glMultiDrawArrays", (GLExtFuncPtr)&nglContext::glMultiDrawArrays },
/* 91  */ { "glMultiDrawElements", (GLExtFuncPtr)&nglContext::glMultiDrawElements },
/* 92  */ { "glPointParameterf", (GLExtFuncPtr)&nglContext::glPointParameterf },
/* 93  */ { "glPointParameterfv", (GLExtFuncPtr)&nglContext::glPointParameterfv },
/* 94  */ { "glPointParameteri", (GLExtFuncPtr)&nglContext::glPointParameteri },
/* 95  */ { "glPointParameteriv", (GLExtFuncPtr)&nglContext::glPointParameteriv },
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

/* GL_ARB_multitexture
 */
/* 241 */ { "glActiveTextureARB", (GLExtFuncPtr)&nglContext::glActiveTextureARB },
/* 242 */ { "glClientActiveTextureARB", (GLExtFuncPtr)&nglContext::glClientActiveTextureARB },
/* 243 */ { "glMultiTexCoord1dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1dARB },
/* 244 */ { "glMultiTexCoord1dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1dvARB },
/* 245 */ { "glMultiTexCoord1fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1fARB },
/* 246 */ { "glMultiTexCoord1fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1fvARB },
/* 247 */ { "glMultiTexCoord1iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1iARB },
/* 248 */ { "glMultiTexCoord1ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1ivARB },
/* 249 */ { "glMultiTexCoord1sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1sARB },
/* 250 */ { "glMultiTexCoord1svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord1svARB },
/* 251 */ { "glMultiTexCoord2dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2dARB },
/* 252 */ { "glMultiTexCoord2dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2dvARB },
/* 253 */ { "glMultiTexCoord2fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2fARB },
/* 254 */ { "glMultiTexCoord2fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2fvARB },
/* 255 */ { "glMultiTexCoord2iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2iARB },
/* 256 */ { "glMultiTexCoord2ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2ivARB },
/* 257 */ { "glMultiTexCoord2sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2sARB },
/* 258 */ { "glMultiTexCoord2svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord2svARB },
/* 259 */ { "glMultiTexCoord3dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3dARB },
/* 260 */ { "glMultiTexCoord3dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3dvARB },
/* 261 */ { "glMultiTexCoord3fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3fARB },
/* 262 */ { "glMultiTexCoord3fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3fvARB },
/* 263 */ { "glMultiTexCoord3iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3iARB },
/* 264 */ { "glMultiTexCoord3ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3ivARB },
/* 265 */ { "glMultiTexCoord3sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3sARB },
/* 266 */ { "glMultiTexCoord3svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord3svARB },
/* 267 */ { "glMultiTexCoord4dARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4dARB },
/* 268 */ { "glMultiTexCoord4dvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4dvARB },
/* 269 */ { "glMultiTexCoord4fARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4fARB },
/* 270 */ { "glMultiTexCoord4fvARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4fvARB },
/* 271 */ { "glMultiTexCoord4iARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4iARB },
/* 272 */ { "glMultiTexCoord4ivARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4ivARB },
/* 273 */ { "glMultiTexCoord4sARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4sARB },
/* 274 */ { "glMultiTexCoord4svARB", (GLExtFuncPtr)&nglContext::glMultiTexCoord4svARB },

/* GL_ARB_transpose_matrix
 */
/* 275 */ { "glLoadTransposeMatrixfARB", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixfARB },
/* 276 */ { "glLoadTransposeMatrixdARB", (GLExtFuncPtr)&nglContext::glLoadTransposeMatrixdARB },
/* 277 */ { "glMultTransposeMatrixfARB", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixfARB },
/* 278 */ { "glMultTransposeMatrixdARB", (GLExtFuncPtr)&nglContext::glMultTransposeMatrixdARB },

/* GL_ARB_multisample
 */
/* 279 */ { "glSampleCoverageARB", (GLExtFuncPtr)&nglContext::glSampleCoverageARB },

/* GL_ARB_texture_compression
 */
/* 280 */ { "glCompressedTexImage3DARB", (GLExtFuncPtr)&nglContext::glCompressedTexImage3DARB },
/* 281 */ { "glCompressedTexImage2DARB", (GLExtFuncPtr)&nglContext::glCompressedTexImage2DARB },
/* 282 */ { "glCompressedTexImage1DARB", (GLExtFuncPtr)&nglContext::glCompressedTexImage1DARB },
/* 283 */ { "glCompressedTexSubImage3DARB", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage3DARB },
/* 284 */ { "glCompressedTexSubImage2DARB", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage2DARB },
/* 285 */ { "glCompressedTexSubImage1DARB", (GLExtFuncPtr)&nglContext::glCompressedTexSubImage1DARB },
/* 286 */ { "glGetCompressedTexImageARB", (GLExtFuncPtr)&nglContext::glGetCompressedTexImageARB },

/* GL_ARB_point_parameters
 */
/* 287 */ { "glPointParameterfARB", (GLExtFuncPtr)&nglContext::glPointParameterfARB },
/* 288 */ { "glPointParameterfvARB", (GLExtFuncPtr)&nglContext::glPointParameterfvARB },

/* GL_ARB_vertex_blend
 */
/* 289 */ { "glWeightbvARB", (GLExtFuncPtr)&nglContext::glWeightbvARB },
/* 290 */ { "glWeightsvARB", (GLExtFuncPtr)&nglContext::glWeightsvARB },
/* 291 */ { "glWeightivARB", (GLExtFuncPtr)&nglContext::glWeightivARB },
/* 292 */ { "glWeightfvARB", (GLExtFuncPtr)&nglContext::glWeightfvARB },
/* 293 */ { "glWeightdvARB", (GLExtFuncPtr)&nglContext::glWeightdvARB },
/* 294 */ { "glWeightubvARB", (GLExtFuncPtr)&nglContext::glWeightubvARB },
/* 295 */ { "glWeightusvARB", (GLExtFuncPtr)&nglContext::glWeightusvARB },
/* 296 */ { "glWeightuivARB", (GLExtFuncPtr)&nglContext::glWeightuivARB },
/* 297 */ { "glWeightPointerARB", (GLExtFuncPtr)&nglContext::glWeightPointerARB },
/* 298 */ { "glVertexBlendARB", (GLExtFuncPtr)&nglContext::glVertexBlendARB },

/* GL_ARB_matrix_palette
 */
/* 299 */ { "glCurrentPaletteMatrixARB", (GLExtFuncPtr)&nglContext::glCurrentPaletteMatrixARB },
/* 300 */ { "glMatrixIndexubvARB", (GLExtFuncPtr)&nglContext::glMatrixIndexubvARB },
/* 301 */ { "glMatrixIndexusvARB", (GLExtFuncPtr)&nglContext::glMatrixIndexusvARB },
/* 302 */ { "glMatrixIndexuivARB", (GLExtFuncPtr)&nglContext::glMatrixIndexuivARB },
/* 303 */ { "glMatrixIndexPointerARB", (GLExtFuncPtr)&nglContext::glMatrixIndexPointerARB },

/* GL_ARB_window_pos
 */
/* 304 */ { "glWindowPos2dARB", (GLExtFuncPtr)&nglContext::glWindowPos2dARB },
/* 305 */ { "glWindowPos2dvARB", (GLExtFuncPtr)&nglContext::glWindowPos2dvARB },
/* 306 */ { "glWindowPos2fARB", (GLExtFuncPtr)&nglContext::glWindowPos2fARB },
/* 307 */ { "glWindowPos2fvARB", (GLExtFuncPtr)&nglContext::glWindowPos2fvARB },
/* 308 */ { "glWindowPos2iARB", (GLExtFuncPtr)&nglContext::glWindowPos2iARB },
/* 309 */ { "glWindowPos2ivARB", (GLExtFuncPtr)&nglContext::glWindowPos2ivARB },
/* 310 */ { "glWindowPos2sARB", (GLExtFuncPtr)&nglContext::glWindowPos2sARB },
/* 311 */ { "glWindowPos2svARB", (GLExtFuncPtr)&nglContext::glWindowPos2svARB },
/* 312 */ { "glWindowPos3dARB", (GLExtFuncPtr)&nglContext::glWindowPos3dARB },
/* 313 */ { "glWindowPos3dvARB", (GLExtFuncPtr)&nglContext::glWindowPos3dvARB },
/* 314 */ { "glWindowPos3fARB", (GLExtFuncPtr)&nglContext::glWindowPos3fARB },
/* 315 */ { "glWindowPos3fvARB", (GLExtFuncPtr)&nglContext::glWindowPos3fvARB },
/* 316 */ { "glWindowPos3iARB", (GLExtFuncPtr)&nglContext::glWindowPos3iARB },
/* 317 */ { "glWindowPos3ivARB", (GLExtFuncPtr)&nglContext::glWindowPos3ivARB },
/* 318 */ { "glWindowPos3sARB", (GLExtFuncPtr)&nglContext::glWindowPos3sARB },
/* 319 */ { "glWindowPos3svARB", (GLExtFuncPtr)&nglContext::glWindowPos3svARB },

/* GL_ARB_vertex_program
 */
/* 320 */ { "glVertexAttrib1dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1dARB },
/* 321 */ { "glVertexAttrib1dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1dvARB },
/* 322 */ { "glVertexAttrib1fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1fARB },
/* 323 */ { "glVertexAttrib1fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1fvARB },
/* 324 */ { "glVertexAttrib1sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1sARB },
/* 325 */ { "glVertexAttrib1svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib1svARB },
/* 326 */ { "glVertexAttrib2dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2dARB },
/* 327 */ { "glVertexAttrib2dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2dvARB },
/* 328 */ { "glVertexAttrib2fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2fARB },
/* 329 */ { "glVertexAttrib2fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2fvARB },
/* 330 */ { "glVertexAttrib2sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2sARB },
/* 331 */ { "glVertexAttrib2svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib2svARB },
/* 332 */ { "glVertexAttrib3dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3dARB },
/* 333 */ { "glVertexAttrib3dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3dvARB },
/* 334 */ { "glVertexAttrib3fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3fARB },
/* 335 */ { "glVertexAttrib3fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3fvARB },
/* 336 */ { "glVertexAttrib3sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3sARB },
/* 337 */ { "glVertexAttrib3svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib3svARB },
/* 338 */ { "glVertexAttrib4NbvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NbvARB },
/* 339 */ { "glVertexAttrib4NivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NivARB },
/* 340 */ { "glVertexAttrib4NsvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NsvARB },
/* 341 */ { "glVertexAttrib4NubARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NubARB },
/* 342 */ { "glVertexAttrib4NubvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NubvARB },
/* 343 */ { "glVertexAttrib4NuivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NuivARB },
/* 344 */ { "glVertexAttrib4NusvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4NusvARB },
/* 345 */ { "glVertexAttrib4bvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4bvARB },
/* 346 */ { "glVertexAttrib4dARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4dARB },
/* 347 */ { "glVertexAttrib4dvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4dvARB },
/* 348 */ { "glVertexAttrib4fARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4fARB },
/* 349 */ { "glVertexAttrib4fvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4fvARB },
/* 350 */ { "glVertexAttrib4ivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4ivARB },
/* 351 */ { "glVertexAttrib4sARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4sARB },
/* 352 */ { "glVertexAttrib4svARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4svARB },
/* 353 */ { "glVertexAttrib4ubvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubvARB },
/* 354 */ { "glVertexAttrib4uivARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4uivARB },
/* 355 */ { "glVertexAttrib4usvARB", (GLExtFuncPtr)&nglContext::glVertexAttrib4usvARB },
/* 356 */ { "glVertexAttribPointerARB", (GLExtFuncPtr)&nglContext::glVertexAttribPointerARB },
/* 357 */ { "glEnableVertexAttribArrayARB", (GLExtFuncPtr)&nglContext::glEnableVertexAttribArrayARB },
/* 358 */ { "glDisableVertexAttribArrayARB", (GLExtFuncPtr)&nglContext::glDisableVertexAttribArrayARB },
/* 359 */ { "glProgramStringARB", (GLExtFuncPtr)&nglContext::glProgramStringARB },
/* 360 */ { "glBindProgramARB", (GLExtFuncPtr)&nglContext::glBindProgramARB },
/* 361 */ { "glDeleteProgramsARB", (GLExtFuncPtr)&nglContext::glDeleteProgramsARB },
/* 362 */ { "glGenProgramsARB", (GLExtFuncPtr)&nglContext::glGenProgramsARB },
/* 363 */ { "glProgramEnvParameter4dARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4dARB },
/* 364 */ { "glProgramEnvParameter4dvARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4dvARB },
/* 365 */ { "glProgramEnvParameter4fARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4fARB },
/* 366 */ { "glProgramEnvParameter4fvARB", (GLExtFuncPtr)&nglContext::glProgramEnvParameter4fvARB },
/* 367 */ { "glProgramLocalParameter4dARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4dARB },
/* 368 */ { "glProgramLocalParameter4dvARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4dvARB },
/* 369 */ { "glProgramLocalParameter4fARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4fARB },
/* 370 */ { "glProgramLocalParameter4fvARB", (GLExtFuncPtr)&nglContext::glProgramLocalParameter4fvARB },
/* 371 */ { "glGetProgramEnvParameterdvARB", (GLExtFuncPtr)&nglContext::glGetProgramEnvParameterdvARB },
/* 372 */ { "glGetProgramEnvParameterfvARB", (GLExtFuncPtr)&nglContext::glGetProgramEnvParameterfvARB },
/* 373 */ { "glGetProgramLocalParameterdvARB", (GLExtFuncPtr)&nglContext::glGetProgramLocalParameterdvARB },
/* 374 */ { "glGetProgramLocalParameterfvARB", (GLExtFuncPtr)&nglContext::glGetProgramLocalParameterfvARB },
/* 375 */ { "glGetProgramivARB", (GLExtFuncPtr)&nglContext::glGetProgramivARB },
/* 376 */ { "glGetProgramStringARB", (GLExtFuncPtr)&nglContext::glGetProgramStringARB },
/* 377 */ { "glGetVertexAttribdvARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribdvARB },
/* 378 */ { "glGetVertexAttribfvARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribfvARB },
/* 379 */ { "glGetVertexAttribivARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribivARB },
/* 380 */ { "glGetVertexAttribPointervARB", (GLExtFuncPtr)&nglContext::glGetVertexAttribPointervARB },
/* 381 */ { "glIsProgramARB", (GLExtFuncPtr)&nglContext::glIsProgramARB },

/* GL_ARB_vertex_buffer_object
 */
/* 382 */ { "glBindBufferARB", (GLExtFuncPtr)&nglContext::glBindBufferARB },
/* 383 */ { "glDeleteBuffersARB", (GLExtFuncPtr)&nglContext::glDeleteBuffersARB },
/* 384 */ { "glGenBuffersARB", (GLExtFuncPtr)&nglContext::glGenBuffersARB },
/* 385 */ { "glIsBufferARB", (GLExtFuncPtr)&nglContext::glIsBufferARB },
/* 386 */ { "glBufferDataARB", (GLExtFuncPtr)&nglContext::glBufferDataARB },
/* 387 */ { "glBufferSubDataARB", (GLExtFuncPtr)&nglContext::glBufferSubDataARB },
/* 388 */ { "glGetBufferSubDataARB", (GLExtFuncPtr)&nglContext::glGetBufferSubDataARB },
/* 389 */ { "glMapBufferARB", (GLExtFuncPtr)&nglContext::glMapBufferARB },
/* 390 */ { "glUnmapBufferARB", (GLExtFuncPtr)&nglContext::glUnmapBufferARB },
/* 391 */ { "glGetBufferParameterivARB", (GLExtFuncPtr)&nglContext::glGetBufferParameterivARB },
/* 392 */ { "glGetBufferPointervARB", (GLExtFuncPtr)&nglContext::glGetBufferPointervARB },

/* GL_ARB_occlusion_query
 */
/* 393 */ { "glGenQueriesARB", (GLExtFuncPtr)&nglContext::glGenQueriesARB },
/* 394 */ { "glDeleteQueriesARB", (GLExtFuncPtr)&nglContext::glDeleteQueriesARB },
/* 395 */ { "glIsQueryARB", (GLExtFuncPtr)&nglContext::glIsQueryARB },
/* 396 */ { "glBeginQueryARB", (GLExtFuncPtr)&nglContext::glBeginQueryARB },
/* 397 */ { "glEndQueryARB", (GLExtFuncPtr)&nglContext::glEndQueryARB },
/* 398 */ { "glGetQueryivARB", (GLExtFuncPtr)&nglContext::glGetQueryivARB },
/* 399 */ { "glGetQueryObjectivARB", (GLExtFuncPtr)&nglContext::glGetQueryObjectivARB },
/* 400 */ { "glGetQueryObjectuivARB", (GLExtFuncPtr)&nglContext::glGetQueryObjectuivARB },

/* GL_ARB_shader_objects
 */
/* 401 */ { "glDeleteObjectARB", (GLExtFuncPtr)&nglContext::glDeleteObjectARB },
/* 402 */ { "glGetHandleARB", (GLExtFuncPtr)&nglContext::glGetHandleARB },
/* 403 */ { "glDetachObjectARB", (GLExtFuncPtr)&nglContext::glDetachObjectARB },
/* 404 */ { "glCreateShaderObjectARB", (GLExtFuncPtr)&nglContext::glCreateShaderObjectARB },
/* 405 */ { "glShaderSourceARB", (GLExtFuncPtr)&nglContext::glShaderSourceARB },
/* 406 */ { "glCompileShaderARB", (GLExtFuncPtr)&nglContext::glCompileShaderARB },
/* 407 */ { "glCreateProgramObjectARB", (GLExtFuncPtr)&nglContext::glCreateProgramObjectARB },
/* 408 */ { "glAttachObjectARB", (GLExtFuncPtr)&nglContext::glAttachObjectARB },
/* 409 */ { "glLinkProgramARB", (GLExtFuncPtr)&nglContext::glLinkProgramARB },
/* 410 */ { "glUseProgramObjectARB", (GLExtFuncPtr)&nglContext::glUseProgramObjectARB },
/* 411 */ { "glValidateProgramARB", (GLExtFuncPtr)&nglContext::glValidateProgramARB },
/* 412 */ { "glUniform1fARB", (GLExtFuncPtr)&nglContext::glUniform1fARB },
/* 413 */ { "glUniform2fARB", (GLExtFuncPtr)&nglContext::glUniform2fARB },
/* 414 */ { "glUniform3fARB", (GLExtFuncPtr)&nglContext::glUniform3fARB },
/* 415 */ { "glUniform4fARB", (GLExtFuncPtr)&nglContext::glUniform4fARB },
/* 416 */ { "glUniform1iARB", (GLExtFuncPtr)&nglContext::glUniform1iARB },
/* 417 */ { "glUniform2iARB", (GLExtFuncPtr)&nglContext::glUniform2iARB },
/* 418 */ { "glUniform3iARB", (GLExtFuncPtr)&nglContext::glUniform3iARB },
/* 419 */ { "glUniform4iARB", (GLExtFuncPtr)&nglContext::glUniform4iARB },
/* 420 */ { "glUniform1fvARB", (GLExtFuncPtr)&nglContext::glUniform1fvARB },
/* 421 */ { "glUniform2fvARB", (GLExtFuncPtr)&nglContext::glUniform2fvARB },
/* 422 */ { "glUniform3fvARB", (GLExtFuncPtr)&nglContext::glUniform3fvARB },
/* 423 */ { "glUniform4fvARB", (GLExtFuncPtr)&nglContext::glUniform4fvARB },
/* 424 */ { "glUniform1ivARB", (GLExtFuncPtr)&nglContext::glUniform1ivARB },
/* 425 */ { "glUniform2ivARB", (GLExtFuncPtr)&nglContext::glUniform2ivARB },
/* 426 */ { "glUniform3ivARB", (GLExtFuncPtr)&nglContext::glUniform3ivARB },
/* 427 */ { "glUniform4ivARB", (GLExtFuncPtr)&nglContext::glUniform4ivARB },
/* 428 */ { "glUniformMatrix2fvARB", (GLExtFuncPtr)&nglContext::glUniformMatrix2fvARB },
/* 429 */ { "glUniformMatrix3fvARB", (GLExtFuncPtr)&nglContext::glUniformMatrix3fvARB },
/* 430 */ { "glUniformMatrix4fvARB", (GLExtFuncPtr)&nglContext::glUniformMatrix4fvARB },
/* 431 */ { "glGetObjectParameterfvARB", (GLExtFuncPtr)&nglContext::glGetObjectParameterfvARB },
/* 432 */ { "glGetObjectParameterivARB", (GLExtFuncPtr)&nglContext::glGetObjectParameterivARB },
/* 433 */ { "glGetInfoLogARB", (GLExtFuncPtr)&nglContext::glGetInfoLogARB },
/* 434 */ { "glGetAttachedObjectsARB", (GLExtFuncPtr)&nglContext::glGetAttachedObjectsARB },
/* 435 */ { "glGetUniformLocationARB", (GLExtFuncPtr)&nglContext::glGetUniformLocationARB },
/* 436 */ { "glGetActiveUniformARB", (GLExtFuncPtr)&nglContext::glGetActiveUniformARB },
/* 437 */ { "glGetUniformfvARB", (GLExtFuncPtr)&nglContext::glGetUniformfvARB },
/* 438 */ { "glGetUniformivARB", (GLExtFuncPtr)&nglContext::glGetUniformivARB },
/* 439 */ { "glGetShaderSourceARB", (GLExtFuncPtr)&nglContext::glGetShaderSourceARB },

/* GL_ARB_vertex_shader
 */
/* 440 */ { "glBindAttribLocationARB", (GLExtFuncPtr)&nglContext::glBindAttribLocationARB },
/* 441 */ { "glGetActiveAttribARB", (GLExtFuncPtr)&nglContext::glGetActiveAttribARB },
/* 442 */ { "glGetAttribLocationARB", (GLExtFuncPtr)&nglContext::glGetAttribLocationARB },

/* GL_ARB_draw_buffers
 */
/* 443 */ { "glDrawBuffersARB", (GLExtFuncPtr)&nglContext::glDrawBuffersARB },

/* GL_ARB_color_buffer_float
 */
/* 444 */ { "glClampColorARB", (GLExtFuncPtr)&nglContext::glClampColorARB },

/* GL_EXT_blend_color
 */
/* 445 */ { "glBlendColorEXT", (GLExtFuncPtr)&nglContext::glBlendColorEXT },

/* GL_EXT_polygon_offset
 */
/* 446 */ { "glPolygonOffsetEXT", (GLExtFuncPtr)&nglContext::glPolygonOffsetEXT },

/* GL_EXT_texture3D
 */
/* 447 */ { "glTexImage3DEXT", (GLExtFuncPtr)&nglContext::glTexImage3DEXT },
/* 448 */ { "glTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glTexSubImage3DEXT },

/* GL_SGIS_texture_filter4
 */
/* 449 */ { "glGetTexFilterFuncSGIS", (GLExtFuncPtr)&nglContext::glGetTexFilterFuncSGIS },
/* 450 */ { "glTexFilterFuncSGIS", (GLExtFuncPtr)&nglContext::glTexFilterFuncSGIS },

/* GL_EXT_subtexture
 */
/* 451 */ { "glTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glTexSubImage1DEXT },
/* 452 */ { "glTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glTexSubImage2DEXT },

/* GL_EXT_copy_texture
 */
/* 453 */ { "glCopyTexImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyTexImage1DEXT },
/* 454 */ { "glCopyTexImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyTexImage2DEXT },
/* 455 */ { "glCopyTexSubImage1DEXT", (GLExtFuncPtr)&nglContext::glCopyTexSubImage1DEXT },
/* 456 */ { "glCopyTexSubImage2DEXT", (GLExtFuncPtr)&nglContext::glCopyTexSubImage2DEXT },
/* 457 */ { "glCopyTexSubImage3DEXT", (GLExtFuncPtr)&nglContext::glCopyTexSubImage3DEXT },

/* GL_EXT_histogram
 */
/* 458 */ { "glGetHistogramEXT", (GLExtFuncPtr)&nglContext::glGetHistogramEXT },
/* 459 */ { "glGetHistogramParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetHistogramParameterfvEXT },
/* 460 */ { "glGetHistogramParameterivEXT", (GLExtFuncPtr)&nglContext::glGetHistogramParameterivEXT },
/* 461 */ { "glGetMinmaxEXT", (GLExtFuncPtr)&nglContext::glGetMinmaxEXT },
/* 462 */ { "glGetMinmaxParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetMinmaxParameterfvEXT },
/* 463 */ { "glGetMinmaxParameterivEXT", (GLExtFuncPtr)&nglContext::glGetMinmaxParameterivEXT },
/* 464 */ { "glHistogramEXT", (GLExtFuncPtr)&nglContext::glHistogramEXT },
/* 465 */ { "glMinmaxEXT", (GLExtFuncPtr)&nglContext::glMinmaxEXT },
/* 466 */ { "glResetHistogramEXT", (GLExtFuncPtr)&nglContext::glResetHistogramEXT },
/* 467 */ { "glResetMinmaxEXT", (GLExtFuncPtr)&nglContext::glResetMinmaxEXT },

/* GL_EXT_convolution
 */
/* 468 */ { "glConvolutionFilter1DEXT", (GLExtFuncPtr)&nglContext::glConvolutionFilter1DEXT },
/* 469 */ { "glConvolutionFilter2DEXT", (GLExtFuncPtr)&nglContext::glConvolutionFilter2DEXT },
/* 470 */ { "glConvolutionParameterfEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameterfEXT },
/* 471 */ { "glConvolutionParameterfvEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameterfvEXT },
/* 472 */ { "glConvolutionParameteriEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameteriEXT },
/* 473 */ { "glConvolutionParameterivEXT", (GLExtFuncPtr)&nglContext::glConvolutionParameterivEXT },
/* 474 */ { "glCopyConvolutionFilter1DEXT", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter1DEXT },
/* 475 */ { "glCopyConvolutionFilter2DEXT", (GLExtFuncPtr)&nglContext::glCopyConvolutionFilter2DEXT },
/* 476 */ { "glGetConvolutionFilterEXT", (GLExtFuncPtr)&nglContext::glGetConvolutionFilterEXT },
/* 477 */ { "glGetConvolutionParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetConvolutionParameterfvEXT },
/* 478 */ { "glGetConvolutionParameterivEXT", (GLExtFuncPtr)&nglContext::glGetConvolutionParameterivEXT },
/* 479 */ { "glGetSeparableFilterEXT", (GLExtFuncPtr)&nglContext::glGetSeparableFilterEXT },
/* 480 */ { "glSeparableFilter2DEXT", (GLExtFuncPtr)&nglContext::glSeparableFilter2DEXT },

/* GL_SGI_color_table
 */
/* 481 */ { "glColorTableSGI", (GLExtFuncPtr)&nglContext::glColorTableSGI },
/* 482 */ { "glColorTableParameterfvSGI", (GLExtFuncPtr)&nglContext::glColorTableParameterfvSGI },
/* 483 */ { "glColorTableParameterivSGI", (GLExtFuncPtr)&nglContext::glColorTableParameterivSGI },
/* 484 */ { "glCopyColorTableSGI", (GLExtFuncPtr)&nglContext::glCopyColorTableSGI },
/* 485 */ { "glGetColorTableSGI", (GLExtFuncPtr)&nglContext::glGetColorTableSGI },
/* 486 */ { "glGetColorTableParameterfvSGI", (GLExtFuncPtr)&nglContext::glGetColorTableParameterfvSGI },
/* 487 */ { "glGetColorTableParameterivSGI", (GLExtFuncPtr)&nglContext::glGetColorTableParameterivSGI },

/* GL_SGIX_pixel_texture
 */
/* 488 */ { "glPixelTexGenSGIX", (GLExtFuncPtr)&nglContext::glPixelTexGenSGIX },

/* GL_SGIS_pixel_texture
 */
/* 489 */ { "glPixelTexGenParameteriSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameteriSGIS },
/* 490 */ { "glPixelTexGenParameterivSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameterivSGIS },
/* 491 */ { "glPixelTexGenParameterfSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameterfSGIS },
/* 492 */ { "glPixelTexGenParameterfvSGIS", (GLExtFuncPtr)&nglContext::glPixelTexGenParameterfvSGIS },
/* 493 */ { "glGetPixelTexGenParameterivSGIS", (GLExtFuncPtr)&nglContext::glGetPixelTexGenParameterivSGIS },
/* 494 */ { "glGetPixelTexGenParameterfvSGIS", (GLExtFuncPtr)&nglContext::glGetPixelTexGenParameterfvSGIS },

/* GL_SGIS_texture4D
 */
/* 495 */ { "glTexImage4DSGIS", (GLExtFuncPtr)&nglContext::glTexImage4DSGIS },
/* 496 */ { "glTexSubImage4DSGIS", (GLExtFuncPtr)&nglContext::glTexSubImage4DSGIS },

/* GL_EXT_texture_object
 */
/* 497 */ { "glAreTexturesResidentEXT", (GLExtFuncPtr)&nglContext::glAreTexturesResidentEXT },
/* 498 */ { "glBindTextureEXT", (GLExtFuncPtr)&nglContext::glBindTextureEXT },
/* 499 */ { "glDeleteTexturesEXT", (GLExtFuncPtr)&nglContext::glDeleteTexturesEXT },
/* 500 */ { "glGenTexturesEXT", (GLExtFuncPtr)&nglContext::glGenTexturesEXT },
/* 501 */ { "glIsTextureEXT", (GLExtFuncPtr)&nglContext::glIsTextureEXT },
/* 502 */ { "glPrioritizeTexturesEXT", (GLExtFuncPtr)&nglContext::glPrioritizeTexturesEXT },

/* GL_SGIS_detail_texture
 */
/* 503 */ { "glDetailTexFuncSGIS", (GLExtFuncPtr)&nglContext::glDetailTexFuncSGIS },
/* 504 */ { "glGetDetailTexFuncSGIS", (GLExtFuncPtr)&nglContext::glGetDetailTexFuncSGIS },

/* GL_SGIS_sharpen_texture
 */
/* 505 */ { "glSharpenTexFuncSGIS", (GLExtFuncPtr)&nglContext::glSharpenTexFuncSGIS },
/* 506 */ { "glGetSharpenTexFuncSGIS", (GLExtFuncPtr)&nglContext::glGetSharpenTexFuncSGIS },

/* GL_SGIS_multisample
 */
/* 507 */ { "glSampleMaskSGIS", (GLExtFuncPtr)&nglContext::glSampleMaskSGIS },
/* 508 */ { "glSamplePatternSGIS", (GLExtFuncPtr)&nglContext::glSamplePatternSGIS },

/* GL_EXT_vertex_array
 */
/* 509 */ { "glArrayElementEXT", (GLExtFuncPtr)&nglContext::glArrayElementEXT },
/* 510 */ { "glColorPointerEXT", (GLExtFuncPtr)&nglContext::glColorPointerEXT },
/* 511 */ { "glDrawArraysEXT", (GLExtFuncPtr)&nglContext::glDrawArraysEXT },
/* 512 */ { "glEdgeFlagPointerEXT", (GLExtFuncPtr)&nglContext::glEdgeFlagPointerEXT },
/* 513 */ { "glGetPointervEXT", (GLExtFuncPtr)&nglContext::glGetPointervEXT },
/* 514 */ { "glIndexPointerEXT", (GLExtFuncPtr)&nglContext::glIndexPointerEXT },
/* 515 */ { "glNormalPointerEXT", (GLExtFuncPtr)&nglContext::glNormalPointerEXT },
/* 516 */ { "glTexCoordPointerEXT", (GLExtFuncPtr)&nglContext::glTexCoordPointerEXT },
/* 517 */ { "glVertexPointerEXT", (GLExtFuncPtr)&nglContext::glVertexPointerEXT },

/* GL_EXT_blend_minmax
 */
/* 518 */ { "glBlendEquationEXT", (GLExtFuncPtr)&nglContext::glBlendEquationEXT },

/* GL_SGIX_sprite
 */
/* 519 */ { "glSpriteParameterfSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameterfSGIX },
/* 520 */ { "glSpriteParameterfvSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameterfvSGIX },
/* 521 */ { "glSpriteParameteriSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameteriSGIX },
/* 522 */ { "glSpriteParameterivSGIX", (GLExtFuncPtr)&nglContext::glSpriteParameterivSGIX },

/* GL_EXT_point_parameters
 */
/* 523 */ { "glPointParameterfEXT", (GLExtFuncPtr)&nglContext::glPointParameterfEXT },
/* 524 */ { "glPointParameterfvEXT", (GLExtFuncPtr)&nglContext::glPointParameterfvEXT },

/* GL_SGIS_point_parameters
 */
/* 525 */ { "glPointParameterfSGIS", (GLExtFuncPtr)&nglContext::glPointParameterfSGIS },
/* 526 */ { "glPointParameterfvSGIS", (GLExtFuncPtr)&nglContext::glPointParameterfvSGIS },

/* GL_SGIX_instruments
 */
/* 527 */ { "glGetInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glGetInstrumentsSGIX },
/* 528 */ { "glInstrumentsBufferSGIX", (GLExtFuncPtr)&nglContext::glInstrumentsBufferSGIX },
/* 529 */ { "glPollInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glPollInstrumentsSGIX },
/* 530 */ { "glReadInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glReadInstrumentsSGIX },
/* 531 */ { "glStartInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glStartInstrumentsSGIX },
/* 532 */ { "glStopInstrumentsSGIX", (GLExtFuncPtr)&nglContext::glStopInstrumentsSGIX },

/* GL_SGIX_framezoom
 */
/* 533 */ { "glFrameZoomSGIX", (GLExtFuncPtr)&nglContext::glFrameZoomSGIX },

/* GL_SGIX_tag_sample_buffer
 */
/* 534 */ { "glTagSampleBufferSGIX", (GLExtFuncPtr)&nglContext::glTagSampleBufferSGIX },

/* GL_SGIX_polynomial_ffd
 */
/* 535 */ { "glDeformationMap3dSGIX", (GLExtFuncPtr)&nglContext::glDeformationMap3dSGIX },
/* 536 */ { "glDeformationMap3fSGIX", (GLExtFuncPtr)&nglContext::glDeformationMap3fSGIX },
/* 537 */ { "glDeformSGIX", (GLExtFuncPtr)&nglContext::glDeformSGIX },
/* 538 */ { "glLoadIdentityDeformationMapSGIX", (GLExtFuncPtr)&nglContext::glLoadIdentityDeformationMapSGIX },

/* GL_SGIX_reference_plane
 */
/* 539 */ { "glReferencePlaneSGIX", (GLExtFuncPtr)&nglContext::glReferencePlaneSGIX },

/* GL_SGIX_flush_raster
 */
/* 540 */ { "glFlushRasterSGIX", (GLExtFuncPtr)&nglContext::glFlushRasterSGIX },

/* GL_SGIS_fog_function
 */
/* 541 */ { "glFogFuncSGIS", (GLExtFuncPtr)&nglContext::glFogFuncSGIS },
/* 542 */ { "glGetFogFuncSGIS", (GLExtFuncPtr)&nglContext::glGetFogFuncSGIS },

/* GL_HP_image_transform
 */
/* 543 */ { "glImageTransformParameteriHP", (GLExtFuncPtr)&nglContext::glImageTransformParameteriHP },
/* 544 */ { "glImageTransformParameterfHP", (GLExtFuncPtr)&nglContext::glImageTransformParameterfHP },
/* 545 */ { "glImageTransformParameterivHP", (GLExtFuncPtr)&nglContext::glImageTransformParameterivHP },
/* 546 */ { "glImageTransformParameterfvHP", (GLExtFuncPtr)&nglContext::glImageTransformParameterfvHP },
/* 547 */ { "glGetImageTransformParameterivHP", (GLExtFuncPtr)&nglContext::glGetImageTransformParameterivHP },
/* 548 */ { "glGetImageTransformParameterfvHP", (GLExtFuncPtr)&nglContext::glGetImageTransformParameterfvHP },

/* GL_EXT_color_subtable
 */
/* 549 */ { "glColorSubTableEXT", (GLExtFuncPtr)&nglContext::glColorSubTableEXT },
/* 550 */ { "glCopyColorSubTableEXT", (GLExtFuncPtr)&nglContext::glCopyColorSubTableEXT },

/* GL_PGI_misc_hints
 */
/* 551 */ { "glHintPGI", (GLExtFuncPtr)&nglContext::glHintPGI },

/* GL_EXT_paletted_texture
 */
/* 552 */ { "glColorTableEXT", (GLExtFuncPtr)&nglContext::glColorTableEXT },
/* 553 */ { "glGetColorTableEXT", (GLExtFuncPtr)&nglContext::glGetColorTableEXT },
/* 554 */ { "glGetColorTableParameterivEXT", (GLExtFuncPtr)&nglContext::glGetColorTableParameterivEXT },
/* 555 */ { "glGetColorTableParameterfvEXT", (GLExtFuncPtr)&nglContext::glGetColorTableParameterfvEXT },

/* GL_SGIX_list_priority
 */
/* 556 */ { "glGetListParameterfvSGIX", (GLExtFuncPtr)&nglContext::glGetListParameterfvSGIX },
/* 557 */ { "glGetListParameterivSGIX", (GLExtFuncPtr)&nglContext::glGetListParameterivSGIX },
/* 558 */ { "glListParameterfSGIX", (GLExtFuncPtr)&nglContext::glListParameterfSGIX },
/* 559 */ { "glListParameterfvSGIX", (GLExtFuncPtr)&nglContext::glListParameterfvSGIX },
/* 560 */ { "glListParameteriSGIX", (GLExtFuncPtr)&nglContext::glListParameteriSGIX },
/* 561 */ { "glListParameterivSGIX", (GLExtFuncPtr)&nglContext::glListParameterivSGIX },

/* GL_EXT_index_material
 */
/* 562 */ { "glIndexMaterialEXT", (GLExtFuncPtr)&nglContext::glIndexMaterialEXT },

/* GL_EXT_index_func
 */
/* 563 */ { "glIndexFuncEXT", (GLExtFuncPtr)&nglContext::glIndexFuncEXT },

/* GL_EXT_compiled_vertex_array
 */
/* 564 */ { "glLockArraysEXT", (GLExtFuncPtr)&nglContext::glLockArraysEXT },
/* 565 */ { "glUnlockArraysEXT", (GLExtFuncPtr)&nglContext::glUnlockArraysEXT },

/* GL_EXT_cull_vertex
 */
/* 566 */ { "glCullParameterdvEXT", (GLExtFuncPtr)&nglContext::glCullParameterdvEXT },
/* 567 */ { "glCullParameterfvEXT", (GLExtFuncPtr)&nglContext::glCullParameterfvEXT },

/* GL_SGIX_fragment_lighting
 */
/* 568 */ { "glFragmentColorMaterialSGIX", (GLExtFuncPtr)&nglContext::glFragmentColorMaterialSGIX },
/* 569 */ { "glFragmentLightfSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightfSGIX },
/* 570 */ { "glFragmentLightfvSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightfvSGIX },
/* 571 */ { "glFragmentLightiSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightiSGIX },
/* 572 */ { "glFragmentLightivSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightivSGIX },
/* 573 */ { "glFragmentLightModelfSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModelfSGIX },
/* 574 */ { "glFragmentLightModelfvSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModelfvSGIX },
/* 575 */ { "glFragmentLightModeliSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModeliSGIX },
/* 576 */ { "glFragmentLightModelivSGIX", (GLExtFuncPtr)&nglContext::glFragmentLightModelivSGIX },
/* 577 */ { "glFragmentMaterialfSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialfSGIX },
/* 578 */ { "glFragmentMaterialfvSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialfvSGIX },
/* 579 */ { "glFragmentMaterialiSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialiSGIX },
/* 580 */ { "glFragmentMaterialivSGIX", (GLExtFuncPtr)&nglContext::glFragmentMaterialivSGIX },
/* 581 */ { "glGetFragmentLightfvSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentLightfvSGIX },
/* 582 */ { "glGetFragmentLightivSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentLightivSGIX },
/* 583 */ { "glGetFragmentMaterialfvSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentMaterialfvSGIX },
/* 584 */ { "glGetFragmentMaterialivSGIX", (GLExtFuncPtr)&nglContext::glGetFragmentMaterialivSGIX },
/* 585 */ { "glLightEnviSGIX", (GLExtFuncPtr)&nglContext::glLightEnviSGIX },

/* GL_EXT_draw_range_elements
 */
/* 586 */ { "glDrawRangeElementsEXT", (GLExtFuncPtr)&nglContext::glDrawRangeElementsEXT },

/* GL_EXT_light_texture
 */
/* 587 */ { "glApplyTextureEXT", (GLExtFuncPtr)&nglContext::glApplyTextureEXT },
/* 588 */ { "glTextureLightEXT", (GLExtFuncPtr)&nglContext::glTextureLightEXT },
/* 589 */ { "glTextureMaterialEXT", (GLExtFuncPtr)&nglContext::glTextureMaterialEXT },

/* GL_SGIX_async
 */
/* 590 */ { "glAsyncMarkerSGIX", (GLExtFuncPtr)&nglContext::glAsyncMarkerSGIX },
/* 591 */ { "glFinishAsyncSGIX", (GLExtFuncPtr)&nglContext::glFinishAsyncSGIX },
/* 592 */ { "glPollAsyncSGIX", (GLExtFuncPtr)&nglContext::glPollAsyncSGIX },
/* 593 */ { "glGenAsyncMarkersSGIX", (GLExtFuncPtr)&nglContext::glGenAsyncMarkersSGIX },
/* 594 */ { "glDeleteAsyncMarkersSGIX", (GLExtFuncPtr)&nglContext::glDeleteAsyncMarkersSGIX },
/* 595 */ { "glIsAsyncMarkerSGIX", (GLExtFuncPtr)&nglContext::glIsAsyncMarkerSGIX },

/* GL_INTEL_parallel_arrays
 */
/* 596 */ { "glVertexPointervINTEL", (GLExtFuncPtr)&nglContext::glVertexPointervINTEL },
/* 597 */ { "glNormalPointervINTEL", (GLExtFuncPtr)&nglContext::glNormalPointervINTEL },
/* 598 */ { "glColorPointervINTEL", (GLExtFuncPtr)&nglContext::glColorPointervINTEL },
/* 599 */ { "glTexCoordPointervINTEL", (GLExtFuncPtr)&nglContext::glTexCoordPointervINTEL },

/* GL_EXT_pixel_transform
 */
/* 600 */ { "glPixelTransformParameteriEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameteriEXT },
/* 601 */ { "glPixelTransformParameterfEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameterfEXT },
/* 602 */ { "glPixelTransformParameterivEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameterivEXT },
/* 603 */ { "glPixelTransformParameterfvEXT", (GLExtFuncPtr)&nglContext::glPixelTransformParameterfvEXT },

/* GL_EXT_secondary_color
 */
/* 604 */ { "glSecondaryColor3bEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3bEXT },
/* 605 */ { "glSecondaryColor3bvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3bvEXT },
/* 606 */ { "glSecondaryColor3dEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3dEXT },
/* 607 */ { "glSecondaryColor3dvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3dvEXT },
/* 608 */ { "glSecondaryColor3fEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3fEXT },
/* 609 */ { "glSecondaryColor3fvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3fvEXT },
/* 610 */ { "glSecondaryColor3iEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3iEXT },
/* 611 */ { "glSecondaryColor3ivEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3ivEXT },
/* 612 */ { "glSecondaryColor3sEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3sEXT },
/* 613 */ { "glSecondaryColor3svEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3svEXT },
/* 614 */ { "glSecondaryColor3ubEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3ubEXT },
/* 615 */ { "glSecondaryColor3ubvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3ubvEXT },
/* 616 */ { "glSecondaryColor3uiEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3uiEXT },
/* 617 */ { "glSecondaryColor3uivEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3uivEXT },
/* 618 */ { "glSecondaryColor3usEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3usEXT },
/* 619 */ { "glSecondaryColor3usvEXT", (GLExtFuncPtr)&nglContext::glSecondaryColor3usvEXT },
/* 620 */ { "glSecondaryColorPointerEXT", (GLExtFuncPtr)&nglContext::glSecondaryColorPointerEXT },

/* GL_EXT_texture_perturb_normal
 */
/* 621 */ { "glTextureNormalEXT", (GLExtFuncPtr)&nglContext::glTextureNormalEXT },

/* GL_EXT_multi_draw_arrays
 */
/* 622 */ { "glMultiDrawArraysEXT", (GLExtFuncPtr)&nglContext::glMultiDrawArraysEXT },
/* 623 */ { "glMultiDrawElementsEXT", (GLExtFuncPtr)&nglContext::glMultiDrawElementsEXT },

/* GL_EXT_fog_coord
 */
/* 624 */ { "glFogCoordfEXT", (GLExtFuncPtr)&nglContext::glFogCoordfEXT },
/* 625 */ { "glFogCoordfvEXT", (GLExtFuncPtr)&nglContext::glFogCoordfvEXT },
/* 626 */ { "glFogCoorddEXT", (GLExtFuncPtr)&nglContext::glFogCoorddEXT },
/* 627 */ { "glFogCoorddvEXT", (GLExtFuncPtr)&nglContext::glFogCoorddvEXT },
/* 628 */ { "glFogCoordPointerEXT", (GLExtFuncPtr)&nglContext::glFogCoordPointerEXT },

/* GL_EXT_coordinate_frame
 */
/* 629 */ { "glTangent3bEXT", (GLExtFuncPtr)&nglContext::glTangent3bEXT },
/* 630 */ { "glTangent3bvEXT", (GLExtFuncPtr)&nglContext::glTangent3bvEXT },
/* 631 */ { "glTangent3dEXT", (GLExtFuncPtr)&nglContext::glTangent3dEXT },
/* 632 */ { "glTangent3dvEXT", (GLExtFuncPtr)&nglContext::glTangent3dvEXT },
/* 633 */ { "glTangent3fEXT", (GLExtFuncPtr)&nglContext::glTangent3fEXT },
/* 634 */ { "glTangent3fvEXT", (GLExtFuncPtr)&nglContext::glTangent3fvEXT },
/* 635 */ { "glTangent3iEXT", (GLExtFuncPtr)&nglContext::glTangent3iEXT },
/* 636 */ { "glTangent3ivEXT", (GLExtFuncPtr)&nglContext::glTangent3ivEXT },
/* 637 */ { "glTangent3sEXT", (GLExtFuncPtr)&nglContext::glTangent3sEXT },
/* 638 */ { "glTangent3svEXT", (GLExtFuncPtr)&nglContext::glTangent3svEXT },
/* 639 */ { "glBinormal3bEXT", (GLExtFuncPtr)&nglContext::glBinormal3bEXT },
/* 640 */ { "glBinormal3bvEXT", (GLExtFuncPtr)&nglContext::glBinormal3bvEXT },
/* 641 */ { "glBinormal3dEXT", (GLExtFuncPtr)&nglContext::glBinormal3dEXT },
/* 642 */ { "glBinormal3dvEXT", (GLExtFuncPtr)&nglContext::glBinormal3dvEXT },
/* 643 */ { "glBinormal3fEXT", (GLExtFuncPtr)&nglContext::glBinormal3fEXT },
/* 644 */ { "glBinormal3fvEXT", (GLExtFuncPtr)&nglContext::glBinormal3fvEXT },
/* 645 */ { "glBinormal3iEXT", (GLExtFuncPtr)&nglContext::glBinormal3iEXT },
/* 646 */ { "glBinormal3ivEXT", (GLExtFuncPtr)&nglContext::glBinormal3ivEXT },
/* 647 */ { "glBinormal3sEXT", (GLExtFuncPtr)&nglContext::glBinormal3sEXT },
/* 648 */ { "glBinormal3svEXT", (GLExtFuncPtr)&nglContext::glBinormal3svEXT },
/* 649 */ { "glTangentPointerEXT", (GLExtFuncPtr)&nglContext::glTangentPointerEXT },
/* 650 */ { "glBinormalPointerEXT", (GLExtFuncPtr)&nglContext::glBinormalPointerEXT },

/* GL_SUNX_constant_data
 */
/* 651 */ { "glFinishTextureSUNX", (GLExtFuncPtr)&nglContext::glFinishTextureSUNX },

/* GL_SUN_global_alpha
 */
/* 652 */ { "glGlobalAlphaFactorbSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorbSUN },
/* 653 */ { "glGlobalAlphaFactorsSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorsSUN },
/* 654 */ { "glGlobalAlphaFactoriSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactoriSUN },
/* 655 */ { "glGlobalAlphaFactorfSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorfSUN },
/* 656 */ { "glGlobalAlphaFactordSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactordSUN },
/* 657 */ { "glGlobalAlphaFactorubSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorubSUN },
/* 658 */ { "glGlobalAlphaFactorusSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactorusSUN },
/* 659 */ { "glGlobalAlphaFactoruiSUN", (GLExtFuncPtr)&nglContext::glGlobalAlphaFactoruiSUN },

/* GL_SUN_triangle_list
 */
/* 660 */ { "glReplacementCodeuiSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiSUN },
/* 661 */ { "glReplacementCodeusSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeusSUN },
/* 662 */ { "glReplacementCodeubSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeubSUN },
/* 663 */ { "glReplacementCodeuivSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuivSUN },
/* 664 */ { "glReplacementCodeusvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeusvSUN },
/* 665 */ { "glReplacementCodeubvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeubvSUN },
/* 666 */ { "glReplacementCodePointerSUN", (GLExtFuncPtr)&nglContext::glReplacementCodePointerSUN },

/* GL_SUN_vertex
 */
/* 667 */ { "glColor4ubVertex2fSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex2fSUN },
/* 668 */ { "glColor4ubVertex2fvSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex2fvSUN },
/* 669 */ { "glColor4ubVertex3fSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex3fSUN },
/* 670 */ { "glColor4ubVertex3fvSUN", (GLExtFuncPtr)&nglContext::glColor4ubVertex3fvSUN },
/* 671 */ { "glColor3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glColor3fVertex3fSUN },
/* 672 */ { "glColor3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glColor3fVertex3fvSUN },
/* 673 */ { "glNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glNormal3fVertex3fSUN },
/* 674 */ { "glNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glNormal3fVertex3fvSUN },
/* 675 */ { "glColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glColor4fNormal3fVertex3fSUN },
/* 676 */ { "glColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glColor4fNormal3fVertex3fvSUN },
/* 677 */ { "glTexCoord2fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fVertex3fSUN },
/* 678 */ { "glTexCoord2fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fVertex3fvSUN },
/* 679 */ { "glTexCoord4fVertex4fSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fVertex4fSUN },
/* 680 */ { "glTexCoord4fVertex4fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fVertex4fvSUN },
/* 681 */ { "glTexCoord2fColor4ubVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4ubVertex3fSUN },
/* 682 */ { "glTexCoord2fColor4ubVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4ubVertex3fvSUN },
/* 683 */ { "glTexCoord2fColor3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor3fVertex3fSUN },
/* 684 */ { "glTexCoord2fColor3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor3fVertex3fvSUN },
/* 685 */ { "glTexCoord2fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fNormal3fVertex3fSUN },
/* 686 */ { "glTexCoord2fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fNormal3fVertex3fvSUN },
/* 687 */ { "glTexCoord2fColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4fNormal3fVertex3fSUN },
/* 688 */ { "glTexCoord2fColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord2fColor4fNormal3fVertex3fvSUN },
/* 689 */ { "glTexCoord4fColor4fNormal3fVertex4fSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fColor4fNormal3fVertex4fSUN },
/* 690 */ { "glTexCoord4fColor4fNormal3fVertex4fvSUN", (GLExtFuncPtr)&nglContext::glTexCoord4fColor4fNormal3fVertex4fvSUN },
/* 691 */ { "glReplacementCodeuiVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiVertex3fSUN },
/* 692 */ { "glReplacementCodeuiVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiVertex3fvSUN },
/* 693 */ { "glReplacementCodeuiColor4ubVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4ubVertex3fSUN },
/* 694 */ { "glReplacementCodeuiColor4ubVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4ubVertex3fvSUN },
/* 695 */ { "glReplacementCodeuiColor3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor3fVertex3fSUN },
/* 696 */ { "glReplacementCodeuiColor3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor3fVertex3fvSUN },
/* 697 */ { "glReplacementCodeuiNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiNormal3fVertex3fSUN },
/* 698 */ { "glReplacementCodeuiNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiNormal3fVertex3fvSUN },
/* 699 */ { "glReplacementCodeuiColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4fNormal3fVertex3fSUN },
/* 700 */ { "glReplacementCodeuiColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiColor4fNormal3fVertex3fvSUN },
/* 701 */ { "glReplacementCodeuiTexCoord2fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fVertex3fSUN },
/* 702 */ { "glReplacementCodeuiTexCoord2fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fVertex3fvSUN },
/* 703 */ { "glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN },
/* 704 */ { "glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN },
/* 705 */ { "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN },
/* 706 */ { "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN", (GLExtFuncPtr)&nglContext::glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN },

/* GL_EXT_blend_func_separate
 */
/* 707 */ { "glBlendFuncSeparateEXT", (GLExtFuncPtr)&nglContext::glBlendFuncSeparateEXT },

/* GL_INGR_blend_func_separate
 */
/* 708 */ { "glBlendFuncSeparateINGR", (GLExtFuncPtr)&nglContext::glBlendFuncSeparateINGR },

/* GL_EXT_vertex_weighting
 */
/* 709 */ { "glVertexWeightfEXT", (GLExtFuncPtr)&nglContext::glVertexWeightfEXT },
/* 710 */ { "glVertexWeightfvEXT", (GLExtFuncPtr)&nglContext::glVertexWeightfvEXT },
/* 711 */ { "glVertexWeightPointerEXT", (GLExtFuncPtr)&nglContext::glVertexWeightPointerEXT },

/* GL_NV_vertex_array_range
 */
/* 712 */ { "glFlushVertexArrayRangeNV", (GLExtFuncPtr)&nglContext::glFlushVertexArrayRangeNV },
/* 713 */ { "glVertexArrayRangeNV", (GLExtFuncPtr)&nglContext::glVertexArrayRangeNV },

/* GL_NV_register_combiners
 */
/* 714 */ { "glCombinerParameterfvNV", (GLExtFuncPtr)&nglContext::glCombinerParameterfvNV },
/* 715 */ { "glCombinerParameterfNV", (GLExtFuncPtr)&nglContext::glCombinerParameterfNV },
/* 716 */ { "glCombinerParameterivNV", (GLExtFuncPtr)&nglContext::glCombinerParameterivNV },
/* 717 */ { "glCombinerParameteriNV", (GLExtFuncPtr)&nglContext::glCombinerParameteriNV },
/* 718 */ { "glCombinerInputNV", (GLExtFuncPtr)&nglContext::glCombinerInputNV },
/* 719 */ { "glCombinerOutputNV", (GLExtFuncPtr)&nglContext::glCombinerOutputNV },
/* 720 */ { "glFinalCombinerInputNV", (GLExtFuncPtr)&nglContext::glFinalCombinerInputNV },
/* 721 */ { "glGetCombinerInputParameterfvNV", (GLExtFuncPtr)&nglContext::glGetCombinerInputParameterfvNV },
/* 722 */ { "glGetCombinerInputParameterivNV", (GLExtFuncPtr)&nglContext::glGetCombinerInputParameterivNV },
/* 723 */ { "glGetCombinerOutputParameterfvNV", (GLExtFuncPtr)&nglContext::glGetCombinerOutputParameterfvNV },
/* 724 */ { "glGetCombinerOutputParameterivNV", (GLExtFuncPtr)&nglContext::glGetCombinerOutputParameterivNV },
/* 725 */ { "glGetFinalCombinerInputParameterfvNV", (GLExtFuncPtr)&nglContext::glGetFinalCombinerInputParameterfvNV },
/* 726 */ { "glGetFinalCombinerInputParameterivNV", (GLExtFuncPtr)&nglContext::glGetFinalCombinerInputParameterivNV },

/* GL_MESA_resize_buffers
 */
/* 727 */ { "glResizeBuffersMESA", (GLExtFuncPtr)&nglContext::glResizeBuffersMESA },

/* GL_MESA_window_pos
 */
/* 728 */ { "glWindowPos2dMESA", (GLExtFuncPtr)&nglContext::glWindowPos2dMESA },
/* 729 */ { "glWindowPos2dvMESA", (GLExtFuncPtr)&nglContext::glWindowPos2dvMESA },
/* 730 */ { "glWindowPos2fMESA", (GLExtFuncPtr)&nglContext::glWindowPos2fMESA },
/* 731 */ { "glWindowPos2fvMESA", (GLExtFuncPtr)&nglContext::glWindowPos2fvMESA },
/* 732 */ { "glWindowPos2iMESA", (GLExtFuncPtr)&nglContext::glWindowPos2iMESA },
/* 733 */ { "glWindowPos2ivMESA", (GLExtFuncPtr)&nglContext::glWindowPos2ivMESA },
/* 734 */ { "glWindowPos2sMESA", (GLExtFuncPtr)&nglContext::glWindowPos2sMESA },
/* 735 */ { "glWindowPos2svMESA", (GLExtFuncPtr)&nglContext::glWindowPos2svMESA },
/* 736 */ { "glWindowPos3dMESA", (GLExtFuncPtr)&nglContext::glWindowPos3dMESA },
/* 737 */ { "glWindowPos3dvMESA", (GLExtFuncPtr)&nglContext::glWindowPos3dvMESA },
/* 738 */ { "glWindowPos3fMESA", (GLExtFuncPtr)&nglContext::glWindowPos3fMESA },
/* 739 */ { "glWindowPos3fvMESA", (GLExtFuncPtr)&nglContext::glWindowPos3fvMESA },
/* 740 */ { "glWindowPos3iMESA", (GLExtFuncPtr)&nglContext::glWindowPos3iMESA },
/* 741 */ { "glWindowPos3ivMESA", (GLExtFuncPtr)&nglContext::glWindowPos3ivMESA },
/* 742 */ { "glWindowPos3sMESA", (GLExtFuncPtr)&nglContext::glWindowPos3sMESA },
/* 743 */ { "glWindowPos3svMESA", (GLExtFuncPtr)&nglContext::glWindowPos3svMESA },
/* 744 */ { "glWindowPos4dMESA", (GLExtFuncPtr)&nglContext::glWindowPos4dMESA },
/* 745 */ { "glWindowPos4dvMESA", (GLExtFuncPtr)&nglContext::glWindowPos4dvMESA },
/* 746 */ { "glWindowPos4fMESA", (GLExtFuncPtr)&nglContext::glWindowPos4fMESA },
/* 747 */ { "glWindowPos4fvMESA", (GLExtFuncPtr)&nglContext::glWindowPos4fvMESA },
/* 748 */ { "glWindowPos4iMESA", (GLExtFuncPtr)&nglContext::glWindowPos4iMESA },
/* 749 */ { "glWindowPos4ivMESA", (GLExtFuncPtr)&nglContext::glWindowPos4ivMESA },
/* 750 */ { "glWindowPos4sMESA", (GLExtFuncPtr)&nglContext::glWindowPos4sMESA },
/* 751 */ { "glWindowPos4svMESA", (GLExtFuncPtr)&nglContext::glWindowPos4svMESA },

/* GL_IBM_multimode_draw_arrays
 */
/* 752 */ { "glMultiModeDrawArraysIBM", (GLExtFuncPtr)&nglContext::glMultiModeDrawArraysIBM },
/* 753 */ { "glMultiModeDrawElementsIBM", (GLExtFuncPtr)&nglContext::glMultiModeDrawElementsIBM },

/* GL_IBM_vertex_array_lists
 */
/* 754 */ { "glColorPointerListIBM", (GLExtFuncPtr)&nglContext::glColorPointerListIBM },
/* 755 */ { "glSecondaryColorPointerListIBM", (GLExtFuncPtr)&nglContext::glSecondaryColorPointerListIBM },
/* 756 */ { "glEdgeFlagPointerListIBM", (GLExtFuncPtr)&nglContext::glEdgeFlagPointerListIBM },
/* 757 */ { "glFogCoordPointerListIBM", (GLExtFuncPtr)&nglContext::glFogCoordPointerListIBM },
/* 758 */ { "glIndexPointerListIBM", (GLExtFuncPtr)&nglContext::glIndexPointerListIBM },
/* 759 */ { "glNormalPointerListIBM", (GLExtFuncPtr)&nglContext::glNormalPointerListIBM },
/* 760 */ { "glTexCoordPointerListIBM", (GLExtFuncPtr)&nglContext::glTexCoordPointerListIBM },
/* 761 */ { "glVertexPointerListIBM", (GLExtFuncPtr)&nglContext::glVertexPointerListIBM },

/* GL_3DFX_tbuffer
 */
/* 762 */ { "glTbufferMask3DFX", (GLExtFuncPtr)&nglContext::glTbufferMask3DFX },

/* GL_EXT_multisample
 */
/* 763 */ { "glSampleMaskEXT", (GLExtFuncPtr)&nglContext::glSampleMaskEXT },
/* 764 */ { "glSamplePatternEXT", (GLExtFuncPtr)&nglContext::glSamplePatternEXT },

/* GL_SGIS_texture_color_mask
 */
/* 765 */ { "glTextureColorMaskSGIS", (GLExtFuncPtr)&nglContext::glTextureColorMaskSGIS },

/* GL_SGIX_igloo_interface
 */
/* 766 */ { "glIglooInterfaceSGIX", (GLExtFuncPtr)&nglContext::glIglooInterfaceSGIX },

/* GL_NV_fence
 */
/* 767 */ { "glDeleteFencesNV", (GLExtFuncPtr)&nglContext::glDeleteFencesNV },
/* 768 */ { "glGenFencesNV", (GLExtFuncPtr)&nglContext::glGenFencesNV },
/* 769 */ { "glIsFenceNV", (GLExtFuncPtr)&nglContext::glIsFenceNV },
/* 770 */ { "glTestFenceNV", (GLExtFuncPtr)&nglContext::glTestFenceNV },
/* 771 */ { "glGetFenceivNV", (GLExtFuncPtr)&nglContext::glGetFenceivNV },
/* 772 */ { "glFinishFenceNV", (GLExtFuncPtr)&nglContext::glFinishFenceNV },
/* 773 */ { "glSetFenceNV", (GLExtFuncPtr)&nglContext::glSetFenceNV },

/* GL_NV_evaluators
 */
/* 774 */ { "glMapControlPointsNV", (GLExtFuncPtr)&nglContext::glMapControlPointsNV },
/* 775 */ { "glMapParameterivNV", (GLExtFuncPtr)&nglContext::glMapParameterivNV },
/* 776 */ { "glMapParameterfvNV", (GLExtFuncPtr)&nglContext::glMapParameterfvNV },
/* 777 */ { "glGetMapControlPointsNV", (GLExtFuncPtr)&nglContext::glGetMapControlPointsNV },
/* 778 */ { "glGetMapParameterivNV", (GLExtFuncPtr)&nglContext::glGetMapParameterivNV },
/* 779 */ { "glGetMapParameterfvNV", (GLExtFuncPtr)&nglContext::glGetMapParameterfvNV },
/* 780 */ { "glGetMapAttribParameterivNV", (GLExtFuncPtr)&nglContext::glGetMapAttribParameterivNV },
/* 781 */ { "glGetMapAttribParameterfvNV", (GLExtFuncPtr)&nglContext::glGetMapAttribParameterfvNV },
/* 782 */ { "glEvalMapsNV", (GLExtFuncPtr)&nglContext::glEvalMapsNV },

/* GL_NV_register_combiners2
 */
/* 783 */ { "glCombinerStageParameterfvNV", (GLExtFuncPtr)&nglContext::glCombinerStageParameterfvNV },
/* 784 */ { "glGetCombinerStageParameterfvNV", (GLExtFuncPtr)&nglContext::glGetCombinerStageParameterfvNV },

/* GL_NV_vertex_program
 */
/* 785 */ { "glAreProgramsResidentNV", (GLExtFuncPtr)&nglContext::glAreProgramsResidentNV },
/* 786 */ { "glBindProgramNV", (GLExtFuncPtr)&nglContext::glBindProgramNV },
/* 787 */ { "glDeleteProgramsNV", (GLExtFuncPtr)&nglContext::glDeleteProgramsNV },
/* 788 */ { "glExecuteProgramNV", (GLExtFuncPtr)&nglContext::glExecuteProgramNV },
/* 789 */ { "glGenProgramsNV", (GLExtFuncPtr)&nglContext::glGenProgramsNV },
/* 790 */ { "glGetProgramParameterdvNV", (GLExtFuncPtr)&nglContext::glGetProgramParameterdvNV },
/* 791 */ { "glGetProgramParameterfvNV", (GLExtFuncPtr)&nglContext::glGetProgramParameterfvNV },
/* 792 */ { "glGetProgramivNV", (GLExtFuncPtr)&nglContext::glGetProgramivNV },
/* 793 */ { "glGetProgramStringNV", (GLExtFuncPtr)&nglContext::glGetProgramStringNV },
/* 794 */ { "glGetTrackMatrixivNV", (GLExtFuncPtr)&nglContext::glGetTrackMatrixivNV },
/* 795 */ { "glGetVertexAttribdvNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribdvNV },
/* 796 */ { "glGetVertexAttribfvNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribfvNV },
/* 797 */ { "glGetVertexAttribivNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribivNV },
/* 798 */ { "glGetVertexAttribPointervNV", (GLExtFuncPtr)&nglContext::glGetVertexAttribPointervNV },
/* 799 */ { "glIsProgramNV", (GLExtFuncPtr)&nglContext::glIsProgramNV },
/* 800 */ { "glLoadProgramNV", (GLExtFuncPtr)&nglContext::glLoadProgramNV },
/* 801 */ { "glProgramParameter4dNV", (GLExtFuncPtr)&nglContext::glProgramParameter4dNV },
/* 802 */ { "glProgramParameter4dvNV", (GLExtFuncPtr)&nglContext::glProgramParameter4dvNV },
/* 803 */ { "glProgramParameter4fNV", (GLExtFuncPtr)&nglContext::glProgramParameter4fNV },
/* 804 */ { "glProgramParameter4fvNV", (GLExtFuncPtr)&nglContext::glProgramParameter4fvNV },
/* 805 */ { "glProgramParameters4dvNV", (GLExtFuncPtr)&nglContext::glProgramParameters4dvNV },
/* 806 */ { "glProgramParameters4fvNV", (GLExtFuncPtr)&nglContext::glProgramParameters4fvNV },
/* 807 */ { "glRequestResidentProgramsNV", (GLExtFuncPtr)&nglContext::glRequestResidentProgramsNV },
/* 808 */ { "glTrackMatrixNV", (GLExtFuncPtr)&nglContext::glTrackMatrixNV },
/* 809 */ { "glVertexAttribPointerNV", (GLExtFuncPtr)&nglContext::glVertexAttribPointerNV },
/* 810 */ { "glVertexAttrib1dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1dNV },
/* 811 */ { "glVertexAttrib1dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1dvNV },
/* 812 */ { "glVertexAttrib1fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1fNV },
/* 813 */ { "glVertexAttrib1fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1fvNV },
/* 814 */ { "glVertexAttrib1sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1sNV },
/* 815 */ { "glVertexAttrib1svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1svNV },
/* 816 */ { "glVertexAttrib2dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2dNV },
/* 817 */ { "glVertexAttrib2dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2dvNV },
/* 818 */ { "glVertexAttrib2fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2fNV },
/* 819 */ { "glVertexAttrib2fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2fvNV },
/* 820 */ { "glVertexAttrib2sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2sNV },
/* 821 */ { "glVertexAttrib2svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2svNV },
/* 822 */ { "glVertexAttrib3dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3dNV },
/* 823 */ { "glVertexAttrib3dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3dvNV },
/* 824 */ { "glVertexAttrib3fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3fNV },
/* 825 */ { "glVertexAttrib3fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3fvNV },
/* 826 */ { "glVertexAttrib3sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3sNV },
/* 827 */ { "glVertexAttrib3svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3svNV },
/* 828 */ { "glVertexAttrib4dNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4dNV },
/* 829 */ { "glVertexAttrib4dvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4dvNV },
/* 830 */ { "glVertexAttrib4fNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4fNV },
/* 831 */ { "glVertexAttrib4fvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4fvNV },
/* 832 */ { "glVertexAttrib4sNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4sNV },
/* 833 */ { "glVertexAttrib4svNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4svNV },
/* 834 */ { "glVertexAttrib4ubNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubNV },
/* 835 */ { "glVertexAttrib4ubvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4ubvNV },
/* 836 */ { "glVertexAttribs1dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1dvNV },
/* 837 */ { "glVertexAttribs1fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1fvNV },
/* 838 */ { "glVertexAttribs1svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1svNV },
/* 839 */ { "glVertexAttribs2dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2dvNV },
/* 840 */ { "glVertexAttribs2fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2fvNV },
/* 841 */ { "glVertexAttribs2svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2svNV },
/* 842 */ { "glVertexAttribs3dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3dvNV },
/* 843 */ { "glVertexAttribs3fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3fvNV },
/* 844 */ { "glVertexAttribs3svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3svNV },
/* 845 */ { "glVertexAttribs4dvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4dvNV },
/* 846 */ { "glVertexAttribs4fvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4fvNV },
/* 847 */ { "glVertexAttribs4svNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4svNV },
/* 848 */ { "glVertexAttribs4ubvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4ubvNV },

/* GL_ATI_envmap_bumpmap
 */
/* 849 */ { "glTexBumpParameterivATI", (GLExtFuncPtr)&nglContext::glTexBumpParameterivATI },
/* 850 */ { "glTexBumpParameterfvATI", (GLExtFuncPtr)&nglContext::glTexBumpParameterfvATI },
/* 851 */ { "glGetTexBumpParameterivATI", (GLExtFuncPtr)&nglContext::glGetTexBumpParameterivATI },
/* 852 */ { "glGetTexBumpParameterfvATI", (GLExtFuncPtr)&nglContext::glGetTexBumpParameterfvATI },

/* GL_ATI_fragment_shader
 */
/* 853 */ { "glGenFragmentShadersATI", (GLExtFuncPtr)&nglContext::glGenFragmentShadersATI },
/* 854 */ { "glBindFragmentShaderATI", (GLExtFuncPtr)&nglContext::glBindFragmentShaderATI },
/* 855 */ { "glDeleteFragmentShaderATI", (GLExtFuncPtr)&nglContext::glDeleteFragmentShaderATI },
/* 856 */ { "glBeginFragmentShaderATI", (GLExtFuncPtr)&nglContext::glBeginFragmentShaderATI },
/* 857 */ { "glEndFragmentShaderATI", (GLExtFuncPtr)&nglContext::glEndFragmentShaderATI },
/* 858 */ { "glPassTexCoordATI", (GLExtFuncPtr)&nglContext::glPassTexCoordATI },
/* 859 */ { "glSampleMapATI", (GLExtFuncPtr)&nglContext::glSampleMapATI },
/* 860 */ { "glColorFragmentOp1ATI", (GLExtFuncPtr)&nglContext::glColorFragmentOp1ATI },
/* 861 */ { "glColorFragmentOp2ATI", (GLExtFuncPtr)&nglContext::glColorFragmentOp2ATI },
/* 862 */ { "glColorFragmentOp3ATI", (GLExtFuncPtr)&nglContext::glColorFragmentOp3ATI },
/* 863 */ { "glAlphaFragmentOp1ATI", (GLExtFuncPtr)&nglContext::glAlphaFragmentOp1ATI },
/* 864 */ { "glAlphaFragmentOp2ATI", (GLExtFuncPtr)&nglContext::glAlphaFragmentOp2ATI },
/* 865 */ { "glAlphaFragmentOp3ATI", (GLExtFuncPtr)&nglContext::glAlphaFragmentOp3ATI },
/* 866 */ { "glSetFragmentShaderConstantATI", (GLExtFuncPtr)&nglContext::glSetFragmentShaderConstantATI },

/* GL_ATI_pn_triangles
 */
/* 867 */ { "glPNTrianglesiATI", (GLExtFuncPtr)&nglContext::glPNTrianglesiATI },
/* 868 */ { "glPNTrianglesfATI", (GLExtFuncPtr)&nglContext::glPNTrianglesfATI },

/* GL_ATI_vertex_array_object
 */
/* 869 */ { "glNewObjectBufferATI", (GLExtFuncPtr)&nglContext::glNewObjectBufferATI },
/* 870 */ { "glIsObjectBufferATI", (GLExtFuncPtr)&nglContext::glIsObjectBufferATI },
/* 871 */ { "glUpdateObjectBufferATI", (GLExtFuncPtr)&nglContext::glUpdateObjectBufferATI },
/* 872 */ { "glGetObjectBufferfvATI", (GLExtFuncPtr)&nglContext::glGetObjectBufferfvATI },
/* 873 */ { "glGetObjectBufferivATI", (GLExtFuncPtr)&nglContext::glGetObjectBufferivATI },
/* 874 */ { "glFreeObjectBufferATI", (GLExtFuncPtr)&nglContext::glFreeObjectBufferATI },
/* 875 */ { "glArrayObjectATI", (GLExtFuncPtr)&nglContext::glArrayObjectATI },
/* 876 */ { "glGetArrayObjectfvATI", (GLExtFuncPtr)&nglContext::glGetArrayObjectfvATI },
/* 877 */ { "glGetArrayObjectivATI", (GLExtFuncPtr)&nglContext::glGetArrayObjectivATI },
/* 878 */ { "glVariantArrayObjectATI", (GLExtFuncPtr)&nglContext::glVariantArrayObjectATI },
/* 879 */ { "glGetVariantArrayObjectfvATI", (GLExtFuncPtr)&nglContext::glGetVariantArrayObjectfvATI },
/* 880 */ { "glGetVariantArrayObjectivATI", (GLExtFuncPtr)&nglContext::glGetVariantArrayObjectivATI },

/* GL_EXT_vertex_shader
 */
/* 881 */ { "glBeginVertexShaderEXT", (GLExtFuncPtr)&nglContext::glBeginVertexShaderEXT },
/* 882 */ { "glEndVertexShaderEXT", (GLExtFuncPtr)&nglContext::glEndVertexShaderEXT },
/* 883 */ { "glBindVertexShaderEXT", (GLExtFuncPtr)&nglContext::glBindVertexShaderEXT },
/* 884 */ { "glGenVertexShadersEXT", (GLExtFuncPtr)&nglContext::glGenVertexShadersEXT },
/* 885 */ { "glDeleteVertexShaderEXT", (GLExtFuncPtr)&nglContext::glDeleteVertexShaderEXT },
/* 886 */ { "glShaderOp1EXT", (GLExtFuncPtr)&nglContext::glShaderOp1EXT },
/* 887 */ { "glShaderOp2EXT", (GLExtFuncPtr)&nglContext::glShaderOp2EXT },
/* 888 */ { "glShaderOp3EXT", (GLExtFuncPtr)&nglContext::glShaderOp3EXT },
/* 889 */ { "glSwizzleEXT", (GLExtFuncPtr)&nglContext::glSwizzleEXT },
/* 890 */ { "glWriteMaskEXT", (GLExtFuncPtr)&nglContext::glWriteMaskEXT },
/* 891 */ { "glInsertComponentEXT", (GLExtFuncPtr)&nglContext::glInsertComponentEXT },
/* 892 */ { "glExtractComponentEXT", (GLExtFuncPtr)&nglContext::glExtractComponentEXT },
/* 893 */ { "glGenSymbolsEXT", (GLExtFuncPtr)&nglContext::glGenSymbolsEXT },
/* 894 */ { "glSetInvariantEXT", (GLExtFuncPtr)&nglContext::glSetInvariantEXT },
/* 895 */ { "glSetLocalConstantEXT", (GLExtFuncPtr)&nglContext::glSetLocalConstantEXT },
/* 896 */ { "glVariantbvEXT", (GLExtFuncPtr)&nglContext::glVariantbvEXT },
/* 897 */ { "glVariantsvEXT", (GLExtFuncPtr)&nglContext::glVariantsvEXT },
/* 898 */ { "glVariantivEXT", (GLExtFuncPtr)&nglContext::glVariantivEXT },
/* 899 */ { "glVariantfvEXT", (GLExtFuncPtr)&nglContext::glVariantfvEXT },
/* 900 */ { "glVariantdvEXT", (GLExtFuncPtr)&nglContext::glVariantdvEXT },
/* 901 */ { "glVariantubvEXT", (GLExtFuncPtr)&nglContext::glVariantubvEXT },
/* 902 */ { "glVariantusvEXT", (GLExtFuncPtr)&nglContext::glVariantusvEXT },
/* 903 */ { "glVariantuivEXT", (GLExtFuncPtr)&nglContext::glVariantuivEXT },
/* 904 */ { "glVariantPointerEXT", (GLExtFuncPtr)&nglContext::glVariantPointerEXT },
/* 905 */ { "glEnableVariantClientStateEXT", (GLExtFuncPtr)&nglContext::glEnableVariantClientStateEXT },
/* 906 */ { "glDisableVariantClientStateEXT", (GLExtFuncPtr)&nglContext::glDisableVariantClientStateEXT },
/* 907 */ { "glBindLightParameterEXT", (GLExtFuncPtr)&nglContext::glBindLightParameterEXT },
/* 908 */ { "glBindMaterialParameterEXT", (GLExtFuncPtr)&nglContext::glBindMaterialParameterEXT },
/* 909 */ { "glBindTexGenParameterEXT", (GLExtFuncPtr)&nglContext::glBindTexGenParameterEXT },
/* 910 */ { "glBindTextureUnitParameterEXT", (GLExtFuncPtr)&nglContext::glBindTextureUnitParameterEXT },
/* 911 */ { "glBindParameterEXT", (GLExtFuncPtr)&nglContext::glBindParameterEXT },
/* 912 */ { "glIsVariantEnabledEXT", (GLExtFuncPtr)&nglContext::glIsVariantEnabledEXT },
/* 913 */ { "glGetVariantBooleanvEXT", (GLExtFuncPtr)&nglContext::glGetVariantBooleanvEXT },
/* 914 */ { "glGetVariantIntegervEXT", (GLExtFuncPtr)&nglContext::glGetVariantIntegervEXT },
/* 915 */ { "glGetVariantFloatvEXT", (GLExtFuncPtr)&nglContext::glGetVariantFloatvEXT },
/* 916 */ { "glGetVariantPointervEXT", (GLExtFuncPtr)&nglContext::glGetVariantPointervEXT },
/* 917 */ { "glGetInvariantBooleanvEXT", (GLExtFuncPtr)&nglContext::glGetInvariantBooleanvEXT },
/* 918 */ { "glGetInvariantIntegervEXT", (GLExtFuncPtr)&nglContext::glGetInvariantIntegervEXT },
/* 919 */ { "glGetInvariantFloatvEXT", (GLExtFuncPtr)&nglContext::glGetInvariantFloatvEXT },
/* 920 */ { "glGetLocalConstantBooleanvEXT", (GLExtFuncPtr)&nglContext::glGetLocalConstantBooleanvEXT },
/* 921 */ { "glGetLocalConstantIntegervEXT", (GLExtFuncPtr)&nglContext::glGetLocalConstantIntegervEXT },
/* 922 */ { "glGetLocalConstantFloatvEXT", (GLExtFuncPtr)&nglContext::glGetLocalConstantFloatvEXT },

/* GL_ATI_vertex_streams
 */
/* 923 */ { "glVertexStream1sATI", (GLExtFuncPtr)&nglContext::glVertexStream1sATI },
/* 924 */ { "glVertexStream1svATI", (GLExtFuncPtr)&nglContext::glVertexStream1svATI },
/* 925 */ { "glVertexStream1iATI", (GLExtFuncPtr)&nglContext::glVertexStream1iATI },
/* 926 */ { "glVertexStream1ivATI", (GLExtFuncPtr)&nglContext::glVertexStream1ivATI },
/* 927 */ { "glVertexStream1fATI", (GLExtFuncPtr)&nglContext::glVertexStream1fATI },
/* 928 */ { "glVertexStream1fvATI", (GLExtFuncPtr)&nglContext::glVertexStream1fvATI },
/* 929 */ { "glVertexStream1dATI", (GLExtFuncPtr)&nglContext::glVertexStream1dATI },
/* 930 */ { "glVertexStream1dvATI", (GLExtFuncPtr)&nglContext::glVertexStream1dvATI },
/* 931 */ { "glVertexStream2sATI", (GLExtFuncPtr)&nglContext::glVertexStream2sATI },
/* 932 */ { "glVertexStream2svATI", (GLExtFuncPtr)&nglContext::glVertexStream2svATI },
/* 933 */ { "glVertexStream2iATI", (GLExtFuncPtr)&nglContext::glVertexStream2iATI },
/* 934 */ { "glVertexStream2ivATI", (GLExtFuncPtr)&nglContext::glVertexStream2ivATI },
/* 935 */ { "glVertexStream2fATI", (GLExtFuncPtr)&nglContext::glVertexStream2fATI },
/* 936 */ { "glVertexStream2fvATI", (GLExtFuncPtr)&nglContext::glVertexStream2fvATI },
/* 937 */ { "glVertexStream2dATI", (GLExtFuncPtr)&nglContext::glVertexStream2dATI },
/* 938 */ { "glVertexStream2dvATI", (GLExtFuncPtr)&nglContext::glVertexStream2dvATI },
/* 939 */ { "glVertexStream3sATI", (GLExtFuncPtr)&nglContext::glVertexStream3sATI },
/* 940 */ { "glVertexStream3svATI", (GLExtFuncPtr)&nglContext::glVertexStream3svATI },
/* 941 */ { "glVertexStream3iATI", (GLExtFuncPtr)&nglContext::glVertexStream3iATI },
/* 942 */ { "glVertexStream3ivATI", (GLExtFuncPtr)&nglContext::glVertexStream3ivATI },
/* 943 */ { "glVertexStream3fATI", (GLExtFuncPtr)&nglContext::glVertexStream3fATI },
/* 944 */ { "glVertexStream3fvATI", (GLExtFuncPtr)&nglContext::glVertexStream3fvATI },
/* 945 */ { "glVertexStream3dATI", (GLExtFuncPtr)&nglContext::glVertexStream3dATI },
/* 946 */ { "glVertexStream3dvATI", (GLExtFuncPtr)&nglContext::glVertexStream3dvATI },
/* 947 */ { "glVertexStream4sATI", (GLExtFuncPtr)&nglContext::glVertexStream4sATI },
/* 948 */ { "glVertexStream4svATI", (GLExtFuncPtr)&nglContext::glVertexStream4svATI },
/* 949 */ { "glVertexStream4iATI", (GLExtFuncPtr)&nglContext::glVertexStream4iATI },
/* 950 */ { "glVertexStream4ivATI", (GLExtFuncPtr)&nglContext::glVertexStream4ivATI },
/* 951 */ { "glVertexStream4fATI", (GLExtFuncPtr)&nglContext::glVertexStream4fATI },
/* 952 */ { "glVertexStream4fvATI", (GLExtFuncPtr)&nglContext::glVertexStream4fvATI },
/* 953 */ { "glVertexStream4dATI", (GLExtFuncPtr)&nglContext::glVertexStream4dATI },
/* 954 */ { "glVertexStream4dvATI", (GLExtFuncPtr)&nglContext::glVertexStream4dvATI },
/* 955 */ { "glNormalStream3bATI", (GLExtFuncPtr)&nglContext::glNormalStream3bATI },
/* 956 */ { "glNormalStream3bvATI", (GLExtFuncPtr)&nglContext::glNormalStream3bvATI },
/* 957 */ { "glNormalStream3sATI", (GLExtFuncPtr)&nglContext::glNormalStream3sATI },
/* 958 */ { "glNormalStream3svATI", (GLExtFuncPtr)&nglContext::glNormalStream3svATI },
/* 959 */ { "glNormalStream3iATI", (GLExtFuncPtr)&nglContext::glNormalStream3iATI },
/* 960 */ { "glNormalStream3ivATI", (GLExtFuncPtr)&nglContext::glNormalStream3ivATI },
/* 961 */ { "glNormalStream3fATI", (GLExtFuncPtr)&nglContext::glNormalStream3fATI },
/* 962 */ { "glNormalStream3fvATI", (GLExtFuncPtr)&nglContext::glNormalStream3fvATI },
/* 963 */ { "glNormalStream3dATI", (GLExtFuncPtr)&nglContext::glNormalStream3dATI },
/* 964 */ { "glNormalStream3dvATI", (GLExtFuncPtr)&nglContext::glNormalStream3dvATI },
/* 965 */ { "glClientActiveVertexStreamATI", (GLExtFuncPtr)&nglContext::glClientActiveVertexStreamATI },
/* 966 */ { "glVertexBlendEnviATI", (GLExtFuncPtr)&nglContext::glVertexBlendEnviATI },
/* 967 */ { "glVertexBlendEnvfATI", (GLExtFuncPtr)&nglContext::glVertexBlendEnvfATI },

/* GL_ATI_element_array
 */
/* 968 */ { "glElementPointerATI", (GLExtFuncPtr)&nglContext::glElementPointerATI },
/* 969 */ { "glDrawElementArrayATI", (GLExtFuncPtr)&nglContext::glDrawElementArrayATI },
/* 970 */ { "glDrawRangeElementArrayATI", (GLExtFuncPtr)&nglContext::glDrawRangeElementArrayATI },

/* GL_SUN_mesh_array
 */
/* 971 */ { "glDrawMeshArraysSUN", (GLExtFuncPtr)&nglContext::glDrawMeshArraysSUN },

/* GL_NV_occlusion_query
 */
/* 972 */ { "glGenOcclusionQueriesNV", (GLExtFuncPtr)&nglContext::glGenOcclusionQueriesNV },
/* 973 */ { "glDeleteOcclusionQueriesNV", (GLExtFuncPtr)&nglContext::glDeleteOcclusionQueriesNV },
/* 974 */ { "glIsOcclusionQueryNV", (GLExtFuncPtr)&nglContext::glIsOcclusionQueryNV },
/* 975 */ { "glBeginOcclusionQueryNV", (GLExtFuncPtr)&nglContext::glBeginOcclusionQueryNV },
/* 976 */ { "glEndOcclusionQueryNV", (GLExtFuncPtr)&nglContext::glEndOcclusionQueryNV },
/* 977 */ { "glGetOcclusionQueryivNV", (GLExtFuncPtr)&nglContext::glGetOcclusionQueryivNV },
/* 978 */ { "glGetOcclusionQueryuivNV", (GLExtFuncPtr)&nglContext::glGetOcclusionQueryuivNV },

/* GL_NV_point_sprite
 */
/* 979 */ { "glPointParameteriNV", (GLExtFuncPtr)&nglContext::glPointParameteriNV },
/* 980 */ { "glPointParameterivNV", (GLExtFuncPtr)&nglContext::glPointParameterivNV },

/* GL_EXT_stencil_two_side
 */
/* 981 */ { "glActiveStencilFaceEXT", (GLExtFuncPtr)&nglContext::glActiveStencilFaceEXT },

/* GL_APPLE_element_array
 */
/* 982 */ { "glElementPointerAPPLE", (GLExtFuncPtr)&nglContext::glElementPointerAPPLE },
/* 983 */ { "glDrawElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glDrawElementArrayAPPLE },
/* 984 */ { "glDrawRangeElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glDrawRangeElementArrayAPPLE },
/* 985 */ { "glMultiDrawElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glMultiDrawElementArrayAPPLE },
/* 986 */ { "glMultiDrawRangeElementArrayAPPLE", (GLExtFuncPtr)&nglContext::glMultiDrawRangeElementArrayAPPLE },

/* GL_APPLE_fence
 */
/* 987 */ { "glGenFencesAPPLE", (GLExtFuncPtr)&nglContext::glGenFencesAPPLE },
/* 988 */ { "glDeleteFencesAPPLE", (GLExtFuncPtr)&nglContext::glDeleteFencesAPPLE },
/* 989 */ { "glSetFenceAPPLE", (GLExtFuncPtr)&nglContext::glSetFenceAPPLE },
/* 990 */ { "glIsFenceAPPLE", (GLExtFuncPtr)&nglContext::glIsFenceAPPLE },
/* 991 */ { "glTestFenceAPPLE", (GLExtFuncPtr)&nglContext::glTestFenceAPPLE },
/* 992 */ { "glFinishFenceAPPLE", (GLExtFuncPtr)&nglContext::glFinishFenceAPPLE },
/* 993 */ { "glTestObjectAPPLE", (GLExtFuncPtr)&nglContext::glTestObjectAPPLE },
/* 994 */ { "glFinishObjectAPPLE", (GLExtFuncPtr)&nglContext::glFinishObjectAPPLE },

/* GL_APPLE_vertex_array_object
 */
/* 995 */ { "glBindVertexArrayAPPLE", (GLExtFuncPtr)&nglContext::glBindVertexArrayAPPLE },
/* 996 */ { "glDeleteVertexArraysAPPLE", (GLExtFuncPtr)&nglContext::glDeleteVertexArraysAPPLE },
/* 997 */ { "glGenVertexArraysAPPLE", (GLExtFuncPtr)&nglContext::glGenVertexArraysAPPLE },
/* 998 */ { "glIsVertexArrayAPPLE", (GLExtFuncPtr)&nglContext::glIsVertexArrayAPPLE },

/* GL_APPLE_vertex_array_range
 */
/* 999 */ { "glVertexArrayRangeAPPLE", (GLExtFuncPtr)&nglContext::glVertexArrayRangeAPPLE },
/* 1000 */ { "glFlushVertexArrayRangeAPPLE", (GLExtFuncPtr)&nglContext::glFlushVertexArrayRangeAPPLE },
/* 1001 */ { "glVertexArrayParameteriAPPLE", (GLExtFuncPtr)&nglContext::glVertexArrayParameteriAPPLE },

/* GL_ATI_draw_buffers
 */
/* 1002 */ { "glDrawBuffersATI", (GLExtFuncPtr)&nglContext::glDrawBuffersATI },

/* GL_NV_fragment_program
 */
/* 1003 */ { "glProgramNamedParameter4fNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4fNV },
/* 1004 */ { "glProgramNamedParameter4dNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4dNV },
/* 1005 */ { "glProgramNamedParameter4fvNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4fvNV },
/* 1006 */ { "glProgramNamedParameter4dvNV", (GLExtFuncPtr)&nglContext::glProgramNamedParameter4dvNV },
/* 1007 */ { "glGetProgramNamedParameterfvNV", (GLExtFuncPtr)&nglContext::glGetProgramNamedParameterfvNV },
/* 1008 */ { "glGetProgramNamedParameterdvNV", (GLExtFuncPtr)&nglContext::glGetProgramNamedParameterdvNV },

/* GL_NV_half_float
 */
/* 1009 */ { "glVertex2hNV", (GLExtFuncPtr)&nglContext::glVertex2hNV },
/* 1010 */ { "glVertex2hvNV", (GLExtFuncPtr)&nglContext::glVertex2hvNV },
/* 1011 */ { "glVertex3hNV", (GLExtFuncPtr)&nglContext::glVertex3hNV },
/* 1012 */ { "glVertex3hvNV", (GLExtFuncPtr)&nglContext::glVertex3hvNV },
/* 1013 */ { "glVertex4hNV", (GLExtFuncPtr)&nglContext::glVertex4hNV },
/* 1014 */ { "glVertex4hvNV", (GLExtFuncPtr)&nglContext::glVertex4hvNV },
/* 1015 */ { "glNormal3hNV", (GLExtFuncPtr)&nglContext::glNormal3hNV },
/* 1016 */ { "glNormal3hvNV", (GLExtFuncPtr)&nglContext::glNormal3hvNV },
/* 1017 */ { "glColor3hNV", (GLExtFuncPtr)&nglContext::glColor3hNV },
/* 1018 */ { "glColor3hvNV", (GLExtFuncPtr)&nglContext::glColor3hvNV },
/* 1019 */ { "glColor4hNV", (GLExtFuncPtr)&nglContext::glColor4hNV },
/* 1020 */ { "glColor4hvNV", (GLExtFuncPtr)&nglContext::glColor4hvNV },
/* 1021 */ { "glTexCoord1hNV", (GLExtFuncPtr)&nglContext::glTexCoord1hNV },
/* 1022 */ { "glTexCoord1hvNV", (GLExtFuncPtr)&nglContext::glTexCoord1hvNV },
/* 1023 */ { "glTexCoord2hNV", (GLExtFuncPtr)&nglContext::glTexCoord2hNV },
/* 1024 */ { "glTexCoord2hvNV", (GLExtFuncPtr)&nglContext::glTexCoord2hvNV },
/* 1025 */ { "glTexCoord3hNV", (GLExtFuncPtr)&nglContext::glTexCoord3hNV },
/* 1026 */ { "glTexCoord3hvNV", (GLExtFuncPtr)&nglContext::glTexCoord3hvNV },
/* 1027 */ { "glTexCoord4hNV", (GLExtFuncPtr)&nglContext::glTexCoord4hNV },
/* 1028 */ { "glTexCoord4hvNV", (GLExtFuncPtr)&nglContext::glTexCoord4hvNV },
/* 1029 */ { "glMultiTexCoord1hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord1hNV },
/* 1030 */ { "glMultiTexCoord1hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord1hvNV },
/* 1031 */ { "glMultiTexCoord2hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord2hNV },
/* 1032 */ { "glMultiTexCoord2hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord2hvNV },
/* 1033 */ { "glMultiTexCoord3hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord3hNV },
/* 1034 */ { "glMultiTexCoord3hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord3hvNV },
/* 1035 */ { "glMultiTexCoord4hNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord4hNV },
/* 1036 */ { "glMultiTexCoord4hvNV", (GLExtFuncPtr)&nglContext::glMultiTexCoord4hvNV },
/* 1037 */ { "glFogCoordhNV", (GLExtFuncPtr)&nglContext::glFogCoordhNV },
/* 1038 */ { "glFogCoordhvNV", (GLExtFuncPtr)&nglContext::glFogCoordhvNV },
/* 1039 */ { "glSecondaryColor3hNV", (GLExtFuncPtr)&nglContext::glSecondaryColor3hNV },
/* 1040 */ { "glSecondaryColor3hvNV", (GLExtFuncPtr)&nglContext::glSecondaryColor3hvNV },
/* 1041 */ { "glVertexWeighthNV", (GLExtFuncPtr)&nglContext::glVertexWeighthNV },
/* 1042 */ { "glVertexWeighthvNV", (GLExtFuncPtr)&nglContext::glVertexWeighthvNV },
/* 1043 */ { "glVertexAttrib1hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1hNV },
/* 1044 */ { "glVertexAttrib1hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib1hvNV },
/* 1045 */ { "glVertexAttrib2hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2hNV },
/* 1046 */ { "glVertexAttrib2hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib2hvNV },
/* 1047 */ { "glVertexAttrib3hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3hNV },
/* 1048 */ { "glVertexAttrib3hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib3hvNV },
/* 1049 */ { "glVertexAttrib4hNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4hNV },
/* 1050 */ { "glVertexAttrib4hvNV", (GLExtFuncPtr)&nglContext::glVertexAttrib4hvNV },
/* 1051 */ { "glVertexAttribs1hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs1hvNV },
/* 1052 */ { "glVertexAttribs2hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs2hvNV },
/* 1053 */ { "glVertexAttribs3hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs3hvNV },
/* 1054 */ { "glVertexAttribs4hvNV", (GLExtFuncPtr)&nglContext::glVertexAttribs4hvNV },

/* GL_NV_pixel_data_range
 */
/* 1055 */ { "glPixelDataRangeNV", (GLExtFuncPtr)&nglContext::glPixelDataRangeNV },
/* 1056 */ { "glFlushPixelDataRangeNV", (GLExtFuncPtr)&nglContext::glFlushPixelDataRangeNV },

/* GL_NV_primitive_restart
 */
/* 1057 */ { "glPrimitiveRestartNV", (GLExtFuncPtr)&nglContext::glPrimitiveRestartNV },
/* 1058 */ { "glPrimitiveRestartIndexNV", (GLExtFuncPtr)&nglContext::glPrimitiveRestartIndexNV },

/* GL_ATI_map_object_buffer
 */
/* 1059 */ { "glMapObjectBufferATI", (GLExtFuncPtr)&nglContext::glMapObjectBufferATI },
/* 1060 */ { "glUnmapObjectBufferATI", (GLExtFuncPtr)&nglContext::glUnmapObjectBufferATI },

/* GL_ATI_separate_stencil
 */
/* 1061 */ { "glStencilOpSeparateATI", (GLExtFuncPtr)&nglContext::glStencilOpSeparateATI },
/* 1062 */ { "glStencilFuncSeparateATI", (GLExtFuncPtr)&nglContext::glStencilFuncSeparateATI },

/* GL_ATI_vertex_attrib_array_object
 */
/* 1063 */ { "glVertexAttribArrayObjectATI", (GLExtFuncPtr)&nglContext::glVertexAttribArrayObjectATI },
/* 1064 */ { "glGetVertexAttribArrayObjectfvATI", (GLExtFuncPtr)&nglContext::glGetVertexAttribArrayObjectfvATI },
/* 1065 */ { "glGetVertexAttribArrayObjectivATI", (GLExtFuncPtr)&nglContext::glGetVertexAttribArrayObjectivATI },

/* GL_EXT_depth_bounds_test
 */
/* 1066 */ { "glDepthBoundsEXT", (GLExtFuncPtr)&nglContext::glDepthBoundsEXT },

/* GL_EXT_blend_equation_separate
 */
/* 1067 */ { "glBlendEquationSeparateEXT", (GLExtFuncPtr)&nglContext::glBlendEquationSeparateEXT },

/* GL_EXT_framebuffer_object
 */
/* 1068 */ { "glIsRenderbufferEXT", (GLExtFuncPtr)&nglContext::glIsRenderbufferEXT },
/* 1069 */ { "glBindRenderbufferEXT", (GLExtFuncPtr)&nglContext::glBindRenderbufferEXT },
/* 1070 */ { "glDeleteRenderbuffersEXT", (GLExtFuncPtr)&nglContext::glDeleteRenderbuffersEXT },
/* 1071 */ { "glGenRenderbuffersEXT", (GLExtFuncPtr)&nglContext::glGenRenderbuffersEXT },
/* 1072 */ { "glRenderbufferStorageEXT", (GLExtFuncPtr)&nglContext::glRenderbufferStorageEXT },
/* 1073 */ { "glGetRenderbufferParameterivEXT", (GLExtFuncPtr)&nglContext::glGetRenderbufferParameterivEXT },
/* 1074 */ { "glIsFramebufferEXT", (GLExtFuncPtr)&nglContext::glIsFramebufferEXT },
/* 1075 */ { "glBindFramebufferEXT", (GLExtFuncPtr)&nglContext::glBindFramebufferEXT },
/* 1076 */ { "glDeleteFramebuffersEXT", (GLExtFuncPtr)&nglContext::glDeleteFramebuffersEXT },
/* 1077 */ { "glGenFramebuffersEXT", (GLExtFuncPtr)&nglContext::glGenFramebuffersEXT },
/* 1078 */ { "glCheckFramebufferStatusEXT", (GLExtFuncPtr)&nglContext::glCheckFramebufferStatusEXT },
/* 1079 */ { "glFramebufferTexture1DEXT", (GLExtFuncPtr)&nglContext::glFramebufferTexture1DEXT },
/* 1080 */ { "glFramebufferTexture2DEXT", (GLExtFuncPtr)&nglContext::glFramebufferTexture2DEXT },
/* 1081 */ { "glFramebufferTexture3DEXT", (GLExtFuncPtr)&nglContext::glFramebufferTexture3DEXT },
/* 1082 */ { "glFramebufferRenderbufferEXT", (GLExtFuncPtr)&nglContext::glFramebufferRenderbufferEXT },
/* 1083 */ { "glGetFramebufferAttachmentParameterivEXT", (GLExtFuncPtr)&nglContext::glGetFramebufferAttachmentParameterivEXT },
/* 1084 */ { "glGenerateMipmapEXT", (GLExtFuncPtr)&nglContext::glGenerateMipmapEXT },

/* GL_GREMEDY_string_marker
 */
/* 1085 */ { "glStringMarkerGREMEDY", (GLExtFuncPtr)&nglContext::glStringMarkerGREMEDY },

  // End of list
  { NULL, (GLExtFuncPtr)NULL }
};


/* 116 extensions, 1086 total functions */
