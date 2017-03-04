#pragma once
#include <wrl/client.h>

struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
struct ID3D11Texture3D;

namespace Epoch
{
	namespace FileIO
	{

		bool LoadTexture2D(const char *_path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* _srv, Microsoft::WRL::ComPtr<ID3D11Texture2D>* _texture);
		//bool LoadTexture3D(const char *_path, std::shared_ptr<ID3D11ShaderResourceView*>* _srv, std::shared_ptr<ID3D11Texture3D*>* _texture);

	}
}