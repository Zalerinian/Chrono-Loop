//#include "stdafx.h"
#include "Math.h"
#include <memory>

#define EPSILON 0.001f

#pragma region VECTOR4F_MATH

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

//bool vec4f::operator!=(vec4f const& _other)
//{
//	return !(*this == _other);
//}

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
	return x * _other.x + y * _other.y + z * _other.z;
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

float vec4f::Magnitude3() const
{
	return sqrtf(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f));
}

float vec4f::Magnitude4() const
{
	return sqrtf(powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f) + powf(w, 2.0f));
}

float vec4f::SquaredMagnitude3() const
{
	return powf(Magnitude3(), 2.0f);
}

float vec4f::SquaredMagnitude4() const
{
	return powf(Magnitude4(), 2.0f);
}

vec4f vec4f::Normalize() const
{
	vec4f temp;
	float norm = 1 / Magnitude3();
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
	return x * _other.x + y * _other.y + z * _other.z;
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
	matrix = _copy.matrix;
}

matrix4::matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44) {
	tiers[0][0] = _11;
	tiers[0][1] = _12;
	tiers[0][2] = _13;
	tiers[0][3] = _14;
	tiers[1][0] = _21;
	tiers[1][1] = _22;
	tiers[1][2] = _23;
	tiers[1][3] = _24;
	tiers[2][0] = _31;
	tiers[2][1] = _32;
	tiers[2][2] = _33;
	tiers[2][3] = _34;
	tiers[3][0] = _41;
	tiers[3][1] = _42;
	tiers[3][2] = _43;
	tiers[3][3] = _44;
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
		this->matrix = _other.matrix;
	return *this;
}

matrix4 matrix4::operator*(matrix4 const& _other)
{
	/// A grave reminde that for some reason OpenGL and DirectX multiply matrices in REVERSE ORDER FROM EACH OTHER.
	//matrix4 temp(_floats[0] * _other._floats[0] + _floats[4] * _other._floats[1] + _floats[8] * _other._floats[2] + _floats[12] * _other._floats[3], _floats[1] * _other._floats[0] + _floats[5] * _other._floats[1] + _floats[9] * _other._floats[2] + _floats[13] * _other._floats[3], _floats[2] * _other._floats[0] + _floats[6] * _other._floats[1] + _floats[10] * _other._floats[2] + _floats[14] * _other._floats[3], _floats[3] * _other._floats[0] + _floats[7] * _other._floats[1] + _floats[11] * _other._floats[2] + _floats[15] * _other._floats[3],
	//	_floats[0] * _other._floats[4] + _floats[4] * _other._floats[5] + _floats[8] * _other._floats[6] + _floats[12] * _other._floats[7], _floats[1] * _other._floats[4] + _floats[5] * _other._floats[5] + _floats[9] * _other._floats[6] + _floats[13] * _other._floats[7], _floats[2] * _other._floats[4] + _floats[6] * _other._floats[5] + _floats[10] * _other._floats[6] + _floats[14] * _other._floats[7], _floats[3] * _other._floats[4] + _floats[7] * _other._floats[5] + _floats[11] * _other._floats[6] + _floats[15] * _other._floats[7],
	//	_floats[0] * _other._floats[8] + _floats[4] * _other._floats[9] + _floats[8] * _other._floats[10] + _floats[12] * _other._floats[11], _floats[1] * _other._floats[8] + _floats[5] * _other._floats[9] + _floats[9] * _other._floats[10] + _floats[13] * _other._floats[11], _floats[2] * _other._floats[8] + _floats[6] * _other._floats[9] + _floats[10] * _other._floats[10] + _floats[14] * _other._floats[11], _floats[3] * _other._floats[8] + _floats[7] * _other._floats[9] + _floats[11] * _other._floats[10] + _floats[15] * _other._floats[11],
	//	_floats[0] * _other._floats[12] + _floats[4] * _other._floats[13] + _floats[8] * _other._floats[14] + _floats[12] * _other._floats[15], _floats[1] * _other._floats[12] + _floats[5] * _other._floats[13] + _floats[9] * _other._floats[14] + _floats[13] * _other._floats[15], _floats[2] * _other._floats[12] + _floats[6] * _other._floats[13] + _floats[10] * _other._floats[14] + _floats[14] * _other._floats[15], _floats[3] * _other._floats[12] + _floats[7] * _other._floats[13] + _floats[11] * _other._floats[14] + _floats[15] * _other._floats[15]);
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
	temp.matrix = matrix * _other;
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
	temp.matrix = matrix + _other.matrix;
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
	m.matrix = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(matrix), matrix);
	return m;
}

#pragma endregion


matrix4 Math::MatrixRotateAxis(vec4f const& _axis, float _rads)
{
	matrix4 temp;
	DirectX::XMVECTOR vec;

	for (int i = 0; i < 4; ++i)
		vec.m128_f32[i] = _axis.xyzw[i];

	temp.matrix = DirectX::XMMatrixRotationAxis(vec, _rads);
	return temp;
}

matrix4 Math::MatrixRotateX(float _rads)
{
	matrix4 temp;
	temp.matrix = DirectX::XMMatrixRotationX(_rads);
	return temp;
}

matrix4 Math::MatrixRotateY(float _rads)
{
	matrix4 temp;
	temp.matrix = DirectX::XMMatrixRotationY(_rads);
	return temp;
}

matrix4 Math::MatrixRotateZ(float _rads)
{
	matrix4 temp;
	temp.matrix = DirectX::XMMatrixRotationZ(_rads);
	return temp;
}

matrix4 Math::MatrixTranslation(float _x, float _y, float _z)
{
	matrix4 temp;
	temp.matrix = DirectX::XMMatrixTranslation(_x, _y, _z);
	return temp;
}

matrix4 Math::MatrixTranspose(matrix4& other)
{
	matrix4 temp;
	temp.matrix = DirectX::XMMatrixTranspose(other.matrix);
	return temp;
}

matrix4 Math::MatrixScale(float _x, float _y, float _z)
{
	matrix4 temp;
	temp.matrix = DirectX::XMMatrixScaling(_x, _y, _z);
	return temp;
}

matrix4 Math::MatrixIdentity() 
{
	matrix4 m;
	m.matrix = DirectX::XMMatrixIdentity();
	return m;
}

matrix4 Math::MatrixRotateInPlace(matrix4 _self, float _x, float _y, float _z, float _rads) {
	vec4f pos;
	for (int i = 0; i < 4; ++i) {
		pos[i] = _self[3][i];
		_self[3][i] = 0;
	}
	_self *= Math::MatrixRotateAxis({ _x, _y, _z, 0 }, _rads);
	for (int i = 0; i < 4; ++i) {
		_self[3][i] = pos[i];
	}
	return _self;
}

matrix4 Math::MatrixRotateInPlace(matrix4 _self, vec4f _axis, float _rads) {
	return MatrixRotateInPlace(_self, _axis.x, _axis.y, _axis.z, _rads);
}


matrix4 Math::MatrixRotateAround(matrix4 _self, vec4f _axis, vec4f _point, float _rads) {
	vec4f pos;
	for (int i = 0; i < 3; ++i) {
		pos[i] = _self[i][3];
		_self[i][3] = pos[i] - _point[i]; // Set object's position to the delta of where it is to where it rotates about.
		pos[i] = _point[i] - pos[i];
	}
	// TODO: Fix this.
	_self *= Math::MatrixRotateAxis(_axis, _rads);
	for (int i = 0; i < 3; ++i) {
		_self[i][3] += pos[i]; // pos[i] is now the offset on each component, which we add back into the rotated object.
	}
	return _self;
}

matrix4 Math::FromMatrix(vr::HmdMatrix44_t _mat)
{
	// so [i][j] would leave us with a column major version, which is bad.
	matrix4 temp;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			temp.tiers[i].xyzw[j] = _mat.m[j][i];

	return temp;
}

matrix4 Math::FromMatrix(vr::HmdMatrix34_t _mat)
{
	matrix4 matrixObj(
		_mat.m[0][0], _mat.m[1][0], _mat.m[2][0], 0.0,
		_mat.m[0][1], _mat.m[1][1], _mat.m[2][1], 0.0,
		_mat.m[0][2], _mat.m[1][2], _mat.m[2][2], 0.0,
		_mat.m[0][3], _mat.m[1][3], _mat.m[2][3], 1.0f
	);
	return matrixObj;
}

vec2f vec2f::operator-(const vec2f & _r) {
	vec2f m;
	m.x = x - _r.x;
	m.y = y - _r.y;
	return m;
}

vec2f vec2f::operator-() {
	return vec2f(-x, -y);
}

float vec2f::operator*(const vec2f & _r) {
	return x * _r.x + y * _r.y;
}

float vec2f::Magnitude() {
	return sqrtf(SquaredMagnitude());
}

float vec2f::SquaredMagnitude() {
	return x * x + y * y;
}

vec2f vec2f::Normalize() {
	float mag = Magnitude();
	return vec2f(x / mag, y / mag);
}

float vec2f::Dot(const vec2f & _r) {
	return (*this) * _r;
}

vec2f vec2f::Cross() {
	return vec2f(y, -x);
}

bool vec2f::operator==(const vec2f & _other) {
	return (fabs(x - _other.x) < EPSILON &&
					fabs(y - _other.y) < EPSILON);
}

bool vec2f::operator!=(const vec2f & _other) {
	return !(*this == _other);
}
