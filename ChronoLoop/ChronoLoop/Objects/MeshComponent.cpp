#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"
#include "../Rendering/Renderer.h"
#include "MeshComponent.h"

namespace Epoch
{

	MeshComponent::MeshComponent(const char * _path)
	{
		mType = eCOMPONENT_MESH;
		mShape = new RenderShape(_path, true, ePS_TEXTURED, eVS_TEXTURED);
		mNode = Renderer::Instance()->AddNode(mShape);
	}

	void MeshComponent::Update()
	{
		mObject->GetTransform().GetMatrix(mShape->mPosition);
		mShape->mPosition = mShape->mPosition.Transpose();
		if (mNode) {
			mNode->data = mShape->mPosition.Transpose();
		}
	}

	void MeshComponent::Destroy()
	{
		mDestroyed = true;
		Renderer::Instance()->RemoveNode(mShape);
		delete mNode;
		delete mShape;
	}

	void MeshComponent::SetVisible(bool _vis)
	{
		// TODO: Check to see if this is an efficient way to make meshes appear and disappear, because this involves
		// a bit of looping. It could possibly be more efficient to set a bool on the render shape saying it's
		// invisible.
		// Side thought: Make the render set actually a list of MeshComponents, since they have shapes and direct
		// access to the position of the object.
		if (_vis && mNode == nullptr)
		{
			mNode = Renderer::Instance()->AddNode(mShape);
		}
		else
		{
			Renderer::Instance()->RemoveNode(mShape);
			delete mNode;
			mNode = nullptr;
		}
	}

	void MeshComponent::AddTexture(const char * _path, TextureType _type)
	{
		mShape->AddTexture(_path, _type);
		Renderer::Instance()->RemoveNode(mShape);
		delete mNode;
		mNode = Renderer::Instance()->AddNode(mShape);
	}

	void MeshComponent::SetRasterState(RasterState _t)
	{
		mShape->GetContext().mRasterState = _t;
		Renderer::Instance()->RemoveNode(mShape);
		delete mNode;
		mNode = Renderer::Instance()->AddNode(mShape);
	}

}