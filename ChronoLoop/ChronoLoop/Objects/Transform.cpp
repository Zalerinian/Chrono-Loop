#include "stdafx.h"
#include "Transform.h"

void Transform::TranslateLocal(float x, float y, float z)
{
	matrix4 temp;
	temp[0] = this->right;
	temp[1] = this->up;
	temp[2] = this->forward;

	temp = MatrixTranslation(x, y, z);
}

Transform const * Transform::operator=(Transform const _t)
{
	if (!(this->up == _t.up)) this->up = _t.up;
	if (!(this->right == _t.right)) this->right = _t.right;
	if (!(this->forward == _t.forward)) this->forward = _t.forward;
	if (!(this->m_worldMatrix == _t.m_worldMatrix)) this->m_worldMatrix = _t.m_worldMatrix;
	return this;
}

bool Transform::operator==(Transform const _t)
{
	if (!(this->up == _t.up)) return false;
	if (!(this->right == _t.right)) return false;
	if (!(this->forward == _t.forward)) return false;
	if (!(this->m_worldMatrix == _t.m_worldMatrix)) return false;
	return true;
}

bool Transform::operator!=(Transform const _t)
{
	if (!(this->up == _t.up) || 
		!(this->right == _t.right) ||
		!(this->forward == _t.forward) || 
		!(this->m_worldMatrix == _t.m_worldMatrix)) 
		return true;
	return false;
}

