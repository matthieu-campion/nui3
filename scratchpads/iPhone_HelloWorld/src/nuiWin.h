#ifndef __nuiWin_h__
#define __nuiWin_h__

#include "nuiMainWindow.h"

class nuiWin : public nuiMainWindow
{
public:
  nuiWin(const nglContextInfo& rContext, 
         const nglWindowInfo& rInfo,
         const nglContext* pShared = NULL);
  ~nuiWin();
  
  void OnCreation();
  void OnClose();
};

#endif
