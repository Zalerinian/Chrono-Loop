#pragma once
#include <string>
#include <unordered_map>
#include <thread>
#include "../Common/Logger.h"


class CommandConsole;

typedef void (*Command)(void* self, std::wstring);//function pointer

class CommandConsole
{
	
private:
	bool mTakeInput; //On/Off switch

	static CommandConsole* sInstance;
	std::wstring mCurCommand; //A string to hold the thing that's being typed to the console
	std::wstring mCurDisplay; //A string to display to the console
	std::unordered_map<std::wstring, Command> mCommandLines; // Holds all of the Commands and their function pointers
	std::thread mInputThread; 

	//Function Specific
	bool isFPSon; //Toggle for FPS Counter
	int mFps; //FPS display
	float mFrameTime; 
	
	//SystemLogger mLogger;
public:
	CommandConsole();
	~CommandConsole() {};
	static CommandConsole& Instance();
	static void DestroyInstance();

	bool willTakeInput() { return mTakeInput; };
	std::wstring GetCurrentCommmand() { return mCurCommand; };


	void Update();
	//Command String Editing
	void AddCharacter(wchar_t _letter) { mCurCommand = mCurCommand + _letter; };
	void RemoveCharacter() { mCurCommand = mCurCommand.substr(0, mCurCommand.length() - 1); }
	//Command List Editing
	void ClearCommand() { mCurCommand.clear(); }
	bool CheckCommand(std::wstring _commandName, std::wstring _item = nullptr);
	void AddCommand(std::wstring _commandName, Command _functionPtr);

	//Display To Console
	void Display();
	void DisplaySet(std::wstring _display) { mCurDisplay = _display; }


	

	//Function Pointer
	static void Help(void* _self, std::wstring _nothing);
	static void ToggleFPS(void* _self, std::wstring _ifOn);
	void Toggle();

	void InputFunction();



};