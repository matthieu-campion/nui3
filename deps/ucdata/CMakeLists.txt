project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories( . )


set(UCDATA_SOURCES
   ucdata.c  
   ucpgba.c  
)

add_library(ucdata STATIC ${UCDATA_SOURCES})

