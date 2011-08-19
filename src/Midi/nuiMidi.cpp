/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiMidi.h"

//class nuiMidiPort
nuiMidiPort::nuiMidiPort()
{
  mIsPresent = false;
}

nuiMidiPort::~nuiMidiPort()
{
}
  
//bool nuiMidiPort::Open(nuiMidiProcessFn pProcessFunction)
//{
//  return false
//}
//
//bool nuiMidiPort::Close()
//{
//}
//  

const nglString& nuiMidiPort::GetName() const
{
  return mName;
}

const nglString& nuiMidiPort::GetDeviceName() const
{
  return mDeviceName;
}

const nglString& nuiMidiPort::GetManufacturer() const
{
  return mManufacturer;
}

bool nuiMidiPort::IsPresent() const
{
  return mIsPresent;
}
  

//class nuiMidiManager
nuiMidiManager& nuiMidiManager::Get()
{
  static nuiMidiManager manager;
  return manager;
}

nuiMidiManager::nuiMidiManager()
{
  mInPortCount = 0;
  mOutPortCount = 0;
  RegisterAPIS();
}

nuiMidiManager::~nuiMidiManager()
{
}

void nuiMidiManager::Update()
{
  mInPortCount = 0;
  mOutPortCount = 0;
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    uint32 incount = it->second->GetInPortCount();
    mInPortCount += incount;
    uint32 outcount = it->second->GetOutPortCount();
    mOutPortCount += outcount;
  }
}

uint32 nuiMidiManager::GetInPortCount() const
{
  return mInPortCount;
}

uint32 nuiMidiManager::GetOutPortCount() const
{
  return mOutPortCount;
}

nuiMidiInPort* nuiMidiManager::GetInPort(uint32 PortIndex)
{
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiMidiPortAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetInPortCount();
    if (PortIndex >= devcount)
    {
      PortIndex -= devcount;
    }
    else
    {
      return pAPI->GetInPort(PortIndex);
    }
  }
  NGL_ASSERT(0);
  return NULL;
}

nuiMidiOutPort* nuiMidiManager::GetOutPort(uint32 PortIndex)
{
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiMidiPortAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetOutPortCount();
    if (PortIndex >= devcount)
    {
      PortIndex -= devcount;
    }
    else
    {
      return pAPI->GetOutPort(PortIndex);
    }
  }
  NGL_ASSERT(0);
  return NULL;
}

nglString nuiMidiManager::GetInPortName(uint32 PortIndex) const
{
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiMidiPortAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetOutPortCount();
    if (PortIndex >= devcount)
    {
      PortIndex -= devcount;
    }
    else
    {
      return pAPI->GetInPortName(PortIndex);
    }
  }
  return nglString::Null;
}

nglString nuiMidiManager::GetOutPortName(uint32 PortIndex) const
{
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiMidiPortAPI* pAPI = it->second;
    uint32 devcount = pAPI->GetOutPortCount();
    if (PortIndex >= devcount)
    {
      PortIndex -= devcount;
    }
    else
    {
      return pAPI->GetOutPortName(PortIndex);
    }
  }
  return nglString::Null;
}

nuiMidiInPort* nuiMidiManager::GetDefaultInPort()
{
  return mAPIs.begin()->second->GetDefaultInPort();
}

nuiMidiOutPort* nuiMidiManager::GetDefaultOutPort()
{
  return mAPIs.begin()->second->GetDefaultOutPort();
}

#ifdef _WIN32_
//  #include "nuiMidi_Win32.h"
  void nuiMidiManager::RegisterAPIS()
  {
//    Win32MidiAPI.RegisterWithManager(*this);
  }
#elif (defined _CARBON_) || (defined _COCOA_) || (defined _UIKIT_)
  #include "nuiMidi_CoreMidi.h"
  void nuiMidiManager::RegisterAPIS()
  {
    CoreMidiAPI.RegisterWithManager(*this);
  }
#else
  void nuiMidiManager::RegisterAPIS()
  {
  }
#endif

void nuiMidiManager::RegisterAPI(const nglString& rAPIName, nuiMidiPortAPI* pAPI)
{
  NGL_LOG(_T("nuiMidiManager"), NGL_LOG_DEBUG, _T("RegisterAPI('%s') [0x%x]\n"), rAPIName.GetChars(), pAPI);
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  nuiMidiAPIMap::const_iterator it = mAPIs.find(rAPIName);
  if (it != end)
  {
    nuiMidiPortAPI* pOldAPI = it->second;
    NGL_LOG(_T("nuiMidiManager"), NGL_LOG_DEBUG, _T("\tkilling previous entry for this API [0x%p]\n"), pOldAPI);
    delete pOldAPI;
  }
  mAPIs[rAPIName] = pAPI;
  Update();
}
