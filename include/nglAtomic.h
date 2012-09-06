/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once



/////////////////////////////// Atomic.h 
#ifdef _WIN32_
typedef volatile uint32 nglAtomic32;

// read atomic variable
inline uint32 ngl_atomic_read(const nglAtomic32 &value)
{
  return value;
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic32& value, uint32 oldValue, uint32 newValue)
{
  uint32 res = InterlockedCompareExchange((volatile LONG*)&value, newValue, oldValue);
  return (res == oldValue);
}



// set atomic variable
inline void ngl_atomic_set(nglAtomic32& value, uint32 newValue)
{
  InterlockedExchange((volatile LONG*)&value, newValue);
}

// add integer to atomic variable
inline void ngl_atomic_add(nglAtomic32& value, uint32 addValue)
{
  InterlockedExchangeAdd((volatile LONG*)&value, addValue);
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic32& value, uint32 subValue)
{
  InterlockedExchangeAdd((volatile LONG*)&value, (uint32)-(int32)subValue);
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic32 &value)
{
  InterlockedIncrement((volatile LONG*)&value);
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic32 &value)
{
  InterlockedDecrement((volatile LONG*)&value);
}


#ifdef __NUI64__
typedef volatile uint64 nglAtomic64;
#pragma warning(push) 
#pragma warning(disable : 4035) // disable no-return warning 
inline unsigned __int64 nuiInterlockedCompareExchange64(volatile unsigned __int64 *dest, unsigned __int64 exchange, unsigned __int64 comperand) 
{
#if 0
  //value returned in eax::edx 
  __asm
  {
    lea esi,comperand; 
    lea edi,exchange; 
    mov eax,[esi]; 
    mov edx,4[esi]; 
    mov ebx,[edi]; 
    mov ecx,4[edi]; 
    mov esi,dest; 
    //lock CMPXCHG8B [esi] is equivalent to the following except 
    //that it's atomic: 
    //ZeroFlag = (edx:eax == *esi); 
    //if (ZeroFlag) *esi = ecx:ebx; 
    //else edx:eax = *esi; 
    lock CMPXCHG8B [esi]; 
  }
#else
  return InterlockedCompareExchange64((volatile LONG64*)dest, exchange, comperand);
#endif

} 

#pragma warning(pop) 

// read atomic variable
inline uint64 ngl_atomic_read(const nglAtomic64 &value)
{
  return value;
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic64& value, uint64 oldValue, uint64 newValue)
{
  uint64 res = nuiInterlockedCompareExchange64((volatile unsigned __int64 *)&value, newValue, oldValue);
  return (res == oldValue);
}

// set atomic variable
inline void ngl_atomic_set(nglAtomic64& value, uint64 newValue)
{
  uint64 val;
  do
  {
    val = value;
  }
  while (!ngl_atomic_compare_and_swap(value, val, newValue));
  //value = newValue;
}

// add integer to atomic variable
inline void ngl_atomic_add(nglAtomic64& value, uint64 addValue)
{
  uint64 val;
  do
  {
    val = value;
  }
  while (!ngl_atomic_compare_and_swap(value, val, val + addValue));
  //value += addValue;
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic64& value, uint64 subValue)
{
  uint64 val;
  do
  {
    val = value;
  }
  while (!ngl_atomic_compare_and_swap(value, val, val - subValue));
  //value -= subValue;
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic64 &value)
{
  ngl_atomic_add(value, 1);
  //value++;
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic64 &value)
{
  ngl_atomic_sub(value, 1);
  //value--;
}
#endif // 64bit

#endif /* _WIN32_ */

#ifdef __APPLE__
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4) 


#include <libkern/OSAtomic.h> 

typedef volatile int32_t nglAtomic32;

// read atomic variable
inline uint32 ngl_atomic_read(const nglAtomic32 &value)
{
  return value;
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic32& value, uint32 oldValue, uint32 newValue)
{
  return OSAtomicCompareAndSwap32Barrier(oldValue, newValue, (int32_t*)&value);
}


// set atomic variable
inline void ngl_atomic_set(nglAtomic32& value, uint32 newValue)
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
inline void ngl_atomic_add(nglAtomic32& value, uint32 addValue)
{
  OSAtomicAdd32(addValue, (int32_t*)&value);
  //value += addValue;
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic32& value, uint32 subValue)
{
  OSAtomicAdd32(-subValue, (int32_t*)&value);
  //value -= subValue;
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic32 &value)
{
  OSAtomicAdd32(1, (int32_t*)&value);
  //OSIncrementAtomic(&value);
  //value++;
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic32 &value)
{
  OSAtomicAdd32(-1, (int32_t*)&value);
  //OSDecrementAtomic(&value);
  //value--;
}


#ifdef __NUI64__
typedef volatile int64_t nglAtomic64;

// 64 bit:
inline uint64 ngl_atomic_read(const nglAtomic64 &value)
{
  return value;
}


// compare and swap atomic variable 64bits
inline bool ngl_atomic_compare_and_swap(nglAtomic64& value, uint64 oldValue, uint64 newValue)
{
  return OSAtomicCompareAndSwap64Barrier(oldValue, newValue, (int64_t*)&value);
}


// set atomic variable
inline void ngl_atomic_set(nglAtomic64& value, uint64 newValue)
{
  uint64 val;
  do
  {
    val = value;
  }
  while (!OSAtomicCompareAndSwap64Barrier(val, newValue, (int64_t*)&value));
  //value = newValue;
}

// add integer to atomic variable
inline void ngl_atomic_add(nglAtomic64& value, uint64 addValue)
{
  OSAtomicAdd64(addValue, (int64_t*)&value);
  //value += addValue;
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic64& value, uint64 subValue)
{
  OSAtomicAdd64(-subValue, (int64_t*)&value);
  //value -= subValue;
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic64 &value)
{
  OSAtomicAdd64(1, (int64_t*)&value);
  //OSIncrementAtomic(&value);
  //value++;
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic64 &value)
{
  OSAtomicAdd64(-1, (int64_t*)&value);
  //OSDecrementAtomic(&value);
  //value--;
}
#endif // 64bits

#else
#include <CoreServices/CoreServices.h>

typedef volatile SInt32 nglAtomic32;

// 32 bit:
// read atomic variable
inline uint32 ngl_atomic_read(const nglAtomic32 &value)
{
  return value;
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic32& value, uint32 oldValue, uint32 newValue)
{
  return OTCompareAndSwap32Barrier(oldValue, newValue, (int32_t*)&value);
}


// set atomic variable
inline void ngl_atomic_set(nglAtomic32& value, uint32 newValue)
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
inline void ngl_atomic_add(nglAtomic32& value, uint32 addValue)
{
  OTAtomicAdd32(addValue, (SInt32*)&value);
  //value += addValue;
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic32& value, uint32 subValue)
{
  OTAtomicAdd32(-subValue, (SInt32*)&value);
  //value -= subValue;
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic32 &value)
{
  OTAtomicAdd32(1, (SInt32*)&value);
  //OSIncrementAtomic(&value);
  //value++;
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic32 &value)
{
  OTAtomicAdd32(-1, (SInt32*)&value);
  //OSDecrementAtomic(&value);
  //value--;
}

#ifdef __NUI64__
// 64 bit:
// read atomic variable
inline uint64 ngl_atomic_read(const nglAtomic64 &value)
{
  return value;
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic64& value, uint64 oldValue, uint64 newValue)
{
  return OTCompareAndSwap64Barrier(oldValue, newValue, (int64_t*)&value);
}


// set atomic variable
inline void ngl_atomic_set(nglAtomic64& value, uint64 newValue)
{
  uint64 val;
  do
  {
    val = value;
  }
  while (!OTCompareAndSwap64Barrier(val, newValue, (UInt64*)&value));
  //value = newValue;
}

// add integer to atomic variable
inline void ngl_atomic_add(nglAtomic64& value, uint64 addValue)
{
  OTAtomicAdd64(addValue, (SInt64*)&value);
  //value += addValue;
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic64& value, uint64 subValue)
{
  OTAtomicAdd64(-subValue, (SInt64*)&value);
  //value -= subValue;
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic64 &value)
{
  OTAtomicAdd64(1, (SInt64*)&value);
  //OSIncrementAtomic(&value);
  //value++;
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic64 &value)
{
  OTAtomicAdd64(-1, (SInt64*)&value);
  //OSDecrementAtomic(&value);
  //value--;
}
#endif // 64bits

#endif
//#endif /* __APPLE__ */

#elif defined _LINUX_ || defined _MINUI3_
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

 typedef volatile uint32 nglAtomic32;

 // read atomic variable
 inline uint32 ngl_atomic_read(const nglAtomic32 &value)
 {
   return value;
 }

 // set atomic variable
 inline void ngl_atomic_set(nglAtomic32& value, uint32 newValue)
 {
   InterlockedExchange((int32 *)&value, newValue);
 }

 // add integer to atomic variable
 inline void ngl_atomic_add(nglAtomic32& value, uint32 addValue)
 {
   InterlockedExchangeAdd((int32 *)&value, addValue);
 }

 // subtract the atomic variable
 inline void ngl_atomic_sub(nglAtomic32& value, uint32 subValue)
 {
   InterlockedExchangeAdd((int32 *)&value, -subValue);
 }

 // increment atomic variable
 inline void ngl_atomic_inc(nglAtomic32 &value)
 {
   ngl_atomic_add(value, 1);
 }

 // decrement atomic variable
 inline void ngl_atomic_dec(nglAtomic32 &value)
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
 inline bool ngl_atomic_compare_and_swap(nglAtomic32& value, uint oldValue, uint newValue)
 {
  uint32 res = InterlockedCompareExchange(&value, newValue, oldValue);
  return (res == oldValue);
 }

#ifdef __NUI64__
// 64bits #TODO Implement 64 bits CAS
// 64 bit:
// read atomic variable
typedef volatile int64_t nglAtomic64;

inline uint64 ngl_atomic_read(const nglAtomic64 &value)
{
  return value;
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic64& value, uint64 oldValue, uint64 newValue)
{
  return __sync_val_compare_and_swap((int64_t*)&value, oldValue, newValue);
}


// set atomic variable
inline void ngl_atomic_set(nglAtomic64& value, uint64 newValue)
{
  __sync_lock_test_and_set((int64_t*)&value, newValue);
}

// add integer to atomic variable
inline void ngl_atomic_add(nglAtomic64& value, uint64 addValue)
{
  __sync_fetch_and_add((int64_t*)&value, addValue);
  //value += addValue;
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic64& value, uint64 subValue)
{
  __sync_fetch_and_sub((int64_t*)&value, subValue);
  //value -= subValue;
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic64 &value)
{
  ngl_atomic_add(value, 1);
  //value++;
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic64 &value)
{
  ngl_atomic_sub(value, 1);
  //value--;
}

#endif
#endif /* _LINUX_ */


#ifdef _ANDROID_
inline uint32 InterlockedExchangeAdd(int32* Addend, uint32 Increment)
{
  //#FIXME Implement correct interlocked exchange add
  return *Addend += Increment;
}

inline uint32 InterlockedExchange(int32* volatile Target, uint32 Value)
{
  //#FIXME Implement correct interlocked exchange
  return *Target = Value;
}

typedef volatile uint32 nglAtomic32;

// read atomic variable
inline uint32 ngl_atomic_read(const nglAtomic32 &value)
{
  return value;
}

// set atomic variable
inline void ngl_atomic_set(nglAtomic32& value, uint32 newValue)
{
  InterlockedExchange((int32 *)&value, newValue);
}

// add integer to atomic variable
inline void ngl_atomic_add(nglAtomic32& value, uint32 addValue)
{
  InterlockedExchangeAdd((int32 *)&value, addValue);
}

// subtract the atomic variable
inline void ngl_atomic_sub(nglAtomic32& value, uint32 subValue)
{
  InterlockedExchangeAdd((int32 *)&value, -subValue);
}

// increment atomic variable
inline void ngl_atomic_inc(nglAtomic32 &value)
{
  ngl_atomic_add(value, 1);
}

// decrement atomic variable
inline void ngl_atomic_dec(nglAtomic32 &value)
{
  ngl_atomic_sub(value, 1);
}

inline uint32 InterlockedCompareExchange(volatile uint32 *dest, long exch, long comp)
{
  //#FIXME Implement correct interlocked exchange
  uint32 old = *dest;
  if (old == comp)
    *dest = exch;
  return(old);
}

// compare and swap atomic variable
inline bool ngl_atomic_compare_and_swap(nglAtomic32& value, uint oldValue, uint newValue)
{
  uint32 res = InterlockedCompareExchange(&value, newValue, oldValue);
  return (res == oldValue);
}
#endif /* _ANDROID_ */


#ifdef __NUI64__
typedef volatile nglAtomic64 nglAtomic;
#else
typedef volatile nglAtomic32 nglAtomic;
#endif

