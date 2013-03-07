LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := my_native_module
LOCAL_SRC_FILES := my_native_code.cpp \
                   nuiAndroidBridge.cpp

LOCAL_ARM_MODE := arm

MY_C_FLAGS := -x c++ -fmessage-length=0 -pipe -Wno-trigraphs -O0 -D_ANDROID_ -DNGL_STATIC_BUILD -D_DEBUG -DDEBUG -fexceptions -fvisibility=hidden -fno-threadsafe-statics -Wno-deprecated-declarations

LOCAL_CFLAGS    :=  $(MY_C_FLAGS)

LOCAL_LDLIBS := -lGLESv1_CM -lEGL -landroid -llog -lstdc++ -lOpenSLES -lGLESv2

LOCAL_STATIC_LIBRARIES := libnui3 android_native_app_glue freetype expat tess harfbuzz libcss libpng libjpeg tidy ucdata ungif libmpg123

include $(BUILD_SHARED_LIBRARY)

$(call import-module,jni)


