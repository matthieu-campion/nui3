project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories(
  .
)

add_definitions(
  -DDEBUG
)

add_definitions(
  -fPIC
)

set(HARFBUZZ_SOURCES
  hb-blob.cc
  hb-buffer.cc
  hb-common.cc
  hb-fallback-shape.cc
  hb-font.cc
  hb-ft.cc
  #hb-glib.cc
  #hb-gobject.cc
  #hb-gobject-structs.cc
  #hb-icu.cc
  hb-ot-layout.cc
  hb-ot-map.cc
  hb-ot-shape.cc
  hb-ot-shape-complex-arabic.cc
  hb-ot-shape-complex-indic.cc
  hb-ot-shape-complex-misc.cc
  hb-ot-shape-normalize.cc
  hb-ot-tag.cc
  hb-shape.cc
  #hb-tt-font.cc
  hb-unicode.cc
  #hb-uniscribe-shape.cc
)

add_library(harfbuzz STATIC ${HARFBUZZ_SOURCES})


