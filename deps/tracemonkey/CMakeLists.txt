project( nui3 )

IF(${CMAKE_SYSTEM} MATCHES "Linux")
SET(linux 1)
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux")

include_directories(
  .
)

add_definitions(
  -DXP_UNIX
  -DNGL_STATIC_BUILD
  -DHAVE_BCOPY
  -DFEATURE_NANOJIT
  -DJS_TRACER
  -DAVMPLUS_IA32
  -DDEBUG
)


set(TRACEMONKEY_SOURCES
  jsapi.cpp
  jsexn.cpp
  jsopcode.cpp
  jsscan.cpp
  jsutil.cpp
  jsarena.cpp
  jsdate.cpp
  jsfun.cpp
  jslock.cpp
  jsscope.cpp
  jsxdrapi.cpp
  jsarray.cpp
  jsdbgapi.cpp
  jsgc.cpp
  jslog2.cpp
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
  jsinvoke.cpp
  jsobj.cpp
  jstracer.cpp
  jscntxt.cpp
  jsemit.cpp
  jsiter.cpp
  json.cpp
  jsregexp.cpp
  jstypedarray.cpp
  nanojit/Allocator.cpp
  nanojit/Containers.cpp
  nanojit/Nativei386.cpp
  nanojit/NativeX64.cpp
  nanojit/Assembler.cpp
  nanojit/Fragmento.cpp
  nanojit/NativeMIPS.cpp
  nanojit/njconfig.cpp
  nanojit/avmplus.cpp
  nanojit/LIR.cpp
  nanojit/NativePPC.cpp
  nanojit/RegAlloc.cpp
  nanojit/CodeAlloc.cpp
  nanojit/NativeARM.cpp
  nanojit/NativeSparc.cpp
  nanojit/VMPI.cpp
  vprof/vprof.cpp
)

add_library(tracemonkey STATIC ${TRACEMONKEY_SOURCES})


