#pragma once
#include "MeshComponent.h"
#include "../Rendering/RenderList.h"

namespace Epoch {

	class TransparentMeshComponent : public MeshComponent {
	protected:
		struct AlphaData {
			union {
				struct {
					vec4f alpha;
				};
				BufferWidth padding;
			};

			AlphaData() {
				alpha.Set(0, 0, 0, 0);
			}
		};

		AlphaData mAlpha;
		//bool mOpaque = true, mWasOpaque = true;
		virtual void CreateNode();
		virtual void RemoveShape();
		void RemoveTransparentShape();
		void RemoveOpaqueShape();
		void CreateAlphaBuffer();
	public:
		TransparentMeshComponent(const char* _file, float _alpha = 1.0f);
		float GetAlpha();
		void SetAlpha(float _a);
		bool IsOpaque();
	};

}