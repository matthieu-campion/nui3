project(nui3)

include_directories(resources)

add_executable (rss src/Application.cpp src/MainWindow.cpp)

target_link_libraries(rss expat jpeg png freetype ungif nui3 curl tidy ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
