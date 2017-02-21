#include "Breakpoint.h"

namespace Debug {

	void SetBreakpoint() {
#if _DEBUG && _MSC_VER
		__debugbreak();
#endif
	}

}