#pragma once
#include "Component.h"
#include "../Rendering/RendererDefines.h"
#include "../Rendering/RenderShape.h"
#include "../Common/GhostList.h"
#include <unordered_map>

#define DESTROY_NODE(x) { \
	if(x != nullptr) { \
		delete x; \
		x = nullptr; \
	} \
}

namespace Epoch
{
	struct RenderShape;

	enum BufferDataType {
		/// <summary>
		/// Represents that the buffer does not contain any data.
		/// </summary>
		eBufferDataType_Nullptr,

		/// <summary>The buffer only holds an alpha value, used to control transparency.</summary>
		eBufferDataType_Alpha,

		/// <summary>
		/// The buffer holds an alpha value, and a vec4f for multi-texturing offsets. 
		/// The vec4f contains, in order, the Multiscan V offset, the Multiscan alpha value,
		/// the Scanline V offset, and the Scanline Alpha value.
		/// </summary>
		eBufferDataType_Scanline

	};

	class MeshComponent : public Component
	{
	protected:
		GhostList<matrix4>::GhostNode* mNode = nullptr;
		RenderShape* mShape = nullptr;
		bool mVisible = true;
		bool mBuffersCanUpdate = true;

		BufferDataType mVertexBufferTypes[eVB_MAX];
		BufferDataType mPixelBufferTypes[ePB_MAX];
		BufferDataType mGeoBufferTypes[eGB_MAX];


		virtual void CreateNode();
		virtual void RemoveShape();
	public:
		MeshComponent(const char *_path);
		virtual void Update() override;
		virtual void Destroy() override;
		MeshComponent* SetVisible(bool _vis);
		MeshComponent* SetAlpha(float _a);
		MeshComponent* AddTexture(const char *_path, TextureType _type);
		inline bool IsVisible() { return mVisible; };
		inline Triangle *GetTriangles() { return mShape->GetTriangles(); }
		inline size_t GetTriangleCount() { return mShape->GetTriangleCount(); }
		inline RenderContext& GetContext() { return mShape->GetContext(); }
		void SetRasterState(RasterState _t);
		void SetVertexShader(VertexShaderFormat _vf);
		void SetPixelShader(PixelShaderFormat _pf);
		void SetGeometryShader(GeometryShaderFormat _gf);
		RenderShape* GetShape();
		void ForceReinsertion();
		void EnableBufferUpdates(bool _updateNow = false);
		void DisableBufferUpdates();
		bool GetBufferUpdates();
		void SetData(ConstantBufferType _t, BufferDataType _bt, unsigned char _index, void* _data);
		//void SetDataBytes(ConstantBufferType _t, unsigned char _index, unsigned char _offset, unsigned char _size, void* _data);


		bool CanCreateNode();
	};

}
