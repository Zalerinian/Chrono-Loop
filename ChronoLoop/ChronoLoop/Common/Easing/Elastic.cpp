#include "Elastic.h"
#include <math.h>

#ifndef PI
	#define PI  3.14159265
#endif

namespace Epoch {
	namespace Easing {

		float ElasticIn(float t, float b, float c, float d) {
			if (t == 0) return b;  if ((t /= d) == 1) return b + c;
			float p = d*.3f;
			float a = c;
			float s = p / 4;
			float postFix = a*(float)pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
			return -(postFix * (float)sin((t*d - s)*(2 * PI) / p)) + b;
		}

		float ElasticOut(float t, float b, float c, float d) {
			if (t == 0) return b;  if ((t /= d) == 1) return b + c;
			float p = d*.3f;
			float a = c;
			float s = p / 4;
			return (a*(float)pow(2, -10.0f * t) * (float)sin((t*d - s)*(2.0f * PI) / p) + c + b);
		}

		float ElasticInOut(float t, float b, float c, float d) {
			if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;
			float p = d*(.3f*1.5f);
			float a = c;
			float s = p / 4;

			if (t < 1) {
				float postFix = a*(float)pow(2, 10 * (t -= 1)); // postIncrement is evil
				return -.5f*(postFix* (float)sin((t*d - s)*(2 * PI) / p)) + b;
			}
			float postFix = a*(float)pow(2, -10 * (t -= 1)); // postIncrement is evil
			return postFix * (float)sin((t*d - s)*(2 * PI) / p)*.5f + c + b;
		}

	}
}