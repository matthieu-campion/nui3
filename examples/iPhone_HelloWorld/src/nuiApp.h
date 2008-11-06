#ifndef __nuiApp_h__
#define __nuiApp_h__

#include "nuiApplication.h"

class nuiApp : public nuiApplication
{
public:
  nuiApp();
  ~nuiApp();
  
  void OnInit();
  void OnExit (int Code);
  void OnWillExit();
};

#endif//__nuiApp_h__
