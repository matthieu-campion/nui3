project(nui3)

include_directories(resources)

add_executable (fileselector src/Application.cpp src/MainWindow.cpp)

target_link_libraries(fileselector expat jpeg png freetype nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
