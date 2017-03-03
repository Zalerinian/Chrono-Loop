#pragma once
#include <DirectXMath.h>

namespace Epoch {

	struct vec3f;
	struct matrix4;
	struct vec4f {
		union {
			DirectX::XMVECTOR vector;
			struct {
				float x, y, z, w;
			};
			float xyzw[4];
		};

		vec4f() { x = 0; y = 0; z = 0; w = 0; };
		vec4f(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; };
		vec4f(const vec4f& _copy);
		vec4f(const vec3f& _copy);

		bool   operator==(const vec4f& _other) const;
		inline bool operator!=(const vec4f& _other) const { return !(*this == _other); }
		vec4f& operator=(const vec4f& _other);
		vec4f  operator^(const vec4f& _other) const;
		vec4f  operator*(const matrix4& _other) const;
		vec4f& operator*=(const matrix4& _other);
		float  operator*(const vec4f  & _other) const;
		vec4f  operator*(const float  & _other) const;
		vec4f& operator*=(const float &_other);
		vec4f  operator/(const float  & _other) const;
		vec4f& operator/=(const float & _other);
		vec4f  operator-() const;
		vec4f  operator-(const vec4f & _other) const;
		vec4f  operator-(const float & _other) const;
		vec4f& operator-=(const vec4f& _other);
		vec4f& operator-=(const float& _other);
		vec4f  operator+(const float & _other) const;
		vec4f  operator+(const vec4f & _other) const;
		vec4f& operator+=(const float& _other);
		vec4f& operator+=(const vec4f& _other);
		float& operator[](const unsigned int _index);
		vec4f  Cross(const vec4f& _other) const;
		float  Dot(const vec4f& _other) const;
		float  Magnitude() const;
		float  Magnitude3() const;
		float  SquaredMagnitude() const;
		float  SquaredMagnitude3() const;
		vec4f  Normalize() const;
		vec4f  Reflect(const vec4f& other) const;
		vec4f& Set(const float _x, const float _y, const float _z, const float _w);
	};

}