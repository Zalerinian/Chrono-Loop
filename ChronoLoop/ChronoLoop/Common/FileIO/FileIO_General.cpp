#include "FileIO_General.h"
#include <fstream>

namespace FileIO {

	bool LoadBytes(const char * _path, char **_bytes, int & _size) {
			std::ifstream load;
			load.open(_path, std::ios_base::binary);
			if (!load.is_open()) {
				*_bytes = nullptr;
				_size = 0;
				return false;
			}
			load.seekg(0, std::ios_base::end);
			_size = int(load.tellg());
			*_bytes = new char[_size];
			load.seekg(0, std::ios_base::beg);
			load.read(*_bytes, _size);
			load.close();
			return true;
	}
}
