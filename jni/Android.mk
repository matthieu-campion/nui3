# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libnui3

LOCAL_ARM_MODE := arm

MY_INCLUDE_FILES :=   \
                      $(LOCAL_PATH)/.. \
                      $(LOCAL_PATH)/../include \
                      $(LOCAL_PATH)/../src/Stream/Zip \
                      $(LOCAL_PATH)/../src/Text/HTML \
                      $(LOCAL_PATH)/../src/Application/Unix \
                      $(LOCAL_PATH)/../deps \

MY_C_FLAGS := -x c++ -fmessage-length=0 -pipe -Wno-trigraphs -O0 -D_ANDROID_ -DNGL_STATIC_BUILD -DHAVE_BCOPY -DFT2_BUILD_LIBRARY -DDARWIN_NO_CARBON -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DHAVE_BCOPY -DFEATURE_NANOJIT -DJS_TRACER -DXP_UNIX -DAVMPLUS_IA32 -D_DEBUG -DDEBUG -fexceptions -fvisibility=hidden -fno-threadsafe-statics -Wno-deprecated-declarations -frtti -DGL_GLEXT_PROTOTYPES=1

MY_LDLIBS := -llog -lGLESv1_CM -lz -lGLESv2


LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

LOCAL_CFLAGS    :=  $(MY_C_FLAGS)
LOCAL_EXPORT_CFLAGS    :=  $(MY_C_FLAGS)

LOCAL_LDLIBS := $(MY_LDLIBS)
LOCAL_EXPORT_LDLIBS := $(MY_LDLIBS)

LOCAL_STATIC_LIBRARIES := android_native_app_glue freetype expat tess harfbuzz libcss libpng libjpeg tidy ucdata ungif mpg123

NUI_LOCAL_SRC_FILES_ANIMATIONS := ../src/Base/nuiAnimation.cpp \
                                  ../src/Animations/nuiAttributeAnimation.cpp \
                                  ../src/Animations/nuiWidgetAnimation.cpp


NUI_LOCAL_SRC_FILES_APPLICATION := ../src/Application/nglApplication.cpp \
                                   ../src/Application/Android/nglApplication_Android.cpp \
                                   ../src/Application/Android/nglClipBoard_Android.cpp \
                                   ../src/Application/Android/nglConsole_Android.cpp \
                                   ../src/Application/Android/nglDataObjects_Android.cpp \
                                   ../src/Application/Android/nglKernel_Android.cpp \
                                   ../src/Application/Android/nglModule_Android.cpp \
                                   ../src/Application/nglClipBoard.cpp \
                                   ../src/Application/nglConsole.cpp \
                                   ../src/Application/nglCPUInfo.cpp \
                                   ../src/Application/nglDataObjects.cpp \
                                   ../src/Application/nglDataTypesRegistry.cpp \
                                   ../src/Application/nglDragAndDropObjects.cpp \
                                   ../src/Application/nglKernel.cpp \
                                   ../src/Application/nglLog.cpp \
                                   ../src/Application/nglMimeSource.cpp \
                                   ../src/Application/nglModule.cpp \
                                   ../src/Application/nuiLocale.cpp \



NUI_LOCAL_SRC_FILES_ATTRIBUTES := ../src/Attributes/nuiAttribute.cpp \
                                  ../src/Attributes/nuiAttributeEditor.cpp \
                                  ../src/Attributes/nuiBooleanAttributeEditor.cpp \
                                  ../src/Attributes/nuiBorderAttributeEditor.cpp \
                                  ../src/Attributes/nuiColorAttributeEditor.cpp \
                                  ../src/Attributes/nuiGenericAttributeEditor.cpp \
                                  ../src/Attributes/nuiPathAttributeEditor.cpp \
                                  ../src/Attributes/nuiPercentAttributeEditor.cpp \
                                  ../src/Attributes/nuiPointAttributeEditor.cpp \
                                  ../src/Attributes/nuiRangeAttributeEditor.cpp \
                                  ../src/Attributes/nuiRangeKnobAttributeEditor.cpp \
                                  ../src/Attributes/nuiRectAttributeEditor.cpp \
                                  ../src/Attributes/nuiSizeAttributeEditor.cpp \
                                  ../src/Attributes/nuiStringAttributeEditor.cpp \
                                  ../src/Attributes/nuiPopupValueAttributeEditor.cpp \

NUI_LOCAL_SRC_FILES_AUDIO := ../src/Audio/nuiAudioConvert.cpp \
                             ../src/Audio/nuiAudioFifo.cpp \
                             ../src/Audio/nuiAudioDevice.cpp \
                             ../src/Audio/Android/nuiAudioDevice_Android.cpp \

NUI_LOCAL_SRC_FILES_AUDIO_ENGINE := ../src/AudioEngine/nuiAudioDb.cpp \
                                    ../src/AudioEngine/nuiAudioEngine.cpp \
                                    ../src/AudioEngine/nuiSound.cpp \
                                    ../src/AudioEngine/nuiSoundManager.cpp \
                                    ../src/AudioEngine/nuiVoice.cpp \
                                    ../src/AudioEngine/nuiMemorySound.cpp \
                                    ../src/AudioEngine/nuiMemoryVoice.cpp \
                                    ../src/AudioEngine/nuiFileSound.cpp \
                                    ../src/AudioEngine/nuiFileVoice.cpp \
                                    ../src/AudioEngine/nuiSynthSound.cpp \
                                    ../src/AudioEngine/nuiSynthVoice.cpp \


NUI_LOCAL_SRC_FILES_AUDIO_SAMPLES := ../src/AudioSamples/nuiAiffReader.cpp \
                                     ../src/AudioSamples/nuiAiffWriter.cpp \
                                     ../src/AudioSamples/nuiAudioDecoder.cpp \
                                     ../src/AudioSamples/Unix/nuiAudioDecoder_Unix.cpp \
                                     ../src/AudioSamples/nuiChunkSampleReader.cpp \
                                     ../src/AudioSamples/nuiSample.cpp \
                                     ../src/AudioSamples/nuiSampleInfo.cpp \
                                     ../src/AudioSamples/nuiSampleReader.cpp \
                                     ../src/AudioSamples/nuiSampleWriter.cpp \
                                     ../src/AudioSamples/nuiWaveReader.cpp \
                                     ../src/AudioSamples/nuiWaveWriter.cpp \


NUI_LOCAL_SRC_FILES_BASE := ../src/Base/nuiBindingManager.cpp \
                            ../src/Base/nuiBuilder.cpp \
                            ../src/Base/nuiColor.cpp \
                            ../src/Base/nuiCommand.cpp \
                            ../src/Base/nuiCommandManager.cpp \
                            ../src/Base/nuiCSS.cpp \
                            ../src/Base/nuiEvent.cpp \
                            ../src/Base/nuiFlags.cpp \
                            ../src/Base/nuiGladeLoader.cpp \
                            ../src/Base/nuiHotKey.cpp \
                            ../src/Base/nuiInit.cpp \
                            ../src/Base/nuiMemoryPool.cpp \
                            ../src/Base/nuiMessageQueue.cpp \
                            ../src/Base/nuiNativeResource.cpp \
                            ../src/Base/nuiNotification.cpp \
                            ../src/Base/nuiObject.cpp \
                            ../src/Base/nuiRange.cpp \
                            ../src/Base/nuiRect.cpp \
                            ../src/Base/nuiSerializeContext.cpp \
                            ../src/Base/nuiSignalsSlots.cpp \
                            ../src/Base/nuiTheme.cpp \
                            ../src/Base/nuiTimer.cpp \
                            ../src/Base/nuiToken.cpp \
                            ../src/Base/nuiTree.cpp \
                            ../src/Base/nuiXML.cpp \
                            ../src/Base/nuiApplication.cpp \
                            ../src/Base/nuiTask.cpp \
                            ../src/Base/nuiHTML.cpp \


NUI_LOCAL_SRC_FILES_BINDINGS := ../src/Bindings/nuiBindings.cpp \
                                ../src/Bindings/nuiScriptEngine.cpp \


NUI_LOCAL_SRC_FILES_DECORATIONS := ../src/Decorations/nuiBorderDecoration.cpp \
                                   ../src/Decorations/nuiNavigationViewDecoration.cpp \
                                   ../src/Decorations/nuiColorDecoration.cpp \
                                   ../src/Decorations/nuiDecoration.cpp \
                                   ../src/Decorations/nuiDefaultDecoration.cpp \
                                   ../src/Decorations/nuiFrame.cpp \
                                   ../src/Decorations/nuiGradientDecoration.cpp \
                                   ../src/Decorations/nuiImageDecoration.cpp \
                                   ../src/Decorations/nuiMetaDecoration.cpp \
                                   ../src/Decorations/nuiStateDecoration.cpp \
                                   ../src/Decorations/nuiCheckerboardDecoration.cpp \
                                   ../src/Decorations/nuiTreeHandleDecoration.cpp \


NUI_LOCAL_SRC_FILES_DIALOGS := ../src/Dialogs/nuiMessageBox.cpp \
                               ../src/Dialogs/nuiDialogSelectDirectory.cpp \
                               ../src/Dialogs/nuiDialogSelectFile.cpp \

NUI_LOCAL_SRC_FILES_FILE := ../src/File/nglFile.cpp \
                            ../src/File/nglNativeVolume.cpp \
                            ../src/File/nglPath.cpp \
                            ../src/File/Unix/nglPath_Unix.cpp \
                            ../src/File/nglVolume.cpp \
                            ../src/File/nuiNativeResourceVolume.cpp \


NUI_LOCAL_SRC_FILES_FONT := ../src/Font/nuiFont.cpp \
                            ../src/Font/nuiFontBase.cpp \
                            ../src/Font/nuiFontManager.cpp \
                            ../src/Font/hb_nui.cpp \
                            ../src/Font/nuiTextLayout.cpp \
                            ../src/Font/nuiTextStyle.cpp \
                            ../src/Font/nuiTextRun.cpp \
                            ../src/Font/nuiTextLine.cpp \
                            ../src/Font/nuiFontInstance.cpp \
                            ../src/Font/nuiPanose.cpp \
                            ../src/Font/ngl_default_font.cpp \

NUI_LOCAL_SRC_FILES_IMAGE := ../src/Image/nglBitmapTools.cpp \
                             ../src/Image/nglImage.cpp \
                             ../src/Image/nglImageCodec.cpp \
                             ../src/Image/nglImageGIFCodec.cpp \
                             ../src/Image/nglImageJPEGCodec.cpp \
                             ../src/Image/nglImagePNGCodec.cpp \
                             ../src/Image/nglImagePPMCodec.cpp \
                             ../src/Image/nglImageTGACodec.cpp \


NUI_LOCAL_SRC_FILES_INTROSPECTOR := ../src/Introspector/nuiIntrospector.cpp \
                                    ../src/Introspector/nuiWidgetInspector.cpp \
                                    ../src/Introspector/nuiThreadInspector.cpp \
                                    ../src/Introspector/nuiFontInspector.cpp \
                                    ../src/Introspector/nuiDecorationInspector.cpp \
                                    ../src/Introspector/nuiTextureInspector.cpp \
                                    ../src/Introspector/nuiObjectInspector.cpp \


NUI_LOCAL_SRC_FILES_JSON := ../src/Json/json_reader.cpp \
                            ../src/Json/json_value.cpp \
                            ../src/Json/json_writer.cpp \


NUI_LOCAL_SRC_FILES_LAYOUT := ../src/Layout/nuiAnimView.cpp \
                              ../src/Layout/nuiBackgroundPane.cpp \
                              ../src/Layout/nuiBox.cpp \
                              ../src/Layout/nuiDialog.cpp \
                              ../src/Layout/nuiFixed.cpp \
                              ../src/Layout/nuiFlowView.cpp \
                              ../src/Layout/nuiFolderPane.cpp \
                              ../src/Layout/nuiGrid.cpp \
                              ../src/Layout/nuiHBox.cpp \
                              ../src/Layout/nuiImageSequence.cpp \
                              ../src/Layout/nuiList.cpp \
                              ../src/Layout/nuiMatrixView.cpp \
                              ../src/Layout/nuiModalContainer.cpp \
                              ../src/Layout/nuiPane.cpp \
                              ../src/Layout/nuiPopupView.cpp \
                              ../src/Layout/nuiScrollBox.cpp \
                              ../src/Layout/nuiScrollView.cpp \
                              ../src/Layout/nuiSplitter.cpp \
                              ../src/Layout/nuiTabView.cpp \
                              ../src/Layout/nuiTitledPane.cpp \
                              ../src/Layout/nuiUserArea.cpp \
                              ../src/Layout/nuiVBox.cpp \
                              ../src/Layout/nuiWindow.cpp \
                              ../src/Layout/nuiZoomView.cpp \


NUI_LOCAL_SRC_FILES_MATH := ../src/Math/nglMath.cpp \


NUI_LOCAL_SRC_FILES_NAVIGATIONS_VIEWS := ../src/NavigationViews/nuiViewController.cpp \
                                         ../src/NavigationViews/nuiNavigationController.cpp \
                                         ../src/NavigationViews/nuiNavigationBar.cpp \
                                         ../src/NavigationViews/nuiNavigationButton.cpp \
                                         ../src/NavigationViews/nuiImageAnimation.cpp \
                                         ../src/NavigationViews/nuiGestureRecognizer.cpp \


NUI_LOCAL_SRC_FILES_NET := ../src/Net/nuiNetworkHost.cpp \
                           ../src/Net/Android/nuiURL_Android.cpp \
                           ../src/Net/Android/nuiHTTP_Android.cpp \
                           ../src/Net/nuiSocket.cpp \
                           ../src/Net/nuiTCPClient.cpp \
                           ../src/Net/nuiTCPServer.cpp \
                           ../src/Net/nuiMimeMultiPart.cpp \
                           ../src/Net/nuiHTTP.cpp \


NUI_LOCAL_SRC_FILES_RENDERERS_PAINTERS := ../src/Renderers/nuiGLPainter.cpp \
                                          ../src/Renderers/nuiPainter.cpp \
                                          ../src/Renderers/nuiGL2Painter.cpp \
                                          ../src/Renderers/nuiShader.cpp \
                                          ../src/Renderers/nuiMetaPainter.cpp

NUI_LOCAL_SRC_FILES_RENDERERS_SHAPES_CONTOURS := ../src/Renderers/nuiArc.cpp \
                                                 ../src/Renderers/nuiContour.cpp \
                                                 ../src/Renderers/nuiGradient.cpp \
                                                 ../src/Renderers/nuiOutliner.cpp \
                                                 ../src/Renderers/nuiPath.cpp \
                                                 ../src/Renderers/nuiPathGenerator.cpp \
                                                 ../src/Renderers/nuiPathOptimizer.cpp \
                                                 ../src/Renderers/nuiPoint.cpp \
                                                 ../src/Renderers/nuiPolyLine.cpp \
                                                 ../src/Renderers/nuiRectPath.cpp \
                                                 ../src/Renderers/nuiShape.cpp \
                                                 ../src/Renderers/nuiSpline.cpp \
                                                 ../src/Renderers/nuiUniformDesc.cpp \
                                                 ../src/Renderers/nuiVertexAttribDesc.cpp \
                                                 ../src/Renderers/nuiSVGShape.cpp \
                                                 ../src/Renderers/nuiTessellator.cpp \

NUI_LOCAL_SRC_FILES_RENDERERS := ../src/Renderers/nuiDrawContext.cpp \
                                 ../src/Renderers/nuiRenderArray.cpp \
                                 ../src/Renderers/nuiRenderState.cpp \
                                 ../src/Renderers/nuiSurface.cpp \
                                 ../src/Renderers/nuiTexture.cpp \
                                 ../src/Renderers/nuiTextureHelpers.cpp \
                                 ../src/Renderers/AAPrimitives.cpp \
                                 $(NUI_LOCAL_SRC_FILES_RENDERERS_PAINTERS) \
                                 $(NUI_LOCAL_SRC_FILES_RENDERERS_SHAPES_CONTOURS) \


NUI_LOCAL_SRC_FILES_SIMPLE_WIDGETS := ../src/SimpleWidgets/nuiButton.cpp \
                                      ../src/SimpleWidgets/nuiColorSelector.cpp \
                                      ../src/SimpleWidgets/nuiComboBox.cpp \
                                      ../src/SimpleWidgets/nuiCoverFlow.cpp \
                                      ../src/SimpleWidgets/nuiFileList.cpp \
                                      ../src/SimpleWidgets/nuiFileSelector.cpp \
                                      ../src/SimpleWidgets/nuiFileTree.cpp \
                                      ../src/SimpleWidgets/nuiFontBrowser.cpp \
                                      ../src/SimpleWidgets/nuiFrameView.cpp \
                                      ../src/SimpleWidgets/nuiHotKeyEditor.cpp \
                                      ../src/SimpleWidgets/nuiHoverDummy.cpp \
                                      ../src/SimpleWidgets/nuiImage.cpp \
                                      ../src/SimpleWidgets/nuiImageDropZone.cpp \
                                      ../src/SimpleWidgets/nuiKnob.cpp \
                                      ../src/SimpleWidgets/nuiProgressBar.cpp \
                                      ../src/SimpleWidgets/nuiRadioButton.cpp \
                                      ../src/SimpleWidgets/nuiRadioButtonGroup.cpp \
                                      ../src/SimpleWidgets/nuiRectView.cpp \
                                      ../src/SimpleWidgets/nuiScrollBar.cpp \
                                      ../src/SimpleWidgets/nuiSeparator.cpp \
                                      ../src/SimpleWidgets/nuiShapeView.cpp \
                                      ../src/SimpleWidgets/nuiSlider.cpp \
                                      ../src/SimpleWidgets/nuiStateDummy.cpp \
                                      ../src/SimpleWidgets/nuiSVGView.cpp \
                                      ../src/SimpleWidgets/nuiTabBar.cpp \
                                      ../src/SimpleWidgets/nuiToggleButton.cpp \
                                      ../src/SimpleWidgets/nuiZoomBar.cpp \
                                      ../src/SimpleWidgets/nuiRSSView.cpp \
                                      ../src/SimpleWidgets/nuiHugeImage.cpp \

NUI_LOCAL_SRC_FILES_SPRITES := ../src/Sprites/nuiSpriteView.cpp \

NUI_LOCAL_SRC_FILES_STREAM := ../src/Stream/nglIFile.cpp \
                              ../src/Stream/nglIMemory.cpp \
                              ../src/Stream/nglIOFile.cpp \
                              ../src/Stream/nglIStream.cpp \
                              ../src/Stream/nglIZip.cpp \
                              ../src/Stream/nglOFile.cpp \
                              ../src/Stream/nglOMemory.cpp \
                              ../src/Stream/nglOStream.cpp \
                              ../src/Stream/nglStream.cpp \
                              ../src/Stream/nglZipFS.cpp \
                              ../src/Stream/nuiAsyncIStream.cpp \
                              ../src/Stream/Zip/ioapi.c \
                              ../src/Stream/Zip/unzip.c \
                              ../src/Stream/Zip/zip.c \


NUI_LOCAL_SRC_FILES_STRING := ../src/String/nglString.cpp \
                              ../src/String/nglStringConv_Android.cpp \
                              ../src/String/nuiRegExp.cpp \
                              ../src/String/nuiTranslator.cpp \
                              ../src/String/ConvertUTF.cpp \
                              ../src/String/nglUTFStringConv.cpp \
                              ../src/String/nuiUnicode.cpp \


NUI_LOCAL_SRC_FILES_TEXT := ../src/Text/nuiEditLine.cpp \
                            ../src/Text/nuiEditText.cpp \
                            ../src/Text/nuiFormatedLabel.cpp \
                            ../src/Text/nuiFormater.cpp \
                            ../src/Text/nuiHTMLView.cpp \
                            ../src/Text/nuiHyperLink.cpp \
                            ../src/Text/nuiLabel.cpp \
                            ../src/Text/nuiLabelAttribute.cpp \
                            ../src/Text/nuiScrollingLabel.cpp \
                            ../src/Text/nuiSpinnerLabel.cpp \
                            ../src/Text/nuiText.cpp \
                            ../src/Text/HTML/nuiHTMLBox.cpp \
                            ../src/Text/HTML/nuiHTMLContext.cpp \
                            ../src/Text/HTML/nuiHTMLFont.cpp \
                            ../src/Text/HTML/nuiHTMLHeader.cpp \
                            ../src/Text/HTML/nuiHTMLImage.cpp \
                            ../src/Text/HTML/nuiHTMLItem.cpp \
                            ../src/Text/HTML/nuiHTMLStyle.cpp \
                            ../src/Text/HTML/nuiHTMLTable.cpp \
                            ../src/Text/HTML/nuiHTMLText.cpp \
                            ../src/Text/HTML/nuiWebCSS.cpp \


NUI_LOCAL_SRC_FILES_THREADING := ../src/Threading/posix/nglCondition_posix.cpp \
                                 ../src/Threading/nglReaderWriterLock.cpp \
                                 ../src/Threading/posix/nglCriticalSection_posix.cpp \
                                 ../src/Threading/nglLightLock.cpp \
                                 ../src/Threading/nglLock.cpp \
                                 ../src/Threading/posix/nglSyncEvent_posix.cpp \
                                 ../src/Threading/nglThread.cpp \
                                 ../src/Threading/posix/nglThread_posix.cpp \
                                 ../src/Threading/nglThreadChecker.cpp \

NUI_LOCAL_SRC_FILES_TIME := ../src/Time/nglTime.cpp \
                            ../src/Time/nglTimer.cpp \
                            ../src/Time/Android/nglTimer_Android.cpp \


NUI_LOCAL_SRC_FILES_TREE_VIEWS := ../src/TreeViews/nuiPopupMenu.cpp \
                                  ../src/TreeViews/nuiColumnTreeView.cpp \
                                  ../src/TreeViews/nuiTreeView.cpp \


NUI_LOCAL_SRC_FILES_UTILS := ../src/Utils/nuiCSV.cpp \
                             ../src/Utils/ngl3DSLoader.cpp \
                             ../src/Utils/nglError.cpp \
                             ../src/Utils/nuiParser.cpp \
                             ../src/Utils/nuiRSS.cpp \
                             ../src/Utils/nuiStopWatch.cpp \
                             ../src/Utils/TextureAtlas.cpp \


NUI_LOCAL_SRC_FILES_VIDEO := ../src/Video/nuiAVIwriter.cpp \
                             ../src/Video/nuiVideoDecoder.cpp \



NUI_LOCAL_SRC_FILES_WIDGET_TREE := ../src/WidgetTree/nuiContainer.cpp \
                                   ../src/WidgetTree/nuiMainWindow.cpp \
                                   ../src/WidgetTree/nuiSimpleContainer.cpp \
                                   ../src/WidgetTree/nuiTopLevel.cpp \
                                   ../src/WidgetTree/nuiWidget.cpp \
                                   ../src/WidgetTree/nuiWindowManager.cpp \
                                   ../src/WidgetTree/nuiMatrixNode.cpp \


NUI_LOCAL_SRC_FILES_WINDOW := ../src/Window/nglContext.cpp \
                              ../src/Window/Android/nglContext_Android.cpp \
                              ../src/Window/Android/nglEvent_Android.cpp \
                              ../src/Window/Android/nglInputDevice_Android.cpp \
                              ../src/Window/Android/nglVideoMode_Android.cpp \
                              ../src/Window/Android/nglWindow_Android.cpp \
                              ../src/Window/Android/nuiMainMenu_Android.cpp \
                              ../src/Window/nglDeviceInfo.cpp \
                              ../src/Window/nglInputDevice.cpp \
                              ../src/Window/nglInputDeviceInstance.cpp \
                              ../src/Window/nglKeyboard.cpp \
                              ../src/Window/nglVideoMode.cpp \
                              ../src/Window/nglWindow.cpp \
                              ../src/Window/nuiGLUTBridge.cpp \
                              ../src/Window/nuiMainMenu.cpp \
                              ../src/Window/glext/ngl_glext_table.cpp \


NUI_LOCAL_SRC_FILES_DEFAULT_DECORATION := ../graphics/DefaultDecoration.cpp \
                                          ../graphics/DefaultDecoration/AtlasTexture.cpp \
                                          ../graphics/DefaultDecoration/ImageDraggedFile.cpp \




LOCAL_SRC_FILES := ../src/Application/Win/ngl.cpp \
                   $(NUI_LOCAL_SRC_FILES_DEFAULT_DECORATION) \
                   $(NUI_LOCAL_SRC_FILES_BASE) \
                   $(NUI_LOCAL_SRC_FILES_RENDERERS) \
                   $(NUI_LOCAL_SRC_FILES_TIME) \
                   $(NUI_LOCAL_SRC_FILES_WINDOW) \
                   $(NUI_LOCAL_SRC_FILES_NET) \
                   $(NUI_LOCAL_SRC_FILES_APPLICATION) \
                   $(NUI_LOCAL_SRC_FILES_STREAM) \
                   $(NUI_LOCAL_SRC_FILES_THREADING) \
                   $(NUI_LOCAL_SRC_FILES_WIDGET_TREE) \
                   $(NUI_LOCAL_SRC_FILES_VIDEO) \
                   $(NUI_LOCAL_SRC_FILES_UTILS) \
                   $(NUI_LOCAL_SRC_FILES_TREE_VIEWS) \
                   $(NUI_LOCAL_SRC_FILES_TEXT) \
                   $(NUI_LOCAL_SRC_FILES_STRING) \
                   $(NUI_LOCAL_SRC_FILES_SPRITES) \
                   $(NUI_LOCAL_SRC_FILES_SIMPLE_WIDGETS) \
                   $(NUI_LOCAL_SRC_FILES_NAVIGATIONS_VIEWS) \
                   $(NUI_LOCAL_SRC_FILES_LAYOUT) \
                   $(NUI_LOCAL_SRC_FILES_JSON) \
                   $(NUI_LOCAL_SRC_FILES_INTROSPECTOR) \
                   $(NUI_LOCAL_SRC_FILES_IMAGE) \
                   $(NUI_LOCAL_SRC_FILES_FONT) \
                   $(NUI_LOCAL_SRC_FILES_FILE) \
                   $(NUI_LOCAL_SRC_FILES_DIALOGS) \
                   $(NUI_LOCAL_SRC_FILES_BINDINGS) \
                   $(NUI_LOCAL_SRC_FILES_AUDIO) \
                   $(NUI_LOCAL_SRC_FILES_AUDIO_ENGINE) \
                   $(NUI_LOCAL_SRC_FILES_AUDIO_SAMPLES) \
                   $(NUI_LOCAL_SRC_FILES_ANIMATIONS) \
                   $(NUI_LOCAL_SRC_FILES_ATTRIBUTES) \
                   $(NUI_LOCAL_SRC_FILES_DECORATIONS) \
                   $(NUI_LOCAL_SRC_FILES_MATH) \


include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libnui3Shared
LOCAL_STATIC_LIBRARIES := libnui3

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,deps/mpg123/jni)
$(call import-module,deps/freetype2/jni)
$(call import-module,deps/expat/jni)
$(call import-module,deps/glu/libtess/jni)
$(call import-module,deps/harfbuzz/jni)
$(call import-module,deps/libcss/jni)
$(call import-module,deps/libpng/jni)
$(call import-module,deps/libjpeg/jni)
$(call import-module,deps/tidy/jni)
$(call import-module,deps/ucdata/jni)
$(call import-module,deps/ungif/jni)
