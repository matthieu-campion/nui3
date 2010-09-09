project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories(
  include
  src
  src/libparserutils
)

add_definitions(
  -DDEBUG
)


set(LIBCSS_SOURCES
  src/libparserutils/charset/codec.c
  src/libparserutils/charset/aliases.c
  src/libparserutils/charset/charset.c
  src/libparserutils/charset/encodings
  src/libparserutils/charset/encodings/utf8.c
  src/libparserutils/charset/encodings/utf16.c
  src/libparserutils/charset/codecs/codec_8859.c
  src/libparserutils/charset/codecs/codec_utf16.c
  src/libparserutils/charset/codecs/codec_ext8.c
  src/libparserutils/charset/codecs/codec_utf8.c
  src/libparserutils/charset/codecs/codec_ascii.c
  src/libparserutils/input/filter.c
  src/libparserutils/input/inputstream.c
  src/libparserutils/parserutils.c
  src/libparserutils/utils/vector.c
  src/libparserutils/utils/utils.h
  src/libparserutils/utils/buffer.c
  src/libparserutils/utils/parser_errors.c
  src/libparserutils/utils/stack.c
  src/stylesheet.c
  src/charset/detect.c
  src/select/select.c
  src/select/dispatch.c
  src/select/select_properties.c
  src/select/hash.c
  src/select/computed.c
  src/libwapcaplet/libwapcaplet.c
  src/parse/language.c
  src/parse/properties/table.c
  src/parse/properties/positioning.c
  src/parse/properties/margin.c
  src/parse/properties/generated_list.c
  src/parse/properties/parse_properties.c
  src/parse/properties/font.c
  src/parse/properties/aural.c
  src/parse/properties/ui.c
  src/parse/properties/page.c
  src/parse/properties/text.c
  src/parse/properties/padding.c
  src/parse/properties/background.c
  src/parse/properties/properties_utils.c
  src/parse/properties/border_outline.c
  src/parse/properties/display_sizing.c
  src/parse/properties/effects_stacking.c
  src/parse/parse.c
  src/parse/propstrings.c
  src/parse/important.c
  src/libcss.c
  src/lex/lex.c
  src/utils/utils.c
  src/utils/errors.c
)

add_library(libcss STATIC ${LIBCSS_SOURCES})


