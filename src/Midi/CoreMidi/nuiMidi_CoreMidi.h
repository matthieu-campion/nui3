/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiMidi.h"
#include "nglThread.h"
#include "nuiSingleton.h"
#import <CoreMidi/CoreMidi.h>

class nuiMidiInPort_CoreMidi : public nuiMidiInPort
{
public:
  
  virtual ~nuiMidiInPort_CoreMidi();
  
  virtual bool Open(nuiMidiProcessFn pProcessFunction);
  virtual bool Close();
  
  static nuiMidiInPort_CoreMidi* GetPort(MIDIClientRef pClient, int32 id);
  
protected:
  nuiMidiInPort_CoreMidi(MIDIClientRef pClient, int32 id);
  int32 mPortID;
  MIDIClientRef mpClient;
  MIDIPortRef mpPort;
  MIDIEndpointRef mpSource;
  nuiMidiProcessFn mpProcessFunction;

  friend void nuiCoreMidiReadProc(const MIDIPacketList *pktlist, void *refCon, void *connRefCon);
  void ReadProc(const MIDIPacketList *pktlist);

  
  static std::map<int32, nuiMidiInPort_CoreMidi*> mPorts;
};


class nuiMidiOutPort_CoreMidi : public nuiMidiOutPort
{
public:
  
  virtual ~nuiMidiOutPort_CoreMidi();
  
  virtual bool Open();
  virtual bool Close();
  virtual bool Send(const uint8* pData, int32 size);

  static nuiMidiOutPort_CoreMidi* GetPort(MIDIClientRef pClient, int32 id);
  
protected:
  nuiMidiOutPort_CoreMidi(MIDIClientRef pClient, int32 id);
  int32 mPortID;
  MIDIClientRef mpClient;
  MIDIPortRef mpPort;
  MIDIEndpointRef mpDestination;

  static std::map<int32, nuiMidiOutPort_CoreMidi*> mPorts;
};




class nuiMidiPortAPI_CoreMidi : public nuiMidiPortAPI
{
public:
  nuiMidiPortAPI_CoreMidi();
  virtual ~nuiMidiPortAPI_CoreMidi();
  
  virtual int32 GetInPortCount() const;
  virtual int32 GetOutPortCount() const;
  virtual nuiMidiInPort* GetInPort(int32 index);
  virtual nuiMidiOutPort* GetOutPort(int32 index);
  
protected:
  mutable std::vector<int32> mDeviceIDs;
  MIDIClientRef mpMidiClientRef;
};

extern nuiSingletonHolder<nuiMidiPortAPI_CoreMidi> CoreMidiAPI;



