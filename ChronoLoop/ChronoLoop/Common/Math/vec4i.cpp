#include "vec4i.h"

using namespace std;

namespace Epoch {

#define EPSILON 0.001f

	vec4i::vec4i(const vec4i& _copy) {
		for (int i = 0; i < 4; ++i)
			xyzw[i] = _copy.xyzw[i];
	}

	bool vec4i::operator==(const vec4i& _other) const {
		for (int i = 0; i < 4; ++i)
			if (xyzw[i] != _other.xyzw[i])
				return false;
		return true;
	}

	vec4i& vec4i::operator=(const vec4i& _other) {
		if (*this != _other)
			for (int i = 0; i < 4; ++i)
				xyzw[i] = _other.xyzw[i];
		return *this;
	}

	vec4i vec4i::operator^(const vec4i& _other) const {
		vec4i temp;
		temp.x = (y * _other.z) - (z * _other.y);
		temp.y = (z * _other.x) - (x * _other.z);
		temp.z = (x * _other.y) - (y * _other.x);
		temp.w = 1;
		return temp;
	}

	int vec4i::operator*(const vec4i& _other) const {
		return x * _other.x + y * _other.y + z * _other.z;
	}

	vec4i vec4i::operator*(const int& _other) const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] * _other;
		temp.w = 1;
		return temp;
	}

	vec4i& vec4i::operator*=(const int& _other) {
		*this = *this * _other;
		return *this;
	}

	vec4i vec4i::operator/(const int& _other) const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] / _other;
		temp.w = 1;
		return temp;
	}

	vec4i& vec4i::operator/=(const int& _other) {
		*this = *this / _other;
		return *this;
	}

	vec4i vec4i::operator-() const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] * -1;
		temp.w = 1;
		return temp;
	}

	vec4i vec4i::operator-(const vec4i& _other) const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] - _other.xyzw[i];
		temp.w = 1;
		return temp;
	}

	vec4i vec4i::operator-(const int& _other) const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] - _other;
		temp.w = 1;
		return temp;
	}

	vec4i& vec4i::operator-=(const vec4i& _other) {
		*this = *this - _other;
		return *this;
	}

	vec4i& vec4i::operator-=(const int& _other) {
		*this = *this - _other;
		return *this;
	}

	vec4i vec4i::operator+(const int& _other) const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] + _other;
		temp.w = 1;
		return temp;
	}

	vec4i vec4i::operator+(const vec4i& _other) const {
		vec4i temp;
		for (int i = 0; i < 3; ++i)
			temp.xyzw[i] = xyzw[i] + _other.xyzw[i];
		temp.w = 1;
		return temp;
	}

	vec4i& vec4i::operator+=(const int& _other) {
		*this = *this + _other;
		return *this;
	}

	vec4i& vec4i::operator+=(const vec4i& _other) {
		*this = *this + _other;
		return *this;
	}

	int& vec4i::operator[](const unsigned int _index) {
		if (_index < 4)
			return xyzw[_index];
		return xyzw[0];
	}

	vec4i vec4i::Cross(const vec4i& _other) const {
		return *this ^ _other;
	}

	int vec4i::Dot(const vec4i& _other) const {
		return *this * _other;
	}

	int vec4i::Magnitude() const {
		return sqrt(SquaredMagnitude());
	}

	int vec4i::SquaredMagnitude() const {
		return pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2);
	}

	vec4i vec4i::Normalize() const {
		vec4i temp;
		int norm = 1 / Magnitude();
		for (int i = 0; i < 4; ++i)
			temp.xyzw[i] = xyzw[i] * norm;
		return temp;
	}

	vec4i vec4i::Reflect(const vec4i& _other) const {
		vec4i temp, N;
		N = _other.Normalize();
		int reflect = -2 * (*this * N);
		temp = N * reflect - *this;
		return temp;
	}

	vec4i & vec4i::Set(const int _x, const int _y, const int _z, const int _w) {
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
		return *this;
	}

}