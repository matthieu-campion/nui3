#ifndef __MainWindow_h__
#define __MainWindow_h__

#include "nui.h"
#include "nuiGrid.h"
#include "nuiPane.h"
#include "nuiRectView.h"
#include "nuiColorSelector.h"

class MainWindow : public nglApplication
{
public:
  MainWindow();
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();
  nuiWindowManager* WorkStationPane;

  nuiTreeNode* mpWidgetTree;
  nuiTreeNode* mpFrameTree;
  nuiGrid* h;
  nuiPane* img ;
  nuiRectView* ff;
  nuiWindow* ViewWinP;
  nuiPane* perColor;
  nuiList* mpList;
  nuiColorSelector* pSelector;
  nuiTreeView* mpElementTree;
  void OnCreation();
  void OnInit();
  bool ColorChanged(const nuiEvent& rEvent);
  bool NewWidget(const nuiEvent& rEvent);  
  bool TreeUpdate(const nuiEvent& rEvent);
  bool WidgetMove(const nuiEvent& rEvent);
  void SetSize(uint pH,uint pW);

  
  bool OnCloset(const nuiEvent& rEvent);
  void OnClose();
protected:

  nuiEventSink<MainWindow> mSink;
};

#endif//__MainWindow_h__
