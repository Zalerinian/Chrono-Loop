#pragma once
#include <sstream>

namespace Engine {

	void MakeWide(const char* _in, wchar_t **_out, unsigned int _size);
	void MakeNarrow(const wchar_t *_in, char **_out, unsigned int _size);

}
