//#include "stdafx.h"
#include "Transform.h"

void Transform::TranslateLocal(float x, float y, float z)
{
	matrix4 temp;
	temp[0] = this->m_right;
	temp[1] = this->m_up;
	temp[2] = this->m_foward;

	temp = MatrixTranslation(x, y, z);
}

Transform const * Transform::operator=(Transform const _t)
{
	if (!(this->m_up == _t.m_up)) this->m_up = _t.m_up;
	if (!(this->m_right == _t.m_right)) this->m_right = _t.m_right;
	if (!(this->m_foward == _t.m_foward)) this->m_foward = _t.m_foward;
	if (!(this->m_worldMatrix == _t.m_worldMatrix)) this->m_worldMatrix = _t.m_worldMatrix;
	return this;
}

bool Transform::operator==(Transform const _t)
{
	if (!(this->m_up == _t.m_up)) return false;
	if (!(this->m_right == _t.m_right)) return false;
	if (!(this->m_foward == _t.m_foward)) return false;
	if (!(this->m_worldMatrix == _t.m_worldMatrix)) return false;
	return true;
}

bool Transform::operator!=(Transform const _t)
{
	if (!(this->m_up == _t.m_up) || 
		!(this->m_right == _t.m_right) ||
		!(this->m_foward == _t.m_foward) || 
		!(this->m_worldMatrix == _t.m_worldMatrix)) 
		return true;
	return false;
}

