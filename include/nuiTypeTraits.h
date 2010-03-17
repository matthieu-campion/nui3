/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

template <int> struct static_assert_NUI {};
template <bool> struct STATIC_ASSERTION_FAILED;
template <> struct STATIC_ASSERTION_FAILED<true> {};

#define _NUI_JOIN(x,y) _NUI_DO_JOIN(x,y)
#define _NUI_DO_JOIN(x,y) _NUI_DO_JOIN2(x,y)
#define _NUI_DO_JOIN2(x,y) x##y

#define NUI_STATIC_ASSERT(condition) \
typedef ::static_assert_NUI<sizeof (::STATIC_ASSERTION_FAILED<(bool)(condition)>)> \
_NUI_JOIN(NUI_static_assert_line_, __LINE__)


template <typename _Type>
struct remove_const { typedef _Type type; };

template <typename _Type>
struct remove_const<_Type const> { typedef _Type type; };

template <typename _Type>
struct remove_volatile { typedef _Type type; };

template <typename _Type>
struct remove_volatile<_Type volatile> { typedef _Type type; };

template <typename _Type>
struct remove_cv
{
  typedef typename remove_const<typename remove_volatile<_Type>::type>::type type;
};

template <typename _Type>
struct remove_pointer
{
  typedef _Type type;
};

#define _RP_SPECIALIZE(_ModifiedType) \
template <typename _Type> \
struct remove_pointer<_ModifiedType> \
{ \
typedef _Type type; \
};

_RP_SPECIALIZE(_Type *)
_RP_SPECIALIZE(_Type * const)
_RP_SPECIALIZE(_Type * volatile)
_RP_SPECIALIZE(_Type * const volatile)

#undef _RP_SPECIALIZE

template <typename _Type, _Type _Value>
struct integral_constant
{
  static const _Type value = _Value;
  typedef _Type value_type;
  typedef integral_constant<_Type, _Value> type;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <typename, typename>
struct is_same : public false_type { };

template <typename _Type>
struct is_same<_Type, _Type> : public true_type { };

template <typename _Type>
struct _is_class_or_union_helper
{
private:
  typedef char _one;
  typedef struct { char _c[2]; } _two;
  
public:
  template <typename _T>
  static _one _test (int _T::*);
  template <typename>
  static _two _test (...);
  
  static const bool _value = sizeof (_test<_Type>(0)) == sizeof (_one);
};

template <typename _Type>
struct _is_class_or_union : public integral_constant<bool, _is_class_or_union_helper<_Type>::_value>
{
};

template <typename _Base, typename _Derived>
struct _is_base_of_helper
{
private:
  // Disallow incomplete types.
  NUI_STATIC_ASSERT(sizeof (_Base) != 0);
  NUI_STATIC_ASSERT(sizeof (_Derived) != 0);
  
  typedef typename remove_cv<_Base>::type _RCV_Base;
  typedef typename remove_cv<_Derived>::type _RCV_Derived;
  
  typedef char _one;
  typedef struct { char _c[2]; } _two;
  
public:
  template <typename _T>
  static _one _test (_RCV_Derived &, _T);
  static _two _test (_RCV_Base &, int);
  
  struct _convert
  {
    operator _RCV_Base & () const;
    operator _RCV_Derived & ();
  };
  
  static const bool _value = (_is_class_or_union<_Base>::value &&
                              _is_class_or_union<_Derived>::value &&
                              (sizeof (_test(_convert(), 0)) == sizeof (_one) ||
                               is_same<_Base, _Derived>::value));
};

template <typename _Base, typename _Derived>
struct is_base_of :
public integral_constant<bool, _is_base_of_helper<_Base, _Derived>::_value>
{
};

template <class T>
struct is_pointer
{
	enum { value = false };
};

template <class T>
struct is_pointer<T*>
{
	enum { value = true };
};

template <bool condition, class T1, class T2>
struct if_ { };

template <class T1, class T2>
struct if_<true, T1, T2>
{
	typedef T1 type;
};

template <class T1, class T2>
struct if_<false, T1, T2>
{
	typedef T2 type;
};


