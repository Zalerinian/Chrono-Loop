#pragma once
#include "MeshComponent.h"

namespace Epoch {

	struct LightData {
		union {
			struct {
				union {
					struct {

						float Red;
						float Green;
						float Blue;
						float Attenuation;
						float x;
						float y;
						float z;
						float w;
					};
					struct {
						vec4f color;
						vec4f position;
					};
				};
			};
			BufferWidth padding;
		};
		LightData() {
			memset(&padding, 0, sizeof(padding));
		}
	};

	class LightComponent : public MeshComponent {
		LightData mData;

		virtual void Update() override;
		void UpdateBuffer();

	public:
		LightComponent(const char* _path, const vec4f& _color);
		void SetColor(const vec4f& _color);
		void SetColor(float _r, float _g, float _b, float _attenuation);
	};

}