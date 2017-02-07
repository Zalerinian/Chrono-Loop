// SoundEngine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
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
bool AudioWrapper::initialize()
{

}

void AudioWrapper::shutdown()
{

}

void AudioWrapper::update()
{

}
//----------------------------------------------------
void AudioWrapper::setWorldScale(float _scale)
{

}

//------------Add/Remove-Listeners/Emitters-----------------------------------------
bool AudioWrapper::addListener(const Listener* _listener, const char * _name)
{

}

bool AudioWrapper::removeListener(const Listener* _listener)
{

}

bool AudioWrapper::addEmitter(const Emitter * _emitter, const char * _name)
{

}

bool AudioWrapper::removeEmitter(const Emitter * _emitter)
{

}

//-------------EVENTS-----------------------------------------------
bool AudioWrapper::makeEvent(AudioEvent _id, float * _pos)
{

}

bool AudioWrapper::makeEvent(AudioEvent _id, const Emitter * _emitter)
{

}

bool AudioWrapper::makeEvent(AudioEvent _id, unsigned int _listenerID)
{

}
//------------Soundbank-Stuff---------------------------------------------

void AudioWrapper::setBasePath(const wchar_t* _strPath)
{

}

bool AudioWrapper::loadSoundBank(const wchar_t* _BankName)
{

}

bool AudioWrapper::unloadSoundBank(const wchar_t* _BankName)
{

}