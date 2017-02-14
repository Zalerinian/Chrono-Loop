//#include "stdafx.h"
#include "Physics.h"
#include "..\Objects\BaseObject.h"
#include "..\Rendering\Mesh.h"
#include "..\Common\Logger.h"
#include "..\Objects\Component.h"

Physics* Physics::mInstance;

Physics::Physics()
{
	mRayCasting = false;
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

bool Physics::RayToTriangle(vec4f& _vert0, vec4f& _vert1, vec4f& _vert2, vec4f& _normal, vec4f& _start, vec4f& _dir, float& _time)
{
	if ((_normal * _dir) > 0 || ((_vert0 - _start) * _normal) > 0)
		return false;

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

	if (dn1 == 0.0f && dn2 == 0.0f && dn3 == 0.0f)
	{
		_time = 0;
		return false;
	}

	if (SameSign(dn1, dn2) && SameSign(dn2, dn3))
	{
		float offset = _normal * _vert0;
		_time = ((offset - (_start * _normal)) / (_normal * _dir));
		return true;
	}
	return false;
}

bool Physics::RayToSphere(vec4f & _pos, vec4f & _dir, vec4f & _center, float _radius, float & _time, vec4f & _out)
{
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
	if (_time < 0.0f)
	{
		_time = 0.0f;
		vec4f np = { _pos.x + _time, _pos.y + _time, _pos.z + _time, 1.0f };
		_out = np ^ _dir;
	}

	return true;
}

bool Physics::RayToCylinder(vec4f & _start, vec4f & _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time)
{
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

bool Physics::RayToCapsule(vec4f & _start, vec4f & _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time)
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

	vec4f pcol, qcol;
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

#pragma endregion


#pragma region MOVING_SPHERE

bool Physics::MovingSphereToTriangle(vec4f & _vert0, vec4f & _vert1, vec4f & _vert2, vec4f & _normal, vec4f & _start, vec4f & _dir, float _radius, float & _time, vec4f & _outNormal)
{
	bool bReturn = false;
	float fTime = FLT_MAX;
	_time = FLT_MAX;

	vec4f verts[3] = { _vert0, _vert1, _vert2 };
	vec4f offset0 = _vert0 + (_normal * _radius);
	vec4f offset1 = _vert1 + (_normal * _radius);
	vec4f offset2 = _vert2 + (_normal * _radius);

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
			vec4f temp1 = _dir * _time + _start;
			vec4f temp2 = (_vert1 - _vert0) * _time + _vert0;
			_outNormal = (temp1 - temp2).Normalize();
			bReturn = true;
		}

		if (RayToCapsule(_start, _dir, _vert1, _vert2, _radius, _time))
		{
			fTime = PMIN(_time, fTime);
			_time = fTime;
			vec4f temp1 = _dir * _time + _start;
			vec4f temp2 = (_vert1 - _vert0) * _time + _vert0;
			_outNormal = (temp1 - temp2).Normalize();
			bReturn = true;
		}

		if (RayToCapsule(_start, _dir, _vert2, _vert0, _radius, _time))
		{
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

bool Physics::MovingSphereToMesh(vec4f & _start, vec4f & _dir, float _radius, Mesh* _mesh, float & _time, vec4f & _outNormal)
{
	bool bCollision = false;
	_time = FLT_MAX;
	float fTime = FLT_MAX;
	
	for (unsigned int i = 0; i < _mesh->GetNumTriangles(); i++) 
	{
		Triangle currTri = _mesh->GetTriangles()[i];
		vec4f currNorm = _mesh->GetTriangles()[i].Normal;

		if (MovingSphereToTriangle(
			vec4f(currTri.Vertex[0]),
			vec4f(currTri.Vertex[1]),
			vec4f(currTri.Vertex[2]),
			currNorm, _start, _dir, _radius, fTime, _outNormal))
		{
			_time = fminf(_time, fTime);
			_outNormal = currNorm;
			bCollision = true;
		}
	}

	return bCollision;
}

#pragma endregion


#pragma region PLANE_COLISION

void Physics::BuildPlane(Plane& _plane, vec4f& _pointA, vec4f& _pointB, vec4f& _pointC)
{
	_plane.mNormal = (_pointA - _pointC) ^ (_pointB - _pointA);
	_plane.mNormal.Normalize();
	_plane.mOffset = _pointA * _plane.mNormal;
}

//Returns 1 if on or in front of plane
//Returns 2 if behind plane
int Physics::PointToPlane(Plane& _plane, vec4f& _point)
{
	float pOffset = _point * _plane.mNormal;
	if (pOffset >= _plane.mOffset)
		return 1;
	return 2;
}

//Returns 1 if in front of plane
//Returns 2 if behind plane
//Returns 3 if intersecting plane
int Physics::SphereToPlane(Plane& _plane, Sphere& _sphere)
{
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
int Physics::AabbToPlane(Plane& _plane, AABB& _aabb)
{
	vec4f center = (_aabb.mMax + _aabb.mMin) * 0.5f;
	float pOffset = center * _plane.mNormal - _plane.mOffset;
	vec4f E = _aabb.mMax - center;
	float r = E.x * fabsf(_plane.mNormal.x) + E.y * fabsf(_plane.mNormal.y) + E.z * fabsf(_plane.mNormal.z);

	if (pOffset > r)
		return 1;
	else if (pOffset < -r)
		return 2;
	return 3;
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

bool Physics::AABBtoAABB(AABB& _aabb1, AABB& _aabb2)
{
	if (_aabb1.mMax.x < _aabb2.mMin.x || _aabb1.mMin.x > _aabb2.mMax.x)
		return false;

	if (_aabb1.mMax.y < _aabb2.mMin.y || _aabb1.mMin.y > _aabb2.mMax.y)
		return false;

	if (_aabb1.mMax.z < _aabb2.mMin.z || _aabb1.mMin.z >  _aabb2.mMax.z)
		return false;

	return true;
}

bool Physics::SphereToSphere(Sphere& _sphere1, Sphere& _sphere2)
{
	vec4f pos = _sphere1.mPosition - _sphere2.mPosition;
	float distance = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
	float minDist = _sphere1.mRadius + _sphere2.mRadius;

	return distance <= (minDist * minDist);
}

bool Physics::SphereToAABB(Sphere& _sphere, AABB& _aabb)
{
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

bool Physics::SphereToTriangle(Sphere& _sphere, Triangle& _tri, vec4f& _displacement)
{
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
		PointToPlane(plane2, projected) == 1)
	{
		Cpt = projected;
	}
	else
	{
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

	if ((Cpt - _sphere.mPosition) * (Cpt - _sphere.mPosition) < (_sphere.mRadius * _sphere.mRadius))
	{
		vec4f v = _sphere.mPosition - Cpt;
		float dist = v.Magnitude();
		vec4f n = v.Normalize();
		_displacement = n * (_sphere.mRadius - dist);
		return true;
	}

	return false;
}

#pragma endregion

#pragma region SIMULATION

vec4f Physics::CalcAcceleration(vec4f& _force, float _mass)
{
	return _force / _mass;
}

vec4f Physics::CalcVelocity(vec4f& _vel, vec4f& _accel, float _time)
{
	return _vel + _accel * _time;
}

vec4f Physics::CalcPosition(vec4f& _pos, vec4f& _vel, float _time)
{
	return _pos + _vel * _time;
}

void Physics::CalcReaction(Collider& _col1, Collider& _col2, float _time)
{
	float avgElasticity;     // average coefficient of restitution
	vec4f impulse; //  impulse vector 
	vec4f collisionNormal; // the normalized vector of the collision normal vector
	vec4f relativeVelocity; // relative velocity of the ball 
				 // compute the average coefficient of restitution (e) of the 2 colliding bodies
	avgElasticity = (_col1.mElasticity + _col2.mElasticity) / 2;
	//compute collision Normal n= C1-C2   C1=ball1 center position ,C2=ball2 center position
	collisionNormal = _col1.GetPos() - _col2.GetPos();
	// normalize n 
	collisionNormal.Normalize();
	// set the Collision normal(theCollision.norma) to n
	//theCollision.normal = collisionNormal;
	// compute the relative velocity of the two balls  Vr=v1-v2  where v1=body1 velocity and v2=body2 velocity
	relativeVelocity = _col1.mVelocity - _col2.mVelocity;
	// set theCollision relative velocity to Vr 
	//theCollision.relativeVelocity = relativeVelocity;
	//compute the magnitutde of impulse  impulse= -(1+e)*m1*m2*(v1-v2)*n/(m1+m2) , v1-v2=relative velocity=Vr
	float impulseMagnitude;
	impulseMagnitude = -(1 + avgElasticity) * _col1.mMass * _col2.mMass * (relativeVelocity * collisionNormal) / (_col1.mMass + _col2.mMass);
	// compute now the impulse vector impulse in the direction of the collision normal vector n, impulse= impulse*n
	impulse = collisionNormal * impulseMagnitude;
	// compute the impulsive force (impulsiveForce) of ball1 .remember F=impulse/deltaT
	_col1.mImpulsiveForce = impulse / _time;
	// compute the impulsive force (impulsiveForce) of ball2 .remember F=-impulse/deltaT
	_col2.mImpulsiveForce = -impulse / _time;
	// compute the velocities of body1 after collision:v1=v1+ impulse/m1
	_col1.mVelocity = _col1.mVelocity + impulse / _col1.mMass;
	// compute the velocities of body2 after collision:v2=v2-impulse/m2
	_col2.mVelocity = _col2.mVelocity - impulse / _col2.mMass;
	// compute the total force on body1 during collision : body1 totalForce= body1 impulsiveForce 
	_col1.mTotalForce = _col1.mImpulsiveForce + _col1.mGravity;
	// compute the total force on body2 during collision : body2 totalForce= body2 impulsiveForce 
	_col2.mTotalForce = _col2.mImpulsiveForce + _col2.mGravity;
	// calculate the collision contact point C=C1 + ball1Radius*n  from ball1 .
	//theCollision.contactPoint = body1.position + body1.radius * collisionNormal;
}

#pragma endregion

void Physics::Update(float _time)
{
	Collider* collider;
	Collider* otherCol;
	vec4f norm;
	
	int objs = mColliders.size();
	for (int i = 0; i < objs; ++i)
	{
		collider = mColliders[i];
		if (collider->mShouldMove)
		{
			collider->mAcceleration = CalcAcceleration(collider->mTotalForce, collider->mMass);
			collider->mVelocity = CalcVelocity(collider->mVelocity, collider->mAcceleration, _time);
			collider->SetPos(CalcPosition(collider->GetPos(), collider->mVelocity, _time));
		}

		if (collider->mIsSphere)
		{
			Sphere s1(collider->GetPos(), collider->mRadius);
			for (int j = 0; j < objs; ++j)
			{
				if (mColliders[j] != mColliders[i])
				{
					otherCol = mColliders[j];
					//Not sure what outnorm is used for at the moment might just stick with basic cube/sphere collisions
					if (MovingSphereToMesh(collider->GetPos(), collider->mVelocity, collider->mRadius, mColliders[j]->mMesh, _time, norm))
					{
						SystemLogger::GetLog() << "SPHERE TO MESH COLLISION!";
					}
					else if (otherCol->mIsSphere)
					{
						Sphere s2(otherCol->GetPos(), otherCol->mRadius);
						if (SphereToSphere(s1, s2))
						{
							//reflect? dissapear?
							SystemLogger::GetLog() << "SPHERE TO SPHERE COLLISION!";
						}
						break;
					}
					else if (otherCol->mIsCube)
					{
						AABB aabb(otherCol->mCubeMin, otherCol->mCubeMax);
						if (SphereToAABB(s1, aabb))
						{
							//reflect? dissapear?
							SystemLogger::GetLog() << "SPHERE TO AABB COLLISION FROM SPHERE!";
						}
						break;
					}
					else if (otherCol->mIsPlane)
					{
						Plane plane(otherCol->mPlaneNorm, otherCol->mPlaneOffset);
						int result = SphereToPlane(plane, s1);
						if (result == 1)//in front of plane
						{
							SystemLogger::GetLog() << "SPHERE IN FRONT OF PLANE!";
						}
						else if (result == 2)//behind plane
						{
							SystemLogger::GetLog() << "SPHERE BEHIND PLANE!";
						}
						else if (result == 3)// intersecting plane
						{
							SystemLogger::GetLog() << "SPHERE INTERSECTING PLANE!";
						}
					}
				}
			}
		}
		else if (collider->mIsCube)
		{
			AABB aabb1(collider->mCubeMin, collider->mCubeMax);
			for (int j = 0; j < objs; ++j)
			{
				if (mColliders[j] != mColliders[i])
				{
					otherCol = mColliders[j];
					if (otherCol->mIsCube)
					{
						AABB aabb2(otherCol->mCubeMin, otherCol->mCubeMax);
						if (AABBtoAABB(aabb1, aabb2))
						{
							//reflect? dissapear?
							SystemLogger::GetLog() << "AABB TO AABB COLLISION!";
						}
						break;
					}
					else if (otherCol->mIsSphere)
					{
						Sphere s1(otherCol->object->GetTransform().GetMatrix().fourth, otherCol->mRadius);
						if (SphereToAABB(s1, aabb1))
						{
							//reflect? dissapear?
							SystemLogger::GetLog() << "SPHERE TO AABB COLLISION FROM AABB!";
						}
						break;
					}
					else if (otherCol->mIsPlane)
					{
						Plane plane(otherCol->mPlaneNorm, otherCol->mPlaneOffset);
						int result = AabbToPlane(plane, aabb1);
						if (result == 1)//in front of plane
						{
							SystemLogger::GetLog() << "AABB IN FRONT OF PLANE!";
						}
						else if (result == 2)//behind plane
						{
							SystemLogger::GetLog() << "AABB BEHIND PLANE!";
						}
						else if (result == 3)// intersecting plane
						{
							SystemLogger::GetLog() << "AABB INTERSECTING PLANE!";
							CalcReaction(*collider, *otherCol, _time);
						}
					}
				}
			}
		}
	}
}