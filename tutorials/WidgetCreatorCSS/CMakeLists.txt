project(nui3)

include_directories(resources)

add_executable (widgetcreatorcss src/Application.cpp src/MainWindow.cpp)

target_link_libraries(widgetcreatorcss expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
