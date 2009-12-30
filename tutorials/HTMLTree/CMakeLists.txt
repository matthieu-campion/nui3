project(nui3)

include_directories(resources)

add_executable (htmltree src/Application.cpp src/MainWindow.cpp src/HTMLTree.cpp)

target_link_libraries(htmltree expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
