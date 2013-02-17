/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiHotKeyEditor::nuiHotKeyEditor()
: nuiSimpleContainer(), mEventSink(this)
{
  SetObjectClass(_T("nuiHotKeyEditor"));
  
  mShortcutColumnWidth = 150;
  
  mpEditLabel = NULL;
  mpSelectedLabel = NULL;
  mpSelectedHotKey = NULL;
  
  // #FIXME: temp highlight : write text in green when a label is seleted. Background coloring doesn't seem to work though
  SetColor(eNormalTextFg, nuiColor(_T("black")));
  SetColor(eSelectedTextFg, nuiColor(_T("Green")));
  SetColor(eDisabledTextFg, nuiColor(_T("red")));
  SetColor(eNormalTextBg, nuiColor(_T("Red")));
  SetColor(eSelectedTextBg, nuiColor(_T("Green")));
  SetColor(eDisabledTextBg, nuiColor(_T("Blue")));
  
  SetWantKeyboardFocus(true);
}

nuiHotKeyEditor::~nuiHotKeyEditor()
{
  std::map<nglString, nuiHotKey*>::iterator pUserHotKeysIt;
  
  for (pUserHotKeysIt = mpUserHotKeys.begin(); pUserHotKeysIt != mpUserHotKeys.end(); ++pUserHotKeysIt)
  {
    nglString name = pUserHotKeysIt->first;
    nuiHotKey* pHotKey = pUserHotKeysIt->second;
    
    if (pHotKey != mpTopLevel->GetHotKey(name))
    {
      // #TODO:
      // Warn user about unsaved hotkeys, and allow him to save / apply changes before killing the editor ?
      
      // a new hotkey has been created for that command but wasn't saved, delete it
      delete pHotKey;
    }
  }
}

void nuiHotKeyEditor::ConnectTopLevel()
{
  nuiSimpleContainer::ConnectTopLevel();
  
  mpTopLevel = GetTopLevel();
  
  mpMainVBox = new nuiVBox();
  mpMainVBox->SetObjectName(_T("nuiHotKeyEditor_MainVBox"));
  mpMainVBox->SetExpand(nuiExpandGrow);
  
  // Column Headers
  mpColumnHeadersHBox = new nuiHBox(2);
  mpColumnHeadersHBox->SetObjectName(_T("nuiHotKeyEditor_ColumnHeaders"));
  
  nuiLabel* pDescriptionColHeader = new nuiLabel(_T("Description"));
  pDescriptionColHeader->SetObjectName(_T("nuiHotKeyEditor_DescriptionColumnHeaderLabel"));
  mpColumnHeadersHBox->SetCell(0, pDescriptionColHeader, nuiCenter);
  
  nuiLabel* pShortcutColHeader = new nuiLabel(_T("Shortcut"));
  pShortcutColHeader->SetObjectName(_T("nuiHotKeyEditor_ShortcutColumnHeaderLabel"));
  mpColumnHeadersHBox->SetCell(1, pShortcutColHeader, nuiCenter);
  
  mpColumnHeadersHBox->SetCellExpand(0, nuiExpandGrow);
  mpColumnHeadersHBox->SetCellPixels(1, mShortcutColumnWidth);
  mpColumnHeadersHBox->SetCellExpand(1, nuiExpandFixed);
//  mpColumnHeadersHBox->DisplayGridBorder(true);
  
  mpMainVBox->AddCell(mpColumnHeadersHBox);
  
  mpHotKeysGrid = new nuiGrid();
  mpHotKeysGrid->SetObjectName(_T("nuiHotKeyEditor_HotKeys Grid"));
  
  // retrieve toplevel hotkeys and initialize the user hotkeys map
  const std::map<nglString, nuiHotKey*>& pTopLevelHotKeys = mpTopLevel->GetHotKeys();
  std::map<nglString, nuiHotKey*>::const_iterator pTopLevelHotKeysIt;
  for (pTopLevelHotKeysIt = pTopLevelHotKeys.begin(); pTopLevelHotKeysIt != pTopLevelHotKeys.end(); ++pTopLevelHotKeysIt)
  {
    mpUserHotKeys[pTopLevelHotKeysIt->first] = pTopLevelHotKeysIt->second;
  }
  
  // Fill the grid with now up-to-date toplevel hotkeys
  FillHotKeysGrid();
  mpMainVBox->AddCell(mpHotKeysGrid);
  
  AddChild(mpMainVBox);
}

void nuiHotKeyEditor::FillHotKeysGrid()
{
  // Reset Grid
  mpHotKeysGrid->ClearCells();
    
  std::map<nglString, nuiHotKey*>::iterator pUserHotKeysIt;
  mpHotKeysGrid->SetGridSize(2, mpUserHotKeys.size());
  int row = 0;
  
  for (pUserHotKeysIt = mpUserHotKeys.begin(); pUserHotKeysIt != mpUserHotKeys.end(); ++pUserHotKeysIt)
  {   
    nglString name = pUserHotKeysIt->first;
    nuiHotKey* pHotKey = pUserHotKeysIt->second;
    
    nuiLabel* pDescriptionLabel = new nuiLabel(pHotKey->GetDescription());
    pDescriptionLabel->SetObjectName(_T("nuiHotKeyEditor_DescriptionCell"));
    mpHotKeysGrid->SetCell(0, row, pDescriptionLabel, nuiFill);
    
    nuiLabel* pHotKeyLabel = new nuiLabel(pHotKey->ShortcutToString());
    pHotKeyLabel->SetObjectName(_T("nuiHotKeyEditor_ShortcutCell"));
    pHotKeyLabel->SetToken(new nuiToken<nglString>(name));
    mpHotKeysGrid->SetCell(1, row, pHotKeyLabel, nuiFill);
    
    mEventSink.Connect(pHotKeyLabel->Clicked, &nuiHotKeyEditor::OnCellClicked, (void*)pHotKeyLabel);
    
    ++row;
  }
  
//  mpHotKeysGrid->DisplayGridBorder(true);
  mpHotKeysGrid->SetColumnExpand(0, nuiExpandGrow);
  mpHotKeysGrid->SetColumnPixels(1, mShortcutColumnWidth);
  mpHotKeysGrid->SetColumnExpand(1, nuiExpandFixed);
}

void nuiHotKeyEditor::OnCellClicked(const nuiEvent& rEvent)
{
  NGL_ASSERT(rEvent.mpUser != NULL);
  nuiLabel* pLabel = (nuiLabel*) rEvent.mpUser;
  // Retrieve the token we stored: <nglString> HotKeyName
  nglString HotKeyName;
  nuiGetTokenValue<nglString>(pLabel->GetToken(), HotKeyName);
    
  const nuiMouseClickedEvent& rClicked((const nuiMouseClickedEvent&)rEvent);
  
  if (rClicked.mButton & nglMouseInfo::ButtonLeft)
  {
    if (pLabel == mpEditLabel) // this shortcut was in edition
    {
      // cancel editing
      mpEditLabel->SetSelected(false);
      mpEditLabel->SetText(mpSelectedHotKey->ShortcutToString());
      mpEditLabel = NULL;
      mpSelectedLabel = NULL;
      mpSelectedHotKey = NULL;
    }
    else if (pLabel == mpSelectedLabel)
    {
      // the cell was already selected, enter edit mode
      mpEditLabel = pLabel;
      mpEditLabel->SetText(_T("....."));
    }
    else // the cell was inactive, cancel everything in progress and select this one
    {
      // cancel editing
      if (mpEditLabel)
      {        
        mpEditLabel->SetText(mpSelectedHotKey->ShortcutToString());
        mpEditLabel = NULL;
      }
      
      // cancel current selection
      if (mpSelectedLabel)
      {
        mpSelectedLabel->SetSelected(false);
      }
      
      // finally, select this label
      mpSelectedLabel = pLabel;
      mpSelectedLabel->SetSelected(true);
      mpSelectedHotKey = mpUserHotKeys[HotKeyName];
    }
    
    rEvent.Cancel();
    return;
  } 
}


bool nuiHotKeyEditor::KeyDown(const nglKeyEvent& rEvent)
{
  if (mpEditLabel == NULL) // we're not editing anything
  {
    return false;
  }
  
  // pressing a modifier shouldn't do anything. In case it is a non-modifier key, we'll see what modifiers are down, and then retrieve the trigger key
  
  // NK_RMETA and NK_LMETA won't fire keydown anyway, but in case they do someday, those keys are tested as well
  if ((rEvent.mKey != NK_LSHIFT) && (rEvent.mKey != NK_RSHIFT) && (rEvent.mKey != NK_LCTRL) && (rEvent.mKey != NK_RCTRL) && (rEvent.mKey != NK_LALT) && (rEvent.mKey != NK_RALT) && (rEvent.mKey != NK_LMETA) && (rEvent.mKey != NK_RMETA) && (rEvent.mKey != NK_MENU))  // then it's the trigger Key
  {
    nuiKeyModifier modifiers = 0;
    	
    // retrieve modifiers
    if ((IsKeyDown(NK_LSHIFT)) || (IsKeyDown(NK_RSHIFT)))
    {
      modifiers |= nuiShiftKey;
    }
    
    if ((IsKeyDown(NK_LCTRL)) || (IsKeyDown(NK_RCTRL)))
    {
      modifiers |= nuiControlKey;
    }
    
    if ((IsKeyDown(NK_LALT)) || (IsKeyDown(NK_RALT)))
    {
      modifiers |= nuiAltKey;
    }
    
    if ((IsKeyDown(NK_LMETA)) || (IsKeyDown(NK_RMETA)))
    {
      modifiers |= nuiMetaKey;
    }
    
    if (IsKeyDown(NK_MENU))
    {
      modifiers |= nuiMenuKey;
    }
    
    // Now, the trigger
    if ((rEvent.mKey == NK_NONE) || (rEvent.mKey == NK_ESC) || (rEvent.mKey == NK_F1) || (rEvent.mKey == NK_F2) || (rEvent.mKey == NK_F3) || (rEvent.mKey == NK_F4) || (rEvent.mKey == NK_F5) || (rEvent.mKey == NK_F6) || (rEvent.mKey == NK_F7) || (rEvent.mKey == NK_F8) || (rEvent.mKey == NK_F9) || (rEvent.mKey == NK_F10) || (rEvent.mKey == NK_F11) || (rEvent.mKey == NK_F12) || 
        (rEvent.mKey == NK_SYSREQ) || (rEvent.mKey == NK_SCRLOCK) || (rEvent.mKey == NK_PAUSE) || 
        (rEvent.mKey == NK_CAPSLOCK) || (rEvent.mKey == NK_BACKSPACE) || (rEvent.mKey == NK_ENTER) || (rEvent.mKey == NK_TAB) || (rEvent.mKey == NK_SPACE) ||
        (rEvent.mKey == NK_DELETE) || (rEvent.mKey == NK_END) || (rEvent.mKey == NK_INSERT) || (rEvent.mKey == NK_HOME) || (rEvent.mKey == NK_PAGEUP) || (rEvent.mKey == NK_PAGEDOWN) || 
        (rEvent.mKey == NK_PAD_LOCK) || (rEvent.mKey == NK_PAD_ASTERISK) || (rEvent.mKey == NK_PAD_SLASH) || (rEvent.mKey == NK_PAD_PLUS) || (rEvent.mKey == NK_PAD_MINUS) || (rEvent.mKey == NK_PAD_0) || (rEvent.mKey == NK_PAD_1) || (rEvent.mKey == NK_PAD_2) || (rEvent.mKey == NK_PAD_3) || (rEvent.mKey == NK_PAD_4) || (rEvent.mKey == NK_PAD_5) || (rEvent.mKey == NK_PAD_6) || (rEvent.mKey == NK_PAD_7) || (rEvent.mKey == NK_PAD_8) || (rEvent.mKey == NK_PAD_9) || (rEvent.mKey == NK_PAD_PERIOD) || (rEvent.mKey == NK_PAD_ENTER) || 
        (rEvent.mKey == NK_LEFT) || (rEvent.mKey == NK_DOWN) || (rEvent.mKey == NK_RIGHT) || (rEvent.mKey == NK_UP))
    {
      // Use KeyCode
      nglKeyCode triggerKeyCode = rEvent.mKey;
      
      // retrieve the other settings from toplevel:
      bool hasPriority = mpSelectedHotKey->HasPriority();
      bool isOnKeyUp = mpSelectedHotKey->IsOnKeyUp();
      nglString name = mpSelectedHotKey->GetName();
      nglString description = mpSelectedHotKey->GetDescription();
      bool enabled = mpSelectedHotKey->IsEnabled();
      
      nuiHotKeyKey* pHotKeyKey = new nuiHotKeyKey(triggerKeyCode, modifiers, hasPriority, isOnKeyUp);
      pHotKeyKey->SetName(name);
      pHotKeyKey->SetDescription(description);
      pHotKeyKey->SetEnabled(enabled);
      
      mpEditLabel->SetText(pHotKeyKey->ShortcutToString());
      
      // if a user hotkey was already assigned, but hasn't been applied yet, delete it
      nuiHotKey* pTopLevelHotKey = mpTopLevel->GetHotKey(name);
      if (mpSelectedHotKey != pTopLevelHotKey)
      {
        delete mpSelectedHotKey;
      }
      // update the list of user hotkeys
      mpUserHotKeys[name] = pHotKeyKey;
    }
    else
    {
      // Use Char
      nglChar triggerChar = rEvent.mRawChar;
      
      // retrieve the other settings from toplevel:
      bool hasPriority = mpSelectedHotKey->HasPriority();
      bool isOnKeyUp = mpSelectedHotKey->IsOnKeyUp();
      nglString name = mpSelectedHotKey->GetName();
      nglString description = mpSelectedHotKey->GetDescription();
      bool enabled = mpSelectedHotKey->IsEnabled();
      
      nuiHotKeyChar* pHotKeyChar = new nuiHotKeyChar(triggerChar, modifiers, hasPriority, isOnKeyUp);
      pHotKeyChar->SetName(name);
      pHotKeyChar->SetDescription(description);
      pHotKeyChar->SetEnabled(enabled);
      
      mpEditLabel->SetText(pHotKeyChar->ShortcutToString());
      
      // if a user hotkey was already assigned, but hasn't been applied yet, delete it
      nuiHotKey* pTopLevelHotKey = mpTopLevel->GetHotKey(name);
      if (mpSelectedHotKey != pTopLevelHotKey)
      {
        delete mpSelectedHotKey;
      }
      // update the list of user hotkeys
      mpUserHotKeys[name] = pHotKeyChar;
    }    
    
    // exit editing
    mpEditLabel = NULL;
    mpSelectedLabel->SetSelected(false);
    mpSelectedLabel = NULL;
    mpSelectedHotKey = NULL;
  }
  return true;
}

bool nuiHotKeyEditor::KeyUp(const nglKeyEvent& rEvent)
{
  if (mpEditLabel == NULL) // we're not editing anything, or we just assigned a new key by pressing a trigger
  {
    return false;
  }
  else // a modifier key has been pressed, while editing a hotkey
  {
    return true;
  }
}


void nuiHotKeyEditor::ApplyChanges()
{
  std::map<nglString, nuiHotKey*>::iterator pUserHotKeysIt;
  
  for (pUserHotKeysIt = mpUserHotKeys.begin(); pUserHotKeysIt != mpUserHotKeys.end(); ++pUserHotKeysIt)
  {
    nglString name = pUserHotKeysIt->first;
    nuiHotKey* pHotKey = pUserHotKeysIt->second;
    
    if (pHotKey == mpTopLevel->GetHotKey(name))
    {
      // unchanged hotkey, nothing to do here
    }
    else
    {
      mpTopLevel->SetHotKey(name, pHotKey);
    }
  }
}

bool nuiHotKeyEditor::Import(nuiXMLNode* pInputNode)
{
  nglString nodeName = pInputNode->GetName();
  if (nodeName.Compare(NUIHOTKEYS_XML_NODEID))
  {
    NGL_OUT(_T("Not a hotkeys node\n : %s"), nodeName.GetChars());
    return false;
  }
  
  // delete all user hotkeys that have not been applied yet ?
//  std::map<nglString, nuiHotKey*>::iterator pUserHotKeysIt;
//  for (pUserHotKeysIt = mpUserHotKeys.begin(); pUserHotKeysIt != mpUserHotKeys.end(); ++pUserHotKeysIt)
//  {
//    nglString name = pUserHotKeysIt->first;
//    nuiHotKey* pHotKey = pUserHotKeysIt->second;
//    
//    if (pHotKey != mpTopLevel->GetHotKey(name));
//    {
//      delete pHotKey;
//    }
//  }
  // Empty the user hotkeys list ?
//  mpUserHotKeys.clear();
  // IF HOTKEYS ARE DELETED, CHECKS ON EDITED HOTKEYS ARE NOT NECESSARY ANYMORE BELOW
  
  // Parse the list of hotkeys to import
  const nuiXMLNodeList& pHotKeysList = pInputNode->GetChildren();
  nuiXMLNodeList::const_iterator pImportHotKeysIt;
  
  for (pImportHotKeysIt = pHotKeysList.begin(); pImportHotKeysIt != pHotKeysList.end(); ++pImportHotKeysIt)
  {
    nuiXMLNode* pNode = *pImportHotKeysIt;
    nglString name = pNode->GetAttribute(_T("Name"));
    nglString description = pNode->GetAttribute(_T("Description"));
    nuiKeyModifier modifiers = pNode->GetAttribute(_T("Modifiers")).GetUInt();
    bool hasPriority = pNode->GetAttribute(_T("Priority")).Compare(_T("true")) ? false : true;
    bool isOnKeyUp = pNode->GetAttribute(_T("OnKeyUp")).Compare(_T("true")) ? false : true;
    
    // if that hotkey has been modified, but changes haven't been applied yet, delete it first
    std::map<nglString, nuiHotKey*>::iterator pUserHotKeysIt = mpUserHotKeys.find(name);
    if (pUserHotKeysIt != mpUserHotKeys.end())
    {
      nuiHotKey* pUserHotKey = pUserHotKeysIt->second;
      if (pUserHotKey != mpTopLevel->GetHotKey(name))
      {
        delete(pUserHotKey);
      }
    }
    
    bool isHotKeyKey = pNode->GetAttribute(_T("IsHotKeyKey")).Compare(_T("true")) ? false : true;
    
    if (isHotKeyKey)
    {
      nglKeyCode trigger = pNode->GetAttribute(_T("Trigger_KeyCode")).GetUInt();
      nuiHotKeyKey* pHotKeyKey = new nuiHotKeyKey(trigger, modifiers, hasPriority, isOnKeyUp);
      pHotKeyKey->SetName(name);
      pHotKeyKey->SetDescription(description);
      mpUserHotKeys[name] = pHotKeyKey;
    }
    else
    {
      nglString trig = pNode->GetAttribute(_T("Trigger_Char"));
      nglChar trigger = (nglChar)trig.GetCInt();
      nuiHotKeyChar* pHotKeyChar = new nuiHotKeyChar(trigger, modifiers, hasPriority, isOnKeyUp);
      pHotKeyChar->SetName(name);
      pHotKeyChar->SetDescription(description);
      mpUserHotKeys[name] = pHotKeyChar;
    }
  }
  
  // update the grid
  FillHotKeysGrid();
  
  return true;
}

nuiXMLNode* nuiHotKeyEditor::Serialize(nuiXMLNode* pParentNode) const
{
  nuiXMLNode* pHotKeysNode = new nuiXMLNode(NUIHOTKEYS_XML_NODEID, pParentNode);
  
  std::map<nglString, nuiHotKey*>::const_iterator pUserHotKeysIt;
  
  for (pUserHotKeysIt = mpUserHotKeys.begin(); pUserHotKeysIt != mpUserHotKeys.end(); ++pUserHotKeysIt)
  {
    nglString name = pUserHotKeysIt->first;
    nuiHotKey* pUserHotKey = pUserHotKeysIt->second;
    
    nuiHotKeyKey* pUserHotKeyKey = dynamic_cast<nuiHotKeyKey*> (pUserHotKey);
    nuiHotKeyChar* pUserHotKeyChar = dynamic_cast<nuiHotKeyChar*> (pUserHotKey);
    
    if (pUserHotKeyKey)
    {
      nuiXMLNode* pNode = new nuiXMLNode(_T("HotKey"), pHotKeysNode);
      pNode->SetAttribute(_T("Name"), name);
      pNode->SetAttribute(_T("Description"), pUserHotKeyKey->GetDescription());
      pNode->SetAttribute(_T("IsHotKeyKey"), true);
      pNode->SetAttribute(_T("Trigger_KeyCode"), (int64)pUserHotKeyKey->GetTrigger());
      pNode->SetAttribute(_T("Modifiers"), (int64)pUserHotKeyKey->GetModifiers());
      pNode->SetAttribute(_T("Priority"), pUserHotKeyKey->HasPriority());
      pNode->SetAttribute(_T("OnKeyUp"), pUserHotKeyKey->IsOnKeyUp());
    }
    else if (pUserHotKeyChar)
    {
      nuiXMLNode* pNode = new nuiXMLNode(_T("HotKey"), pHotKeysNode);
      pNode->SetAttribute(_T("Name"), name);
      pNode->SetAttribute(_T("Description"), pUserHotKeyChar->GetDescription());
      pNode->SetAttribute(_T("IsHotKeyKey"), false);
      pNode->SetAttribute(_T("Trigger_Char"), (int64)pUserHotKeyChar->GetTrigger());
      pNode->SetAttribute(_T("Modifiers"), (int64)pUserHotKeyChar->GetModifiers());
      pNode->SetAttribute(_T("Priority"), pUserHotKeyChar->HasPriority());
      pNode->SetAttribute(_T("OnKeyUp"), pUserHotKeyChar->IsOnKeyUp());
    }
    else
    {
      NGL_ASSERT(pUserHotKeyKey || pUserHotKeyChar);
    }
  }
  return pHotKeysNode;
}

bool nuiHotKeyEditor::LoadFromStream(nglIStream* pInput)
{
  nuiXML* pXML = new nuiXML(nglString::Null);
  if (pXML->Load(*pInput))
  {
    NGL_OUT(pXML->Dump());
    bool success = Import(pXML->GetChild(NUIHOTKEYS_XML_NODEID));
    delete pXML;
    
    return success;
  }
  else
  {
    NGL_OUT(_T("Couldn't load hotkeys from file\n"));
    delete pXML;
    
    return false;
  }
}

bool nuiHotKeyEditor::SaveToStream(nglOStream* pOutput) const
{
  nuiXML* pHotKeysXML = new nuiXML(_T("HotKeys_XML_File"));
  nuiXMLNode* pHotKeyNode = Serialize(pHotKeysXML);
  if (pHotKeysXML->Save(*pOutput))
  {
    NGL_OUT(pHotKeysXML->Dump());
    delete pHotKeysXML;
    
    return true;
  }
  else
  {
    NGL_OUT(_T("Couldn't save hotkeys to file\n"));
    delete pHotKeysXML;
    
    return false;
  }
}
