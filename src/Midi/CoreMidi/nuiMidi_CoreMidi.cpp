/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiMidi_CoreMidi.h"


//class nuiMidiInPort_CoreMidi
nuiMidiInPort_CoreMidi::nuiMidiInPort_CoreMidi(int32 id)
{
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
  
}

nuiMidiOutPort* nuiMidiPortAPI_CoreMidi::GetOutPort(uint32 index)
{
  
}

nuiMidiInPort* nuiMidiPortAPI_CoreMidi::GetInPort(const nglString& rDeviceName)
{
  
}

nuiMidiOutPort* nuiMidiPortAPI_CoreMidi::GetOutPort(const nglString& rDeviceName)
{
  
}

nglString nuiMidiPortAPI_CoreMidi::GetInPortName(uint32 index) const
{
  
}

nglString nuiMidiPortAPI_CoreMidi::GetOutPortName(uint32 index) const
{
  
}

nuiMidiInPort* nuiMidiPortAPI_CoreMidi::GetDefaultInPort()
{
  
}

nuiMidiOutPort* nuiMidiPortAPI_CoreMidi::GetDefaultOutPort()
{
  
}

nuiMidiPortAPI_CoreMidi CoreMidiAPI;

