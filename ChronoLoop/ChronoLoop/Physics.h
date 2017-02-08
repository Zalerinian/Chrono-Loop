///////////////////////////////////////
//Written: 2/8/2017
//Author: Chris Burt
///////////////////////////////////////
#pragma once

#include "..\ChronoLoop\Common\Math.h"

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
	Plane* mFaces[6];
	vec4f* mPoints[8];
};

struct AABB
{
	vec4f mMin;
	vec4f mMax;
};

class Physics
{
	vec4f mGravity;
	float mFrictionCo;

	bool IntersectRayToTriangle(vec4f& _vert0, vec4f& _vert1, vec4f& _vert2, vec4f& _normal, vec4f& _start, vec4f& _dir, float& _time);
	bool IntersectRayToSphere(vec4f& _pos, vec4f& _dir, vec4f& _center, float _radius, float& _time, vec4f& _out);
	bool IntersectRayToCylinder(vec4f& _start, vec4f& _normal, vec4f& _point1, vec4f& _point2, float _radius, float& _time);
	bool IntersectRayToCapsule(vec4f& _start, vec4f& _normal, vec4f& _point1, vec4f& _point2, float _radius, float& _time);
	bool IntersectMovingSphereToTriangle(vec4f& _vert0, vec4f& _vert1, vec4f& _vert2, vec4f& _normal, vec4f& _start, vec4f& _dir, float _radius, float& _time, vec4f& _outNormal);
	//bool IntersectMovingSphereToMesh(vec4f& _start, vec4f& _dir, float _radius, ED2Mesh* mesh, float& _time, vec4f& _outNormal);

	void BuildPlane(Plane& _plane, vec4f _pointA, vec4f& _pointB, vec4f& _pointC);
	int ClassifyPointToPlane(Plane& _plane, vec4f& _point);
	int ClassifySphereToPlane(Plane& _plane, Sphere& _sphere);
	int ClassifyAabbToPlane(Plane& _plane, AABB& _aabb);
	//void BuildFrustum(Frustum& _frustum, float _fov, float _nearPlane, float _farPlane, float _ratio, matrix4& _camPos);
};