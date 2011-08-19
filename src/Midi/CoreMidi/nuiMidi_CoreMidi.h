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
  nuiMidiInPort_CoreMidi(int32 id);
  
  virtual ~nuiMidiInPort_CoreMidi();
  
  virtual bool Open(nuiMidiProcessFn pProcessFunction);
  virtual bool Close();
  
  
protected:
  int32 mPortID;
};


class nuiMidiOutPort_CoreMidi : public nuiMidiOutPort
{
public:
  nuiMidiOutPort_CoreMidi(int32 id);
  
  virtual ~nuiMidiOutPort_CoreMidi();
  
  virtual bool Open();
  virtual bool Close();
  
  
protected:
  int32 mPortID;
};




class nuiMidiPortAPI_CoreMidi : public nuiMidiPortAPI
{
public:
  nuiMidiPortAPI_CoreMidi();
  virtual ~nuiMidiPortAPI_CoreMidi();
  
  virtual uint32 GetInPortCount() const;
  virtual uint32 GetOutPortCount() const;
  virtual nglString GetInPortName(uint32 index) const;
  virtual nglString GetOutPortName(uint32 index) const;
  virtual nuiMidiInPort* GetInPort(uint32 index);
  virtual nuiMidiInPort* GetInPort(const nglString& rPortName);
  virtual nuiMidiOutPort* GetOutPort(uint32 index);
  virtual nuiMidiOutPort* GetOutPort(const nglString& rPortName);
  virtual nuiMidiInPort* GetDefaultInPort();
  virtual nuiMidiOutPort* GetDefaultOutPort();
  
protected:
  mutable std::vector<int32> mDeviceIDs;
  MIDIClientRef* mpMidiClient;
};

extern nuiMidiPortAPI_CoreMidi CoreMidiAPI;



