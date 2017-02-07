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

vec4f::vec4f(vec4f & _copy)
{
	for (int i = 0; i < 4; ++i)
		data.xyzw[i] = _copy.data.xyzw[i];
}

DirectX::XMVECTOR vec4f::GetUnderlyingType()
{
	return data.vector;
}

bool vec4f::operator==(vec4f & _other)
{
	for (int i = 0; i < 4; ++i)
		if (data.xyzw[i] != _other.data.xyzw[i])
			return false;

	return true;
}

vec4f& vec4f::operator=(vec4f& _other)
{
	if (!(*this == _other))
	{
		for (int i = 0; i < 4; ++i)
			data.xyzw[i] = _other.data.xyzw[i];
	}

	return *this;
}

vec4f vec4f::operator^(vec4f _other)
{
	vec4f temp;
	temp.data.x = (data.y * _other.data.z) - (data.z * _other.data.y);
	temp.data.y = (data.z * _other.data.x) - (data.x * _other.data.z);
	temp.data.z = (data.x * _other.data.y) - (data.y * _other.data.x);
	temp.data.w = 1;
	return temp;
}

vec4f vec4f::operator*(matrix4 _other)
{
	vec4f temp;
	for(int i = 0; i < 3; ++i)
		temp.data.xyzw[i] = (data.xyzw[0] * _other.data.first.data.xyzw[i]) + (data.xyzw[1] * _other.data.second.data.xyzw[i]) - (data.xyzw[2] * _other.data.third.data.xyzw[i]);

	temp.data.w = 1;
	return temp;
}

//vec4f& vec4f::operator*=(matrix4 _other)
//{
//	vec4f temp = ;
//
//}

