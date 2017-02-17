#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"
#include "../Rendering/Renderer.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(const char * _path) {
	mType = eCOMPONENT_MESH;
	mShape = new RenderEngine::RenderShape(_path, true, RenderEngine::ePS_TEXTURED, RenderEngine::eVS_TEXTURED);
	RenderEngine::Renderer::Instance()->AddNode(mShape);
}

void MeshComponent::Update() {
	mObject->GetTransform().GetMatrix(mShape->mPosition);
	mShape->mPosition = Math::MatrixTranspose(mShape->mPosition);
}

void MeshComponent::Destroy() {
	mDestroyed = true;
	RenderEngine::Renderer::Instance()->RemoveNode(mShape);
	delete mShape;
}

void MeshComponent::SetVisible(bool _vis) {
	// TODO: Implement mesh component visibility.
}

void MeshComponent::AddTexture(const char * _path, RenderEngine::TextureType _type) {
	mShape->AddTexture(_path, _type);
}
