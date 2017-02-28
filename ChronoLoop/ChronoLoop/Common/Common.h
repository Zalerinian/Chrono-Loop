#pragma once
#include <sstream>

#if defined(DEBUG) | defined(_DEBUG)
#ifndef SetD3DName
#define SetD3DName(object, name) \
	{ \
		if(name) \
			object->SetPrivateData(WKPDID_D3DDebugObjectName, \
				(UINT)strlen( name ), name ); \
	}
#endif
#else
#ifndef SetD3DName
#define SetD3DName(object, name) 
#endif
#endif 

namespace Engine {

	void MakeWide(const char* _in, wchar_t **_out, unsigned int _size);
	void MakeNarrow(const wchar_t *_in, char **_out, unsigned int _size);

}
