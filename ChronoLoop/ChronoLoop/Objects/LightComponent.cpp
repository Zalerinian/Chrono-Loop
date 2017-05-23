#include "LightComponent.h"

namespace Epoch {

	void LightComponent::Update() {
		MeshComponent::Update();
		if(mNode && mNode->data.Position != mData.position) {
			mData.position = mNode->data.Position;
			SetData(eCB_PIXEL, eBufferDataType_Light, ePB_REGISTER1, &mData);
		}
	}

	LightComponent::LightComponent(const char* _path, const vec4f& _color) : MeshComponent(_path) {
		SetAsLight(true);
		mData.color = _color;
		Update();
	}


}
