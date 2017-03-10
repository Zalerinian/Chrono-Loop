#pragma once
#include "RendererDefines.h"
#include <memory>

struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace Epoch {

	class ShaderManager {
		static ShaderManager* sInstance;
		std::shared_ptr<ID3D11PixelShader*> mPixelShaders[ePS_MAX];
		std::shared_ptr<ID3D11VertexShader*> mVertexShaders[eVS_MAX];



		ShaderManager();
		~ShaderManager();
	public:
		static ShaderManager* Instance();
		static void DestroyInstance();
		
		void ApplyVShader(VertexShaderFormat f);
		void ApplyPShader(PixelShaderFormat f);
	
		// Public Instance Functions
		std::shared_ptr<ID3D11PixelShader*> GetPixelShader(PixelShaderFormat f);
		std::shared_ptr<ID3D11VertexShader*> GetVertexShader(VertexShaderFormat f);
	
	};
}