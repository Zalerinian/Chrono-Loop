#pragma once

#include "..\Common\Math.h"
#include "..\Rendering\Mesh.h"

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)
#define SameSign(x, y) (x > 0 && y > 0) || (x < 0 && y < 0)



struct Plane
{
	vec4f mNormal;
	float mOffset;
};

struct Sphere
{
	vec4f mPosition;
	float mRadius;
};

struct Frustum
{
	Plane mFaces[6];
	vec4f mPoints[8];
};

struct AABB
{
	vec4f mMin;
	vec4f mMax;
};

namespace Physics
{
	bool RayToTriangle(vec4f& _vert0, vec4f& _vert1, vec4f& _vert2, vec4f& _normal, vec4f& _start, vec4f& _dir, float& _time);
	bool RayToSphere(vec4f& _pos, vec4f & _dir, vec4f & _center, float _radius, float & _time, vec4f & _out);
	bool RayToCylinder(vec4f& _start, vec4f & _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time);
	bool RayToCapsule(vec4f& _start, vec4f& _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time);

	bool MovingSphereToTriangle(vec4f & _vert0, vec4f & _vert1, vec4f & _vert2, vec4f & _normal, vec4f & _start, vec4f & _dir, float _radius, float & _time, vec4f & _outNormal);
	bool MovingSphereToMesh(vec4f & _start, vec4f & _dir, float _radius, Mesh* mesh, float & _time, vec4f & _outNormal);

	void BuildPlane(Plane& _plane, vec4f& _pointA, vec4f& _pointB, vec4f& _pointC);
	int  PointToPlane(Plane& _plane, vec4f& _point);
	int  SphereToPlane(Plane& _plane, Sphere& _sphere);
	int  AabbToPlane(Plane& _plane, AABB& _aabb);
	
	//void BuildFrustum(Frustum& _frustum, float _fov, float _nearPlane, float _farPlane, float _ratio, matrix4& _camPos);
	//bool FrustumToSphere(Frustum& _frustum, Sphere& _sphere);
	//bool FrustumToAABB(Frustum& _frustum, AABB& _aabb);

	bool AABBtoAABB(AABB& _aabb1, AABB& _aabb2);
	bool SphereToSphere(Sphere& _sphere1, Sphere& _sphere2);
	bool SphereToAABB(Sphere& _sphere, AABB& _aabb);

	vec4f CalcAcceleration(vec4f& _force, float _mass);
	vec4f CalcVelocity(vec4f& _vel, vec4f& _accel, float _time);
	vec4f CalcPosition(vec4f& _pos, vec4f& _vel, float _time);
	//vec4f AddForce(vec4f _pos, vec4f _force, float _time);
};