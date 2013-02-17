/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

//class nuiMidiPort
nuiMidiPort::nuiMidiPort()
{
  mIsPresent = false;
  mIsOpen = false;
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
  
bool nuiMidiPort::IsOpen() const
{
  return mIsOpen;
}

void nuiMidiPortAPI::RegisterWithManager(nuiMidiManager& rManager)
{
  rManager.RegisterAPI(mName, this);
}


//class nuiMidiManager
nuiMidiManager& nuiMidiManager::Get()
{
  static nuiSingletonHolder<nuiMidiManager> manager;
  return *manager.Instance();
}

nuiMidiManager::nuiMidiManager()
{
  RegisterAPIS();
}

nuiMidiManager::~nuiMidiManager()
{
}

void nuiMidiManager::Update()
{
  for (int32 i = 0; i < mInputs.size(); i++)
    mInputs[i]->Release();
  mInputs.clear();
  for (int32 i = 0; i < mOutputs.size(); i++)
    mOutputs[i]->Release();
  mOutputs.clear();
  
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    int32 incount = it->second->GetInPortCount();
    for (int32 i = 0; i < incount; i++)
    {
      nuiMidiInPort* pPort = it->second->GetInPort(i);
      pPort->Acquire();
      mInputs.push_back(pPort);
    }
    int32 outcount = it->second->GetOutPortCount();
    for (int32 i = 0; i < outcount; i++)
    {
      nuiMidiOutPort* pPort = it->second->GetOutPort(i);
      pPort->Acquire();
      mOutputs.push_back(pPort);
    }
  }
}

int32 nuiMidiManager::GetInPortCount() const
{
  return mInputs.size();
}

int32 nuiMidiManager::GetOutPortCount() const
{
  return mOutputs.size();
}

nuiMidiInPort* nuiMidiManager::GetInPort(int32 PortIndex)
{
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiMidiPortAPI* pAPI = it->second;
    int32 devcount = pAPI->GetInPortCount();
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

nuiMidiOutPort* nuiMidiManager::GetOutPort(int32 PortIndex)
{
  nuiMidiAPIMap::const_iterator end = mAPIs.end();
  for (nuiMidiAPIMap::const_iterator it = mAPIs.begin(); it != end; ++it)
  {
    nuiMidiPortAPI* pAPI = it->second;
    int32 devcount = pAPI->GetOutPortCount();
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
    CoreMidiAPI.Instance()->RegisterWithManager(*this);
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
