//#include "stdafx.h"
#include "KeyboardInput.h"

namespace Epoch
{

	KeyboardInput* KeyboardInput::sInstance = nullptr;

	KeyboardInput::KeyboardInput() {}

	KeyboardInput::~KeyboardInput() {}

	KeyboardInput & KeyboardInput::Instance()
	{
		if (!sInstance)
			sInstance = new KeyboardInput();
		return *sInstance;
	}
	void KeyboardInput::DestroyInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}
	void KeyboardInput::CheckKeyboardButtonPress()
	{
		if (GetAsyncKeyState(Enter) & 0x1)
		{

		}
		if (GetAsyncKeyState(Backspace) & 0x1)
		{
			if (CommandConsole::Instance().willTakeInput() && CommandConsole::Instance().GetCurrentCommmand().length() > 0)
				CommandConsole::Instance().RemoveCharacter();
		}
		if (GetAsyncKeyState(Tab) & 0x1)
		{

		}
		if (GetAsyncKeyState(Shift) & 0x1)
		{

		}
		if (GetAsyncKeyState(Space) & 0x1)
		{
			if (CommandConsole::Instance().willTakeInput()) { CommandConsole::Instance().AddCharacter(' '); }
		}
		for (int x = 0; x < 9; ++x)
		{

			if (GetAsyncKeyState(0x30 + x) & 0x1)
			{

			}
			if (GetAsyncKeyState(0x60 + x) & 0x1)
			{

			}
		}
		if (GetAsyncKeyState(Multiply) & 0x1)
		{

		}
		if (GetAsyncKeyState(Add) & 0x1)
		{

		}
		if (GetAsyncKeyState(Subtract) & 0x1)
		{

		}

		if (GetAsyncKeyState(Tilde) & 0x1)
		{
			CommandConsole::Instance().Toggle();
		}
#pragma region ALPHABET
		if (CommandConsole::Instance().willTakeInput() && CommandConsole::Instance().GetCurrentCommmand().length() < 22)
		{
			if (GetAsyncKeyState(A) & 0x1) { CommandConsole::Instance().AddCharacter('A'); }
			if (GetAsyncKeyState(B) & 0x1) { CommandConsole::Instance().AddCharacter('B'); }
			if (GetAsyncKeyState(C) & 0x1) { CommandConsole::Instance().AddCharacter('C'); }
			if (GetAsyncKeyState(D) & 0x1) { CommandConsole::Instance().AddCharacter('D'); }
			if (GetAsyncKeyState(E) & 0x1) { CommandConsole::Instance().AddCharacter('E'); }
			if (GetAsyncKeyState(F) & 0x1) { CommandConsole::Instance().AddCharacter('F'); }
			if (GetAsyncKeyState(G) & 0x1) { CommandConsole::Instance().AddCharacter('G'); }
			if (GetAsyncKeyState(H) & 0x1) { CommandConsole::Instance().AddCharacter('H'); }
			if (GetAsyncKeyState(I) & 0x1) { CommandConsole::Instance().AddCharacter('I'); }
			if (GetAsyncKeyState(J) & 0x1) { CommandConsole::Instance().AddCharacter('J'); }
			if (GetAsyncKeyState(K) & 0x1) { CommandConsole::Instance().AddCharacter('K'); }
			if (GetAsyncKeyState(L) & 0x1) { CommandConsole::Instance().AddCharacter('L'); }
			if (GetAsyncKeyState(M) & 0x1) { CommandConsole::Instance().AddCharacter('M'); }
			if (GetAsyncKeyState(N) & 0x1) { CommandConsole::Instance().AddCharacter('N'); }
			if (GetAsyncKeyState(O) & 0x1) { CommandConsole::Instance().AddCharacter('O'); }
			if (GetAsyncKeyState(P) & 0x1) { CommandConsole::Instance().AddCharacter('P'); }
			if (GetAsyncKeyState(Q) & 0x1) { CommandConsole::Instance().AddCharacter('Q'); }
			if (GetAsyncKeyState(R) & 0x1) { CommandConsole::Instance().AddCharacter('R'); }
			if (GetAsyncKeyState(S) & 0x1) { CommandConsole::Instance().AddCharacter('S'); }
			if (GetAsyncKeyState(T) & 0x1) { CommandConsole::Instance().AddCharacter('T'); }
			if (GetAsyncKeyState(U) & 0x1) { CommandConsole::Instance().AddCharacter('U'); }
			if (GetAsyncKeyState(V) & 0x1) { CommandConsole::Instance().AddCharacter('V'); }
			if (GetAsyncKeyState(W) & 0x1) { CommandConsole::Instance().AddCharacter('W'); }
			if (GetAsyncKeyState(X) & 0x1) { CommandConsole::Instance().AddCharacter('X'); }
			if (GetAsyncKeyState(Y) & 0x1) { CommandConsole::Instance().AddCharacter('Y'); }
			if (GetAsyncKeyState(Z) & 0x1) { CommandConsole::Instance().AddCharacter('Z'); }
			if (GetAsyncKeyState(Slash) & 0x1) { CommandConsole::Instance().AddCharacter('/'); }
		}
#pragma endregion

	}

}