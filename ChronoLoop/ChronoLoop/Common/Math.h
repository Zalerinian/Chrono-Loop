///////////////////////////////////////
//Written: 2/7/2017
//Author: Chris Burt
///////////////////////////////////////

#pragma once
#include <DirectXMath.h>

struct matrix4;

struct vec4i {
	union {
		struct {
			friend vec4i;
		private:
			DirectX::XMINT4 vector;
		public:
			struct {
				int x, y, z, w;
			};
			float xyzw[4];
		} data;
	};
};

struct vec2f {
	union {
		struct {
			friend vec2f;
		private:
			DirectX::XMFLOAT2 vector;
		public:
			struct {
				float x, y;
			};
			float xy[2];
		} data;
	};
};

struct vec3f {
	union {
		struct {
			friend vec3f;
		private:
			DirectX::XMFLOAT3 vector;
		public:
			struct {
				float x, y, z;
			};
			float xyz[3];
		} data;
	};
};

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
	float  Magnitude() const;
	float  SquaredMagnitude() const;
	vec4f  Normalize() const;
	vec4f  Reflect(const vec4f& other);
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

