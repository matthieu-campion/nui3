LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH)/lib
MY_CFLAGS := \
-Wall \
-Wmissing-prototypes \
-Wstrict-prototypes \
-fexceptions \
-DHAVE_EXPAT_CONFIG_H \


LOCAL_MODULE := libexpat
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_CFLAGS    := $(MY_CFLAGS)
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
lib/xmlparse.c \
lib/xmlrole.c \
lib/xmltok.c \

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := libexpat-shared
LOCAL_STATIC_LIBRARIES := libexpat

include $(BUILD_SHARED_LIBRARY)
