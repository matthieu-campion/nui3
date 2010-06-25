project(nui3)

include_directories(src)

add_executable(viewer 
ViewerApp.cpp
ViewerWin.cpp
)

target_link_libraries(viewer expat jpeg png freetype nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
