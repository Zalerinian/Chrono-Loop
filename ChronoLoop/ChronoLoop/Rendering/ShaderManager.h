#pragma once
#include "RendererDefines.h"
#include <memory>

struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace RenderEngine {

	class ShaderManager {
		static ShaderManager* sInstance;
		std::shared_ptr<ID3D11PixelShader*> mPixelShaders[ePS_MAX];
		std::shared_ptr<ID3D11VertexShader*> mVertexShaders[eVS_MAX];

		bool LoadShaderBinary(const char *_path, char *_bytes, unsigned int &_size);



		ShaderManager();
		~ShaderManager();
	public:
		static ShaderManager* Instance();
		static void DestroyInstance();
		
	
		// Public Instance Functions
		std::shared_ptr<ID3D11PixelShader*> GetPixelShader(PixelShaderFormat f);
		std::shared_ptr<ID3D11VertexShader*> GetVertexShader(VertexShaderFormat f);
	
	};
}