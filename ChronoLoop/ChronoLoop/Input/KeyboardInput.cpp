#include "stdafx.h"
#include "KeyboardInput.h"

void KeyboardInput::CheckKeyboardButtonPress()
{
	if (GetAsyncKeyState(Enter)) {

	}
	if (GetAsyncKeyState(Backspace)) {

	}
	if (GetAsyncKeyState(Tab)) {

	}
	if (GetAsyncKeyState(Shift)) {

	}
	if (GetAsyncKeyState(Space)) {

	}
	for (int x = 0; x < 9; ++x)
	{

		if (GetAsyncKeyState(0x30 + x)) {

		}
		if (GetAsyncKeyState(0x60 + x)) {

		}
	}
	if (GetAsyncKeyState(Multiply)) {

	}
	if (GetAsyncKeyState(Add)) {

	}
	if (GetAsyncKeyState(Subtract)) {

	}
	if (GetAsyncKeyState(Slash)) {

	}
#pragma region ALPHABET
	if (GetAsyncKeyState(A)) {}
	if (GetAsyncKeyState(B)) {}
	if (GetAsyncKeyState(C)) {}
	if (GetAsyncKeyState(D)) {}
	if (GetAsyncKeyState(E)) {}
	if (GetAsyncKeyState(F)) {}
	if (GetAsyncKeyState(G)) {}
	if (GetAsyncKeyState(H)) {}
	if (GetAsyncKeyState(I)) {}
	if (GetAsyncKeyState(J)) {}
	if (GetAsyncKeyState(K)) {}
	if (GetAsyncKeyState(L)) {}
	if (GetAsyncKeyState(M)) {}
	if (GetAsyncKeyState(N)) {}
	if (GetAsyncKeyState(O)) {}
	if (GetAsyncKeyState(P)) {}
	if (GetAsyncKeyState(Q)) {}
	if (GetAsyncKeyState(R)) {}
	if (GetAsyncKeyState(S)) {}
	if (GetAsyncKeyState(T)) {}
	if (GetAsyncKeyState(U)) {}
	if (GetAsyncKeyState(V)) {}
	if (GetAsyncKeyState(W)) {}
	if (GetAsyncKeyState(X)) {}
	if (GetAsyncKeyState(Y)) {}
	if (GetAsyncKeyState(Z)) {}
#pragma endregion

}