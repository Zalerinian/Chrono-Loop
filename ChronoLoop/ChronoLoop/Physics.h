///////////////////////////////////////
//Written: 2/8/2017
//Author: Chris Burt
///////////////////////////////////////
#pragma once

#include "..\ChronoLoop\Common\Math.h"

class Physics
{
	vec4f mGravity;
	float mFrictionCo;

	bool SameSign(float _f1, float _f2);
	bool IntersectRayToTriangle(vec4f& vert0, vec4f& vert1, vec4f& vert2, vec4f& norm, vec4f& start, vec4f& d, float& t);
	bool IntersectRayToSphere(vec4f &p, vec4f &d, vec4f &center, float radius, float &t, vec4f &q);
	bool IntersectRayToCylinder(vec4f& sa, vec4f& n, vec4f& p, vec4f& q, float r, float& t);
	bool IntersectRayToCapsule(vec4f& sa, vec4f& n, vec4f& p, vec4f& q, float r, float& t);
	bool IntersectMovingSphereToTriangle(vec4f& vert0, vec4f& vert1, vec4f& vert2, vec4f& norm, vec4f& start, vec4f& d, float r, float& t, vec4f& outNormal);
	//bool IntersectMovingSphereToMesh(vec4f& start, vec4f& d, float r, ED2Mesh* mesh, float& t, vec4f& outNormal);
};