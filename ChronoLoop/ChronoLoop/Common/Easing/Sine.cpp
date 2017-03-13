#include "Sine.h"
#include <math.h>

#ifndef PI
#define PI  3.14159265
#endif

namespace Epoch {
	namespace Easing {

		float SineIn(float t, float b, float c, float d) {
			return -c * cos(t / d * (PI / 2)) + c + b;
		}

		float SineOut(float t, float b, float c, float d) {
			return c * sin(t / d * (PI / 2)) + b;
		}

		float SineInOut(float t, float b, float c, float d) {
			return -c / 2 * (cos(PI*t / d) - 1) + b;
		}

	}
}