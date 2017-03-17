#include "Expo.h"
#include <math.h>

namespace Epoch{
	namespace Easing {

		float ExpoIn(float t, float b, float c, float d) {
			return (t == 0) ? b : c * (float)pow(2, 10 * (t / d - 1)) + b;
		}

		float ExpoOut(float t, float b, float c, float d) {
			return (t == d) ? b + c : c * ((float)-pow(2, -10 * t / d) + 1) + b;
		}

		float ExpoInOut(float t, float b, float c, float d) {
			if (t == 0) return b;
			if (t == d) return b + c;
			if ((t /= d / 2) < 1) return c / 2 * (float)pow(2, 10 * (t - 1)) + b;
			return c / 2 * ((float)-pow(2, -10 * --t) + 2) + b;
		}

	}
}