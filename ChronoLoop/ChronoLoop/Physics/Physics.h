#pragma once

#include "..\Common\Math.h"
#include "..\Rendering\Mesh.h"
#include <vector>
#include <mutex>
#define PMIN(x, y) ((x < y) ? x : y)
#define PMAX(x, y) ((x > y) ? x : y)
#define SameSign(x, y) ((x > 0 && y > 0) || (x < 0 && y < 0))

namespace Epoch
{

	class  BaseObject;
	class  Collider;
	class  PlaneCollider;
	class  SphereCollider;
	class  CubeCollider;
	class  OrientedCubeCollider;
	struct Triangle;

	class Physics
	{
		static Physics* mInstance;

		Physics();
		~Physics() {};

	public:
		std::vector< BaseObject*> mObjects;
		std::mutex PhysicsLock;

		static Physics* Instance();
		static void		Destroy();

		void			Update(float _time);

		bool			RayToTriangle(vec3f& _vert0, vec3f& _vert1, vec3f& _vert2, vec3f& _normal, vec3f& _start, vec3f& _dir, float& _time);
		bool			RayToSphere(vec3f& _pos, vec3f & _dir, vec3f & _center, float _radius, float & _time, vec3f & _out);
		bool			RayToCylinder(vec3f& _start, vec3f & _normal, vec3f & _point1, vec3f & _point2, float _radius, float & _time);
		bool			RayToCapsule(vec3f& _start, vec3f& _normal, vec3f & _point1, vec3f & _point2, float _radius, float & _time);
		bool			Linecast(vec3f& _vert0, vec3f& _vert1, vec3f& _vert2, vec3f& _normal, vec3f& _start, vec3f& _end, vec3f& _hit);

		bool			MovingSphereToTriangle(vec3f & _vert0, vec3f & _vert1, vec3f & _vert2, vec3f & _normal, vec3f & _start, vec3f & _dir, float _radius, float & _time, vec3f & _outNormal);
		//bool			MovingSphereToMesh(vec3f & _start, vec3f & _dir, float _radius, Mesh* mesh, float & _time, vec3f & _outNormal);

		int				PointToPlane(PlaneCollider& _plane, vec3f& _point);
		int				SphereToPlane(PlaneCollider& _plane, SphereCollider& _sphere);
		int				AabbToPlane(PlaneCollider& _plane, CubeCollider& _aabb);

		//void BuildFrustum(Frustum& _frustum, float _fov, float _nearPlane, float _farPlane, float _ratio, matrix4& _camPos);
		//bool FrustumToSphere(Frustum& _frustum, Sphere& _sphere);
		//bool FrustumToAABB(Frustum& _frustum, AABB& _aabb);

		bool			AABBtoAABB(CubeCollider& _aabb1, CubeCollider& _aabb2);
		bool			OBBtoOBB(OrientedCubeCollider& _obb1, OrientedCubeCollider& _obb2);
		bool			OBBtoPlane(OrientedCubeCollider& _obb, PlaneCollider& _plane);
		bool			SphereToSphere(SphereCollider& _sphere1, SphereCollider& _sphere2);
		bool			SphereToAABB(SphereCollider& _sphere, CubeCollider& _aabb);
		bool			SphereToTriangle(SphereCollider& _sphere, Triangle& _tri, vec3f& _displacement);

		vec3f			CalcAcceleration(vec3f& _force, float _mass);
		vec3f			CalcVelocity(vec3f& _vel, vec3f& _accel, float _time);
		vec3f			CalcPosition(vec3f& _pos, vec3f& _vel, float _time);
		void			CalcFriction(Collider& _col, vec3f& _norm, float _static, float _kinetic);
	};

}