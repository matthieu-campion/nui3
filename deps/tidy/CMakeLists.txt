project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories( . )


set(TIDY_SOURCES
   access.c  
   attrdict.c  
   buffio.c    
   config.c    
   iconvtc.c  
   localize.c  
   pprint.c    
   tags.c     
   utf8.c
   alloc.c    
   attrget.c   
   charsets.c  
   entities.c  
   istack.c   
   mappedio.c  
   streamio.c  
   tidylib.c  
   win32tc.c
   attrask.c  
   attrs.c     
   clean.c     
   fileio.c    
   lexer.c    
   parser.c    
   tagask.c    
   tmbstr.c
)

add_library(tidy STATIC ${TIDY_SOURCES})

