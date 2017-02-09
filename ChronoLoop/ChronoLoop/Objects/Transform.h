#pragma once
#include "../Common/Math.h"

class Transform
{
private:
	matrix4 m_matrix;
	vec4f up,forward,right;
public:
	Transform();
	~Transform();
	Transform& operator=(const Transform& _copy);
	matrix4& GetMatrix();
	//Transform& Translate(float x, float y, float z) { /*Insert Code Here*/ };
	//Transform& TranslateGlobal(float x, float y, float z) {/*Insert Code Here*/ };
	//Transform& Rotate(vec4f axis, float radians) {/*Insert Code Here*/ };
	//Transform& RotateAround(vec4f point, vec4f axis, float radians) {/*Insert Code Here*/ };
	//Transform& RotateInPlace(float x, float y, float z) {/*Insert Code Here*/ };
	//Transform& Scale(float x, float y, float z) {/*Insert Code Here*/ };
};
