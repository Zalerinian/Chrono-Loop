#include "Linear.h"

namespace Epoch {
	namespace Easing {

		float Linear(float t, float b, float c, float d) {
			return c*t / d + b;
		}

	}
}