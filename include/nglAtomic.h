/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once



/////////////////////////////// Atomic.h 
#ifdef _WIN32_
   typedef volatile uint32 nglAtomic;

   // read atomic variable
   inline uint32 ngl_atomic_read(const nglAtomic &value)
   {
     return value;
   }

     // compare and swap atomic variable
    inline bool ngl_atomic_compare_and_swap(nglAtomic& value, uint32 oldValue, uint32 newValue)
    {
      uint32 res = InterlockedCompareExchange((volatile LONG*)&value, newValue, oldValue);
      return (res == oldValue);
    }


   // set atomic variable
   inline void ngl_atomic_set(nglAtomic& value, uint32 newValue)
   {
     InterlockedExchange((volatile LONG*)&value, newValue);
   }

   // add integer to atomic variable
   inline void ngl_atomic_add(nglAtomic& value, uint32 addValue)
   {
     InterlockedExchangeAdd((volatile LONG*)&value, addValue);
   }

   // subtract the atomic variable
   inline void ngl_atomic_sub(nglAtomic& value, uint32 subValue)
   {
     InterlockedExchangeAdd((volatile LONG*)&value, (uint32)-(int32)subValue);
   }

   // increment atomic variable
   inline void ngl_atomic_inc(nglAtomic &value)
   {
     InterlockedIncrement((volatile LONG*)&value);
   }

   // decrement atomic variable
   inline void ngl_atomic_dec(nglAtomic &value)
   {
     InterlockedDecrement((volatile LONG*)&value);
   }
#endif /* _WIN32_ */

#ifdef __APPLE__
  #if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4) 


    #include <libkern/OSAtomic.h> 

    typedef volatile int32_t nglAtomic;

    // read atomic variable
    inline uint32 ngl_atomic_read(const nglAtomic &value)
    {
      return value;
    }


    // compare and swap atomic variable
    inline bool ngl_atomic_compare_and_swap(nglAtomic& value, uint32 oldValue, uint32 newValue)
    {
      return OSAtomicCompareAndSwap32Barrier(oldValue, newValue, (int32_t*)&value);
    }


    // set atomic variable
    inline void ngl_atomic_set(nglAtomic& value, uint32 newValue)
    {
      uint32 val;
      do
      {
        val = value;
      }
      while (!OSAtomicCompareAndSwap32Barrier(val, newValue, (int32_t*)&value));
      //value = newValue;
    }

    // add integer to atomic variable
    inline void ngl_atomic_add(nglAtomic& value, uint32 addValue)
    {
      OSAtomicAdd32(addValue, (int32_t*)&value);
      //value += addValue;
    }

    // subtract the atomic variable
    inline void ngl_atomic_sub(nglAtomic& value, uint32 subValue)
    {
      OSAtomicAdd32(-subValue, (int32_t*)&value);
      //value -= subValue;
    }

    // increment atomic variable
    inline void ngl_atomic_inc(nglAtomic &value)
    {
      OSAtomicAdd32(1, (int32_t*)&value);
      //OSIncrementAtomic(&value);
      //value++;
    }

    // decrement atomic variable
    inline void ngl_atomic_dec(nglAtomic &value)
    {
      OSAtomicAdd32(-1, (int32_t*)&value);
      //OSDecrementAtomic(&value);
      //value--;
    }
#else
    #include <CoreServices/CoreServices.h>

    typedef volatile SInt32 nglAtomic;

    // read atomic variable
    inline uint32 ngl_atomic_read(const nglAtomic &value)
    {
      return value;
    }

    // compare and swap atomic variable
    inline bool ngl_atomic_compare_and_swap(nglAtomic& value, uint32 oldValue, uint32 newValue)
    {
      return OTCompareAndSwap32Barrier(oldValue, newValue, (int32_t*)&value);
    }


    // set atomic variable
    inline void ngl_atomic_set(nglAtomic& value, uint32 newValue)
    {
      uint32 val;
      do
      {
        val = value;
      }
      while (!OTCompareAndSwap32Barrier(val, newValue, (UInt32*)&value));
      //value = newValue;
    }

    // add integer to atomic variable
    inline void ngl_atomic_add(nglAtomic& value, uint32 addValue)
    {
      OTAtomicAdd32(addValue, (SInt32*)&value);
      //value += addValue;
    }

    // subtract the atomic variable
    inline void ngl_atomic_sub(nglAtomic& value, uint32 subValue)
    {
      OTAtomicAdd32(-subValue, (SInt32*)&value);
      //value -= subValue;
    }

    // increment atomic variable
    inline void ngl_atomic_inc(nglAtomic &value)
    {
      OTAtomicAdd32(1, (SInt32*)&value);
      //OSIncrementAtomic(&value);
      //value++;
    }

    // decrement atomic variable
    inline void ngl_atomic_dec(nglAtomic &value)
    {
      OTAtomicAdd32(-1, (SInt32*)&value);
      //OSDecrementAtomic(&value);
      //value--;
    }
  #endif
#endif /* __APPLE__ */

#ifdef _LINUX_
	inline uint32 InterlockedExchangeAdd(int32* Addend, uint32 Increment)
	{
		uint32 ret;
		__asm__ (
			/* lock for SMP systems */
			"lock\n\t"
			"xaddl %0,(%1)"
			:"=r" (ret)
			:"r" (Addend), "0" (Increment)
			:"memory" );
		return ret;
	}

	inline uint32 InterlockedExchange(int32* volatile Target, uint32 Value)
	{
		uint32 ReturnValue;
		
		__asm __volatile(
			"lock xchgl %2,(%1)"
			: "=r" (ReturnValue)
			: "r" (Target), "0" (Value)
			: "memory");
		
		return ReturnValue;
	}

   typedef volatile uint32 nglAtomic;

   // read atomic variable
   inline uint32 ngl_atomic_read(const nglAtomic &value)
   {
     return value;
   }

   // set atomic variable
   inline void ngl_atomic_set(nglAtomic& value, uint32 newValue)
   {
     InterlockedExchange((int32 *)&value, newValue);
   }

   // add integer to atomic variable
   inline void ngl_atomic_add(nglAtomic& value, uint32 addValue)
   {
     InterlockedExchangeAdd((int32 *)&value, addValue);
   }

   // subtract the atomic variable
   inline void ngl_atomic_sub(nglAtomic& value, uint32 subValue)
   {
     InterlockedExchangeAdd((int32 *)&value, -subValue);
   }

   // increment atomic variable
   inline void ngl_atomic_inc(nglAtomic &value)
   {
     ngl_atomic_add(value, 1);
   }

   // decrement atomic variable
   inline void ngl_atomic_dec(nglAtomic &value)
   {
     ngl_atomic_sub(value, 1);
   }
   
   inline uint32 InterlockedCompareExchange(volatile uint32 *dest, long exch, long comp)
   {
   uint32 old;

   __asm__ __volatile__ ("lock; cmpxchgl %2, %0"
   : "=m" (*dest), "=a" (old)
   : "r" (exch), "m" (*dest), "a" (comp));
   return(old);
   }
   
   // compare and swap atomic variable
   inline bool ngl_atomic_compare_and_swap(nglAtomic& value, uint oldValue, uint newValue)
   {
    uint32 res = InterlockedCompareExchange(&value, newValue, oldValue);
    return (res == oldValue);
   }
  
#endif /* _LINUX_ */
