// Messager.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "Messager.h"
#include "Message.h"
//#include "..\SoundEngine\SoundEngine.h"
#include <thread>

Messager::Messager()
{
	std::thread thrd (&Messager::Process, this);
	thrd.detach();
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

template<class... Args>
void Messager::ProcessMessage(Message<Args...> _msg)
{

}

void Messager::Process()
{
	//While alive, process messages in the queue
}