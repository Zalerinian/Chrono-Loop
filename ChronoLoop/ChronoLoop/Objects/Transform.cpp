#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
{
	
}

Transform::~Transform()
{
}

Transform & Transform::operator=(const Transform & _copy)
{
	// TODO: insert return statement here
	this->m_matrix = _copy.m_matrix;
	this->forward = _copy.forward;
	this->right = _copy.right;
	this->up = _copy.up;
	

}

matrix4& Transform::GetMatrix()
{
	return m_matrix;
}

