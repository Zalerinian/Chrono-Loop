#include "Common.h"

namespace Epoch
{

	void MakeWide(const char* _in, wchar_t **_out, unsigned int _size)
	{
		*_out = new wchar_t[_size + 1];
		mbstowcs_s(nullptr, *_out, _size + 1, _in, _size);
	}

	void MakeNarrow(const wchar_t *_in, char **_out, unsigned int _size)
	{
		*_out = new char[_size + 1];
		wcstombs_s(nullptr, *_out, _size, _in, _size + 1);
	}
}