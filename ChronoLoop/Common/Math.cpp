///////////////////////////////////////
//Written: 2/7/2017
//Author: Chris Burt
///////////////////////////////////////

#include "Math.h"

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

DirectX::XMVECTOR vec4f::GetUnderlyingType()
{
	return data.vector;
}

bool vec4f::operator==(vec4f const& _other)
{
	for (int i = 0; i < 4; ++i)
		if (data.xyzw[i] != _other.data.xyzw[i])
			return false;

	return true;
}

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

vec4f & vec4f::operator/=(float const & _other)
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

vec4f & vec4f::operator-=(float const & _other)
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

vec4f & vec4f::operator+=(float const & _other)
{
	*this = *this + _other;
	return *this;
}

vec4f & vec4f::operator+=(vec4f const & _other)
{
	*this = *this + _other;
	return *this;
}

float& vec4f::operator[](unsigned int _index)
{
	if (_index < 4)
		return data.xyzw[_index];
}

vec4f vec4f::Cross(vec4f const & _other)
{
	return *this ^ _other;
}

float vec4f::Dot(vec4f const & _other)
{
	return *this * _other;
}

float vec4f::Magnitude()
{
	return sqrt(powf(data.x, 2.0f) + powf(data.y, 2.0f) + powf(data.z, 2.0f) + powf(data.w, 2.0f));
}

float vec4f::SquaredMagnitude()
{
	return powf(Magnitude(), 2.0f);
}

vec4f vec4f::Normalize()
{
	vec4f temp;
	float norm = 1 / Magnitude();
	for(int i = 0; i < 4; ++i)
		temp.data.xyzw[i] = data.xyzw[i] *= norm;
	return temp;
}

vec4f vec4f::Reflect(vec4f _other)
{
	vec4f temp, N;
	N = _other.Normalize();
	float reflect = -2 * (*this * N);
	temp = N * reflect - *this;
	return temp;
}

