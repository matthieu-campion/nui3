project(nui3)

include_directories(resources)

add_executable (flowlayout src/Application.cpp src/MainWindow.cpp)

target_link_libraries(flowlayout expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
