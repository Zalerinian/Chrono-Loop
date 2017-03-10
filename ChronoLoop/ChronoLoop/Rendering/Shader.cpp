//#include "stdafx.h"
#include "Shader.h"
#include <fstream>

using namespace std;

namespace Epoch
{

	/*Shader::Shader() {}

	PixelShader::PixelShader(const char * _path)
	{
		this->path = _path;
		vector<byte> bytes;
		ifstream file(_path, ios::in | ios::binary);
		if (!file.is_open())
		{
			shader = nullptr;
			return;
		}
		unsigned int length = 0;
		file.seekg(0, std::ios_base::end);
		length = (unsigned int)file.tellg();
		file.seekg(0, std::ios_base::beg);
		bytecode.reserve(length);
		byte *bite = new byte;
		for (unsigned int i = 0; i < length; i++)
		{
			file.read((char*)bite, 1);
			bytecode.push_back(*bite);
		}
		delete bite;
	}*/

}