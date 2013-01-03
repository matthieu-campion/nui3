LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH)

LOCAL_MODULE    := libjpeg
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
jcapimin.c \
jcapistd.c \
jccoefct.c \
jccolor.c \
jcdctmgr.c \
jchuff.c \
jcinit.c \
jcmainct.c \
jcmarker.c \
jcmaster.c \
jcomapi.c \
jcparam.c \
jcphuff.c \
jcprepct.c \
jcsample.c \
jdapimin.c \
jdapistd.c \
jdatadst.c \
jdatasrc.c \
jdcoefct.c \
jdcolor.c \
jddctmgr.c \
jdhuff.c \
jdinput.c \
jdmainct.c \
jdmarker.c \
jdmaster.c \
jdmerge.c \
jdphuff.c \
jdpostct.c \
jdsample.c \
jerror.c \
jfdctflt.c \
jfdctfst.c \
jfdctint.c \
jidctflt.c \
jidctfst.c \
jidctint.c \
jmemmgr.c \
jmemnobs.c \
jquant1.c \
jquant2.c \
jutils.c \
jidctred.c \

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := libjpeg-shared
LOCAL_STATIC_LIBRARIES := libjpeg

include $(BUILD_SHARED_LIBRARY)
