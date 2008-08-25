project(nui3)

include_directories(tests)

add_executable (nuitest Gears.cpp nuiApp.cpp nuiGearWin.cpp nuiWin.cpp tests/ThreadInspectorTest)

target_link_libraries(nuitest gameswf expat jpeg png freetype nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
