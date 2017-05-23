//#include "stdafx.h"
#include "Physics.h"
#include "..\Common\Settings.h"
#include "..\Objects\BaseObject.h"
#include "..\Rendering\Mesh.h"
#include "..\Common\Logger.h"
#include "..\Objects\Component.h"
#include "..\Actions\CodeComponent.hpp"
#include "..\Input\VRInputManager.h"
#include "..\Core\LevelManager.h"
#include "../Common/Settings.h"

#define DEBUG_LEVEL1 0
#define DEBUG_LEVEL2 0

namespace Epoch
{

	Physics* Physics::mInstance;

	Physics::Physics()
	{
		mInstance = nullptr;
	}

	Physics* Physics::Instance()
	{
		if (!mInstance)
			mInstance = new Physics();
		return mInstance;
	}

	void Physics::Destroy()
	{
		if (mInstance)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}


#pragma region RAY_CASTING

	bool Physics::RayToTriangle(vec3f& _vert0, vec3f& _vert1, vec3f& _vert2, vec3f& _normal, vec3f& _start, vec3f& _dir, float& _time)
	{
		vec3f rayToCentroid = ((_vert0 + _vert1 + _vert2) / 3) - _start;
		if ((_normal * _dir) > 0.001f || (rayToCentroid * _normal) > 0)
		{
			return false;
		}

		vec3f sa = _vert0 - _start;
		vec3f sb = _vert1 - _start;
		vec3f sc = _vert2 - _start;
		vec3f N1, N2, N3;
		N1 = sc ^ sb;
		N2 = sa ^ sc;
		N3 = sb ^ sa;
		float dn1 = _dir * N1;
		float dn2 = _dir * N2;
		float dn3 = _dir * N3;

		//if (dn1 == 0.0f && dn2 == 0.0f && dn3 == 0.0f)
		//{
		//	_time = 0;
		//	return true;
		//}

		if (SameSign(dn1, dn2) && SameSign(dn2, dn3))
		{
			float offset = _normal * _vert0;
			_time = ((offset - (_start * _normal)) / (_normal * _dir));
			return true;
		}
		return false;
	}

	bool Physics::RayToSphere(vec3f & _pos, vec3f & _dir, vec3f & _center, float _radius, float & _time, vec3f & _out)
	{
		vec3f check = _pos - _center;

		vec3f m = _pos - _center;
		float b = m * _dir;
		float c = (m * m) - (_radius * _radius);
		if (c < 0.0f || b > 0.0f)
			return false;

		float dis = b*b - c;
		if (dis < 0.0f)
			return false;

		_time = -b - sqrtf(dis);
		if (_time < 0.0f)
		{
			_time = 0.0f;
			vec3f np = { _pos.x + _time, _pos.y + _time, _pos.z + _time };
			_out = np ^ _dir;
		}

		return true;
	}

	bool Physics::RayToCylinder(vec3f & _start, vec3f & _normal, vec3f & _point1, vec3f & _point2, float _radius, float & _time)
	{
		vec3f d = _point2 - _point1;
		vec3f od = _point1 - _point2;
		vec3f m = _start - _point1;

		float dd = d * d;
		float nd = _normal * d;
		float mn = m * _normal;
		float md = m * d;
		float mm = m * m;

		if ((md < 0 && nd < 0) || (md > dd && nd > 0))
			return false;

		float a = dd - nd * nd;
		float b = dd * mn - nd * md;
		float c = dd * (mm - _radius * _radius) - md * md;

		if (fabsf(a) < FLT_EPSILON)
			return false;

		float dis = b*b - a*c;
		if (dis < 0)
			return false;

		_time = (-b - sqrtf(dis)) / a;

		if (_time < 0)
			return false;

		vec3f col = _start + _normal * _time;

		if (od * (_point2 - col) > 0 || d * (_point1 - col) > 0)
			return false;

		return true;
	}

	bool Physics::RayToCapsule(vec3f & _start, vec3f & _normal, vec3f & _point1, vec3f & _point2, float _radius, float & _time)
	{
		float fTime = FLT_MAX;
		_time = FLT_MAX;
		bool bReturn = false;

		if (RayToCylinder(_start, _normal, _point1, _point2, _radius, _time))
		{
			fTime = PMIN(_time, fTime);
			_time = fTime;
			bReturn = true;
		}

		vec3f pcol, qcol;
		if (RayToSphere(_start, _normal, _point1, _radius, _time, pcol))
		{
			fTime = PMIN(_time, fTime);
			_time = fTime;
			bReturn = true;
		}

		if (RayToSphere(_start, _normal, _point2, _radius, _time, qcol))
		{
			fTime = PMIN(_time, fTime);
			_time = fTime;
			bReturn = true;
		}
		return bReturn;
	}

	bool Physics::Linecast(vec3f& _vert0, vec3f& _vert1, vec3f& _vert2, vec3f& _normal, vec3f& _start, vec3f& _end, vec3f& _hit)
	{
		float time;
		vec3f dir = _end - _start;
		if (RayToTriangle(_vert0, _vert1, _vert2, _normal, _start, dir, time))
		{

			vec3f proj = _start + (dir)* time;
			_hit = proj;
			proj = proj - _end;
			float d = proj.Dot(dir);

			if (d < 0)
				return true;
			else
				return false;
		}
		return false;

	}
#pragma endregion


#pragma region COLISION

	//Returns 1 if on or in front of plane
	//Returns 2 if behind plane
	int Physics::PointToPlane(PlaneCollider& _plane, vec3f& _point)
	{
		float pOffset = _point * _plane.mNormal;
		if (pOffset >= _plane.mOffset)
			return 1;
		return 2;
	}

	//Returns 1 if in front of plane
	//Returns 2 if behind plane
	//Returns 3 if intersecting plane
	int Physics::SphereToPlane(PlaneCollider& _plane, SphereCollider& _sphere)
	{
		float pOffset = _sphere.mCenter * _plane.mNormal;
		if (pOffset - _plane.mOffset > _sphere.mRadius)
			return 1;
		else if (pOffset - _plane.mOffset < -_sphere.mRadius)
			return 2;
		return 3;
	}

	//Returns 1 if in front of plane.
	//Returns 2 if behind plane.
	//Returns 3 if intersecting plane.
	int Physics::AabbToPlane(PlaneCollider& _plane, CubeCollider& _aabb)
	{
		vec3f center = (_aabb.mMax + _aabb.mMin) * 0.5f;
		float pOffset = center * _plane.mNormal - _plane.mOffset;
		vec3f E = _aabb.mMax - center;
		float r = E * _plane.mNormal;
		SphereCollider s(center, r);
		int t = SphereToPlane(_plane, s);
		return t;
	}

	bool Physics::AABBtoAABB(CubeCollider& _aabb1, CubeCollider& _aabb2)
	{
		if (_aabb1.mMax.x < _aabb2.mMin.x || _aabb1.mMin.x > _aabb2.mMax.x)
			return false;

		if (_aabb1.mMax.y < _aabb2.mMin.y || _aabb1.mMin.y > _aabb2.mMax.y)
			return false;

		if (_aabb1.mMax.z < _aabb2.mMin.z || _aabb1.mMin.z >  _aabb2.mMax.z)
			return false;

		return true;
	}

	bool Epoch::Physics::OBBtoOBB(OrientedCubeCollider& _obb1, OrientedCubeCollider& _obb2)
	{
		vec3f T = _obb2.GetPos() - _obb1.GetPos();

		vec3f xRot1 = _obb1.mObject->GetTransform().GetMatrix().first;
		vec3f yRot1 = _obb1.mObject->GetTransform().GetMatrix().second;
		vec3f zRot1 = -_obb1.mObject->GetTransform().GetMatrix().third;

		vec3f xRot2 = _obb2.mObject->GetTransform().GetMatrix().first;
		vec3f yRot2 = _obb2.mObject->GetTransform().GetMatrix().second;
		vec3f zRot2 = -_obb2.mObject->GetTransform().GetMatrix().third;

		//X1 Axis
		if (fabsf(T * _obb1.mAxis[0]) >
			_obb1.mWidth +
			fabsf(_obb2.mWidth * (xRot1 * xRot2)) +
			fabsf(_obb2.mHeight * (xRot1 * yRot2)) +
			fabsf(_obb2.mDepth * (xRot1 * zRot2)))
			return false;

		//Y1 Axis
		if (fabsf(T * _obb1.mAxis[1]) >
			_obb1.mHeight +
			fabsf(_obb2.mWidth * (yRot1 * xRot2)) +
			fabsf(_obb2.mHeight * (yRot1 * yRot2)) +
			fabsf(_obb2.mDepth * (yRot1 * zRot2)))
			return false;

		//Z1 Axis
		if (fabsf(T * _obb1.mAxis[2]) >
			_obb1.mDepth +
			fabsf(_obb2.mWidth * (zRot1 * xRot2)) +
			fabsf(_obb2.mHeight * (zRot1 * yRot2)) +
			fabsf(_obb2.mDepth * (zRot1 * zRot2)))
			return false;

		//X2 Axis
		if (fabsf(T * _obb2.mAxis[0]) >
			fabsf(_obb1.mWidth * (xRot1 * xRot2)) +
			fabsf(_obb1.mHeight * (yRot1 * xRot2)) +
			fabsf(_obb1.mDepth * (zRot1 * xRot2)) +
			_obb2.mWidth)
			return false;

		//Y2 Axis
		if (fabsf(T * _obb2.mAxis[1]) >
			fabsf(_obb1.mWidth * (xRot1 * yRot2)) +
			fabsf(_obb1.mHeight * (yRot1 * yRot2)) +
			fabsf(_obb1.mDepth * (zRot1 * yRot2)) +
			_obb2.mHeight)
			return false;

		//Z2 Axis
		if (fabsf(T * _obb2.mAxis[2]) >
			fabsf(_obb1.mWidth * (xRot1 * zRot2)) +
			fabsf(_obb1.mHeight * (yRot1 * zRot2)) +
			fabsf(_obb1.mDepth * (zRot1 * zRot2)) +
			_obb2.mDepth)
			return false;

		//X1 x X2
		if (fabsf(((T * _obb1.mAxis[2]) * (yRot1 * xRot2)) - ((T * _obb1.mAxis[1]) * (zRot1 * xRot2))) >
			fabsf(_obb1.mHeight * (zRot1 * xRot2)) +
			fabsf(_obb1.mDepth * (yRot1 * xRot2)) +
			fabsf(_obb2.mHeight * (xRot1 * zRot2)) +
			fabsf(_obb2.mDepth * (xRot1 * yRot2)))
			return false;

		//X1 x Y2
		if (fabsf(((T * _obb1.mAxis[2]) * (yRot1 * yRot2)) - ((T * _obb1.mAxis[1]) * (zRot1 * yRot2))) >
			fabsf(_obb1.mHeight * (zRot1 * yRot2)) +
			fabsf(_obb1.mDepth * (yRot1 * yRot2)) +
			fabsf(_obb2.mWidth * (xRot1 * zRot2)) +
			fabsf(_obb2.mDepth * (xRot1 * xRot2)))
			return false;

		//X1 x Z2
		if (fabsf(((T * _obb1.mAxis[2]) * (yRot1 * zRot2)) - ((T * _obb1.mAxis[1]) * (zRot1 * zRot2))) >
			fabsf(_obb1.mHeight * (zRot1 * zRot2)) +
			fabsf(_obb1.mDepth * (yRot1 * zRot2)) +
			fabsf(_obb2.mWidth * (xRot1 * yRot2)) +
			fabsf(_obb2.mHeight * (xRot1 * xRot2)))
			return false;

		//Y1 x X2
		if (fabsf(((T * _obb1.mAxis[0]) * (zRot1 * xRot2)) - ((T * _obb1.mAxis[2]) * (xRot1 * xRot2))) >
			fabsf(_obb1.mWidth * (zRot1 * xRot2)) +
			fabsf(_obb1.mDepth * (xRot1 * xRot2)) +
			fabsf(_obb2.mHeight * (yRot1 * zRot2)) +
			fabsf(_obb2.mDepth * (yRot1 * yRot2)))
			return false;

		//Y1 x Y2
		if (fabsf(((T * _obb1.mAxis[0]) * (zRot1 * yRot2)) - ((T * _obb1.mAxis[2]) * (xRot1 * yRot2))) >
			fabsf(_obb1.mWidth * (zRot1 * yRot2)) +
			fabsf(_obb1.mDepth * (xRot1 * yRot2)) +
			fabsf(_obb2.mWidth * (yRot1 * zRot2)) +
			fabsf(_obb2.mDepth * (yRot1 * xRot2)))
			return false;

		//Y1 x Z2
		if (fabsf(((T * _obb1.mAxis[0]) * (zRot1 * zRot2)) - ((T * _obb1.mAxis[2]) * (xRot1 * zRot2))) >
			fabsf(_obb1.mWidth * (zRot1 * zRot2)) +
			fabsf(_obb1.mDepth * (xRot1 * zRot2)) +
			fabsf(_obb2.mWidth * (yRot1 * yRot2)) +
			fabsf(_obb2.mHeight * (yRot1 * xRot2)))
			return false;

		//Z1 x X2
		if (fabsf(((T * _obb1.mAxis[1]) * (xRot1 * xRot2)) - ((T * _obb1.mAxis[0]) * (yRot1 * xRot2))) >
			fabsf(_obb1.mWidth * (yRot1 * xRot2)) +
			fabsf(_obb1.mHeight * (xRot1 * xRot2)) +
			fabsf(_obb2.mHeight * (zRot1 * zRot2)) +
			fabsf(_obb2.mDepth * (zRot1 * yRot2)))
			return false;

		//z1 x Y2
		if (fabsf(((T * _obb1.mAxis[1]) * (xRot1 * yRot2)) - ((T * _obb1.mAxis[0]) * (yRot1 * yRot2))) >
			fabsf(_obb1.mWidth * (yRot1 * yRot2)) +
			fabsf(_obb1.mHeight * (xRot1 * yRot2)) +
			fabsf(_obb2.mWidth * (zRot1 * zRot2)) +
			fabsf(_obb2.mDepth * (zRot1 * xRot2)))
			return false;

		//z1 x Z2
		if (fabsf(((T * _obb1.mAxis[1]) * (xRot1 * zRot2)) - ((T * _obb1.mAxis[0]) * (yRot1 * zRot2))) >
			fabsf(_obb1.mWidth * (yRot1 * zRot2)) +
			fabsf(_obb1.mHeight * (xRot1 * zRot2)) +
			fabsf(_obb2.mWidth * (zRot1 * yRot2)) +
			fabsf(_obb2.mHeight * (zRot1 * xRot2)))
			return false;

		return true;
	}

	bool Epoch::Physics::OBBtoPlane(OrientedCubeCollider & _obb, PlaneCollider & _plane)
	{
		return fabsf((_obb.GetPos() * _plane.mNormal) - _plane.mOffset) <=
			(_obb.mWidth * fabsf(_obb.mAxis[0].Magnitude())) +
			(_obb.mHeight * fabsf(_obb.mAxis[1].Magnitude())) +
			(_obb.mDepth * fabsf(_obb.mAxis[2].Magnitude()));
	}

	bool Physics::SphereToSphere(SphereCollider& _sphere1, SphereCollider& _sphere2)
	{
		vec3f pos = _sphere1.GetPos() - _sphere2.GetPos();
		float distance = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
		float minDist = _sphere1.mRadius + _sphere2.mRadius;

		return distance <= (minDist * minDist);
	}

	bool Physics::SphereToAABB(SphereCollider& _sphere, CubeCollider& _aabb)
	{
		float X, Y, Z;
		vec3f pos = _sphere.GetPos();

		if (pos.x < _aabb.mMin.x)
			X = _aabb.mMin.x;
		else if (pos.x > _aabb.mMax.x)
			X = _aabb.mMax.x;
		else
			X = pos.x;

		if (pos.y < _aabb.mMin.y)
			Y = _aabb.mMin.y;
		else if (pos.y > _aabb.mMax.y)
			Y = _aabb.mMax.y;
		else
			Y = pos.y;

		if (pos.z < _aabb.mMin.z)
			Z = _aabb.mMin.z;
		else if (pos.z > _aabb.mMax.z)
			Z = _aabb.mMax.z;
		else
			Z = pos.z;

		vec3f point = { X, Y, Z };

		return (fabsf((point.x - pos.x)) < _sphere.mRadius &&
			fabsf((point.y - pos.y)) < _sphere.mRadius &&
			fabsf((point.z - pos.z)) < _sphere.mRadius);
	}

	bool Physics::SphereToTriangle(SphereCollider& _sphere, Triangle& _tri, vec3f& _displacement)
	{
		vec3f pos = _sphere.GetPos();
		float offset = (pos - _tri.Vertex[0]) * _tri.Normal;
		vec3f scaled = _tri.Normal * offset;
		vec3f projected = pos - scaled;
		vec3f edge0 = _tri.Vertex[1] - _tri.Vertex[0];
		vec3f edge1 = _tri.Vertex[2] - _tri.Vertex[1];
		vec3f edge2 = _tri.Vertex[0] - _tri.Vertex[2];
		vec3f norm0, norm1, norm2, Cpt = pos;
		norm0 = edge0 ^ _tri.Normal;
		norm1 = edge1 ^ _tri.Normal;
		norm2 = edge2 ^ _tri.Normal;
		PlaneCollider plane0(norm0, _tri.Vertex[0] * norm0);
		PlaneCollider plane1(norm1, _tri.Vertex[1] * norm1);
		PlaneCollider plane2(norm2, _tri.Vertex[2] * norm2);

		if (PointToPlane(plane0, projected) == 1 &&
			PointToPlane(plane1, projected) == 1 &&
			PointToPlane(plane2, projected) == 1)
		{
			Cpt = projected;
		}
		else
		{
			vec3f s0 = projected - _tri.Vertex[0];
			vec3f s1 = projected - _tri.Vertex[1];
			vec3f s2 = projected - _tri.Vertex[2];
			float p0 = (s0 * edge0) / (edge0 * edge0);
			float p1 = (s1 * edge1) / (edge1 * edge1);
			float p2 = (s2 * edge2) / (edge2 * edge2);

			if (p0 < 0)
				p0 = 0;
			else if (p0 > 1)
				p0 = 1;

			if (p1 < 1)
				p1 = 0;
			else if (p1 > 1)
				p1 = 1;

			if (p2 < 0)
				p2 = 0;
			else if (p2 > 1)
				p2 = 1;

			vec3f c0 = _tri.Vertex[0] + edge0 * p0;
			vec3f c1 = _tri.Vertex[1] + edge1 * p1;
			vec3f c2 = _tri.Vertex[2] + edge2 * p2;
			float r0 = (projected - c0) * (projected - c0);
			float r1 = (projected - c1) * (projected - c1);
			float r2 = (projected - c2) * (projected - c2);

			float min = fminf(fminf(r0, r1), r2);
			if (min == r0)
				Cpt = c0;
			else if (min == r1)
				Cpt = c1;
			else if (min == r2)
				Cpt = c2;
		}

		if ((Cpt - pos) * (Cpt - pos) < (_sphere.mRadius * _sphere.mRadius))
		{
			vec3f v = pos - Cpt;
			float dist = v.Magnitude();
			vec3f n = v.Normalize();
			_displacement = n * (_sphere.mRadius - dist);
			return true;
		}

		return false;
	}

	bool Physics::MovingSphereToTriangle(vec3f & _vert0, vec3f & _vert1, vec3f & _vert2, vec3f & _normal, vec3f & _start, vec3f & _dir, float _radius, float & _time, vec3f & _outNormal)
	{
		bool bReturn = false;
		float fTime = FLT_MAX;
		_time = FLT_MAX;

		vec3f verts[3] = { _vert0, _vert1, _vert2 };
		vec3f offset0 = _vert0 + (_normal * _radius);
		vec3f offset1 = _vert1 + (_normal * _radius);
		vec3f offset2 = _vert2 + (_normal * _radius);

		if (RayToTriangle(offset0, offset1, offset2, _normal, _start, _dir, _time))
		{
			fTime = PMIN(_time, fTime);
			_time = fTime;
			_outNormal = _normal;
			bReturn = true;
		}
		else
		{
			if (RayToCapsule(_start, _dir, _vert0, _vert1, _radius, _time))
			{
				fTime = PMIN(_time, fTime);
				_time = fTime;
				vec3f temp1 = _dir * _time + _start;
				vec3f temp2 = (_vert1 - _vert0) * _time + _vert0;
				_outNormal = (temp1 - temp2).Normalize();
				bReturn = true;
			}

			if (RayToCapsule(_start, _dir, _vert1, _vert2, _radius, _time))
			{
				fTime = PMIN(_time, fTime);
				_time = fTime;
				vec3f temp1 = _dir * _time + _start;
				vec3f temp2 = (_vert1 - _vert0) * _time + _vert0;
				_outNormal = (temp1 - temp2).Normalize();
				bReturn = true;
			}

			if (RayToCapsule(_start, _dir, _vert2, _vert0, _radius, _time))
			{
				fTime = PMIN(_time, fTime);
				_time = fTime;
				vec3f temp1 = _dir * _time + _start;
				vec3f temp2 = (_vert1 - _vert0) * _time + _vert0;
				_outNormal = (temp1 - temp2).Normalize();
				bReturn = true;
			}
		}

		return bReturn;
	}

	//bool Physics::MovingSphereToMesh(vec3f & _start, vec3f & _dir, float _radius, Mesh* _mesh, float & _time, vec3f & _outNormal)
	//{
	//	bool bCollision = false;
	//	_time = FLT_MAX;
	//	float fTime = FLT_MAX;
	//
	//	for (unsigned int i = 0; i < _mesh->GetNumTriangles(); i++)
	//	{
	//		Triangle currTri = _mesh->GetTriangles()[i];
	//		vec3f currNorm = _mesh->GetTriangles()[i].Normal;
	//
	//		if (MovingSphereToTriangle(
	//			vec3f(currTri.Vertex[0]),
	//			vec3f(currTri.Vertex[1]),
	//			vec3f(currTri.Vertex[2]),
	//			currNorm, _start, _dir, _radius, fTime, _outNormal))
	//		{
	//			_time = fminf(_time, fTime);
	//			_outNormal = currNorm;
	//			bCollision = true;
	//		}
	//	}
	//
	//	return bCollision;
	//}

#pragma endregion


#pragma region FRUSTUM_COLLISION
	/*
	void Physics::BuildFrustum(Frustum& _frustum, float _fov, float _nearDist, float _farDist, float _ratio, matrix4& _camXform)
	{
	vec3f fc = _camXform.axis_pos - _camXform.axis_z * _farDist;
	vec3f nc = _camXform.axis_pos - _camXform.axis_z * _nearDist;
	float Hnear = 2 * tan(_fov / 2) * _nearDist;
	float Hfar = 2 * tan(_fov / 2) * _farDist;
	float Wnear = Hnear * _ratio;
	float Wfar = Hfar * _ratio;

	_frustum.mPoints[0] = fc + _camXform.axis_y * (Hfar * 0.5f) - _camXform.axis_x * (Wfar * 0.5f);
	_frustum.mPoints[3] = fc + _camXform.axis_y * (Hfar * 0.5f) + _camXform.axis_x * (Wfar * 0.5f);
	_frustum.mPoints[1] = fc - _camXform.axis_y * (Hfar * 0.5f) - _camXform.axis_x * (Wfar * 0.5f);
	_frustum.mPoints[2] = fc - _camXform.axis_y * (Hfar * 0.5f) + _camXform.axis_x * (Wfar * 0.5f);

	_frustum.mPoints[4] = nc + _camXform.axis_y * (Hnear * 0.5f) - _camXform.axis_x * (Wnear * 0.5f);
	_frustum.mPoints[5] = nc + _camXform.axis_y * (Hnear * 0.5f) + _camXform.axis_x * (Wnear * 0.5f);
	_frustum.mPoints[7] = nc - _camXform.axis_y * (Hnear * 0.5f) - _camXform.axis_x * (Wnear * 0.5f);
	_frustum.mPoints[6] = nc - _camXform.axis_y * (Hnear * 0.5f) + _camXform.axis_x * (Wnear * 0.5f);

	BuildPlane(_frustum.mFaces[0], _frustum.mPoints[6], _frustum.mPoints[7], _frustum.mPoints[4]);
	BuildPlane(_frustum.mFaces[1], _frustum.mPoints[1], _frustum.mPoints[2], _frustum.mPoints[3]);
	BuildPlane(_frustum.mFaces[2], _frustum.mPoints[7], _frustum.mPoints[1], _frustum.mPoints[0]);
	BuildPlane(_frustum.mFaces[3], _frustum.mPoints[2], _frustum.mPoints[6], _frustum.mPoints[5]);
	BuildPlane(_frustum.mFaces[4], _frustum.mPoints[5], _frustum.mPoints[4], _frustum.mPoints[0]);
	BuildPlane(_frustum.mFaces[5], _frustum.mPoints[7], _frustum.mPoints[6], _frustum.mPoints[2]);
	}

	bool Physics::FrustumToSphere(Frustum & _frustum, Sphere & _sphere)
	{
	int test = 0;
	for (int i = 0; i < 6; ++i)
	{
	if (SphereToPlane(_frustum.mFaces[i], _sphere) == 2)
	break;
	else
	test++;
	}

	if (test == 6)
	return true;
	return false;
	}

	bool Physics::FrustumToAABB(Frustum & _frustum, AABB & _aabb)
	{
	int test = 0;
	for (int i = 0; i < 6; ++i)
	{
	if (AabbToPlane(_frustum.mFaces[i], _aabb) == 2)
	break;
	else
	test++;
	}

	if (test == 6)
	return true;
	return false;
	}
	*/
#pragma endregion


#pragma region RIGIDBODY_SIMULATION

	vec3f Physics::CalcAcceleration(vec3f& _force, float _mass)
	{
		return _force / _mass;
	}

	vec3f Physics::CalcVelocity(vec3f& _vel, vec3f& _accel, float _time)
	{
		return _vel + _accel * _time;
	}

	vec3f Physics::CalcPosition(vec3f& _pos, vec3f& _vel, float _time)
	{
		return _pos + _vel * _time;
	}

	void Physics::CalcFriction(Collider& _col, vec3f& _norm, float _static, float _kinetic)
	{
		vec3f tangentForce = _col.mForces - (_norm * (_col.mForces * _norm));
		float staticFriction = 0;
		float avgStatic = _col.mStaticFriction < _static ? _static : _col.mStaticFriction;
		float avgKinetic = _col.mKineticFriction < _kinetic ? _kinetic : _col.mKineticFriction;

		if (fabs(_col.mVelocity.x) < 0.1f)
			_col.mVelocity.x = 0;
		if (fabsf(_col.mVelocity.y) < 0.1f)
			_col.mVelocity.y = 0;
		if (fabsf(_col.mVelocity.z) < 0.1f)
			_col.mVelocity.z = 0;

		if (fabsf(_col.mVelocity.Magnitude()) < 0.001f)
		{
			staticFriction = avgStatic * (-_col.mWeight).Magnitude();
			_col.mTotalForce = { 0,0,0 };
		}
		else if (_col.mVelocity.Magnitude() > 0)
		{
			//vec3f normDir(fabsf(_col.mVelocity.Normalize().x), fabsf(_col.mVelocity.Normalize().y), fabs(_col.mVelocity.Normalize().z), 1);
			vec3f totalFriction = _col.mVelocity.Normalize() * (-avgKinetic * (-_col.mWeight).Magnitude());
			_col.mTotalForce = _col.mForces + totalFriction + _col.mDragForce;
		}

		if (fabsf(_col.mVelocity.Magnitude()) < 0.001f && tangentForce.Magnitude() >= staticFriction)
		{
			float sliding = tangentForce.Magnitude() / staticFriction;
			_col.mVelocity = tangentForce.Normalize() * sliding;
			vec3f kineticFriction = _col.mVelocity.Normalize() * (-avgKinetic * (-_col.mWeight).Magnitude());
			_col.mTotalForce = _col.mForces + kineticFriction + _col.mDragForce;
		}
	}

#pragma endregion


	void Physics::Update(float _time)
	{
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

		//Time pause
		bool paused = false;
		//bool paused = false;
		//if(cLevel->GetPauseMenu() != nullptr)
		//{
		//	paused = cLevel->GetPauseMenu()->isPauseMenuOn();
		//}
		if (cLevel->GetTimeManipulator() != nullptr)
		{
			paused = cLevel->GetTimeManipulator()->isTimePaused();
		}
		if(Settings::GetInstance().GetInt("CurrentLevel") == 0 && Settings::GetInstance().GetBool("PauseMenuUp"))
		{
			Settings::GetInstance().SetBool("PauseMenuUp", false);
		}
		if (!paused && !Settings::GetInstance().GetBool("PauseMenuUp"))
		{
			//SystemLogger::GetLog() << _time << std::endl;
			Collider* collider = nullptr;
			Collider* otherCol = nullptr;
			vec3f norm;
			std::vector<Component*> codeComponents;
			std::vector<Component*> Colliders;
			std::vector<Component*> otherColliders;

			int objs = (int)mObjects.size();
			for (int i = 0; i < objs; ++i)
			{

#if DEBUG_LEVEL1
				if (mObjects[i]->GetName() == "mmDoor")
					((CodeComponent*)mObjects[i]->GetComponents(eCOMPONENT_CODE)[0])->OnTriggerEnter(*collider, *otherCol);
#endif

#if DEBUG_LEVEL2
				if (mObjects[i]->GetName() == "mmDoor2")
					((CodeComponent*)mObjects[i]->GetComponents(eCOMPONENT_CODE)[0])->OnTriggerEnter(*collider, *otherCol);
#endif

				Colliders = mObjects[i]->mComponents[eCOMPONENT_COLLIDER];
				int cols = (int)Colliders.size();
				for (int x = 0; x < cols; ++x)
				{
					collider = (Collider*)Colliders[x];
					if (collider->IsEnabled() && (collider->mIsTrigger || collider->mShouldMove))
					{
						if (!collider->mIsTrigger)
							collider->mTotalForce = collider->mForces + collider->mWeight + collider->mDragForce;

						codeComponents = collider->mObject->GetComponents(eCOMPONENT_CODE);
						if (collider->mColliderType == Collider::eCOLLIDER_Sphere)//Check SphereCollider's collision with other objects
						{
							SphereCollider* s1 = (SphereCollider*)collider;
							for (int j = 0; j < objs; ++j)
							{
								if (mObjects[j] != mObjects[i])
								{
									otherColliders = mObjects[j]->mComponents[eCOMPONENT_COLLIDER];
									int othercols = (int)otherColliders.size();
									for (int k = 0; k < othercols; ++k)
									{
										otherCol = (Collider*)otherColliders[k];
										if (otherCol->mIsEnabled)
										{
											//if (otherCol->mColliderType == Collider::eCOLLIDER_Mesh)
											//{
											//	//Not sure what outnorm is used for at the moment might just stick with basic cube/sphere collisions
											//	if (MovingSphereToMesh(collider->GetPos(), collider->mVelocity, ((SphereCollider*)collider)->mRadius, ((MeshCollider*)otherCol)->mMesh, _time, norm))
											//	{
											//		
											//	}
											//}
											if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere)
											{
												SphereCollider* s2 = (SphereCollider*)otherCol;
												if (SphereToSphere(*s1, *s2))
												{
													for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f)
													{
														if (collider->mIsTrigger)
															((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
														else
															((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
													}
												}
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Cube)
											{
												CubeCollider* aabb = (CubeCollider*)otherCol;
												if (SphereToAABB(*s1, *aabb))
												{
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														if (collider->mIsTrigger)
															((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
														else
															((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
													}
												}
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Plane)
											{
												PlaneCollider* plane = (PlaneCollider*)otherCol;
												int result = SphereToPlane(*plane, *s1);
												if (result == 2)//behind plane
												{
													float bottom = collider->GetPos().y - ((SphereCollider*)collider)->mRadius;
													if (bottom < ((PlaneCollider*)otherCol)->mOffset)
													{
														vec3f pos = collider->GetPos();
														collider->SetPos(vec3f(pos.x, ((PlaneCollider*)otherCol)->mOffset + ((SphereCollider*)collider)->mRadius, pos.z));

														CalcFriction(*collider, plane->mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
														for (unsigned int f = 0; f < codeComponents.size(); ++f)
														{
															if (collider->mIsTrigger)
																((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
															else
																((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
														}
													}
												}
												else if (result == 3)// intersecting plane
												{
													CalcFriction(*collider, plane->mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														if (collider->mIsTrigger)
															((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
														else
															((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
													}

													float bottom = collider->GetPos().y - ((SphereCollider*)collider)->mRadius;
													if (bottom < ((PlaneCollider*)otherCol)->mOffset)
													{
														vec3f pos = collider->GetPos();
														collider->SetPos(vec3f(pos.x, ((PlaneCollider*)otherCol)->mOffset + ((SphereCollider*)collider)->mRadius, pos.z));
													}
												}
											}
											if (collider->mIsTrigger && otherCol->mColliderType == Collider::eCOLLIDER_Controller)
											{
												for (unsigned int f = 0; f < codeComponents.size(); ++f)
												{
													((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
												}
											}
										}
									}
								}
							}
						}
						else if (collider->mColliderType == Collider::eCOLLIDER_Cube)//Check CubeCollider's collision with other objects
						{
							CubeCollider* aabb1 = (CubeCollider*)collider;
							for (int j = 0; j < objs; ++j)
							{
								if (mObjects[j] != mObjects[i])
								{
									otherColliders = mObjects[j]->mComponents[eCOMPONENT_COLLIDER];
									int othercols = (int)otherColliders.size();
									for (int k = 0; k < othercols; ++k)
									{
										otherCol = (Collider*)otherColliders[k];
										if (otherCol->mIsEnabled)
										{
											if (otherCol->mColliderType == Collider::eCOLLIDER_Cube)
											{
												CubeCollider* aabb2 = (CubeCollider*)otherCol;
												//SystemLogger::GetLog() << "Collision State: " << collider->mColliding << std::endl;
												if (AABBtoAABB(*aabb1, *aabb2))
												{
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														if (collider->mIsTrigger)
															((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
														else
															((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
													}
												}
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere)
											{
												SphereCollider* s1 = (SphereCollider*)otherCol;
												if (SphereToAABB(*s1, *aabb1))
												{
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														if (collider->mIsTrigger)
															((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
														else
															((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
													}
												}
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Plane)
											{
												PlaneCollider* plane = (PlaneCollider*)otherCol;
												int result = AabbToPlane(*plane, *aabb1);
												if (result == 2 && collider->GetPos().y > otherCol->GetPos().y)//behind plane TODO: Make this not jank
												{
													if (((CubeCollider*)collider)->mMin.y < otherCol->GetPos().y)
													{
														vec3f pos = collider->GetPos();
														collider->SetPos(vec3f(pos.x, ((PlaneCollider*)otherCol)->mOffset + ((CubeCollider*)collider)->mMinOffset.y, pos.z));

														CalcFriction(*collider, plane->mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
														for (unsigned int f = 0; f < codeComponents.size(); ++f)
														{
															if (collider->mIsTrigger)
																((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
															else
																((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
														}
													}
												}
												if (result == 3)// intersecting plane
												{
													CalcFriction(*collider, plane->mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														if (collider->mIsTrigger)
															((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
														else
															((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
													}

													if (((CubeCollider*)collider)->mMin.y < ((PlaneCollider*)otherCol)->mOffset)
													{
														vec3f pos = collider->GetPos();
														collider->SetPos(vec3f(pos.x, ((PlaneCollider*)otherCol)->mOffset + fabsf(((CubeCollider*)collider)->mMinOffset.y), pos.z));
													}
												}
											}
											if (collider->mIsTrigger && otherCol->mColliderType == Collider::eCOLLIDER_Controller)
											{
												CubeCollider* aabb2 = (CubeCollider*)otherCol;
												if (AABBtoAABB(*aabb1, *aabb2))
												{
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
													}
												}
											}
										}
									}
								}
							}
						}
						else if (collider->mColliderType == Collider::eCOLLIDER_OrientedCube)
						{
							for (int j = 0; j < objs; ++j)
							{
								if (mObjects[j] != mObjects[i])
								{
									otherColliders = mObjects[j]->mComponents[eCOMPONENT_COLLIDER];
									int othercols = (int)otherColliders.size();
									for (int k = 0; k < othercols; ++k)
									{
										otherCol = (Collider*)otherColliders[k];
										if (otherCol->mIsEnabled)
										{
											if (otherCol->mColliderType == Collider::eCOLLIDER_OrientedCube)
											{
												if (OBBtoOBB(*((OrientedCubeCollider*)collider), *((OrientedCubeCollider*)otherCol)))
												{
													collider->mShouldMove = false;
												}
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Plane)
											{
												if (OBBtoPlane(*((OrientedCubeCollider*)collider), *((PlaneCollider*)otherCol)))
												{
													collider->mShouldMove = false;
												}
											}
										}
									}
								}
							}
						}
						else if (collider->mColliderType == Collider::eCOLLIDER_Button)//Check ButtonCollider's collision with other objects/controller
						{
							CubeCollider* aabb1 = (CubeCollider*)collider;
							if (AabbToPlane(((ButtonCollider*)collider)->mUpperBound, *aabb1) != 2)
							{
								collider->mVelocity = { 0,0,0 };
								collider->mAcceleration = { 0,0,0 };
								collider->mTotalForce = { 0,0,0 };
							}

							for (int j = 0; j < objs; ++j)
							{
								if (mObjects[j] != mObjects[i])
								{
									otherColliders = mObjects[j]->mComponents[eCOMPONENT_COLLIDER];
									int othercols = (int)otherColliders.size();
									for (int k = 0; k < othercols; ++k)
									{
										otherCol = (Collider*)otherColliders[k];
										if (otherCol->mIsEnabled)
										{
											if (otherCol->mColliderType == Collider::eCOLLIDER_Cube || otherCol->mColliderType == Collider::eCOLLIDER_Controller &&
												(otherCol->GetPos() - collider->GetPos()) * ((ButtonCollider*)collider)->mPushNormal > 0)
											{
												CubeCollider* aabb2 = (CubeCollider*)otherCol;
												if ((AabbToPlane(((ButtonCollider*)collider)->mLowerBound, *aabb1) == 1) && AABBtoAABB(*aabb1, *aabb2))
												{
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
														((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
												}
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere && otherCol->mVelocity * ((ButtonCollider*)collider)->mPushNormal < 0)
											{
												SphereCollider* s1 = (SphereCollider*)otherCol;
												if ((AabbToPlane(((ButtonCollider*)collider)->mLowerBound, *aabb1) == 1) && SphereToAABB(*s1, *aabb1))
												{
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
														((CodeComponent*)codeComponents[f])->OnCollision(*collider, *otherCol, _time);
												}
											}
										}
									}
								}
							}

							if (AabbToPlane(((ButtonCollider*)collider)->mLowerBound, *aabb1) != 1)
							{
								if(((ButtonCollider*)collider)->mPress)
								{
									collider->mVelocity = { 0,0,0 };
									collider->mAcceleration = { 0,0,0 };
									collider->mTotalForce = { 0,0,0 };
								}
								else
								{
									collider->mTotalForce = collider->mForces + (collider->mGravity * collider->mMass);
									collider->mVelocity = collider->mTotalForce / collider->mMass;
									collider->mAcceleration = collider->mVelocity / _time;
								}
							}
						}
					}
					else if (collider->mColliderType == Collider::eCOLLIDER_Controller && VRInputManager::GetInstance().IsVREnabled())//Update ControllerCollider position, do not apply physics to player
					{
						CubeCollider* aabb1 = (CubeCollider*)collider;
						for (int j = 0; j < objs; ++j)
						{
							if (mObjects[j] != mObjects[i])
							{
								otherColliders = mObjects[j]->mComponents[eCOMPONENT_COLLIDER];
								int othercols = (int)otherColliders.size();
								for (int k = 0; k < othercols; ++k)
								{
									otherCol = (Collider*)otherColliders[k];
									if (otherCol->mIsEnabled)
									{
										//if physics is being applied to an object, or if object is pick upable we still want to add it to mHitting of the controller
										if (otherCol->mShouldMove || otherCol->mPickUpAble)
										{
											if (otherCol->mColliderType == Collider::eCOLLIDER_Cube)
											{
												CubeCollider* aabb2 = (CubeCollider*)otherCol;
												if (AABBtoAABB(*aabb1, *aabb2))
												{
													((ControllerCollider*)collider)->mHitting.insert(otherCol);
													for (unsigned int f = 0; f < codeComponents.size(); ++f)
													{
														((CodeComponent*)codeComponents[f])->OnTriggerEnter(*collider, *otherCol);
													}
												}
												else if (((ControllerCollider*)collider)->mHitting.find(otherCol) != ((ControllerCollider*)collider)->mHitting.end())
													((ControllerCollider*)collider)->mHitting.erase(otherCol);
											}
											else if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere)
											{
												SphereCollider* s1 = (SphereCollider*)otherCol;
												if (SphereToAABB(*s1, *aabb1))
													((ControllerCollider*)collider)->mHitting.insert(otherCol);
												else if (((ControllerCollider*)collider)->mHitting.find(otherCol) != ((ControllerCollider*)collider)->mHitting.end())
													((ControllerCollider*)collider)->mHitting.erase(otherCol);
											}
										}
									}
								}
							}
						}
						Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
						if (((ControllerCollider*)collider)->mLeft &&
							(collider->mObject->GetUniqueID() == cLevel->GetLeftController()->GetUniqueID() ||
							 collider->mObject->GetUniqueID() == cLevel->GetRightController()->GetUniqueID()))
						{
							collider->mVelocity = VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetVelocity();
							collider->mAcceleration = collider->mVelocity / TimeManager::Instance()->GetDeltaTime();
							collider->mForces = collider->mAcceleration * collider->mMass;
							collider->mTotalForce = collider->mForces + (collider->mGravity * collider->mMass);
							collider->SetPos(VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetPosition().Position);
						}
						else if ((!(((ControllerCollider*)collider)->mLeft) &&
							(collider->mObject->GetUniqueID() == cLevel->GetLeftController()->GetUniqueID() ||
							 collider->mObject->GetUniqueID() == cLevel->GetRightController()->GetUniqueID())))
						{

							collider->mVelocity = VRInputManager::GetInstance().GetController(eControllerType_Primary).GetVelocity();
							collider->mAcceleration = collider->mVelocity / TimeManager::Instance()->GetDeltaTime();
							collider->mForces = collider->mAcceleration * collider->mMass;
							collider->mTotalForce = collider->mForces + (collider->mGravity * collider->mMass);
							collider->SetPos(VRInputManager::GetInstance().GetController(eControllerType_Primary).GetPosition().Position);
						}
					}

					if (collider->IsEnabled() && collider->mShouldMove && collider->mColliderType != Collider::eCOLLIDER_Controller)
					{
						if((collider->mVelocity.Magnitude() > .01f || collider->mAcceleration.Magnitude() > .01f || collider->mDragForce.Magnitude() > .01f) || collider->mTotalForce.Magnitude() > .01f)
						{
							collider->mDragForce = collider->mVelocity * (-0.5f * collider->mRHO * collider->mVelocity.Magnitude() * collider->mDrag * collider->mArea);
							collider->mAcceleration = CalcAcceleration(collider->mTotalForce, collider->mMass);
							collider->mVelocity = CalcVelocity(collider->mVelocity, collider->mAcceleration, _time);

							if (fabs(collider->mForces.x) < 0.01f && fabsf(collider->mForces.y) < 0.01f && fabsf(collider->mForces.z) < 0.01f)
								collider->mForces = { 0,0,0 };
							else
								collider->mForces *= 0.99f;

							collider->SetPos(CalcPosition(collider->GetPos(), collider->mVelocity, _time));
						}
					}
				}//For all colliders of object end
			}//For all objects end
		}
	}//Physics loop end

} // Epoch Namespace