#pragma once
#include <Windows.h>
#include "../Input/CommandConsole.h"

namespace Epoch
{

	enum Keys
	{
		Backspace = 0x08,
		Tab = 0x09,
		Enter = 0x0D,
		Shift = 0x10,
		Space = 0x20,
		_0 = 0x30,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Numpad0 = 0x60,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Multiply,
		Add,
		Subtract = 0x6D,
		Slash = 0xBF,
		Tilde = 0xC0
	};
	//static const Keys ArrayOfKeys[] = { 
	//Backspace,Tab,Enter,Shift,Space,//Shit???
	//_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,//Top Numbers
	//A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,//A-Z
	//Numpad0,Numpad1,Numpad2,Numpad3,Numpad4,Numpad5,Numpad6,Numpad7,Numpad8,Numpad9,//Number Pad
	//Multiply,Add,Subtract,//Math Symbols
	//Slash };
	class KeyboardInput
	{
	private:
		static KeyboardInput* sInstance;
	public:
		KeyboardInput();
		~KeyboardInput();
		static KeyboardInput& Instance();
		static void DestroyInstance();
		void CheckKeyboardButtonPress();
		void OnKeyDown(WPARAM _wp, LPARAM _lp);
	};

}