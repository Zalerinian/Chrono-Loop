// SoundEngine.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "SoundEngine.h"

using namespace AK;

AudioWrapper * AudioWrapper::audioSystem = nullptr;

AudioWrapper::AudioWrapper()
{

}

AudioWrapper::~AudioWrapper()
{

}
//-------Initialize-Shutdown-Update------------------------------------
bool AudioWrapper::Initialize()
{




	return true;
}

void AudioWrapper::Shutdown()
{

}

void AudioWrapper::Update()
{

}
//----------------------------------------------------
void AudioWrapper::SetWorldScale(float _scale)
{

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