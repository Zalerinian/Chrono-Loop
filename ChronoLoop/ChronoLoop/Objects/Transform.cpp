//#include "stdafx.h"
#include "Transform.h"

namespace Epoch
{

	//void Transform::TranslateLocal(float x, float y, float z)
	//{
	//	matrix4 temp;
	//	temp[0] = this->mRight;
	//	temp[1] = this->mUp;
	//	temp[2] = this->mFoward;
	//
	//	temp = matrix4::CreateNewTranslation(x, y, z);
	//}

	void Transform::Rotate(vec4f axis, float radians)
	{
		matrix4 rotation = matrix4::CreateNewAxisRotation(axis, radians);
		this->mMatrix *= rotation;
	}

	Transform& Transform::operator=(Transform const&	 _t)
	{
		mMatrix = _t.mMatrix;
		return *this;
	}

	bool Transform::operator==(Transform const& _t)
	{
		if (!(this->mMatrix == _t.mMatrix)) return false;
		return true;
	}

	bool Transform::operator!=(Transform const& _t)
	{
		if (!((*this) == _t))
		{
			return true;
		}
		return false;
	}


}