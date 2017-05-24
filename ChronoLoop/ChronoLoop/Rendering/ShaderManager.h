#pragma once
#include "RendererDefines.h"
#include <memory>
#include <wrl/client.h>

struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;

namespace Epoch {

	class ShaderManager {
		static ShaderManager* sInstance;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShaders[ePS_MAX];
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShaders[eVS_MAX];
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeoShaders[eGS_MAX];


		ShaderManager();
		~ShaderManager();
	public:
		static ShaderManager* Instance();
		static void DestroyInstance();
		
		void ApplyVShader(VertexShaderFormat f);
		void ApplyPShader(PixelShaderFormat f);
		void ApplyGShader(GeometryShaderFormat _f);
	
		// Public Instance Functions
		Microsoft::WRL::ComPtr<ID3D11PixelShader>& GetPixelShader(PixelShaderFormat f);
		Microsoft::WRL::ComPtr<ID3D11VertexShader>& GetVertexShader(VertexShaderFormat f);
	
	};
}