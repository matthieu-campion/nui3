// as_global.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script global functions implementation


#ifndef GAMESWF_AS_GLOBAL_H
#define GAMESWF_AS_GLOBAL_H

namespace gameswf
{
	void	as_global_trace(const fn_call& fn);
	void	as_global_get_version(const fn_call& fn);
	void	as_global_assetpropflags(const fn_call& fn);
  void  as_global_setinterval(const fn_call& fn);
  void  as_global_clearinterval(const fn_call& fn);


}	// end namespace gameswf


#endif // GAMESWF_AS_GLOBAL_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
