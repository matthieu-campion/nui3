project(nui3)

include_directories(resources)

add_executable (layoutanimation src/Application.cpp src/MainWindow.cpp)

target_link_libraries(layoutanimation nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
