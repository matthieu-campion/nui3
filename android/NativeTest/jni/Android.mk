LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := my_native_module
LOCAL_SRC_FILES := my_native_code.cpp \
                   nuiAndroidBridge.cpp

LOCAL_ARM_MODE := arm

MY_C_FLAGS := -x c++ -fmessage-length=0 -pipe -Wno-trigraphs -O0 -D_ANDROID_ -DNGL_STATIC_BUILD -D_DEBUG -DDEBUG -fexceptions -fvisibility=hidden -fno-threadsafe-statics -Wno-deprecated-declarations

LOCAL_CFLAGS    :=  $(MY_C_FLAGS)

LOCAL_LDLIBS := -lGLESv1_CM -lEGL -landroid -llog -lstdc++ -lOpenSLES

LOCAL_STATIC_LIBRARIES := android_native_app_glue libnui3 freetype expat tess harfbuzz libcss libpng libjpeg tidy ucdata ungif

LOCAL_SHARED_LIBRARIES := libopencore_common libomx_mp3_component_lib

include $(BUILD_SHARED_LIBRARY)

#include $(LOCAL_PATH)/../../../deps/opencore/Android.mk
$(call import-module,deps/opencore)
$(call import-module,android/native_app_glue)
$(call import-module,jni)


