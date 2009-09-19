
#if 0
#undef UNICODE
#undef _UNICODE
#define MAX(X,Y) ((X>Y)?X:Y)
#include "nglExceptionTrace.h"

void nglInitExceptionHandler()
{
  // Additional information flags
  UINT64 t = TraceSystemTime
  |  TraceLocation 
  |  TracePID
  |  TraceTID
  |  TracePrTime
  |  TraceThrTime
  |  TraceRealPrThrTime
  |  TraceAddInfoMask
    // Functions information flags
  |  TraceFnName
  |  TraceFnArgTypes
  |  TraceFnArgs
  |  TraceFnCallDisp
  //|  TraceFnStripTemplArgs
  //|  TraceFnStripStringArgs
  //|  TraceFnNoRawWideStrings
  //|  TraceFnDontUndecorate
    // Representation flags
  |  TraceMicroseconds
  |  TraceRtiHex
  |  TraceFnDecIntArgs
  |  TraceFnHexIntArgs
  |  TraceFnDispHex;

  SET_STACK_TRACE_OPTS(TraceAll | t);
}

#endif
