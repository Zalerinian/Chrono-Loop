#pragma once
//Credit to John Murphy
#include <vector>
#include <map>

#include "../ObjectEngine/Components.h"
#include "AkSoundEngineDLL.h"

#define DLL __declspec(dllexport)

//Needed to work with Wwise soundbanks and the .h they generate
#if defined(_WIN64)
typedef unsigned __int64 AudioEvent;				///< Integer (unsigned) type for pointers
#else
typedef __w64 unsigned int AudioEvent;			///< Integer (unsigned) type for pointers
#endif

class AudioWrapper
{
	static AudioWrapper* audioSystem;

private:
	std::vector<Emitter> emitters;
	std::vector<Listener> listeners;
	std::map<std::wstring, AkBankID> RegisteredSoundBanks;

	bool isInitialize = false;
	float worldScale;
public:
	DLL AudioWrapper();
	DLL ~AudioWrapper();

	DLL bool initialize();
	DLL void shutdown();
	DLL void update();

	DLL void setWorldScale(float _scale);
	DLL bool isInitialized() { return isInitialize; }

	DLL bool addListener(const Listener * _listener, const char* _name);
	DLL bool removeListener(const Listener * _listener);
	DLL bool addEmitter(const Emitter * _emitter, const char* _name);
	DLL bool removeEmitter(const Emitter * _emitter);

	//Posts an event at a pos, emitter location, or near a listener.
	DLL bool makeEvent(AudioEvent _id, float* _pos);
	DLL bool makeEvent(AudioEvent _id, const Emitter * _emitter);
	DLL bool makeEvent(AudioEvent _id, unsigned int _listenerID = 0);

	//Set soundbank path(s)
	DLL void setBasePath(const wchar_t* _strPath);
	DLL bool loadSoundBank(const wchar_t* _BankName);
	DLL bool unloadSoundBank(const wchar_t* _BankName);

};
