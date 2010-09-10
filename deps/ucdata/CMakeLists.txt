project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories( . )

add_definitions(
  -fPIC
)

set(UCDATA_SOURCES
   ucdata.c  
   ucpgba.c  
)

add_library(ucdata STATIC ${UCDATA_SOURCES})

