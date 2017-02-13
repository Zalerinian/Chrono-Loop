#pragma once
#if _DEBUG && _MSC_VER
#include <intrin.h>
#endif

namespace Debug {

	void SetBreakpoint() {
	#if _DEBUG && _MSC_VER
		__debugbreak();
	#endif
	}

}