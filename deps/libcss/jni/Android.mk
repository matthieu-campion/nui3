LOCAL_PATH := $(call my-dir)/..


#STATIC LIBRARY

include $(CLEAR_VARS)

MY_INCLUDE_FILES := \
$(LOCAL_PATH)/include \
$(LOCAL_PATH)/src \
$(LOCAL_PATH)/src/libparserutils \
$(LOCAL_PATH)/src/libwapcaplet \

LOCAL_MODULE    := libcss
LOCAL_C_INCLUDES := $(MY_INCLUDE_FILES)
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
src/charset/detect.c \
src/lex/lex.c \
src/libcss.c \
src/libparserutils/charset/aliases.c \
src/libparserutils/charset/charset.c \
src/libparserutils/charset/codec.c \
src/libparserutils/charset/codecs/codec_8859.c \
src/libparserutils/charset/codecs/codec_ascii.c \
src/libparserutils/charset/codecs/codec_ext8.c \
src/libparserutils/charset/codecs/codec_utf16.c \
src/libparserutils/charset/codecs/codec_utf8.c \
src/libparserutils/charset/encodings/utf16.c \
src/libparserutils/charset/encodings/utf8.c \
src/libparserutils/input/filter.c \
src/libparserutils/input/inputstream.c \
src/libparserutils/parserutils.c \
src/libparserutils/utils/buffer.c \
src/libparserutils/utils/parser_errors.c \
src/libparserutils/utils/stack.c \
src/libparserutils/utils/vector.c \
src/libwapcaplet/libwapcaplet.c \
src/parse/important.c \
src/parse/language.c \
src/parse/parse.c \
src/parse/properties/aural.c \
src/parse/properties/background.c \
src/parse/properties/border_outline.c \
src/parse/properties/display_sizing.c \
src/parse/properties/effects_stacking.c \
src/parse/properties/font.c \
src/parse/properties/generated_list.c \
src/parse/properties/margin.c \
src/parse/properties/padding.c \
src/parse/properties/page.c \
src/parse/properties/parse_properties.c \
src/parse/properties/positioning.c \
src/parse/properties/properties_utils.c \
src/parse/properties/table.c \
src/parse/properties/text.c \
src/parse/properties/ui.c \
src/parse/propstrings.c \
src/select/computed.c \
src/select/dispatch.c \
src/select/hash.c \
src/select/select.c \
src/select/select_properties.c \
src/stylesheet.c \
src/utils/errors.c \
src/utils/utils.c \

LOCAL_EXPORT_C_INCLUDES := $(MY_INCLUDE_FILES)

include $(BUILD_STATIC_LIBRARY)



# SHARED LIBRARY

# Build the shared lib to force building of the static lib. 
# If you try to build (with ndk-build) a Android.mk file wich contains only a static library, it won't compile anything since there are no dependencies

include $(CLEAR_VARS)
LOCAL_MODULE := libcss-shared
LOCAL_STATIC_LIBRARIES := libcss

include $(BUILD_SHARED_LIBRARY)
