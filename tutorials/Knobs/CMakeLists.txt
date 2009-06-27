project(nui3)

include_directories(resources)

add_executable (knobs src/Application.cpp src/MainWindow.cpp)

target_link_libraries(knobs expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
