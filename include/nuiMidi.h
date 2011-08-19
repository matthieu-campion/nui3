/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiMidiManager;

typedef nuiFastDelegate2<const uint8*, uint32> nuiMidiProcessFn; // the params are the pointer to the data and the size of the data in bytes

class nuiMidiPort
{
public:
  virtual ~nuiMidiPort();
  
  virtual bool Close() = 0;
  
  const nglString& GetName() const; ///< Return the name of this port.
  const nglString& GetDeviceName() const; ///< Return the name of the device containing this port.
  const nglString& GetManufacturer() const; ///< Return the manufacturer of this port.

  bool IsPresent() const; ///< Return true if the device is connected to the system and online.
  
protected:
  nuiMidiPort();
  
  nglString mName;
  nglString mDeviceName;
  nglString mManufacturer;
  bool mIsPresent;
};

class nuiMidiInPort : public nuiMidiPort
{
public:
  virtual ~nuiMidiInPort();
  
  virtual bool Open(nuiMidiProcessFn pProcessFunction) = 0;
  virtual bool Close() = 0;
  
protected:
  nuiMidiInPort();
};

class nuiMidiOutPort : public nuiMidiPort
{
public:
  virtual ~nuiMidiOutPort();
  
  virtual bool Open() = 0;
  virtual bool Close() = 0;
  
protected:
  nuiMidiOutPort();
};

class nuiMidiPortAPI : public nuiNonCopyable
{
public:
  virtual ~nuiMidiPortAPI();
  
  const nglString& GetAPIName() const
  {
    return mName;
  }

  virtual uint32 GetInPortCount() const = 0;
  virtual uint32 GetOutPortCount() const = 0;
  virtual nglString GetInPortName(uint32 index) const = 0;
  virtual nglString GetOutPortName(uint32 index) const = 0;
  virtual nuiMidiInPort* GetInPort(uint32 index) = 0;
  virtual nuiMidiInPort* GetInPort(const nglString& rPortName) = 0;
  virtual nuiMidiOutPort* GetOutPort(uint32 index) = 0;
  virtual nuiMidiOutPort* GetOutPort(const nglString& rPortName) = 0;
  virtual nuiMidiInPort* GetDefaultInPort() = 0;
  virtual nuiMidiOutPort* GetDefaultOutPort() = 0;
  
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
  nglString GetInPortName(uint32 PortIndex) const;
  nglString GetOutPortName(uint32 PortIndex) const;
  nuiMidiInPort* GetInPort(uint32 PortIndex);
  nuiMidiOutPort* GetOutPort(uint32 PortIndex);

  nuiMidiInPort* GetDefaultInPort();
  nuiMidiOutPort* GetDefaultOutPort();
  
protected:
  friend void nuiMidiPortAPI::RegisterWithManager(nuiMidiManager& rManager);
  void RegisterAPIS();
  void RegisterAPI(const nglString& rAPIName, nuiMidiPortAPI* pAPI);
  nuiMidiAPIMap mAPIs;
  
  nuiMidiManager();
  int32 mInPortCount;
  int32 mOutPortCount;
};

