#include "Linear.h"

namespace Epoch {
	namespace Easing {

		float LinearNone(float t, float b, float c, float d) {
			return c*t / d + b;
		}
		
		float LinearIn(float t, float b, float c, float d) {
			return c*t / d + b;
		}
		
		float LinearOut(float t, float b, float c, float d) {
			return c*t / d + b;
		}

		float LinearInOut(float t, float b, float c, float d) {
			return c*t / d + b;
		}

	}
}