#include "LightComponent.h"

namespace Epoch {

	void LightComponent::Update() {
		MeshComponent::Update();
		if(mNode && mNode->data.Position != mData.position) {
			mData.position = mNode->data.Position;
			UpdateBuffer();
		}
	}

	void LightComponent::UpdateBuffer() {
		SetData(eCB_PIXEL, eBufferDataType_Light, ePB_REGISTER1, &mData);

	}

	LightComponent::LightComponent(const char* _path, const vec4f& _color) : MeshComponent(_path) {
		mType = eCOMPONENT_LIGHT;
		SetAsLight(true);
		mVisible = true;
		mData.color = _color;
		Update();
	}

	void LightComponent::SetColor(const vec4f & _color) {
		mData.color = _color;
		UpdateBuffer();
	}

	void LightComponent::SetColor(float _r, float _g, float _b, float _attenuation) {
		mData.color.Set(_r, _g, _b, _attenuation);
		UpdateBuffer();
	}


}
