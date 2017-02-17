#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"
#include "../Rendering/Renderer.h"

MeshComponent::MeshComponent(const char * _path) {
	mType = eCOMPONENT_MESH;
	mShape = new RenderEngine::RenderShape(_path, true, RenderEngine::ePS_TEXTURED, RenderEngine::eVS_TEXTURED);
	RenderEngine::Renderer::Instance()->AddNode(mShape);
	// TODO - Remove the RenderShape from the renderset when the mesh component is destroyed.
}

void MeshComponent::Update() {
	mObject->GetTransform().GetMatrix(mShape->mPosition);
	mShape->mPosition = Math::MatrixTranspose(mShape->mPosition);
}

void MeshComponent::Destroy() {

}

void MeshComponent::SetVisible(bool _vis) {

}
