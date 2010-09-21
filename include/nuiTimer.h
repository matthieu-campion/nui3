/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


// This timer class is derived from nglTimer. It implements the const nuiEvent& rEvent model.

#ifndef __nuiTimer_h__
#define __nuiTimer_h__

//#include "nui.h"
#include "nglTimer.h"
#include "nuiEvent.h"

/// This class is a helper class that wraps an nuiEvent interface over a nglTimer timer system which is far more useful for GUI programming.
class NUI_API nuiTickEvent: public nuiEvent
{
public:
  nuiTickEvent(nglTime Lap);

  nglTime GetLap();
  void SetLap(nglTime Lap);
protected:
  nglTime mLap;
};

class NUI_API nuiTimer : public nglTimer
{
public:
  nuiTimer(nglTime Period); // in seconds
  virtual ~nuiTimer();

  virtual void OnTick(nglTime Lap);

  nuiEventSource Tick; /// Sends an nuiTickEvent.

  bool StartOnNextTick(bool Reset = true);
  /*!<
   \param Reset reset event counter
   \return true if the timer was successfully started
   
   You must invoke StartOnNextTick() to run the timer on the next animation timer tick, it does not run automatically at
   instantiation time.
   */

};

#endif // __nuiTimer_h__
