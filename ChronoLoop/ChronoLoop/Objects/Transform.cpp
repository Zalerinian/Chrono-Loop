//#include "stdafx.h"
#include "Transform.h"

namespace Epoch
{

	void Transform::TranslateLocal(float x, float y, float z)
	{
		matrix4 temp;
		temp[0] = this->mRight;
		temp[1] = this->mUp;
		temp[2] = this->mFoward;

		temp = matrix4::CreateTranslation(x, y, z);
	}

	void Transform::Rotate(vec4f axis, float radians)
	{
		matrix4 rotation = matrix4::CreateAxisRotation(axis, radians);
		this->mMatrix *= rotation;
		this->mFoward *= rotation;
		this->mRight *= rotation;
		this->mUp *= rotation;
	}

	Transform& Transform::operator=(Transform const&	 _t)
	{
		mUp = _t.mUp;
		mRight = _t.mRight;
		mFoward = _t.mFoward;
		mMatrix = _t.mMatrix;
		return *this;
	}

	bool Transform::operator==(Transform const& _t)
	{
		if (!(this->mUp == _t.mUp)) return false;
		if (!(this->mRight == _t.mRight)) return false;
		if (!(this->mFoward == _t.mFoward)) return false;
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