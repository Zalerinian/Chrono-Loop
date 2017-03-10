#pragma once
#include <DirectXMath.h>

namespace Epoch {

	struct vec2f {
		union {
			DirectX::XMVECTOR vector;
			struct {
				float x, y;
			};
			float xy[2];
		};

		vec2f() { x = 0; y = 0; };
		vec2f(const vec2f& _other);
		vec2f(float _x, float _y) { x = _x; y = _y; };
		bool   operator==(const vec2f& _other) const;
		bool   operator!=(const vec2f& _other) const;
		vec2f& operator=(const vec2f& _other);
		float  operator*(const vec2f& _other) const;
		vec2f  operator*(const float& _other) const;
		vec2f& operator*=(const float&_other);
		vec2f  operator/(const float& _other) const;
		vec2f& operator/=(const float& _other);
		vec2f  operator-() const;
		vec2f  operator-(const vec2f& _other) const;
		vec2f  operator-(const float& _other) const;
		vec2f& operator-=(const vec2f& _other);
		vec2f& operator-=(const float& _other);
		vec2f  operator+(const float& _other) const;
		vec2f  operator+(const vec2f& _other) const;
		vec2f& operator+=(const float& _other);
		vec2f& operator+=(const vec2f& _other);
		float& operator[](unsigned int _index);

		float Magnitude() const;
		float SquaredMagnitude() const;
		vec2f Normalize() const;
		float Dot(const vec2f& _r) const;
		vec2f Cross() const;
		vec2f& Set(const float _x, const float _y);
	};

}