#include "matrix4.h"
#include "vec4f.h"
#include "vec3f.h"
#include <openvr.h>

using namespace DirectX;

namespace Epoch {

	matrix4::matrix4() {
		tiers[0].Set(1, 0, 0, 0);
		tiers[1].Set(0, 1, 0, 0);
		tiers[2].Set(0, 0, 1, 0);
		tiers[3].Set(0, 0, 0, 1);
	}

	matrix4::matrix4(matrix4& _copy) {
		matrix = _copy.matrix;
	}

	matrix4::matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44) {
		tiers[0].Set(_11, _12, _13, _14);
		tiers[1].Set(_21, _22, _23, _24);
		tiers[2].Set(_31, _32, _33, _34);
		tiers[3].Set(_41, _42, _43, _44);
	}

	matrix4::matrix4(DirectX::XMMATRIX _xm) : matrix(_xm) {}

	matrix4::matrix4(vr::HmdMatrix44_t & _m) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				rows[i].xyzw[j] = _m.m[j][i];
	}

	matrix4::matrix4(vr::HmdMatrix34_t & _m) {
		rows[0].Set(_m.m[0][0], _m.m[1][0], _m.m[2][0], 0.0f);
		rows[1].Set(_m.m[0][1], _m.m[1][1], _m.m[2][1], 0.0f);
		rows[2].Set(_m.m[0][2], _m.m[1][2], _m.m[2][2], 0.0f);
		rows[3].Set(_m.m[0][3], _m.m[1][3], _m.m[2][3], 1.0f);
	}

	bool matrix4::operator==(matrix4 const& _other) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (tiers[i].xyzw[j] != _other.tiers[i].xyzw[j])
					return false;
		return true;
	}

	bool matrix4::operator!=(matrix4 const& _other) {
		return !(*this == _other);
	}

	matrix4& matrix4::operator=(matrix4 const& _other) {
		if (*this != _other)
			this->matrix = _other.matrix;
		return *this;
	}

	matrix4 matrix4::operator*(matrix4 const& _other) {
		matrix4 temp;
		temp.matrix = this->matrix * _other.matrix;
		return temp;
	}

	matrix4& matrix4::operator*=(matrix4 const& _other) {
		*this = *this * _other;
		return *this;
	}

	matrix4 matrix4::operator*(float _other) {
		matrix4 temp;
		temp.matrix = matrix * _other;
		return temp;
	}

	matrix4& matrix4::operator*=(float _other) {
		*this = *this * _other;
		return *this;
	}

	matrix4 matrix4::operator+(matrix4 const& _other) {
		matrix4 temp;
		temp.matrix = matrix + _other.matrix;
		return temp;
	}

	matrix4& matrix4::operator+=(matrix4 const& _other) {
		*this = *this + _other;
		return *this;
	}

	vec4f& matrix4::operator[](unsigned int _index) {
		return rows[_index];
	}

	vec4f matrix4::operator[](unsigned int _index) const {
		return rows[_index];
	}

	vec4f & matrix4::operator[](int _index) {
		return rows[_index];
	}

	vec4f matrix4::operator[](int _index) const {
		return rows[_index];
	}

	matrix4 matrix4::Invert() {
		return DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(matrix), matrix);
	}

	matrix4 matrix4::Transpose() {
		return XMMatrixTranspose(matrix);
	}

	matrix4 & matrix4::ToIdentity() {
		rows[0].Set(1, 0, 0, 0);
		rows[1].Set(0, 1, 0, 0);
		rows[2].Set(0, 0, 1, 0);
		rows[3].Set(0, 0, 0, 1);
		return *this;
	}

	matrix4& matrix4::RotateInPlace(float _x, float _y, float _z, float _rads) {
		vec3f axis;
		axis.Set(_x, _y, _z);
		return RotateInPlace(axis, _rads);
	}

	matrix4 & matrix4::RotateInPlace(const vec3f& _axis, float _rads) {
		vec4f pos = Position;
		Position.Set(0, 0, 0, 1);
		*this *= CreateNewAxisRotation(_axis, _rads);
		Position = pos;
		return *this;
	}

	matrix4 & matrix4::RotateInPlaceQuaternion(const vec4f & _quat) {
		vec4f pos = Position;
		Position.Set(0, 0, 0, 1);
		*this *= CreateNewQuaternionRotation(_quat);
		Position = pos;
		return *this;
	}

	matrix4 matrix4::CreateAxisRotation(const vec3f & _axis, float _rads)
	{
		return *this * XMMatrixRotationAxis(_axis.vector, _rads);
	}

	matrix4 matrix4::CreateQuaternionRotation(const vec4f & _quat)
	{
		return *this * XMMatrixRotationQuaternion(_quat.vector);
	}

	matrix4 matrix4::CreateXRotation(float _rads)
	{
		return *this * XMMatrixRotationX(_rads);
	}

	matrix4 matrix4::CreateYRotation(float _rads)
	{
		return *this * XMMatrixRotationY(_rads);
	}

	matrix4 matrix4::CreateZRotation(float _rads)
	{
		return *this * XMMatrixRotationZ(_rads);
	}

	matrix4 matrix4::CreateYawPitchRollRotation(vec3f & _xyz)
	{
		return *this * XMMatrixRotationRollPitchYawFromVector(_xyz.vector);
	}

	matrix4 matrix4::CreateYawPitchRollRotation(float _x, float _y, float _z)
	{
		return *this * XMMatrixRotationRollPitchYaw(_x, _y, _z);
	}

	matrix4 matrix4::CreateScale(float _x, float _y, float _z)
	{
		return *this * XMMatrixScaling(_x, _y, _z);
	}

	matrix4 matrix4::CreateScale(vec3f & _scale)
	{
		return *this * XMMatrixScalingFromVector(_scale.vector);
	}

	matrix4 matrix4::CreateTranslation(float _x, float _y, float _z)
	{
		return *this * XMMatrixTranslation(_x, _y, _z);
	}

	matrix4 matrix4::CreateTranslation(vec3f _vec)
	{
		return *this * CreateNewTranslation(_vec.x, _vec.y, _vec.z);
	}

	matrix4 matrix4::CreateTranslation(vec4f _vec)
	{
		return *this * CreateNewTranslation(_vec.x, _vec.y, _vec.z);
	}

	matrix4 matrix4::CreateNewAxisRotation(const vec3f & _axis, float _rads) {
		return XMMatrixRotationAxis(_axis.vector, _rads);
	}

	matrix4 matrix4::CreateNewQuaternionRotation(const vec4f & _quat) {
		return XMMatrixRotationQuaternion(_quat.vector);
	}

	matrix4 matrix4::CreateNewXRotation(float _rads) {
		return XMMatrixRotationX(_rads);
	}

	matrix4 matrix4::CreateNewYRotation(float _rads) {
		return XMMatrixRotationY(_rads);
	}

	matrix4 matrix4::CreateNewZRotation(float _rads) {
		return XMMatrixRotationZ(_rads);
	}

	matrix4 matrix4::CreateNewYawPitchRollRotation(vec3f & _xyz) {
		return XMMatrixRotationRollPitchYawFromVector(_xyz.vector);
	}

	matrix4 matrix4::CreateNewYawPitchRollRotation(float _x, float _y, float _z) {
		return XMMatrixRotationRollPitchYaw(_x, _y, _z);
	}

	matrix4 matrix4::CreateNewScale(float _x, float _y, float _z) {
		return XMMatrixScaling(_x, _y, _z);
	}

	matrix4 matrix4::CreateNewScale(vec3f & _scale) {
		return XMMatrixScalingFromVector(_scale.vector);
	}

	matrix4 matrix4::CreateNewTranslation(float _x, float _y, float _z) {
		return XMMatrixTranslation(_x, _y, _z);
	}

	matrix4 matrix4::CreateNewTranslation(vec3f _vec)
	{
		return CreateNewTranslation(_vec.x, _vec.y, _vec.z);
	}

	matrix4 matrix4::CreateNewTranslation(vec4f _vec)
	{
		return CreateNewTranslation(_vec.x, _vec.y, _vec.z);
	}

}