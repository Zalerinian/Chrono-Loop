#pragma once
#include "Rendering\Mesh.h"
#include <vector>
#include <unordered_set>

namespace Epoch
{
	class NavMesh
	{
	public:
		std::vector<Triangle> mTriangles;
		std::vector<unsigned int> mEdgeIndicies;//points to the triangle that is on the edge of a triangle
		std::vector<unsigned int> mEdgeAmount;//should never exceed 3

		NavMesh();
		~NavMesh();
	};
}