project(nui3)

include_directories(resources)

add_executable (htmlview src/Application.cpp src/MainWindow.cpp)

target_link_libraries(htmlview nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
