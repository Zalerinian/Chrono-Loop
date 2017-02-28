#include "stdafx.h"
#include "CommandConsole.h"
#include "../Input/KeyboardInput.h"
#include "../Rendering/Draw2D.h"
#include "../Core/TimeManager.h"
#include <iostream>

CommandConsole* CommandConsole::sInstance = nullptr;
CommandConsole & CommandConsole::Instance()
{
	if (!sInstance)
	{
		sInstance = new CommandConsole();
		sInstance->AddCommand(L"/HELP", sInstance->Help);
		sInstance->AddCommand(L"/FPS", sInstance->ToggleFPS);
	}
	return *sInstance;
}
void CommandConsole::DestroyInstance()
{
	if (sInstance != nullptr)
		delete sInstance;
	sInstance = nullptr;
}
CommandConsole::CommandConsole()
{
	mTakeInput = false;
	isFPSon = false;
	mFps = 0;
	mFrameTime = 0.0f;
	mInputThread = std::thread(&CommandConsole::InputFunction, this);
	//mInputThread.join();
}
void CommandConsole::Update()
{
	KeyboardInput::Instance().CheckKeyboardButtonPress();
	if (willTakeInput())
	{
		Draw::Instance().DrawRectangleToBitmap(0.0f,
			(*Draw::Instance().GetContext2D())->GetSize().height*(5.0f / 8.0f),
			(*Draw::Instance().GetContext2D())->GetSize().width*(3.0f / 8.0f),
			(*Draw::Instance().GetContext2D())->GetSize().height,
			D2D1::ColorF(D2D1::ColorF::Black, 0.9f),
			*(Draw::Instance().GetScreenBitmap().get())
		);
		Draw::Instance().DrawRectangleToBitmap(0.0f,
			(*Draw::Instance().GetContext2D())->GetSize().height*(31.0f / 32.0f),
			(*Draw::Instance().GetContext2D())->GetSize().width*(48.1f / 128.0f),
			(*Draw::Instance().GetContext2D())->GetSize().height,
			D2D1::ColorF(D2D1::ColorF::Black, 9.6f),
			*(Draw::Instance().GetScreenBitmap().get())
		);
		Font* tempFont = new Font(L"Times New Roman", 15, (D2D1::ColorF(D2D1::ColorF::White, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		Draw::Instance().DrawTextToBitmap(
			0.0f,
			(*Draw::Instance().GetContext2D())->GetSize().height*(31.0f / 32.0f),
			(*Draw::Instance().GetContext2D())->GetSize().width*(3.0f / 8.0f),
			(*Draw::Instance().GetContext2D())->GetSize().height,
			*tempFont, mCurCommand,
			*(Draw::Instance().GetScreenBitmap()).get()
		);
		if (sInstance->isFPSon)
		{
			float _deltaTime = TimeManager::Instance()->GetDeltaTime();
			sInstance->mFrameTime += _deltaTime;
			if (sInstance->mFrameTime > .5f) {
				sInstance->mFps = (int)(1000.0f / (_deltaTime * 1000));
				sInstance->mFrameTime = 0;
			}
			std::wstring FPS = L"FPS: " + std::to_wstring(sInstance->mFps);
			Font* tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::White, 1.0f)), DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			Draw::Instance().DrawTextToBitmap(
				(*Draw::Instance().GetContext2D())->GetSize().width*(7.0f / 8.0f),
				(*Draw::Instance().GetContext2D())->GetSize().height*(30.0f / 32.0f),
				(*Draw::Instance().GetContext2D())->GetSize().width,
				(*Draw::Instance().GetContext2D())->GetSize().height, *tempFont,
				FPS, *(Draw::Instance().GetScreenBitmap()).get());
		}
		Display();
	}
}
bool CommandConsole::CheckCommand(std::wstring _commandName,std::wstring _item)
{
	sInstance->ClearCommand();

	for (auto x : mCommandLines)
	{
		if (x.first == _commandName) {
			if (x.second != nullptr) {
				x.second(this, _item);
				return true;
			}
			else
			{
				sInstance->DisplaySet(L"FUNCTION POINTER NOT SET UP");
				return false;
			}
		}
		
	}
	sInstance->DisplaySet(L"THAT COMMAND DOES NOT EXIST\nPLEASE USE /HELP TO SEE COMMANDS");
	return false;
}
void CommandConsole::AddCommand(std::wstring _commandName, Command _functionPtr)
{
	mCommandLines.insert({_commandName,_functionPtr});
}
void CommandConsole::Display()
{
	Font* tempFont = new Font(L"Times New Roman", 15, (D2D1::ColorF(D2D1::ColorF::Red, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	Draw::Instance().DrawTextToBitmap(
		0.0f,
		(*Draw::Instance().GetContext2D())->GetSize().height*(5.0f / 8.0f),
		(*Draw::Instance().GetContext2D())->GetSize().width*(3.0f / 8.0f),
		(*Draw::Instance().GetContext2D())->GetSize().height*(31.0f / 32.0f),
		*tempFont, mCurDisplay,
		*(Draw::Instance().GetScreenBitmap()).get()
	);
}
void CommandConsole::Help(void* _self, std::wstring _nothing)
{
	CommandConsole* self = (CommandConsole*)_self;
	std::list<std::wstring> temp;
	for (auto x : self->mCommandLines)
		temp.push_back(x.first);

	temp.sort();

	//**FOR DISPLAYING**//
	int cnt = 0;
	std::wstring tempDisplay;
	if (temp.size() == 1)
		tempDisplay = L"CURRENTLY NO OTHER COMMANDS RIGHT NOW";
	else
	{
		std::cout << "\n";
		for (auto iter = temp.begin(); iter != temp.end(); iter++)
		{
			if (cnt % 2 == 0) {
				tempDisplay += (*iter) + L"\t\t";
			}
			else {
				tempDisplay += (*iter) + L"\n";
			}
			cnt++;
		}
	}
	sInstance->DisplaySet(tempDisplay);
}
void CommandConsole::ToggleFPS(void * _self, std::wstring _ifOn)
{
	CommandConsole* self = (CommandConsole*)_self;
	if (_ifOn == L"ON") {
		sInstance->isFPSon = true;
		sInstance->DisplaySet(L"");
	}
	else if (_ifOn == L"OFF") {
		sInstance->isFPSon = false;
		sInstance->DisplaySet(L"");
	}
	else{
		sInstance->DisplaySet(L"INVALID INPUT:" + _ifOn + L"\nCORRECT INPUT: /FPS (ON/OFF)");
	}
}
void CommandConsole::Toggle()
{
	if (mTakeInput)
		mTakeInput = false;
	else
		mTakeInput = true;
}
void CommandConsole::InputFunction()
{
	while (true)
	{
		if (mTakeInput && GetAsyncKeyState(VK_RETURN) & 0x1) {

			if (mCurCommand.length() == 0)
				continue;

			std::wstring command = mCurCommand.substr(0, mCurCommand.find(L" "));
			std::wstring item = mCurCommand.substr(mCurCommand.find(L" ") + 1, mCurCommand.length() - 1);
			bool isCalled = CheckCommand(command, item);
			//Toggle();
		}
	}
}

