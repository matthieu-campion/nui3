LOCAL_PATH := $(call my-dir)/..


# STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := $(LOCAL_PATH)/../freetype-overlay/include $(LOCAL_PATH)/include
LOCAL_MODULE := freetype
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_LDLIBS := -lz
LOCAL_CFLAGS := -DFT2_BUILD_LIBRARY
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
src/base/ftsystem.c \
src/base/ftinit.c \
src/base/ftdebug.c \
src/base/ftbase.c \
src/base/ftbbox.c \
src/base/ftglyph.c \
src/base/ftbitmap.c \
src/base/ftcid.c \
src/base/ftfstype.c \
src/base/ftgasp.c \
src/base/ftgxval.c \
src/base/ftlcdfil.c \
src/base/ftmm.c \
src/base/ftotval.c \
src/base/ftpatent.c \
src/base/ftstroke.c \
src/base/ftsynth.c \
src/base/fttype1.c \
src/base/ftxf86.c \
src/cff/cff.c \
src/cid/type1cid.c \
src/sfnt/sfnt.c \
src/truetype/truetype.c \
src/type1/type1.c \
src/raster/raster.c \
src/smooth/smooth.c \
src/autofit/autofit.c \
src/cache/ftcache.c \
src/gxvalid/gxvalid.c \
src/otvalid/otvalid.c \
src/psaux/psaux.c \
src/pshinter/pshinter.c \
src/psnames/psnames.c \
src/pfr/pfr.c \
src/type42/type42.c \
src/winfonts/winfnt.c \
src/pcf/pcf.c \
src/gzip/ftgzip.c \
src/lzw/ftlzw.c \
src/bdf/bdf.c

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := freetype-shared
LOCAL_STATIC_LIBRARIES := freetype

include $(BUILD_SHARED_LIBRARY)
