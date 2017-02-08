#include "stdafx.h"
#include "Transform.h"

//Transform const * Transform::operator=(Transform _t)
//{
//	if (this->up != _t.up) this->up = _t.up;
//	if (this->right != _t.right) this->right = _t.right;
//	if (this->forward != _t.forward) this->forward = _t.forward;
//	if (this->m_matrix != _t.m_matrix) this->m_matrix = _t.m_matrix;
//	return this;
//}
//
//bool const * Transform::operator==(Transform _t)
//{
//	if (this->up != _t.up) return false;
//	if (this->right != _t.right) return false;
//	if (this->forward != _t.forward) return false;
//	if (this->m_matrix != _t.m_matrix) return false;
//	return true;
//}
//
//bool const * Transform::operator!=(Transform _t)
//{
//	if (this->up != _t.up || 
//		this->right != _t.right ||
//		this->forward != _t.forward || 
//		this->m_matrix != _t.m_matrix) 
//		return true;
//	return false;
//}
