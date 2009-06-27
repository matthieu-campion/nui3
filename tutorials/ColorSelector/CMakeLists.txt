project(nui3)

include_directories(resources)

add_executable (colorselector src/Application.cpp src/MainWindow.cpp)

target_link_libraries(colorselector expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
