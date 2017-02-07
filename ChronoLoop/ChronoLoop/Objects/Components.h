#pragma once

#define DLL __declspec(dllexport)
extern "C" 
{
	//Base Component
	class Component
	{

	};

	//Audio Listener
	class Listener : Component
	{

	};

	//Audio Emitter
	class Emitter : Component
	{

	};

	DLL void WHY()
	{

	}
}