///////////////////////////////////////
//Written: 2/7/2017
//Author: Chris Burt
///////////////////////////////////////

#pragma once
#include <DirectXMath.h>
#include <openvr.h>

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
			int xyzw[4];
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
		DirectX::XMVECTOR vector;
		struct {
			float x, y, z, w;
		};
		float xyzw[4];
	};

	vec4f();
	vec4f(float _x, float _y, float _z, float _w);
	vec4f(vec4f const& _copy);
	//DirectX::XMVECTOR GetUnderlyingType() { return vector; }

	bool   vec4f::operator==(vec4f const& _other);
	bool   vec4f::operator!=(vec4f const& _other);
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
	vec4f  Reflect(vec4f const& other);
};

struct matrix4 {
	union {
		DirectX::XMMATRIX matrix;
		vec4f tiers[4];
		struct {
			vec4f first, second, third, fourth;
		};
	};

	matrix4();
	matrix4(matrix4& _copy);
	DirectX::XMMATRIX matrix4::GetUnderlyingType() { return matrix; }

	bool     matrix4::operator==(matrix4 const& _other);
	bool     matrix4::operator!=(matrix4 const& _other);
	matrix4& matrix4::operator=(matrix4 const& _other);
	matrix4  matrix4::operator*(matrix4 const& _other);
	matrix4& matrix4::operator*=(matrix4 const& _other);
	matrix4  matrix4::operator*(float _other);
	matrix4& matrix4::operator*=(float _other);
	matrix4  matrix4::operator+(matrix4 const& _other);
	matrix4& matrix4::operator+=(matrix4 const& _other);
	vec4f&   matrix4::operator[](unsigned int _index);
	
	matrix4  Inverse();
	float*   AsArray() { return reinterpret_cast<float*>(matrix.r); }
};

namespace Math
{
	matrix4 MatrixRotateAxis(vec4f const& _axis, float _rads);
	matrix4 MatrixRotateX(float _rads);
	matrix4 MatrixRotateY(float _rads);
	matrix4 MatrixRotateZ(float _rads);
	matrix4 MatrixTranslation(float _x, float _y, float _z);
	matrix4 MatrixTranspose(matrix4 & other);
	matrix4 Projection(float _aspect, float _fov, float _near, float _far);
	matrix4 FromMatrix(vr::HmdMatrix44_t _mat);
	matrix4 FromMatrix(vr::HmdMatrix34_t _mat);
	matrix4 Identity();
}