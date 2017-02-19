// SoundEngine.cpp : Defines the exported functions for the DLL application.
//Again -> Credit to John Murphy

//#include "stdafx.h"
#include "SoundEngine.h"
#include <iostream>
#include <string>
#include <sstream>
#include "../Common/Math.h"
#include "../Objects/Component.h"
#include "../Objects/BaseObject.h"
using namespace Math;

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
	AkListenerPosition lPos;
	AkSoundPosition sPos;

	unsigned int i;
	// Update listener pos & orientation
	for (i = 0; i < mListeners.size(); ++i) {
		const vec4f * pos = mListeners[i]->GetTransform().GetPosition();

		lPos.SetPosition(pos->x, pos->y, pos->z);

		const vec4f * zAxis = mListeners[i]->GetTransform().GetZAxis();
		const vec4f * yAxis = mListeners[i]->GetTransform().GetYAxis();

		lPos.SetOrientation(zAxis->x, zAxis->y, zAxis->z, yAxis->x, yAxis->y, yAxis->z);

		SoundEngine::SetListenerPosition(lPos, i);
	}
	// Update emitter pos & orientation
	for (i = 0; i < mEmitters.size(); ++i) {
		const vec4f * pos = mEmitters[i]->GetTransform().GetPosition();

		sPos.SetPosition(pos->x, pos->y, pos->z);

		const vec4f * zAxis = mEmitters[i]->GetTransform().GetZAxis();
		const vec4f * yAxis = mEmitters[i]->GetTransform().GetYAxis();

		sPos.SetOrientation(zAxis->x, zAxis->y, zAxis->z, yAxis->x, yAxis->y, yAxis->z);

		SoundEngine::SetPosition((AkGameObjectID)mEmitters[i], sPos);
	}
	// Render Audio
	SOUNDENGINE_DLL::Tick();
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
	// If no name give pos in vector as name
	if (!_name)
	{
		std::stringstream ssNum;
		ssNum << (mListeners.size() + 1);
		std::string strTemp = "listener" + ssNum.str();

		//Still should probably added additional error checking?
		if (SoundEngine::RegisterGameObj((AkGameObjectID)_listener, strTemp.c_str()) != AK_Success) {
			return false;
		}
	}
	else
	{
		if (SoundEngine::RegisterGameObj((AkGameObjectID)_listener, _name) != AK_Success) {
			return false;
		}
	}

	SoundEngine::SetListenerScalingFactor((AkUInt32)mListeners.size(), mWorldScale);
	mListeners.push_back(_listener);

	return true;
}

bool AudioWrapper::RemoveListener(const Listener* _listener)
{
	for (unsigned int i = 0; i < mListeners.size(); ++i) {
		if (mListeners[i] == _listener) {
			mListeners.erase(mListeners.begin() + i);

			if (SoundEngine::UnregisterGameObj((AkGameObjectID)_listener) != AK_Success) {
				return false;
			}
			break;
		}
	}

	return true;
}

bool AudioWrapper::AddEmitter(const Emitter * _emitter, const char * _name)
{
	// If no name give pos in vector as name
	if (!_name)
	{
		std::stringstream ssNum;
		ssNum << (mListeners.size() + 1);
		std::string strTemp = "listener" + ssNum.str();

		//Still should probably added additional error checking?
		if (SoundEngine::RegisterGameObj((AkGameObjectID)_emitter, strTemp.c_str()) != AK_Success) {
			return false;
		}
	}
	else
	{
		if (SoundEngine::RegisterGameObj((AkGameObjectID)_emitter, _name) != AK_Success) {
			return false;
		}
	}

	mEmitters.push_back(_emitter);

	return true;
}

bool AudioWrapper::RemoveEmitter(const Emitter * _emitter)
{
	for (unsigned int i = 0; i < mEmitters.size(); ++i) {
		if (mEmitters[i] == _emitter) {
			mEmitters.erase(mEmitters.begin() + i);

			if (SoundEngine::UnregisterGameObj((AkGameObjectID)_emitter) != AK_Success) {
				return false;
			}
			break;
		}
	}

	return true;
}

//-------------EVENTS-----------------------------------------------
bool AudioWrapper::MakeEventAtLocation(AudioEvent _id, vec4f * _pos)
{
	static long dummyID = -1;
	++dummyID;

	AkSoundPosition sPos;
	sPos.SetOrientation(0, 0, 1.0f, 0, 0, 1.0f);
	sPos.SetPosition(_pos->x, _pos->y, _pos->z);

	SoundEngine::SetPosition(dummyID, sPos);
	if (SoundEngine::PostEvent((AkUniqueID)_id, dummyID) == AK_INVALID_PLAYING_ID)
		return false;

	return true;
}

bool AudioWrapper::MakeEvent(AudioEvent _id, const Emitter * _emitter)
{
	//Check to see if it exists???

	if (SoundEngine::PostEvent((AkUniqueID)_id, (AkGameObjectID)_emitter) == AK_INVALID_PLAYING_ID)
		return false;

	return true;
}

bool AudioWrapper::MakeEventAtListener(AudioEvent _id, unsigned int _listenerID)
{
	if ((mListeners.size() - _listenerID) < 0)
		return false;

	if (SoundEngine::PostEvent((AkUniqueID)_id, (AkGameObjectID)mListeners[_listenerID]) == AK_INVALID_PLAYING_ID)
		return false;

	return true;
}
//------------Soundbank-Stuff---------------------------------------------

void AudioWrapper::SetBasePath(const wchar_t* _strPath)
{
	SOUNDENGINE_DLL::SetBasePath(_strPath);
}

bool AudioWrapper::LoadSoundBank(const wchar_t* _BankName)
{
	AkBankID newID;
	if (SoundEngine::LoadBank(_BankName, AK_DEFAULT_POOL_ID, newID) == AK_Success)
		mRegisteredSoundBanks[_BankName] = newID;
	else
		return false;

	return true;
}

bool AudioWrapper::UnloadSoundBank(const wchar_t* _BankName)
{
	if (mRegisteredSoundBanks.find(_BankName) == mRegisteredSoundBanks.end())
		return false;

	if (SoundEngine::UnloadBank(mRegisteredSoundBanks[_BankName], nullptr, nullptr) != AK_Success)
		return false;

	mRegisteredSoundBanks.erase(_BankName);

	return true;
}