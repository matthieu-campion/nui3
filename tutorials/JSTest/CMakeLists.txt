project(nui3)

include_directories(resources)

add_executable (jstest src/application.cpp src/mainwindow.cpp)

target_link_libraries(jstest expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
