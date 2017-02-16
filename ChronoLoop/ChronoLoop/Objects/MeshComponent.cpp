#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"

void MeshComponent::Update() {
	object->GetTransform().GetMatrix(mShape->mPosition);
}

void MeshComponent::Destroy() {

}

void MeshComponent::SetVisible(bool _vis) {

}
