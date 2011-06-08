LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH)/..
LOCAL_MODULE    := tess
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
dict.c \
geom.c \
memalloc.c \
mesh.c \
normal.c \
priorityq.c \
render.c \
sweep.c \
tess.c \
tessmono.c \
#priorityq-heap.c # do not build this file, it is directly included by priorityq.c!!!

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := tess-shared
LOCAL_STATIC_LIBRARIES := tess

include $(BUILD_SHARED_LIBRARY)


