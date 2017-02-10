#pragma once
#include <DirectXMath.h>
#include <openvr.h>

struct matrix4;

struct vec4i {
	union {
		DirectX::XMVECTOR vector;
		struct {
			int x, y, z, w;
		};
		int xyzw[4];
	};
};

struct vec2f {
	union {
		DirectX::XMVECTOR vector;
		struct {
			float x, y;
		};
		float xy[2];
	};
	vec2f() { x = 0; y = 0; };
	vec2f(float _x, float _y) { x = _x; y = _y; };
};

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
	vec3f(vec3f const& _copy);
	vec3f(vec4f const& _copy);

	//DirectX::XMVECTOR GetUnderlyingType() { return vector; }

	bool   vec3f::operator==(vec3f const& _other);
	bool   vec3f::operator!=(vec3f const& _other);
	vec3f& vec3f::operator=(vec3f const& _other);
	vec3f  vec3f::operator^(vec3f const& _other);
	vec3f  vec3f::operator*(matrix4 const& _other);
	vec3f& vec3f::operator*=(matrix4 const& _other);
	float  vec3f::operator*(vec3f const& _other);
	vec3f  vec3f::operator*(float const& _other);
	vec3f& vec3f::operator*=(float const&_other);
	vec3f  vec3f::operator/(float const& _other);
	vec3f& vec3f::operator/=(float const& _other);
	vec3f  vec3f::operator-();
	vec3f  vec3f::operator-(vec3f const& _other);
	vec3f  vec3f::operator-(float const& _other);
	vec3f& vec3f::operator-=(vec3f const& _other);
	vec3f& vec3f::operator-=(float const& _other);
	vec3f  vec3f::operator+(float const& _other);
	vec3f  vec3f::operator+(vec3f const& _other);
	vec3f& vec3f::operator+=(float const& _other);
	vec3f& vec3f::operator+=(vec3f const& _other);
	float& vec3f::operator[](unsigned int _index);
	vec3f  Cross(vec3f const& _other);
	float  Dot(vec3f const& _other);
	float  Magnitude() const;
	float  SquaredMagnitude() const;
	vec3f  Normalize() const;
	vec3f  Reflect(vec3f const& other);
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
	vec4f(vec3f const& _copy);
	//DirectX::XMVECTOR GetUnderlyingType() { return vector; }

	bool   vec4f::operator==(vec4f const& _other);
	inline bool   vec4f::operator!=(vec4f const& _other) { return !(*this == _other); }
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
	matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44);
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
	matrix4 MatrixScale(float _x, float _y, float _z);
	matrix4 Projection(float _aspect, float _fov, float _near, float _far);
	matrix4 FromMatrix(vr::HmdMatrix44_t _mat);
	matrix4 FromMatrix(vr::HmdMatrix34_t _mat);
	matrix4 Identity();
}