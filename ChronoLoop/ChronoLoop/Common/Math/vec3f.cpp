#include "vec3f.h"
#include "vec4f.h"
#include <math.h>

using namespace std;

namespace Epoch {

#define EPSILON 0.001f

	vec3f::vec3f() {
		x = y = z = 0;
	}

	vec3f::vec3f(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	vec3f::vec3f(const vec3f& _copy) {
		for (int i = 0; i < 3; ++i)
			xyz[i] = _copy.xyz[i];
	}

	vec3f::vec3f(const vec4f& _other) {
		x = _other.x;
		y = _other.y;
		z = _other.z;
	}

	bool vec3f::operator==(const vec3f& _other) const {
		for (int i = 0; i < 4; ++i)
			if (xyz[i] != _other.xyz[i])
				return false;
		return true;
	}

	bool vec3f::operator!=(const vec3f& _other) const {
		return !(*this == _other);
	}

	vec3f& vec3f::operator=(const vec3f& _other) {
		if (*this != _other)
			for (int i = 0; i < 4; ++i)
				xyz[i] = _other.xyz[i];
		return *this;
	}

	vec3f vec3f::operator^(const vec3f& _other) const {
		vec3f temp;
		temp.x = (y * _other.z) - (z * _other.y);
		temp.y = (z * _other.x) - (x * _other.z);
		temp.z = (x * _other.y) - (y * _other.x);
		return temp;
	}

	float vec3f::operator*(const vec3f& _other) const {
		return x * _other.x + y * _other.y + z * _other.z;
	}

	vec3f vec3f::operator*(const float& _other) const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] * _other;
		return temp;
	}

	vec3f& vec3f::operator*=(const float& _other) {
		*this = *this * _other;
		return *this;
	}

	vec3f vec3f::operator/(const float& _other) const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] / _other;
		return temp;
	}

	vec3f& vec3f::operator/=(const float& _other) {
		*this = *this / _other;
		return *this;
	}

	vec3f vec3f::operator-() const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] * -1.0f;
		return temp;
	}

	vec3f vec3f::operator-(const vec3f& _other) const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] - _other.xyz[i];
		return temp;
	}

	vec3f vec3f::operator-(const float& _other) const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] - _other;
		return temp;
	}

	vec3f& vec3f::operator-=(const vec3f& _other) {
		*this = *this - _other;
		return *this;
	}

	vec3f& vec3f::operator-=(const float& _other) {
		*this = *this - _other;
		return *this;
	}

	vec3f vec3f::operator+(const float& _other) const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] + _other;
		return temp;
	}

	vec3f vec3f::operator+(const vec3f& _other) const {
		vec3f temp;
		for (int i = 0; i < 3; ++i)
			temp.xyz[i] = xyz[i] + _other.xyz[i];
		return temp;
	}

	vec3f& vec3f::operator+=(const float& _other) {
		*this = *this + _other;
		return *this;
	}

	vec3f& vec3f::operator+=(const vec3f& _other) {
		*this = *this + _other;
		return *this;
	}

	float& vec3f::operator[](const unsigned int _index) {
		return xyz[_index];
	}

	vec3f vec3f::Cross(const vec3f& _other) const {
		return *this ^ _other;
	}

	float vec3f::Dot(const vec3f& _other) const {
		return *this * _other;
	}

	float vec3f::Magnitude() const {
		return sqrtf(SquaredMagnitude());
	}

	float vec3f::SquaredMagnitude() const {
		return x * x + y * y + z * z;
	}

	vec3f vec3f::Normalize() const {
		vec3f temp;
		float norm = 1 / Magnitude();
		for (int i = 0; i < 4; ++i)
			temp.xyz[i] = xyz[i] * norm;
		return temp;
	}

	vec3f vec3f::Reflect(const vec3f& _other) const {
		vec3f temp, N;
		N = _other.Normalize();
		float reflect = -2 * (*this * N);
		temp = N * reflect - *this;
		return temp;
	}

	vec3f & vec3f::Set(const float _x, const float _y, const float _z) {
		this->x = _x;
		this->y = _y;
		this->z = _z;
		return *this;
	}

}