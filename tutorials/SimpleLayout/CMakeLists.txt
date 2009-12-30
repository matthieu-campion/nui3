project(nui3)

include_directories(resources)

add_executable (simplelayout src/Application.cpp src/MainWindow.cpp)

target_link_libraries(simplelayout expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
