/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSingleton.h"

class nuiMidiManager;
class nuiMidiInPort;

typedef nuiFastDelegate4<nuiMidiInPort*, const uint8*, uint32, double> nuiMidiProcessFn; // the params are the pointer to the data, the size of the data in bytes and the time stamp

class nuiMidiPort : public nuiRefCount
{
public:
  virtual ~nuiMidiPort();
  
  virtual bool Close() = 0;
  
  const nglString& GetName() const; ///< Return the name of this port.
  const nglString& GetDeviceName() const; ///< Return the name of the device containing this port.
  const nglString& GetManufacturer() const; ///< Return the manufacturer of this port.

  bool IsPresent() const; ///< Return true if the device is connected to the system and online.
  bool IsOpen() const; ///< Return true if the device is open.
  
protected:
  nuiMidiPort();
  
  nglString mName;
  nglString mDeviceName;
  nglString mManufacturer;
  bool mIsPresent;
  bool mIsOpen;
};

class nuiMidiInPort : public nuiMidiPort
{
public:
  virtual ~nuiMidiInPort(){}
  
  virtual bool Open(nuiMidiProcessFn pProcessFunction) = 0;
  virtual bool Close() = 0;
  
protected:
  nuiMidiInPort(){}
};

class nuiMidiOutPort : public nuiMidiPort
{
public:
  virtual ~nuiMidiOutPort(){}
  
  virtual bool Open() = 0;
  virtual bool Close() = 0;

  virtual bool Send(const uint8* pData, uint32 size) = 0;
  
protected:
  nuiMidiOutPort(){}
};

class nuiMidiPortAPI : public nuiNonCopyable
{
public:
  virtual ~nuiMidiPortAPI() {}
  
  const nglString& GetAPIName() const
  {
    return mName;
  }

  virtual uint32 GetInPortCount() const = 0;
  virtual uint32 GetOutPortCount() const = 0;
  virtual nuiMidiInPort* GetInPort(uint32 index) = 0;
  virtual nuiMidiOutPort* GetOutPort(uint32 index) = 0;
  
  void RegisterWithManager(nuiMidiManager& rManager);
protected:
  nuiMidiPortAPI() {};
  
  nglString mName;
};

typedef std::map<nglString, nuiMidiPortAPI*> nuiMidiAPIMap;


class nuiMidiManager
{
public:
  static nuiMidiManager& Get();
  ~nuiMidiManager();
  
  void Update();
  
  nuiSimpleEventSource<0> DeviceConnected;
  nuiSimpleEventSource<0> DeviceDisconnected;
  
  uint32 GetInPortCount() const;
  uint32 GetOutPortCount() const;
  nuiMidiInPort* GetInPort(uint32 PortIndex);
  nuiMidiOutPort* GetOutPort(uint32 PortIndex);

protected:
  friend void nuiMidiPortAPI::RegisterWithManager(nuiMidiManager& rManager);
  friend class nuiSingletonHolder<nuiMidiManager>;
  void RegisterAPIS();
  void RegisterAPI(const nglString& rAPIName, nuiMidiPortAPI* pAPI);
  nuiMidiAPIMap mAPIs;
  
  nuiMidiManager();
  
  std::vector<nuiMidiInPort*> mInputs;
  std::vector<nuiMidiOutPort*> mOutputs;
};

