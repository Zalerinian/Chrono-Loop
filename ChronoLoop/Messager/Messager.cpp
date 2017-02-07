// Messager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Messager.h"
#include "Message.h"
#include "..\SoundEngine\SoundEngine.h"
#include <thread>


Messager::Messager()
{
	std::thread thrd (&Process);
}
Messager::~Messager()
{

}

void Messager::SendInMessage()
{

}

void Messager::SendOutMessage()
{

}

void Messager::Process()
{

}

void Messager::ProcessMessage(Message _msg)
{

}