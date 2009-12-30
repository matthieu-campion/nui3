project(nui3)

include_directories(resources)

add_executable (videodecoder src/Application.cpp src/MainWindow.cpp)

target_link_libraries(videodecoder expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
