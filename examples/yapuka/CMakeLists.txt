project(nui3)

include_directories(src)

add_executable(yapuka 
src/DecorationManager.cpp
src/ElementDesc.cpp
src/ElementEditor.cpp
src/ElementInspector.cpp
src/FrameEditor.cpp
src/MainWindow.cpp
src/Preferences.cpp
src/ProjectCreator.cpp
src/ToolPaneAttributes.cpp
src/ToolPane.cpp
src/WidgetDelegates.cpp
src/WidgetEditor.cpp
src/Yapuka.cpp

)

target_link_libraries(yapuka expat jpeg png freetype nui3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
