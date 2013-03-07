LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH) $(LOCAL_PATH)/../freetype2/include

LOCAL_MODULE := harfbuzz
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_ARM_MODE := arm

LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := \
hb-blob.cc \
hb-buffer.cc \
hb-common.cc \
hb-font.cc \
hb-ft.cc \
hb-ot-layout.cc \
hb-ot-map.cc \
hb-ot-shape-complex-arabic.cc \
hb-ot-shape-complex-indic.cc \
hb-ot-shape.cc \
hb-ot-tag.cc \
hb-shape.cc \
hb-fallback-shape.cc \
hb-unicode.cc \

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

LOCAL_STATIC_LIBRARIES := libfreetype

include $(BUILD_STATIC_LIBRARY)
$(call import-module,deps/freetype2/jni)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib.
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := harfbuzz-shared
LOCAL_STATIC_LIBRARIES := harfbuzz

include $(BUILD_SHARED_LIBRARY)
