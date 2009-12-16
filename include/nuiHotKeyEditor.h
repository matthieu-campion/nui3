/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#pragma once

#include "nui.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "nuiGrid.h"

class nuiHotKeyEditor : public nuiSimpleContainer
{
public:
  nuiHotKeyEditor();
  virtual ~nuiHotKeyEditor();
  virtual void ConnectTopLevel(); /// When connecting to the toplevel, retrieve the hotkeys stored in that toplevel in order to initialize the display with current hotkeys.

  void FillHotKeysGrid(); // clears current grid, and redraw it using toplevel's registered hotkeys, but with the user selected shortcuts rather than the ones registered if there are unsaved modifications in the editor
  
  virtual bool KeyDown(const nglKeyEvent& rEvent);
  virtual bool KeyUp(const nglKeyEvent& rEvent);
  bool OnCellClicked(const nuiEvent& rEvent); // 1st click = select // second click = edit // 3rd click = deselect
  
  void ApplyChanges(); // update toplevel's hotkey map 
  
  // Save (-Load) To (-From) an XMLNode
  bool Import(nuiXMLNode* pInputNode);
  nuiXMLNode* Serialize(nuiXMLNode* pParentNode) const;
  
  // Save (-Load) To (-From) a stream
  bool LoadFromStream(nglIStream* pInput);
  bool SaveToStream(nglOStream* pOutput) const;
      
protected:
  
  nuiTopLevel* mpTopLevel;
  
  nuiEventSink<nuiHotKeyEditor> mEventSink;
  nuiVBox* mpMainVBox;
  nuiHBox* mpColumnHeadersHBox;
  nuiGrid* mpHotKeysGrid;
  int mShortcutColumnWidth;
  
  std::map<nglString, nuiHotKey*> mpUserHotKeys; // stores the hotkeys that have been modified until they are applied
  
  nuiLabel* mpEditLabel; // label we're currently editing
  nuiLabel* mpSelectedLabel; // selected label
  
  nuiHotKey* mpSelectedHotKey; // the hotkey we're currently editing, as saved in toplevel
};