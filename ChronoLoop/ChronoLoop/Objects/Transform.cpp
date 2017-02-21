//#include "stdafx.h"
#include "Transform.h"

void Transform::TranslateLocal(float x, float y, float z) {
	matrix4 temp;
	temp[0] = this->mRight;
	temp[1] = this->mUp;
	temp[2] = this->mFoward;

	temp = MatrixTranslation(x, y, z);
}

Transform& Transform::operator=(Transform const&	 _t) {
	mUp     = _t.mUp;
	mRight  = _t.mRight;
	mFoward = _t.mFoward;
	mMatrix = _t.mMatrix;
	return *this;
}

bool Transform::operator==(Transform const& _t) {
	if (!(this->mUp == _t.mUp)) return false;
	if (!(this->mRight == _t.mRight)) return false;
	if (!(this->mFoward == _t.mFoward)) return false;
	if (!(this->mMatrix == _t.mMatrix)) return false;
	return true;
}

bool Transform::operator!=(Transform const& _t) {
	if (!((*this) == _t)) {
		return true;
	}
	return false;
}

