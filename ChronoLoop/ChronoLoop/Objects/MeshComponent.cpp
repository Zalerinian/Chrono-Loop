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
	// TODO: Check to see if this is an efficient way to make meshes appear and disappear, because this involves
	// a bit of looping. It could possibly be more efficient to set a bool on the render shape saying it's
	// invisible.
	// Side thought: Make the render set actually a list of MeshComponents, since they have shapes and direct
	// access to the position of the object.
	if (_vis) {
		RenderEngine::Renderer::Instance()->AddNode(mShape);
	} else {
		RenderEngine::Renderer::Instance()->RemoveNode(mShape);
	}
}

void MeshComponent::AddTexture(const char * _path, RenderEngine::TextureType _type) {
	mShape->AddTexture(_path, _type);
	RenderEngine::Renderer::Instance()->RemoveNode(mShape);
	RenderEngine::Renderer::Instance()->AddNode(mShape);
}
