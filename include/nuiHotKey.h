/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiHotKey_h__
#define __nuiHotKey_h__

#include "nglKeyboard.h"
#include "nglString.h"
#include "nglKernel.h"
#include "nuiEvent.h"
#include "nuiEventRegistry.h"

#define NUIHOTKEYS_XML_NODEID _T("HotKeys")

typedef uint32 nuiKeyModifier;

// physical keys
const nuiKeyModifier nuiNoKey = 0;
const nuiKeyModifier nuiControlKey = 1 << 0;
const nuiKeyModifier nuiAltKey = 1 << 1;
const nuiKeyModifier nuiShiftKey = 1 << 2;
const nuiKeyModifier nuiMetaKey = 1 << 3; // better known as the "WIN" key on WIN32, and "COMMAND" key on OSX, see below
const nuiKeyModifier nuiMenuKey = 1 << 4;

// Win / Command / Meta key platform-dependent usage
const nuiKeyModifier nuiCommandKey = nuiMetaKey; // you can use this one if you're more comfortable with the "COMMAND" key
const nuiKeyModifier nuiWinKey = nuiMetaKey; // or you can use this one if you're more comfortable with the "WIN" key

// Ctrl / Command (Mac / PC trick)
#ifdef _CARBON_
const nuiKeyModifier nuiModKey = nuiCommandKey; 
#else
const nuiKeyModifier nuiModKey = nuiControlKey;
#endif


nglString ModifiersToString(nuiKeyModifier mod);

class nuiHotKey
{
public:
  virtual ~nuiHotKey() {};
  
  virtual bool OnKeyDown(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated) = 0;
  virtual bool OnKeyUp(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated) = 0;
  virtual bool Trigger(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated) = 0;
  
  virtual void SetName(const nglString& rName) = 0;
  virtual void SetDescription(const nglString& rDescription) = 0;
  virtual const nglString& GetName() const = 0;
  virtual const nglString& GetDescription() const = 0;
  virtual void SetEnabled(bool set) = 0;
  virtual bool IsEnabled() const = 0;
  virtual nglString ShortcutToString() const = 0;
  virtual nuiKeyModifier GetModifiers() const = 0;
  virtual void SetPriority(bool set) = 0;
  virtual void SetOnKeyUp(bool set) = 0;
  virtual bool HasPriority() const = 0;
  virtual bool IsOnKeyUp() const = 0;
  virtual void SetReferenceCount(uint refCount) = 0;
  virtual uint GetReferenceCount() const = 0;
  
  virtual void Register() = 0;
  virtual bool Release() = 0;   // Returns true if reference count is 0;
};

template<typename T>
class nuiHotKeyImpl : public nuiHotKey
{
public:
  nuiHotKeyImpl(T Trigger, nuiKeyModifier Modifiers, bool Priority = false, bool FireOnKeyUp = false);

  virtual ~nuiHotKeyImpl();

  bool OnKeyDown(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated);
  bool OnKeyUp(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated);
  bool Trigger(const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated);
  void SetParams(T Trigger, nuiKeyModifier Modifiers, bool Priority = false, bool FireOnKeyUp = false, bool Enabled = true);

  void SetName(const nglString& rName);
  void SetDescription(const nglString& rDescription);
  const nglString& GetName() const;
  const nglString& GetDescription() const;
  void SetEnabled(bool set);
  bool IsEnabled() const;
  nglString ShortcutToString() const;
  T GetTrigger() const;
  nuiKeyModifier GetModifiers() const;
  void SetPriority(bool set);
  void SetOnKeyUp(bool set);
  bool HasPriority() const;
  bool IsOnKeyUp() const;
  void SetReferenceCount(uint refCount);
  uint GetReferenceCount() const;
  
  void Register();  // Add 1 reference
  bool Release();   // Returns true if reference count is 0

private:
  T mTrigger;             // Trigger (usually of type nglKeyCode or nglChar)
  nuiKeyModifier mModifiers;
  bool mPriority;       // HotKey Priority over nuiWidget::KeyDown/KeyUp
  bool mOnKeyUp;          // HotKey should be activated on key up
  nglString mName;
  nglString mDescription;
  bool mEnabled;
  uint mReferenceCount;   // add 1 for each widget using this hotkey
};

typedef nuiHotKeyImpl<nglKeyCode> nuiHotKeyKey;
typedef nuiHotKeyImpl<nglChar> nuiHotKeyChar;

template <typename T>
nuiHotKeyImpl<T>::nuiHotKeyImpl( T Trigger, nuiKeyModifier Modifiers, bool Priority /*= false*/, bool FireOnKeyUp /*= false*/ )
: mTrigger(Trigger), mModifiers(Modifiers), mPriority(Priority), mOnKeyUp(FireOnKeyUp), mReferenceCount(0), mEnabled(true)
{  
}

template <typename T>
nuiHotKeyImpl<T>::~nuiHotKeyImpl()
{
}

template <typename T>
bool nuiHotKeyImpl<T>::OnKeyDown( const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated )
{
  if (mOnKeyUp)
    return false;
  
  return Trigger(rEvent, CurrentModifiers, Activated);
}

template <typename T>
bool nuiHotKeyImpl<T>::OnKeyUp( const nglKeyEvent& rEvent, nuiKeyModifier CurrentModifiers, nuiSimpleEventSource<nuiWidgetActivated>& Activated )
{
  if (!mOnKeyUp)
    return false;
  
  return Trigger(rEvent, CurrentModifiers, Activated);
}

template <typename T>
void nuiHotKeyImpl<T>::SetName( const nglString& rName )
{
  mName = rName;
}

template <typename T>
const nglString& nuiHotKeyImpl<T>::GetName() const
{
  return mName;
}

template <typename T>
T nuiHotKeyImpl<T>::GetTrigger() const
{
  return mTrigger;
} 

template <typename T>
void nuiHotKeyImpl<T>::SetDescription(const nglString& rDescription)
{
  mDescription = rDescription;
}

template <typename T>
const nglString& nuiHotKeyImpl<T>::GetDescription() const
{
  return mDescription;
}

template <typename T>
void nuiHotKeyImpl<T>::SetEnabled(bool set)
{
  mEnabled = set;
}

template <typename T>
bool nuiHotKeyImpl<T>::IsEnabled() const
{
  return mEnabled;
}

template <typename T>
nuiKeyModifier nuiHotKeyImpl<T>::GetModifiers() const
{
  return mModifiers;
} 

template <typename T>
void nuiHotKeyImpl<T>::SetPriority(bool set)
{
  mPriority = set;
}

template <typename T>
void nuiHotKeyImpl<T>::SetOnKeyUp(bool set)
{
  mOnKeyUp = set;
}

template <typename T>
bool nuiHotKeyImpl<T>::HasPriority() const
{
  return mPriority;
}

template <typename T>
bool nuiHotKeyImpl<T>::IsOnKeyUp() const
{
  return mOnKeyUp;
}

template <typename T>
void nuiHotKeyImpl<T>::SetReferenceCount(uint refCount)
{
  mReferenceCount = refCount;
}

template <typename T>
uint nuiHotKeyImpl<T>::GetReferenceCount() const
{
  return mReferenceCount;
}

template <typename T>
void nuiHotKeyImpl<T>::Register()
{
  mReferenceCount++;
}

template <typename T>
bool nuiHotKeyImpl<T>::Release()
{
  NGL_ASSERT(mReferenceCount);
  return !(--mReferenceCount);
}

template <typename T>
void nuiHotKeyImpl<T>::SetParams(T Trigger, nuiKeyModifier Modifiers, bool Priority /*= false*/, bool FireOnKeyUp /*= false*/, bool Enabled /* true */)
{
  mTrigger = Trigger;
  mModifiers = Modifiers;
  mPriority = Priority;
  mOnKeyUp = FireOnKeyUp;
  mEnabled = Enabled;
}


#endif // __nuiHotKey_h__


