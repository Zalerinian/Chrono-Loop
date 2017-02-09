// SoundEngine.cpp : Defines the exported functions for the DLL application.
//Again -> Credit to John Murphy

#include "stdafx.h"
#include "SoundEngine.h"
#include <iostream>

#pragma comment(lib, "AkSoundEngineDLL.lib")
using namespace AK;

AudioWrapper * AudioWrapper::audioSystem = nullptr;

AudioWrapper::AudioWrapper() : mWorldScale(1.f), mIsInitialize(false)
{
	if (!audioSystem)
		audioSystem = this;
}

AudioWrapper::~AudioWrapper()
{

}
//-------Initialize-Shutdown-Update------------------------------------
bool AudioWrapper::Initialize()
{
	AkMemSettings memorySettings;
	memorySettings.uMaxNumPools = 40;

	AkStreamMgrSettings streamSettings;
	StreamMgr::GetDefaultSettings(streamSettings);
	AkDeviceSettings deviceSettings;
	StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	AkInitSettings InitSettings;
	AkPlatformInitSettings platInitSetings;
	SoundEngine::GetDefaultInitSettings(InitSettings);
	SoundEngine::GetDefaultPlatformInitSettings(platInitSetings);

	//TODO: Set pool sizes

	AkMusicSettings musicInitialize;
	MusicEngine::GetDefaultInitSettings(musicInitialize);
	musicInitialize.fStreamingLookAheadRatio = 100;

	AKRESULT eResult = SOUNDENGINE_DLL::Init(&memorySettings, &streamSettings, &deviceSettings, &InitSettings, &platInitSetings, &musicInitialize);

	//More error checking?
	/*switch (eResult) {}*/

	if (eResult != AK_Success)
	{
		std::cout << "Initializing sound failed. Game will now run without sound" << std::endl;
		SOUNDENGINE_DLL::Term();
		return false;
	}

	mIsInitialize = true;
	return true;
}

void AudioWrapper::Shutdown()
{
	//Un-register everything and terminate
	SoundEngine::UnregisterAllGameObj();
	SOUNDENGINE_DLL::Term();
	mIsInitialize = false;
}

void AudioWrapper::Update()
{

}
//----------------------------------------------------
void AudioWrapper::SetWorldScale(float _scale)
{
	mWorldScale = _scale;
	for (unsigned int index = 0; index < mListeners.size(); index++)
		SoundEngine::SetListenerScalingFactor(index, mWorldScale);

}

//------------Add/Remove-Listeners/Emitters-----------------------------------------
bool AudioWrapper::AddListener(const Listener* _listener, const char * _name)
{


	return true;
}

bool AudioWrapper::RemoveListener(const Listener* _listener)
{


	return true;
}

bool AudioWrapper::AddEmitter(const Emitter * _emitter, const char * _name)
{


	return true;
}

bool AudioWrapper::RemoveEmitter(const Emitter * _emitter)
{


	return true;
}

//-------------EVENTS-----------------------------------------------
bool AudioWrapper::MakeEvent(AudioEvent _id, float * _pos)
{


	return true;
}

bool AudioWrapper::MakeEvent(AudioEvent _id, const Emitter * _emitter)
{

	return true;
}

bool AudioWrapper::MakeEvent(AudioEvent _id, unsigned int _listenerID)
{


	return true;
}
//------------Soundbank-Stuff---------------------------------------------

void AudioWrapper::SetBasePath(const wchar_t* _strPath)
{

}

bool AudioWrapper::LoadSoundBank(const wchar_t* _BankName)
{


	return true;
}

bool AudioWrapper::UnloadSoundBank(const wchar_t* _BankName)
{


	return true;
}