project(nui3)

include_directories(resources)

add_executable (grids src/Application.cpp src/MainWindow.cpp)

target_link_libraries(grids expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
