#pragma once
#include <DirectXMath.h>

namespace Epoch {

	struct vec4f;
	struct vec3f {
		union {
			DirectX::XMVECTOR vector;
			struct {
				float x, y, z;
			};
			float xyz[3];
		};

		vec3f();
		vec3f(float _x, float _y, float _z);
		vec3f(const vec3f& _copy);
		vec3f(const vec4f& _copy);

		bool   operator==(const vec3f& _other) const;
		bool   operator!=(const vec3f& _other) const;
		vec3f& operator=(const vec3f& _other);
		vec3f  operator^(const vec3f& _other) const;
		float  operator*(const vec3f& _other) const;
		vec3f  operator*(const float& _other) const;
		vec3f& operator*=(const float&_other);
		vec3f  operator/(const float& _other) const;
		vec3f& operator/=(const float& _other);
		vec3f  operator-() const;
		vec3f  operator-(const vec3f& _other) const;
		vec3f  operator-(const float& _other) const;
		vec3f& operator-=(const vec3f& _other);
		vec3f& operator-=(const float& _other);
		vec3f  operator+(const float& _other) const;
		vec3f  operator+(const vec3f& _other) const;
		vec3f& operator+=(const float& _other);
		vec3f& operator+=(const vec3f& _other);
		float& operator[](unsigned int _index);
		vec3f  Cross(const vec3f& _other) const;
		float  Dot(const vec3f& _other) const;
		float  Magnitude() const;
		float  SquaredMagnitude() const;
		vec3f  Normalize() const;
		vec3f  Reflect(const vec3f& other) const;
		vec3f& Set(const float _x, const float _y, const float _z);
	};

}