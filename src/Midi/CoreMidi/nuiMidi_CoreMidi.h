/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMidi.h"
#include "nglThread.h"
#import <CoreMidi/CoreMidi.h>

class nuiMidiInPort_CoreMidi : public nuiMidiInPort
{
public:
  
  virtual ~nuiMidiInPort_CoreMidi();
  
  virtual bool Open(nuiMidiProcessFn pProcessFunction);
  virtual bool Close();
  
  static nuiMidiInPort_CoreMidi* GetPort(uint32 id);
  
protected:
  nuiMidiInPort_CoreMidi(int32 id);
  int32 mPortID;
  static std::map<uint32, nuiMidiInPort_CoreMidi*> mPorts;
};


class nuiMidiOutPort_CoreMidi : public nuiMidiOutPort
{
public:
  
  virtual ~nuiMidiOutPort_CoreMidi();
  
  virtual bool Open();
  virtual bool Close();

  static nuiMidiOutPort_CoreMidi* GetPort(uint32 id);
  
protected:
  nuiMidiOutPort_CoreMidi(int32 id);
  int32 mPortID;
  static std::map<uint32, nuiMidiOutPort_CoreMidi*> mPorts;
};




class nuiMidiPortAPI_CoreMidi : public nuiMidiPortAPI
{
public:
  nuiMidiPortAPI_CoreMidi();
  virtual ~nuiMidiPortAPI_CoreMidi();
  
  virtual uint32 GetInPortCount() const;
  virtual uint32 GetOutPortCount() const;
  virtual nuiMidiInPort* GetInPort(uint32 index);
  virtual nuiMidiOutPort* GetOutPort(uint32 index);
  
protected:
  mutable std::vector<int32> mDeviceIDs;
  MIDIClientRef* mpMidiClient;
};

extern nuiMidiPortAPI_CoreMidi CoreMidiAPI;



