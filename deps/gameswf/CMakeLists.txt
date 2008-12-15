FIND_PACKAGE(SDL REQUIRED)

include_directories (. ../ platforms )

add_library(gameswf STATIC

gameswf_abc.cpp        
gameswf_character.cpp    
#gameswf_fontlib_unused.cpp  
gameswf_listener.cpp   
gameswf_player.cpp               
gameswf_root.cpp               
gameswf_stream.cpp     
gameswf_types.cpp
gameswf_action.cpp     
gameswf_disasm.cpp       
gameswf_freetype.cpp        
gameswf_log.cpp        
gameswf_processor.cpp            
gameswf_shape.cpp              
gameswf_styles.cpp     
gameswf_value.cpp
gameswf_as_sprite.cpp  
gameswf_dlist.cpp        
gameswf_function.cpp        
gameswf_morph2.cpp     
gameswf_render.cpp               
gameswf_sound.cpp              
gameswf_tesselate.cpp  
gameswf_video_base.cpp
gameswf_avm2.cpp       
gameswf_environment.cpp  
gameswf_impl.cpp            
gameswf_movie_def.cpp  
# gameswf_render_handler_d3d.cpp   
#gameswf_sound_handler_sdl.cpp  
#gameswf_test_ogl.cpp   
gameswf_video_impl.cpp
gameswf_button.cpp     
gameswf_filters.cpp      
gameswf_jit.cpp             
gameswf_object.cpp     
gameswf_render_handler_ogl.cpp   
gameswf_sprite.cpp             
gameswf_text.cpp       
gameswf_video_ogl.cpp
gameswf_canvas.cpp     
gameswf_font.cpp         
gameswf_jit_opcode.cpp      
gameswf_parser.cpp     
gameswf_render_handler_xbox.cpp  
gameswf_sprite_def.cpp         
gameswf_tools.cpp

base/component_hackery.cpp  
base/ear_clip_triangulate_float.cpp   
base/image_filters.cpp  
base/ogl.cpp          
#base/test_ogl.cpp            
# base/tu_gc_singlethreaded_marksweep.cpp  
base/tu_timer.cpp  
base/zlib_adapter.cpp
base/configvars.cpp         
base/ear_clip_triangulate_sint16.cpp  
base/jpeg.cpp           
base/png_helper.cpp   
base/triangulate_float.cpp   
#base/tu_gc_test.cpp                      
base/tu_types.cpp
base/container.cpp          
base/file_util.cpp                    
base/logger.cpp         
base/postscript.cpp   
base/triangulate_sint32.cpp  
base/tu_loadlib.cpp                      
base/utf8.cpp
#base/demo.cpp               
base/image.cpp                        
base/membuf.cpp        
# base/Stackwalker.cpp  
base/tu_file.cpp             
base/tu_random.cpp                       
base/utility.cpp

net/http_client.cpp  
net/http_file_handler.cpp  
net/http_helper.cpp  
net/http_server.cpp  
net/net_interface_tcp.cpp  
#net/net_test.cpp  
net/tu_net_file.cpp  
net/webtweaker.cpp  
net/webwidgets.cpp

gameswf_as_classes/as_array.cpp            
gameswf_as_classes/as_date.cpp    
gameswf_as_classes/as_key.cpp       
gameswf_as_classes/as_mouse_event.cpp    
gameswf_as_classes/as_selection.cpp   
gameswf_as_classes/as_transform.cpp
gameswf_as_classes/as_boolean.cpp          
gameswf_as_classes/as_event.cpp   
gameswf_as_classes/as_loadvars.cpp  
gameswf_as_classes/as_netconnection.cpp  
gameswf_as_classes/as_sound.cpp       
gameswf_as_classes/as_xml.cpp
gameswf_as_classes/as_broadcaster.cpp      
gameswf_as_classes/as_flash.cpp   
gameswf_as_classes/as_math.cpp      
gameswf_as_classes/as_netstream.cpp      
gameswf_as_classes/as_string.cpp      
gameswf_as_classes/as_xmlsocket.cpp
gameswf_as_classes/as_color.cpp            
gameswf_as_classes/as_geom.cpp    
gameswf_as_classes/as_matrix.cpp    
gameswf_as_classes/as_number.cpp         
gameswf_as_classes/as_textformat.cpp
gameswf_as_classes/as_color_transform.cpp  
gameswf_as_classes/as_global.cpp  
gameswf_as_classes/as_mcloader.cpp  
gameswf_as_classes/as_point.cpp          
gameswf_as_classes/as_timer.cpp

)










































