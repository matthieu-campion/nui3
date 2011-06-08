LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := my_native_module
LOCAL_SRC_FILES := my_native_code.c

LOCAL_LDLIBS := -lGLESv1_CM -lEGL -landroid -llog

LOCAL_STATIC_LIBRARIES := android_native_app_glue freetype expat tess harfbuzz libcss libpng libjpeg tidy ucdata ungif
LOCAL_SHARED_LIBRARY := libnui

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,jni)
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
