#include "MeshComponent.h"
#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"
#include "../Rendering/Renderer.h"
#include "../Common/Settings.h"

#define DESTROY_NODE(x) { \
	if(x != nullptr) { \
		delete x; \
		x = nullptr; \
	} \
}

namespace Epoch {

	MeshComponent::MeshComponent(const char * _path) {
		mType = eCOMPONENT_MESH;
		mShape = new RenderShape(_path, true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_FILLED;
		if (!Settings::GetInstance().GetBool("LevelIsLoading")) {
			mNode = Renderer::Instance()->AddNode(mShape);
			mVisible = true;
		}
		else
		{
			mVisible = false;
			mNode = nullptr;
		}
	}

	void MeshComponent::Update() {
		if (mNode) {
			mObject->GetTransform().GetMatrix(mNode->data);
		}
	}

	void MeshComponent::Destroy() {
		SystemLogger::Debug() << "Destroying shape " << mShape->GetName() << std::endl;
		DESTROY_NODE(mNode);
		delete mShape;
	}

	MeshComponent* MeshComponent::SetVisible(bool _vis) {
		// TODO: Check to see if this is an efficient way to make meshes appear and disappear, because this involves
		// a bit of looping. It could possibly be more efficient to set a bool on the render shape saying it's
		// invisible.
		// Side thought: Make the render set actually a list of MeshComponents, since they have shapes and direct
		// access to the position of the object.
		if (_vis) {
			if (!mVisible) {
				mNode = Renderer::Instance()->AddNode(mShape);
			}
		}
		else {
			if (mVisible) {
				DESTROY_NODE(mNode);
			}
		}
		return this;
	}

	MeshComponent* MeshComponent::AddTexture(const char * _path, TextureType _type) {
		DESTROY_NODE(mNode);
		mShape->AddTexture(_path, _type);
		mNode = Renderer::Instance()->AddNode(mShape);
		return this;
	}

	void MeshComponent::SetRasterState(RasterState _t) {
		if (_t != mShape->GetContext().mRasterState) {
			DESTROY_NODE(mNode);
			mShape->GetContext().mRasterState = _t;
			mNode = Renderer::Instance()->AddNode(mShape);
		}
	}

}