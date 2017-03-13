//#include "stdafx.h"
#include "Physics.h"
#include "..\Objects\BaseObject.h"
#include "..\Rendering\Mesh.h"
#include "..\Common\Logger.h"
#include "..\Objects\Component.h"
#include "..\Actions\CodeComponent.hpp"
#include "..\Input\VRInputManager.h"
#include "..\Core\LevelManager.h"

namespace Epoch {

	Physics* Physics::mInstance;

	Physics::Physics() {
		mRayCasting = false;
		mInstance = nullptr;
	}

	Physics* Physics::Instance() {
		if (!mInstance)
			mInstance = new Physics();
		return mInstance;
	}

	void Physics::Destroy() {
		if (mInstance) {
			delete mInstance;
			mInstance = nullptr;
		}
	}

#pragma region RAY_CASTING

	bool Physics::RayToTriangle(vec4f& _vert0, vec4f& _vert1, vec4f& _vert2, vec4f& _normal, vec4f& _start, vec4f& _dir, float& _time) {
		vec3f rayToCentroid = ((_vert0 + _vert1 + _vert2) / 3) - _start;
		if ((_normal * _dir) > 0.001f || (rayToCentroid * _normal) > 0) {
			return false;
		}

		vec4f sa = _vert0 - _start;
		vec4f sb = _vert1 - _start;
		vec4f sc = _vert2 - _start;
		vec4f N1, N2, N3;
		N1 = sc ^ sb;
		N2 = sa ^ sc;
		N3 = sb ^ sa;
		float dn1 = _dir * N1;
		float dn2 = _dir * N2;
		float dn3 = _dir * N3;

		if (dn1 == 0.0f && dn2 == 0.0f && dn3 == 0.0f) {
			_time = 0;
			return true;
		}

		if (SameSign(dn1, dn2) && SameSign(dn2, dn3)) {
			float offset = _normal * _vert0;
			_time = ((offset - (_start * _normal)) / (_normal * _dir));
			return true;
		}
		return false;
	}

	bool Physics::RayToSphere(vec4f & _pos, vec4f & _dir, vec4f & _center, float _radius, float & _time, vec4f & _out) {
		vec4f check = _pos - _center;

		vec4f m = _pos - _center;
		float b = m * _dir;
		float c = (m * m) - (_radius * _radius);
		if (c < 0.0f || b > 0.0f)
			return false;

		float dis = b*b - c;
		if (dis < 0.0f)
			return false;

		_time = -b - sqrtf(dis);
		if (_time < 0.0f) {
			_time = 0.0f;
			vec4f np = { _pos.x + _time, _pos.y + _time, _pos.z + _time, 1.0f };
			_out = np ^ _dir;
		}

		return true;
	}

	bool Physics::RayToCylinder(vec4f & _start, vec4f & _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time) {
		vec4f d = _point2 - _point1;
		vec4f od = _point1 - _point2;
		vec4f m = _start - _point1;

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

		vec4f col = _start + _normal * _time;

		if (od * (_point2 - col) > 0 || d * (_point1 - col) > 0)
			return false;

		return true;
	}

	bool Physics::RayToCapsule(vec4f & _start, vec4f & _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time) {
		float fTime = FLT_MAX;
		_time = FLT_MAX;
		bool bReturn = false;

		if (RayToCylinder(_start, _normal, _point1, _point2, _radius, _time)) {
			fTime = PMIN(_time, fTime);
			_time = fTime;
			bReturn = true;
		}

		vec4f pcol, qcol;
		if (RayToSphere(_start, _normal, _point1, _radius, _time, pcol)) {
			fTime = PMIN(_time, fTime);
			_time = fTime;
			bReturn = true;
		}

		if (RayToSphere(_start, _normal, _point2, _radius, _time, qcol)) {
			fTime = PMIN(_time, fTime);
			_time = fTime;
			bReturn = true;
		}
		return bReturn;
	}

#pragma endregion


#pragma region MOVING_SPHERE

	bool Physics::MovingSphereToTriangle(vec4f & _vert0, vec4f & _vert1, vec4f & _vert2, vec4f & _normal, vec4f & _start, vec4f & _dir, float _radius, float & _time, vec4f & _outNormal) {
		bool bReturn = false;
		float fTime = FLT_MAX;
		_time = FLT_MAX;

		vec4f verts[3] = { _vert0, _vert1, _vert2 };
		vec4f offset0 = _vert0 + (_normal * _radius);
		vec4f offset1 = _vert1 + (_normal * _radius);
		vec4f offset2 = _vert2 + (_normal * _radius);

		if (RayToTriangle(offset0, offset1, offset2, _normal, _start, _dir, _time)) {
			fTime = PMIN(_time, fTime);
			_time = fTime;
			_outNormal = _normal;
			bReturn = true;
		} else {
			if (RayToCapsule(_start, _dir, _vert0, _vert1, _radius, _time)) {
				fTime = PMIN(_time, fTime);
				_time = fTime;
				vec4f temp1 = _dir * _time + _start;
				vec4f temp2 = (_vert1 - _vert0) * _time + _vert0;
				_outNormal = (temp1 - temp2).Normalize();
				bReturn = true;
			}

			if (RayToCapsule(_start, _dir, _vert1, _vert2, _radius, _time)) {
				fTime = PMIN(_time, fTime);
				_time = fTime;
				vec4f temp1 = _dir * _time + _start;
				vec4f temp2 = (_vert1 - _vert0) * _time + _vert0;
				_outNormal = (temp1 - temp2).Normalize();
				bReturn = true;
			}

			if (RayToCapsule(_start, _dir, _vert2, _vert0, _radius, _time)) {
				fTime = PMIN(_time, fTime);
				_time = fTime;
				vec4f temp1 = _dir * _time + _start;
				vec4f temp2 = (_vert1 - _vert0) * _time + _vert0;
				_outNormal = (temp1 - temp2).Normalize();
				bReturn = true;
			}
		}

		return bReturn;
	}

	bool Physics::MovingSphereToMesh(vec4f & _start, vec4f & _dir, float _radius, Mesh* _mesh, float & _time, vec4f & _outNormal) {
		bool bCollision = false;
		_time = FLT_MAX;
		float fTime = FLT_MAX;

		for (unsigned int i = 0; i < _mesh->GetNumTriangles(); i++) {
			Triangle currTri = _mesh->GetTriangles()[i];
			vec4f currNorm = _mesh->GetTriangles()[i].Normal;

			if (MovingSphereToTriangle(
				vec4f(currTri.Vertex[0]),
				vec4f(currTri.Vertex[1]),
				vec4f(currTri.Vertex[2]),
				currNorm, _start, _dir, _radius, fTime, _outNormal)) {
				_time = fminf(_time, fTime);
				_outNormal = currNorm;
				bCollision = true;
			}
		}

		return bCollision;
	}

#pragma endregion


#pragma region PLANE_COLISION

	void Physics::BuildPlane(Plane& _plane, vec4f& _pointA, vec4f& _pointB, vec4f& _pointC) {
		_plane.mNormal = (_pointA - _pointC) ^ (_pointB - _pointA);
		_plane.mNormal.Normalize();
		_plane.mOffset = _pointA * _plane.mNormal;
	}

	//Returns 1 if on or in front of plane
	//Returns 2 if behind plane
	int Physics::PointToPlane(Plane& _plane, vec4f& _point) {
		float pOffset = _point * _plane.mNormal;
		if (pOffset >= _plane.mOffset)
			return 1;
		return 2;
	}

	//Returns 1 if in front of plane
	//Returns 2 if behind plane
	//Returns 3 if intersecting plane
	int Physics::SphereToPlane(Plane& _plane, Sphere& _sphere) {
		float pOffset = _sphere.mPosition * _plane.mNormal;
		if (pOffset - _plane.mOffset > _sphere.mRadius)
			return 1;
		else if (pOffset - _plane.mOffset < -_sphere.mRadius)
			return 2;
		return 3;
	}

	//Returns 1 if in front of plane.
	//Returns 2 if behind plane.
	//Returns 3 if intersecting plane.
	int Physics::AabbToPlane(Plane& _plane, AABB& _aabb) {
		vec4f center = (_aabb.mMax + _aabb.mMin) * 0.5f;
		float pOffset = center * _plane.mNormal - _plane.mOffset;
		vec4f E = _aabb.mMax - center;
		float r = E * _plane.mNormal;
		Sphere s(center, r);
		return SphereToPlane(_plane, s);
	}

#pragma endregion


#pragma region FRUSTUM_COLLISION
	/*
	void Physics::BuildFrustum(Frustum& _frustum, float _fov, float _nearDist, float _farDist, float _ratio, matrix4& _camXform)
	{
	vec4f fc = _camXform.axis_pos - _camXform.axis_z * _farDist;
	vec4f nc = _camXform.axis_pos - _camXform.axis_z * _nearDist;
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


#pragma region MISC_COLLISION

	bool Physics::AABBtoAABB(AABB& _aabb1, AABB& _aabb2) {
		if (_aabb1.mMax.x < _aabb2.mMin.x || _aabb1.mMin.x > _aabb2.mMax.x)
			return false;

		if (_aabb1.mMax.y < _aabb2.mMin.y || _aabb1.mMin.y > _aabb2.mMax.y)
			return false;

		if (_aabb1.mMax.z < _aabb2.mMin.z || _aabb1.mMin.z >  _aabb2.mMax.z)
			return false;

		return true;
	}

	bool Physics::SphereToSphere(Sphere& _sphere1, Sphere& _sphere2) {
		vec4f pos = _sphere1.mPosition - _sphere2.mPosition;
		float distance = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
		float minDist = _sphere1.mRadius + _sphere2.mRadius;

		return distance <= (minDist * minDist);
	}

	bool Physics::SphereToAABB(Sphere& _sphere, AABB& _aabb) {
		float X, Y, Z;

		if (_sphere.mPosition.x < _aabb.mMin.x)
			X = _aabb.mMin.x;
		else if (_sphere.mPosition.x > _aabb.mMax.x)
			X = _aabb.mMax.x;
		else
			X = _sphere.mPosition.x;

		if (_sphere.mPosition.y < _aabb.mMin.y)
			Y = _aabb.mMin.y;
		else if (_sphere.mPosition.y > _aabb.mMax.y)
			Y = _aabb.mMax.y;
		else
			Y = _sphere.mPosition.y;

		if (_sphere.mPosition.z < _aabb.mMin.z)
			Z = _aabb.mMin.z;
		else if (_sphere.mPosition.z > _aabb.mMax.z)
			Z = _aabb.mMax.z;
		else
			Z = _sphere.mPosition.z;

		vec4f point = { X, Y, Z, 1.0f };

		return (fabsf((point.x - _sphere.mPosition.x)) < _sphere.mRadius &&
			fabsf((point.y - _sphere.mPosition.y)) < _sphere.mRadius &&
			fabsf((point.z - _sphere.mPosition.z)) < _sphere.mRadius);
	}

	bool Physics::SphereToTriangle(Sphere& _sphere, Triangle& _tri, vec4f& _displacement) {
		float offset = (_sphere.mPosition - _tri.Vertex[0]) * _tri.Normal;
		vec4f scaled = _tri.Normal * offset;
		vec4f projected = _sphere.mPosition - scaled;
		vec4f edge0 = _tri.Vertex[1] - _tri.Vertex[0];
		vec4f edge1 = _tri.Vertex[2] - _tri.Vertex[1];
		vec4f edge2 = _tri.Vertex[0] - _tri.Vertex[2];
		vec4f norm0, norm1, norm2, Cpt = _sphere.mPosition;
		norm0 = edge0 ^ _tri.Normal;
		norm1 = edge1 ^ _tri.Normal;
		norm2 = edge2 ^ _tri.Normal;
		Plane plane0(norm0, _tri.Vertex[0] * norm0);
		Plane plane1(norm1, _tri.Vertex[1] * norm1);
		Plane plane2(norm2, _tri.Vertex[2] * norm2);

		if (PointToPlane(plane0, projected) == 1 &&
			PointToPlane(plane1, projected) == 1 &&
			PointToPlane(plane2, projected) == 1) {
			Cpt = projected;
		} else {
			vec4f s0 = projected - _tri.Vertex[0];
			vec4f s1 = projected - _tri.Vertex[1];
			vec4f s2 = projected - _tri.Vertex[2];
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

			vec4f c0 = _tri.Vertex[0] + edge0 * p0;
			vec4f c1 = _tri.Vertex[1] + edge1 * p1;
			vec4f c2 = _tri.Vertex[2] + edge2 * p2;
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

		if ((Cpt - _sphere.mPosition) * (Cpt - _sphere.mPosition) < (_sphere.mRadius * _sphere.mRadius)) {
			vec4f v = _sphere.mPosition - Cpt;
			float dist = v.Magnitude3();
			vec4f n = v.Normalize();
			_displacement = n * (_sphere.mRadius - dist);
			return true;
		}

		return false;
	}

#pragma endregion

#pragma region SIMULATION

	vec4f Physics::CalcAcceleration(vec4f& _force, float _mass) {
		return _force / _mass;
	}

	vec4f Physics::CalcVelocity(vec4f& _vel, vec4f& _accel, float _time) {
		return _vel + _accel * _time;
	}

	vec4f Physics::CalcPosition(vec4f& _pos, vec4f& _vel, float _time) {
		return _pos + _vel * _time;
	}

	void Physics::CalcReaction(Collider& _col1, Collider& _col2, float _time) {
		//float avgElasticity = (_col1.mElasticity + _col2.mElasticity) / 2;
		//vec4f collisionNormal = _col1.GetPos() - _col2.GetPos();
		//collisionNormal.Normalize();
		//vec4f relativeVelocity = _col1.mVelocity - _col2.mVelocity;
		//
		//float impulseMagnitude3 = -(1 + avgElasticity) * _col1.mMass * _col2.mMass * (relativeVelocity * collisionNormal) / (_col1.mMass + _col2.mMass);
		//vec4f impulse = collisionNormal * impulseMagnitude3;
		//_col1.mImpulsiveForce = impulse / _time;
		//_col2.mImpulsiveForce = -impulse / _time;
		//_col1.mVelocity = _col1.mVelocity + impulse / _col1.mMass;
		//_col2.mVelocity = _col2.mVelocity - impulse / _col2.mMass;
		//_col1.mTotalForce = _col1.mImpulsiveForce;
		//_col2.mTotalForce = _col2.mImpulsiveForce;
	}

	void Physics::PlaneColReaction(Collider& _col, Collider& _plane) {
		vec4f normalVel = (((PlaneCollider*)&_plane)->mNormal * (_col.mVelocity * ((PlaneCollider*)&_plane)->mNormal));
		_col.mVelocity = (normalVel * -_col.mElasticity) + (_col.mVelocity - normalVel);
	}

	void Physics::CalcFriction(Collider& _col, vec4f& _norm, float _static, float _kinetic) {
		vec4f tangentForce = _col.mForces - (_norm * (_col.mForces * _norm));
		float staticFriction = 0;
		float avgStatic = _col.mStaticFriction < _static ? _static : _col.mStaticFriction;
		float avgKinetic = _col.mKineticFriction < _kinetic ? _kinetic : _col.mKineticFriction;

		if (!_col.mRewind) {
			if (fabs(_col.mVelocity.x) < 0.1f)
				_col.mVelocity.x = 0;
			if (fabsf(_col.mVelocity.y) < 0.1f)
				_col.mVelocity.y = 0;
			if (fabsf(_col.mVelocity.z) < 0.1f)
				_col.mVelocity.z = 0;
		}

		if (fabsf(_col.mVelocity.Magnitude3()) < 0.001f) {
			staticFriction = avgStatic * (-_col.mWeight).Magnitude3();
			_col.mTotalForce = { 0,0,0,0 };
		} else if (_col.mVelocity.Magnitude3() > 0) {
			//vec4f normDir(fabsf(_col.mVelocity.Normalize().x), fabsf(_col.mVelocity.Normalize().y), fabs(_col.mVelocity.Normalize().z), 1);
			vec4f totalFriction = _col.mVelocity.Normalize() * (-avgKinetic * (-_col.mWeight).Magnitude3());
			_col.mTotalForce = _col.mForces + totalFriction + _col.mDragForce;
		}

		if (fabsf(_col.mVelocity.Magnitude3()) < 0.001f && tangentForce.Magnitude3() >= staticFriction) {
			float sliding = tangentForce.Magnitude3() / staticFriction;
			_col.mVelocity = tangentForce.Normalize() * sliding;
			vec4f kineticFriction = _col.mVelocity.Normalize() * (-avgKinetic * (-_col.mWeight).Magnitude3());
			_col.mTotalForce = _col.mForces + kineticFriction + _col.mDragForce;
		}
	}

#pragma endregion

	void Physics::Update(float _time) {
		//SystemLogger::GetLog() << _time << std::endl;
		Collider* collider;
		Collider* otherCol;
		vec4f norm;

		int objs = (int)mObjects.size();
		for (int i = 0; i < objs; ++i) {
			int cols = (int)mObjects[i]->mComponents[eCOMPONENT_COLLIDER].size();
			for (int x = 0; x < cols; ++x) {
				collider = (Collider*)mObjects[i]->mComponents[eCOMPONENT_COLLIDER][x];
				collider->mTotalForce = collider->mForces + collider->mWeight + collider->mDragForce;

				if (collider->mShouldMove && collider->mColliderType == Collider::eCOLLIDER_Sphere)//Check SphereCollider's collision with other objects
				{
					Sphere s1(((SphereCollider*)collider)->GetPos(), ((SphereCollider*)collider)->mRadius);
					for (int j = 0; j < objs; ++j) {
						if (mObjects[j] != mObjects[i]) {
							int othercols = (int)mObjects[j]->mComponents[eCOMPONENT_COLLIDER].size();
							for (int k = 0; k < othercols; ++k) {
								otherCol = (Collider*)mObjects[j]->mComponents[eCOMPONENT_COLLIDER][k];
								//if (otherCol->mColliderType == Collider::eCOLLIDER_Mesh)
								//{
								//	//Not sure what outnorm is used for at the moment might just stick with basic cube/sphere collisions
								//	if (MovingSphereToMesh(collider->GetPos(), collider->mVelocity, ((SphereCollider*)collider)->mRadius, ((MeshCollider*)otherCol)->mMesh, _time, norm))
								//	{
								//		
								//	}
								//}
								if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere) {
									Sphere s2(otherCol->GetPos(), ((SphereCollider*)otherCol)->mRadius);
									if (SphereToSphere(s1, s2)) {
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}
									}
								} else if (otherCol->mColliderType == Collider::eCOLLIDER_Cube) {
									AABB aabb(((CubeCollider*)otherCol)->mMin, ((CubeCollider*)otherCol)->mMax);
									if (SphereToAABB(s1, aabb)) {
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}
									}
								} else if (otherCol->mColliderType == Collider::eCOLLIDER_Plane) {
									Plane plane(((PlaneCollider*)otherCol)->mNormal, ((PlaneCollider*)otherCol)->mOffset);
									int result = SphereToPlane(plane, s1);
									if (result == 2)//behind plane
									{
										float bottom = ((SphereCollider*)collider)->mCenter.y - ((SphereCollider*)collider)->mRadius;
										if (bottom < otherCol->GetPos().y) {
											vec4f pos = collider->GetPos();
											collider->SetPos(vec4f(pos.x, otherCol->GetPos().y + ((CubeCollider*)collider)->mMinOffset.y, pos.z, 1));

											CalcFriction(*collider, plane.mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
											for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
												((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
												((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
											}
										}
									} else if (result == 3)// intersecting plane
									{
										CalcFriction(*collider, plane.mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}

										float bottom = ((SphereCollider*)collider)->mCenter.y - ((SphereCollider*)collider)->mRadius;
										if (bottom < otherCol->GetPos().y) {
											vec4f pos = collider->GetPos();
											collider->SetPos(vec4f(pos.x, otherCol->GetPos().y + fabsf(((SphereCollider*)collider)->mRadius), pos.z, 1));
										}
									}
								}
							}
						}
					}
				} else if (collider->mShouldMove && collider->mColliderType == Collider::eCOLLIDER_Cube)//Check CubeCollider's collision with other objects
				{
					AABB aabb1(((CubeCollider*)collider)->mMin, ((CubeCollider*)collider)->mMax);
					for (int j = 0; j < objs; ++j) {
						if (mObjects[j] != mObjects[i]) {
							int othercols = (int)mObjects[j]->mComponents[eCOMPONENT_COLLIDER].size();
							for (int k = 0; k < othercols; ++k) {
								otherCol = (Collider*)mObjects[j]->mComponents[eCOMPONENT_COLLIDER][k];
								if (otherCol->mColliderType == Collider::eCOLLIDER_Cube) {
									AABB aabb2(((CubeCollider*)otherCol)->mMin, ((CubeCollider*)otherCol)->mMax);
									//SystemLogger::GetLog() << "Collision State: " << collider->mColliding << std::endl;
									if (collider->mShouldMove && AABBtoAABB(aabb1, aabb2)) {
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}
									}
								} else if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere) {
									Sphere s1(otherCol->GetPos(), ((SphereCollider*)otherCol)->mRadius);
									if (SphereToAABB(s1, aabb1)) {
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}
									}
								} else if (otherCol->mColliderType == Collider::eCOLLIDER_Plane) {
									if (collider->mShouldMove) {
										Plane plane(((PlaneCollider*)otherCol)->mNormal, ((PlaneCollider*)otherCol)->mOffset);
										int result = AabbToPlane(plane, aabb1);
										if (result == 2)//behind plane
										{
											if (((CubeCollider*)collider)->mMin.y < otherCol->GetPos().y) {
												vec4f pos = collider->GetPos();
												collider->SetPos(vec4f(pos.x, otherCol->GetPos().y + ((CubeCollider*)collider)->mMinOffset.y, pos.z, 1));

												CalcFriction(*collider, plane.mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
												for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
													((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
													((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
												}
											}
										}
										if (result == 3)// intersecting plane
										{
											CalcFriction(*collider, plane.mNormal, otherCol->mStaticFriction, otherCol->mKineticFriction);
											for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
												((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
												((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
											}

											if (((CubeCollider*)collider)->mMin.y < otherCol->GetPos().y) {
												vec4f pos = collider->GetPos();
												collider->SetPos(vec4f(pos.x, otherCol->GetPos().y + fabsf(((CubeCollider*)collider)->mMinOffset.y), pos.z, 1));
											}
										}
									}
								}
							}
						}
					}
				} else if (collider->mColliderType == Collider::eCOLLIDER_Button)//Check ButtonCollider's collision with other objects/controller
				{
					AABB aabb1(((ButtonCollider*)collider)->mMin, ((ButtonCollider*)collider)->mMax);

					if (AabbToPlane(((ButtonCollider*)collider)->mUpperBound, aabb1) != 2) {
						collider->mVelocity = { 0,0,0,0 };
						collider->mAcceleration = { 0,0,0,0 };
						collider->mTotalForce = { 0,0,0,0 };
					}

					for (int j = 0; j < objs; ++j) {
						if (mObjects[j] != mObjects[i]) {
							int othercols = (int)mObjects[j]->mComponents[eCOMPONENT_COLLIDER].size();
							for (int k = 0; k < othercols; ++k) {
								otherCol = (Collider*)mObjects[j]->mComponents[eCOMPONENT_COLLIDER][k];
								if (otherCol->mColliderType == Collider::eCOLLIDER_Cube || otherCol->mColliderType == Collider::eCOLLIDER_Controller) {
									AABB aabb2(((CubeCollider*)otherCol)->mMin, ((CubeCollider*)otherCol)->mMax);
									if (collider->mShouldMove && (AabbToPlane(((ButtonCollider*)collider)->mLowerBound, aabb1) == 1) && AABBtoAABB(aabb1, aabb2)) {
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}
									}
								} else if (otherCol->mColliderType == Collider::eCOLLIDER_Sphere) {
									Sphere s1(otherCol->GetPos(), ((SphereCollider*)otherCol)->mRadius);
									if (collider->mShouldMove && (AabbToPlane(((ButtonCollider*)collider)->mLowerBound, aabb1) == 1) && SphereToAABB(s1, aabb1)) {
										for (unsigned int f = 0; f < collider->mObject->GetComponentCount(eCOMPONENT_CODE); ++f) {
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnCollision(*collider, *otherCol, _time);
											((CodeComponent*)(collider->mObject->GetComponents(eCOMPONENT_CODE)[f]))->OnTriggerEnter(*collider, *otherCol);
										}
									}
								}
							}
						}
					}

					if (AabbToPlane(((ButtonCollider*)collider)->mLowerBound, aabb1) != 1) {
						collider->mVelocity = -collider->mVelocity;
						collider->mAcceleration = -collider->mAcceleration;
						collider->mTotalForce = collider->mForces + (collider->mGravity * collider->mMass);
					}
				} else if (collider->mColliderType == Collider::eCOLLIDER_Controller)//Update ControllerCollider position, do not apply physics to player
				{
					AABB aabb1(((CubeCollider*)collider)->mMin, ((CubeCollider*)collider)->mMax);
					for (int j = 0; j < objs; ++j) {
						if (mObjects[j] != mObjects[i]) {
							int othercols = (int)mObjects[j]->mComponents[eCOMPONENT_COLLIDER].size();
							for (int k = 0; k < othercols; ++k) {
								otherCol = (Collider*)mObjects[j]->mComponents[eCOMPONENT_COLLIDER][k];
								if (otherCol->mShouldMove && otherCol->mColliderType == Collider::eCOLLIDER_Cube) {
									AABB aabb2(((CubeCollider*)otherCol)->mMin, ((CubeCollider*)otherCol)->mMax);
									if (AABBtoAABB(aabb1, aabb2))
										((ControllerCollider*)collider)->mHitting.insert(otherCol);
									else if (((ControllerCollider*)collider)->mHitting.find(otherCol) != ((ControllerCollider*)collider)->mHitting.end())
										((ControllerCollider*)collider)->mHitting.erase(otherCol);
								} else if (otherCol->mShouldMove && otherCol->mColliderType == Collider::eCOLLIDER_Sphere) {
									Sphere s1(otherCol->GetPos(), ((SphereCollider*)otherCol)->mRadius);
									if (SphereToAABB(s1, aabb1))
										((ControllerCollider*)collider)->mHitting.insert(otherCol);
									else if (((ControllerCollider*)collider)->mHitting.find(otherCol) != ((ControllerCollider*)collider)->mHitting.end())
										((ControllerCollider*)collider)->mHitting.erase(otherCol);
								}
							}
						}
					}

					Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
					if (((ControllerCollider*)collider)->mLeft &&
						(collider->mObject->GetUniqueID() == cLevel->GetLeftController()->GetUniqueID() ||
							collider->mObject->GetUniqueID() == cLevel->GetRightController()->GetUniqueID())) {
						collider->mTotalForce = collider->mForces + (collider->mGravity * collider->mMass);
						collider->mAcceleration = CalcAcceleration(collider->mTotalForce, collider->mMass);
						collider->mVelocity = VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetVelocity();
						collider->SetPos(VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetPosition().Position);
					} else if ((!(((ControllerCollider*)collider)->mLeft) &&
						(collider->mObject->GetUniqueID() == cLevel->GetLeftController()->GetUniqueID() ||
							collider->mObject->GetUniqueID() == cLevel->GetRightController()->GetUniqueID()))) {
						collider->mTotalForce = collider->mForces + (collider->mGravity * collider->mMass);
						collider->mAcceleration = CalcAcceleration(collider->mTotalForce, collider->mMass);
						collider->mVelocity = VRInputManager::GetInstance().GetController(eControllerType_Primary).GetVelocity();
						collider->SetPos(VRInputManager::GetInstance().GetController(eControllerType_Primary).GetPosition().Position);
					}
				}

				if (collider->mShouldMove && collider->mColliderType != Collider::eCOLLIDER_Controller) {
					if (collider->mShouldMove || !collider->mRewind) {
						collider->mDragForce = collider->mVelocity * (-0.5f * collider->mRHO * collider->mVelocity.Magnitude3() * collider->mDrag * collider->mArea);
						collider->mAcceleration = CalcAcceleration(collider->mTotalForce, collider->mMass);
						collider->mVelocity = CalcVelocity(collider->mVelocity, collider->mAcceleration, _time);
						collider->mRewind = false;

						if (!collider->mRewind && fabs(collider->mForces.x) < 0.01f && fabsf(collider->mForces.y) < 0.01f && fabsf(collider->mForces.z) < 0.01f)
							collider->mForces = { 0,0,0,0 };
						else
							collider->mForces *= 0.99f;
					}

					collider->SetPos(CalcPosition(collider->GetPos(), collider->mVelocity, _time));
				}
			}//For all colliders of object end
		}//For all objects end
	}//Physics loop end

} // Epoch Namespace