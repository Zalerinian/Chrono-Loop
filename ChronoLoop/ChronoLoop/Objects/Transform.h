#pragma once
#include "../Common/Math.h"
using namespace Math;
class Transform
{
private:
	matrix4 mMatrix;//World
	vec4f mUp, mFoward, mRight; //Local//y,z,x
	
public:
	void TranslateLocal(float x, float y, float z);
	void TranslateGlobal(float x, float y, float z) { this->mMatrix = MatrixTranslation(x, y, z); };
	void Rotate(vec4f axis, float radians) { this->mMatrix = MatrixRotateAxis(axis, radians); };
	void RotateAround(vec4f point, vec4f axis, float radians) {/*Insert Code Here*/ };
	void RotateInPlace(vec4f& _axis, float _rads) { /*MatrixRotateInPlace()*/ };
	void Scale(float x, float y, float z) { this->mMatrix = MatrixScale(x, y, z); };

	inline matrix4& GetMatrix() { return mMatrix; };
	inline void GetMatrix(matrix4& _filler) { _filler = mMatrix; }
	inline void SetMatrix(matrix4& _mat) { mMatrix = _mat; };
	//**GETTERS**//
	const vec4f*			GetXAxis() { return reinterpret_cast<vec4f*> (&mMatrix.first); };
	const vec4f*			GetYAxis() { return reinterpret_cast<vec4f*> (&mMatrix.second); };
	const vec4f*			GetZAxis() { return reinterpret_cast<vec4f*> (&mMatrix.third); };
	const vec4f*			GetPosition() { return reinterpret_cast<vec4f*> (&mMatrix.fourth); };

	//**OPERATORS**//
	Transform& operator=(Transform const& _t);
	bool operator==(Transform const& _t);
	bool operator!=(Transform const& _t);
};