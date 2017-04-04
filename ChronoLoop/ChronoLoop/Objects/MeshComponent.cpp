#include "MeshComponent.h"
#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"
#include "../Rendering/Renderer.h"
#include "../Common/Settings.h"

namespace Epoch {
	void MeshComponent::CreateNode() {
		mNode = Renderer::Instance()->AddOpaqueNode(*mShape);
	}

	void MeshComponent::RemoveShape() {
		DESTROY_NODE(mNode);
		Renderer::Instance()->RemoveOpaqueNode(*mShape);
	}

	MeshComponent::MeshComponent(const char * _path) {
		mType = eCOMPONENT_MESH;
		mShape = new RenderShape(_path, true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_FILLED;
		if (CanCreateNode()) {
			CreateNode();
			mVisible = true;
		} else {
			mVisible = false;
			mNode = nullptr;
		}
	}

	void MeshComponent::Update() {
		if (mNode) {
			mNode->data = mObject->GetWorld();
		}
	}

	void MeshComponent::Destroy() {
		SystemLogger::Debug() << "Destroying shape " << mShape->GetName() << std::endl;
		RemoveShape();
		delete mShape;
	}

	MeshComponent* MeshComponent::SetVisible(bool _vis) {
		if (_vis) {
			if (!mVisible) {
				mVisible = true;
				if (mNode == nullptr && CanCreateNode()) {
					CreateNode();
				}
				Update();
			}
		} else {
			if (mVisible) {
				if (mNode) {
					memset(&mNode->data, 0, sizeof(mNode->data));
				}
				mVisible = false;
			}
		}
		return this;
	}

	MeshComponent* MeshComponent::AddTexture(const char * _path, TextureType _type) {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		if (TextureManager::Instance()->iGetTexture2D(_path, &srv, nullptr) == TextureManager::TextureStatus::eSuccess) {

			// If the texture at the given path is not what is already set, remove the shape from the render set, add the
			// texture, and, if visible, reinsert the shape.
			if (srv != mShape->GetContext().mTextures[_type]) {
				RemoveShape();
				mShape->AddTexture(_path, _type);
				if (mVisible) {
					CreateNode();
				}
			}
		}
		return this;
	}

	void MeshComponent::SetRasterState(RasterState _t) {
		if (_t != mShape->GetContext().mRasterState) {
			RemoveShape();
			mShape->GetContext().mRasterState = _t;
			if (mVisible) {
				CreateNode();
			}
		}
	}

	void MeshComponent::SetVertexShader(VertexShaderFormat _vf) {
		if (_vf != mShape->GetContext().mGeoShaderFormat) {
			RemoveShape();
			mShape->GetContext().mVertexShaderFormat = _vf;
			if (mVisible) {
				CreateNode();
			}
		}
	}

	void MeshComponent::SetPixelShader(PixelShaderFormat _pf) {
		if (_pf != mShape->GetContext().mGeoShaderFormat) {
			RemoveShape();
			mShape->GetContext().mPixelShaderFormat = _pf;
			if (mVisible) {
				CreateNode();
			}
		}
	}

	void MeshComponent::SetGeometryShader(GeometryShaderFormat _gf) {
		if (_gf != mShape->GetContext().mGeoShaderFormat) {
			RemoveShape();
			mShape->GetContext().mGeoShaderFormat = _gf;
			if (mVisible) {
				CreateNode();
			}
		}
	}

	RenderShape * MeshComponent::GetShape() {
		return mShape;
	}

	void MeshComponent::ForceReinsertion() {
		RemoveShape();
		CreateNode();
	}

	bool MeshComponent::CanCreateNode() {
		return !Settings::GetInstance().GetBool("LevelIsLoading");
	}

}