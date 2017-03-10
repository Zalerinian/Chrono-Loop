#pragma once
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>

struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
struct ID3D11Texture3D;

namespace Epoch {

	class TextureManager {
		static TextureManager* sInstance;
	
		std::unordered_map<std::string, std::pair<std::shared_ptr<ID3D11ShaderResourceView*>, std::shared_ptr<ID3D11Texture2D*>>> mTextureMap2D;
		std::unordered_map<std::string, std::pair<std::shared_ptr<ID3D11ShaderResourceView*>, std::shared_ptr<ID3D11Texture3D*>>> mTextureMap3D;
		std::unordered_set<std::string> mLoading;
	
		std::mutex mAsyncMutex;

		TextureManager();
		~TextureManager();
	
	public:
		enum class TextureStatus {
			eSuccess = 0,
			eLoading,
			eError
		};
	
		static TextureManager* Instance();
		static void DestroyInstance();
	
		TextureStatus iAddTexture2D(std::string& _name, ID3D11Texture2D *_tex, std::shared_ptr<ID3D11ShaderResourceView*>* _srv);
		TextureStatus iGetTexture2D(const char* _path, std::shared_ptr<ID3D11ShaderResourceView*>* _srv, std::shared_ptr<ID3D11Texture2D*>* _texture);
		//TextureStatus iGetTexture2DAsync(const char* _path, ID3D11ShaderResourceView** _srv, ID3D11Texture2D** _texture);
		//TextureStatus iGetTexture3D(const char* _path, ID3D11ShaderResourceView** _srv, ID3D11Texture3D** _texture);
		//TextureStatus iGetTexture3DAsync(const char* _path, ID3D11ShaderResourceView** _srv, ID3D11Texture3D** _texture);
	};

}