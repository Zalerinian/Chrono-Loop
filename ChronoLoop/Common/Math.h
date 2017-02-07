///////////////////////////////////////
//Written: 2/7/2017
//Author: Chris Burt
///////////////////////////////////////

#pragma once
#include <DirectXMath.h>

struct matrix4;

struct vec4f {
	union {
		struct {
			friend vec4f;
		private:
			DirectX::XMVECTOR vector;
		public:
			struct {
				float x, y, z, w;
			};
			float xyzw[4];
		} data;
	};

	vec4f();
	vec4f(float _x, float _y, float _z, float _w);
	vec4f(vec4f const& _copy);

	DirectX::XMVECTOR GetUnderlyingType();
	bool   vec4f::operator==(vec4f const& _other);
	vec4f& vec4f::operator=(vec4f const& _other);
	vec4f  vec4f::operator^(vec4f const& _other);
	vec4f  vec4f::operator*(matrix4 const& _other);
	vec4f& vec4f::operator*=(matrix4 const& _other);
	float  vec4f::operator*(vec4f const& _other);
	vec4f  vec4f::operator*(float const& _other);
	vec4f& vec4f::operator*=(float const&_other);
	vec4f  vec4f::operator/(float const& _other);
	vec4f& vec4f::operator/=(float const& _other);
	vec4f  vec4f::operator-();
	vec4f  vec4f::operator-(vec4f const& _other);
	vec4f  vec4f::operator-(float const& _other);
	vec4f& vec4f::operator-=(vec4f const& _other);
	vec4f& vec4f::operator-=(float const& _other);
	vec4f  vec4f::operator+(float const& _other);
	vec4f  vec4f::operator+(vec4f const& _other);
	vec4f& vec4f::operator+=(float const& _other);
	vec4f& vec4f::operator+=(vec4f const& _other);
	float& vec4f::operator[](unsigned int _index);
	vec4f  Cross(vec4f const& _other);
	float  Dot(vec4f const& _other);
	float  Magnitude();
	float  SquaredMagnitude();
	vec4f  Normalize();
	vec4f  Reflect(vec4f other);
};

struct matrix4 {
	union {
		struct {
		private:
			DirectX::XMMATRIX matrix;
		public:
			vec4f tiers[4];
			struct {
				vec4f first, second, third, fourth;
			};
		} data;
	};
};

