project(nui3)

include_directories(resources)

add_executable (simpletouch src/Application.cpp)

target_link_libraries(simpletouch expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
