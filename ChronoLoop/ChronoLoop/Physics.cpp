///////////////////////////////////////
//Written: 2/8/2017
//Author: Chris Burt
///////////////////////////////////////

#include "stdafx.h"
#include "Physics.h"



bool Physics::SameSign(float _f1, float _f2)
{
	if ((_f1 > 0 && _f2 > 0) || (_f1 < 0 && _f2 < 0))
		return true;
	return false;
}

bool Physics::IntersectRayToTriangle(vec4f& vert0, vec4f& vert1, vec4f& vert2, vec4f& norm, vec4f& start, vec4f& d, float& t)
{
	if (((norm * d) > 0 || (vert0 - start) * norm) > 0)
		return false;

	vec4f sa = vert0 - start;
	vec4f sb = vert1 - start;
	vec4f sc = vert2 - start;
	vec4f N1, N2, N3;
	N1 = sc ^ sb;
	N2 = sa ^ sc;
	N3 = sb ^ sa;
	float dn1 = d * N1;
	float dn2 = d * N2;
	float dn3 = d * N3;

	if (dn1 == 0.0f && dn2 == 0.0f && dn3 == 0.0f)
	{
		t = 0;
		return false;
	}

	if (SameSign(dn1, dn2) && SameSign(dn2, dn3))
	{
		float offset = norm * vert0;
		t = ((offset - (start * norm)) / (norm * d));
		return true;
	}
}

bool Physics::IntersectRayToSphere(vec4f& p, vec4f& d, vec4f& center, float radius, float& t, vec4f& q)
{
	vec4f check = p - center;

	vec4f m = p - center;
	float b = m * d;
	float c = (m * m) - (radius * radius);
	if (c < 0.0f || b > 0.0f)
		return false;

	float dis = b*b - c;
	if (dis < 0.0f)
		return false;

	t = -b - sqrt(dis);
	if (t < 0.0f)
	{
		t = 0.0f;
		vec4f np = { p.data.x + t, p.data.y + t, p.data.z + t, 1.0f };
		q = np ^ d;
	}

	return true;
}

bool Physics::IntersectRayToCylinder(vec4f & sa, vec4f & n, vec4f & p, vec4f & q, float r, float & t)
{
	vec4f d = q - p;
	vec4f od = p - q;
	vec4f m = sa - p; 

	float dd = d * d;
	float nd = n * d;
	float mn = m * n;
	float md = m * d;
	float mm = m * m;

	if ((md < 0 && nd < 0) || (md > dd && nd > 0))
		return false;

	float a = dd - nd * nd;
	float b = dd*mn - nd*md;
	float c = dd*(mm - r*r) - md*md;

	if (abs(a) < FLT_EPSILON)
		return false;

	float dis = b*b - a*c;
	if (dis < 0)
		return false;

	t = (-b - sqrt(dis)) / a;

	if (t < 0)
		return false;

	vec4f col = sa + n * t;

	if (od * (q - col) > 0 || d * (p - col) > 0)
		return false;

	return true;
}

bool Physics::IntersectRayToCapsule(vec4f & sa, vec4f & n, vec4f & p, vec4f & q, float r, float & t)
{
	float fTime = FLT_MAX;
	t = FLT_MAX;
	bool bReturn = false;

	if (IntersectRayToCylinder(sa, n, p, q, r, t))
	{
		fTime = min(t, fTime);
		t = fTime;
		bReturn = true;
	}

	vec4f pcol, qcol;
	if (IntersectRayToSphere(sa, n, p, r, t, pcol))
	{
		fTime = min(t, fTime);
		t = fTime;
		bReturn = true;
	}

	if (IntersectRayToSphere(sa, n, q, r, t, qcol))
	{
		fTime = min(t, fTime);
		t = fTime;
		bReturn = true;
	}
	return bReturn;
}

bool Physics::IntersectMovingSphereToTriangle(vec4f & vert0, vec4f & vert1, vec4f & vert2, vec4f & norm, vec4f & start, vec4f & d, float r, float & t, vec4f & outNormal)
{
	bool bReturn = false;
	float fTime = FLT_MAX;
	t = FLT_MAX;

	vec4f verts[3] = { vert0, vert1, vert2 };
	vec4f offset0 = vert0 + (norm * r);
	vec4f offset1 = vert1 + (norm * r);
	vec4f offset2 = vert2 + (norm * r);

	if (IntersectRayToTriangle(offset0, offset1, offset2, norm, start, d, t))
	{
		fTime = min(t, fTime);
		t = fTime;
		outNormal = norm;
		bReturn = true;
	}
	else
	{
		if (IntersectRayToCapsule(start, d, vert0, vert1, r, t))
		{
			fTime = min(t, fTime);
			t = fTime;
			vec4f temp1 = d * t + start;
			vec4f temp2 = (vert1 - vert0) * t + vert0;
			outNormal = (temp1 - temp2).Normalize();
			bReturn = true;
		}

		if (IntersectRayToCapsule(start, d, vert1, vert2, r, t))
		{
			fTime = min(t, fTime);
			t = fTime;
			vec4f temp1 = d * t + start;
			vec4f temp2 = (vert1 - vert0) * t + vert0;
			outNormal = (temp1 - temp2).Normalize();
			bReturn = true;
		}

		if (IntersectRayToCapsule(start, d, vert2, vert0, r, t))
		{
			fTime = min(t, fTime);
			t = fTime;
			vec4f temp1 = d * t + start;
			vec4f temp2 = (vert1 - vert0) * t + vert0;
			outNormal = (temp1 - temp2).Normalize();
			bReturn = true;
		}
	}

	return bReturn;
}

//bool Physics::IntersectMovingSphereToMesh(vec4f & start, vec4f & d, float r, ED2Mesh * mesh, float & t, vec4f & outNormal)
//{
//	bool bCollision = false;
//	t = FLT_MAX;
//	float fTime = FLT_MAX;
//	
//	unsigned int tempSortedIndicies[3] = { 0,0,0 };
//
//	for (unsigned int i = 0; i < mesh->m_Triangles.size(); i++) {
//
//		ED2Triangle currTri = mesh->m_Triangles[i];
//		vec4f currNorm = mesh->m_TriNorms[i];
//
//		for (unsigned int x = 0; x < 3; x++) {
//			tempSortedIndicies[x] = currTri.indices[x];
//		}
//
//		unsigned int temp = 0;
//		for (unsigned int i = 1; i < 4; i++) {
//			for (unsigned int j = 0; j < 3; j++) {
//				if (tempSortedIndicies[j + 1] < tempSortedIndicies[j]) {
//					temp = tempSortedIndicies[j];
//					tempSortedIndicies[j] = tempSortedIndicies[j + 1];
//					tempSortedIndicies[j + 1] = temp;
//					break;
//				}
//			}
//		}
//
//		if (IntersectMovingSphereToTriangle(
//			mesh->m_Vertices[tempSortedIndicies[0]].pos,
//			mesh->m_Vertices[tempSortedIndicies[1]].pos,
//			mesh->m_Vertices[tempSortedIndicies[2]].pos,
//			currNorm, start, d, r, fTime, outNormal))
//		{
//			t = fminf(t, fTime);
//			outNormal = currNorm;
//			bCollision = true;
//		}
//	}
//
//	return bCollision;
//}

