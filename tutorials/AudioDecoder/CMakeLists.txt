project(nui3)

#include_directories(resources)

add_executable (audiodecoder src/Application.cpp)

target_link_libraries(audiodecoder mpg123 expat jpeg png freetype ungif nui3 ${OPENGL_LIBRARIES} GLU)
