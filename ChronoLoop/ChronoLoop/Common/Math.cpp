///////////////////////////////////////
//Written: 2/7/2017
//Author: Chris Burt
///////////////////////////////////////

#include "stdafx.h"
#include "Math.h"


#pragma region VECTOR_MATH

vec4f::vec4f()
{
	data.x = 0;
	data.y = 0;
	data.z = 0;
	data.w = 0;
}

vec4f::vec4f(float _x, float _y, float _z, float _w)
{
	data.x = _x;
	data.y = _y;
	data.z = _z;
	data.w = _w;
}

vec4f::vec4f(vec4f const& _copy)
{
	for (int i = 0; i < 4; ++i)
		data.xyzw[i] = _copy.data.xyzw[i];
}

bool vec4f::operator==(vec4f const& _other)
{
	for (int i = 0; i < 4; ++i)
		if (data.xyzw[i] != _other.data.xyzw[i])
			return false;

	return true;
}

//bool vec4f::operator!=(vec4f const& _other)
//{
//	return !(*this == _other);
//}

vec4f& vec4f::operator=(vec4f const& _other)
{
	if (!(*this == _other))
	{
		for (int i = 0; i < 4; ++i)
			data.xyzw[i] = _other.data.xyzw[i];
	}

	return *this;
}

vec4f vec4f::operator^(vec4f const& _other)
{
	vec4f temp;
	temp.data.x = (data.y * _other.data.z) - (data.z * _other.data.y);
	temp.data.y = (data.z * _other.data.x) - (data.x * _other.data.z);
	temp.data.z = (data.x * _other.data.y) - (data.y * _other.data.x);
	temp.data.w = 1;
	return temp;
}

vec4f vec4f::operator*(matrix4 const& _other)
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = (data.xyzw[0] * _other.data.first.data.xyzw[i]) + 
							(data.xyzw[1] * _other.data.second.data.xyzw[i]) - 
							(data.xyzw[2] * _other.data.third.data.xyzw[i]);

	temp.data.w = 1;
	return temp;
}

vec4f& vec4f::operator*=(matrix4 const& _other)
{
	*this = *this * _other;
	return *this;
}

float vec4f::operator*(vec4f const& _other)
{
	return data.x * _other.data.x + data.y * _other.data.y + data.z + _other.data.z;
}

vec4f vec4f::operator*(float const & _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] * _other;
	temp.data.w = 1;
	return temp;
}

vec4f& vec4f::operator*=(float const & _other)
{
	*this = *this * _other;
	return *this;
}

vec4f vec4f::operator/(float const & _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] / _other;
	temp.data.w = 1;
	return temp;
}

vec4f& vec4f::operator/=(float const & _other)
{
	*this = *this / _other;
	return *this;
}

vec4f vec4f::operator-()
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] * -1.0f;
	temp.data.w = 1;
	return temp;
}

vec4f vec4f::operator-(vec4f const & _other)
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] - _other.data.xyzw[i];
	temp.data.w = 1;
	return temp;
}

vec4f vec4f::operator-(float const & _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] - _other;
	temp.data.w = 1;
	return temp;
}

vec4f& vec4f::operator-=(vec4f const & _other)
{
	*this = *this - _other;
	return *this;
}

vec4f& vec4f::operator-=(float const & _other)
{
	*this = *this - _other;
	return *this;
}

vec4f vec4f::operator+(float const & _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] + _other;
	temp.data.w = 1;
	return temp;
}

vec4f vec4f::operator+(vec4f const & _other)
{
	vec4f temp;
	for (int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = data.xyzw[i] + _other.data.xyzw[i];
	temp.data.w = 1;
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
		return data.xyzw[_index];
	return data.xyzw[0];
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
	return (float)sqrt(powf(data.x, 2.0f) + powf(data.y, 2.0f) + powf(data.z, 2.0f) + powf(data.w, 2.0f));
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
		temp.data.xyzw[i] = data.xyzw[i] * norm;
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


#pragma region MATRIX_MATH

matrix4::matrix4()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			data.tiers[i].data.xyzw[j] = 0;
}

matrix4::matrix4(matrix4 const& _copy)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			data.tiers[i].data.xyzw[j] = _copy.data.tiers[i].data.xyzw[j];
}

bool matrix4::operator==(matrix4 const& _other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (data.tiers[i].data.xyzw[j] != _other.data.tiers[i].data.xyzw[j])
				return false;
	return true;
}

bool matrix4::operator!=(matrix4 const& _other)
{
	return !(*this == _other);
}

matrix4& matrix4::operator=(matrix4 const& _other)
{
	if (!(*this == _other))
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				data.tiers[i].data.xyzw[j] = _other.data.tiers[i].data.xyzw[j];
	}
	return *this;
}

matrix4 matrix4::operator*(matrix4 const& _other)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1, temp2, temp3;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp1.r[i].m128_f32[j] = data.tiers[i].data.xyzw[j];
			temp2.r[i].m128_f32[j] = _other.data.tiers[i].data.xyzw[j];
		}
	}
	
	temp3 = temp1 * temp2;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	
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
			temp.data.tiers[i].data.xyzw[j] = data.tiers[i].data.xyzw[j] * _other;
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
			temp.data.tiers[i].data.xyzw[j] = data.tiers[i].data.xyzw[j] + _other.data.tiers[i].data.xyzw[j];
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
		return data.tiers[_index];
	return data.tiers[0];
}

matrix4 matrix4::Inverse()
{
	matrix4 temp;
	DirectX::XMMATRIX temp1, temp2;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp1.r[i].m128_f32[j] = data.tiers[i].data.xyzw[j];

	temp2 = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(temp1), temp1);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

#pragma endregion


matrix4 Math::MatrixRotateAxis(vec4f _axis, float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1;
	DirectX::XMVECTOR vec;

	for (int i = 0; i < 4; ++i)
		vec.m128_f32[i] = _axis.data.xyzw[i];

	temp1 = DirectX::XMMatrixRotationAxis(vec, _rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixRotateX(float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixIdentity();
	temp1 = DirectX::XMMatrixRotationX(_rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixRotateY(float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixIdentity();
	temp1 = DirectX::XMMatrixRotationY(_rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixRotateZ(float _rads)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixIdentity();
	temp1 = DirectX::XMMatrixRotationZ(_rads);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}

matrix4 Math::MatrixTranslation(float _x, float _y, float _z)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixIdentity();
	temp1 = DirectX::XMMatrixTranslation(_x, _y, _z);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
}
matrix4 Math::MatrixScale(float x, float y, float z)
{
	matrix4 temp;
	DirectX::XMMATRIX temp1 = DirectX::XMMatrixIdentity();
	temp1 = DirectX::XMMatrixScaling(x, y, z);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = temp1.r[i].m128_f32[j];
	return temp;
	
}
matrix4 Math::FromMatrix(vr::HmdMatrix44_t _mat)
{
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 3; ++j)
			temp.data.tiers[i].data.xyzw[j] = _mat.m[i][j];

	temp.data.tiers[0].data.xyzw[3] = 0;
	temp.data.tiers[1].data.xyzw[3] = 0;
	temp.data.tiers[2].data.xyzw[3] = 0;
	temp.data.tiers[3].data.xyzw[3] = 1;
	return temp;
}

matrix4 Math::FromMatrix(vr::HmdMatrix34_t _mat)
{
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.data.tiers[i].data.xyzw[j] = _mat.m[i][j];
	return temp;
}