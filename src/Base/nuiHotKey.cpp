#include "nui.h"


nglString ModifiersToString(nuiKeyModifier mod)
{
//  nglString res = nglString::Empty;
  nglString res;

  if (mod &  nuiShiftKey)
  {
#ifdef _CARBON_
    res += _T("\u21e7");
#else
    res += _T("_SHIFT_");
#endif
  }
  
  if (mod &  nuiControlKey)
  {
#ifdef _CARBON_
    res += _T("\u2303");
#else
    res += _T("_CTRL_");
#endif
  }
  
  if (mod &  nuiAltKey)
  {
#ifdef _CARBON_
    res += _T("\u2325");
#else
    res += _T("_ALT_");
#endif
  }
  
  if (mod &  nuiMetaKey)
  {
#ifdef _CARBON_
    res += _T("\u2318");
#elif defined _WIN32_
    // #FIXME: the following unicode will only show the windows logo with the wingdings font
    //res += _T("\uf8ff");
    res += _T("_WIN_");
#else
    res += _T("_META_");
#endif
  }
  
  if (mod &  nuiMenuKey)
  {
    res += _T("_MENU_");
  }
  
  return res;
}




/////////////////////////
//
// TRIGGER = KEYCODE
//
/////////////////////////


template<>
bool nuiHotKeyImpl<nglKeyCode>::Trigger(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated)
{
  bool trig = (rEvent.mKey == mTrigger || !mTrigger);
  bool modControl = (mModifiers & nuiControlKey) == (CurrentModifiers & nuiControlKey);
  bool modAlt = (mModifiers & nuiAltKey) == (CurrentModifiers & nuiAltKey);
  bool modShift = (mModifiers & nuiShiftKey) == (CurrentModifiers & nuiShiftKey);
  bool modMeta = (mModifiers & nuiMetaKey) == (CurrentModifiers & nuiMetaKey);
  bool modMenu = (mModifiers & nuiMenuKey) == (CurrentModifiers & nuiMenuKey);

  if (trig && modControl && modAlt && modShift && modMeta && modMenu)
  {
    Activated();
    return true;
  }
  return false;
} 

template<>
nglString nuiHotKeyImpl<nglKeyCode>::ShortcutToString() const
{
  nglString shortcut = ModifiersToString(mModifiers);
  shortcut += GetKeyName(mTrigger);
  
  shortcut.ToUpper();
  return shortcut;
} 


/////////////////////////
//
// TRIGGER = CHAR
//
/////////////////////////


template<>
bool nuiHotKeyImpl<nglChar>::Trigger(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated)
{
  bool trig = (rEvent.mRawChar == mTrigger || !mTrigger);
  bool modControl = (mModifiers & nuiControlKey) == (CurrentModifiers & nuiControlKey);
  bool modAlt = (mModifiers & nuiAltKey) == (CurrentModifiers & nuiAltKey);
  bool modShift = (mModifiers & nuiShiftKey) == (CurrentModifiers & nuiShiftKey);
  bool modMeta = (mModifiers & nuiMetaKey) == (CurrentModifiers & nuiMetaKey);
  bool modMenu = (mModifiers & nuiMenuKey) == (CurrentModifiers & nuiMenuKey);
  
  if (trig && modControl && modAlt && modShift && modMeta && modMenu)
  {
    Activated();
    return true;
  }
  return false;
} 

template<>
nglString nuiHotKeyImpl<nglChar>::ShortcutToString() const
{
  nglString shortcut = ModifiersToString(mModifiers);
  shortcut += mTrigger;
  shortcut.ToUpper();
  return shortcut;
} 
