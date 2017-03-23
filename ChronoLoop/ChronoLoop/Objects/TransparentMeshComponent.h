#pragma once
#include "MeshComponent.h"

namespace Epoch {

	class TransparentMeshComponent : public MeshComponent {
	protected:
		float mAlpha = 1.0f;
		virtual void CreateNode();
		void CreateAlphaBuffer();
	public:
		TransparentMeshComponent(const char* _file, float _alpha = 1.0f);
		float GetAlpha();
		void SetAlpha(float _a);
	};

}