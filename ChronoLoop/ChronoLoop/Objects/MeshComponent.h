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
		bool mBlended = false;

		BufferDataType mVertexBufferTypes[eVB_MAX];
		BufferDataType mPixelBufferTypes[ePB_MAX];
		BufferDataType mGeoBufferTypes[eGB_MAX];

		virtual void CreateOpaqueNode();
		virtual void CreateTransparentNode();
		virtual void CreateNode();
		virtual void RemoveOpaqueNode();
		virtual void RemoveTransparentNode();
		virtual void RemoveNode();

		virtual void UpdateBuffer(ConstantBufferType _t, unsigned char _index);
		void CreateAlphaBuffer(float alpha = 1.0f);
	public:
		MeshComponent(const char *_path, bool _EnableBlending = false);
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

		
		// ********************************************************************************
		/// <summary>
		/// 
		/// </summary>
		/// <param name="_ButWillItBlend"></param> 
		/// <created>Drew,4/5/2017</created>
		/// <changed>Drew,4/5/2017</changed>
		// ********************************************************************************
		void SetBlended(bool _ButWillItBlend);
		RenderShape* GetShape();
		bool GetBlended();
		void ForceReinsertion();
		void EnableBufferUpdates(bool _updateNow = false);
		void DisableBufferUpdates();
		bool GetBufferUpdates();

		// ********************************************************************************
		/// <summary>
		/// Sets the data and buffer data type for the given index.
		/// </summary>
		/// <param name="_t">The Constant Buffer type, is the data for a Vertex, Pixel, or Geometry constant buffer?</param>
		/// <param name="_bt">The Buffer Data Type, an enum value used to see what type of data is stored at a buffer slot, to help prevent errornous writes to a given index.</param>
		/// <param name="_index">The index in the corresponding array of buffers for the shape, for the ConstantBufferType. This should be a e*B_REGISTERn enum value, where * is V, P, or G, and n is the register number it maps to.</param>
		/// <param name="_data">The actual data to put into the buffer. Actual content depends on the buffer data type, but it should be no larger than the size of a BufferWidth object.</param>
		// ********************************************************************************
		void SetData(ConstantBufferType _t, BufferDataType _bt, unsigned char _index, void* _data);
		void UpdateData(ConstantBufferType _t, unsigned char _index, void* _data);
		//void SetDataBytes(ConstantBufferType _t, unsigned char _index, unsigned char _offset, unsigned char _size, void* _data); // Would use CopySubresourceRegion to update


		bool CanCreateNode();
	};

}
