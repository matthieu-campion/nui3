project(testGLUTintegration)

INCLUDE(FindOpenGL)
INCLUDE(FindGLUT)

add_definitions( 
  -DFT_CONFIG_OPTION_SYSTEM_ZLIB
  -DNGL_STATIC_BUILD
  -D__LINUX__
  -D_LIB
)

add_executable(context
main.cpp
MainWindow.cpp
)

target_link_libraries(context expat jpeg png freetype nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})