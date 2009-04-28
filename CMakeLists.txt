cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project( nui3 )

set(SOURCES 

  graphics/DefaultDecoration.cpp

  src/Base/nui.cpp
  src/Base/nuiAnimation.cpp
  src/Base/nuiBindingManager.cpp
  src/Base/nuiBuilder.cpp
  src/Base/nuiColor.cpp
  src/Base/nuiCommand.cpp
  src/Base/nuiCSS.cpp
  src/Base/nuiDragAndDrop.cpp
  src/Base/nuiEvent.cpp
  src/Base/nuiFlags.cpp
  src/Base/nuiFont.cpp
  src/Base/nuiFontBase.cpp
  src/Base/nuiFontManager.cpp
  src/Base/nuiGladeLoader.cpp
  src/Base/nuiHotKey.cpp
  src/Base/nuiInit.cpp
  src/Base/nuiMessageQueue.cpp
  src/Base/nuiNativeResource.cpp
  src/Base/nuiNotification.cpp
  src/Base/nuiObject.cpp
  src/Base/nuiProperty.cpp
  src/Base/nuiRange.cpp
  src/Base/nuiRect.cpp
  src/Base/nuiSignalsSlots.cpp
  src/Base/nuiTheme.cpp
  src/Base/nuiTimer.cpp
  src/Base/nuiTree.cpp
  src/Base/nuiXML.cpp
  src/Base/nuiHTML.cpp
  src/Bindings/nuiBindings.cpp
  src/Animations/nuiAttributeAnimation.cpp
  src/Animations/nuiWidgetAnimation.cpp
  src/Text/nuiHyperLink.cpp
  src/Text/nuiEditLine.cpp
  src/Text/nuiEditText.cpp
  src/Text/nuiFormatedLabel.cpp
  src/Text/nuiFormater.cpp
  src/Text/nuiLabel.cpp
  src/Text/nuiLabelAttribute.cpp
  src/Text/nuiSpinnerLabel.cpp
  src/Text/nuiText.cpp
  src/Renderers/nuiSurface.cpp
  src/Renderers/AAPrimitives.cpp
  src/Renderers/nuiArc.cpp
  src/Renderers/nuiContour.cpp
  src/Renderers/nuiD3DPainter.cpp
  src/Renderers/nuiDrawContext.cpp
  src/Renderers/nuiGLPainter.cpp
  src/Renderers/nuiGradient.cpp
  src/Renderers/nuiMetaPainter.cpp
  src/Renderers/nuiOutliner.cpp
  src/Renderers/nuiPainter.cpp
  src/Renderers/nuiPath.cpp
  src/Renderers/nuiPathGenerator.cpp
  src/Renderers/nuiPathOptimizer.cpp
  src/Renderers/nuiPoint.cpp
  src/Renderers/nuiPolyLine.cpp
  src/Renderers/nuiRectPath.cpp
  src/Renderers/nuiRenderArray.cpp
  src/Renderers/nuiRenderState.cpp
  src/Renderers/nuiShader.cpp
  src/Renderers/nuiShape.cpp
  src/Renderers/nuiSoftwarePainter.cpp
  src/Renderers/nuiSpline.cpp
  src/Renderers/nuiSVGShape.cpp
  src/Renderers/nuiTessellator.cpp
  src/Renderers/nuiTexture.cpp
  src/Renderers/nuiTextureHelpers.cpp
  src/WidgetTree/nuiContainer.cpp
  src/WidgetTree/nuiMainWindow.cpp
  src/WidgetTree/nuiSimpleContainer.cpp
  src/WidgetTree/nuiTopLevel.cpp
  src/WidgetTree/nuiWidget.cpp
  src/WidgetTree/nuiWindowManager.cpp
  src/SimpleWidgets/nuiButton.cpp
  src/SimpleWidgets/nuiColorSelector.cpp
  src/SimpleWidgets/nuiCoverFlow.cpp
  src/SimpleWidgets/nuiComboBox.cpp
  src/SimpleWidgets/nuiFontBrowser.cpp
  src/SimpleWidgets/nuiFileList.cpp
  src/SimpleWidgets/nuiFileSelector.cpp
  src/SimpleWidgets/nuiFileTable.cpp
  src/SimpleWidgets/nuiFrameView.cpp
  src/SimpleWidgets/nuiHoverDummy.cpp
  src/SimpleWidgets/nuiImage.cpp
  src/SimpleWidgets/nuiImageDropZone.cpp
  src/SimpleWidgets/nuiKnob.cpp
  src/SimpleWidgets/nuiMesh.cpp
  src/SimpleWidgets/nuiMeshEngine.cpp
  src/SimpleWidgets/nuiOffscreenView.cpp
  src/SimpleWidgets/nuiRadioButton.cpp
  src/SimpleWidgets/nuiProgressBar.cpp
  src/SimpleWidgets/nuiRadioButtonGroup.cpp
  src/SimpleWidgets/nuiRectView.cpp
  src/SimpleWidgets/nuiScrollBar.cpp
  src/SimpleWidgets/nuiSeparator.cpp
  src/SimpleWidgets/nuiShaderView.cpp
  src/SimpleWidgets/nuiShapeView.cpp
  src/SimpleWidgets/nuiSlider.cpp
  src/SimpleWidgets/nuiSplineEdit.cpp
  src/SimpleWidgets/nuiStateDummy.cpp
  src/SimpleWidgets/nuiSVGView.cpp
  src/SimpleWidgets/nuiSWF.cpp
  src/SimpleWidgets/nuiTabBar.cpp
  src/SimpleWidgets/nuiToggleButton.cpp
  src/SimpleWidgets/nuiZoomBar.cpp
  src/TreeViews/nuiPopupMenu.cpp
  src/TreeViews/nuiColumnTreeView.cpp
  src/TreeViews/nuiTreeView.cpp
  src/Layout/nuiImageSequence.cpp
  src/Layout/nuiBackgroundPane.cpp
  src/Layout/nuiBox.cpp
  src/Layout/nuiAnimView.cpp
  src/Layout/nuiDialog.cpp
  src/Layout/nuiFixed.cpp
  src/Layout/nuiFolderPane.cpp
  src/Layout/nuiGrid.cpp
  src/Layout/nuiHBox.cpp
  src/Layout/nuiList.cpp
  src/Layout/nuiMatrixView.cpp
  src/Layout/nuiModalContainer.cpp
  src/Dialogs/nuiMessageBox.cpp
  src/Dialogs/nuiDialogSelectFile.cpp
  src/Dialogs/nuiDialogSelectDirectory.cpp
  src/Layout/nuiPane.cpp
  src/Layout/nuiPopupView.cpp
  src/Layout/nuiPositioner.cpp
  src/Layout/nuiScrollView.cpp
  src/Layout/nuiSplitter.cpp
  src/Layout/nuiTable.cpp
  src/Layout/nuiTabView.cpp
  src/Layout/nuiTitledPane.cpp
  src/Layout/nuiUserArea.cpp
  src/Layout/nuiVBox.cpp
  src/Layout/nuiWindow.cpp
  src/Layout/nuiZoomView.cpp
  src/Audio/nuiAudioConvert.cpp
  src/Audio/nuiAudioDevice.cpp
  src/Audio/nuiAudioFifo.cpp
  src/Attributes/nuiAttribute.cpp
  src/Attributes/nuiRangeAttributeEditor.cpp
  src/Attributes/nuiRangeKnobAttributeEditor.cpp
  src/Attributes/nuiSizeAttributeEditor
  src/Attributes/nuiAttributeEditor.cpp
  src/Attributes/nuiBooleanAttributeEditor.cpp
  src/Attributes/nuiBorderAttributeEditor.cpp
  src/Attributes/nuiColorAttributeEditor.cpp
  src/Attributes/nuiGenericAttributeEditor.cpp
  src/Attributes/nuiPathAttributeEditor.cpp
  src/Attributes/nuiPercentAttributeEditor.cpp
  src/Attributes/nuiPointAttributeEditor.cpp
  src/Attributes/nuiRectAttributeEditor.cpp
  src/Attributes/nuiStringAttributeEditor.cpp
  src/Decorations/nuiDefaultDecoration.cpp
  src/Decorations/nuiImageDecoration.cpp
  src/Decorations/nuiDecoration.cpp
  src/Decorations/nuiCheckerboardDecoration.cpp
  src/Decorations/nuiBorderDecoration.cpp
  src/Decorations/nuiTreeHandleDecoration.cpp
  src/Decorations/nuiFrame.cpp
  src/Decorations/nuiGradientDecoration.cpp
  src/Decorations/nuiMetaDecoration.cpp
  src/Decorations/nuiStateDecoration.cpp
  src/Decorations/nuiColorDecoration.cpp
  src/Utils/nuiCSV.cpp
  src/String/nglString.cpp
  src/String/nglStringConv_iconv.cpp
  src/String/nglUTFStringConv.cpp
  src/String/ConvertUTF.cpp
  src/String/nuiTranslator.cpp
  src/Math/nglMath.cpp
  src/Stream/nglIFile.cpp
  src/Stream/nglIMemory.cpp
  src/Stream/nglIOFile.cpp
  src/Stream/nglIStream.cpp
  src/Stream/nglIZip.cpp
  src/Stream/nglOFile.cpp
  src/Stream/nglOMemory.cpp
  src/Stream/nglOStream.cpp
  src/Stream/nglStream.cpp
  src/Stream/nglZipFS.cpp
  src/Stream/Zip/ioapi.c
  src/Stream/Zip/unzip.c
  src/Stream/Zip/zip.c
  src/Image/nglBitmapTools.cpp
  src/Image/nglImage.cpp
  src/Image/nglImageCodec.cpp
  src/Image/nglImageJPEGCodec.cpp
  src/Image/nglImagePNGCodec.cpp
  src/Image/nglImagePPMCodec.cpp
  src/Image/nglImageTGACodec.cpp
  src/File/nglFile.cpp
  src/File/nglNativeVolume.cpp
  src/File/nglPath.cpp
  src/File/nglVolume.cpp
  src/File/nuiNativeResourceVolume.cpp
  src/Font/nuiPanose.cpp
  src/Font/ngl_default_font.cpp
  src/Font/nglFont.cpp
  src/Font/nglFontBase.cpp
  src/Font/nglFontCache.cpp
  src/Font/nglFontInstance.cpp
  src/Font/nglFontLayout.cpp
  src/Font/nglFontTexCache.cpp
  src/Time/nglTime.cpp
  src/Time/nglTimer.cpp
  src/Application/nglApplication.cpp
  src/Application/nuiLocale.cpp
  src/Application/nglClipBoard.cpp
  src/Application/nglConsole.cpp
  src/Application/nglCPUInfo.cpp
  src/Application/nglCPUInfo_x86.cpp
  src/Application/nglDataObjects.cpp
  src/Application/nglDataTypesRegistry.cpp
  src/Application/nglDragAndDropObjects.cpp
  src/Application/nglKernel.cpp
  src/Application/nglLog.cpp
  src/Application/nglMimeSource.cpp
  src/Application/nglModule.cpp
  src/Window/glext/ngl_glext_table.cpp
  src/Window/nglContext.cpp
  src/Window/nglDeviceInfo.cpp
  src/Window/nglInputDevice.cpp
  src/Window/nglInputDeviceInstance.cpp
  src/Window/nglKeyboard.cpp
  src/Window/nglVideoMode.cpp
  src/Window/nglWindow.cpp
  src/Utils/ngl3DSLoader.cpp
  src/Utils/nglError.cpp
  src/Threading/nglThread.cpp
  src/Threading/nglLock.cpp
  src/Threading/nglLightLock.cpp
  src/Threading/nglThreadChecker.cpp
  src/Net/nuiHTTP.cpp
  src/Net/Unix/nuiURL_Unix.cpp
  src/Introspector/nuiFontInspector.cpp
  src/Introspector/nuiDecorationInspector.cpp
  src/Introspector/nuiIntrospector.cpp
  src/Introspector/nuiThreadInspector.cpp
  src/Introspector/nuiWidgetInspector.cpp
)

include_directories(
  deps/freetype2/include
  deps/agg/include
  deps/gameswf
  deps/harfbuzz
  deps/tidy
  include
  src/Stream/Zip
  .
)

add_definitions( 
  -DFT_CONFIG_OPTION_SYSTEM_ZLIB
  -DNGL_STATIC_BUILD
  -D_DEBUG
  -D_LIB
)

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(LINUX 1)
#MESSAGE (${LINUX})
ELSEIF (${CMAKE_SYSTEM} MATCHES "Windows")
SET(WINDOWS 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")
#set(WINDOWS ${CMAKE_SYSTEM} MATCHES "Windows")

IF (${LINUX})
MESSAGE("Linux Build")
add_definitions( -D__LINUX__ )
include_directories( 
src/Application/Unix
src/Window/Unix
)
set(SOURCES 
  src/Time/Unix/nglTimer_Unix.cpp
  src/Application/Unix/nglApplication_Unix.cpp
  src/Application/Unix/nglClipBoard_Unix.cpp
  src/Application/Unix/nglConsole_Unix.cpp
  src/Application/Unix/nglDataObjects_Unix.cpp
  src/Application/Unix/nglKernel_Unix.cpp
  src/Application/Unix/nglModule_Unix.cpp
  src/Window/Unix/nglContext_Unix.cpp
  src/Window/Unix/nglEvent_Unix.cpp
  src/Window/Unix/nglInputDevice_Unix.cpp
  src/Window/Unix/nglInputDevice_Linux.cpp
  src/Window/Unix/nglVideoMode_Unix.cpp
  src/Window/Unix/nglWindow_Unix.cpp
  src/Threading/posix/nglCriticalSection_posix.cpp
  src/Threading/posix/nglSyncEvent_posix.cpp
  src/Threading/posix/nglThread_posix.cpp
  
  src/Net/Unix/nuiHTTP_Unix.cpp
  
 # src/File/Unix/nglFile_Unix.cpp
  src/File/Unix/nglPath_Unix.cpp
  
  ${SOURCES}
)
#should be inside of and IF -> debug
add_definitions(
  -O0
  -g
)
INSTALL(DIRECTORY examples/data DESTINATION /usr/share/nuitest)

ELSEIF (${WINDOWS})
add_definitions( -DWIN32 -DNGL_USE_WIN32_MMTIMER)

include_directories( 
deps/GL 
src/Application/Win
src/Window/Win
deps/expat/lib
deps/freetype2/include
deps/libjpeg
deps/libpng
deps/zlib/
)

set(${SOURCES} ${SOURCES} 
  src/Time/Unix/nglTimer_WinXX.cpp
  
  src/Application/WinX/ngl.cpp
  src/Application/WinX/nglDataObjects_WinXX.cpp
  src/Application/WinX/nglExceptionTrace_WinXX.cpp
  src/Application/WinX/nglLog_WinXX.cpp
  src/Application/WinX/nglMimeSource_WinXX.cpp
  src/Application/WinX/nglPlugin_WinXX.cpp
  src/Application/Win/nglApplication_WinXX.cpp
  src/Application/Win/nglClipBoard_WinXX.cpp
  src/Application/Win/nglConsole_WinXX.cpp
  src/Application/Win/nglKernel_WinXX.cpp
  src/Application/WinX/nglModule_WinXX.cpp
  
  src/Window/Win/nglContext_WinXX.cpp
  src/Window/Win/nglEvent_WinXX.cpp
  src/Window/Win/nglInputDevice_WinXX.cpp
  src/Window/Win/nglInputDevice_DirectX.cpp
  src/Window/Win/nglVideoMode_WinXX.cpp
  src/Window/Win/nglWindow_WinXX.cpp
  
  src/Threading/win32/nglCriticalSection_win32.cpp
  src/Threading/win32/nglSyncEvent_win32.cpp
  src/Threading/win32/nglThread_win32.cpp
  
  src/Net/Win/nuiHTTP_WinXX.cpp
)
ENDIF (${LINUX})


add_subdirectory(deps)

add_library( nui3 STATIC ${SOURCES} )

target_link_libraries(nui3 gameswf tidy ${EXPAT_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES})

add_subdirectory(examples)
add_subdirectory(tutorials)

