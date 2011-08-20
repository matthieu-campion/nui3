/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiMidi_CoreMidi.h"

std::map<uint32, nuiMidiInPort_CoreMidi*> nuiMidiInPort_CoreMidi::mPorts;
std::map<uint32, nuiMidiOutPort_CoreMidi*> nuiMidiOutPort_CoreMidi::mPorts;

nuiMidiInPort_CoreMidi* nuiMidiInPort_CoreMidi::GetPort(uint32 id)
{
  nuiMidiInPort_CoreMidi* pPort = NULL;
  std::map<uint32, nuiMidiInPort_CoreMidi*>::iterator it = mPorts.find(id);
  if (it != mPorts.end())
    pPort = it->second;
  else
  {
    pPort = new nuiMidiInPort_CoreMidi(id);
    mPorts[id] = pPort;
  }
  
  pPort->Acquire();
  return pPort;
}

nuiMidiOutPort_CoreMidi* nuiMidiOutPort_CoreMidi::GetPort(uint32 id)
{
  nuiMidiOutPort_CoreMidi* pPort = NULL;
  std::map<uint32, nuiMidiOutPort_CoreMidi*>::iterator it = mPorts.find(id);
  if (it != mPorts.end())
    pPort = it->second;
  else
  {
    pPort = new nuiMidiOutPort_CoreMidi(id);
    mPorts[id] = pPort;
  }
  
  pPort->Acquire();
  return pPort;
}


//class nuiMidiInPort_CoreMidi
nuiMidiInPort_CoreMidi::nuiMidiInPort_CoreMidi(int32 id)
{
  CFStringRef pname, pmanuf, pmodel;
  char name[64], manuf[64], model[64];
  
  MIDIEndpointRef ep = MIDIGetSource(id);
  MIDIEntityRef ent;
  MIDIDeviceRef dev;
  MIDIEndpointGetEntity(ep, &ent);
  MIDIEntityGetDevice(ent, &dev);
  MIDIObjectGetStringProperty(ep, kMIDIPropertyName, &pname);
  MIDIObjectGetStringProperty(ep, kMIDIPropertyManufacturer, &pmanuf);
  MIDIObjectGetStringProperty(dev, kMIDIPropertyName, &pmodel);
  
  CFStringGetCString(pname, name, sizeof(name), 0);
  CFStringGetCString(pmanuf, manuf, sizeof(manuf), 0);
  CFStringGetCString(pmodel, model, sizeof(model), 0);
  CFRelease(pname);
  CFRelease(pmanuf);
  CFRelease(pmodel);
  
  mName = name;
  mManufacturer = manuf;
  mDeviceName = model;
  
  mIsPresent = true;
}

nuiMidiInPort_CoreMidi::~nuiMidiInPort_CoreMidi()
{
}


bool nuiMidiInPort_CoreMidi::Open(nuiMidiProcessFn pProcessFunction)
{
  
}

bool nuiMidiInPort_CoreMidi::Close()
{
  
}

//class nuiMidiOutPort_CoreMidi
nuiMidiOutPort_CoreMidi::nuiMidiOutPort_CoreMidi(int32 id)
{
  CFStringRef pname, pmanuf, pmodel;
  char name[64], manuf[64], model[64];
  
  MIDIEndpointRef ep = MIDIGetDestination(id);
  MIDIEntityRef ent;
  MIDIDeviceRef dev;
  MIDIEndpointGetEntity(ep, &ent);
  MIDIEntityGetDevice(ent, &dev);
  MIDIObjectGetStringProperty(ep, kMIDIPropertyName, &pname);
  MIDIObjectGetStringProperty(ep, kMIDIPropertyManufacturer, &pmanuf);
  MIDIObjectGetStringProperty(dev, kMIDIPropertyName, &pmodel);
  
  CFStringGetCString(pname, name, sizeof(name), 0);
  CFStringGetCString(pmanuf, manuf, sizeof(manuf), 0);
  CFStringGetCString(pmodel, model, sizeof(model), 0);
  CFRelease(pname);
  CFRelease(pmanuf);
  CFRelease(pmodel);
  
  mName = name;
  mManufacturer = manuf;
  mDeviceName = model;
  
  mIsPresent = true;
}

nuiMidiOutPort_CoreMidi::~nuiMidiOutPort_CoreMidi()
{
  
}

bool nuiMidiOutPort_CoreMidi::Open()
{
  
}

bool nuiMidiOutPort_CoreMidi::Close()
{
  
}

/////////////////////

nuiMidiPortAPI_CoreMidi::nuiMidiPortAPI_CoreMidi()
{
  
}

nuiMidiPortAPI_CoreMidi::~nuiMidiPortAPI_CoreMidi()
{
  
}

uint32 nuiMidiPortAPI_CoreMidi::GetInPortCount() const
{
  return MIDIGetNumberOfSources();
}

uint32 nuiMidiPortAPI_CoreMidi::GetOutPortCount() const
{
  return MIDIGetNumberOfDestinations();
}

nuiMidiInPort* nuiMidiPortAPI_CoreMidi::GetInPort(uint32 index)
{
  return nuiMidiInPort_CoreMidi::GetPort(index);
}

nuiMidiOutPort* nuiMidiPortAPI_CoreMidi::GetOutPort(uint32 index)
{
  return nuiMidiOutPort_CoreMidi::GetPort(index);
}

nuiMidiPortAPI_CoreMidi CoreMidiAPI;

