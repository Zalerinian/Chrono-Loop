///////////////////////////////////////
//Written: 2/7/2017
//Author: Chris Burt
///////////////////////////////////////

#include "stdafx.h"
#include "Math.h"
#include <memory>

#pragma region VECTOR4F_MATH

vec4f::vec4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vec4f::vec4f(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

vec4f::vec4f(vec4f const& _copy)
{
	for (int i = 0; i < 4; ++i)
		xyzw[i] = _copy.xyzw[i];
}

vec4f::vec4f(vec3f const& _copy)
{
	for (int i = 0; i < 3; ++i)
		xyzw[i] = _copy.xyz[i];
	w = 1;
}

bool vec4f::operator==(vec4f const& _other)
{
	for (int i = 0; i < 4; ++i)
		if (xyzw[i] != _other.xyzw[i])
			return false;
	return true;
}

bool vec4f::operator!=(vec4f const& _other)
{
	return !(*this == _other);
}

vec4f& vec4f::operator=(vec4f const& _other)
{
	if (*this != _other)
		for (int i = 0; i < 4; ++i)
			xyzw[i] = _other.xyzw[i];
	return *this;
}

vec4f vec4f::operator^(vec4f const& _other)
{
	vec4f temp;
	temp.x = (y * _other.z) - (z * _other.y);
	temp.y = (z * _other.x) - (x * _other.z);
	temp.z = (x * _other.y) - (y * _other.x);
	temp.w = 1;
	return temp;
}

vec4f vec4f::operator*(matrix4 const& _other)
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.xyzw[i] = (xyzw[0] * _other.first.xyzw[i]) + 
					   (xyzw[1] * _other.second.xyzw[i]) - 
					   (xyzw[2] * _other.third.xyzw[i]);

	temp.w = 1;
	return temp;
}

vec4f& vec4f::operator*=(matrix4 const& _other)
{
	*this = *this * _other;
	return *this;
}

float vec4f::operator*(vec4f const& _other)
{
	return x * _other.x + y * _other.y + z + _other.z;
}

vec4f vec4f::operator*(float const& _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] * _other;
	temp.w = 1;
	return temp;
}

vec4f& vec4f::operator*=(float const& _other)
{
	*this = *this * _other;
	return *this;
}

vec4f vec4f::operator/(float const& _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] / _other;
	temp.w = 1;
	return temp;
}

vec4f& vec4f::operator/=(float const& _other)
{
	*this = *this / _other;
	return *this;
}

vec4f vec4f::operator-()
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] * -1.0f;
	temp.w = 1;
	return temp;
}

vec4f vec4f::operator-(vec4f const& _other)
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] - _other.xyzw[i];
	temp.w = 1;
	return temp;
}

vec4f vec4f::operator-(float const& _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] - _other;
	temp.w = 1;
	return temp;
}

vec4f& vec4f::operator-=(vec4f const& _other)
{
	*this = *this - _other;
	return *this;
}

vec4f& vec4f::operator-=(float const& _other)
{
	*this = *this - _other;
	return *this;
}

vec4f vec4f::operator+(float const& _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] + _other;
	temp.w = 1;
	return temp;
}

vec4f vec4f::operator+(vec4f const& _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyzw[i] = xyzw[i] + _other.xyzw[i];
	temp.w = 1;
	return temp;
}

vec4f& vec4f::operator+=(float const & _other)
{
	*this = *this + _other;
	return *this;
}

vec4f& vec4f::operator+=(vec4f const & _other)
{
	*this = *this + _other;
	return *this;
}

float& vec4f::operator[](unsigned int _index)
{
	if (_index < 4)
		return xyzw[_index];
	return xyzw[0];
}

vec4f vec4f::Cross(vec4f const & _other)
{
	return *this ^ _other;
}

float vec4f::Dot(vec4f const & _other)
{
	return *this * _other;
}

float vec4f::Magnitude() const
{
	return sqrtf(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f) + powf(w, 2.0f));
}

float vec4f::SquaredMagnitude() const
{
	return powf(Magnitude(), 2.0f);
}

vec4f vec4f::Normalize() const
{
	vec4f temp;
	float norm = 1 / Magnitude();
	for(int i = 0; i < 4; ++i)
		temp.xyzw[i] = xyzw[i] * norm;
	return temp;
}

vec4f vec4f::Reflect(vec4f const& _other)
{
	vec4f temp, N;
	N = _other.Normalize();
	float reflect = -2 * (*this * N);
	temp = N * reflect - *this;
	return temp;
}

#pragma endregion


#pragma region VECTOR3F_MATH

vec3f::vec3f()
{
	x = 0;
	y = 0;
	z = 0;
}

vec3f::vec3f(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

vec3f::vec3f(vec3f const& _copy)
{
	for (int i = 0; i < 3; ++i)
		xyz[i] = _copy.xyz[i];
}

vec3f::vec3f(vec4f const& _copy)
{
	for (int i = 0; i < 3; ++i)
		xyz[i] = _copy.xyzw[i];
}

bool vec3f::operator==(vec3f const& _other)
{
	for (int i = 0; i < 3; ++i)
		if (xyz[i] != _other.xyz[i])
			return false;
	return true;
}

bool vec3f::operator!=(vec3f const& _other)
{
	return !(*this == _other);
}

vec3f& vec3f::operator=(vec3f const& _other)
{
	if (*this != _other)
		for (int i = 0; i < 3; ++i)
			xyz[i] = _other.xyz[i];
	return *this;
}

vec3f vec3f::operator^(vec3f const& _other)
{
	vec3f temp;
	temp.x = (y * _other.z) - (z * _other.y);
	temp.y = (z * _other.x) - (x * _other.z);
	temp.z = (x * _other.y) - (y * _other.x);
	return temp;
}

vec3f vec3f::operator*(matrix4 const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = (xyz[0] * _other.first.xyzw[i]) +
		(xyz[1] * _other.second.xyzw[i]) -
		(xyz[2] * _other.third.xyzw[i]);
	
	return temp;
}

vec3f& vec3f::operator*=(matrix4 const& _other)
{
	*this = *this * _other;
	return *this;
}

float vec3f::operator*(vec3f const& _other)
{
	return x * _other.x + y * _other.y + z + _other.z;
}

vec3f vec3f::operator*(float const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] * _other;
	return temp;
}

vec3f& vec3f::operator*=(float const& _other)
{
	*this = *this * _other;
	return *this;
}

vec3f vec3f::operator/(float const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] / _other;
	return temp;
}

vec3f& vec3f::operator/=(float const& _other)
{
	*this = *this / _other;
	return *this;
}

vec3f vec3f::operator-()
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] * -1.0f;
	return temp;
}

vec3f vec3f::operator-(vec3f const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] - _other.xyz[i];
	return temp;
}

vec3f vec3f::operator-(float const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] - _other;
	return temp;
}

vec3f& vec3f::operator-=(vec3f const& _other)
{
	*this = *this - _other;
	return *this;
}

vec3f& vec3f::operator-=(float const& _other)
{
	*this = *this - _other;
	return *this;
}

vec3f vec3f::operator+(float const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] + _other;
	return temp;
}

vec3f vec3f::operator+(vec3f const& _other)
{
	vec3f temp;
	for (int i = 0; i < 3; ++i)
		temp.xyz[i] = xyz[i] + _other.xyz[i];
	return temp;
}

vec3f& vec3f::operator+=(float const & _other)
{
	*this = *this + _other;
	return *this;
}

vec3f& vec3f::operator+=(vec3f const & _other)
{
	*this = *this + _other;
	return *this;
}

float& vec3f::operator[](unsigned int _index)
{
	if (_index < 3)
		return xyz[_index];
	return xyz[0];
}

vec3f vec3f::Cross(vec3f const& _other)
{
	return *this ^ _other;
}

float vec3f::Dot(vec3f const& _other)
{
	return *this * _other;
}

float vec3f::Magnitude() const
{
	return sqrtf(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f));
}

float vec3f::SquaredMagnitude() const
{
	return powf(Magnitude(), 2.0f);
}

vec3f vec3f::Normalize() const
{
	vec3f temp;
	float norm = 1 / Magnitude();
	for (int i = 0; i < 4; ++i)
		temp.xyz[i] = xyz[i] * norm;
	return temp;
}

vec3f vec3f::Reflect(vec3f const& _other)
{
	vec3f temp, N;
	N = _other.Normalize();
	float reflect = -2 * (*this * N);
	temp = N * reflect - *this;
	return temp;
}

#pragma endregion


#pragma region MATRIX_MATH

matrix4::matrix4()
{
	memset(&matrix, 0, sizeof(matrix));
}

matrix4::matrix4(matrix4& _copy)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			tiers[i].xyzw[j] = _copy.tiers[i].xyzw[j];
}

bool matrix4::operator==(matrix4 const& _other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (tiers[i].xyzw[j] != _other.tiers[i].xyzw[j])
				return false;
	return true;
}

bool matrix4::operator!=(matrix4 const& _other)
{
	return !(*this == _other);
}

matrix4& matrix4::operator=(matrix4 const& _other)
{
	if (*this != _other)
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				tiers[i].xyzw[j] = _other.tiers[i].xyzw[j];
	}
	return *this;
}

matrix4 matrix4::operator*(matrix4 const& _other)
{
	matrix4 temp;
	temp.matrix = this->matrix * _other.matrix;
	return temp;
}

matrix4& matrix4::operator*=(matrix4 const& _other)
{
	*this = *this * _other;
	return *this;
}

matrix4 matrix4::operator*(float _other)
{
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = tiers[i].xyzw[j] * _other;
	return temp;
}

matrix4& matrix4::operator*=(float _other)
{
	*this = *this * _other;
	return *this;
}

matrix4 matrix4::operator+(matrix4 const& _other)
{
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = tiers[i].xyzw[j] + _other.tiers[i].xyzw[j];
	return temp;
}

matrix4& matrix4::operator+=(matrix4 const& _other)
{
	*this = *this + _other;
	return *this;
}

vec4f& matrix4::operator[](unsigned int _index)
{
	if (_index < 4)
		return tiers[_index];
	return tiers[0];
}

matrix4 matrix4::Inverse()
{
	matrix4 m;
	m.matrix = DirectX::XMMatrixInverse(nullptr, matrix);
	return m;
}

matrix4 matrix4::Identity()
{
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (i == j)
				temp.tiers[i].xyzw[j] = 1;
			else
				temp.tiers[i].xyzw[j] = 0;
		}
	return temp;
}

#pragma endregion


matrix4 Math::MatrixRotateAxis(vec4f const& _axis, float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1;
	DirectX::XMVECTOR vec;

	for (int i = 0; i < 4; ++i)
		vec.m128_f32[i] = _axis.xyzw[i];

	temp1 = DirectX::XMMatrixRotationAxis(vec, _rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixRotateX(float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixRotationX(_rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixRotateY(float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixRotationY(_rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixRotateZ(float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixRotationZ(_rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixTranslation(float _x, float _y, float _z)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixTranslation(_x, _y, _z);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixTranspose(matrix4& other)
{
	matrix4 _new;
	for (int r = 0; r < 4; ++r) 
		for (int c = 0; c < 4; ++c) 
			_new[r][c] = other[c][r];
	return _new;
}

matrix4 Math::Projection(float _aspect, float _fov, float _near, float _far)
{
	matrix4 _new;
	_new.matrix = DirectX::XMMatrixPerspectiveFovRH(_fov, _aspect, _near, _far);
	return _new;
}

matrix4 Math::FromMatrix(vr::HmdMatrix44_t _mat)
{
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 3; ++j)
			temp.tiers[i].xyzw[j] = _mat.m[i][j];

	temp.tiers[0].xyzw[3] = 0;
	temp.tiers[1].xyzw[3] = 0;
	temp.tiers[2].xyzw[3] = 0;
	temp.tiers[3].xyzw[3] = 1;
	return temp;
}

matrix4 Math::FromMatrix(vr::HmdMatrix34_t _mat)
{
	matrix4 temp;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = _mat.m[i][j];
	memset(temp.tiers[3].xyzw, 0, sizeof(float) * 4);
	temp.tiers[3].xyzw[3] = 1;
	return temp;
}