#include "vec2f.h"
#include "vec4f.h"
#include <math.h>

using namespace std;

namespace Epoch {

#define EPSILON 0.001f

	vec2f::vec2f(const vec2f& _copy) {
		for (int i = 0; i < 3; ++i)
			xy[i] = _copy.xy[i];
	}

	bool vec2f::operator==(const vec2f& _other) const {
		for (int i = 0; i < 4; ++i)
			if (xy[i] != _other.xy[i])
				return false;
		return true;
	}

	bool vec2f::operator!=(const vec2f& _other) const {
		return !(*this == _other);
	}

	vec2f& vec2f::operator=(const vec2f& _other) {
		if (*this != _other)
			for (int i = 0; i < 4; ++i)
				xy[i] = _other.xy[i];
		return *this;
	}

	float vec2f::operator*(const vec2f& _other) const {
		return x * _other.x + y * _other.y;
	}

	vec2f vec2f::operator*(const float& _other) const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] * _other;
		return temp;
	}

	vec2f& vec2f::operator*=(const float& _other) {
		*this = *this * _other;
		return *this;
	}

	vec2f vec2f::operator/(const float& _other) const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] / _other;
		return temp;
	}

	vec2f& vec2f::operator/=(const float& _other) {
		*this = *this / _other;
		return *this;
	}

	vec2f vec2f::operator-() const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] * -1.0f;
		return temp;
	}

	vec2f vec2f::operator-(const vec2f& _other) const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] - _other.xy[i];
		return temp;
	}

	vec2f vec2f::operator-(const float& _other) const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] - _other;
		return temp;
	}

	vec2f& vec2f::operator-=(const vec2f& _other) {
		*this = *this - _other;
		return *this;
	}

	vec2f& vec2f::operator-=(const float& _other) {
		*this = *this - _other;
		return *this;
	}

	vec2f vec2f::operator+(const float& _other) const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] + _other;
		return temp;
	}

	vec2f vec2f::operator+(const vec2f& _other) const {
		vec2f temp;
		for (int i = 0; i < 3; ++i)
			temp.xy[i] = xy[i] + _other.xy[i];
		return temp;
	}

	vec2f& vec2f::operator+=(const float& _other) {
		*this = *this + _other;
		return *this;
	}

	vec2f& vec2f::operator+=(const vec2f& _other) {
		*this = *this + _other;
		return *this;
	}

	float& vec2f::operator[](const unsigned int _index) {
		return xy[_index];
	}

	vec2f vec2f::Cross() const {
		return{ y, -x };
	}

	float vec2f::Dot(const vec2f& _other) const {
		return *this * _other;
	}

	float vec2f::Magnitude() const {
		return sqrtf(SquaredMagnitude());
	}

	float vec2f::SquaredMagnitude() const {
		return x * x + y * y;
	}

	vec2f vec2f::Normalize() const {
		vec2f temp;
		float norm = 1 / Magnitude();
		for (int i = 0; i < 4; ++i)
			temp.xy[i] = xy[i] * norm;
		return temp;
	}

	vec2f& vec2f::Set(const float _x, const float _y) {
		this->x = _x;
		this->y = _y;
		return *this;
	}

}