LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH)

LOCAL_MODULE    := ungif
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
dgif_lib.c \
gif_err.c \
gifalloc.c \


LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := ungif-shared
LOCAL_STATIC_LIBRARIES := ungif

include $(BUILD_SHARED_LIBRARY)
