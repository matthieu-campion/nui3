// tu_gc_test.cpp  -- Thatcher Ulrich <http://tulrich.com> 2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some test code for garbage collectors.
//
// cl -Zi -GX -GR tu_gc_test.cpp tu_gc_singlethreaded_marksweep.cpp -I.. -DTEST_GC_SINGLETHREADED_MARKSWEEP


#ifdef TEST_GC_SINGLETHREADED_MARKSWEEP

#include "base/tu_gc_singlethreaded_marksweep.h"
#include "base/tu_gc_singlethreaded_refcount.h"
#include <stdio.h>
#include <set>
#include <vector>

namespace test_ms {
#define GC_COLLECTOR tu_gc::singlethreaded_marksweep
#include "base/tu_gc_test_impl.h"
#undef GC_COLLECTOR
}  // test_ms

namespace test_rc {
#define GC_COLLECTOR tu_gc::singlethreaded_refcount
#include "base/tu_gc_test_impl.h"
#undef GC_COLLECTOR
}  // test_rc
	
int main() {
	printf("\nmark-sweep:\n\n");
	test_ms::run_tests();

	printf("\nref-counting:\n\n");
	test_rc::run_tests();

	return 0;
}


#endif  // TEST_GC_SINGLETHREADED_MARKSWEEP
