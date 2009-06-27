project(nui3)

INCLUDE(FindJPEG)
INCLUDE(FindPNG)
INCLUDE(FindZLIB)

FIND_PACKAGE ( EXPAT REQUIRED )
IF ( EXPAT_FOUND )
        INCLUDE_DIRECTORIES ( ${EXPAT_INCLUDE_DIRS} )
ELSE ( EXPAT_FOUND )
        add_subdirectory (expat)
ENDIF( EXPAT_FOUND )


add_subdirectory (ungif)         
add_subdirectory (harfbuzz)

