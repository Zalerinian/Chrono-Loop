#pragma once

#include "..\Common\Math.h"
#include "..\Rendering\Mesh.h"
#include <vector>
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
	struct Triangle;

	class Physics
	{
		static Physics* mInstance;

		Physics();
		~Physics() {};

	public:
		bool mRayCasting;
		std::vector< BaseObject*> mObjects;

		static Physics* Instance();
		static void		Destroy();

		void			Update(float _time);

		bool			RayToTriangle(vec4f& _vert0, vec4f& _vert1, vec4f& _vert2, vec4f& _normal, vec4f& _start, vec4f& _dir, float& _time);
		bool			RayToSphere(vec4f& _pos, vec4f & _dir, vec4f & _center, float _radius, float & _time, vec4f & _out);
		bool			RayToCylinder(vec4f& _start, vec4f & _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time);
		bool			RayToCapsule(vec4f& _start, vec4f& _normal, vec4f & _point1, vec4f & _point2, float _radius, float & _time);

		bool			MovingSphereToTriangle(vec4f & _vert0, vec4f & _vert1, vec4f & _vert2, vec4f & _normal, vec4f & _start, vec4f & _dir, float _radius, float & _time, vec4f & _outNormal);
		bool			MovingSphereToMesh(vec4f & _start, vec4f & _dir, float _radius, Mesh* mesh, float & _time, vec4f & _outNormal);

		void			BuildPlane(PlaneCollider& _plane, vec4f& _pointA, vec4f& _pointB, vec4f& _pointC);
		int				PointToPlane(PlaneCollider& _plane, vec4f& _point);
		int				SphereToPlane(PlaneCollider& _plane, SphereCollider& _sphere);
		int				AabbToPlane(PlaneCollider& _plane, CubeCollider& _aabb);

		//void BuildFrustum(Frustum& _frustum, float _fov, float _nearPlane, float _farPlane, float _ratio, matrix4& _camPos);
		//bool FrustumToSphere(Frustum& _frustum, Sphere& _sphere);
		//bool FrustumToAABB(Frustum& _frustum, AABB& _aabb);

		bool			AABBtoAABB(CubeCollider& _aabb1, CubeCollider& _aabb2);
		bool			SphereToSphere(SphereCollider& _sphere1, SphereCollider& _sphere2);
		bool			SphereToAABB(SphereCollider& _sphere, CubeCollider& _aabb);
		bool			SphereToTriangle(SphereCollider& _sphere, Triangle& _tri, vec4f& _displacement);

		vec4f			CalcAcceleration(vec4f& _force, float _mass);
		vec4f			CalcVelocity(vec4f& _vel, vec4f& _accel, float _time);
		vec4f			CalcPosition(vec4f& _pos, vec4f& _vel, float _time);
		void			CalcReaction(Collider& _col1, Collider& _col2, float _time);
		void			PlaneColReaction(Collider& _col, Collider& _plane);
		void			CalcFriction(Collider& _col, vec4f& _norm, float _static, float _kinetic);
	};

}