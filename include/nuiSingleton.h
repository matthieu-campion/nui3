/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
*/

#pragma once


template <class T>	class nuiSingleton
{
public:

  static T* Instance()
  { 
    if (!mpInstance)	
      mpInstance = new T;		
    
    return mpInstance; 
  };
  
  static void DestroyInstance()
  { 
    delete mpInstance;
    mpInstance = NULL;
  };

protected:

  Singleton()
  {};
  
  virtual ~Singleton()
  {};

private:

  Singleton(const Singleton& source)
  {};

  // Singleton class instance
  static T* mpInstance;
};


// Static member initialisation
template <class T>	T* nuiSingleton<T>::mpInstance = NULL;


