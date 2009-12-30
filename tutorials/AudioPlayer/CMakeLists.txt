project(nui3)

include_directories(resources)

add_executable (buttons src/Application.cpp src/MainWindow.cpp)

target_link_libraries(buttons expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
