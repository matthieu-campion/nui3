LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH)
LOCAL_MODULE    := tidy
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS    := \
-D_ANDROID_\ # added "|| defined(_ANDROID_)" in platform.h at the end of the line 523 to have the "ulong" type defined

LOCAL_SRC_FILES := \
access.c \
attrdict.c \
buffio.c \
config.c \
iconvtc.c \
localize.c \
pprint.c \
tags.c \
utf8.c \
alloc.c \
attrget.c \
charsets.c \
entities.c \
istack.c \
mappedio.c \
streamio.c \
tidylib.c \
win32tc.c \
attrask.c \
attrs.c \
clean.c \
fileio.c \
lexer.c \
parser.c \
tagask.c \
tmbstr.c \

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := tidy-shared
LOCAL_STATIC_LIBRARIES := tidy

include $(BUILD_SHARED_LIBRARY)
