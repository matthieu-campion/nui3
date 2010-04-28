project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories( . )

add_definitions(
  -DXP_UNIX
)


set(TRACEMONKEY_SOURCES
  jsapi.cpp
  #jscpucfg.cpp
  jsexn.cpp
  #jskwgen.cpp
  jsopcode.cpp
  jsscan.cpp
  jsutil.cpp
  jsarena.cpp
  jsdate.cpp
  jsfun.cpp
  jslock.cpp
  #jsoplengen.cpp
  jsscope.cpp
  jsxdrapi.cpp
  jsarray.cpp
  jsdbgapi.cpp
  jsgc.cpp
  jslog2.cpp
  jsops.cpp
  jsscript.cpp
  jsxml.cpp
  jsatom.cpp
  jsdhash.cpp
  jshash.cpp
  jsmath.cpp
  jsparse.cpp
  jsstr.cpp
  prmjtime.cpp
  jsbool.cpp
  jsdtoa.cpp
  jsinterp.cpp
  jsnum.cpp
  jsprf.cpp
  jstask.cpp
  jsbuiltins.cpp
  #jsdtracef.cpp
  jsinvoke.cpp
  jsobj.cpp
  #jsrecursion.cpp
  jstracer.cpp
  jscntxt.cpp
  jsemit.cpp
  jsiter.cpp
  json.cpp
  jsregexp.cpp
  jstypedarray.cpp
)

add_library(tracemonkey STATIC ${TRACEMONKEY_SOURCES})


